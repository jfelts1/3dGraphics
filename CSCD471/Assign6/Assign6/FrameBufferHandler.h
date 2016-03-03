#pragma once
#include <GL/glew.h>
class FrameBufferHandler
{
public:
	FrameBufferHandler()=default;
	~FrameBufferHandler()=default;

	void genFBO();
	void bindFBO() const;
	void unbindFBO() const;
	GLuint generateTexture() const;
	void genRBO();
private:
	GLuint m_fbo = 0;
	GLuint m_rbo = 0;
};

