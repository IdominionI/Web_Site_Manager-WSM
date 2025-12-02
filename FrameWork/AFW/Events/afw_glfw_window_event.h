#pragma once

#include <Framework/AFW/Application/afw_global.h> // Must be placed here above all other includes

#include <glfw/glfw3.h>

#include <cstdio>

class glfw_window_event_class {
public:
	glfw_window_event_class() {}
	~glfw_window_event_class() {}

	static void window_move_callback(GLFWwindow* window, int xpos, int ypos) {
		if (!window) {
			afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to perform GLFW window move call back : Window not defined! ");
			return;
		}

		window_moved = true;
		x_pos        = xpos;
		y_pos        = ypos;
		moved_position = { xpos ,ypos };
	}

	static inline bool window_moved = false;
	static inline int  x_pos = 0;
	static inline int  y_pos = 0;
	static inline glm::vec2 moved_position = { 0,0 };

	void end_window_moved() {
		window_moved = false;
	}

	static void window_resize_callback(GLFWwindow* window, int width, int height) {
		if (!window) {
			afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to perform GLFW window resize call back : Window not defined! ");
			return;
		}
	
		window_resized = true;
		resized_width  = width;
		resized_height = height;
		resized_size = { width, height };
	}

	static inline bool window_resized = false;

	static inline int resized_width  = 0;
	static inline int resized_height = 0;
	static inline glm::ivec2 resized_size = { 0,0 };

	static inline int previous_width = 0;
	static inline int previous_height = 0;

	void end_window_resized() {
		previous_width  = resized_width; // Not sure this will work
		previous_height = resized_height;// Not sure this will work
		window_resized = false;

		resized_width = 0;
		resized_height = 0;
		resized_size = { 0,0 };
	}

	// -----------------------------------------------------

	static void window_refreah_callback(GLFWwindow* window) {
		if (!window) {
			afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to perform GLFW window refresh call back : Window not defined! ");
			return;
		}
		// Window refresh actions to go here or needs to be in appl
	}
	
	// --------------------------------------------

	static void window_focus_callback(GLFWwindow* window, int focused) {
		if (!window) {
			afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to perform GLFW window focus call back : Window not defined! ");
			return;
		}
		//printf("window_focus_callback %i\n",focused);

		window_focused = focused;

		//printf("Window refresh actions to go here or needs to be in appl\n");
	}

	static inline int window_focused = -1;

	//void unfocus_
	// ------------------------------------------------
	static void window_maximised_callback(GLFWwindow* window, int maximized) {
		if (!window) {
			afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to perform GLFW window maximise call back : Window not definde! ");
			return;
		}
		//printf("window_maximized_callback %i\n", maximized);

		window_maximized = maximized;

		//printf("Window refresh actions to go here or needs to be in appl\n");
	}

	static inline int window_maximized = 0;
protected:

private:


};