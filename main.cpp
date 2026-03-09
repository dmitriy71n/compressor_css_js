#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"
#include "translator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize translator manager
    Translator translator;
    translator.installInitialTranslation();

    MainWindow w(&translator);
    w.show();

    return app.exec();
}