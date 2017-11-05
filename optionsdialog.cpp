#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::OptionsDialog }
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &OptionsDialog::saveDictSettings);
    connect(this, &QDialog::accepted, this, &OptionsDialog::saveModeSettings);
    auto flags{ windowFlags() | Qt::CustomizeWindowHint };
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    addFontItems();
}

void OptionsDialog::readSettings()
{
    QSettings settings;
    auto multipleChoice{ settings.value("multiple choice mode").toBool() };
    ui->multipleChoice->setChecked(multipleChoice);
    ui->freeform->setChecked(!multipleChoice);

    ui->dictList->clear();
    settings.beginGroup("dict");
    auto fileNames{ settings.childKeys() };
    for (auto& fileName : fileNames) {
        auto item{ new QListWidgetItem{ fileName, ui->dictList } };
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        auto isChecked{ settings.value(fileName, false).toBool() };
        item->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);
        ui->dictList->addItem(item);
    }
}

QFont OptionsDialog::chineseFont() const
{
    return ui->fontList->currentItem()->font();
}

void OptionsDialog::saveModeSettings() const
{
    QSettings settings;
    auto isChecked{ ui->multipleChoice->isChecked() };
    auto wasChecked{ settings.value("multiple choice mode").toBool() };
    auto changed{ isChecked != wasChecked };
    settings.setValue("multiple choice mode", isChecked);
    if (changed) {
        emit modeSettingChanged();
    }
}

void OptionsDialog::addFontItems()
{
    QString sampleChars{ "中文" };
    QFont songti{ "SimSun", 18 };
    QFont heiti{ "Microsoft YaHei", 16 };
    QFont fangSongti{ "FangSong", 20 };
    QFont kaiti{ "KaiTi", 22 };
    auto item0{ new QListWidgetItem{ sampleChars, ui->fontList } };
    auto item1{ new QListWidgetItem{ sampleChars, ui->fontList } };
    auto item2{ new QListWidgetItem{ sampleChars, ui->fontList } };
    auto item3{ new QListWidgetItem{ sampleChars, ui->fontList } };
    item0->setFont(songti);
    item1->setFont(heiti);
    item2->setFont(fangSongti);
    item3->setFont(kaiti);
    item0->setSelected(true);
    ui->fontList->setCurrentItem(item0);
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
