#include "TWindow.h"
#include <QDebug>
#include <QMouseEvent>
TWindow::TWindow()
{
    resize(800, 600);

}

TWindow::~TWindow()
{
    makeCurrent();
    m_cube = nullptr;
    m_skyBox = nullptr;
#ifdef _DEBUG
    m_logger.stopLogging();
#endif
    doneCurrent();
}

void TWindow::initializeGL()
{
    initializeOpenGLFunctions();

#ifdef _DEBUG
    bool ok = m_logger.initialize();
    connect(&m_logger, &QOpenGLDebugLogger::messageLogged, [](const QOpenGLDebugMessage &debugMessage){
        qWarning() << debugMessage.message();
    });
    m_logger.startLogging();
    qWarning() << "debugger init " << ok;
#endif

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    initMatrix();

    m_skyBox = std::make_shared<SkyBox>();
    m_cube = std::make_shared<Cube>();
    startTimer(1000 / 60);
}

void TWindow::resizeGL(int w, int h)
{
    float aspect = float(w) / (h ? h : 1);
    const float zNear = 3.0f;
    const float zFar = 7.0f;
    const float fov = 45.0f;

    m_project.setToIdentity();
    m_project.perspective(fov, aspect, zNear, zFar);

}

void TWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(m_rotation);
    m_model = matrix;
    m_cube->draw(m_program, m_model, m_view, m_project);
}

void TWindow::timerEvent(QTimerEvent *)
{
    m_angularSpeed *= 0.99f;
    if (m_angularSpeed <= 0.01f) {
        m_angularSpeed = 0;
    } else {
        m_rotation = QQuaternion::fromAxisAndAngle(m_rotationAxis, m_angularSpeed) * m_rotation;
        update();
    }
}

void TWindow::mousePressEvent(QMouseEvent *event)
{
    m_mousePressPosition = QVector2D{event->localPos()};
}

void TWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QVector2D diff = QVector2D{event->localPos()} - m_mousePressPosition;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    float acc = diff.length() / 100.0f;
    m_rotationAxis = (m_rotationAxis * m_angularSpeed + n *acc).normalized();

    m_angularSpeed += acc;
}

void TWindow::initMatrix()
{
    m_model.setToIdentity();
    m_view.setToIdentity();
    m_view.lookAt({0, 0, 3.0}, {0, 0, 0}, {0, 1, 0});

    float aspect = float(width()) / (height() ? height() : 1);
    const float zNear = 3.0f;
    const float zFar = 7.0f;
    const float fov = 45.0f;

    m_project.setToIdentity();
    m_project.perspective(fov, aspect, zNear, zFar);
}
