#ifndef CUBE_H
#define CUBE_H


#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Cube : protected QOpenGLFunctions_3_3_Core
{
public:
    Cube();
    virtual ~Cube();
    void initCube(QOpenGLShaderProgram *program);
    void drawCube(QOpenGLShaderProgram *program);
private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    GLuint vao;
};

#endif // CUBE_H
