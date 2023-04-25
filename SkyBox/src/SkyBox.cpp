#include "SkyBox.h"

SkyBox::SkyBox(GLFuncName* func)
	: m_func(func)
	, m_vertexShader(QOpenGLShader::Vertex)
	, m_fragmentShader(QOpenGLShader::Fragment)
{
	initBuffer();
	initShader();
	initTexture();
}

SkyBox::~SkyBox()
{
	m_arrayBuf.destroy();
}

void SkyBox::initBuffer()
{
	GLfloat skyboxVertices[] = { // Positions
								 -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,	1.0f,  -1.0f, -1.0f, 1.0f,	-1.0f,

								 -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,	 -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

								 1.0f,	-1.0f, -1.0f, 1.0f,	 -1.0f, 1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  -1.0f, 1.0f,	 -1.0f, -1.0f,

								 -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,	1.0f,  1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,  1.0f,	 1.0f,	-1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

								 -1.0f, 1.0f,  -1.0f, 1.0f,	 1.0f,	-1.0f, 1.0f,  1.0f,	 1.0f,	1.0f,  1.0f,  1.0f,	 -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,	-1.0f,

								 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,	 -1.0f, 1.0f
	};
	m_vertexCount = sizeof skyboxVertices / sizeof skyboxVertices[0];
	m_arrayBuf.create();

	m_func->glGenVertexArrays(1, &m_vao);
	m_func->glBindVertexArray(m_vao);

	m_arrayBuf.bind();
	m_arrayBuf.allocate(skyboxVertices, sizeof skyboxVertices);
	const int positionLocation = 0;
	m_func->glEnableVertexAttribArray(positionLocation);
	m_func->glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, nullptr);

	m_func->glBindVertexArray(0);
}

void SkyBox::initShader()
{
	const auto vShader = u8R"(#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 mvp;
out vec3 vTexCoord;
void main()
{
    vTexCoord = position;
    vec4 pos = mvp * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
)";
	const auto fShader = R"(#version 330 core
uniform samplerCube skyBox;
in vec3 vTexCoord;
out vec4 outColor;
void main()
{
    outColor = texture(skyBox, vTexCoord);
}
)";
	if (!m_vertexShader.compileSourceCode(vShader))
	{
		qWarning() << m_vertexShader.log();
	}
	if (!m_fragmentShader.compileSourceCode(fShader))
	{
		qWarning() << m_fragmentShader.log();
	}
}

void SkyBox::initTexture()
{
	m_images[0] = QImage(":/assets/right.jpg").convertToFormat(QImage::Format_RGB888);
	m_images[1] = QImage(":/assets/left.jpg").convertToFormat(QImage::Format_RGB888);
	m_images[2] = QImage(":/assets/top.jpg").convertToFormat(QImage::Format_RGB888);
	m_images[3] = QImage(":/assets/bottom.jpg").convertToFormat(QImage::Format_RGB888);
	m_images[4] = QImage(":/assets/back.jpg").convertToFormat(QImage::Format_RGB888);
	m_images[5] = QImage(":/assets/front.jpg").convertToFormat(QImage::Format_RGB888);
	m_func->glGenTextures(1, &m_cubeTexture);
	m_func->glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTexture);
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_func->glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_images[i].width(), m_images[i].height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_images[i].bits());
	}
	m_func->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_func->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_func->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_func->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_func->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//    m_texture.create();
	//    m_texture.setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
	//    m_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
	//    m_texture.setMipLevels(0);
	//    m_texture.setLayers(0);
	//    m_texture.setSize(m_images[0].width(), m_images[0].height(), m_images[0].depth());
	//    m_texture.setFormat(QOpenGLTexture::RGB8_UNorm);
	//    m_texture.allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);
	//    for (int i = 0; i < 6; ++i) {
	//        m_texture.setData(0, 0, static_cast<QOpenGLTexture::CubeMapFace>(QOpenGLTexture::CubeMapPositiveX + i), QOpenGLTexture::RGB,
	//        QOpenGLTexture::UInt8, m_images[i].bits());
	//    }
	//    m_texture.generateMipMaps();
}
void SkyBox::draw(QOpenGLShaderProgram& program, const QMatrix4x4& model, const QMatrix4x4& view, const QMatrix4x4& project)
{
	m_func->glDepthFunc(GL_LEQUAL);
	program.removeAllShaders();
	program.addShader(&m_vertexShader);
	program.addShader(&m_fragmentShader);
	if (!program.link())
	{
		qWarning() << program.log();
		return;
	}
	if (!program.bind())
	{
		qWarning() << program.log();
		return;
	}
	m_func->glBindVertexArray(m_vao);

	m_func->glActiveTexture(GL_TEXTURE0);
	m_func->glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTexture);
	program.setUniformValue("mvp", project * view * model);
	program.setUniformValue("skyBox", 0);

	m_func->glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

	m_func->glDepthFunc(GL_LESS);
}
