#ifndef BOX_H
#define BOX_H

#include "glfunc.h"
#include <QOpenGLShaderProgram>

class Box : protected GLFuncName
{
public:
    Box();
    void Draw(const QOpenGLShaderProgram & program);
protected:
    void init();
private:
    void initTexture();
    void initBuf();
    GLuint VAO, VBO;
    GLuint texture;
};

#endif // BOX_H
