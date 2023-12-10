#ifndef MOUSE_H
#define MOUSE_H

#include <other/glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse
{
	public:
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_wheel_callback(GLFWwindow* window, double dx, double dy);

	static double get_mouse_x();
	static double get_mouse_y();

	static double get_mouse_dx();
	static double get_mouse_dy();

	static double get_mouse_scroll_dx();
	static double get_mouse_scroll_dy();

	static bool button(int button);
	static bool button_changed(int button);
	static bool button_down(int button);
	static bool button_up(int button); 

	private:
	static double x;
	static double y;

	static double last_x;
	static double last_y;

	static double dx;
	static double dy;

	static double scroll_dx;
	static double scroll_dy;

	static bool first_mouse;

	static bool buttons[];
	static bool buttons_changed[];
};

#endif