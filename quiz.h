#ifndef QUIZ_H
#define QUIZ_H

#include <QVector>

class QTextStream;

class Quiz
{
public:
	enum class Mode { MultipleChoice, Enter };
	enum class Language { Hanzi, Pinyin, English };

	Quiz();
	int wordCount() const;
	int currentIndex() const;
	int score() const;
	int numChoices() const;

	void addWords(QTextStream&);
	void clear();
	QString currentWord(Language) const;
	QString alternative(int number, Language) const;

	void begin();
	bool makeAnswer(int alternative);
	bool makeAnswer(const QString&);
	void nextQuestion();

private:
	int currentCorrectIndex() const;

	int currentQuestion_;
	int score_;
	int numChoices_;
	Mode mode_;
	QVector<QStringList> dictionary_;
	QVector<int> currentChoiceIndexes_;
	QVector<int> answerIndexes_;
};

#endif // QUIZ_H
