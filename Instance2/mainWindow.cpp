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
layout (location = 1) in vec3 qt_InstanceVertex;

void main(void)
{
    gl_Position = vec4(qt_InstanceVertex, 1.0);
}
)";

static const QString sFragmentShader = u8R"(
#version 330

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

uniform vec4 outColor0;
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
	QVector<QVector3D> vertices = {
		QVector3D { -0.05, -0.05, 1.0 },
		QVector3D { -0.05, 0.05, 1.0 },
		QVector3D { 0.05, 0.05, 1.0 },
		QVector3D { 0.05, -0.05, 1.0 },
	};
	QVector<QVector3D> offsets;
	float			   offset = 0.1f;

	for (int i = -10; i < 10; i += 2)
	{
		for (int j = -10; j < 10; j += 2)
		{
			qreal x = i / 10.0f + offset;
			qreal y = j / 10.0f + offset;
			offsets << vertices.at(0) + QVector3D(x, y, 0.0);
			offsets << vertices.at(1) + QVector3D(x, y, 0.0);
			offsets << vertices.at(2) + QVector3D(x, y, 0.0);
			offsets << vertices.at(3) + QVector3D(x, y, 0.0);
		}
	}
	{
		glGenBuffers(1, &mVerticesVBO);
		glGenBuffers(1, &mInstanceVBO);
		glGenVertexArrays(1, &mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * vertices.size(), vertices.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * offsets.size(), offsets.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		const uint32_t vertexLocation = 0;
		const uint32_t offsetLocation = 1;
		glBindVertexArray(mVAO);
		{
			glEnableVertexAttribArray(vertexLocation);
			glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
			glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);

			glEnableVertexAttribArray(offsetLocation);
			glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
			glVertexAttribPointer(offsetLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glVertexAttribDivisor(offsetLocation, 1);
		}
		glBindVertexArray(0);
	}
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
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 100);
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
	//	mCount		  = 2;
	//	qreal x		  = 0.01;
	//	qreal y		  = 0.01;
	//	qreal w		  = 0.03;
	//	qreal h		  = 0.02;
	//	qreal xOffset = 0.01;
	//	qreal yOffset = 0.01;
	//	mVertices.clear();
	//	for (int i = 0; i < mCount; i++)
	//	{
	//		mVertices << QVector3D(x, y, 0.0);
	//		mVertices << QVector3D(x + w, y, 0.0);
	//		mVertices << QVector3D(x + w, y + h, 0.0);
	//		mVertices << QVector3D(x, y + h, 0.0);
	//		x += xOffset;
	//		if (x + w >= 1.0)
	//		{
	//			x = 0.01;
	//			y += yOffset;
	//		}
	//	}
}

void MainWindow::updateVertices() { }
