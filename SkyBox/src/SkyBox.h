#pragma once
#include "GLHeaders.h"

class SkyBox
{
public:
    SkyBox(GLFuncName *func);
    ~SkyBox();
    void draw(QOpenGLShaderProgram &program, const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &project);
private:
    void initBuffer();
    void initShader();
    void initTexture();
private:
    GLFuncName *m_func;
    QOpenGLBuffer m_arrayBuf;
    QOpenGLShader m_vertexShader;
    QOpenGLShader m_fragmentShader;
    QImage m_images[6];
    quint32 m_cubeTexture;
    quint32 m_vao;
    int m_vertexCount = 0;
};

