#include <io/mouse.h>

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::last_x = 0;
double Mouse::last_y = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scroll_dx = 0;
double Mouse::scroll_dy = 0;

bool Mouse::first_mouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttons_changed[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::cursor_position_callback(GLFWwindow *window, double x, double y)
{
	x = x;
	y = y;

	if(first_mouse)
	{
		last_x = x;
		last_y = y;
		first_mouse = false;
	}

	dx = x - last_x;
	dy = last_y - y;
	last_x = x;
	last_y = y;
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(action != GLFW_RELEASE)
	{
		if(!buttons[button])
		{
			buttons[button] = true;
		}
	}
	else
	{
		buttons[button] = false;
	}
	buttons_changed[button] = action != GLFW_REPEAT;
}

void Mouse::mouse_wheel_callback(GLFWwindow* window, double dx, double dy)
{
	scroll_dx = dx;
	scroll_dy = dy;
}

double Mouse::get_mouse_x()
{
	return x;
}

double Mouse::get_mouse_y()
{
	return y;
}

double Mouse::get_mouse_dx()
{
	double _dx = dx;
	dx = 0;
	return _dx;
}

double Mouse::get_mouse_dy()
{
	double _dy = dy;
	dy = 0;
	return _dy;
}

double Mouse::get_mouse_scroll_dx()
{
	double _dx = scroll_dx;
	scroll_dx = 0;
	return _dx;
}

double Mouse::get_mouse_scroll_dy()
{
	double _dy = scroll_dy;
	scroll_dy = 0;
	return _dy;
}

bool Mouse::button(int button)
{
	return buttons[button];
}

bool Mouse::button_changed(int button)
{
	bool ret = buttons_changed[button];
	buttons_changed[button] = false;
	return ret;
}

bool Mouse::button_down(int button)
{
	return !buttons[button] && button_changed(button);
}

bool Mouse::button_up(int button)
{
	return buttons[button] && button_changed(button);
}
