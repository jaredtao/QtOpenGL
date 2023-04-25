#ifndef WINDOW_H
#define WINDOW_H

#include "cube.h"

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

class Cube;

class Window
	: public QOpenGLWidget
	, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit Window(QWidget* parent = 0);
	~Window();

protected:
	void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void timerEvent(QTimerEvent* e) Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;

	void initShaders();
	void initTextures();

private:
	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();

private:
	QBasicTimer			 timer;
	QOpenGLShaderProgram program;
	Cube*				 cube;

	QOpenGLTexture* texture;

	QMatrix4x4 projection;

	QVector2D	mousePressPosition;
	QVector3D	rotationAxis;
	qreal		angularSpeed;
	QQuaternion rotation;
	qreal		fps;
};

#endif // WINDOW_H
