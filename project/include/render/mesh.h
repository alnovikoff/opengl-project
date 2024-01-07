#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "../render_util/shader.h"

#include "../render_util/texture.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
	glm::vec3 tangent;
  glm::vec3 bitangent;
};

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture texture);

	std::vector<Vertex> 				vertices;
	std::vector<unsigned int> 	indices;
	Texture 										texture;
	unsigned int 								VAO;

	void draw(Shader shader);
	std::vector<Vertex> get_vertices();
private:
	unsigned int 								VBO;
	unsigned int 								EBO;
	void setup_mesh();	
};

#endif // MESH_H