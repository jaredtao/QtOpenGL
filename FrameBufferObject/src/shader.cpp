#include "shader.h"
#include "render.h"
#include<QOpenGLFramebufferObject>
#include <QThread>
class ItemRender : public QQuickFramebufferObject::Renderer {
public:
	ItemRender();

	void render() override;
	QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
	void synchronize(QQuickFramebufferObject *) override;
	Render m_render;
};
ItemRender::ItemRender()
{
	m_render.init();
}

void ItemRender::render()
{
	m_render.paint();

}

void ItemRender::synchronize(QQuickFramebufferObject *item)
{
	Shader *shader = qobject_cast<Shader *>(item);
	m_render.setRotate(shader->xRotate(), shader->yRotate(), shader->zRotate());
	shader->updateFPS(m_render.getFPS());
}

QOpenGLFramebufferObject *ItemRender::createFramebufferObject(const QSize &size)
{
	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	format.setSamples(4);
	return new QOpenGLFramebufferObject(size, format);
}

QQuickFramebufferObject::Renderer * Shader::createRenderer() const
{
	return new ItemRender;
}
