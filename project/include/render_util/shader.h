#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <other/glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
	unsigned int id;
	Shader() {}
	Shader(const char* vertex_shader_path, const char* fragment_shader_path);

	void generate(const char* vertex_shader, const char* vfragment_shader);

	void activate();

	std::string load_shader(const char* filepath);
	GLuint compile_shader(const char* filepath, GLenum type);

	void set_bool(const std::string& name, bool value);
	void set_int(const std::string& name, int value);
	void set_float(const std:: string& name, float value);
	void set_4_float(const std::string& name, float v1, float v2, float v3, float v4);
	void set_mat4(const std::string& name, glm::mat4 val);
};

#endif // SHADER_H