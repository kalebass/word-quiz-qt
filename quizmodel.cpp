#include "quizmodel.h"
#include <QBrush>
#include <QFont>
#include "quiz.h"

QuizModel::QuizModel(Quiz& quiz, QObject* parent) :
	QAbstractTableModel{ parent },
	quiz_{ quiz },
	showAnswer_{ false }
{}

int QuizModel::rowCount(const QModelIndex& parent) const
{
	if (quiz_.numChoices() == 0 && showAnswer_) {
		return 1;
	}
	return quiz_.numChoices();
	parent;
}

int QuizModel::columnCount(const QModelIndex& parent) const
{
	constexpr auto hanziPinyinEnglish{ 3 };
	return hanziPinyinEnglish;
	parent;
}

QVariant QuizModel::data(const QModelIndex& index, int role) const
{
	auto choice{ index.row() };
	auto language{ static_cast<Quiz::Language>(index.column()) };
	if (role == Qt::DisplayRole) {
		if (quiz_.numChoices() == 0 && showAnswer_) {
			return quiz_.currentWord(language);
		}
		return { quiz_.alternative(choice, language) };
	} else if (role == Qt::FontRole && language == Quiz::Language::Hanzi) {
		return QVariant::fromValue(*chineseFont);
	} else if (role == Qt::BackgroundRole) {
		return { QBrush{ QColor::fromHsl(0, 255, 128) } };
	} else {
		return QVariant::Invalid;
	}
}

QVariant QuizModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) {
		return QVariant::Invalid;
	}
	return QVariant(tr("header"));
	section, orientation;
}

Qt::ItemFlags QuizModel::flags(const QModelIndex& index) const
{
	if (index.column() == 0) {
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	return Qt::ItemIsEnabled;
}

void QuizModel::showAnswer(bool value)
{
	showAnswer_ = value;
	if (quiz_.numChoices() == 0) {
		emit layoutChanged();
	}
}

void QuizModel::changeQuizMode()
{
	emit layoutChanged();
}

void QuizModel::setChineseFont(QFont& font)
{
	chineseFont = &font;
}
