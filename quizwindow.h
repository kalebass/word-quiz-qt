#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFont>
#include <QMainWindow>
#include "quiz.h"

class QDir;
class QuizModel;
class OptionsDialog;

namespace Ui {
class MainWindow;
}

class QuizWindow : public QMainWindow
{
public:
	explicit QuizWindow(QWidget* parent = nullptr);
	~QuizWindow();

private:
	Q_OBJECT
	static const QDir dictionaryDir;
	static QFont songti;
	static QFont heiti;
	static QFont fangSongti;
	static QFont kaiti;

	void loadDictionaryFiles();
	void displayQuestion();
	void beginQuiz();
	void onAnswer(const QModelIndex&);
	void next();
	void updateUi(bool showQuestion);

	Ui::MainWindow* ui;
	OptionsDialog* optionsDialog;
	Quiz quiz_;
	QuizModel* quizModel;
};

#endif // MAINWINDOW_H
