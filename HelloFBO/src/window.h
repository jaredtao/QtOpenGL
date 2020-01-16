#ifndef WINDOW_H
#define WINDOW_H

#include "cube.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWindow>
#include <QQuaternion>
#include <QVector2D>

#ifdef DEBUG_GL
#include <QOpenGLDebugLogger>
#endif

class Cube;

class Window : public QOpenGLWindow, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Window(QWindow *parent = nullptr);
    ~Window() override;

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    void initShaders();
    void initTextures();
    void initFBO();
    void initQuad();

private:
    void calcFPS();
    void updateFPS(qreal);
    void paintFPS();
    void useCubeShader();
    void useFBOShader();

private:
    QTimer timer;
    QOpenGLShaderProgram *cubeProgram = nullptr;
    QOpenGLShaderProgram *fboProgram = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    Cube *cube = nullptr;

    QOpenGLTexture *texture = nullptr;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
    qreal fps;
    GLuint fbo;
    GLuint rbo;
    GLuint fboTexture;
    GLuint quadVAO;
    GLuint quadVBO;
#ifdef DEBUG_GL
    QOpenGLDebugLogger *logger = nullptr;
#endif
};

#endif // WINDOW_H
