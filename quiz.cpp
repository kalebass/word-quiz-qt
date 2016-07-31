#include "quiz.h"
#include <algorithm>
#include <ctime>
#include <random>
#include <QTextStream>


QVector<int> shuffledRange(int length, int max)
{
	QVector<int> numbers(max);
	std::iota(numbers.begin(), numbers.end(), 0);
	std::random_device rd;
	static std::default_random_engine urng{ rd() };
	std::shuffle(numbers.begin(), numbers.end(), urng);
	return numbers.mid(0, length);
}

Quiz::Quiz() :
	currentQuestion_{ -1 },
	score_{ 0 },
	numChoices_{ 7 },
	mode_{ Mode::MultipleChoice },
	dictionary_{ },
	currentChoiceIndexes_{ },
	answerIndexes_{ }
{
	qsrand(std::time(nullptr));
}

void Quiz::addWords(QTextStream& stream)
{
	const auto separator{ QChar(QChar::Tabulation) };
	while (!stream.atEnd()) {
		auto line{ stream.readLine() };
		dictionary_.append(line.split(separator));
	}
}

void Quiz::clear()
{
	dictionary_.clear();
	currentQuestion_ = -1;
	score_ = 0;
}

int Quiz::wordCount() const
{
	return dictionary_.length();
}

int Quiz::currentIndex() const
{
	return currentQuestion_;
}

int Quiz::score() const
{
	return score_;
}

int Quiz::numChoices() const
{
	return numChoices_;
}

QString Quiz::currentWord(Language language) const
{
	auto words{ dictionary_.at(currentCorrectIndex()) };
	auto index{ static_cast<int>(language) };
	return words.at(index);
}

QString Quiz::alternative(int number, Language language) const
{
	auto wordIndex{ currentChoiceIndexes_.at(number) };
	auto words{ dictionary_.at(wordIndex) };
	auto index{ static_cast<int>(language) };
	return words.at(index);
}

bool Quiz::makeAnswer(int choice)
{
	if (currentChoiceIndexes_.at(choice) == currentCorrectIndex()) {
		++score_;
		return true;
	}
	return false;
}

void Quiz::begin()
{
	answerIndexes_ = shuffledRange(wordCount(), wordCount());
}

void Quiz::nextQuestion()
{
	++currentQuestion_;
	currentChoiceIndexes_ = shuffledRange(numChoices(), wordCount());
	if (!currentChoiceIndexes_.contains(currentCorrectIndex())) {
		auto alternative{ qrand() % numChoices() };
		currentChoiceIndexes_[alternative] = currentCorrectIndex();
	}
}

int Quiz::currentCorrectIndex() const
{
	return answerIndexes_.at(currentQuestion_);
}
