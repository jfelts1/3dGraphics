#include "FrameBufferHandler.h"
#include "GlobalDefines.h"
#include <cstdio>

void FrameBufferHandler::genFBO()
{
	glGenFramebuffers(1, &m_fbo);
}

void FrameBufferHandler::bindFBO() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferHandler::unbindFBO() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferHandler::generateTexture() const
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

void FrameBufferHandler::genRBO()
{
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		puts("ERROR: Framebuffer not complete");
	}
}