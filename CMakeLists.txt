cmake_minimum_required(VERSION 3.16)
project(compressor VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(compressor
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    translator.cpp
    translator.h
    dialogs/aboutdialog.cpp
    dialogs/aboutdialog.h
    dialogs/aboutdialog.ui
    dialogs/languagedialog.cpp
    dialogs/languagedialog.h
    dialogs/languagedialog.ui
    resources/resources.qrc
)

# Указываем директорию проекта как путь для заголовочных файлов
target_include_directories(compressor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(compressor PRIVATE Qt6::Core Qt6::Widgets)

# Translations
set(TS_FILES
    translations/compressor_ru.ts
    translations/compressor_en.ts
)

qt_add_translations(compressor TS_FILES ${TS_FILES})

# Install paths
set_target_properties(compressor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)