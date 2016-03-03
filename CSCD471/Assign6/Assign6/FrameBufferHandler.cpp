#include "FrameBufferHandler.h"
#include "GlobalDefines.h"


void FrameBufferHandler::genFBO()
{
	glGenFramebuffers(1, &m_fbo);
}

void FrameBufferHandler::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferHandler::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferHandler::geneateTexture() const
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texId;
}