#include "floor.h"
#include <QVector2D>
#include <QVector3D>

#define GET_ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
struct VertexData
{
	QVector3D position;
	QVector2D texCoord;
};

Floor::Floor()
	: arrayBuffer(QOpenGLBuffer::VertexBuffer)
	, texture(0)
{
	initializeOpenGLFunctions();
	arrayBuffer.create();
	init();
}

Floor::~Floor()
{
	arrayBuffer.destroy();
	if (texture)
	{
		delete texture;
		texture = nullptr;
	}
}

void Floor::init()
{
	VertexData vertices[] = {
		{ QVector3D(5.0f, -0.5f, 5.0f), QVector2D(2.0f, 0.0f) },   { QVector3D(-5.0f, -0.5f, 5.0f), QVector2D(0.0f, 0.0f) },
		{ QVector3D(-5.0f, -0.5f, -5.0f), QVector2D(0.0f, 2.0f) },

		{ QVector3D(5.0f, -0.5f, 5.0f), QVector2D(2.0f, 0.0f) },   { QVector3D(-5.0f, -0.5f, -5.0f), QVector2D(0.0f, 2.0f) },
		{ QVector3D(5.0f, -0.5f, -5.0f), QVector2D(2.0f, 2.0f) },
	};
	arrayBuffer.bind();
	arrayBuffer.allocate(vertices, sizeof(vertices));

	texture = new QOpenGLTexture(QImage(":/image/w1.png").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);
}
void Floor::Draw(QOpenGLShaderProgram* program)
{
	arrayBuffer.bind();
	int vertexHandle = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexHandle);
	program->setAttributeBuffer(vertexHandle, GL_FLOAT, 0, 3, sizeof(VertexData));

	int texcoordHandle = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordHandle);
	program->setAttributeBuffer(texcoordHandle, GL_FLOAT, offsetof(VertexData, texCoord), 2, sizeof(VertexData));

	texture->bind();
	program->setUniformValue("texture", 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, arrayBuffer.size());
}
