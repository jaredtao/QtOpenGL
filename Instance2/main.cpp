#include "mainWindow.h"
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

	qWarning() << "fmt version" << fmt.version();
	MainWindow w;
	w.showMaximized();

	return app.exec();
}
