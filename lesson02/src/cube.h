#ifndef CUBE_H
#define CUBE_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Cube : protected QOpenGLFunctions
{
public:
    Cube();
    virtual ~Cube();
    void drawCube(QOpenGLShaderProgram *program);
private:
    void initCube();
    
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

};

#endif // CUBE_H
