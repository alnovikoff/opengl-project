#ifndef SKYBOX_H
#define SKYBOX_H

#include "../primitive/cube.h"

#include <other/stb/stb_image.h>

#include <iostream>
#include <filesystem>

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
		// Load the cubemap texture
		std::string cubemap_face[6] = 
		{
				_strdup((path / "skybox/right.jpg").string().c_str()),
				_strdup((path / "skybox/left.jpg").string().c_str()),
				_strdup((path / "skybox/bottom.jpg").string().c_str()),
				_strdup((path / "skybox/top.jpg").string().c_str()),
				_strdup((path / "skybox/front.jpg").string().c_str()),
				_strdup((path / "skybox/back.jpg").string().c_str()),
		};

		glGenTextures(1, &cubemap_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (unsigned int i = 0; i < 6; i++)
		{
				int width, height, nrChannels;
				unsigned char *data = stbi_load(cubemap_face[i].c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
						stbi_image_free(data);
				}
				else
				{
						std::cout << "Failed to load cubemap texture at path: " << cubemap_face[i] << std::endl;
						stbi_image_free(data);
				}
		}
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
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
private:
	unsigned int skybox_vao, skybox_vbo, skybox_ebo;
	unsigned int cubemap_texture;

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