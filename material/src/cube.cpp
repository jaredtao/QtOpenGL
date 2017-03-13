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
    //v1 front top left
    QVector3D ( -1.0f, 1.0f, 1.0f),
    //v2 front top right
    QVector3D ( 1.0f,  1.0f, 1.0f),
    //v3 front bottom left
    QVector3D (-1.0f, -1.0f, 1.0f),
    //v4 front bottom right
    QVector3D ( 1.0f, -1.0f, 1.0f),

    //v5 back top left
    QVector3D ( -1.0f, 1.0f, -1.0f),
    //v6 back top right
    QVector3D ( 1.0f,  1.0f, -1.0f),
    //v7 back bottom left
    QVector3D (-1.0f, -1.0f, -1.0f),
    //v8 back bottom right
    QVector3D ( 1.0f, -1.0f, -1.0f),
};
enum {
    COORD_F_T_L = 0,
    COORD_F_T_R,
    COORD_F_B_L,
    COORD_F_B_R,

    COORD_B_T_L,
    COORD_B_T_R,
    COORD_B_B_L,
    COORD_B_B_R,
};
enum {
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
{
    initializeOpenGLFunctions();

    // Initializes cube geometry and transfers it to VBOs
    initCube();
}
Cube::~Cube()
{
}

void Cube::initCube()
{
    //back
    vertices << QVector3D(-0.5,  -0.5, -0.5f)
             << QVector3D( 0.5,  -0.5, -0.5f)
             << QVector3D( 0.5,   0.5, -0.5f)
             << QVector3D( 0.5,   0.5, -0.5f)
             << QVector3D(-0.5,   0.5, -0.5f)
             << QVector3D(-0.5,  -0.5, -0.5f);

    normals << QVector3D(0.0f, 0.0f, -1.0f)
            << QVector3D(0.0f, 0.0f, -1.0f)
            << QVector3D(0.0f, 0.0f, -1.0f)
            << QVector3D(0.0f, 0.0f, -1.0f)
            << QVector3D(0.0f, 0.0f, -1.0f)
            << QVector3D(0.0f, 0.0f, -1.0f);

    coords << QVector2D(0, 0)
           << QVector2D(1, 0)
           << QVector2D(1, 1)
           << QVector2D(1, 1)
           << QVector2D(0, 1)
           << QVector2D(0, 0);

    //front
    vertices << QVector3D(-0.5,  -0.5, 0.5f)
             << QVector3D( 0.5,  -0.5, 0.5f)
             << QVector3D( 0.5,   0.5, 0.5f)
             << QVector3D( 0.5,   0.5, 0.5f)
             << QVector3D(-0.5,   0.5, 0.5f)
             << QVector3D(-0.5,  -0.5, 0.5f);

    normals << QVector3D(0.0f, 0.0f, 1.0f)
            << QVector3D(0.0f, 0.0f, 1.0f)
            << QVector3D(0.0f, 0.0f, 1.0f)
            << QVector3D(0.0f, 0.0f, 1.0f)
            << QVector3D(0.0f, 0.0f, 1.0f)
            << QVector3D(0.0f, 0.0f, 1.0f);

    coords << QVector2D(0, 0)
           << QVector2D(1, 0)
           << QVector2D(1, 1)
           << QVector2D(1, 1)
           << QVector2D(0, 1)
           << QVector2D(0, 0);

    //left
    vertices << QVector3D(-0.5,   0.5,  0.5f)
             << QVector3D(-0.5,   0.5, -0.5f)
             << QVector3D(-0.5,  -0.5, -0.5f)
             << QVector3D(-0.5,  -0.5, -0.5f)
             << QVector3D(-0.5,  -0.5,  0.5f)
             << QVector3D(-0.5,   0.5,  0.5f);

    normals << QVector3D(-1.0f, 0.0f, 0.0f)
            << QVector3D(-1.0f, 0.0f, 0.0f)
            << QVector3D(-1.0f, 0.0f, 0.0f)
            << QVector3D(-1.0f, 0.0f, 0.0f)
            << QVector3D(-1.0f, 0.0f, 0.0f)
            << QVector3D(-1.0f, 0.0f, 0.0f);

    coords << QVector2D(1, 0)
           << QVector2D(1, 1)
           << QVector2D(0, 1)
           << QVector2D(0, 1)
           << QVector2D(0, 0)
           << QVector2D(1, 0);

    //right
    vertices << QVector3D(0.5,   0.5,  0.5f)
             << QVector3D(0.5,   0.5, -0.5f)
             << QVector3D(0.5,  -0.5, -0.5f)
             << QVector3D(0.5,  -0.5, -0.5f)
             << QVector3D(0.5,  -0.5,  0.5f)
             << QVector3D(0.5,   0.5,  0.5f);

    normals << QVector3D(1.0f, 0.0f, 0.0f)
            << QVector3D(1.0f, 0.0f, 0.0f)
            << QVector3D(1.0f, 0.0f, 0.0f)
            << QVector3D(1.0f, 0.0f, 0.0f)
            << QVector3D(1.0f, 0.0f, 0.0f)
            << QVector3D(1.0f, 0.0f, 0.0f);

    coords << QVector2D(1, 0)
           << QVector2D(1, 1)
           << QVector2D(0, 1)
           << QVector2D(0, 1)
           << QVector2D(0, 0)
           << QVector2D(1, 0);
    //down
    vertices << QVector3D(-0.5,  -0.5, -0.5f)
             << QVector3D( 0.5,  -0.5, -0.5f)
             << QVector3D( 0.5,  -0.5,  0.5f)
             << QVector3D( 0.5,  -0.5,  0.5f)
             << QVector3D(-0.5,  -0.5,  0.5f)
             << QVector3D(-0.5,  -0.5, -0.5f);

    normals << QVector3D(0.0f, -1.0f, 0.0f)
            << QVector3D(0.0f, -1.0f, 0.0f)
            << QVector3D(0.0f, -1.0f, 0.0f)
            << QVector3D(0.0f, -1.0f, 0.0f)
            << QVector3D(0.0f, -1.0f, 0.0f)
            << QVector3D(0.0f, -1.0f, 0.0f);

    coords << QVector2D(0, 1)
           << QVector2D(1, 1)
           << QVector2D(1, 0)
           << QVector2D(1, 0)
           << QVector2D(0, 0)
           << QVector2D(0, 1);

    //up
    vertices << QVector3D(-0.5,  0.5, -0.5f)
             << QVector3D( 0.5,  0.5, -0.5f)
             << QVector3D( 0.5,  0.5,  0.5f)
             << QVector3D( 0.5,  0.5,  0.5f)
             << QVector3D(-0.5,  0.5,  0.5f)
             << QVector3D(-0.5,  0.5, -0.5f);

    normals << QVector3D(0.0f, 1.0f, 0.0f)
            << QVector3D(0.0f, 1.0f, 0.0f)
            << QVector3D(0.0f, 1.0f, 0.0f)
            << QVector3D(0.0f, 1.0f, 0.0f)
            << QVector3D(0.0f, 1.0f, 0.0f)
            << QVector3D(0.0f, 1.0f, 0.0f);

    coords << QVector2D(0, 1)
           << QVector2D(1, 1)
           << QVector2D(1, 0)
           << QVector2D(1, 0)
           << QVector2D(0, 0)
           << QVector2D(0, 1);

}
void Cube::drawCube(QOpenGLShaderProgram *program)
{
    int verticesLoc	= program->attributeLocation("a_position");
    int coordLoc    = program->attributeLocation("a_texcoord");
    int normalLoc   = program->attributeLocation("a_normal");
    program->enableAttributeArray(verticesLoc);
    program->setAttributeArray(verticesLoc, vertices.constData());

    program->enableAttributeArray(coordLoc);
    program->setAttributeArray(coordLoc, coords.constData());

    program->enableAttributeArray(normalLoc);

    program->setAttributeArray(normalLoc, normals.constData());

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    program->disableAttributeArray(verticesLoc);
    program->disableAttributeArray(coordLoc);
    program->disableAttributeArray(normalLoc);
}
