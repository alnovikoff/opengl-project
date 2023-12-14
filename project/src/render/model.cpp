#include "render/model.h"

#include <iostream>

#include "E:\MUNI\ComputerGraphicsAPI\project\project\include\other\stb\stb_image.h"

Model::Model(char *path)
{
  load_model(path);
}

void Model::draw(Shader &shader)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw(shader);
  }
}

void Model::set_position(const glm::vec3& position)
{
  model_matrix = glm::translate(glm::mat4(1.0f), position);
}

glm::vec3 Model::get_position()
{
  if (!meshes.empty() && !meshes[0].get_vertices().empty())
  {
    // Assuming the position is stored in the first vertex of the first mesh
    return meshes[0].get_vertices()[0].position;
  }
  // Return a default position if no vertices are available
  return glm::vec3(0.0f);
}

void Model::load_model(std::string path)
{
  // Чтение файла с помощью Assimp
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

  // Проверка на ошибки
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // если НЕ 0
  {
    std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
    return;
  }

  // Получение пути к файлу
  directory = path.substr(0, path.find_last_of('/'));

  // Рекурсивная обработка корневого узла Assimp
  process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene)
{
  // Обрабатываем все меши (если они есть) у выбранного узла
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(process_mesh(mesh, scene));
  }
  // И проделываем то же самое для всех дочерних узлов
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    process_node(node->mChildren[i], scene);
  }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
  // Данные для заполнения
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<TextureStructure> textures;

  // Цикл по всем вершинам меша
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if (mesh->mTextureCoords[0])
    {
      glm::vec2 vec;

      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.tex_coord = vec;
    }
    else
      vertex.tex_coord = glm::vec2(0.0f, 0.0f);

    // Касательный вектор
    vector.x = mesh->mTangents[i].x;
    vector.y = mesh->mTangents[i].y;
    vector.z = mesh->mTangents[i].z;
    vertex.tangent = vector;

    // Вектор бинормали
    vector.x = mesh->mBitangents[i].x;
    vector.y = mesh->mBitangents[i].y;
    vector.z = mesh->mBitangents[i].z;
    vertex.bitangent = vector;
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<TextureStructure> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

  std::vector<TextureStructure> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

  std::vector<TextureStructure> normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

  std::vector<TextureStructure> height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  return Mesh(vertices, indices, textures);
}

std::vector<TextureStructure> Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name)
{
  std::vector<TextureStructure> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);

    // Проверяем, не была ли текстура загружена ранее, и если - да, то пропускаем загрузку новой текстуры и переходим к следующей итерации
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
      {
        textures.push_back(textures_loaded[j]);
        skip = true; // текстура с тем же путем к файлу уже загружена, переходим к следующей (оптимизация)
        break;
      }
    }
    if (!skip)
    { // если текстура еще не была загружена, то загружаем её
      TextureStructure texture;
      texture.id = texture_from_file(str.C_Str(), this->directory);
      texture.type = type_name;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture); // сохраняем текстуру в массиве с уже загруженными текстурами, тем самым гарантируя, что у нас не появятся без необходимости дубликаты текстур
    }
  }
  return textures;
}

unsigned int texture_from_file(const char *path, const std::string &directory)
{
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}