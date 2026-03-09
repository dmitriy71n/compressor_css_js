#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QDialog>
#include <QComboBox> // Заменяем QButtonGroup/QRadioButton

class Translator;

namespace Ui {
class LanguageDialog;
}

class LanguageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageDialog(Translator* translator, QWidget *parent = nullptr);
    ~LanguageDialog();

private slots:
    void onLanguageSelected();

private:
    Ui::LanguageDialog *ui;
    Translator* m_translator;

    // Заменяем на QComboBox
    QComboBox* m_comboLanguage;
};

#endif // LANGUAGEDIALOG_H