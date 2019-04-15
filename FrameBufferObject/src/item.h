#ifndef SHADER_H
#define SHADER_H
#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
class Item: public QQuickFramebufferObject{
	Q_OBJECT
	Q_PROPERTY(qreal xRotate READ xRotate WRITE setXRotate NOTIFY xRotateChanged)
	Q_PROPERTY(qreal yRotate READ yRotate WRITE setYRotate NOTIFY yRotateChanged)
	Q_PROPERTY(qreal zRotate READ zRotate WRITE setZRotate NOTIFY zRotateChanged)
    Q_PROPERTY(bool full READ full WRITE setFull NOTIFY fullChanged)
	Q_PROPERTY(qreal fps READ fps NOTIFY fpsChanged)

public:
    Item(QQuickItem *parent = 0)
		:	QQuickFramebufferObject(parent)
		,	m_xRotate(0)
		,	m_yRotate(0)
		,	m_zRotate(0)
		,	m_fps(0)
	{
        connect(this, &Item::xRotateChanged, [=](){update();});
        connect(this, &Item::yRotateChanged, [=](){update();});
        connect(this, &Item::zRotateChanged, [=](){update();});

        connect(this, &Item::fullChanged, [&](){
            if (full()) {
                m_size.setWidth(this->width());
                m_size.setHeight(this->height());
                m_pos = this->position();
                this->setPosition(QPointF(0, 0));
                this->setWidth(QApplication::desktop()->width());
                this->setHeight(QApplication::desktop()->height());
            } else {
                setPosition(m_pos);
                this->setWidth(m_size.width());
                this->setHeight(m_size.height());
            }
        });
        startTimer(1000 / 60);
	}
	void timerEvent(QTimerEvent *)
	{
		update();
	}
	Renderer * createRenderer() const override;

	qreal xRotate() const {return m_xRotate;}
	qreal yRotate() const {return m_yRotate;}
	qreal zRotate() const {return m_zRotate;}
	qreal fps() const {return m_fps;}
    bool full() const {return m_full;}

	void setXRotate(qreal v)
	{
		if (v == m_xRotate) return ;
		m_xRotate = v;
		emit xRotateChanged();
	}
	void setYRotate(qreal v)
	{
		if (v == m_yRotate) return ;
		m_yRotate = v;
		emit yRotateChanged();
	}
	void setZRotate(qreal v)
	{
		if (v == m_zRotate) return ;
		m_zRotate = v;
		emit zRotateChanged();
	}

	void updateFPS(qreal fps)
	{
		m_fps = fps;
		emit fpsChanged();
	}
    void setFull(bool v)
    {
        if (v == m_full) return ;
        m_full = v;
        emit fullChanged();

    }
signals:
	void xRotateChanged();
	void yRotateChanged();
	void zRotateChanged();
	void fpsChanged();
    void fullChanged();
private:
	qreal m_xRotate;
	qreal m_yRotate;
	qreal m_zRotate;
	qreal m_fps;
    bool  m_full;
    QSize m_size;
    QPointF m_pos;
};

#endif // SHADER_H
