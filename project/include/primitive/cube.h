#ifndef CUBE_H
#define CUBE_H

#include "../render/mesh.h"
#include "../render_util/shader.h"

#include <vector>

class Cube : Mesh
{
public:
	Cube();
	void draw(Shader shader);
protected:
	unsigned int vao_, vbo_, ebo_;

	std::vector<float> vertices =
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

	std::vector<unsigned int> indices = 
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

#endif // CUBE_H