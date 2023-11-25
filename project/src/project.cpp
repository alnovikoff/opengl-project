#include "project.h"

#include <filesystem>
#include <other/stb/stb_image.h>

#include "window.h"
#include "render_util/shader.h"
#include "render_util/texture.h"

void Project::run()
{
	// Get the current executable's path
	std::filesystem::path executablePath = std::filesystem::current_path();
	// Move up to the project/build/bin directory
	std::filesystem::path binPath = executablePath.parent_path();
	// Move up to the project directory
	std::filesystem::path path = binPath.parent_path();
	std::filesystem::path projectPath = path.parent_path();
	// Add your asset folder to the path
	std::filesystem::path assetPath = projectPath / "project" / "assets";


	// glfw setup
	int success;
	char infoLog[1024];

	if (!window.init())
	{
		std::cout << "Failed to initialize window!" << std::endl;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		glfwTerminate();
	}
	
	window.set_parameters();
	glEnable(GL_DEPTH_TEST);

	Shader main_shader((assetPath / "main_vert.glsl").string().c_str(), 
											(assetPath / "main_frag.glsl").string().c_str());

	float vertices[] = {
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Coord attribs
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribs
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Texture texture1_t;
	unsigned int texture1;
	texture1 = texture1_t.load_texture((assetPath / "img2.jpg").string().c_str());
	main_shader.activate();
	glUniform1i(glGetUniformLocation(main_shader.id, "texture1"), 0);

	while (!window.should_close())
	{
		// Render loop
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window.update();
    main_shader.activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    window.new_frame();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}