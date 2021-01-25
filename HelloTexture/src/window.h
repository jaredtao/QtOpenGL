#ifndef WINDOW_H
#define WINDOW_H

#include "GLWindow.h"
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
class Window : public GLWindow
{
    Q_OBJECT
public:
    Window(QWindow *parent = 0);
    ~Window();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) override;
    void initShaders();
    void initTextures();

    void initData();
    void draw();

    void paintFPS();

private:
    QOpenGLShaderProgram program;
    QOpenGLTexture *texture = nullptr;

    QVector<QVector3D> vertices;
    QVector<QVector4D> colors;
    QVector<QVector2D> texcoords;
    int mMVPMatrixHandle = 0;
    int mVerticesHandle = 0;
    int mColorsHandle = 0;
    int mTexCoordHandle = 0;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mMVPMatrix;

    qreal angleInDegrees;

};

#endif // WINDOW_H
