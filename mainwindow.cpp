#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "translator.h"
#include "dialogs/aboutdialog.h"
#include "dialogs/languagedialog.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QFileInfo>
#include <QRegularExpression>

MainWindow::MainWindow(Translator* translator, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_translator(translator)
    , m_stack(new QStackedWidget(this))
    , m_compressWidget(new QWidget())
    , m_concatWidget(new QWidget())
    , m_settings("DmitriyNosov", "Compressor")
{
    setupUI();
    setupConnections();
    updateTranslations();
    setFixedSize(396, 208); // Установить фиксированный размер
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle(tr("Compressor"));

    // Toolbar buttons
    m_btnCompressTab = new QPushButton(tr("Compress File"), this);
    m_btnConcatTab = new QPushButton(tr("Concatenate CSS"), this);
    m_btnLanguage = new QPushButton(tr("Language"), this); // Moved up
    m_btnAbout = new QPushButton(tr("?"), this);           // Moved down

    auto* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_btnCompressTab);
    topLayout->addWidget(m_btnConcatTab);
    topLayout->addStretch();
    topLayout->addWidget(m_btnLanguage); // Now first
    topLayout->addWidget(m_btnAbout);    // Then ?

    // Compress Widget UI
    m_lblCompressInput = new QLabel(tr("Input File:"), m_compressWidget);
    m_leCompressInputPath = new QLineEdit(m_compressWidget);
    m_btnCompressBrowseInput = new QPushButton(tr("Browse..."), m_compressWidget);
    auto* inputLayout = new QHBoxLayout();
    inputLayout->addWidget(m_leCompressInputPath);
    inputLayout->addWidget(m_btnCompressBrowseInput);

    m_lblCompressOutput = new QLabel(tr("Output File:"), m_compressWidget);
    m_leCompressOutputPath = new QLineEdit(m_compressWidget);
    m_btnCompressBrowseOutput = new QPushButton(tr("Browse..."), m_compressWidget);
    auto* outputLayout = new QHBoxLayout();
    outputLayout->addWidget(m_leCompressOutputPath);
    outputLayout->addWidget(m_btnCompressBrowseOutput);

    m_lblCompressOperation = new QLabel(tr("Operation:"), m_compressWidget);
    m_cbCompressOperation = new QComboBox(m_compressWidget);
    m_cbCompressOperation->addItems({
        tr("Compress CSS"),
        tr("Compress HTML"),
        tr("SASS to CSS"),
        tr("Compress HTML+CSS+JS"),
        tr("Compress JavaScript, JQuery"),
        tr("Format CSS"),
        tr("Format HTML"),
        tr("Format JavaScript, JQuery")
    });

    m_btnCompressExecute = new QPushButton(tr("Execute"), m_compressWidget);
    m_btnCompressExecute->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* compressLayout = new QVBoxLayout(m_compressWidget);
    auto* formLayout = new QFormLayout();
    formLayout->addRow(m_lblCompressInput, inputLayout);
    formLayout->addRow(m_lblCompressOutput, outputLayout);
    formLayout->addRow(m_lblCompressOperation, m_cbCompressOperation);
    compressLayout->addLayout(formLayout);

    auto* buttonLayout = new QHBoxLayout(); // New layout for Execute button
    buttonLayout->addWidget(m_btnCompressExecute);
    buttonLayout->addStretch(); // Pushes button to the left
    compressLayout->addLayout(buttonLayout);
    compressLayout->addStretch();

    // Concatenate Widget UI
    m_lblConcatInput = new QLabel(tr("CSS Files Directory:"), m_concatWidget);
    m_leConcatInputPath = new QLineEdit(m_concatWidget);
    m_btnConcatBrowseInput = new QPushButton(tr("Browse..."), m_concatWidget);
    auto* concatInputLayout = new QHBoxLayout();
    concatInputLayout->addWidget(m_leConcatInputPath);
    concatInputLayout->addWidget(m_btnConcatBrowseInput);

    m_lblConcatOutput = new QLabel(tr("Output CSS File:"), m_concatWidget);
    m_leConcatOutputPath = new QLineEdit(m_concatWidget);
    m_btnConcatBrowseOutput = new QPushButton(tr("Browse..."), m_concatWidget);
    auto* concatOutputLayout = new QHBoxLayout();
    concatOutputLayout->addWidget(m_leConcatOutputPath);
    concatOutputLayout->addWidget(m_btnConcatBrowseOutput);

    m_btnConcatExecute = new QPushButton(tr("Execute"), m_concatWidget);
    m_btnConcatExecute->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* concatLayout = new QVBoxLayout(m_concatWidget);
    auto* concatFormLayout = new QFormLayout();
    concatFormLayout->addRow(m_lblConcatInput, concatInputLayout);
    concatFormLayout->addRow(m_lblConcatOutput, concatOutputLayout);
    concatLayout->addLayout(concatFormLayout);

    auto* concatButtonLayout = new QHBoxLayout(); // New layout for Execute button
    concatButtonLayout->addWidget(m_btnConcatExecute);
    concatButtonLayout->addStretch(); // Pushes button to the left
    concatLayout->addLayout(concatButtonLayout);
    concatLayout->addStretch();

    // Stack
    m_stack->addWidget(m_compressWidget);
    m_stack->addWidget(m_concatWidget);
    m_stack->setCurrentIndex(0); // Start with compress tab

    auto* centralLayout = new QVBoxLayout();
    centralLayout->addLayout(topLayout);
    centralLayout->addWidget(m_stack);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections()
{
    connect(m_btnCompressTab, &QPushButton::clicked, this, &MainWindow::onCompressTabClicked);
    connect(m_btnConcatTab, &QPushButton::clicked, this, &MainWindow::onConcatenateTabClicked);
    connect(m_btnAbout, &QPushButton::clicked, this, &MainWindow::onAboutClicked);
    connect(m_btnLanguage, &QPushButton::clicked, this, &MainWindow::onLanguageClicked);

    connect(m_btnCompressBrowseInput, &QPushButton::clicked, this, &MainWindow::onCompressBrowseInput);
    connect(m_btnCompressBrowseOutput, &QPushButton::clicked, this, &MainWindow::onCompressBrowseOutput);
    connect(m_btnCompressExecute, &QPushButton::clicked, this, &MainWindow::onCompressExecute);

    connect(m_btnConcatBrowseInput, &QPushButton::clicked, this, &MainWindow::onConcatBrowseInput);
    connect(m_btnConcatBrowseOutput, &QPushButton::clicked, this, &MainWindow::onConcatBrowseOutput);
    connect(m_btnConcatExecute, &QPushButton::clicked, this, &MainWindow::onConcatExecute);
}

void MainWindow::updateTranslations()
{
    if (!m_btnCompressTab) return;

    m_btnCompressTab->setText(tr("Compress File"));
    m_btnConcatTab->setText(tr("Concatenate CSS"));
    m_btnLanguage->setText(tr("Language"));
    m_btnAbout->setText(tr("?"));

    m_lblCompressInput->setText(tr("Input File:"));
    m_lblCompressOutput->setText(tr("Output File:"));
    m_lblCompressOperation->setText(tr("Operation:"));
    m_btnCompressBrowseInput->setText(tr("Browse..."));
    m_btnCompressBrowseOutput->setText(tr("Browse..."));
    m_btnCompressExecute->setText(tr("Execute"));

    m_lblConcatInput->setText(tr("CSS Files Directory:"));
    m_lblConcatOutput->setText(tr("Output CSS File:"));
    m_btnConcatBrowseInput->setText(tr("Browse..."));
    m_btnConcatBrowseOutput->setText(tr("Browse..."));
    m_btnConcatExecute->setText(tr("Execute"));

    // Update combo box items
    QStringList operations = {
        tr("Compress CSS"),
        tr("Compress HTML"),
        tr("SASS to CSS"),
        tr("Compress HTML+CSS+JS"),
        tr("Compress JavaScript, JQuery"),
        tr("Format CSS"),
        tr("Format HTML"),
        tr("Format JavaScript, JQuery")
    };
    m_cbCompressOperation->clear();
    m_cbCompressOperation->addItems(operations);

    setWindowTitle(tr("Compressor"));
}

void MainWindow::onCompressTabClicked()
{
    m_stack->setCurrentIndex(0);
}

void MainWindow::onConcatenateTabClicked()
{
    m_stack->setCurrentIndex(1);
}

void MainWindow::onAboutClicked()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::onLanguageClicked()
{
    LanguageDialog dlg(m_translator, this);
    dlg.exec();
}

void MainWindow::onCompressBrowseInput()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Input File"),
        QDir::homePath(),
        tr("Files (*.html *.htm *.css *.js *.sass *.scss);;All Files (*)"));
    if (!fileName.isEmpty()) {
        m_leCompressInputPath->setText(fileName);
        if (m_leCompressOutputPath->text().isEmpty()) {
            QFileInfo info(fileName);
            QString baseName = info.baseName();
            QString dir = info.absolutePath();
            int index = m_cbCompressOperation->currentIndex();
            QString ext;
            if (index == 5 || index == 6 || index == 7) { // Format
                ext = getFormattedExtension(info.suffix());
            } else { // Compress
                ext = getMinifiedExtension(info.suffix());
            }
            m_leCompressOutputPath->setText(dir + "/" + baseName + ext);
        }
    }
}

void MainWindow::onCompressBrowseOutput()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Select Output File"),
        QDir::homePath(),
        tr("All Files (*)"));
    if (!fileName.isEmpty()) {
        m_leCompressOutputPath->setText(fileName);
    }
}

void MainWindow::onCompressExecute()
{
    QString inputPath = m_leCompressInputPath->text();
    QString outputPath = m_leCompressOutputPath->text();
    int operationIndex = m_cbCompressOperation->currentIndex();

    if (inputPath.isEmpty() || outputPath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please specify both input and output files."));
        return;
    }

    QFileInfo inputInfo(inputPath);
    if (!inputInfo.exists()) {
        QMessageBox::critical(this, tr("Error"), tr("Input file does not exist."));
        return;
    }

    QString inputExt = inputInfo.suffix().toLower();

    switch (operationIndex) {
        case 0: { // Compress CSS
            if (inputExt != "css") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .css file for this operation."));
                return;
            }
            QFile src(inputPath);
            if (src.open(QIODevice::ReadOnly)) {
                QByteArray data = src.readAll();
                QString content = QString::fromUtf8(data);
                content.replace(QRegularExpression("\\s+"), " ");
                content.replace(QRegularExpression("/\\*.*?\\*/"), "");
                QFile dst(outputPath);
                if (dst.open(QIODevice::WriteOnly)) {
                    dst.write(content.toUtf8());
                    QMessageBox::information(this, tr("Success"), tr("File compressed successfully."));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
                }
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
            }
            break;
        }
        case 1: { // Compress HTML
            if (inputExt != "html" && inputExt != "htm") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .html or .htm file for this operation."));
                return;
            }
            QFile hsrc(inputPath);
            if (hsrc.open(QIODevice::ReadOnly)) {
                QByteArray data = hsrc.readAll();
                QString content = QString::fromUtf8(data);
                content.replace(QRegularExpression(">\\s+<"), "><");
                content.replace(QRegularExpression("\\s+"), " ");
                content.replace(QRegularExpression("<!--.*?-->"), "");
                QFile hdst(outputPath);
                if (hdst.open(QIODevice::WriteOnly)) {
                    hdst.write(content.toUtf8());
                    QMessageBox::information(this, tr("Success"), tr("File compressed successfully."));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
                }
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
            }
            break;
        }
        case 2: { // SASS to CSS
            if (inputExt != "sass" && inputExt != "scss") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .sass or .scss file for this operation."));
                return;
            }
            runSassCompiler(inputPath, outputPath);
            break;
        }
        case 3: { // Compress HTML+CSS+JS
            if (inputExt != "html" && inputExt != "htm") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .html or .htm file for this operation."));
                return;
            }
            runHtmlParser2(inputPath, outputPath, true); // true for minify
            break;
        }
        case 4: { // Compress JavaScript, JQuery
            if (inputExt != "js") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .js file for this operation."));
                return;
            }
            QFile jsrc(inputPath);
            if (jsrc.open(QIODevice::ReadOnly)) {
                QByteArray data = jsrc.readAll();
                QString content = QString::fromUtf8(data);
                content.replace(QRegularExpression("\\s+"), " ");
                content.replace(QRegularExpression("/\\*.*?\\*/"), "");
                content.replace(QRegularExpression("//.*"), "");
                QFile jdst(outputPath);
                if (jdst.open(QIODevice::WriteOnly)) {
                    jdst.write(content.toUtf8());
                    QMessageBox::information(this, tr("Success"), tr("File compressed successfully."));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
                }
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
            }
            break;
        }
        case 5: { // Format CSS
            if (inputExt != "css") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .css file for this operation."));
                return;
            }
            QFile fcss_src(inputPath);
            if (fcss_src.open(QIODevice::ReadOnly)) {
                QByteArray data = fcss_src.readAll();
                QString content = QString::fromUtf8(data);
                content.replace("{", "{\n  ");
                content.replace(";", ";\n  ");
                content.replace("}", "\n}\n");
                QFile fcss_dst(outputPath);
                if (fcss_dst.open(QIODevice::WriteOnly)) {
                    fcss_dst.write(content.toUtf8());
                    QMessageBox::information(this, tr("Success"), tr("File formatted successfully."));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
                }
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
            }
            break;
        }
        case 6: { // Format HTML
            if (inputExt != "html" && inputExt != "htm") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .html or .htm file for this operation."));
                return;
            }
            QFile fhtml_src(inputPath);
            if (fhtml_src.open(QIODevice::ReadOnly)) {
                QByteArray data = fhtml_src.readAll();
                QString content = QString::fromUtf8(data);
                content.replace("><", ">\n<");
                QFile fhtml_dst(outputPath);
                if (fhtml_dst.open(QIODevice::WriteOnly)) {
                    fhtml_dst.write(content.toUtf8());
                    QMessageBox::information(this, tr("Success"), tr("File formatted successfully."));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
                }
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
            }
            break;
        }
        case 7: { // Format JavaScript, JQuery
            if (inputExt != "js") {
                QMessageBox::warning(this, tr("Warning"), tr("Expected .js file for this operation."));
                return;
            }
            QFile fjs_src(inputPath);
            if (fjs_src.open(QIODevice::ReadOnly)) {
                QByteArray data = fjs_src.readAll();
                QString content = QString::fromUtf8(data);
                content.replace(";", ";\n");
                content.replace("{", "{\n");
                content.replace("}", "\n}");
                QFile fjs_dst(outputPath);
                if (fjs_dst.open(QIODevice::WriteOnly)) {
                    fjs_dst.write(content.toUtf8());
                    QMessageBox::information(this, tr("Success"), tr("File formatted successfully."));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
                }
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
            }
            break;
        }
        default: {
            QMessageBox::information(this, tr("Info"), tr("Operation not implemented yet."));
            break;
        }
    }
}

void MainWindow::onConcatBrowseInput()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select CSS Files Directory"),
        QDir::homePath());
    if (!dirName.isEmpty()) {
        m_leConcatInputPath->setText(dirName);
        if (m_leConcatOutputPath->text().isEmpty()) {
            m_leConcatOutputPath->setText(dirName + "/concatenated.css");
        }
    }
}

void MainWindow::onConcatBrowseOutput()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Select Output CSS File"),
        QDir::homePath(),
        tr("CSS Files (*.css);;All Files (*)"));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".css", Qt::CaseInsensitive)) {
            fileName += ".css";
        }
        m_leConcatOutputPath->setText(fileName);
    }
}

void MainWindow::onConcatExecute()
{
    QString inputDirPath = m_leConcatInputPath->text();
    QString outputPath = m_leConcatOutputPath->text();

    if (inputDirPath.isEmpty() || outputPath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please specify both input directory and output file."));
        return;
    }

    QDir inputDir(inputDirPath);
    if (!inputDir.exists()) {
        QMessageBox::critical(this, tr("Error"), tr("Input directory does not exist.")); // Fixed missing closing parenthesis
        return;
    }

    QFileInfoList fileInfoList = inputDir.entryInfoList(QStringList("*.css"), QDir::Files, QDir::Name | QDir::IgnoreCase);
    if (fileInfoList.isEmpty()) {
        QMessageBox::information(this, tr("Info"), tr("No .css files found in the selected directory."));
        return;
    }

    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
        return;
    }

    for (const QFileInfo &fileInfo : fileInfoList) {
        QFile inputFile(fileInfo.absoluteFilePath());
        if (!inputFile.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, tr("Warning"), tr("Could not read file: %1").arg(fileInfo.fileName()));
            continue;
        }
        outputFile.write("\n/* --- File: ");
        outputFile.write(fileInfo.fileName().toUtf8());
        outputFile.write(" --- */\n");
        outputFile.write(inputFile.readAll());
        inputFile.close();
    }

    outputFile.close();
    QMessageBox::information(this, tr("Success"), tr("CSS files concatenated successfully."));
}

QString MainWindow::getMinifiedExtension(const QString& originalExt) const
{
    return "." + originalExt + ".min";
}

QString MainWindow::getFormattedExtension(const QString& originalExt) const
{
    return "." + originalExt + ".fmt";
}

void MainWindow::runSassCompiler(const QString& inputFile, const QString& outputFile)
{
    QString sassBinary = "/opt/compressor/lib/dart-sass/sass";
    QProcess process;
    QStringList arguments;
    arguments << inputFile << outputFile;

    process.start(sassBinary, arguments);
    if (!process.waitForStarted()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to start sass compiler. Is it installed correctly at /opt/compressor/lib/dart-sass/sass ?"));
        return;
    }

    process.waitForFinished(-1);
    QByteArray errorData = process.readAllStandardError();
    if (!errorData.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Sass compilation failed:\n%1").arg(QString::fromUtf8(errorData)));
        return;
    }

    QMessageBox::information(this, tr("Success"), tr("Sass file compiled to CSS successfully."));
}

void MainWindow::runHtmlParser2(const QString& inputFile, const QString& outputFile, bool minify)
{
    // This requires Node.js and htmlparser2 to be available via npx
    // For now, simulate with basic minification as a fallback
    QFile src(inputFile);
    if (!src.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot read input file."));
        return;
    }
    QByteArray data = src.readAll();
    QString content = QString::fromUtf8(data);

    if (minify) {
        content.replace(QRegularExpression(">\\s+<"), "><");
        content.replace(QRegularExpression("\\s+"), " ");
        content.replace(QRegularExpression("<!--.*?-->"), ""); // Remove comments
    } else {
        content.replace("><", ">\n<");
    }

    QFile dst(outputFile); // ИСПРАВЛЕНО: было outputPath, стало outputFile
    if (!dst.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot write output file."));
        return;
    }
    dst.write(content.toUtf8());
    dst.close();

    QMessageBox::information(this, tr("Success"), tr("HTML file processed successfully."));
}