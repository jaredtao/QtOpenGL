#include <QOpenGLExtraFunctions>
#include <QOpenGLWindow>
#ifdef GL_DEBUG
#    include <QOpenGLDebugLogger>
#endif
#include <QTimer>
#include <QElapsedTimer>
class GLWindow : public QOpenGLWindow, public QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit GLWindow(UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = nullptr);
    explicit GLWindow(QOpenGLContext *shareContext, UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = nullptr);
    virtual ~GLWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    void timerEvent(QTimerEvent *) override;
protected slots:
#ifdef GL_DEBUG
    void onDebuggerMessage(const QOpenGLDebugMessage &debugMessage);
#endif
protected:
    void calcFPS();
    void updateFPS(qreal v);

protected:
#ifdef GL_DEBUG
    QOpenGLDebugLogger mDebugger;
#endif
    qreal mFps = 60.0;
    QElapsedTimer mElapsed;
    int mFrame = 0;
};
