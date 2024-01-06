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
  //std::vector<TextureStructure> textures;

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

  Texture diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  std::cout << diffuse_maps.id << '\n';

  return Mesh(vertices, indices, diffuse_maps);
}

// Load material textures
Texture Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name)
{
  Texture txt;
  aiString str;
  mat->GetTexture(type, 0, &str);
  txt.texture_from_file(str.C_Str(), this->directory);
  return txt;
}