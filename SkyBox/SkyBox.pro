QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SkyBox
TEMPLATE = app

SOURCES += \
    src/Cube.cpp \
    src/SkyBox.cpp \
    src/TWindow.cpp \
    src/main.cpp

HEADERS += \
    src/Cube.h \
    src/GLHeaders.h \
    src/SkyBox.h \
    src/TWindow.h

RESOURCES += \
    Assets.qrc
