#ifndef WINDOW_H
#define WINDOW_H

#include "cube.h"

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QTime>
#include <QTimer>
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

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;

	void initShaders();
	void initTextures();
	void initDatas();
	void useLightShader();
	void useMaterialShader();
private slots:
	void processTimeout();

private:
	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();

private:
	QTimer				 timer;
	QOpenGLShaderProgram program;
	QOpenGLShader *		 materialV, *materialF;
	QOpenGLShader *		 lightF, *lightV;
	Cube*				 cube;

	QOpenGLTexture *textureMaterial, *textureSpecular;

	QVector<QVector3D> cubePositions;
	QVector<QVector3D> poitLightPositions;

	QMatrix4x4 projection;
	QMatrix4x4 model;
	QMatrix4x4 view;

	GLfloat shininess;

	QVector3D lightAmibent;
	QVector3D lightDiffuse;
	QVector3D lightSpecular;

	QVector3D lightColor;
	QVector3D lightDir;
	QVector2D mousePressPosition;
	QVector3D rotationAxis;

	qreal		angularSpeed;
	QQuaternion rotation;
	qreal		fps;
};

#endif // WINDOW_H
