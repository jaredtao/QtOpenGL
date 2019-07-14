#include "Cube.h"
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};
Cube::Cube(GLFuncName *func)
    : m_func(func)
    , m_indexBuf(QOpenGLBuffer::IndexBuffer)
    , m_vertexShader(QOpenGLShader::Vertex)
    , m_fragmentShader(QOpenGLShader::Fragment)
    , m_texture(QOpenGLTexture::Target2D)
{
    initBuffer();
    initShader();
    initTexture();
}
Cube::~Cube()
{
    m_arrayBuf.destroy();
    m_indexBuf.destroy();
    m_texture.destroy();
}
void Cube::initBuffer()
{
    m_arrayBuf.create();
    m_indexBuf.create();

    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-0.5f, -0.5f,  0.5f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 0.5f, -0.5f,  0.5f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-0.5f,  0.5f,  0.5f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 0.5f,  0.5f,  0.5f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 0.5f, -0.5f,  0.5f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 0.5f, -0.5f, -0.5f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 0.5f,  0.5f,  0.5f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 0.5f,  0.5f, -0.5f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 0.5f, -0.5f, -0.5f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 0.5f,  0.5f, -0.5f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-0.5f,  0.5f, -0.5f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-0.5f, -0.5f,  0.5f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-0.5f,  0.5f, -0.5f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-0.5f,  0.5f,  0.5f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 0.5f, -0.5f, -0.5f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-0.5f, -0.5f,  0.5f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 0.5f, -0.5f,  0.5f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-0.5f,  0.5f,  0.5f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 0.5f,  0.5f,  0.5f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-0.5f,  0.5f, -0.5f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 0.5f,  0.5f, -0.5f), QVector2D(0.66f, 1.0f)}  // v23
    };
    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };
    m_indexCount = sizeof indices / sizeof(indices[0]);

    m_func->glGenVertexArrays(1, &m_vao);
    m_func->glBindVertexArray(m_vao);

    m_indexBuf.bind();
    m_indexBuf.allocate(indices, sizeof indices);

    m_arrayBuf.bind();
    m_arrayBuf.allocate(vertices, sizeof vertices);

    const int positionLocation = 0;
    m_func->glEnableVertexAttribArray(positionLocation);
    m_func->glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof (VertexData), nullptr);

    const int texCoordLocation =  1;
    m_func->glEnableVertexAttribArray(texCoordLocation);
    m_func->glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof (QVector3D)));

    m_func->glBindVertexArray(0);
}

void Cube::initShader()
{
    const auto vShader = u8R"(#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
uniform mat4 mvp;
out vec2 vTexCoord;
void main()
{
    vTexCoord = texCoord;
    gl_Position = mvp * vec4(position, 1.0);
}
)";
    const auto fShader = R"(#version 330 core
uniform sampler2D cubeTexture;
in vec2 vTexCoord;
out vec4 outColor;
void main()
{
    outColor = texture(cubeTexture, vTexCoord);
}
)";
    if (!m_vertexShader.compileSourceCode(vShader)) {
        qWarning() << m_vertexShader.log();
    }
    if (!m_fragmentShader.compileSourceCode(fShader)) {
        qWarning() << m_fragmentShader.log();
    }
}
void Cube::initTexture()
{
    QImage img(":/assets/cube.png");
    m_texture.setData(img.mirrored());
    m_texture.setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture.setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture.setWrapMode(QOpenGLTexture::Repeat);
}
void Cube::draw(QOpenGLShaderProgram &program, const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &project)
{
    program.removeAllShaders();
    program.addShader(&m_vertexShader);
    program.addShader(&m_fragmentShader);
    if (!program.link()) {
        qWarning() << program.log();
        return;
    }
    if (!program.bind()) {
        qWarning() << program.log();
        return;
    }
    program.setUniformValue("mvp", project * view * model);

    m_texture.bind();
    program.setUniformValue("cubeTexture", 0);

    m_func->glBindVertexArray(m_vao);

    m_func->glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_SHORT, nullptr);
}
