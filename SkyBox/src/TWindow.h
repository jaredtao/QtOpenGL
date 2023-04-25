#pragma once

#include "Cube.h"
#include "GLHeaders.h"
#include "SkyBox.h"
#include <QObject>
#include <QOpenGLWidget>
#include <memory>
class TWindow
	: public QOpenGLWidget
	, public GLFuncName
{
public:
	TWindow();
	~TWindow() override;

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void timerEvent(QTimerEvent*) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void initMatrix();
	void checkKey();

private:
	std::shared_ptr<Cube>	m_cube	 = nullptr;
	std::shared_ptr<SkyBox> m_skyBox = nullptr;
	QOpenGLShaderProgram	m_program;
	QMatrix4x4				m_model, m_view, m_project;
#ifdef _DEBUG
	QOpenGLDebugLogger m_logger;
#endif

	QVector2D m_mousePressPosition;
	QVector3D m_rotationAxis;
	float	  m_angularSpeed = 0;

	QQuaternion m_rotation;

	const float c_near	  = 0.1f;
	const float c_far	  = 100.0f;
	qreal		m_elapsed = 100;
	qreal		m_yaw	  = -90;
	qreal		m_pitch	  = 0;
	qreal		m_aspect  = 45;

	QVector3D m_cameraPos	= { 0.0f, 0.0f, 3.0f };
	QVector3D m_cameraFront = { 0.0f, 0.0f, -1.0f };
	QVector3D m_cameraUp	= { 0.0f, 1.0f, 0.0f };
	QPoint	  m_last;
	bool	  m_keys[1024] = { false };
};
