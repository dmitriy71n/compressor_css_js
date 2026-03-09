#include "languagedialog.h"
#include "translator.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

LanguageDialog::LanguageDialog(Translator* translator, QWidget *parent)
    : QDialog(parent)
    , ui(nullptr) // ui не используется, т.к. интерфейс создаётся программно
    , m_translator(translator)
{
    setWindowTitle(tr("Select Language"));
    setFixedSize(250, 120); // Уменьшаем высоту

    auto* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(tr("Choose Language:"), this);
    layout->addWidget(label);

    m_comboLanguage = new QComboBox(this);
    m_comboLanguage->addItem(tr("English"), "en");
    m_comboLanguage->addItem(tr("Русский"), "ru");

    // Устанавливаем текущий язык
    QString currentLang = m_translator->getCurrentLanguage();
    int index = m_comboLanguage->findData(currentLang);
    if (index != -1) {
        m_comboLanguage->setCurrentIndex(index);
    }

    layout->addWidget(m_comboLanguage);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &LanguageDialog::onLanguageSelected);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

LanguageDialog::~LanguageDialog()
{
}

void LanguageDialog::onLanguageSelected()
{
    QString selectedLangCode = m_comboLanguage->currentData().toString();
    m_translator->switchToLanguage(selectedLangCode);
    accept();
}