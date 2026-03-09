#!/bin/bash

set -e

APP_NAME="compressor"
INSTALL_PREFIX="/opt/compressor"
# Путь к бинарнику относительно директории, откуда запускается скрипт
BINARY_PATH_SOURCE="./build/bin/$APP_NAME"
BIN_DIR="$INSTALL_PREFIX/bin"
LIB_DIR="$INSTALL_PREFIX/lib"
SHARE_DIR="$INSTALL_PREFIX/share"
TRANSLATIONS_DIR="$INSTALL_PREFIX/translations"

# Проверяем, существует ли бинарник
if [ ! -f "$BINARY_PATH_SOURCE" ]; then
    echo "Error: Binary file '$BINARY_PATH_SOURCE' not found. Did the build complete successfully?"
    exit 1
fi

echo "Ensuring installation directories exist..."
# Явно создаём каждую директорию
sudo mkdir -p "$BIN_DIR"
sudo mkdir -p "$LIB_DIR"
sudo mkdir -p "$SHARE_DIR/icons"
sudo mkdir -p "$TRANSLATIONS_DIR"

echo "Checking if directories were created..."
if [ ! -d "$BIN_DIR" ]; then
    echo "Error: Could not create $BIN_DIR"
    exit 1
fi

echo "Downloading and installing dart-sass..."
DART_SASS_URL="https://github.com/sass/dart-sass/releases/download/1.77.8/dart-sass-1.77.8-linux-x64.tar.gz"
DART_SASS_DIR="$LIB_DIR/dart-sass"
if [ ! -d "$DART_SASS_DIR" ]; then
    echo "Downloading dart-sass..."
    wget -O /tmp/dart-sass.tar.gz $DART_SASS_URL
    echo "Extracting dart-sass..."
    tar -xzf /tmp/dart-sass.tar.gz -C /tmp/
    sudo mv /tmp/dart-sass $DART_SASS_DIR
    echo "dart-sass installed to $DART_SASS_DIR"
else
    echo "dart-sass already exists at $DART_SASS_DIR"
fi

echo "Copying application binary..."
sudo cp "$BINARY_PATH_SOURCE" "$BIN_DIR/$APP_NAME"
sudo chmod +x "$BIN_DIR/$APP_NAME"

echo "Copying translation files..."
sudo cp ./translations/compressor_*.qm "$TRANSLATIONS_DIR/"

echo "Copying logo..."
sudo cp ./share/icons/logo.png "$SHARE_DIR/icons/"

echo "Deployment completed to $INSTALL_PREFIX"
