#include "qnamespace.h"
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QVector>
class MainWindow
	: public QOpenGLWidget
	, public QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
	using Super = QOpenGLWidget;

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
	virtual void timerEvent(QTimerEvent* e) override;

private:
	void initVertices();
	void updateVertices();

private:
#ifdef _DEBUG
	QOpenGLDebugLogger* mLogger = nullptr;
#endif
	QOpenGLShaderProgram* mProgram = nullptr;
	GLuint				  mVBO	   = 0;
	GLuint				  mVAO	   = 0;
	QColor				  mColor   = Qt::red;
	QVector<QVector3D>	  mVertices;
	uint32_t			  mCount = 0;
};
