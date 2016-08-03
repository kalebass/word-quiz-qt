#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::OptionsDialog }
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
	QSettings settings;
	settings.beginGroup("dict");
	auto fileNames{ settings.childKeys() };
	for (auto& fileName : fileNames) {
		auto item{ new QListWidgetItem{ fileName, ui->dictList } };
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		auto isChecked{ settings.value(fileName, false).toBool() };
		item->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);
		ui->dictList->addItem(item);
	}
}

void OptionsDialog::saveDictSettings() const
{
	QSettings settings;
	settings.beginGroup("dict");
	auto changed{ false };
	auto fileCount{ ui->dictList->count() };
	for (auto i{ 0 }; i < fileCount; ++i) {
		auto item{ ui->dictList->item(i) };
		auto fileName{ item->text() };
		auto isChecked{ item->checkState() == Qt::Checked };
		auto wasChecked{ settings.value(fileName).toBool() };
		if (isChecked != wasChecked) {
			settings.setValue(fileName, isChecked);
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
