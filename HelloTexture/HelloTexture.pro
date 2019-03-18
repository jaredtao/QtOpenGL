QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = lesson01

TEMPLATE = app

HEADERS += \
    src/window.h

SOURCES += \
    src/main.cpp \
    src/window.cpp

RESOURCES += \
    res/img.qrc \
    res/shader.qrc

