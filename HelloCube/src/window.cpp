#include "window.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTime>
#include <QtMath>

Window::Window(QWidget* parent)
	: QOpenGLWidget(parent)
	, cube(0)
	, texture(0)
	, angularSpeed(0)
{
	fps = 60.0;
}
Window::~Window()
{
	makeCurrent();
	delete texture;
	delete cube;
	doneCurrent();
}
void Window::mousePressEvent(QMouseEvent* e)
{
	// Save mouse press position
	mousePressPosition = QVector2D(e->localPos());
}
void Window::mouseReleaseEvent(QMouseEvent* e)
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
void Window::timerEvent(QTimerEvent*)
{
	// Decrease angular speed (friction)
	angularSpeed *= 0.99;

	// Stop rotation when speed goes below threshold
	if (angularSpeed < 0.01)
	{
		angularSpeed = 0.0;
	}
	else
	{
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
	initTextures();

	//	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	cube = new Cube;

	startTimer(12);
	//	timer.start(12, this);
}
void Window::initShaders()
{
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vshader.glsl"))
	{
		qDebug() << __FILE__ << __FUNCTION__ << " add vertex shader file failed.";
		close();
	}
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fshader.glsl"))
	{
		qDebug() << __FILE__ << __FUNCTION__ << " add fragment shader file failed.";
		close();
	}

	if (!program.link())
	{
		qDebug() << __FILE__ << __LINE__ << "program link failed";
		close();
	}
	if (!program.bind())
	{
		qDebug() << __FILE__ << __LINE__ << "program bind failed";
		close();
	}
}
void Window::initTextures()
{
	texture = new QOpenGLTexture(QImage(":/image/cube.png").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Window::resizeGL(int w, int h)
{
	// Calculate aspect ratio
	qreal aspect = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

	// Reset projection
	projection.setToIdentity();
	// Set perspective projection
	projection.perspective(fov, aspect, zNear, zFar);
}

void Window::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texture->bind();

	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	matrix.rotate(rotation);

	program.setUniformValue("mvp_matrix", projection * matrix);

	program.setUniformValue("texture", 0);

	cube->drawCube(&program);

	calcFPS();
	paintFPS();
}

void Window::calcFPS()
{
	static QTime time;
	static int	 once = [=]() {
		  time.start();
		  return 0;
	}();
	Q_UNUSED(once)
	static int frame = 0;
	if (frame++ > 100)
	{
		qreal elasped = time.elapsed();
		updateFPS(frame / elasped * 1000);
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
	//    QString str = QString("FPS:%1").arg(QString::number(fps, 'f', 3));
	//    this->setWindowTitle(str);
}
