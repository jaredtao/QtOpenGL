QT += core gui qml quick widgets

CONFIG += c++11

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

RESOURCES += \
    res/img.qrc \
    res/qml.qrc \
    res/shader.qrc

HEADERS += \
    src/shader.h \
    src/easy.h \
    src/render.h

SOURCES += \
    src/main.cpp \
    src/shader.cpp \
    src/render.cpp
