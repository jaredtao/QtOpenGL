#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
class Cube : protected QOpenGLFunctions
{
public:
	Cube();
	virtual ~Cube();
	void drawCube(QOpenGLShaderProgram* program);

private:
	void initCube();

	void			initTexture();
	void			initVertex();
	QOpenGLBuffer	arrayBuf;
	QOpenGLBuffer	indexBuf;
	QOpenGLTexture* texture;
};

#endif // CUBE_H
