#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "box.h"
#include "glfunc.h"

class Window : public QOpenGLWidget, protected GLFuncName
{
    Q_OBJECT
public:
    Window(QOpenGLWidget *parent = 0);
    ~Window();

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShader();
    void initMatrix();

private:
    QOpenGLShaderProgram program;

    QSharedPointer<Box> box;
    QMatrix4x4 model, view, project;
};

#endif // WINDOW_H
