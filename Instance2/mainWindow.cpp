#include "mainWindow.h"
#include "qopenglext.h"
#include <QDebug>
#include <QTime>
static const QString sVertexShader = u8R"(
#version 330

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 qt_Vertex;
layout (location = 1) in mat4 qt_InstanceMatrix;

uniform mat4 mvpMatrix;
void main(void)
{
    gl_Position = mvpMatrix * qt_InstanceMatrix *vec4(qt_Vertex, 1.0);
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
	connect(mLogger, &QOpenGLDebugLogger::messageLogged, this, [](const QOpenGLDebugMessage& msg) { qWarning() << msg.message(); });
	mLogger->startLogging();
#endif

	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	mMVPMat.setToIdentity();
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

	mVertices = {
		QVector3D { -0.0005f, -0.0005f, 1.0f },
		QVector3D { -0.0005f, 0.0005f, 1.0f },
		QVector3D { 0.0005f, 0.0005f, 1.0f },
		QVector3D { 0.0005f, -0.0005f, 1.0f },
	};
	float offset = 0.001f;
	int	  w		 = 100;
	int	  h		 = 100;
	for (int i = -w; i < w; i += 2)
	{
		for (int j = -h; j < h; j += 2)
		{
			qreal	   px = 1.0f * i / w + offset;
			qreal	   py = 1.0f * j / h + offset;
			QMatrix4x4 mat;
			mat.translate(px, py);
			mInstanceMats.append(mat);
		}
	}

	{
		glGenBuffers(1, &mVerticesVBO);
		glGenBuffers(1, &mInstanceVBO);
		glGenVertexArrays(1, &mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * mVertices.size(), mVertices.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QMatrix4x4) * mInstanceMats.size(), mInstanceMats.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		const uint32_t vertexLocation	 = 0;
		const uint32_t instanceLocation1 = 1;
		const uint32_t instanceLocation2 = 2;
		const uint32_t instanceLocation3 = 3;
		const uint32_t instanceLocation4 = 4;
		glBindVertexArray(mVAO);
		{
			glEnableVertexAttribArray(vertexLocation);
			glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
			glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);

			glEnableVertexAttribArray(instanceLocation1);
			glVertexAttribPointer(instanceLocation1, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)0);

			glEnableVertexAttribArray(instanceLocation2);
			glVertexAttribPointer(instanceLocation2, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)(sizeof(QVector4D)));

			glEnableVertexAttribArray(instanceLocation3);
			glVertexAttribPointer(instanceLocation3, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)(2 * sizeof(QVector4D)));

			glEnableVertexAttribArray(instanceLocation4);
			glVertexAttribPointer(instanceLocation4, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (void*)(3 * sizeof(QVector4D)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glVertexAttribDivisor(instanceLocation1, 1);
			glVertexAttribDivisor(instanceLocation2, 1);
			glVertexAttribDivisor(instanceLocation3, 1);
			glVertexAttribDivisor(instanceLocation4, 1);
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
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	if (!mProgram->bind())
	{
		qWarning() << mProgram->log();
		return;
	}
	mProgram->setUniformValue("outColor0", mColor);
	mProgram->setUniformValue("mvpMatrix", mMVPMat);
	glBindVertexArray(mVAO);
	{
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, mInstanceMats.size());
	}
	glBindVertexArray(0);
	calcFPS();
	paintFPS();
}

void MainWindow::timerEvent(QTimerEvent*)
{
	if (isVisible())
	{
		update();
	}
}

void MainWindow::calcFPS()
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
void MainWindow::updateFPS(qreal v)
{
	mFPS = v;
}
void MainWindow::paintFPS()
{
	setWindowTitle(QString("Instance 2 - FPS: %1").arg(QString::number(mFPS, 'f', 3)));
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
