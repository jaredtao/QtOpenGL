#include <QApplication>
#include "window.h"
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

//	QSurfaceFormat fmt;
//	fmt.setDepthBufferSize(24);
//	fmt.setMajorVersion(3);
//	fmt.setMinorVersion(3);
//	fmt.setProfile(QSurfaceFormat::CoreProfile);
//	QSurfaceFormat::setDefaultFormat(fmt);

	Window win;
	win.show();
	return a.exec();
}
