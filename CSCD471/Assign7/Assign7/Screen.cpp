#include "Screen.h"

extern GLuint fboProg;

void Screen::initScreen()
{
	glGenVertexArrays(1, &m_screenVAO);
	glGenBuffers(1, &m_screenVBO);
	glBindVertexArray(m_screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
	glBufferData(GL_ARRAY_BUFFER, m_screenVertices.size()*sizeof(decltype(m_screenVertices[0])), m_screenVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	m_screenFrameBufferHandler.genFBO();
	m_screenFrameBufferHandler.bindFBO();
	m_screenFBOGeneratedTextureID = m_screenFrameBufferHandler.generateTexture();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenFBOGeneratedTextureID, 0);

	m_screenFrameBufferHandler.genRBO();
	m_screenFrameBufferHandler.unbindFBO();
}

void Screen::bindFBO() const
{
	m_screenFrameBufferHandler.bindFBO();
}

void Screen::unbindFBO() const
{
	m_screenFrameBufferHandler.unbindFBO();
}

void Screen::render() const
{
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(fboProg);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_screenFBOGeneratedTextureID);
	glBindVertexArray(m_screenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}