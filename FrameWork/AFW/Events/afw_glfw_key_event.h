#pragma once

#include <Framework/AFW/Application/afw_global.h> // Must be placed here above all other includes

#include <glfw/glfw3.h>

#include <cstdio>

//#include <FrameWork/AFW/Window/glfw_window_base.h>

//class glfw_window_base_class;

#define KEY_INACTIVE -1

//enum class key_event_type_enum {pressed, release, repeat, none};
//--------------------------------------------
//
// 	Keyboard definitions
//
// 	ok -- why this?
// 	glut key commands have some annoying features,
// 	in that some normal keys have the same value as special keys,
// 	but we want ONE key routine, so we need to redefine several,
// 	so that we get some normalacy across keys routines
//
// 	(everything that comes through "glutSpecialKeyFunc" will get 256 added to it,
// 	to avoid conflicts, before, values like "left, right up down" (ie, 104, 105, 106) were conflicting with
// 	letters.. now they will be 256 + 104, 256 + 105....)

enum ofKey {
	OF_KEY_RETURN = 13,
	OF_KEY_ESC = 27,
	OF_KEY_TAB = 9,


	OF_KEY_BACKSPACE = 8,
	OF_KEY_DEL = 127,


	// For legacy reasons we are mixing up control keys
	// and unicode codepoints when sending key events,
	// for the modifiers that need to be usable as bitmask
	// we are using some control codes that have nothing to do
	// with the keys being represented and then 0x0ee0.. and 0x0e60...
	// which are free in the unicode table

	OF_KEY_SHIFT = 0x1,
	OF_KEY_CONTROL = 0x2,
	OF_KEY_ALT = 0x4,
	OF_KEY_SUPER = 0x10,
	OF_KEY_COMMAND = OF_KEY_SUPER,
	OF_KEY_LEFT_SHIFT = 0xe60,
	OF_KEY_RIGHT_SHIFT = 0xe61,
	OF_KEY_LEFT_CONTROL = 0xe62,
	OF_KEY_RIGHT_CONTROL = 0xe63,
	OF_KEY_LEFT_ALT = 0xe64,
	OF_KEY_RIGHT_ALT = 0xe65,
	OF_KEY_LEFT_SUPER = 0xe66,
	OF_KEY_RIGHT_SUPER = 0xe67,
	OF_KEY_LEFT_COMMAND = OF_KEY_LEFT_SUPER,
	OF_KEY_RIGHT_COMMAND = OF_KEY_RIGHT_SUPER,

	// Use values from the Unicode private use codepoint range E000 - F8FF. 
	// See https://www.unicode.org/faq/private_use.html
	OF_KEY_F1 = 0xe000,
	OF_KEY_F2 = 0xe001,
	OF_KEY_F3 = 0xe002,
	OF_KEY_F4 = 0xe003,
	OF_KEY_F5 = 0xe004,
	OF_KEY_F6 = 0xe005,
	OF_KEY_F7 = 0xe006,
	OF_KEY_F8 = 0xe007,
	OF_KEY_F9 = 0xe008,
	OF_KEY_F10 = 0xe009,
	OF_KEY_F11 = 0xe00A,
	OF_KEY_F12 = 0xe00B,
	OF_KEY_LEFT = 0xe00C,
	OF_KEY_UP = 0xe00D,
	OF_KEY_RIGHT = 0xe00E,
	OF_KEY_DOWN = 0xe00F,
	OF_KEY_PAGE_UP = 0xe010,
	OF_KEY_PAGE_DOWN = 0xe011,
	OF_KEY_HOME = 0xe012,
	OF_KEY_END = 0xe013,
	OF_KEY_INSERT = 0xe014,

	OF_MOUSE_BUTTON_1 = 0,
	OF_MOUSE_BUTTON_2 = 1,
	OF_MOUSE_BUTTON_3 = 2,
	OF_MOUSE_BUTTON_4 = 3,
	OF_MOUSE_BUTTON_5 = 4,
	OF_MOUSE_BUTTON_6 = 5,
	OF_MOUSE_BUTTON_7 = 6,
	OF_MOUSE_BUTTON_8 = 7,
	OF_MOUSE_BUTTON_LAST = OF_MOUSE_BUTTON_8,
	OF_MOUSE_BUTTON_LEFT = OF_MOUSE_BUTTON_1,
	OF_MOUSE_BUTTON_MIDDLE = OF_MOUSE_BUTTON_2,
	OF_MOUSE_BUTTON_RIGHT = OF_MOUSE_BUTTON_3,
};


//enum class key_action_type_enum{ pressed = GLFW_PRESS, released = GLFW_RELEASE, repeat = GLFW_REPEAT};

#define AFW_PRESS_REPEAT 3

struct key_interaction_struct_type {
	bool enabled        = false;
	int  action		    = GLFW_RELEASE;
	int	 key_code       = -1;
	int	 modifiers      = -1;
	int	 interaction_id = -1;
};

struct key_interaction_context_struct_type {
	int interaction_context_id = -1;
	std::vector<key_interaction_struct_type> key_interactions;
};

//std::vector<key_interaction_struct_type> key_interactions;

class glfw_key_event_class {
public:
    glfw_key_event_class() {}
    ~glfw_key_event_class() {}

    // Call back functions must always be static
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (!window) {
            afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Unable to initialize GLFW key call back : Window not definde! ");
            return;
        }

       // printf("hereh\n");

        event_window = window;
        type = action;
        key_code = key;
        modifiers = mods;

        if (action == GLFW_PRESS) {
           // printf("GLFW_PRESS\n");
            key_pressed  = true;
            key_released = false;
            key_repeat   = false;

            key_code     = key;
        }

        if (action == GLFW_RELEASE) {
          // printf("GLFW_RELEASE\n");
            key_pressed  = false;
            key_released = true;
            key_released = false;
        }

        if (action == GLFW_REPEAT) {
           // printf("GLFW_REPEAT\n");
            key_pressed  = true;
            key_released = false;
            key_repeat   = true;
        }
    }


    // Variables to be referenced or modified within call back functions must allways be static inline
    static inline bool key_pressed = false;
    static inline bool key_released = false;
    static inline bool key_repeat = false;

    static inline GLFWwindow* event_window = nullptr;
    static inline int key_code  = KEY_INACTIVE;
    static inline int type      = KEY_INACTIVE;
    static inline int modifiers = KEY_INACTIVE;

    void reset_key_event() {
        GLFWwindow* event_window = nullptr;
        key_code  = KEY_INACTIVE;
        type      = KEY_INACTIVE;
        modifiers = KEY_INACTIVE;
       // key_event_type = key_event_type_enum::none;
    }

   // key_event_type_enum key_event_type = key_event_type_enum::none;

protected:

private:
};