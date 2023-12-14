#include "render_util/shader.h"

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
  generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char *vertexShaderPath, const char *fragmentShaderPath)
{
  int success;
  char infoLog[512];

  GLuint vertex_shader = compile_shader(vertexShaderPath, GL_VERTEX_SHADER);
  GLuint fragment_shader = compile_shader(fragmentShaderPath, GL_FRAGMENT_SHADER);

  id = glCreateProgram();
  glAttachShader(id, vertex_shader);
  glAttachShader(id, fragment_shader);
  glLinkProgram(id);

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    std::cout << "Linking failed:\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::activate()
{
  glUseProgram(id);
}

std::string Shader::load_shader(const char *filepath)
{
  std::ifstream file;
  std::stringstream buffer;

  std::string ret = "";

  file.open(filepath);

  if (file.is_open())
  {
    buffer << file.rdbuf();
    ret = buffer.str();
  }
  else
  {
    std::cout << "Error to load shaders";
  }
  file.close();

  return ret;
}

GLuint Shader::compile_shader(const char *filepath, GLenum type)
{
  int success;
  char infoLog[512];

  GLuint ret = glCreateShader(type);
  std::string shader_src = load_shader(filepath);
  const GLchar *shader = shader_src.c_str();
  glShaderSource(ret, 1, &shader, NULL);
  glCompileShader(ret);

  glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(ret, 512, NULL, infoLog);
    std::cout << "Vertex shader compilation failed:\n"
              << infoLog << std::endl;
  }
  return ret;
}

void Shader::set_bool(const std::string &name, bool value)
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, int value)
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value)
{
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_4_float(const std::string &name, float v1, float v2, float v3, float v4)
{
  glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

void Shader::set_mat4(const std::string &name, glm::mat4 val)
{
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}