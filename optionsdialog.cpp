#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(SettingsHandler& settings, QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::OptionsDialog },
	settings_{ settings }
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
	auto dicts{ settings_.dicts() };
	for (auto it{ dicts.constBegin() }; it != dicts.constEnd(); ++it) {
		auto item{ new QListWidgetItem{ it.key(), ui->dictionaryList } };
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(it.value() ? Qt::Checked : Qt::Unchecked);
		ui->dictionaryList->addItem(item);
	}
}

void OptionsDialog::saveDictionarySettings() const
{
	auto changed{ false };
	auto fileCount{ ui->dictionaryList->count() };
	for (auto i{ 0 }; i < fileCount; ++i) {
		auto item{ ui->dictionaryList->item(i) };
		auto fileName{ item->text() };
		auto isChecked{ item->checkState() == Qt::Checked };
		auto wasChecked{ settings_.isDictEnabled(fileName) };
		if (isChecked != wasChecked) {
			settings_.setDictEnabled(fileName, isChecked);
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
