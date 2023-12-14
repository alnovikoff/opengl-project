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
	std::vector<TextureStructure> textures_loaded; // (оптимизация) сохраняем все загруженные текстуры, чтобы убедиться, что они не загружены более одного раза
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(char *path);

	void setModelMatrix(const glm::mat4& matrix) {
		model_matrix = matrix;
	}
	
	void set_position(const glm::vec3& position);
	glm::vec3 get_position();
	void draw(Shader &shader);	
	void update_model_matrix();
private:
	void load_model(std::string path);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<TextureStructure> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
	glm::mat4 model_matrix;
};

#endif // MODEL_H