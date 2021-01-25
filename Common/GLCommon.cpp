#include <QSurfaceFormat>
#include "GLCommon.h"
#include <QCoreApplication>
void GLCommon::prepareGL()
{
#ifdef Q_OS_ANDROID
    auto fmt = QSurfaceFormat::defaultFormat();
    fmt.setMajorVersion(3);
    fmt.setMinorVersion(0);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setRenderableType(QSurfaceFormat::OpenGLES);
#ifdef GL_DEBUG
    fmt.setOption(QSurfaceFormat::DebugContext);
#endif
    fmt.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(fmt);
#else
    auto fmt = QSurfaceFormat::defaultFormat();
    fmt.setMajorVersion(3);
    fmt.setMinorVersion(3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
#ifdef GL_DEBUG
    fmt.setOption(QSurfaceFormat::DebugContext);
#endif
    fmt.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(fmt);
#endif
}

void GLCommon::prepareApp()
{
    QCoreApplication::setOrganizationName("jaredtao");
    QCoreApplication::setOrganizationDomain("jaredtao.github.io");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
}
