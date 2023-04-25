#ifndef SHADER_H
#define SHADER_H

#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QQuickWindow>
#include <QTimer>
class Render
	: public QObject
	, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	Render(QRect rect);
	~Render();
	void setWindow(QQuickWindow* window);

	void init();
	void resize();
public slots:
	void paint();
	void setGemo(QRect);

private:
	void initShaders();
	void initTextures();

	void initData();
	void draw();

	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();

	QRect m_gemo;
	QSize m_viewportSize;

	QOpenGLShaderProgram m_program;
	QQuickWindow*		 m_window;

	QOpenGLTexture* texture;

	QVector<QVector3D> vertices;
	QVector<QVector4D> colors;
	QVector<QVector2D> texcoords;
	int				   mMVPMatrixHandle;
	int				   mVerticesHandle;
	int				   mColorsHandle;
	int				   mTexCoordHandle;

	QMatrix4x4 mModelMatrix;
	QMatrix4x4 mViewMatrix;
	QMatrix4x4 mProjectionMatrix;
	QMatrix4x4 mMVPMatrix;
	QTimer*	   timer;
	qreal	   angleInDegrees;
	qreal	   fps;
	bool	   isInited;
};
class Line : public QQuickPaintedItem
{
protected:
	void paint(QPainter* painter);
};
class Shader : public QQuickItem
{
	Q_OBJECT
public:
	Shader(QQuickItem* parent = 0);

protected:
	void timerEvent(QTimerEvent* event);

signals:
	void sizeChanged(QRect);
public slots:
	void sync();
	void cleanup();
	void recalcSize();
private slots:
	void handleWindowChanged(QQuickWindow* win);

private:
	Render* m_render;
	Line*	line;
};

#endif // SHADER_H
