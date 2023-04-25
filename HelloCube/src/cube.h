#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Cube : protected QOpenGLFunctions
{
public:
	Cube();
	virtual ~Cube();
	void drawCube(QOpenGLShaderProgram* program);

private:
	void initCube();

	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
};

#endif // CUBE_H
