#include "GLWindow.h"
#include <QDebug>
#include <QSurfaceFormat>
GLWindow::GLWindow(QOpenGLWindow::UpdateBehavior updateBehavior, QWindow* parent)
	: QOpenGLWindow(updateBehavior, parent)
#ifdef GL_DEBUG
	, mDebugger(this)
#endif
{
	resize(1280, 760);
	startTimer(QSurfaceFormat::defaultFormat().swapInterval());
}

GLWindow::GLWindow(QOpenGLContext* shareContext, QOpenGLWindow::UpdateBehavior updateBehavior, QWindow* parent)
	: QOpenGLWindow(shareContext, updateBehavior, parent)
#ifdef GL_DEBUG
	, mDebugger(this)
#endif
{
	resize(1280, 760);
	startTimer(QSurfaceFormat::defaultFormat().swapInterval());
}

GLWindow::~GLWindow()
{
	makeCurrent();
#ifdef GL_DEBUG
	if (mDebugger.isLogging())
	{
		mDebugger.stopLogging();
	}
#endif
	doneCurrent();
}

void GLWindow::initializeGL()
{
	initializeOpenGLFunctions();
#ifdef GL_DEBUG
	mDebugger.initialize();
	connect(&mDebugger, &QOpenGLDebugLogger::messageLogged, this, &GLWindow::onDebuggerMessage);
	mDebugger.startLogging();
#endif
	mElapsed.start();
	mFrame = 0;
}

void GLWindow::resizeGL(int w, int h)
{
	resize(w, h);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void GLWindow::paintGL()
{
	calcFPS();
}

void GLWindow::calcFPS()
{
	mFrame++;
	if (mFrame > 100)
	{
		auto cost = mElapsed.elapsed();
		updateFPS(1.0 * mFrame / cost * 1000);
		mFrame = 0;
		mElapsed.restart();
	}
}
void GLWindow::updateFPS(qreal v)
{
	mFps = v;
}

void GLWindow::timerEvent(QTimerEvent*)
{
	update();
}
#ifdef GL_DEBUG
void GLWindow::onDebuggerMessage(const QOpenGLDebugMessage& debugMessage)
{
	qWarning() << debugMessage.message();
}
#endif
