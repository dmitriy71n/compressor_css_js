#!/bin/bash

set -e

BUILD_DIR="build"
INSTALL_PREFIX="/opt/compressor"

echo "Creating build directory..."
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo "Running cmake..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX

echo "Building project..."
make -j$(nproc)

echo "Compiling translations..."
# Используем lupdate из Qt6
/usr/lib/qt6/bin/lupdate ../mainwindow.h ../mainwindow.cpp ../dialogs/*.h ../dialogs/*.cpp -ts ../translations/compressor_en.ts ../translations/compressor_ru.ts
# Используем lrelease из Qt6
/usr/lib/qt6/bin/lrelease ../translations/compressor_*.ts

echo "Installing to $INSTALL_PREFIX..."
make install

echo "Build completed successfully."
