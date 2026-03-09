#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QSettings>

class Translator;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Translator* translator, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCompressTabClicked();
    void onConcatenateTabClicked();
    void onAboutClicked();
    void onLanguageClicked();
    void onCompressBrowseInput();
    void onCompressBrowseOutput();
    void onCompressExecute();
    void onConcatBrowseInput();
    void onConcatBrowseOutput();
    void onConcatExecute();

private:
    void setupUI();
    void setupConnections();
    void updateTranslations();
    QString getMinifiedExtension(const QString& originalExt) const;
    QString getFormattedExtension(const QString& originalExt) const;
    void runHtmlParser2(const QString& inputFile, const QString& outputFile, bool minify);
    void runSassCompiler(const QString& inputFile, const QString& outputFile);

private:
    Ui::MainWindow *ui;
    Translator* m_translator;
    QStackedWidget* m_stack;
    QWidget* m_compressWidget;
    QWidget* m_concatWidget;

    // Compress Tab widgets
    QPushButton* m_btnCompressBrowseInput;
    QPushButton* m_btnCompressBrowseOutput;
    QPushButton* m_btnCompressExecute;
    QLineEdit* m_leCompressInputPath;
    QLineEdit* m_leCompressOutputPath;
    QComboBox* m_cbCompressOperation;
    QLabel* m_lblCompressInput;
    QLabel* m_lblCompressOutput;
    QLabel* m_lblCompressOperation;

    // Concatenate Tab widgets
    QPushButton* m_btnConcatBrowseInput;
    QPushButton* m_btnConcatBrowseOutput;
    QPushButton* m_btnConcatExecute;
    QLineEdit* m_leConcatInputPath;
    QLineEdit* m_leConcatOutputPath;
    QLabel* m_lblConcatInput;
    QLabel* m_lblConcatOutput;

    // Main toolbar buttons
    QPushButton* m_btnCompressTab;
    QPushButton* m_btnConcatTab;
    QPushButton* m_btnAbout;
    QPushButton* m_btnLanguage;

    // Settings
    QSettings m_settings;
};
#endif // MAINWINDOW_H