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
    void readSettings();
    QFont chineseFont() const;
    ~OptionsDialog();

signals:
    void dictSettingChanged() const;
    void modeSettingChanged() const;

private:
    void saveDictSettings() const;
    void saveModeSettings() const;
    void addFontItems();
    Ui::OptionsDialog* ui;
};

#endif // OPTIONSDIALOG_H
