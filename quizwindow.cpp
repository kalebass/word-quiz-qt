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
	auto status{ tr("%1 of %2\n%3 correct")
		.arg(quiz_.currentIndex() + 1)
		.arg(quiz_.wordCount())
		.arg(quiz_.score()) };
	ui->statusDisplay->setText(status);

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

void QuizWindow::displayQuestion()
{
	ui->nextButton->setEnabled(false);
	ui->choiceTable->setEnabled(true);
	ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::Pinyin), true);
	ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::English), true);
	ui->choiceTable->selectionModel()->clear();
	quiz_.nextQuestion();
	ui->question->setText(quiz_.currentWord(Quiz::Language::English));
	ui->choiceTable->resizeColumnsToContents();
}

void QuizWindow::onAnswer(const QModelIndex& index)
{
	quizModel->makeAnswer(index);
	auto status{ tr("%1 of %2\n%3 correct")
			.arg(quiz_.currentIndex() + 1)
			.arg(quiz_.wordCount())
			.arg(quiz_.score()) };
	ui->statusDisplay->setText(status);
	ui->choiceTable->setEnabled(false);
	ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::Pinyin), false);
	ui->choiceTable->setColumnHidden(static_cast<int>(Quiz::Language::English), false);
	ui->choiceTable->resizeColumnsToContents();
	if (quiz_.currentIndex() + 1 < quiz_.wordCount()) {
		ui->nextButton->setEnabled(true);
	}
}

QuizWindow::~QuizWindow()
{
	delete ui;
}
