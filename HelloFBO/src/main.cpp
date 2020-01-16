#include <QGuiApplication>

#include "window.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setApplicationName("cube");
    QCoreApplication::setApplicationVersion("0.1");

    QGuiApplication app(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();

    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setSamples(8);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);

#ifdef DEBUG_GL
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(format);

    Window win;
    win.show();
    return app.exec();
}
