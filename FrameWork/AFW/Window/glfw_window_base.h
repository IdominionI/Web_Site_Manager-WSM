#pragma once

#define GLFW_INCLUDE_NONE // ++++++++++++++++
#include <GLFW/glfw3.h>// Must be first include statement 
//#include <glad/glad.h>// ++++++++++++++++

#include <Framework/AFW/Application/afw_global.h>

#include "afw_window_base.h"

#include <FrameWork/AFW/Events/afw_glfw_key_event.h>
#include <FrameWork/AFW/Events/afw_glfw_mouse_event.h>
#include <FrameWork/AFW/Events/afw_glfw_cursor_event.h>
#include <FrameWork/AFW/Events/afw_glfw_window_event.h>


#include <FrameWork/AFW/Utils/afw_Logger.h>

#include <iostream>
// GFLW base window class that have funtions that is common in all GFLW windows 

struct glfw_window_settings_struct {
    int  numSamples  = 4;
    int  redBits     = 8;
    int  greenBits   = 8;
    int  blueBits    = 8;
    int  alphaBits   = 8;
    int  depthBits   = 24;
    int  stencilBits = 0;
    bool stereo      = false;
    bool iconified   = false;
    bool decorated   = true;
    bool transparent = false;
    bool visible     = true;
    bool resizable   = true;
    bool doubleBuffering = true;
    int  monitor = 0;
    bool multiMonitorFullScreen = false;

    bool initialise_callbacks = true;
};

static void glfw_error_callback(const int error, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


struct gflw_event_struct_type {
    glfw_key_event_class    *glfw_key_event   = nullptr;
    glfw_mouse_event_class  *glfw_mouse_event = nullptr;
    glfw_cursor_event_class *glfw_cursor_event = nullptr;
};

class glfw_window_base_class: public afw_window_base_class<GLFWwindow> {
public:
	glfw_window_base_class(){}
    virtual ~glfw_window_base_class() { 
        destroy();
        delete glfw_key_event;
        delete glfw_mouse_event;
        delete glfw_cursor_event;
        delete glfw_window_event;
    }

    // vvvvvvvvvvv GLFW call back system vvvvvvvvvvvvvvvvvv

    //this initialisation must be done after window is created and window_ptr defined
    virtual bool initialise_callbacks(){
        if (!window_ptr) {
            //printf("Unable to create initialise vulcan GLFW window callbacks! :: Window not defined as probably not created or deleted.");
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to create initialise vulcan GLFW window callbacks! :: Window not defined as probably not created or deleted.");
            return false;
        }

        glfwSetErrorCallback(glfw_error_callback);

        glfwSetKeyCallback(window_ptr, glfw_key_event->key_callback);

        glfwSetMouseButtonCallback(window_ptr, glfw_mouse_event->glfw_mouse_button_callback);
        glfwSetScrollCallback(window_ptr, glfw_mouse_event->glfw_mouse_scroll_callback);

        glfwSetCursorEnterCallback(window_ptr, glfw_cursor_event->cursor_enter_callback);
        glfwSetCursorPosCallback(window_ptr, glfw_cursor_event->cursor_position_callback);

        glfwSetWindowSizeCallback(window_ptr, glfw_window_event->window_resize_callback);
        glfwSetWindowRefreshCallback(window_ptr, glfw_window_event->window_refreah_callback);
        glfwSetWindowFocusCallback(window_ptr, glfw_window_event->window_focus_callback);
        glfwSetWindowMaximizeCallback(window_ptr, glfw_window_event->window_maximised_callback);

        glfwSetWindowPosCallback(window_ptr, glfw_window_event->window_move_callback);

        //glfwSetFramebufferSizeCallback(window, Window::FramebufferResizeCallback);
        //glfwSetWindowPosCallback(window, Window::WindowChangePosCallback);
        //glfwSetDropCallback(window, Window::WindowDropCallback);
        return true;
    }

    // ---------------------- Key call back system ----------------------

    bool key_pressed() {
        return(glfw_key_event->key_pressed);
    }

    int active_key() {
        return(glfw_key_event->key_code);
    }

    //more to go here

    glfw_key_event_class   *glfw_key_event   = new glfw_key_event_class;

    // ---------------------- mouse call back system ----------------------

    bool mouse_button_pressed() {
        return(glfw_mouse_event->button_pressed);
    }

    int active_mouse_button() {
        return(glfw_mouse_event->active_button);
    }

    bool mouse_scroll_active() {
        return(glfw_mouse_event->mouse_scroll_active);
    }

    glfw_mouse_event_class *glfw_mouse_event = new glfw_mouse_event_class;

    // ---------------------- Cursor call back system -----------------------

    bool entered_window() {
        return(glfw_cursor_event->cursor_entered);
    }

    glm::vec2 cursor_position() {
        return glfw_cursor_event->cursor_window_pos;
        //return pos;
    }

    glfw_cursor_event_class *glfw_cursor_event = new glfw_cursor_event_class;

    // ---------------------- Window call back system -----------------
    bool is_window_moved() {
        return glfw_window_event->window_moved;
    }




    bool is_window_resized() {
        return glfw_window_event->window_resized;
    }

    glm::ivec2 window_resize() {
        int width, height;
        glfwGetWindowSize(window_ptr, &width, &height);
        set_size(width,height);
        
        return glfw_window_event->resized_size;
        //return pos;
    }

    bool window_focused() {
        return glfw_window_event->window_focused;
    }

    bool window_maximised() {
        return glfw_window_event->window_maximized;
    }

    glfw_window_event_class *glfw_window_event = new glfw_window_event_class;

    // ^^^^^^^^^^^^^^^^ GLFW call back system ^^^^^^^^^^^^^^^^^^^^^^^^

    virtual void define_glfw_settings() = 0;

    inline void poll_events() override {
        glfwPollEvents();
    }

    void resize_window(int width, int height) override { set_window_size(width, height); }
    void resize_window(glm::ivec2 size) override { set_window_size(size); }
    void move_window(int x, int y) override { set_window_position(x, y); }
    void move_window(glm::ivec2 position) override { set_window_position(position); }


    virtual void inititialise_window_frame_buffer() = 0;
    virtual void perform_window_move_action() {}
    //virtual void perform_window_enterd_action() {}
    virtual void perform_window_resize_action() {}

    virtual void destroy() {
        glfwDestroyWindow(window_ptr);
    }

    virtual void close() {
        destroy();
    }




    // -----------------------------------------

    virtual void set_key_call_backs() {
        //glfwSetKeyCallback(window_ptr, close_window_callback);
        // Others to go below
    }

    virtual void set_frame_buffer_resize_callback() {
        //glfwSetFramebufferSizeCallback(window_ptr, framebufferResizeCallback);
    }

    void set_window_should_close(int value) {
        glfwSetWindowShouldClose(window_ptr,value);
    }

    // -----------------------------------------

    virtual void set_window_icon() { // In case someone wants to use or create default icon
        //glfwSetWindowIcon(GLFWwindow* window, int count, const GLFWimage* images);
    }

    // !!!! This needs to be tested !!!!!
    virtual void set_size_limits(int min_width, int min_height, int max_width, int max_height) {
        if (!is_window_resizable()) return;

        if (!is_window_full_screen()) {
            int current_width = get_width();
            int current_height = get_height();

            if (current_width > max_width) 
                set_window_width(max_width);
            else
                if (current_width < min_width) 
                    set_window_width(min_width);

            if (current_height > max_height)
                set_window_height(max_height);
            else
                if (current_height < min_height)
                    set_window_height(min_height);
        }

        glfwSetWindowSizeLimits(window_ptr, min_width, min_height, max_width, max_height);
    }

    virtual void set_aspect_ratio(int numer, int denom) {

    }
    // -----------------------------------------

    int get_window_should_close() {
        return glfwWindowShouldClose(window_ptr);
    }

    void get_frame_buffer_size(int *width,int *height) {
        //int width, height;
        //glm::vec2 w = get_window_size();
        glfwGetFramebufferSize(window_ptr, width, height);
    }

    void get_window_frame_size(int* left, int* top, int* right, int* bottom){
        glfwGetWindowFrameSize(window_ptr, left, top, right, bottom);
    }

    void get_window_content_scale(float* xscale, float* yscale) {
        glfwGetWindowContentScale(window_ptr, xscale, yscale);
    }

    int get_window_attrib(int attribute)  {
        return glfwGetWindowAttrib(window_ptr,attribute);
    }

    // ==========================================================

    bool is_window_resizable() override {
        return(get_window_attrib(GLFW_RESIZABLE));
    }

    bool is_window_full_screen() override {
        return(get_window_attrib(GLFW_MAXIMIZED));
    }

    bool is_window_visible() override {
        return(get_window_attrib(GLFW_VISIBLE));
    }

    bool is_window_hovered() override {
        return (get_window_attrib(GLFW_HOVERED));
    }

    // Define GLFW common functions derived from window base class that applies to all GLFW windows

    // ===================================================
    void set_window_position (glm::ivec2 m_position) override {
        glfwSetWindowPos(window_ptr, m_position.x, m_position.y);
        set_position(m_position);
    }
    void set_window_width(int m_width) override {set_window_size(m_width, get_height());}

    void set_window_height(int m_height) override { set_window_size(get_width(), m_height); }

    void set_window_size(int m_width, int m_height) override {
        glfwSetWindowSize(window_ptr, m_width, m_height);
        set_size(m_width, m_height);
    }

//    void set_window_mode(window_mode_enum m_mode) { set_mode(m_mode); }

    void set_window_title(std::string m_title) override {
        glfwSetWindowTitle(window_ptr, m_title.c_str());
        set_title(m_title);
    }
    
    void set_window_position(int x, int y) override {
        set_window_position(glm::ivec2{ x,y });
    }

    void set_window_size(glm::ivec2 m_size) override {
        glfwSetWindowSize(window_ptr, m_size.x, m_size.y);
        set_size(m_size);
    }

    void set_window_opacity(float m_opacity) override {
        if (m_opacity < 0.0f) m_opacity = 0;
        if (m_opacity > 1.0f) m_opacity = 1.0f;
        glfwSetWindowOpacity(window_ptr, m_opacity);
        set_opacity(m_opacity);
    }

    void set_window_opacity(int m_opacity) {
        set_window_opacity(float(m_opacity)/max_int_opacity_value);
    }

    // --------------------------------------------------
    glm::vec2 get_window_position() override {
        if (window_ptr == nullptr) return glm::vec2(- 1.0, -1.0);

        int x, y;
        glfwGetWindowPos(window_ptr, &x, &y);
        return glm::vec2(x, y); //NOTE: shouldn't this be (y,x) ??????
        
        //return get_position();
    }
    
    int get_window_width() override { return get_width(); }
    
    int get_window_height() override { return get_height(); }
    
    glm::vec2 get_window_size()  override {
        //int width, height;
        //glfwGetWindowSize(window_ptr, &width, &height);
        //return glm::vec2(width, height);
        return get_size();
    }

    //window_mode_enum get_window_mode() { return get_mode(); }
   
    std::string get_window_title() override { return get_title(); }

    float get_window_opacity() override { return get_opacity(); }

    glm::vec2 get_window_center() {
        return { get_width()/2 ,get_height()/2 };
    }

    // ---------------------------------------------
    virtual void hide_window() override {
        glfwHideWindow(window_ptr);
    }

    virtual void show_window() override {
        glfwShowWindow(window_ptr);
    }

    virtual void focus_window() override { glfwFocusWindow(window_ptr); }

    virtual void maximise_window() override {
        glfwMaximizeWindow(window_ptr);
    }

    virtual void minimise_window() override {
        glfwIconifyWindow(window_ptr);
    }
    // ===================================================

    virtual void on_resize(){}
    virtual void on_drag(){}

    void pollEvents() {
        glfwPollEvents();
    }

    virtual void update() {
       // coreEvents->notifyUpdate();

        //show the window right before the first draw call.
        if (bWindowNeedsShowing && window_ptr) {
            glfwShowWindow(window_ptr);
            bWindowNeedsShowing = false;
            //if (targetWindowMode == OF_FULLSCREEN) {
            //    setFullscreen(true);
            //}
        }
    }

    bool	bEnableSetupScreen = false;
    int		window_w, window_h;		/// Physical framebuffer pixels extents
    int		current_w, current_h;		/// Extents of the window client area, which may be scaled by pixelsScreenCoordScale to map to physical framebuffer pixels.
    float   pixelScreenCoordScale = 1.0f;  /// Scale factor from virtual operating-system defined client area extents (as seen in currentW, currentH) to physical framebuffer pixel coordinates (as seen in windowW, windowH).

    int		buttonInUse   = false;
    bool	buttonPressed = false;

    int 	nFramesSinceWindowResized = 0;
    bool	bWindowNeedsShowing = true;

    //bool    window_entered = false;
    bool    window_active = false;

protected:

private:
    const float max_int_opacity_value = 255.0f;
};