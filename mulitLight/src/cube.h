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

	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	QVector<QVector3D> cubePositions;
	QVector<QVector2D> coords;
};

#endif // CUBE_H
