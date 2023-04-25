#include "window.h"

#include <QGuiApplication>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QPainter>
#include <QScreen>
#include <QSurfaceFormat>
#include <QTime>
#include <QtMath>

#ifdef Q_OS_ANDROID
#include <qopengles2ext.h>
#endif
Window::Window(QWindow* parent)
	: QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)
{
	setWidth(800);
	setHeight(600);
	fps = 60.0;
	qWarning() << __FUNCTION__;
}
Window::~Window()
{
	makeCurrent();
	delete texture;
	delete cube;

	delete cubeProgram;
	delete fboProgram;
#ifdef DEBUG_GL
	logger->stopLogging();
	delete logger;
#endif

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
	qWarning() << __FUNCTION__;
	initializeOpenGLFunctions();

	glClearColor(0, 0, 0, 1);
	//	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initShaders();
	useCubeShader();

	cube = new Cube;
	cube->initCube(cubeProgram);

	useFBOShader();

	initTextures();

	initFBO();
	useCubeShader();
	initQuad();
#ifdef DEBUG_GL
	logger = new QOpenGLDebugLogger(this);
	connect(logger, &QOpenGLDebugLogger::messageLogged, this, [&](const QOpenGLDebugMessage& debugMessage) {
		qDebug() << debugMessage.id() << debugMessage.type() << debugMessage.source() << debugMessage.message();
	});
	logger->initialize();
	logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
#endif
	int interval = 1000 / qApp->primaryScreen()->refreshRate();
	startTimer(interval);
	//	timer.start(12, this);
}
template <class T>
void logAndQuit(const T& t)
{
	qWarning() << t.log();
	qApp->exit();
}
void Window::initShaders()
{
	cubeProgram = new QOpenGLShaderProgram;
	cubeProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vshader.glsl");
	cubeProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fshader.glsl");

	if (!cubeProgram->link())
	{
		logAndQuit(*cubeProgram);
	}
	fboProgram = new QOpenGLShaderProgram;
	fboProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vFbo.glsl");
	fboProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fFbo.glsl");
	if (!fboProgram->link())
	{
		logAndQuit(*fboProgram);
	}
}
void Window::useCubeShader()
{
	if (!cubeProgram->bind())
	{
		logAndQuit(*cubeProgram);
	}
	program = cubeProgram;
}
void Window::useFBOShader()
{
	if (!fboProgram->bind())
	{
		logAndQuit(*fboProgram);
	}
	program = fboProgram;
}
void Window::initTextures()
{
	texture = new QOpenGLTexture(QImage(":/image/cube.png").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Window::initFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width(), height());

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		qWarning() << "Error ";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::initQuad()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
							 -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

							 -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	 -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	 1.0f, 1.0f
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}
	glBindVertexArray(0);
}

void Window::resizeGL(int w, int h)
{
	qWarning() << __FUNCTION__;
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
	qWarning() << __FUNCTION__;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	useCubeShader();

	glActiveTexture(GL_TEXTURE0);
	texture->bind();
	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	matrix.rotate(rotation);
	program->setUniformValue("mvp_matrix", projection * matrix);
	program->setUniformValue("texture", 0);
	cube->drawCube(program);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.1, 0.2, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	useFBOShader();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	program->setUniformValue("screenTexture", 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

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
