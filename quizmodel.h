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
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void setChineseFont(QFont&);

private:
	Q_OBJECT
	Quiz& quiz_;
	QFont* chineseFont;
};

#endif // QUIZMODEL_H
