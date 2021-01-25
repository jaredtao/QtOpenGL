QT += core gui

CONFIG += utf8_source
TEMPLATE = app

HEADERS += \
    src/window.h

SOURCES += \
    src/main.cpp \
    src/window.cpp

RESOURCES += \
    res/img.qrc \
    res/shader.qrc

include($$PWD/../Common/Common.pri)
