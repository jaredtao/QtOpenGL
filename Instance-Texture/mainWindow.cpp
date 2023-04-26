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
layout (location = 1) in vec2 qt_MultiTexCoord0;
layout (location = 2) in mat4 qt_InstanceMatrix;

uniform mat4 mvpMatrix;
out vec2 qt_TexCoord0;
void main(void)
{
	qt_TexCoord0 = qt_MultiTexCoord0;
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

in vec2 qt_TexCoord0;
uniform sampler2D qt_Texture0;

out vec4 fragColor;
void main(void)
{
    fragColor = texture(qt_Texture0, qt_TexCoord0);
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
static QOpenGLTexture* genTexture(const QString& path)
{
	auto img	 = QImage(path).mirrored();
	auto texture = new QOpenGLTexture(img);
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Nearest);
	texture->setWrapMode(QOpenGLTexture::Repeat);
	return texture;
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

	float offset = 0.06f;
	int	  w		 = 20;
	int	  h		 = 20;

#ifdef CONUTERCLOCKWISE
	mVertices = {
		QVector3D { -offset / 2, -offset / 2, 1.0f },
		QVector3D { offset / 2, -offset / 2, 1.0f },
		QVector3D { offset / 2, offset / 2, 1.0f },
		QVector3D { -offset / 2, offset / 2, 1.0f },
	};
	mTexCoords = {
		QVector2D { 0, 0 },
		QVector2D { 1, 0 },
		QVector2D { 1, 1 },
		QVector2D { 0, 1 },
	};
#else
	mVertices = {
		QVector3D { -offset / 2, -offset / 2, 1.0f },
		QVector3D { -offset / 2, offset / 2, 1.0f },
		QVector3D { offset / 2, offset / 2, 1.0f },
		QVector3D { offset / 2, -offset / 2, 1.0f },
	};
	mTexCoords = {
		QVector2D { 0, 0 },
		QVector2D { 0, 1 },
		QVector2D { 1, 1 },
		QVector2D { 1, 0 },
	};
#endif

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
	mTexture = genTexture(":/pu.png");
	{
		glGenBuffers(1, &mVerticesVBO);
		glGenBuffers(1, &mInstanceVBO);
		glGenBuffers(1, &mTexCoordsVBO);
		glGenVertexArrays(1, &mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * mVertices.size(), mVertices.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D) * mTexCoords.size(), mTexCoords.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(QMatrix4x4) * mInstanceMats.size(), mInstanceMats.constData(), GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		const uint32_t vertexLocation	 = 0;
		const uint32_t texCoordLocation	 = 1;
		const uint32_t instanceLocation1 = 2;
		const uint32_t instanceLocation2 = 3;
		const uint32_t instanceLocation3 = 4;
		const uint32_t instanceLocation4 = 5;
		glBindVertexArray(mVAO);
		{
			glEnableVertexAttribArray(vertexLocation);
			glBindBuffer(GL_ARRAY_BUFFER, mVerticesVBO);
			glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);

			glEnableVertexAttribArray(texCoordLocation);
			glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsVBO);
			glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), (void*)0);

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
	mTexture->bind();
	mProgram->setUniformValue("mvpMatrix", mMVPMat);
	mProgram->setUniformValue("qt_Texture0", 0);
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
