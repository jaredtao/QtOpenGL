#-------------------------------------------------
#
# Project created by QtCreator 2016-09-22T10:04:40
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11 utf8_source

TEMPLATE = app
DEFINES += GL_DEBUG

HEADERS += \
    src/cube.h \
    src/window.h

SOURCES += \
    src/cube.cpp \
    src/main.cpp \
    src/window.cpp

RESOURCES += \
    res/shader.qrc \
    res/texture.qrc


include($$PWD/../Common/Common.pri)
