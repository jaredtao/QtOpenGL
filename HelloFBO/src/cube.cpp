#include "cube.h"
#include <QVector2D>
#include <QVector3D>

#define GET_ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
struct VertexData
{
	QVector3D position;
	QVector2D texCoord;
};

const QVector3D Coords[] = {
	// v1 front top left
	QVector3D(-1.0f, 1.0f, 1.0f),
	// v2 front top right
	QVector3D(1.0f, 1.0f, 1.0f),
	// v3 front bottom left
	QVector3D(-1.0f, -1.0f, 1.0f),
	// v4 front bottom right
	QVector3D(1.0f, -1.0f, 1.0f),

	// v5 back top left
	QVector3D(-1.0f, 1.0f, -1.0f),
	// v6 back top right
	QVector3D(1.0f, 1.0f, -1.0f),
	// v7 back bottom left
	QVector3D(-1.0f, -1.0f, -1.0f),
	// v8 back bottom right
	QVector3D(1.0f, -1.0f, -1.0f),
};
enum
{
	COORD_F_T_L = 0,
	COORD_F_T_R,
	COORD_F_B_L,
	COORD_F_B_R,

	COORD_B_T_L,
	COORD_B_T_R,
	COORD_B_B_L,
	COORD_B_B_R,
};
enum
{
	v1 = COORD_F_T_L,
	v2,
	v3,
	v4,
	v5,
	v6,
	v7,
	v8
};
Cube::Cube()
	: indexBuf(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();

	// Generate 2 VBOs
	arrayBuf.create();
	indexBuf.create();

	// Initializes cube geometry and transfers it to VBOs
}
Cube::~Cube()
{
	arrayBuf.destroy();
	indexBuf.destroy();
}

void Cube::initCube(QOpenGLShaderProgram* program)
{
#if 1
	// For cube we would need only 8 vertices but we have to
	// duplicate vertex for each face because texture coordinate
	// is different.
	VertexData vertices[] = {
		// Vertex data for face 0
		{ QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(0.0f, 0.0f) }, // v0
		{ QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.33f, 0.0f) }, // v1
		{ QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(0.0f, 0.5f) },  // v2
		{ QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f) },  // v3

		// Vertex data for face 1
		{ QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.0f, 0.5f) },   // v4
		{ QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f) }, // v5
		{ QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.0f, 1.0f) },	   // v6
		{ QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.33f, 1.0f) },  // v7

		// Vertex data for face 2
		{ QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f) }, // v8
		{ QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f) }, // v9
		{ QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.66f, 1.0f) },  // v10
		{ QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(1.0f, 1.0f) },  // v11

		// Vertex data for face 3
		{ QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f) }, // v12
		{ QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(1.0f, 0.0f) },	// v13
		{ QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.66f, 0.5f) },	// v14
		{ QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(1.0f, 0.5f) },	// v15

		// Vertex data for face 4
		{ QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f) }, // v16
		{ QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f) },	// v17
		{ QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(0.33f, 0.5f) },	// v18
		{ QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.66f, 0.5f) },	// v19

		// Vertex data for face 5
		{ QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f) },  // v20
		{ QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.66f, 0.5f) },   // v21
		{ QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.33f, 1.0f) }, // v22
		{ QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.66f, 1.0f) }   // v23
	};

	// Indices for drawing cube faces using triangle strips.
	// Triangle strips can be connected by duplicating indices
	// between the strips. If connecting strips have opposite
	// vertex order then last index of the first strip and first
	// index of the second strip needs to be duplicated. If
	// connecting strips have same vertex order then only last
	// index of the first strip needs to be duplicated.
	GLushort indices[] = {
		0,	1,	2,	3,	3,		// Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		4,	4,	5,	6,	7,	7,	// Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		8,	8,	9,	10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23		// Face 5 - triangle strip (v20, v21, v22, v23)
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//! [1]
	// Transfer vertex data to VBO 0
	arrayBuf.bind();
	arrayBuf.allocate(vertices, sizeof(vertices));

	// Transfer index data to VBO 1
	indexBuf.bind();
	indexBuf.allocate(indices, sizeof(indices));

	// Offset for position
	quintptr offset = 0;

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Offset for texture coordinate
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int texcoordLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordLocation);
	program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

	glBindVertexArray(0);
#else
	VertexData vertices[] = {
		// Vertex data for face 1, left
		{ Coords[v5], QVector2D(0.0f, 0.0f) },	// v5
		{ Coords[v1], QVector2D(0.33f, 0.0f) }, // v1
		{ Coords[v7], QVector2D(0.0f, 0.5f) },	// v7
		{ Coords[v3], QVector2D(0.33f, 0.5f) }, // v3

		// Vertex data for face 2, front
		{ Coords[v1], QVector2D(0.33f, 0.0f) }, // v1
		{ Coords[v2], QVector2D(0.66f, 0.0f) }, // v2
		{ Coords[v3], QVector2D(0.33f, 0.5f) }, // v3
		{ Coords[v4], QVector2D(0.66f, 0.5f) }, // v4

		// Vertex data for face 3, right
		{ Coords[v2], QVector2D(0.66f, 0.0f) }, // v2
		{ Coords[v6], QVector2D(1.0f, 0.0f) },	// v6
		{ Coords[v4], QVector2D(0.66f, 0.5f) }, // v4
		{ Coords[v8], QVector2D(1.0f, 0.5f) },	// v8

		// Vertex data for face 4, back
		{ Coords[v6], QVector2D(0.0f, 0.5f) },	// v6
		{ Coords[v5], QVector2D(0.33f, 0.5f) }, // v5
		{ Coords[v8], QVector2D(0.0f, 1.0f) },	// v8
		{ Coords[v7], QVector2D(0.33f, 1.0f) }, // v7

		// Vertex data for face 5, top
		{ Coords[v5], QVector2D(0.33f, 0.5f) }, // v5
		{ Coords[v6], QVector2D(0.66f, 0.5f) }, // v6
		{ Coords[v1], QVector2D(0.33f, 1.0f) }, // v1
		{ Coords[v2], QVector2D(0.66f, 1.0f) }, // v2

		// Vertex data for face 1, left
		{ Coords[v3], QVector2D(0.66f, 0.5f) }, // v5
		{ Coords[v4], QVector2D(1.0f, 0.5f) },	// v1
		{ Coords[v7], QVector2D(0.66f, 1.0f) }, // v7
		{ Coords[v8], QVector2D(1.0f, 1.0f) },	// v3
	};

	// FIXME why? why? why?
	GLushort indices[] = {
		0,	1,	2,	3,	3,		// Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		4,	4,	5,	6,	7,	7,	// Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		8,	8,	9,	10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23		// Face 5 - triangle strip (v20, v21, v22, v23)
	};
	arrayBuf.bind();
	arrayBuf.allocate(vertices, GET_ARRAY_LEN(vertices) * sizeof(VertexData));

	indexBuf.bind();
	indexBuf.allocate(indices, GET_ARRAY_LEN(indices) * sizeof(GLushort));
#endif
}
void Cube::drawCube(QOpenGLShaderProgram* program)
{

	glBindVertexArray(vao);
	// Draw cube geometry using indices from VBO 1
	glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
