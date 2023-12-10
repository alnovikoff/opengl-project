#include "render/shadowmap.h"

bool Shadowmap::initialize(unsigned int width, unsigned int height)
{
	width = width;
	height = height;

	glGenTextures(1, &shadowmap);
	glBindTexture(GL_TEXTURE_2D, shadowmap);
	glTexImage2D(GL_IMAGE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowmap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void Shadowmap::bind_for_writing()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
}

void Shadowmap::bind_for_reading(GLenum texture_unit)
{
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, shadowmap);
}