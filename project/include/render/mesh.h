#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "../render_util/shader.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
	glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct TextureStructure {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureStructure> textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureStructure> textures);
	void draw(Shader shader);
	std::vector<Vertex> get_vertices() const {
		return vertices;
	}
private:
	unsigned int VBO, EBO;

	void setup_mesh();	
};

#endif // MESH_H