#include "qnamespace.h"
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
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

	void calcFPS();
	void updateFPS(qreal);
	void paintFPS();

private:
#ifdef _DEBUG
	QOpenGLDebugLogger* mLogger = nullptr;
#endif
	QOpenGLShaderProgram* mProgram		= nullptr;
	GLuint				  mVerticesVBO	= 0;
	GLuint				  mInstanceVBO	= 0;
	GLuint				  mTexCoordsVBO = 0;
	GLuint				  mVAO			= 0;
	QColor				  mColor		= Qt::red;
	QVector<QVector3D>	  mVertices;
	QMatrix4x4			  mMVPMat;
	QVector<QMatrix4x4>	  mInstanceMats;
	QVector<QVector2D>	  mTexCoords;
	QOpenGLTexture*		  mTexture;
	qreal				  mFPS = 60.0;
};
