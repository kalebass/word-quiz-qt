#include "quizwindow.h"
#include "ui_quizwindow.h"
#include <QAction>
#include <QDirIterator>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include "quizmodel.h"
#include "optionsdialog.h"

QFont QuizWindow::songti{ QString{ "SimSun" }, 18 };
QFont QuizWindow::heiti{ QString{ "Microsoft YaHei" }, 16 };
QFont QuizWindow::fangSongti{ QString{ "FangSong" }, 20 };
QFont QuizWindow::kaiti{ QString{ "KaiTi" }, 22 };
const QDir QuizWindow::dictionaryDir{ "dict/", "*.txt" };

QuizWindow::QuizWindow(QWidget* parent) :
	QMainWindow{ parent },
	ui{ new Ui::MainWindow },
	quiz_{}
{
	ui->setupUi(this);
	quizModel = new QuizModel{ quiz_, this };
	optionsDialog = new OptionsDialog{ this };
	QSettings::setDefaultFormat(QSettings::IniFormat);

	ui->answerEdit->setFont(songti);
	quizModel->setChineseFont(songti);
	beginQuiz();

	auto showOptionsAction{ new QAction{ "Options", this } };
	ui->toolBar->addAction(showOptionsAction);
	connect(ui->nextButton, &QAbstractButton::clicked, this, &QuizWindow::displayQuestion);
	connect(showOptionsAction, &QAction::triggered, optionsDialog, &QWidget::show);
	connect(showOptionsAction, &QAction::triggered, optionsDialog, &OptionsDialog::readDictionaryList);
	connect(optionsDialog, &OptionsDialog::dictionarySettingChanged, this, &QuizWindow::beginQuiz);
	connect(ui->choiceTable, &QAbstractItemView::clicked, this, &QuizWindow::onAnswer);
	connect(ui->choiceTable, &QAbstractItemView::activated, this, &QuizWindow::onAnswer);
}

void QuizWindow::beginQuiz()
{
	ui->choiceTable->setModel(nullptr);
	loadDictionaryFiles();
	if (quiz_.wordCount() != 0) {
		quiz_.begin();
		ui->choiceTable->setModel(quizModel);
		displayQuestion();
	} else {
		ui->question->setText("[Dictionaries empty or not selected]");
		updateUi(false);
	}
}

void QuizWindow::loadDictionaryFiles()
{
	quiz_.clear();
	QSettings settings;
	settings.beginGroup("dictionaries");
	QDirIterator it{ dictionaryDir };
	while (it.hasNext()) {
		it.next();
		if (!settings.contains(it.fileName())) {
			settings.setValue(it.fileName(), true);
		}
		if (settings.value(it.fileName(), true).toBool()) {
			QFile file{ it.filePath() };
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			if (!file.isOpen()) {
				auto error{ tr("Couldn't open %1: %2").arg(it.filePath()).arg(file.errorString()) };
				QMessageBox::critical(this, QString{}, error);
				std::exit(-1);
			}
			QTextStream stream{ &file };
			quiz_.addWords(stream);
		}
	}
}

void QuizWindow::updateUi(bool showQuestion)
{
	auto enableNext{ !showQuestion && quiz_.currentIndex() < quiz_.wordCount() - 1 };
	ui->nextButton->setEnabled(enableNext);
	ui->answerEdit->setEnabled(showQuestion);
	ui->choiceTable->setEnabled(showQuestion);
	ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::Pinyin), showQuestion);
	ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::English), showQuestion);
	auto status{ tr("%1 of %2\n%3 correct")
			.arg(quiz_.currentIndex() + 1)
			.arg(quiz_.wordCount())
			.arg(quiz_.score()) };
	ui->statusDisplay->setText(status);
	if (ui->choiceTable->model()) {
		ui->choiceTable->resizeColumnsToContents();
	}
}

void QuizWindow::displayQuestion()
{
	quiz_.nextQuestion();
	updateUi(true);
	ui->choiceTable->selectionModel()->clear();
	ui->question->setText(quiz_.currentWord(Quiz::Language::English));
}

void QuizWindow::onAnswer(const QModelIndex& index)
{
	quizModel->makeAnswer(index);
	updateUi(false);
}

QuizWindow::~QuizWindow()
{
	delete ui;
}
