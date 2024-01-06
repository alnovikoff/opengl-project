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
	Skybox(std::filesystem::path path)
	{
		glGenVertexArrays(1, &skybox_vao);
		glGenBuffers(1, &skybox_vbo);
		glGenBuffers(1, &skybox_ebo);
		glBindVertexArray(skybox_vao);
		glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox_indices), &skybox_indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		texture.load_cubemap_texture(path);
	}

	~Skybox()
	{
		glDeleteVertexArrays(1, &skybox_vao);
		glDeleteBuffers(1, &skybox_vbo);
		glDeleteBuffers(1, &skybox_ebo);
	}

	void draw(Shader shader)
	{
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(skybox_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
private:
	Texture texture;
	
	unsigned int skybox_vao, skybox_vbo, skybox_ebo;
	
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