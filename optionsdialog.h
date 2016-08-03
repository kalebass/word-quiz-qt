#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "settingshandler.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
	Q_OBJECT

public:
	OptionsDialog(SettingsHandler& settings, QWidget* parent = nullptr);
	void readDictionaryList();
	~OptionsDialog();

signals:
	void dictionarySettingChanged() const;

private:
	void saveDictionarySettings() const;

	SettingsHandler& settings_;
	Ui::OptionsDialog* ui;
};

#endif // OPTIONSDIALOG_H
