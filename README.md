# Compressor

A multi-functional tool for minifying, formatting, concatenating, and converting web assets (HTML, CSS, JS, SASS).

## Features

- Minify HTML, CSS, JS files into single-line formats (e.g., `file.min.css`).
- Format minified files back to readable formats (e.g., `file.fmt.css`).
- Convert SASS/SCSS files to CSS.
- Process combined HTML+CSS+JS files.
- Concatenate multiple CSS files alphabetically into one.
- Multilingual interface (Russian, English).

Compressor uses Qt6 libraries.

## Installation on Windows 10 / 11
– Open the Compressor folder

– Run compressor.exe

## Installation on Linux

Debian / Ubuntu / Mint
- sudo apt install libqt6widgets6

Arch Linux
- sudo pacman -S qt6-base

Fedora
- sudo dnf install qt6-qtbase


unzip compressor_2.0_linux.zip

cp compressor/compressor.desktop ~/.local/share/applications/

sudo cp -r compressor/ /opt

sudo chown -R root:root /opt/compressor

sudo chmod -R 755 /opt/compressor

/opt/compressor/bin/./compressor

If the Compressor shortcut does not appear in the menu, restart your graphical shell session.

Run /opt/compressor/bin/./compressor.

If you encounter the error:

error while loading shared libraries: libQt6Widgets.so.6: cannot open shared object file: No such file or directory
Check if the libraries libqt6widgets6 or qt6-base are installed.

