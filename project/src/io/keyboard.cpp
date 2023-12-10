#include <io/keyboard.h>

bool Keyboard::keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::keys_changes[GLFW_KEY_LAST] = { 0 };


void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action != GLFW_RELEASE)
	{
		if(!keys[key])
		{
			keys[key] = true;
		}
	}
	else
	{
		keys[key] = false;
	}
	keys_changes[key] == action != GLFW_REPEAT;
}


bool Keyboard::key(int key)
{
	return keys[key];
}

bool Keyboard::key_changed(int key)
{
	bool ret = keys_changes[key];
	keys_changes[key] = false;
	return ret;
}

bool Keyboard::key_down(int key)
{
	return keys[key] && key_changed(key);
}

bool Keyboard::key_up(int key)
{
	return !keys[key] && key_changed(key);
}
