QT += qml quick

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
    src/shader.h

SOURCES += \
    src/main.cpp \
    src/shader.cpp
