#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::OptionsDialog }
{
	ui->setupUi(this);
	connect(this, &QDialog::accepted, this, &OptionsDialog::saveDictionarySettings);
	auto flags{ windowFlags() | Qt::CustomizeWindowHint };
	flags &= ~Qt::WindowContextHelpButtonHint;
	flags &= ~Qt::WindowSystemMenuHint;
	flags &= ~Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
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
		auto item{ ui->dictionaryList->item(i) };
		auto fileName{ item->text() };
		auto isChecked{ item->checkState() == Qt::Checked };
		auto wasChecked{ settings.value(fileName).toBool() };
		if (isChecked != wasChecked) {
			settings.setValue(fileName, isChecked);
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
