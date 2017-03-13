#include "window.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QTime>

#include "floor.h"
#include "cube.h"
Window::Window(QWidget *parent):
    QOpenGLWidget(parent) ,
    cube(0),
    floor(0),
    angularSpeed(0)
{
    fps = 60.0;
}
Window::~Window()
{
	makeCurrent();
	delete cube;
	delete floor;
	doneCurrent();
}
void Window::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}
void Window::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
void Window::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;

    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        // Request an update
        update();
    }
}
void Window::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(0, 0, 0, 1);
	initShaders();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	cube = new Cube;
	floor= new Floor;

	startTimer(12);
}
void Window::initShaders()
{
	program.removeAllShaders();
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vshader.glsl")) {
		qDebug() << __FILE__<<__FUNCTION__<< " add vertex shader file failed.";
		close();
	}
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fshader.glsl")) {
		qDebug() << __FILE__<<__FUNCTION__<< " add fragment shader file failed.";
		close();
	}

	if (!program.link()) {
		qDebug() << __FILE__<<__LINE__<< "program link failed";
		close();
	}
	if (!program.bind()) {
		qDebug() << __FILE__<<__LINE__<< "program bind failed";
		close();
	}
}

void Window::useSiangleColorShader()
{
	program.removeAllShaders();
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vshader.glsl")) {
		qDebug() << __FILE__<<__FUNCTION__<< " add vertex shader file failed.";
		close();
	}
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/singleColor.glsl")) {
		qDebug() << __FILE__<<__FUNCTION__<< " add fragment shader file failed.";
		close();
	}

	if (!program.link()) {
		qDebug() << __FILE__<<__LINE__<< "program link failed";
		close();
	}
	if (!program.bind()) {
		qDebug() << __FILE__<<__LINE__<< "program bind failed";
		close();
	}
}


void Window::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear =0.1f, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();
    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void Window::paintGL()
{
	initShaders();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	QMatrix4x4 matrix, view;

	view.setToIdentity();
	view.lookAt(QVector3D(0.0f, 0.5f, 3.0f),
				QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(0.0f, 1.0f, 0.0f));
	view.rotate(QQuaternion::fromAxisAndAngle(1.0, 0, 0, 45));

	program.setUniformValue("mvp_matrix", projection * view);
	glStencilMask(0x00);
	floor->Draw(&program);

	{
		// 1st. Render pass, draw objects as normal, filling the stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		matrix.setToIdentity();
		matrix.translate(-0.5f, 0.0, -1.5f);
		matrix.rotate(rotation);

		program.setUniformValue("mvp_matrix", projection * view * matrix);
		cube->drawCube(&program);

		matrix.setToIdentity();
		matrix.translate(0.3f, 0.0f, 0.0f);
		//	matrix.rotate(rotation);
		program.setUniformValue("mvp_matrix", projection * view * matrix);
		cube->drawCube(&program);
	}
	{
		// 2nd. Render pass, now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are now not drawn, thus only drawing
		// the objects' size differences, making it look like borders.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		useSiangleColorShader();

		matrix.setToIdentity();
		matrix.translate(-0.5f, 0.0, -1.5f);
		matrix.rotate(rotation);
		matrix.scale(1.04);
		program.setUniformValue("mvp_matrix", projection * view * matrix);
		cube->drawCube(&program);

		matrix.setToIdentity();
		matrix.translate(0.3f, 0.0f, 0.0f);
		//	matrix.rotate(rotation);
		program.setUniformValue("mvp_matrix", projection * view * matrix);
		cube->drawCube(&program);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

	}

	calcFPS();
	paintFPS();

}

void Window::calcFPS()
{
    static QTime time;
    static int once = [=](){time.start(); return 0;}();
    Q_UNUSED(once)
    static int frame = 0;
    if (frame++ > 100) {
        qreal elasped = time.elapsed();
        updateFPS(frame/ elasped * 1000);
        time.restart();
        frame = 0;
    }
}
void Window::updateFPS(qreal v)
{
    fps = v;
}
void Window::paintFPS()
{
    QString str = QString("FPS:%1").arg(QString::number(fps, 'f', 3));
    this->setWindowTitle(str);
}


