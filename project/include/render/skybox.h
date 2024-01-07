#ifndef SKYBOX_H
#define SKYBOX_H

#include <other/stb/stb_image.h>

#include <iostream>
#include <filesystem>
#include "../render_util/shader.h"
#include "../render_util/texture.h"

class Skybox
{
public:
	Skybox(std::filesystem::path path);
	~Skybox();

	void draw(Shader shader);
private:
	Texture texture;
	
	unsigned int skybox_vao;
	unsigned int skybox_vbo;
	unsigned int skybox_ebo;
	
	float skybox_vertices[24] =
	{
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f 
	};

	unsigned int skybox_indices[36] = 
	{
		1, 2, 6,
		6, 5, 1,

		0, 4, 7,
		7, 3, 0,

		4, 5, 6,
		6, 7, 4,

		0, 3, 2,
		2, 1, 0,

		0, 1, 5,
		5, 4, 0,

		3, 7, 6,
		6, 2, 3
	};
};

#endif // SKYBOX_H