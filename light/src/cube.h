#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

class Cube : protected QOpenGLExtraFunctions
{
public:
	Cube(QOpenGLShaderProgram* program);
	virtual ~Cube();
	void drawCube(QOpenGLShaderProgram* program);

private:
	void			   initCube();
	void			   bindCube(QOpenGLShaderProgram* program);
	GLuint			   vao;
	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	QVector<QVector3D> cubePositions;
	QVector<QVector2D> coords;
};

#endif // CUBE_H
