#ifndef WINDOW_H
#define WINDOW_H

#include "cube.h"

#include "GLWindow.h"

#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuaternion>
#include <QTime>
#include <QTimer>
#include <QVector2D>

class Cube;

class Window : public GLWindow
{
    Q_OBJECT
public:
    explicit Window(QWindow *parent = 0);
    ~Window();

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    void initShaders();
    void initTextures();
    void initMatrixs();
    void useLightShader();
    void useMaterialShader();

private:
    void paintFPS();
    void timerEvent(QTimerEvent *event) override;
private slots:
    void processTimeout();
private:
    QOpenGLShaderProgram program;
    QOpenGLShader *materialV, *materialF;
    QOpenGLShader *lightF, *lightV;
    Cube *cube;

    QOpenGLTexture *texture;

    QMatrix4x4 projection;
    QMatrix4x4 model;
    QMatrix4x4 view;

    QVector3D lightPos;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;

    QVector3D lightColor;
    qreal angularSpeed;
    QQuaternion rotation;

};

#endif // WINDOW_H
