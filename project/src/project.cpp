#include "project.h"

#include <other/stb/stb_image.h>

#include "render_util/camera.h"

#include "io/mouse.h"
#include "io/keyboard.h"

#include "window.h"
#include "render_util/shader.h"
#include "render_util/texture.h"
#include "render/model.h"

#include "sound/sound_device.h"
#include "sound/sound_buffer.h"
#include "sound/sound_source.h"

#include "render/skybox.h"

#include <vector>

glm::mat4 transform = glm::mat4(1.0f);

float delta_time = 0.0f;
float last_frame = 0.0f;

Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));

void Project::initialize()
{
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
	//glDepthRange(0 ,30);
}


void Project::run_render()
{
	initialize();
	// Shaders compile
	Shader shader_model((get_asset_path()  / "shaders/model_vert.glsl").string().c_str(), (get_asset_path()  / "shaders/model_frag.glsl").string().c_str());
	Shader shader_env_map((get_asset_path()  / "shaders/env_map_vert.glsl").string().c_str(), (get_asset_path()  / "shaders/env_map_frag.glsl").string().c_str());
	Shader skybox_shader((get_asset_path() / "shaders/skybox_vert.glsl").string().c_str(), (get_asset_path() / "shaders/skybox_frag.glsl").string().c_str());
	Shader light_shader((get_asset_path() / "shaders/light_vert.glsl").string().c_str(), (get_asset_path() / "shaders/light_frag.glsl").string().c_str());
	// Load models
	Model ground_model(_strdup((get_asset_path()  / "model/ground.obj").string().c_str()));
	Model pillars_model(_strdup((get_asset_path()  / "model/pillars.obj").string().c_str()));
	Model vehicle_model(_strdup((get_asset_path()  / "model/vehicle.obj").string().c_str()));
	Model vehicle_discs_model(_strdup((get_asset_path()  / "model/vehicle_discs.obj").string().c_str()));
	Model vehicle_windows_model(_strdup((get_asset_path()  / "model/vehicle_windows.obj").string().c_str()));
	Model vehicle_light1_model(_strdup((get_asset_path()  / "model/vehicle_light_one.obj").string().c_str()));
	Model vehicle_light2_model(_strdup((get_asset_path()  / "model/vehicle_light_two.obj").string().c_str()));
	Model wires_model(_strdup((get_asset_path()  / "model/wires.obj").string().c_str()));
	Model lamps_model(_strdup((get_asset_path()  / "model/lamps.obj").string().c_str()));
	Model lamp_light1_model(_strdup((get_asset_path()  / "model/lamp_light_one.obj").string().c_str()));
	Model lamp_light2_model(_strdup((get_asset_path()  / "model/lamp_light_two.obj").string().c_str()));
	Model lamp_light3_model(_strdup((get_asset_path()  / "model/lamp_light_three.obj").string().c_str()));
	Model rock1_model(_strdup((get_asset_path()  / "model/rock1.obj").string().c_str()));
	Model rock2_model(_strdup((get_asset_path()  / "model/rock2.obj").string().c_str()));
	Model rock3_model(_strdup((get_asset_path()  / "model/rock3.obj").string().c_str()));
	Model rock4_model(_strdup((get_asset_path()  / "model/rock4.obj").string().c_str()));
	Model rock5_model(_strdup((get_asset_path()  / "model/rock5.obj").string().c_str()));
	Model rock6_model(_strdup((get_asset_path() / "model/rock6.obj").string().c_str()));
	Model cactus1_model(_strdup((get_asset_path()  / "model/cactus_1.obj").string().c_str()));
	Model cactus2_model(_strdup((get_asset_path()  / "model/cactus_2.obj").string().c_str()));
	Model cactus3_model(_strdup((get_asset_path()  / "model/cactus_3.obj").string().c_str()));
	Model cactus4_model(_strdup((get_asset_path()  / "model/cactus_4.obj").string().c_str()));
	Model cactus5_model(_strdup((get_asset_path()  / "model/cactus_5.obj").string().c_str()));
	Model cactus6_model(_strdup((get_asset_path()  / "model/cactus_6.obj").string().c_str()));
	Model cactus7_model(_strdup((get_asset_path()  / "model/cactus_7.obj").string().c_str()));
	Model cactus8_model(_strdup((get_asset_path()  / "model/cactus_8.obj").string().c_str()));
	Model scene_model(_strdup((get_asset_path()  / "model/scene.obj").string().c_str()));
	Model ufo_model(_strdup((get_asset_path()  / "model/ufo.obj").string().c_str()));
	Model ufo_light_model(_strdup((get_asset_path()  / "model/ufo_light.obj").string().c_str()));
	Model bush_model(_strdup((get_asset_path()  / "model/bush.obj").string().c_str()));
	Model bush_model2(_strdup((get_asset_path()  / "model/bush.obj").string().c_str()));

	Skybox skybox(get_asset_path());
	
	// Variables setup
	// Ufo model & light animation setup
	glm::vec3 ufoPosition = ufo_light_model.get_position();
	glm::vec3 lamp_light1_position = lamp_light1_model.get_position();
	glm::vec3 lamp_light2_position = lamp_light2_model.get_position();
	float yPos = ufo_model.get_position().y;
	float elapsedTime = 0.0f;

	// Light attenuation setup
	float constant = 1.0f; 
	float linear = 0.005f; 
	float quadratic = 0.005f;
	
	while (!window.should_close())
	{
		// Time
		double current_time = glfwGetTime();
		delta_time = current_time - last_frame;
		last_frame = current_time;
		elapsedTime += delta_time;

		// Input
    process_input(delta_time);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.update();

		// Views
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.get_view_matrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.get_zoom()), (float)Window::window_width / (float)Window::window_height, 0.1f, 100.0f);
		glm::vec3 camera_pos = camera.get_camera_pos();

		/* SKYBOX RENDER */
		skybox_shader.activate();
		view = glm::mat4(glm::mat3(view));		// View for env mapping 
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
		skybox.draw(skybox_shader);

		// View after turn off lequial
		view = camera.get_view_matrix();
		
		// models
		shader_model.activate();

		/* LIGHTING RENDER */
		// Global light--------------------------------
		GLint lightDirLoc = glGetUniformLocation(shader_model.id, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(shader_model.id, "viewPos");
		glUniform3f(lightDirLoc, 0.0f, -200.0f, -30.0f);
		glUniform3f(viewPosLoc, camera.get_camera_pos().x, camera.get_camera_pos().y, camera.get_camera_pos().z);
		glUniform3f(glGetUniformLocation(shader_model.id, "light.ambient"), 0.1f, 0.1f, 0.2f);  // Dark blue ambient
		glUniform3f(glGetUniformLocation(shader_model.id, "light.diffuse"), 0.2f, 0.2f, 0.6f);  // Medium blue diffuse
		glUniform3f(glGetUniformLocation(shader_model.id, "light.specular"), 0.02f, 0.02f, 0.06f); // Medium blue specular

		// Spot Light 1 -----------------------------------
		GLint spotLightPosLoc = glGetUniformLocation(shader_model.id, "spotlight[0].position");
		GLint spotLightDirLoc = glGetUniformLocation(shader_model.id, "spotlight[0].direction");
		GLint spotLightCutoffLoc = glGetUniformLocation(shader_model.id, "spotlight[0].cutoff");
		GLint spotLightOuterCutoffLoc = glGetUniformLocation(shader_model.id, "spotlight[0].outerCutoff");
		GLint spotLightAmbientLoc = glGetUniformLocation(shader_model.id, "spotlight[0].ambient");
		GLint spotLightDiffuseLoc = glGetUniformLocation(shader_model.id, "spotlight[0].diffuse");
		GLint spotLightSpecularLoc = glGetUniformLocation(shader_model.id, "spotlight[0].specular");
    glUniform3f(spotLightDirLoc, 0.0f, 1.0f, 0.0f);
    float cutoffAngle = 65.0f;
		float outerCutoffAngle = 70.0f;
		glUniform1f(spotLightCutoffLoc, cos(glm::radians(cutoffAngle)));
		glUniform1f(spotLightOuterCutoffLoc, cos(glm::radians(outerCutoffAngle)));
    glUniform3f(spotLightAmbientLoc, 0.1f, 0.3f, 0.1f);
    glUniform3f(spotLightDiffuseLoc, 0.1f, 2.f, 0.1f);
    glUniform3f(spotLightSpecularLoc, 0.0f, 0.2f, 0.0f);

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
    float cutoffAngle2 = 85.0f;
		float outerCutoffAngle2 = 90.0f; 
		glUniform1f(spotLightCutoffLoc2, cos(glm::radians(cutoffAngle2)));
		glUniform1f(spotLightOuterCutoffLoc2, cos(glm::radians(outerCutoffAngle2)));
		glUniform3f(spotLightAmbientLoc2, 0.5f, 0.5f, 0.0f);
		glUniform3f(spotLightDiffuseLoc2, 0.5f, 0.5f, 0.0f);
		glUniform3f(spotLightSpecularLoc2, 0.08f, 0.08f, 0.0f); 

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
    float cutoffAngle3 = 85.0f; 
		float outerCutoffAngle3 = 90.0f; 
		glUniform1f(spotLightCutoffLoc3, cos(glm::radians(cutoffAngle3)));
		glUniform1f(spotLightOuterCutoffLoc3, cos(glm::radians(outerCutoffAngle3)));
		glUniform3f(spotLightAmbientLoc3, 0.3f, 0.3f, 0.0f); 
		glUniform3f(spotLightDiffuseLoc3, 0.8f, 0.4f, 0.0f); 
		glUniform3f(spotLightSpecularLoc3, 0.1f, 0.1f, 0.0f);
		
		// Spot Light 4 -----------------------------------
		GLint spotLightPosLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].position");
		GLint spotLightRotLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].rotation");
		GLint spotLightDirLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].direction");
		GLint spotLightCutoffLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].cutoff");
		GLint spotLightOuterCutoffLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].outerCutoff");
		GLint spotLightAmbientLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].ambient");
		GLint spotLightDiffuseLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].diffuse");
		GLint spotLightSpecularLoc4 = glGetUniformLocation(shader_model.id, "spotlight[3].specular");
		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
		glUniform3f(spotLightPosLoc4, lamp_light3_model.get_position().x, lamp_light3_model.get_position().y, lamp_light3_model.get_position().z);
    glUniform3f(spotLightDirLoc4, 0.0f, 1.0f, 0.0f);
    float cutoffAngle4 = 85.0f;
		float outerCutoffAngle4 = 90.0f;
		glUniform1f(spotLightCutoffLoc4, cos(glm::radians(cutoffAngle4)));
		glUniform1f(spotLightOuterCutoffLoc4, cos(glm::radians(outerCutoffAngle4)));
		glUniform3f(spotLightAmbientLoc4, 0.8f, 0.8f, 0.0f);
		glUniform3f(spotLightDiffuseLoc4, 0.8f, 0.8f, 0.0f);
		glUniform3f(spotLightSpecularLoc4, 0.08f, 0.08f, 0.0f);

		for(int i = 0; i < 4; i++)
    {
			glUniform1f(glGetUniformLocation(shader_model.id, ("spotlight[" + std::to_string(i) + "].constant").c_str()), constant);
			glUniform1f(glGetUniformLocation(shader_model.id, ("spotlight[" + std::to_string(i) + "].linear").c_str()), linear);
			glUniform1f(glGetUniformLocation(shader_model.id, ("spotlight[" + std::to_string(i) + "].quadratic").c_str()), quadratic);
    }
		
		// Material properties 
		shader_model.set_int("material.diffuse", 0);
		shader_model.set_float("material.shininess", 1.0f);

		// Draw models
		shader_model.set_mat4("projection", projection);
		shader_model.set_mat4("view", view);
		shader_model.set_mat4("model", model);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		ground_model.draw(shader_model);
		pillars_model.draw(shader_model);
		vehicle_light1_model.draw(shader_model);
		vehicle_light2_model.draw(shader_model);
		wires_model.draw(shader_model);
		lamps_model.draw(shader_model);
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
		glDisable(GL_CULL_FACE);
		vehicle_model.draw(shader_model);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// Draw env mapping models
		glUniform3f(spotLightPosLoc, 0, yPos+5, 65);
		shader_env_map.activate();
		shader_env_map.set_mat4("projection", projection);
		shader_env_map.set_mat4("view", view);
		yPos -= sin(elapsedTime * 2.0f) * -7.0f * delta_time;
		glm::mat4 modelMatrix = glm::translate(model, glm::vec3(0.0f, yPos, 0.0f));
		shader_env_map.set_mat4("model", modelMatrix);
		glUniform3f(glGetUniformLocation(shader_env_map.id, "cameraPos"), camera_pos.x, camera_pos.y, camera_pos.z);
		glUniform1i(glGetUniformLocation(shader_env_map.id, "cubemap"), 0);
		ufo_model.draw(shader_env_map);
		shader_env_map.set_mat4("model", model);
		//vehicle_discs_model.draw(shader_env_map);
		glDisable(GL_CULL_FACE);
		
		light_shader.activate();
		light_shader.set_mat4("projection", projection);
		light_shader.set_mat4("view", view);
		light_shader.set_mat4("model", model);
		glUniform3f(glGetUniformLocation(light_shader.id, "objectColor"),  0.7f, 0.6f, 0.0f);
		lamp_light1_model.draw(light_shader);
		lamp_light2_model.draw(light_shader);
		lamp_light3_model.draw(light_shader);
		glUniform3f(glGetUniformLocation(light_shader.id, "objectColor"),  0.1f, 0.5f, 0.1f);
		light_shader.set_mat4("model", modelMatrix);
		ufo_light_model.draw(light_shader);

		// Draw transparent models
		shader_model.activate();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glDepthMask(GL_FALSE);
		glUniform1f(glGetUniformLocation(shader_model.id, "alpha"), 0.7f); 
		vehicle_windows_model.draw(shader_model);
		glUniform1f(glGetUniformLocation(shader_model.id, "alpha"), 1.0f); 
		bush_model.draw(shader_model);
		// Chnaging position of object
		// use prev with anim
		//modelMatrix += glm::translate(model, glm::vec3(-235, 0, 12));
		// define new
		modelMatrix = glm::translate(model, glm::vec3(-25, 0, 12));
		shader_model.set_mat4("model", modelMatrix);
		//glRotatef(45.0f, 0.0f, 1.0f, 0.0f); // apply rotation
		// draw the object here
		bush_model2.draw(shader_model);
		//-------
		//bush_model2.draw(shader_model);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

    window.new_frame();
	}
	// Delete shader programs after termination of render loop
	glDeleteProgram(skybox_shader.id);
	glDeleteProgram(shader_model.id);
	glDeleteProgram(shader_env_map.id);

	glfwTerminate();
}


void Project::run_sound()
{
	ALint state = AL_PLAYING;

	SoundDevice* sound_device = SoundDevice::get();

	uint32_t sound1 = SoundBuffer::get()->add_sound_effect((get_asset_path() / "sound/wind.snd").string().c_str());
	
	SoundSource source1;
	source1.play(sound1);
	while (!window.should_close())
	{
		source1.loop(state);
	}
}

std::filesystem::path Project::get_asset_path()
{
	std::filesystem::path asset_path = "assets";
	return asset_path;
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