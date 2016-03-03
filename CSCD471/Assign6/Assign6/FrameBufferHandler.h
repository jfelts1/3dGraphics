#pragma once
#include <GL/glew.h>
class FrameBufferHandler
{
public:
	FrameBufferHandler()=default;
	~FrameBufferHandler()=default;

	void genFBO();
	void bindFBO();
	void unbindFBO();
	GLuint geneateTexture() const;
private:
	GLuint m_fbo = 0;
};

