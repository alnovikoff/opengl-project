#include "render/mesh.h"

#include <cstddef>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Texture texture)
{
	this->vertices = vertices;
	this->indices = indices;
	this->texture = texture;

	setup_mesh();
}

void Mesh::setup_mesh()
{
	// Создаем буферные объекты/массивы
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Загружаем данные в вершинный буфер
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Самое замечательное в структурах то, что расположение в памяти их внутренних переменных является последовательным.
	// Смысл данного трюка в том, что мы можем просто передать указатель на структуру, и она прекрасно преобразуется в массив данных с элементами типа glm::vec3 (или glm::vec2), который затем будет преобразован в массив данных float, ну а в конце – в байтовый массив
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Устанавливаем указатели вершинных атрибутов

	// Координаты вершин
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Нормали вершин
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Текстурные координаты вершин
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

	// Касательный вектор вершины
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// Вектор бинормали вершины
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}

void Mesh::draw(Shader shader) 
{
	glActiveTexture(GL_TEXTURE0); // перед связыванием активируем нужный текстурный юнит
	glUniform1i(glGetUniformLocation(shader.id, "texture_diffuse1"), 1);

	glBindTexture(GL_TEXTURE_2D, texture.id);

	// Отрисовываем меш
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Считается хорошей практикой возвращать значения переменных к их первоначальным значениям
	glActiveTexture(GL_TEXTURE0);
}  