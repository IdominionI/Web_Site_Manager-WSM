#pragma once
#include <glfw/glfw3.h>

#include <cstdio>

//#include <FrameWork/AFW/Window/glfw_window_base.h>

//class glfw_window_base_class;

#define INACTIVE -1

class glfw_cursor_event_class {
public:
    glfw_cursor_event_class() {}
    ~glfw_cursor_event_class() {}

    // Call back functions must always be static
    static void cursor_enter_callback(GLFWwindow* window, int entered) {
        if (!window) {
            printf("!window\n");
            return;
        }
        cursor_entered = entered;
    }


    // Variables to be referenced or modified within call back functions must allways be static inline
    static inline bool cursor_entered = false;

    static inline GLFWwindow* event_window = nullptr;

    void reset_cursor_enter_event() {
        GLFWwindow* event_window = nullptr;
        cursor_entered = false;
    }

    // -------------------------------------------------

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        if (!window) {
            printf("!window\n");
            return;
        }

        cursor_window_x = xpos;
        cursor_window_y = ypos;
        cursor_window_pos = { xpos,ypos };
        //printf("cursor position callback x : %f :: y : %f\n",xpos,ypos);
    }

    static inline double cursor_window_x;
    static inline double cursor_window_y;
    static inline glm::vec2 cursor_window_pos;

protected:

private:
};