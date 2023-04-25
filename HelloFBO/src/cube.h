#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

class Cube : protected QOpenGLExtraFunctions
{
public:
	Cube();
	virtual ~Cube();
	void initCube(QOpenGLShaderProgram* program);
	void drawCube(QOpenGLShaderProgram* program);

private:
	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
	GLuint		  vao;
};

#endif // CUBE_H
