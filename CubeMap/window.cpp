#include "window.h"
#include <QOpenGLShader>
Window::Window(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
{
    this->resize(800, 600);
}

Window::~Window()
{

}


void Window::initializeGL()
{
    initializeOpenGLFunctions();
    initShader();
    initMatrix();
    box = QSharedPointer<Box> (new Box);
    //    glEnable(GL_DEPTH_TEST);
    //    glDepthFunc(GL_LESS);

}

void Window::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear =0.1f, zFar = 1000.0, fov = 45.0;

    // Reset projection
    project.setToIdentity();
    // Set perspective projection
    project.perspective(fov, aspect, zNear, zFar);
}

void Window::paintGL()
{
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    program.setUniformValue("view", view);
    program.setUniformValue("project", project);
    program.setUniformValue("model", model);
    box->Draw(program);

}
void Window::initShader()
{
    if(!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl")) {
        qDebug() << "add vertex shader failed" << program.log();
    }
    if(!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl")) {
        qDebug() << "add fragment shader failed" << program.log();
    }
    program.link();
    program.bind();
}

void Window::initMatrix()
{
    view.setToIdentity();
    view.lookAt(QVector3D(0, 0, 3.0),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0));
//        view.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0, 0), 45));

    model.setToIdentity();
    project.setToIdentity();
    project.perspective(45.0, (float)width() / height(), 0.1f, 100.0f);
}
