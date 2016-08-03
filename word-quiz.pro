#-------------------------------------------------
#
# Project created by QtCreator 2013-03-30T16:57:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = word-quiz
TEMPLATE = app


SOURCES += main.cpp\
    quizwindow.cpp \
    quiz.cpp \
    quizmodel.cpp \
    optionsdialog.cpp \
    settingshandler.cpp

HEADERS  += \
    quizwindow.h \
    quiz.h \
    quizmodel.h \
    optionsdialog.h \
    settingshandler.h

FORMS    += \
    quizwindow.ui \
    optionsdialog.ui
