#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QTimer>
class Window: public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	Window(QWidget *parent = 0);
	~Window();
protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) override;
	void initShaders();
	void initTextures();

	void initData();
	void draw();

	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();
private:
	QOpenGLShaderProgram program;
	QOpenGLTexture *texture;


	QVector<QVector3D> vertices;
	QVector<QVector4D> colors;
	QVector<QVector2D> texcoords;
	int mMVPMatrixHandle;
	int mVerticesHandle;
	int mColorsHandle;
	int mTexCoordHandle;

	QMatrix4x4 mModelMatrix;
	QMatrix4x4 mViewMatrix;
	QMatrix4x4 mProjectionMatrix;
	QMatrix4x4 mMVPMatrix;
	QTimer *timer;
	qreal angleInDegrees;
	qreal fps;
};

#endif // WINDOW_H
