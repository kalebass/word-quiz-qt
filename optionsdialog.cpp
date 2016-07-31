#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::OptionsDialog }
{
	ui->setupUi(this);
	connect(this, &QDialog::accepted, this, &OptionsDialog::saveDictionarySettings);
}

void OptionsDialog::readDictionaryList()
{
	ui->dictionaryList->clear();
	QSettings settings;
	settings.beginGroup("dictionaries");
	auto fileNames{ settings.childKeys() };
	for (auto& fileName : fileNames) {
		auto item{ new QListWidgetItem{ fileName, ui->dictionaryList } };
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		auto isChecked{ settings.value(fileName, false).toBool() };
		item->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);
		ui->dictionaryList->addItem(item);
	}
}

void OptionsDialog::saveDictionarySettings() const
{
	QSettings settings;
	settings.beginGroup("dictionaries");
	auto changed{ false };
	auto fileCount{ ui->dictionaryList->count() };
	for (auto i{ 0 }; i < fileCount; ++i) {
		auto fileName{ ui->dictionaryList->item(i)->text() };
		auto checked{ ui->dictionaryList->item(i)->checkState() == Qt::Checked };
		auto checkedBefore{ settings.value(fileName).toBool() };
		if (checked != checkedBefore) {
			settings.setValue(fileName, checked);
			changed = true;
		}
	}
	if (changed) {
		emit dictionarySettingChanged();
	}
}

OptionsDialog::~OptionsDialog()
{
	delete ui;
}
