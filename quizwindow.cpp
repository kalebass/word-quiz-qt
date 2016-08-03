#include "quizwindow.h"
#include "ui_quizwindow.h"
#include <QAction>
#include <QDirIterator>
#include <QMessageBox>
#include <QTextStream>
#include "quizmodel.h"
#include "optionsdialog.h"

QFont QuizWindow::songti{ QString{ "SimSun" }, 18 };
QFont QuizWindow::heiti{ QString{ "Microsoft YaHei" }, 16 };
QFont QuizWindow::fangSongti{ QString{ "FangSong" }, 20 };
QFont QuizWindow::kaiti{ QString{ "KaiTi" }, 22 };

QuizWindow::QuizWindow(QWidget* parent) :
	QMainWindow{ parent },
	ui{ new Ui::MainWindow },
	quiz_{},
	settings_{}
{
	ui->setupUi(this);
	quizModel = new QuizModel{ quiz_, this };
	optionsDialog = new OptionsDialog{ settings_, this };

	ui->answerEdit->setFont(songti);
	quizModel->setChineseFont(songti);
	beginQuiz();

	auto showOptionsAction{ new QAction{ "Options", this } };
	ui->toolBar->addAction(showOptionsAction);
	connect(ui->nextButton, &QAbstractButton::clicked, this, &QuizWindow::displayQuestion);
	connect(showOptionsAction, &QAction::triggered, optionsDialog, &QWidget::show);
	connect(showOptionsAction, &QAction::triggered, optionsDialog, &OptionsDialog::readDictList);
	connect(optionsDialog, &OptionsDialog::dictSettingChanged, this, &QuizWindow::beginQuiz);
	connect(ui->choiceTable, &QAbstractItemView::clicked, this, &QuizWindow::onChoice);
	connect(ui->choiceTable, &QAbstractItemView::activated, this, &QuizWindow::onChoice);
	connect(ui->answerEdit, &QLineEdit::returnPressed, this,&QuizWindow::onAnswerEntered);
}

void QuizWindow::beginQuiz()
{
	ui->choiceTable->setModel(nullptr);
	loadDictFiles();
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
	auto dicts{ settings_.dicts() };
	for (auto it{ dicts.constBegin() }; it != dicts.constEnd(); ++it) {
		auto isDictEnabled{ it.value() };
		if (isDictEnabled) {
			auto filePath{ QString{ "dict/%1" }.arg(it.key()) };
			QFile file{ filePath };
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			if (!file.isOpen()) {
				auto error{ tr("Couldn't open %1: %2").arg(filePath).arg(file.errorString()) };
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
	ui->choiceTable->resizeColumnsToContents();
}

void QuizWindow::displayQuestion()
{
	quiz_.nextQuestion();
	updateUi(true);
	ui->choiceTable->selectionModel()->clear();
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
