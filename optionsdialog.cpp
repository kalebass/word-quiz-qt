#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(SettingsHandler& settings, QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::OptionsDialog },
	settings_{ settings }
{
	ui->setupUi(this);
	connect(this, &QDialog::accepted, this, &OptionsDialog::saveDictSettings);
	auto flags{ windowFlags() | Qt::CustomizeWindowHint };
	flags &= ~Qt::WindowContextHelpButtonHint;
	flags &= ~Qt::WindowSystemMenuHint;
	flags &= ~Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
}

void OptionsDialog::readDictList()
{
	ui->dictList->clear();
	auto dicts{ settings_.dicts() };
	for (auto it{ dicts.constBegin() }; it != dicts.constEnd(); ++it) {
		auto item{ new QListWidgetItem{ it.key(), ui->dictList } };
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(it.value() ? Qt::Checked : Qt::Unchecked);
		ui->dictList->addItem(item);
	}
}

void OptionsDialog::saveDictSettings() const
{
	auto changed{ false };
	auto fileCount{ ui->dictList->count() };
	for (auto i{ 0 }; i < fileCount; ++i) {
		auto item{ ui->dictList->item(i) };
		auto fileName{ item->text() };
		auto isChecked{ item->checkState() == Qt::Checked };
		auto wasChecked{ settings_.isDictEnabled(fileName) };
		if (isChecked != wasChecked) {
			settings_.setDictEnabled(fileName, isChecked);
			changed = true;
		}
	}
	if (changed) {
		emit dictSettingChanged();
	}
}

OptionsDialog::~OptionsDialog()
{
	delete ui;
}
