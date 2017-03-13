#include "shader.h"
#include <QTime>
#include <QDebug>
#include <QThread>
#include <QPainter>
Render::Render(QRect rect) :  m_window(0), texture(0)
{
    m_gemo = rect;
    angleInDegrees = 0.9;
    fps = 60.0;
    isInited = false;
    init();
}
Render::~Render()
{

}
void Render::setGemo(QRect rect)
{
	qDebug() << rect;
    m_gemo = rect;
    //    resize();
}
void Render::resize()
{
    QRect rect = m_gemo;
    int w = rect.width();
    int h = rect.height();

	glViewport(rect.x(), h - rect.y(), w, h);
    qDebug() << rect;
    float ratio = (float) w / h;
    float left = -ratio;
    float right = ratio;
    float bottom = -1.0f;
    float top = 1.0f;
    float n  = 1.0f;
    float f = 10.0f;
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(left, right, bottom, top, n, f);
}
void Render::setWindow(QQuickWindow *window)
{
    m_window = window;
}
void Render::init()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    initTextures();
    initShaders();
    initData();
    resize();
    isInited = true;
}
void Render::initShaders()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertex.vsh")) {
        qDebug() << __FILE__<<__FUNCTION__<< " add vertex shader file failed.";
        return ;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragment.fsh")) {
        qDebug() << __FILE__<<__FUNCTION__<< " add fragment shader file failed.";
        return ;
    }
    m_program.bindAttributeLocation("qt_Vertex", 0);
    m_program.bindAttributeLocation("a_Color", 1);
    m_program.bindAttributeLocation("qt_MultiTexCoord0", 2);
    m_program.link();
    m_program.bind();

    mMVPMatrixHandle	= m_program.uniformLocation("qt_ModelViewProjectionMatrix");
    mVerticesHandle		= m_program.attributeLocation("qt_Vertex");
    mColorsHandle		= m_program.attributeLocation("a_Color");
    mTexCoordHandle		= m_program.attributeLocation("qt_MultiTexCoord0");

}
void Render::initTextures()
{
    texture = new QOpenGLTexture(QImage(":/img/s.jpg").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Render::initData()
{
    vertices << QVector3D(-1, -1, 0.0f)
             << QVector3D(1, -1, 0.0f)
             << QVector3D(1, 1, 0.0f)
             << QVector3D(-1, 1, 0.0f);
    texcoords << QVector2D(0, 0)
              << QVector2D(1, 0)
              << QVector2D(1, 1)
              << QVector2D(0, 1);
    colors << QVector4D(1.0f, 0.0f, 0.0f, 1.0f)
           << QVector4D(0.0f, 0.0f, 1.0f, 1.0f)
           << QVector4D(0.0f, 1.0f, 0.0f, 1.0f);
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mModelMatrix.setToIdentity();
}
void Render::draw()
{
    //顶点
    m_program.enableAttributeArray(mVerticesHandle);
    m_program.setAttributeArray(mVerticesHandle, vertices.constData());
    //颜色
    m_program.enableAttributeArray(mColorsHandle);
    m_program.setAttributeArray(mColorsHandle, colors.constData());
    //纹理坐标
    m_program.enableAttributeArray(mTexCoordHandle);
    m_program.setAttributeArray(mTexCoordHandle, texcoords.constData());
    //MVP矩阵
    mModelMatrix.rotate(angleInDegrees, 0, 0, 1);

    mMVPMatrix = mProjectionMatrix  * mViewMatrix * mModelMatrix;
    m_program.setUniformValue(mMVPMatrixHandle, mMVPMatrix);

    //纹理
    texture->bind();
    m_program.setUniformValue("qt_Texture0", 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
    texture->release();
    m_program.disableAttributeArray(mVerticesHandle);
    m_program.disableAttributeArray(mColorsHandle);
}

void Render::calcFPS()
{
    static QTime time;
    static int once = [=](){time.start(); return 0;}();
    Q_UNUSED(once)
    static int frame = 0;
    frame++;
    if (frame > 100) {
        qreal elasped = time.elapsed();
        updateFPS(frame/ (elasped / 1000));
        time.restart();
        frame = 0;
    }
}
void Render::updateFPS(qreal v)
{
    fps = v;
}
void Render::paintFPS()
{
    static int count = 0;
    count++;
    if (count ==100) {
        qDebug() << fps;
        count = 0;
    }

}

void Render::paint()
{
	qDebug() << m_window->geometry() << m_gemo;
	glViewport(m_gemo.x(), m_window->height() - m_gemo.bottom(), m_gemo.width(), m_gemo.height());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();

    draw();

    m_program.release();

    calcFPS();
    paintFPS();
    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}
Shader::Shader(QQuickItem *parent):QQuickItem(parent),m_render(0), line(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
        startTimer(10);
}
void Shader::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    if (window())
        window()->update();
    if (line)
        line->update();
}
void Shader::sync()
{
    if (!m_render) {
        QRect rect = this->boundingRect().toRect();
        QRect target;
        target.setTopLeft(mapToScene(rect.topLeft()).toPoint());
        target.setBottomRight(mapToScene(rect.bottomRight()).toPoint());

        m_render = new Render(target);
        connect(window(), SIGNAL(beforeRendering()), m_render, SLOT(paint()), Qt::DirectConnection);
        connect(this, SIGNAL(xChanged()), this, SLOT(recalcSize()), Qt::DirectConnection);
        connect(this, SIGNAL(yChanged()), this, SLOT(recalcSize()), Qt::DirectConnection);
        connect(this, SIGNAL(widthChanged()), this, SLOT(recalcSize()), Qt::DirectConnection);
        connect(this, SIGNAL(heightChanged()), this, SLOT(recalcSize()), Qt::DirectConnection);
        connect(this, SIGNAL(sizeChanged(QRect)), m_render, SLOT(setGemo(QRect)), Qt::DirectConnection);
    }
    if (!line) {
        line = new Line;
        line->setParentItem(this->parentItem());
        line->setX(this->x());
        line->setY(this->y());
        line->setWidth(this->width());
        line->setHeight(this->height());
    }
    if (window())
        m_render->setWindow(window());
    //    QRect rect(this->x(), this->y(), this->width(), this->height());
    //    m_render->setGemo(rect);
}
void Shader::recalcSize()
{
    QRect rect = this->boundingRect().toRect();
    QRect target;
    target.setTopLeft(mapToScene(rect.topLeft()).toPoint());
    target.setBottomRight(mapToScene(rect.bottomRight()).toPoint());

    emit sizeChanged(target);
    //    m_render->setGemo(target);

}
void Shader::cleanup()
{
    if (m_render) {
        delete m_render;
        m_render = 0;
    }
    if (line) {
        delete line;
        line = 0;
    }
}

void Shader::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeRendering()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}
void Line::paint(QPainter *painter)
{
    static int num = 0;
    painter->save();
    num++;
    painter->drawText(0, 200, QString("%1").arg(num));

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(14);
    painter->setPen(pen);
    painter->drawLine(0, 0, 100, 100);


    painter->restore();
}
