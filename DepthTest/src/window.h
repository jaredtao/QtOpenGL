﻿#ifndef WINDOW_H
#define WINDOW_H

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

class Cube;
class Floor;
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

private:
	void useSiangleColorShader();
	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();

private:
	QBasicTimer			 timer;
	QOpenGLShaderProgram program;
	Cube*				 cube;
	Floor*				 floor;
	QMatrix4x4			 projection;

	QVector2D	mousePressPosition;
	QVector3D	rotationAxis;
	qreal		angularSpeed;
	QQuaternion rotation;
	qreal		fps;

	GLuint fbo;
};

#endif // WINDOW_H
