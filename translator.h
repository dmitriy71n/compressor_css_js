#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <QSettings>

class Translator : public QObject
{
    Q_OBJECT

public:
    explicit Translator(QObject *parent = nullptr);
    void installInitialTranslation(); // Загружает язык из настроек или системный
    void switchToLanguage(const QString& languageCode); // e.g., "ru", "en"
    QString getCurrentLanguage() const { return m_currentLanguage; }

signals:
    void languageChanged();

private:
    void saveLanguageToSettings(const QString& langCode); // Новая функция
    QString loadLanguageFromSettings();                  // Новая функция

private:
    QTranslator m_appTranslator;
    QString m_currentLanguage;
    QSettings m_settings; // Объявляем как член класса, инициализируем в конструкторе
};

#endif // TRANSLATOR_H