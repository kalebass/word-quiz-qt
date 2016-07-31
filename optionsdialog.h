#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit OptionsDialog(QWidget* parent = nullptr);
	void readDictionaryList();
	~OptionsDialog();

signals:
	void dictionarySettingChanged() const;

private:
	void saveDictionarySettings() const;
	Ui::OptionsDialog* ui;
};

#endif // OPTIONSDIALOG_H
