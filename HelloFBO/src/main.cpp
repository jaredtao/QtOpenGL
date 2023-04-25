#include <QGuiApplication>

#include "window.h"

int main(int argc, char* argv[])
{

	QCoreApplication::setApplicationName("cube");
	QCoreApplication::setApplicationVersion("0.1");

	QGuiApplication app(argc, argv);

	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
	format.setRenderableType(QSurfaceFormat::OpenGLES);
	format.setDepthBufferSize(24);
	format.setVersion(3, 1);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
	format.setSamples(8);
#else

	format.setDepthBufferSize(24);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
	format.setSamples(8);
#endif
#ifdef DEBUG_GL
	format.setOption(QSurfaceFormat::DebugContext);
#endif
	QSurfaceFormat::setDefaultFormat(format);

	Window win;
	win.show();
	return app.exec();
}
