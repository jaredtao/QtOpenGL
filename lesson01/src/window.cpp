#include "window.h"
#include <time.h>
#include <QPainter>
#include <QTime>
#include <QKeyEvent>
Window::Window(QWidget *parent):
	QOpenGLWidget(parent)
{
	//	setAttribute(Qt::WA_PaintOnScreen);
	//	setAttribute(Qt::WA_NoSystemBackground);
	angleInDegrees = 0;
	timer = new QTimer(this);
	fps = 0;
    angleInDegrees += 0.3;
	connect(timer, &QTimer::timeout, [=](){
		update();

	});
    timer->start(1);
}
Window::~Window()
{
	delete timer;
}
void Window::initializeGL()
{
	initializeOpenGLFunctions();
	resize(width(), height());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	mModelMatrix.setToIdentity();
	initTextures();
	initShaders();
	initData();

}
void Window::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {

		case Qt::Key_A:
		{
			mModelMatrix.translate(-0.1, 0, 0);
			update();
			break;
		}
		case Qt::Key_D:
		{
			mModelMatrix.translate(0.1, 0, 0);
			update();
			break;
		}
		default: {
			break;
		}
	}
}
void Window::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.bind();

	mModelMatrix.rotate(angleInDegrees, 0.0f, 0.0f, 1.0f);
	draw();

	program.release();

	calcFPS();
	paintFPS();
}

void Window::draw()
{
	//顶点
	program.enableAttributeArray(mVerticesHandle);
	program.setAttributeArray(mVerticesHandle, vertices.constData());
	//颜色
	program.enableAttributeArray(mColorsHandle);
	program.setAttributeArray(mColorsHandle, colors.constData());
	//纹理坐标
	program.enableAttributeArray(mTexCoordHandle);
	program.setAttributeArray(mTexCoordHandle, texcoords.constData());
	//MVP矩阵
	mMVPMatrix = mProjectionMatrix  * mViewMatrix * mModelMatrix;
	program.setUniformValue(mMVPMatrixHandle, mMVPMatrix);

	//纹理
	texture->bind();
	program.setUniformValue("qt_Texture0", 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
	texture->release();
	program.disableAttributeArray(mVerticesHandle);
	program.disableAttributeArray(mColorsHandle);
}
void Window::initData()
{
	vertices << QVector3D(-1, -1, 0.0f)
			 << QVector3D(1, -1, 0.0f)
			 << QVector3D(1, 1, 0.0f)
			 << QVector3D(-1, 1, 0.0f);
	texcoords << QVector2D(0, 0)
			  << QVector2D(1, 0)
			  << QVector2D(1, 1)
			  << QVector2D(0, 1);
	colors << QVector4D(1.0f, 0.0f, 0.0f, 1.0f)
		   << QVector4D(0.0f, 0.0f, 1.0f, 1.0f)
		   << QVector4D(0.0f, 1.0f, 0.0f, 1.0f);
	mViewMatrix.setToIdentity();
	mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
}
void Window::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	float ratio = (float) w / h;
	float left = -ratio;
	float right = ratio;
	float bottom = -1.0f;
	float top = 1.0f;
	float n  = 1.0f;
	float f = 10.0f;
	mProjectionMatrix.setToIdentity();
	mProjectionMatrix.frustum(left, right, bottom, top, n, f);
}

void Window::calcFPS()
{
    static QTime time;
    static int once = [=](){time.start(); return 0;}();
    Q_UNUSED(once)
    static int frame = 0;
    if (frame++ > 100) {
        qreal elasped = time.elapsed();
        updateFPS(frame/ elasped * 1000);
        time.restart();
        frame = 0;
    }
}
void Window::updateFPS(qreal v)
{
	fps = v;
}
void Window::paintFPS()
{
	QPainter painter(this);
	painter.setPen(Qt::green);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.drawText(10, 10, QString("FPS:%1").arg(QString::number(fps, 'f', 3)));
}

void Window::initShaders()
{
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertex.vsh")) {
		qDebug() << __FILE__<<__FUNCTION__<< " add vertex shader file failed.";
		return ;
	}
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragment.fsh")) {
		qDebug() << __FILE__<<__FUNCTION__<< " add fragment shader file failed.";
		return ;
	}
	program.bindAttributeLocation("qt_Vertex", 0);
	program.bindAttributeLocation("a_Color", 1);
	program.bindAttributeLocation("qt_MultiTexCoord0", 2);
	program.link();
	program.bind();

	mMVPMatrixHandle	= program.uniformLocation("qt_ModelViewProjectionMatrix");
	mVerticesHandle		= program.attributeLocation("qt_Vertex");
	mColorsHandle		= program.attributeLocation("a_Color");
	mTexCoordHandle		= program.attributeLocation("qt_MultiTexCoord0");

}
void Window::initTextures()
{
	texture = new QOpenGLTexture(QImage(":/image/wood.jpg").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Linear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);
}
