#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFont>
#include <QMainWindow>
#include "quiz.h"
#include "settingshandler.h"

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
	void onChoice(const QModelIndex&);
	void onAnswerEntered();
	void next();
	void updateUi(bool showQuestion);

	Ui::MainWindow* ui;
	OptionsDialog* optionsDialog;
	QuizModel* quizModel;

	Quiz quiz_;
	SettingsHandler settings_;
};

#endif // MAINWINDOW_H
