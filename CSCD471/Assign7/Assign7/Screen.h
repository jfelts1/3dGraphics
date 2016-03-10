#pragma once
#include <GL/glew.h>
#include <vector>
#include "FrameBufferHandler.h"

class Screen
{
public:
	Screen() = default;
	virtual ~Screen() =default;

	void initScreen();

	void bindFBO() const;
	void unbindFBO() const;
	void render() const;

	int getViewportWidth() const
	{
		return m_viewportWidth;
	}

	void setViewportWidth(int viewportWidth)
	{
		m_viewportWidth = viewportWidth;
	}

	int getViewportHeight() const
	{
		return m_viewportHeight;
	}

	void setViewportHeight(int viewportHeight)
	{
		m_viewportHeight = viewportHeight;
	}
private:
	GLuint m_screenVAO;
	GLuint m_screenVBO;
	GLuint m_screenFBOGeneratedTextureID;
	FrameBufferHandler m_screenFrameBufferHandler;
	int m_viewportWidth;
	int m_viewportHeight;

	std::vector<GLfloat> m_screenVertices{
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
};

