#include "shader.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
int main(int argc, char* argv[])
{
	//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	qSetMessagePattern("log[%{file} %{function} %{line}] %{message}");
	QGuiApplication app(argc, argv);

	qmlRegisterType<Shader>("Shader", 1, 0, "Shader");
	QQuickView view;
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource(QUrl(QLatin1Literal("qrc:/qml/main.qml")));
	QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
	view.show();

	//    QQmlApplicationEngine engine;
	//    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
