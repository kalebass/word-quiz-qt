#include "quizwindow.h"
#include "ui_quizwindow.h"
#include <QAction>
#include <QDirIterator>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include "quizmodel.h"
#include "optionsdialog.h"

const QDir QuizWindow::dictDir{ "dict/", "*.txt" };

QuizWindow::QuizWindow(QWidget* parent) :
    QMainWindow{ parent },
    ui{ new Ui::MainWindow },
    quiz_{}
{
    ui->setupUi(this);
    quizModel = new QuizModel{ quiz_, this };
    optionsDialog = new OptionsDialog{ this };
    QSettings::setDefaultFormat(QSettings::IniFormat);

    beginQuiz();
    optionsDialog->readSettings();
    applyFont();

    auto restartAction{ new QAction{ "Restart", this } };
    auto showOptionsAction{ new QAction{ "Options", this } };
    ui->toolBar->addAction(restartAction);
    ui->toolBar->addAction(showOptionsAction);

    connect(restartAction, &QAction::triggered, this, &QuizWindow::beginQuiz);
    connect(showOptionsAction, &QAction::triggered, optionsDialog, &QWidget::show);
    connect(ui->nextButton, &QAbstractButton::clicked, this, &QuizWindow::displayQuestion);
    connect(ui->choiceTable, &QAbstractItemView::clicked, this, &QuizWindow::onChoice);
    connect(ui->choiceTable, &QAbstractItemView::activated, this, &QuizWindow::onChoice);
    connect(ui->answerEdit, &QLineEdit::returnPressed, this, &QuizWindow::onAnswerEntered);
    connect(optionsDialog, &OptionsDialog::accepted, this, &QuizWindow::applyFont);
    connect(optionsDialog, &OptionsDialog::dictSettingChanged, this, &QuizWindow::beginQuiz);
    connect(optionsDialog, &OptionsDialog::modeSettingChanged, this, &QuizWindow::onModeChanged);
    connect(optionsDialog, &OptionsDialog::modeSettingChanged, quizModel, &QuizModel::changeQuizMode);
}

void QuizWindow::beginQuiz()
{
    ui->choiceTable->setModel(nullptr);
    loadDictFiles();
    QSettings settings;
    auto multipleChoice{ settings.value("multiple choice mode").toBool() };
    quiz_.setNumChoices(multipleChoice ? 7 : 0);
    if (quiz_.wordCount() != 0) {
        quiz_.begin();
        ui->choiceTable->setModel(quizModel);
        displayQuestion();
    } else {
        ui->question->setText("[Dictionaries empty or not selected]");
        updateUi(false);
    }
}

void QuizWindow::loadDictFiles()
{
    quiz_.clear();
    QSettings settings;
    settings.beginGroup("dict");
    QDirIterator it{ dictDir };
    while (it.hasNext()) {
        it.next();
        if (!settings.contains(it.fileName())) {
            settings.setValue(it.fileName(), true);
        }
        if (settings.value(it.fileName(), true).toBool()) {
            addDict(it.filePath());
        }
    }
}

void QuizWindow::addDict(const QString& path)
{
    QFile file{ path };
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()) {
        auto error{ tr("Couldn't open %1: %2").arg(path).arg(file.errorString()) };
        QMessageBox::critical(this, QString{}, error);
        std::exit(-1);
    }
    QTextStream stream{ &file };
    quiz_.addWords(stream);
}

void QuizWindow::updateUi(bool showQuestion)
{
    QSettings settings;
    auto enableNext{ !showQuestion && quiz_.currentIndex() < quiz_.wordCount() - 1 };
    ui->nextButton->setEnabled(enableNext);
    quizModel->showAnswer(!showQuestion);
    ui->answerEdit->setEnabled(showQuestion);
    ui->choiceTable->setEnabled(showQuestion);
    ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::Pinyin), showQuestion);
    ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::English), showQuestion);
    auto status{ tr("%1 of %2\n%3 correct")
            .arg(quiz_.currentIndex() + 1)
            .arg(quiz_.wordCount())
            .arg(quiz_.score()) };
    ui->statusDisplay->setText(status);
    ui->choiceTable->resizeColumnsToContents();
}

void QuizWindow::onModeChanged()
{
    QSettings settings;
    auto multipleChoice{ settings.value("multiple choice mode").toBool() };
    quiz_.setNumChoices(multipleChoice ? 7 : 0);
}

void QuizWindow::applyFont()
{
    auto font{ optionsDialog->chineseFont() };
    quizModel->setChineseFont(font);
    ui->answerEdit->setFont(font);
    ui->choiceTable->resizeColumnsToContents();
}

void QuizWindow::displayQuestion()
{
    quiz_.nextQuestion();
    updateUi(true);
    ui->choiceTable->selectionModel()->clear();
    ui->answerEdit->setFocus();
    ui->answerEdit->clear();
    ui->question->setText(quiz_.currentWord(Quiz::Language::English));
}

void QuizWindow::onChoice(const QModelIndex& index)
{
    auto answer{ quizModel->data(index).toString() };
    quiz_.makeAnswer(answer);
    updateUi(false);
}

void QuizWindow::onAnswerEntered()
{
    quiz_.makeAnswer(ui->answerEdit->text());
    updateUi(false);
}

QuizWindow::~QuizWindow()
{
    delete ui;
}
