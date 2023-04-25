#include "TWindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>
TWindow::TWindow()
{
	resize(800, 600);
}

TWindow::~TWindow()
{
	makeCurrent();
	m_cube	 = nullptr;
	m_skyBox = nullptr;
#ifdef _DEBUG
	m_logger.stopLogging();
#endif
	doneCurrent();
}

void TWindow::initializeGL()
{
	initializeOpenGLFunctions();

#ifdef _DEBUG
	bool ok = m_logger.initialize();
	connect(&m_logger, &QOpenGLDebugLogger::messageLogged, [](const QOpenGLDebugMessage& debugMessage) { qWarning() << debugMessage.message(); });
	m_logger.startLogging();
	qWarning() << "debugger init " << ok;
#endif

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	initMatrix();

	m_cube	 = std::make_shared<Cube>(this);
	m_skyBox = std::make_shared<SkyBox>(this);
	startTimer(1000 / 60);
}

void TWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	float ratio = float(w) / (h ? h : 1);
	m_project.setToIdentity();
	m_project.perspective(m_aspect, ratio, c_near, c_far);
}

void TWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkKey();
	m_view.setToIdentity();
	m_view.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	m_project.setToIdentity();
	m_project.perspective(m_aspect, width() / height(), c_near, c_far);
	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	matrix.rotate(m_rotation);
	m_model = matrix;
	m_cube->draw(m_program, m_model, m_view, m_project);
	m_skyBox->draw(m_program, {}, QMatrix4x4(m_view.normalMatrix()), m_project);
}

void TWindow::timerEvent(QTimerEvent*)
{
	m_angularSpeed *= 0.99f;
	if (m_angularSpeed <= 0.01f)
	{
		m_angularSpeed = 0;
	}
	else
	{
		m_rotation = QQuaternion::fromAxisAndAngle(m_rotationAxis, m_angularSpeed) * m_rotation;
		update();
	}
	//    update();
}

void TWindow::mousePressEvent(QMouseEvent* event)
{
	m_last				 = event->pos();
	m_mousePressPosition = QVector2D { event->localPos() };
}

void TWindow::mouseMoveEvent(QMouseEvent* event)
{
	QPointF diff = event->pos() - m_last;
	m_last		 = event->pos();

	qreal sensitivity = 0.05;
	qreal xOffset	  = diff.x() * sensitivity;
	qreal yOffset	  = diff.y() * sensitivity;
	m_yaw += xOffset;
	m_pitch += yOffset;
	if (m_pitch > 89.0)
	{
		m_pitch = 89.0;
	}
	else if (m_pitch < -89.0)
	{
		m_pitch = -89.0;
	}
	QVector3D front;
	front.setX(cos(qDegreesToRadians(m_pitch)) * cos(qDegreesToRadians(m_yaw)));
	front.setY(sin(qDegreesToRadians(m_pitch)));
	front.setZ(cos(qDegreesToRadians(m_pitch)) * sin(qDegreesToRadians(m_yaw)));
	m_cameraFront = front.normalized();

	event->accept();
}
void TWindow::wheelEvent(QWheelEvent* event)
{
	int	  offset = event->angleDelta().y() < 0 ? -1 : 1;
	qreal speed	 = 10;
	if (1 <= m_aspect + offset * speed && m_aspect + offset * speed <= 45)
	{
		m_aspect = m_aspect + offset * speed;
	}
	event->accept();
}

void TWindow::mouseReleaseEvent(QMouseEvent* event)
{
	QVector2D diff = QVector2D { event->localPos() } - m_mousePressPosition;
	QVector3D n	   = QVector3D(diff.y(), diff.x(), 0.0).normalized();

	float acc	   = diff.length() / 100.0f;
	m_rotationAxis = (m_rotationAxis * m_angularSpeed + n * acc).normalized();

	m_angularSpeed += acc;
}

void TWindow::keyPressEvent(QKeyEvent* event)
{
	if (0 <= event->key() && event->key() < (int)(sizeof(m_keys) / sizeof(m_keys[0])))
	{
		m_keys[event->key()] = true;
	}
	if (event->key() == Qt::Key_F5)
	{
		m_model.setToIdentity();
		m_view.setToIdentity();
		m_project.setToIdentity();

		m_cameraPos	  = QVector3D(0.0f, 0.0f, 3.0f);
		m_cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
		m_cameraUp	  = QVector3D(0.0f, 1.0f, 0.0f);
		m_elapsed	  = 100;
		m_yaw		  = -90;
		m_pitch		  = 0;
		m_aspect	  = 45;
		update();
	}
	event->accept();
}

void TWindow::keyReleaseEvent(QKeyEvent* event)
{
	if (0 <= event->key() && event->key() < (int)(sizeof(m_keys) / sizeof(m_keys[0])))
	{
		m_keys[event->key()] = false;
	}
	event->accept();
}

void TWindow::initMatrix()
{
	m_model.setToIdentity();
	m_view.setToIdentity();
	m_view.lookAt({ 0, 0, 3.0 }, { 0, 0, 0 }, { 0, 1, 0 });

	float ratio = float(width()) / (height() ? height() : 1);

	m_project.setToIdentity();
	m_project.perspective(m_aspect, ratio, c_near, c_far);
}

void TWindow::checkKey()
{
	GLfloat cameraSpeed = 0.05 * m_elapsed / 1000.0;
	if (m_keys[Qt::Key_A])
	{
		m_cameraPos -= QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized() * cameraSpeed;
		qDebug() << "key a, left";
		update();
	}
	if (m_keys[Qt::Key_D])
	{
		m_cameraPos += QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized() * cameraSpeed;
		qDebug() << "key d, right";
		update();
	}
	if (m_keys[Qt::Key_W])
	{
		m_cameraPos += cameraSpeed * m_cameraFront;
		qDebug() << "key w, front";
		update();
	}
	if (m_keys[Qt::Key_S])
	{
		m_cameraPos -= cameraSpeed * m_cameraFront;
		qDebug() << "key s, back";
		update();
	}
}
