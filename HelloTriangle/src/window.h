#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTime>
class Window : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	Window(QWidget *parent = 0);
	~Window();
protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void timerEvent(QTimerEvent *) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();
	void checkKey();
private:
	void makeObject();
	void initShader();
	void initTexture();

	QOpenGLShaderProgram *program;
	QVector<QVector3D> vertices;
	QVector<QVector3D>	cubePositions;
	QVector<QVector2D>  coords;

	QVector3D cameraPos ;
	QVector3D cameraFront ;
	QVector3D cameraUp;

	QMatrix4x4  modelMat, viewMat, projectMat;
	QOpenGLTexture *texture1, *texture2;
	int verticesHandle, coordHandle;
	qreal mixPara;
	QTime m_time;
	qreal fps;
	qreal elapsed;
	qreal yaw, pitch;
	qreal aspect;
	bool keys[1024];
	QPoint last;
};

#endif // WINDOW_H
