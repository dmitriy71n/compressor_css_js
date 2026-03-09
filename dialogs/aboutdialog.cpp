#include "aboutdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QCoreApplication>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About Compressor"));
    setFixedSize(400, 300); // Or whatever size you prefer for this dialog

    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignCenter);

    // Try to load logo from embedded resources first
    QPixmap pixmap;
    if (pixmap.load(":/icons/logo.png")) {
        logoLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (pixmap.load("/opt/compressor/share/icons/logo.png")) {
        logoLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel->setText(tr("Logo")); // Fallback if no image found
    }

    QLabel* titleLabel = new QLabel(tr("<h2>Compressor</h2>"), this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* versionLabel = new QLabel(tr("Version 2.0"), this);
    versionLabel->setAlignment(Qt::AlignCenter);

    QLabel* authorLabel = new QLabel(tr("Author: Dmitriy Nosov"), this);
    authorLabel->setAlignment(Qt::AlignCenter);

    QLabel* developerLabel = new QLabel(tr("Developer: Qwen3-Coder"), this);
    developerLabel->setAlignment(Qt::AlignCenter);

    QLabel* licenseLabel = new QLabel(tr("This application is distributed under the GNU General Public License without any warranty."), this);
    licenseLabel->setAlignment(Qt::AlignCenter);
    licenseLabel->setWordWrap(true);

    layout->addStretch();
    layout->addWidget(logoLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(versionLabel);
    layout->addWidget(authorLabel);
    layout->addWidget(developerLabel);
    layout->addWidget(licenseLabel);
    layout->addStretch();

    setLayout(layout);
}

// --- Добавлена реализация деструктора ---
AboutDialog::~AboutDialog()
{
    // Здесь можно освобождать ресурсы, если нужно.
    // Для этого примера, деструктор может быть пустым,
    // но его *реализация* обязательна.
}