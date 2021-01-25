#include "GLCommon.h"
#include "window.h"
#include <QGuiApplication>
int main(int argc, char *argv[])
{
    GLCommon::prepareApp();
    QGuiApplication a(argc, argv);
    GLCommon::prepareGL();
    Window win;
    win.show();
    return a.exec();
}
