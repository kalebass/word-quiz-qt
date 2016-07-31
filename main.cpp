#include <QApplication>
#include "quizwindow.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName("Word quiz");
	QApplication application(argc, argv);
	QuizWindow w;
	w.show();
	return application.exec();
}
