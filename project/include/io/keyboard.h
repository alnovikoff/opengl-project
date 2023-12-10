#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <other/glad/glad.h>
#include <GLFW/glfw3.h>

class Keyboard
{
	public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


	static bool key(int key);
	static bool key_changed(int key);
	static bool key_down(int key);
	static bool key_up(int key);

	private:
	static bool keys[];
	static bool keys_changes[];
};

#endif