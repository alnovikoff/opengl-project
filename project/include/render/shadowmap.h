#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <other/glad/glad.h>
#include <GLFW/glfw3.h>

class Shadowmap
{
public:
	Shadowmap() {};
	//~Shadowmap();

	bool initialize(unsigned int width, unsigned int height);

	void bind_for_writing();
	void bind_for_reading(GLenum texture_unit);

private:
	int width = 0;
	int height = 0;
	
	GLuint fbo;
	GLuint shadowmap;
};

#endif // SHADOWMAP_H