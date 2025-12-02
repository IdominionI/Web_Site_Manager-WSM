#pragma once

#include <Framework/AFW/Application/afw_global.h> // Must be placed here above all other includes

#include <glfw/glfw3.h>

#include <cstdio>

#define MOUSE_INACTIVE -1

struct mouse_interaction_struct_type {
	bool enabled        = false;
	int  action		    = GLFW_RELEASE;
	int	 button_code    = -1;
	int	 modifiers      = -1;
	int	 interaction_id = -1;
};

struct mouse_interaction_context_struct_type {
	int interaction_context_id = -1;
	std::vector<mouse_interaction_struct_type> mouse_interactions;
};

class glfw_mouse_event_class {
public:
	glfw_mouse_event_class() {}
	~glfw_mouse_event_class() {}

    void enable_mouse_button_repeat(GLFWwindow *glfw_window) {
        glfwSetInputMode(glfw_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    }

    void disable_mouse_button_repeat(GLFWwindow* glfw_window) {
        glfwSetInputMode(glfw_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
    }

    // ************** Mouse button event ***********************

	static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			//printf("button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS\n");

        if (!window) {
            afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to initialize GLFW mouse call back : Window not definde! ");
            return;
        }

        //printf("hereh\n");

        event_window  = window;
        button_action = action;
        active_button = button;
        button_mods   = mods;

        if (action == GLFW_PRESS) {
           // printf("GLFW_PRESS\n");
            button_pressed  = true;
            button_released = false;

            active_button = button;
        }

        if (action == GLFW_RELEASE) {
           // printf("GLFW_RELEASE\n");
            button_pressed  = false;
            button_released = true;
        }
	}

    // Variables to be referenced or modified within call back functions must allways be static inline
    static inline bool button_pressed  = false;
    static inline bool button_released = false;

    static inline GLFWwindow* event_window = nullptr;
    static inline int active_button = MOUSE_INACTIVE;
    static inline int button_action = MOUSE_INACTIVE;
    static inline int button_mods   = MOUSE_INACTIVE;

    void reset_button_event() {
        GLFWwindow* event_window = nullptr;
        active_button = MOUSE_INACTIVE;
        button_action = MOUSE_INACTIVE;
        button_mods   = MOUSE_INACTIVE;
    }

    // ********************* mouse scroll event *********************
    static void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//printf("glfw_mouse_scroll_callback %f : %f\n", xoffset, yoffset);

        if (!window) {
            printf("!window\n");
            return;
        }

        mouse_scroll_active = true;

        y_scroll += yoffset;
        x_scroll += xoffset;

        delta_y_scroll += 0;
        delta_x_scroll += 0;
    }

    static inline bool mouse_scroll_active = false;

    static inline double y_scroll       = 0;
    static inline double delta_y_scroll = 0;
    static inline double x_scroll       = 0;
    static inline double delta_x_scroll = 0;

    void reset_scroll_event() {
        y_scroll       = 0;
        delta_y_scroll = 0;
        x_scroll       = 0;
        delta_x_scroll = 0;
        mouse_scroll_active = false;
    }

    void reset_scroll_delta() {
        delta_y_scroll = 0;
        delta_x_scroll = 0;
    }

 protected:

private:
};