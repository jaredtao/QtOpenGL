#include "window.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

Window::Window(QWindow* parent)
	: GLWindow(NoPartialUpdate, parent)
	, materialV(nullptr)
	, materialF(nullptr)
	, lightF(nullptr)
	, lightV(nullptr)
	, cube(0)
	, texture(0)
	, angularSpeed(0)
{
}
Window::~Window()
{
	makeCurrent();
	delete materialF;
	delete materialV;
	delete lightF;
	delete lightV;
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

void Window::initializeGL()
{
	GLWindow::initializeGL();

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	initShaders();
	initTextures();
	initMatrixs();
	useLightShader();
	cube = new Cube(&program);
}
void Window::timerEvent(QTimerEvent* event)
{
	processTimeout();
	GLWindow::timerEvent(event);
}
void Window::processTimeout()
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
	}
}
void Window::useMaterialShader()
{
	program.removeAllShaders();

	if (!program.addShader(materialV))
	{
		qDebug() << "add vertex shader file failed.";
	}
	if (!program.addShader(materialF))
	{
		qDebug() << "add fragment shader file failed.";
	}
	if (!program.link())
	{
		qDebug() << "program link failed";
	}
	if (!program.bind())
	{
		qDebug() << "program bind failed";
	}
}
void Window::useLightShader()
{
	program.removeAllShaders();
	if (!program.addShader(lightV))
	{
		qDebug() << "add vertex shader file failed.";
	}
	if (!program.addShader(lightF))
	{
		qDebug() << "add fragment shader file failed.";
	}
	if (!program.link())
	{
		qDebug() << "program link failed" << program.log();
	}
	if (!program.bind())
	{
		qDebug() << "program bind failed" << program.log();
	}
}
void Window::initShaders()
{
	bool ret  = false;
	materialV = new QOpenGLShader(QOpenGLShader::Vertex);
	ret		  = materialV->compileSourceFile(":/shader/vshader.glsl");
	if (!ret)
	{
		qDebug() << "compile shader failed -1";
	}

	materialF = new QOpenGLShader(QOpenGLShader::Fragment);
	ret		  = materialF->compileSourceFile(":/shader/fshader.glsl");
	if (!ret)
	{
		qDebug() << "compile shader failed -2";
	}

	lightV = new QOpenGLShader(QOpenGLShader::Vertex);
	ret	   = lightV->compileSourceFile(":/shader/light.vsh");
	if (!ret)
	{
		qDebug() << "compile shader failed -3";
	}

	lightF = new QOpenGLShader(QOpenGLShader::Fragment);
	ret	   = lightF->compileSourceFile(":/shader/light.fsh");
	if (!ret)
	{
		qDebug() << "compile shader failed -4";
	}
}
void Window::initTextures()
{
	texture = new QOpenGLTexture(QImage(":/image/pu.png").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);
}
void Window::initMatrixs()
{
	resize(width(), height());
	lightPos = QVector3D(1.2f, 1.0f, 2.0f);
	view.setToIdentity();
	view.lookAt(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
	model.setToIdentity();

	lightColor = QVector3D(1.0f, 1.0f, 1.0f);
}
void Window::resizeGL(int w, int h)
{
	GLWindow::resizeGL(w, h);
	Q_ASSERT(h);
	glViewport(0, 0, w, h);
	float ratio	 = (float)w / h;
	float aspect = 45;
	projection.setToIdentity();
	projection.perspective(aspect, ratio, 1.0f, 100.0f);
}

void Window::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float angle = 0;
	angle += 10;
	float radian = qDegreesToRadians(angle);
	float ratio	 = sin(radian);
	lightPos	 = QVector3D(ratio * 2.0f, 0.1f, 1.0f);

	//    lightColor.setX(sin(qDegreesToRadians(angle)) * 2.0);
	//    lightColor.setY(sin(qDegreesToRadians(angle)) * 0.7);
	//    lightColor.setZ(sin(qDegreesToRadians(angle)) * 1.3);

	{
		useLightShader();
		program.setUniformValue("lightColor", lightColor);
		model.setToIdentity();

		model.translate(lightPos);
		int modelLoc   = program.uniformLocation("modelMat");
		int projectLoc = program.uniformLocation("projectMat");
		int viewLoc	   = program.uniformLocation("viewMat");
		program.setUniformValue(modelLoc, model);
		program.setUniformValue(projectLoc, projection);
		program.setUniformValue(viewLoc, view);

		cube->drawCube(&program);
	}

	{
		useMaterialShader();
		int viewPosLoc = program.uniformLocation("viewPos");
		program.setUniformValue(viewPosLoc, QVector3D(0.0f, 0.0f, 3.0f));

		int lightColorLoc = program.uniformLocation("lightColor");
		program.setUniformValue(lightColorLoc, lightColor);

		int lightPosLoc = program.uniformLocation("lightPos");
		program.setUniformValue(lightPosLoc, lightPos);

		glActiveTexture(GL_TEXTURE0);
		texture->bind();
		int texLoc = program.uniformLocation("texture");
		program.setUniformValue(texLoc, 0);

		model.setToIdentity();
		model.translate(0.0, 0.0, -3.0);
		model.rotate(rotation);
		int modelLoc   = program.uniformLocation("modelMat");
		int projectLoc = program.uniformLocation("projectMat");
		int viewLoc	   = program.uniformLocation("viewMat");
		program.setUniformValue(modelLoc, model);
		program.setUniformValue(projectLoc, projection);
		program.setUniformValue(viewLoc, view);

		cube->drawCube(&program);
	}

	GLWindow::paintGL();
	paintFPS();
}

void Window::paintFPS()
{
	QString str = QString("FPS:%1").arg(QString::number(mFps, 'f', 3));
	setTitle(str);
}
