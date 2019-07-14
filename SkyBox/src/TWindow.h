#pragma once

#include <QObject>
#include <QOpenGLWidget>
#include "GLHeaders.h"
#include "Cube.h"
#include "SkyBox.h"
class TWindow : public QOpenGLWidget, public GLFuncName
{
public:
    TWindow();
    ~TWindow() override;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void timerEvent(QTimerEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void initMatrix();
private:
    std::shared_ptr<Cube> m_cube = nullptr;
    std::shared_ptr<SkyBox> m_skyBox = nullptr;
    QOpenGLShaderProgram m_program;
    QMatrix4x4 m_model, m_view, m_project;
#ifdef _DEBUG
    QOpenGLDebugLogger m_logger;
#endif

    QVector2D m_mousePressPosition;
    QVector3D m_rotationAxis;
    float m_angularSpeed = 0;
    QQuaternion m_rotation;
};

