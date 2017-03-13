#include "render.h"
#include <QTime>
#include <QDebug>
#include <QThread>
#include <QPainter>
#include "easy.h"
Render::Render(): m_texture(0)
{
	fps = 60.0;
    angleInDegrees = 0;
}
Render::~Render()
{
	SafeDeletePtr(m_texture);
}

void Render::init()
{
	initializeOpenGLFunctions();
    initShaders();
    initData();
    initTextures();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}
void Render::setRotate(qreal x, qreal y, qreal z)
{
	mModelMatrix.setToIdentity();
//	mModelMatrix.translate(0, 0, -0.5);
//	if (x)
		mModelMatrix.rotate(x, 1, 0, 0);
//	if (y)
		mModelMatrix.rotate(y, 0, 1, 0);
//	if (z)
		mModelMatrix.rotate(z, 0, 0, 1);

}
qreal Render::getFPS()
{
	return fps;
}
void Render::initShaders()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertex.glsl")) {
        qDebug() << " add vertex shader file failed." << m_program.log();
		return ;
	}
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragment.glsl")) {
        qDebug() << " add fragment shader file failed." << m_program.log();
		return ;
	}
	m_program.link();
	m_program.bind();

	mMVPMatrixHandle	= m_program.uniformLocation("qt_ModelViewProjectionMatrix");
	mVerticesHandle		= m_program.attributeLocation("qt_Vertex");
	mColorsHandle		= m_program.attributeLocation("a_Color");
	mTexCoordHandle		= m_program.attributeLocation("qt_MultiTexCoord0");

}
void Render::initTextures()
{
	SafeDeletePtr(m_texture);
	m_texture = new QOpenGLTexture(QImage(":/img/s.jpg"));
	m_texture->setMinificationFilter(QOpenGLTexture::Linear);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Render::initData()
{
	vertices << QVector3D(-1, -1, 0.0f)
			 << QVector3D( 1, -1, 0.0f)
			 << QVector3D( 1, 1, 0.0f)
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

	mModelMatrix.setToIdentity();
	mProjectionMatrix.setToIdentity();
	mProjectionMatrix.frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10.0f);
}
void Render::paint()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program.bind();
	draw();
	m_program.release();

	calcFPS();
}

void Render::draw()
{
	//顶点
	m_program.enableAttributeArray(mVerticesHandle);
	m_program.setAttributeArray(mVerticesHandle, vertices.constData());
	//颜色
	m_program.enableAttributeArray(mColorsHandle);
	m_program.setAttributeArray(mColorsHandle, colors.constData());
	//纹理坐标
	m_program.enableAttributeArray(mTexCoordHandle);
	m_program.setAttributeArray(mTexCoordHandle, texcoords.constData());
	//	//MVP矩阵

    mModelMatrix.rotate(angleInDegrees, 0, 0, 1);
    angleInDegrees =  (angleInDegrees + 1 ) % 360;
	mMVPMatrix = mProjectionMatrix  * mViewMatrix * mModelMatrix;
	m_program.setUniformValue(mMVPMatrixHandle, mMVPMatrix);

	//纹理
	m_texture->bind();
	m_program.setUniformValue("qt_Texture0", 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
	m_texture->release();
	m_program.disableAttributeArray(mVerticesHandle);
	m_program.disableAttributeArray(mColorsHandle);
}

void Render::calcFPS()
{
	static QTime time;
	static int once = [=](){time.start(); return 0;}();
	Q_UNUSED(once)
	static int frame = 0;
	frame++;
	if (frame > 10) {
		qreal elasped = time.elapsed();
		updateFPS(frame * 1000.0/ elasped );
		time.restart();
		frame = 0;
	}
}
void Render::updateFPS(float v)
{
	fps = v;
//	qDebug() << "fps " << fps;
}

