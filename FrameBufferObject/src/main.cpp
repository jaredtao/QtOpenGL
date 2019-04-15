#include <QApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include "item.h"
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
//	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	qSetMessagePattern("log[%{file} %{function} %{line}] %{message}");
    QApplication app(argc, argv);

    qmlRegisterType<Item>("TaoItem", 1, 0, "TaoItem");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QLatin1Literal("qrc:/qml/main.qml")));
    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    view.show();

    return app.exec();
}
