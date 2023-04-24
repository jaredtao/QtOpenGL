#include "mainWindow.h"
#include "qopenglext.h"
#include <QDebug>
static const QString sVertexShader = u8R"(
#version 330

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 qt_Vertex;

void main(void)
{
    gl_Position = vec4(qt_Vertex, 1.0);
}
)";

static const QString sFragmentShader = u8R"(
#version 330

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

uniform highp vec4 outColor0;
out vec4 fragColor;
void main(void)
{
    fragColor = outColor0;
}
)";
MainWindow::MainWindow(QWidget *parent) : Super(parent) { }

MainWindow::~MainWindow() { }

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2, 0.3, 0.4, 1);

    mProgram = new QOpenGLShaderProgram(this);
    if (!mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, sVertexShader)) {
        qWarning() << mProgram->log();
        return;
    }
    if (!mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, sFragmentShader)) {
        qWarning() << mProgram->log();
        return;
    }
    if (!mProgram->link()) {
        qWarning() << mProgram->log();
        return;
    }
    initVertices();
    glGenBuffers(1, &mVBO);
    glGenVertexArrays(1, &mVAO);

    const uint32_t vertexLocation = 0;
    glBindVertexArray(mVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(mVBO, sizeof(mVertices), mVertices.constData(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(vertexLocation, 1);
    }
    glBindVertexArray(0);
}

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    Super::resizeGL(w, h);
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.4, 1);
    if (!mProgram->bind()) {
        qWarning() << mProgram->log();
        return;
    }
    mProgram->setUniformValue("outColor0", mColor);
    glBindVertexArray(mVAO);
    {
        glDrawArraysInstanced(GL_TRIANGLES, 0, 4, mCount);
    }
    glBindVertexArray(0);
}

void MainWindow::initVertices()
{
    mCount = 1024;
    qreal w = 0.003;
    qreal h = 0.002;
    qreal x = 0.001;
    qreal y = 0.001;
    qreal xOffset = 0.001;
    qreal yOffset = 0.001;
    mVertices.clear();
    for (int i = 0; i < mCount; i++) {
        mVertices << QVector3D(x, y, 0.0);
        mVertices << QVector3D(x + w, y, 0.0);
        mVertices << QVector3D(x + w, y + h, 0.0);
        mVertices << QVector3D(x, y + h, 0.0);
        x += xOffset;
        if (x + w >= 1.0) {
            x = 0.001;
            y += yOffset;
        }
    }
}

void MainWindow::updateVertices() { }
