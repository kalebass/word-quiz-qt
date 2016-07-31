#ifndef QUIZMODEL_H
#define QUIZMODEL_H

#include <QAbstractTableModel>

class QFont;
class Quiz;

class QuizModel : public QAbstractTableModel
{
public:
	QuizModel(Quiz&, QObject* parent = nullptr);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void makeAnswer(const QModelIndex&);
	void setChineseFont(QFont&);

private:
	Q_OBJECT
	Quiz& quiz_;
	QFont* chineseFont;
};

#endif // QUIZMODEL_H
