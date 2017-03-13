#ifndef RENDER_H
#define RENDER_H

#include <QMatrix4x4>

#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

class Render : protected QOpenGLFunctions_3_3_Core
{
public:
	Render();
	~Render();

	void init();
	void paint();
	void setRotate(qreal x, qreal y, qreal z);

	qreal getFPS();
private:
	void initShaders();
	void initTextures();
	void initData();

	void draw();

	void calcFPS();
	void updateFPS(float);

	QOpenGLShaderProgram m_program;

	QOpenGLTexture *m_texture;

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

	qreal fps;
    int angleInDegrees;
};

#endif // RENDER_H
