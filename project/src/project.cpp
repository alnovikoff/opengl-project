#include "project.h"

#include <filesystem>
#include <other/stb/stb_image.h>

#include "render_util/camera.h"

#include "io/mouse.h"
#include "io/keyboard.h"

#include "window.h"
#include "render_util/shader.h"
#include "render_util/texture.h"
#include "render/model.h"


glm::mat4 transform = glm::mat4(1.0f);

float delta_time = 0.0f;
float last_frame = 0.0f;

Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));

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
	stbi_set_flip_vertically_on_load(true);

	// Компилирование нашей шейдерной программы
	Shader shader_model((assetPath / "model_vert.glsl").string().c_str(), (assetPath / "model_frag.glsl").string().c_str());
	Shader shader_env_map((assetPath / "shaders/env_map_vert.glsl").string().c_str(), (assetPath / "shaders/env_map_frag.glsl").string().c_str());
	Shader shader_window((assetPath / "shaders/window_vert.glsl").string().c_str(), (assetPath / "shaders/window_frag.glsl").string().c_str());


	// Загрузка моделей
	Model ground_model(_strdup((assetPath / "model/ground.obj").string().c_str()));
	Model pillars_model(_strdup((assetPath / "model/pillars.obj").string().c_str()));
	Model vehicle_model(_strdup((assetPath / "model/vehicle.obj").string().c_str()));
	Model vehicle_discs_model(_strdup((assetPath / "model/vehicle_discs.obj").string().c_str()));
	Model vehicle_windows_model(_strdup((assetPath / "model/vehicle_windows.obj").string().c_str()));
	Model vehicle_light1_model(_strdup((assetPath / "model/vehicle_light_one.obj").string().c_str()));
	Model vehicle_light2_model(_strdup((assetPath / "model/vehicle_light_two.obj").string().c_str()));
	Model wires_model(_strdup((assetPath / "model/wires.obj").string().c_str()));
	Model lamps_model(_strdup((assetPath / "model/lamps.obj").string().c_str()));
	Model lamp_light1_model(_strdup((assetPath / "model/lamp_light_one.obj").string().c_str()));
	Model lamp_light2_model(_strdup((assetPath / "model/lamp_light_two.obj").string().c_str()));
	Model rock1_model(_strdup((assetPath / "model/rock_1.obj").string().c_str()));
	Model rock2_model(_strdup((assetPath / "model/rock_2.obj").string().c_str()));
	Model rock3_model(_strdup((assetPath / "model/rock_3.obj").string().c_str()));
	Model rock4_model(_strdup((assetPath / "model/rock_4.obj").string().c_str()));
	Model rock5_model(_strdup((assetPath / "model/rock_5.obj").string().c_str()));
	Model rock6_model(_strdup((assetPath / "model/rock_6.obj").string().c_str()));
	Model cactus1_model(_strdup((assetPath / "model/cactus_1.obj").string().c_str()));
	Model cactus2_model(_strdup((assetPath / "model/cactus_2.obj").string().c_str()));
	Model cactus3_model(_strdup((assetPath / "model/cactus_3.obj").string().c_str()));
	Model cactus4_model(_strdup((assetPath / "model/cactus_4.obj").string().c_str()));
	Model cactus5_model(_strdup((assetPath / "model/cactus_5.obj").string().c_str()));
	Model cactus6_model(_strdup((assetPath / "model/cactus_6.obj").string().c_str()));
	Model cactus7_model(_strdup((assetPath / "model/cactus_7.obj").string().c_str()));
	Model cactus8_model(_strdup((assetPath / "model/cactus_8.obj").string().c_str()));
	Model scene_model(_strdup((assetPath / "model/scene.obj").string().c_str()));
	Model ufo_model(_strdup((assetPath / "model/ufo.obj").string().c_str()));
	Model ufo_light_model(_strdup((assetPath / "model/ufo_light.obj").string().c_str()));


	//------------------------- SKYBOX
	float skybox_vertices[] =
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

	unsigned int skybox_indices[] = 
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

	unsigned int skybox_vao, skybox_vbo, skybox_ebo;
	glGenVertexArrays(1, &skybox_vao);
	glGenBuffers(1, &skybox_vbo);
	glGenBuffers(1, &skybox_ebo);
	glBindVertexArray(skybox_vao);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox_indices), &skybox_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	std::string cubemap_face[6] = 
	{
		"E:\\MUNI\\ComputerGraphicsAPI\\project\\project\\assets\\skybox\\right.jpg",
		"E:\\MUNI\\ComputerGraphicsAPI\\project\\project\\assets\\skybox\\left.jpg",
		"E:\\MUNI\\ComputerGraphicsAPI\\project\\project\\assets\\skybox\\top.jpg",
		"E:\\MUNI\\ComputerGraphicsAPI\\project\\project\\assets\\skybox\\bottom.jpg",
		"E:\\MUNI\\ComputerGraphicsAPI\\project\\project\\assets\\skybox\\front.jpg",
		"E:\\MUNI\\ComputerGraphicsAPI\\project\\project\\assets\\skybox\\back.jpg"
	};


	unsigned int cubemap_texture;
	glGenTextures(1, &cubemap_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(cubemap_face[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << cubemap_face[i] << std::endl;
			stbi_image_free(data);
		}
	}

	Shader skybox_shader((assetPath / "shaders/skybox_vert.glsl").string().c_str(), (assetPath / "shaders/skybox_frag.glsl").string().c_str());
	//------------------------- SKYBOX
	// shader_window.activate();
	// glUniform1f(glGetUniformLocation(shader_window.id, "alpha"), 0.7f); 

	// skybox_shader.activate();
	// glUniform1i(glGetUniformLocation(skybox_shader.id, "skybox"), 0);

	// // // dynamic env map DOESNT WORK JUST BLACK SCREEN BUT ENC MAPPING WORKS
	//  GLuint fbo;
	// // glGenFramebuffers(1, &fbo);
	//  GLuint rbo;
	// // glGenRenderbuffers(1, &rbo);
	// // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, (float)Window::window_width, (float)Window::window_height);
	// // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//  GLuint cubemap;
	// // glGenTextures(1, &cubemap);
	// // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	// // for (GLuint i = 0; i < 6; ++i)
	// // {
	// // 		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, (float)Window::window_width, (float)Window::window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// // }
	// // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// // glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// glm::vec3 directions[] = {
  //   glm::vec3(1.0f, 0.0f, 0.0f),  // Positive X
  //   glm::vec3(-1.0f, 0.0f, 0.0f), // Negative X
  //   glm::vec3(0.0f, 1.0f, 0.0f),  // Positive Y
  //   glm::vec3(0.0f, -1.0f, 0.0f), // Negative Y
  //   glm::vec3(0.0f, 0.0f, 1.0f),  // Positive Z
  //   glm::vec3(0.0f, 0.0f, -1.0f)  // Negative Z
	// };

	// // for (GLuint face = 0; face < 6; ++face)
	// // {
	// // 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemap, 0);
	// // 	// Set the camera's direction to the direction of the current face
	// // 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// // 	camera.set_direction(directions[face]);
	// // }

	// // glActiveTexture(GL_TEXTURE0);
	// // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	// // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	// // {
	// // 		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	// // }

	// // Create the framebuffer
	// float cubemapSize = 512.0f;
	// glGenFramebuffers(1, &fbo);
	// glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	// // Create the depth renderbuffer
	// glGenRenderbuffers(1, &rbo);
	// glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemapSize, cubemapSize);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// // Create the cube map texture
	// glGenTextures(1, &cubemap);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	// for (GLuint i = 0; i < 6; ++i)
	// {
	// 	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, cubemapSize, cubemapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// }
	// // Set the texture parameters...

	// // Check the framebuffer status
	// if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	// {
	// 	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	// }

	// // Render to each face of the cube map
	// for (GLuint face = 0; face < 6; ++face)
	// {
	// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemap, 0);
	// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 	// Set the camera's direction
	// 	camera.set_direction(directions[face]);

	// 	// Render your scene
	// 	// ...
	// }
	glm::vec3 ufoPosition = ufo_light_model.get_position();
	glm::vec3 lamp_light1_position = lamp_light1_model.get_position();
	glm::vec3 lamp_light2_position = lamp_light2_model.get_position();
	float yPos = ufo_model.get_position().y;
	Shader direct_light((assetPath / "shaders/light_vert.glsl").string().c_str(), (assetPath / "shaders/light_frag.glsl").string().c_str());
	float elapsedTime = 0.0f;
	while (!window.should_close())
	{
		double current_time = glfwGetTime();
		delta_time = current_time - last_frame;
		last_frame = current_time;
		elapsedTime += delta_time;

    process_input(delta_time);

		// Render loop
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.update();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.get_view_matrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.get_zoom()), (float)Window::window_width / (float)Window::window_height, 0.1f, 100.0f);
		glm::vec3 cameraPos = camera.get_camera_pos();

		// models
		shader_model.activate();

		// Global light--------------------------------
		GLint lightDirLoc = glGetUniformLocation(shader_model.id, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(shader_model.id, "viewPos");
		glUniform3f(lightDirLoc, 0.0f, -200.0f, -30.0f);
		glUniform3f(viewPosLoc, camera.get_camera_pos().x, camera.get_camera_pos().y, camera.get_camera_pos().z);
		glUniform3f(glGetUniformLocation(shader_model.id, "light.ambient"), 0.1f, 0.1f, 0.2f);  // Dark blue ambient
		glUniform3f(glGetUniformLocation(shader_model.id, "light.diffuse"), 0.2f, 0.2f, 0.6f);  // Medium blue diffuse
		glUniform3f(glGetUniformLocation(shader_model.id, "light.specular"), 0.02f, 0.02f, 0.06f); // Medium blue specular
		//--------------------------------
		// Spot Light 1 -----------------------------------
		GLint spotLightPosLoc = glGetUniformLocation(shader_model.id, "spotlight[0].position");
		GLint spotLightDirLoc = glGetUniformLocation(shader_model.id, "spotlight[0].direction");
		GLint spotLightCutoffLoc = glGetUniformLocation(shader_model.id, "spotlight[0].cutoff");
		GLint spotLightOuterCutoffLoc = glGetUniformLocation(shader_model.id, "spotlight[0].outerCutoff");
		GLint spotLightAmbientLoc = glGetUniformLocation(shader_model.id, "spotlight[0].ambient");
		GLint spotLightDiffuseLoc = glGetUniformLocation(shader_model.id, "spotlight[0].diffuse");
		GLint spotLightSpecularLoc = glGetUniformLocation(shader_model.id, "spotlight[0].specular");

    glUniform3f(spotLightDirLoc, 0.0f, 1.0f, 0.0f);
    float cutoffAngle = 62.0f; // in degrees
		float outerCutoffAngle = 66.0f; // in degrees
		glUniform1f(spotLightCutoffLoc, cos(glm::radians(cutoffAngle)));
		glUniform1f(spotLightOuterCutoffLoc, cos(glm::radians(outerCutoffAngle)));
    glUniform3f(spotLightAmbientLoc, 0.1f, 0.3f, 0.1f);
    glUniform3f(spotLightDiffuseLoc, 0.1f, 2.f, 0.1f);
    glUniform3f(spotLightSpecularLoc, 0.0f, 0.2f, 0.0f);
		// --------------------------------------------------
		// Spot Light 2 -----------------------------------
		GLint spotLightPosLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].position");
		GLint spotLightDirLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].direction");
		GLint spotLightCutoffLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].cutoff");
		GLint spotLightOuterCutoffLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].outerCutoff");
		GLint spotLightAmbientLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].ambient");
		GLint spotLightDiffuseLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].diffuse");
		GLint spotLightSpecularLoc2 = glGetUniformLocation(shader_model.id, "spotlight[1].specular");

		glUniform3f(spotLightPosLoc2, lamp_light1_position.x, lamp_light1_position.y, lamp_light1_position.z);
    glUniform3f(spotLightDirLoc2, 0.0f, 1.0f, 0.0f);
    float cutoffAngle2 = 45.0f; // in degrees
		float outerCutoffAngle2 = 50.0f; // in degrees
		glUniform1f(spotLightCutoffLoc2, cos(glm::radians(cutoffAngle2)));
		glUniform1f(spotLightOuterCutoffLoc2, cos(glm::radians(outerCutoffAngle2)));
		glUniform3f(spotLightAmbientLoc2, 0.3f, 0.3f, 0.0f); // Yellow ambient
		glUniform3f(spotLightDiffuseLoc2, 0.3f, 0.3f, 0.0f); // Yellow diffuse
		glUniform3f(spotLightSpecularLoc2, 0.08f, 0.08f, 0.0f); // Yellow specu
		// -----------------------------------
		// Spot Light 3 -----------------------------------
		GLint spotLightPosLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].position");
		GLint spotLightDirLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].direction");
		GLint spotLightCutoffLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].cutoff");
		GLint spotLightOuterCutoffLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].outerCutoff");
		GLint spotLightAmbientLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].ambient");
		GLint spotLightDiffuseLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].diffuse");
		GLint spotLightSpecularLoc3 = glGetUniformLocation(shader_model.id, "spotlight[2].specular");

		glUniform3f(spotLightPosLoc3, lamp_light2_position.x, lamp_light2_position.y, lamp_light2_position.z);
    glUniform3f(spotLightDirLoc3, 0.0f, 1.0f, 0.0f);
    float cutoffAngle3 = 45.0f; // in degrees
		float outerCutoffAngle3 = 50.0f; // in degrees
		glUniform1f(spotLightCutoffLoc3, cos(glm::radians(cutoffAngle3)));
		glUniform1f(spotLightOuterCutoffLoc3, cos(glm::radians(outerCutoffAngle3)));
		glUniform3f(spotLightAmbientLoc3, 0.3f, 0.3f, 0.0f); // Yellow ambient
		glUniform3f(spotLightDiffuseLoc3, 0.3f, 0.3f, 0.0f); // Yellow diffuse
		glUniform3f(spotLightSpecularLoc3, 0.09f, 0.09f, 0.0f); // Yellow specu
		// ----------------------------------
		
		shader_model.set_int("material.diffuse", 0);
		shader_model.set_float("material.shininess", 1.0f);

		shader_model.set_mat4("projection", projection);
		shader_model.set_mat4("view", view);
		shader_model.set_mat4("model", model);
		ground_model.draw(shader_model);
		pillars_model.draw(shader_model);
		vehicle_model.draw(shader_model);
		vehicle_light1_model.draw(shader_model);
		vehicle_light2_model.draw(shader_model);
		wires_model.draw(shader_model);
		lamps_model.draw(shader_model);
		lamp_light1_model.draw(shader_model);
		lamp_light2_model.draw(shader_model);
		rock1_model.draw(shader_model);
		rock2_model.draw(shader_model);
		rock3_model.draw(shader_model);
		rock4_model.draw(shader_model);
		rock5_model.draw(shader_model);
		rock6_model.draw(shader_model);
		cactus1_model.draw(shader_model);
		cactus2_model.draw(shader_model);
		cactus3_model.draw(shader_model);
		cactus4_model.draw(shader_model);
		cactus5_model.draw(shader_model);
		cactus6_model.draw(shader_model);
		cactus7_model.draw(shader_model);
		cactus8_model.draw(shader_model);
		scene_model.draw(shader_model);
		
		
		// Animation doesnt work???
		yPos -= sin(elapsedTime * -7.0f) * -10.0f * delta_time;
		glm::mat4 modelMatrix = glm::translate(model, glm::vec3(0.0f, yPos, 0.0f));
		shader_model.set_mat4("model", modelMatrix);
		ufo_model.draw(shader_model);
		ufo_light_model.draw(shader_model);
		glUniform3f(spotLightPosLoc, ufoPosition.x, yPos, ufoPosition.z);

		shader_env_map.activate();
		glUniform3f(glGetUniformLocation(shader_env_map.id, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform1i(glGetUniformLocation(shader_env_map.id, "cubemap"), 0);
		shader_env_map.set_mat4("projection", projection);
		shader_env_map.set_mat4("view", view);
		shader_env_map.set_mat4("model", model);
		vehicle_discs_model.draw(shader_env_map);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glDepthMask(GL_FALSE);
		shader_window.activate();
		shader_window.set_mat4("projection", projection);
		shader_window.set_mat4("view", view);
		shader_window.set_mat4("model", model);
		glUniform1f(glGetUniformLocation(shader_window.id, "alpha"), 0.7f); 
		vehicle_windows_model.draw(shader_window);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		
		glDepthFunc(GL_LEQUAL);
		view = glm::mat4(glm::mat3(view));
		skybox_shader.activate();
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(skybox_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


    window.new_frame();
	}
	glDeleteVertexArrays(1, &skybox_vao);
	glDeleteBuffers(1, &skybox_vbo);
	glDeleteBuffers(1, &skybox_ebo);

	glfwTerminate();
}

void Project::process_input(double dt)
{
	if(Keyboard::key(GLFW_KEY_ESCAPE))
	{
		window.set_should_close(true);
	}

	if(Keyboard::key(GLFW_KEY_W))
	{
		camera.update_camera_position(CameraDirection::FORWARD, dt);
	}
	if(Keyboard::key(GLFW_KEY_S))
	{
		camera.update_camera_position(CameraDirection::BACKWARD, dt);
	}
	if(Keyboard::key(GLFW_KEY_D))
	{
		camera.update_camera_position(CameraDirection::RIGHT, dt);
	}
	if(Keyboard::key(GLFW_KEY_A))
	{
		camera.update_camera_position(CameraDirection::LEFT, dt);
	}
	if(Keyboard::key(GLFW_KEY_SPACE))
	{
		camera.update_camera_position(CameraDirection::UP, dt);
	}	
	if(Keyboard::key(GLFW_KEY_LEFT_CONTROL))
	{
		camera.update_camera_position(CameraDirection::DOWN, dt);
	}

	double dx = Mouse::get_mouse_dx(), dy = Mouse::get_mouse_dy();
	if(dx != 0 || dy != 0)
	{
		camera.update_camera_direction(dx, dy);
	}

	double scroll_dy = Mouse::get_mouse_scroll_dy();
	if(scroll_dy != 0)
	{
		camera.update_camera_zoom(scroll_dy);
	}
}