#include "window.h"
#include <QApplication>
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    Window w;
    w.show();


    return a.exec();
}
