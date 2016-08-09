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
	static const QDir dictDir;
	static QFont songti;
	static QFont heiti;
	static QFont fangSongti;
	static QFont kaiti;

	void loadDictFiles();
	void addDict(const QString& path);
	void displayQuestion();
	void beginQuiz();
	void onChoice(const QModelIndex&);
	void onAnswerEntered();
	void onModeChanged();
	void applyFont();
	void next();
	void updateUi(bool showQuestion);

	Ui::MainWindow* ui;
	OptionsDialog* optionsDialog;
	Quiz quiz_;
	QuizModel* quizModel;
};

#endif // MAINWINDOW_H
