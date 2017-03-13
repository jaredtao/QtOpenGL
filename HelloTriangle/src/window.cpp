#include "window.h"
#include <QtMath>
#include <QKeyEvent>
#include <time.h>
#include <QPainter>
#include <QWheelEvent>
Window::Window(QWidget *parent)
	: QOpenGLWidget(parent),
	  program(0),
	  texture1(0),
	  texture2(0)
{
	resize(600, 400);
	startTimer(1);
	qsrand(time(0));
	m_time.start();
	fps = 60;
	elapsed = 1;
	yaw = -90;
	pitch = 0;
	aspect = 45;
}
void Window::timerEvent(QTimerEvent *)
{
	update();

}
void Window::mousePressEvent(QMouseEvent *event)
{
	last = event->pos();
}
void Window::mouseMoveEvent(QMouseEvent *event)
{
	QPointF diff = QPointF(0, 0);

	diff = event->pos() - last;
	last = event->pos();

	qreal sensitivity = 0.05;
	qreal xoffset =  diff.x() * sensitivity;
	qreal yoffset = -diff.y() * sensitivity;
	yaw   += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else  if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	QVector3D front;
	front.setX(cos(qDegreesToRadians(pitch)) * cos( qDegreesToRadians(yaw)));
	front.setY(sin(qDegreesToRadians(pitch)));
	front.setZ(cos(qDegreesToRadians(pitch)) * sin(qDegreesToRadians(yaw)));
	cameraFront = front.normalized();

	event->accept();
}

void Window::wheelEvent(QWheelEvent *event)
{
	int offset = event->angleDelta().y()< 0 ? -1 : 1;
	qreal speed = 10;
	if (1<= aspect + offset * speed && aspect + offset * speed <= 45) {
		aspect = aspect + offset * speed;
	}
	event->accept();
}
void Window::keyPressEvent(QKeyEvent *event)
{
    if (0 <= event->key() && event->key() < (int)(sizeof(keys)/ sizeof(keys[0]))) {
		keys[event->key()] = true;
	}
	if (event->key() == Qt::Key_F5) {
		modelMat.setToIdentity();
		viewMat.setToIdentity();
		projectMat.setToIdentity();

		cameraPos  = QVector3D(0.0f, 0.0f, 3.0f);
		cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
		cameraUp  = QVector3D(0.0f, 1.0f, 0.0f);

		elapsed = 1;
		yaw = -90;
		pitch = 0;
		aspect = 45;
		update();
	}

	event->accept();
}
void Window::keyReleaseEvent(QKeyEvent *event)
{
    if (0 <= event->key() && event->key() < (int)(sizeof(keys)/ sizeof(keys[0]))) {
		keys[event->key()] = false;
	}
	event->accept();
}
//checkKey can real time check key, so player can move left and front one time.
void Window::checkKey()
{
	GLfloat cameraSpeed  = 0.05 * elapsed/ 1000.0;
	if (keys[Qt::Key_1]) {
		mixPara += 0.1;
		if (mixPara > 1) {
			mixPara = 1;
		}
		qDebug() << "key 1 , mix + 0.1";
		update();
	}
	if (keys[Qt::Key_2]) {
		mixPara -= 0.1;
		if (mixPara < 0) {
			mixPara = 0;
		}
		qDebug() << "key 2, mix - 0.1";
		update();
	}
	if (keys[Qt::Key_A]) {
		cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
		qDebug() << "key a, left";
		update();
	}
	if (keys[Qt::Key_D])  {
		cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
		qDebug() << "key d, right";
		update();
	}
	if(keys[Qt::Key_W]) {
		cameraPos += cameraSpeed * cameraFront;
		qDebug() << "key w, front";
		update();
	}
	if (keys[Qt::Key_S])  {
		cameraPos -= cameraSpeed * cameraFront;
		qDebug() << "key s, back";
		update();
	}

}
Window::~Window()
{
	makeCurrent();
	delete program;
	delete	texture1;
	doneCurrent();
}
void Window::initializeGL()
{
	initializeOpenGLFunctions();
	resize(width(), height());
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	mixPara = 0.0;
	cameraPos	 = QVector3D(0.0f, 0.0f, 3.0f);
	cameraFront	 = QVector3D(0.0f, 0.0f, -1.0f);
	cameraUp	 = QVector3D(0.0f, 1.0f, 0.0f);

	for (auto & i : keys) {
		i = false;
	}
	modelMat.setToIdentity();
	viewMat.setToIdentity();
	projectMat.setToIdentity();
	initTexture();
	initShader();
	makeObject();
}
void Window::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkKey();
	program->bind();

	glActiveTexture(GL_TEXTURE0);
	texture1->bind();
	program->setUniformValue("u_texture1", 0);

	glActiveTexture(GL_TEXTURE1);
	texture2->bind();
	program->setUniformValue("u_texture2", 1);

	program->enableAttributeArray(verticesHandle);
	program->setAttributeArray(verticesHandle,vertices.constData());

	program->enableAttributeArray(coordHandle);
	program->setAttributeArray(coordHandle, coords.constData());

	viewMat.setToIdentity();
	viewMat.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	program->setUniformValue("u_viewMatrix", viewMat);

	projectMat.setToIdentity();
	projectMat.perspective(aspect, width()/height(), 0.1f, 100.0f);

	program->setUniformValue("u_projectMatrix", projectMat);

	program->setUniformValue("mixPara",(GLfloat)mixPara);
	for (auto i : cubePositions) {
		modelMat.setToIdentity();
		modelMat.translate(i);
		program->setUniformValue("u_modelMatrix", modelMat);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	program->disableAttributeArray(verticesHandle);
	program->disableAttributeArray(coordHandle);
	program->release();
	calcFPS();
	paintFPS();

}
void Window::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	float ratio = (float) w / h;

	projectMat.setToIdentity();
	projectMat.perspective(aspect, ratio, 0.1f, 100.0f);
}
#define VERTEX_LOCATION 0
void Window::initShader()
{
    program = new QOpenGLShaderProgram;
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertex.vsh")) {
        qDebug() << __FILE__<<__FUNCTION__<< " add vertex shader file failed.";
        return ;
    }
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragment.fsh")) {
        qDebug() << __FILE__<<__FUNCTION__<< " add fragment shader file failed.";
        return ;
    }
    program->bindAttributeLocation("qt_Vertex", 0);
    program->bindAttributeLocation("texCoord", 1);
    program->link();
    program->bind();

    verticesHandle = program->attributeLocation("qt_Vertex");
    coordHandle    = program->attributeLocation("texCoord");
}
void Window::initTexture()
{
	texture1 = new QOpenGLTexture(QImage(":/img/bj.jpg").mirrored());
	texture1->setMagnificationFilter(QOpenGLTexture::Linear);
	texture1->setMinificationFilter(QOpenGLTexture::Linear);
	texture1->setWrapMode(QOpenGLTexture::Repeat);

	texture2 = new QOpenGLTexture(QImage(":/img/ha.png").mirrored());
	texture2->setMagnificationFilter(QOpenGLTexture::Linear);
	texture2->setMinificationFilter(QOpenGLTexture::Linear);
	texture2->setWrapMode(QOpenGLTexture::Repeat);

}
void Window::makeObject()
{
	vertices << QVector3D(-0.5,  -0.5, -0.5f)
			 << QVector3D( 0.5,  -0.5, -0.5f)
			 << QVector3D( 0.5,   0.5, -0.5f)
			 << QVector3D( 0.5,   0.5, -0.5f)
			 << QVector3D(-0.5,   0.5, -0.5f)
			 << QVector3D(-0.5,  -0.5, -0.5f);

	coords << QVector2D(0, 0)
		   << QVector2D(1, 0)
		   << QVector2D(1, 1)
		   << QVector2D(1, 1)
		   << QVector2D(0, 1)
		   << QVector2D(0, 0);

	vertices << QVector3D(-0.5,  -0.5, 0.5f)
			 << QVector3D( 0.5,  -0.5, 0.5f)
			 << QVector3D( 0.5,   0.5, 0.5f)
			 << QVector3D( 0.5,   0.5, 0.5f)
			 << QVector3D(-0.5,   0.5, 0.5f)
			 << QVector3D(-0.5,  -0.5, 0.5f);

	coords << QVector2D(0, 0)
		   << QVector2D(1, 0)
		   << QVector2D(1, 1)
		   << QVector2D(1, 1)
		   << QVector2D(0, 1)
		   << QVector2D(0, 0);

	vertices << QVector3D(-0.5,   0.5,  0.5f)
			 << QVector3D(-0.5,   0.5, -0.5f)
			 << QVector3D(-0.5,  -0.5, -0.5f)
			 << QVector3D(-0.5,  -0.5, -0.5f)
			 << QVector3D(-0.5,  -0.5,  0.5f)
			 << QVector3D(-0.5,   0.5,  0.5f);

	coords << QVector2D(1, 0)
		   << QVector2D(1, 1)
		   << QVector2D(0, 1)
		   << QVector2D(0, 1)
		   << QVector2D(0, 0)
		   << QVector2D(1, 0);

	vertices << QVector3D(0.5,   0.5,  0.5f)
			 << QVector3D(0.5,   0.5, -0.5f)
			 << QVector3D(0.5,  -0.5, -0.5f)
			 << QVector3D(0.5,  -0.5, -0.5f)
			 << QVector3D(0.5,  -0.5,  0.5f)
			 << QVector3D(0.5,   0.5,  0.5f);

	coords << QVector2D(1, 0)
		   << QVector2D(1, 1)
		   << QVector2D(0, 1)
		   << QVector2D(0, 1)
		   << QVector2D(0, 0)
		   << QVector2D(1, 0);

	vertices << QVector3D(-0.5,  -0.5, -0.5f)
			 << QVector3D( 0.5,  -0.5, -0.5f)
			 << QVector3D( 0.5,  -0.5,  0.5f)
			 << QVector3D( 0.5,  -0.5,  0.5f)
			 << QVector3D(-0.5,  -0.5,  0.5f)
			 << QVector3D(-0.5,  -0.5, -0.5f);

	coords << QVector2D(0, 1)
		   << QVector2D(1, 1)
		   << QVector2D(1, 0)
		   << QVector2D(1, 0)
		   << QVector2D(0, 0)
		   << QVector2D(0, 1);

	vertices << QVector3D(-0.5,  0.5, -0.5f)
			 << QVector3D( 0.5,  0.5, -0.5f)
			 << QVector3D( 0.5,  0.5,  0.5f)
			 << QVector3D( 0.5,  0.5,  0.5f)
			 << QVector3D(-0.5,  0.5,  0.5f)
			 << QVector3D(-0.5,  0.5, -0.5f);

	coords << QVector2D(0, 1)
		   << QVector2D(1, 1)
		   << QVector2D(1, 0)
		   << QVector2D(1, 0)
		   << QVector2D(0, 0)
		   << QVector2D(0, 1);
	cubePositions << QVector3D( 0.0f,  0.0f,  0.0f)
				  << QVector3D( 2.0f,  5.0f, -15.0f)
				  << QVector3D(-1.5f, -2.2f, -2.5f)
				  << QVector3D(-3.8f, -2.0f, -12.3f)
				  << QVector3D( 2.4f, -0.4f, -3.5f)
				  << QVector3D(-1.7f,  3.0f, -7.5f)
				  << QVector3D( 1.3f, -2.0f, -2.5f)
				  << QVector3D( 1.5f,  2.0f, -2.5f)
				  << QVector3D( 1.5f,  0.2f, -1.5f)
				  << QVector3D(-1.3f,  1.0f, -1.5f);
}
void Window::calcFPS()
{
    static QTime time;
    static int once = [=](){time.start(); return 0;}();
    Q_UNUSED(once)
    static int frame = 0;
    if (frame++ > 100) {
        elapsed = time.elapsed();
        updateFPS(frame/ elapsed * 1000);
        time.restart();
        frame = 0;
    }
    //	static double start = clock();
    //	double end = clock();
    //	static int count = 0;
    //	if (count++ >=100) {
    //		elapsed = end - start;
    //		updateFPS(100/ elapsed * 1000);
    //		start = end;
    //		count = 0;
    //	}
}
void Window::updateFPS(qreal v)
{
    fps = v;
}
void Window::paintFPS()
{
	QPainter painter(this);
	painter.setPen(Qt::green);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.drawText(10, 10, QString("FPS:%1").arg(QString::number(fps, 'f', 3)));
}
