#ifndef MODEL_H
#define MODEL_H

//#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "../render_util/shader.h"

#include "render/mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int texture_from_file(const char *path, const std::string &directory);

class Model 
{
public:
	Model(char *path);

	void set_model_matrix(const glm::mat4& matrix);
	void set_position(const glm::vec3& position);

	void draw(Shader &shader);	
	void update_model_matrix();

	glm::vec3 get_position();

	std::vector<Mesh> meshes;
	std::string 			directory;
	bool 							gammaCorrection;
private:
	void 		load_model(std::string path);
	void 		process_node(aiNode *node, const aiScene *scene);
	Mesh 		process_mesh(aiMesh *mesh, const aiScene *scene);
	Texture load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
	glm::mat4 model_matrix;
};

#endif // MODEL_H