#include "TWindow.h"

#include <QApplication>
#include <QSurfaceFormat>
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
	fmt.setVersion(3, 3);
	fmt.setDepthBufferSize(24);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
#ifdef _DEBUG
	fmt.setOption(QSurfaceFormat::DebugContext);
#endif
	QSurfaceFormat::setDefaultFormat(fmt);
	TWindow w;
	w.show();
	return app.exec();
}
