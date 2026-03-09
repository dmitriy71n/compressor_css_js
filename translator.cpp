#include "translator.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

// Используем стандартный путь конфигурации Qt, как указано в ТЗ
Translator::Translator(QObject *parent)
    : QObject(parent)
    , m_currentLanguage("en") // Default
    // Используем QStandardPaths::ConfigLocation для получения пути ~/.config/
    // и добавляем подпуть compressor-2.0/language.conf
    , m_settings(
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/compressor-2.0/language.conf",
        QSettings::IniFormat)
{
    qDebug() << "Translator: Settings file path is:" << m_settings.fileName();
    // Убедимся, что директория существует сразу после инициализации m_settings
    QFileInfo fi(m_settings.fileName());
    QDir().mkpath(fi.absolutePath());
    qDebug() << "Translator: Config directory path is:" << fi.absolutePath();
}

void Translator::installInitialTranslation()
{
    qDebug() << "Translator: installInitialTranslation called.";
    // Загружаем сохранённый язык из настроек
    QString savedLang = loadLanguageFromSettings();
    qDebug() << "Translator: Loaded language from settings:" << savedLang;
    if (!savedLang.isEmpty() && (savedLang == "ru" || savedLang == "en")) {
        switchToLanguage(savedLang);
    } else {
        // Fallback: пробуем системный язык
        QLocale systemLocale = QLocale::system();
        QString langCode = systemLocale.name().left(2);
        if (langCode == "ru" || langCode == "en") {
            switchToLanguage(langCode);
        } else {
            switchToLanguage("en"); // Финальный fallback
        }
    }
}

void Translator::switchToLanguage(const QString& languageCode)
{
    qDebug() << "Translator: switchToLanguage called with:" << languageCode;
    // Удаляем текущий перевод
    qApp->removeTranslator(&m_appTranslator);

    if (languageCode == "ru" || languageCode == "en") {
        // Загружаем соответствующий .qm файл ИЗ ФАЙЛОВОЙ СИСТЕМЫ
        // Путь: /opt/compressor/translations/compressor_ru.qm
        QString fileName = QString("/opt/compressor/translations/compressor_%1.qm").arg(languageCode);
        qDebug() << "Translator: Loading translation file from filesystem:" << fileName;
        if (m_appTranslator.load(fileName)) {
            qApp->installTranslator(&m_appTranslator);
            m_currentLanguage = languageCode;
            // Сохраняем выбранный язык в настройки
            saveLanguageToSettings(languageCode);
            emit languageChanged(); // Сигнал для обновления интерфейса
        } else {
            qDebug() << "Translator: Failed to load translation file from filesystem:" << fileName;
        }
    } else {
        qDebug() << "Translator: Invalid language code:" << languageCode;
    }
}

void Translator::saveLanguageToSettings(const QString& langCode)
{
    qDebug() << "Translator: Saving language to settings:" << langCode;
    // Сохраняем в группу [General]
    m_settings.setValue("General/Language", langCode);
    m_settings.sync(); // Принудительно записываем на диск
    qDebug() << "Translator: Sync called, error state:" << m_settings.status();
}

QString Translator::loadLanguageFromSettings()
{
    // Читаем из группы [General], возвращаем "en" по умолчанию
    QString val = m_settings.value("General/Language", "en").toString();
    qDebug() << "Translator: Loaded language value:" << val;
    return val;
}

