#include "mainWindow.h"
#include "qopenglext.h"
#include <QDebug>
static const QString sVertexShader = u8R"(
#version 330

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 qt_Vertex;

void main(void)
{
    gl_Position = vec4(qt_Vertex, 1.0);
}
)";

static const QString sFragmentShader = u8R"(
#version 330

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

uniform highp vec4 outColor0;
out vec4 fragColor;
void main(void)
{
    fragColor = outColor0;
}
)";
MainWindow::MainWindow(QWidget* parent)
	: Super(parent)
{
}

MainWindow::~MainWindow()
{
	makeCurrent();
#ifdef _DEBUG
	if (mLogger)
	{

		mLogger->stopLogging();
		delete mLogger;
		mLogger = nullptr;
	}
#endif
	doneCurrent();
}

void MainWindow::initializeGL()
{
	initializeOpenGLFunctions();
#ifdef _DEBUG
	mLogger = new QOpenGLDebugLogger(this);
	mLogger->initialize();
	connect(mLogger, &QOpenGLDebugLogger::messageLogged, this, [this](const QOpenGLDebugMessage& msg) { qWarning() << msg.message(); });
	mLogger->startLogging();
#endif

	glClearColor(0.2, 0.3, 0.4, 1);

	mProgram = new QOpenGLShaderProgram(this);
	if (!mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, sVertexShader))
	{
		qWarning() << mProgram->log();
		return;
	}
	if (!mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, sFragmentShader))
	{
		qWarning() << mProgram->log();
		return;
	}
	if (!mProgram->link())
	{
		qWarning() << mProgram->log();
		return;
	}
	initVertices();
	glGenBuffers(1, &mVBO);
	glGenVertexArrays(1, &mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * mVertices.size(), mVertices.constData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const uint32_t vertexLocation = 0;
	glBindVertexArray(mVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glEnableVertexAttribArray(vertexLocation);
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);

		glVertexAttribDivisor(vertexLocation, 1);
	}
	glBindVertexArray(0);

	startTimer(1000 / 60);
}

void MainWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	Super::resizeGL(w, h);
}

void MainWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.3, 0.4, 1);
	if (!mProgram->bind())
	{
		qWarning() << mProgram->log();
		return;
	}
	mProgram->setUniformValue("outColor0", mColor);
	glBindVertexArray(mVAO);
	{
		//		glDrawArrays(GL_TRIANGLE_FAN, 0, mVertices.size());
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, mVertices.size(), mCount);
	}
	glBindVertexArray(0);
}

void MainWindow::timerEvent(QTimerEvent* e)
{
	if (isVisible())
	{
		update();
	}
}

void MainWindow::initVertices()
{
	mCount		  = 1;
	qreal x		  = 0.01;
	qreal y		  = 0.01;
	qreal w		  = 0.03;
	qreal h		  = 0.02;
	qreal xOffset = 0.01;
	qreal yOffset = 0.01;
	mVertices.clear();
	for (int i = 0; i < mCount; i++)
	{
		mVertices << QVector3D(x, y, 0.0);
		mVertices << QVector3D(x + w, y, 0.0);
		mVertices << QVector3D(x + w, y + h, 0.0);
		mVertices << QVector3D(x, y + h, 0.0);
		x += xOffset;
		if (x + w >= 1.0)
		{
			x = 0.01;
			y += yOffset;
		}
	}
}

void MainWindow::updateVertices() { }
