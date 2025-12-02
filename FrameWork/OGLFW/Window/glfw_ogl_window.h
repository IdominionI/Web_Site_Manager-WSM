#pragma once
// This include of glew must be performed before anything else so as 
// to prevent the error GL.h included before glew error.
// Why on earth have not the authors of glew not taken account of this
// in writting their glew code to avoid this most common of compilatioon
// errors in dealing with glew and glfw that both use gl.h ????????????????? !!!!!!!!!!!!

// gl.h already included in glfw.h which
// is present in <Framework/AFW/Window/glfw_window_base.h>

#define GLEW_STATIC // Must have this present or will have message for a glew.dll file
#include <GL/glew.h>

#include <Framework/AFW/Application/afw_global.h> // Must be placed here above all other includes

#include <Framework/AFW/Window/glfw_window_base.h>
#include <FrameWork/OGLFW/Window/oglfw_window_settings.h>

struct ogl_glfw_window_settings_struct : public ogl_window_settings_struct, glfw_window_settings_struct {


};

class glfw_openGL_window_class : public glfw_window_base_class {
public:

    GLFWwindow* create_window(std::string m_title = "window", glm::vec2 m_position = { 200,200 }, int m_width = 600, int m_height = 600) override {
   // GLFWwindow* create_window(std::string m_title = "window", int x_pos =200, int ypos =200 , int m_width = 600, int m_height = 600) override {
//printf("create_glfw_window 00\n");
//afw_globalc::get_current_logger()->log(LogLevel::INFO, "create_glfw_window 00");

        if (!initialise_settings(settings, m_title, m_position, m_width, m_height)) return nullptr;

        if (!glfwInit()) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "glfw_openGL_window_class :: Unable to initialize OpenGL GLFW! ");
            return nullptr;
        }

        // glfw glfwWindowHint settings must be defined after glfwInit
        define_glfw_settings();

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

        window_ptr = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!window_ptr) {
            glfwTerminate();
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "glfw_openGL_window_class :: Unable to create GLFW openGL window!");
            return nullptr;
        }

        glfwMakeContextCurrent(window_ptr); // Set the window to be used and then centre that window on the monitor. 
        set_window_graphic_api(window_graphic_api_enum::opengl);
        set_window_api(window_api_enum::glfw);

        set_window_position(m_position);
        current_w = m_width;
        current_h = m_height;
        glfwGetWindowSize(window_ptr, &current_w, &current_h);
        set_size(m_width, m_height);

        window_w = current_w;
        window_h = current_h;

        //set_window_opacity(.5f);
        //set_window_background_color({.2f, 0.0f, 0.0f});
       

        //Initialize GLEW
        if (glewInit() == GLEW_OK)
        {
            // std::cout << "GLEW initialized successfully" << std::endl;
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "glfw_openGL_window_class :: GLEW initialized successfully");
        } else {
            //std::cout << "Unable to initialize GLEW" << std::endl;
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "glfw_openGL_window_class :: Unable to initialize GLEW. Exiting application");
            glfwDestroyWindow(window_ptr);
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }


        ////////////////////////////////////////////
        //if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        //    glfwDestroyWindow(window_ptr);
        //    glfwTerminate();
        //    std::exit(EXIT_FAILURE);
        //}

        //std::clog << "OpenGL Vendor: " << reinterpret_cast<const GLchar*>(glGetString(GL_VENDOR)) << std::endl;
        //std::clog << "OpenGL Renderer: " << reinterpret_cast<const GLchar*>(glGetString(GL_RENDERER)) << std::endl;
        //std::clog << "OpenGL Version: " << reinterpret_cast<const GLchar*>(glGetString(GL_VERSION)) << std::endl;
        //std::clog << "OpenGL Shading Language Version: " << reinterpret_cast<const GLchar*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) << std::endl;
        ////////////////////////////////////////////

        std::string s =reinterpret_cast<const GLchar*>(glGetString(GL_VENDOR));
        afw_globalc::get_current_logger()->log(LogLevel::INFO, "OpenGL Vendor: " + s );
        s = reinterpret_cast<const GLchar*>(glGetString(GL_RENDERER));
        afw_globalc::get_current_logger()->log(LogLevel::INFO, "OpenGL Renderer: " + s);
        s = reinterpret_cast<const GLchar*>(glGetString(GL_VERSION));
        afw_globalc::get_current_logger()->log(LogLevel::INFO, "OpenGL Version: " + s);
        s = reinterpret_cast<const GLchar*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        afw_globalc::get_current_logger()->log(LogLevel::INFO, "OpenGL Shading Language Version: " + s);


        if(settings.initialise_callbacks) initialise_callbacks();
        //setVerticalSync(true);
        glfwSwapInterval(1);
        afw_globalc::get_current_logger()->log(LogLevel::INFO, "Application GLFW OpenGL window : " + m_title + " : created.");
        return window_ptr;
    }


    void set_window_background_color  (glm::vec3 bg_color) { // mandatory virtual function that can only be defined here in openGL
        set_background_color(bg_color);
        float opacity = get_window_opacity();
        glClearColor(bg_color.r * opacity, bg_color.g * opacity, bg_color.b * opacity, opacity);
    }

    virtual bool initialise_settings(glfw_window_settings_struct glfw_window_settings,std::string title,glm::vec2 position,int width,int height) {
         if (window_ptr) {
             //ofLogError() << "window already setup, probably you are mixing old and new style setup";
             //ofLogError() << "call only ofCreateWindow(settings) or ofSetupOpenGL(...)";
             //ofLogError() << "calling window->setup() after ofCreateWindow() is not necessary and won't do anything";

             afw_globalc::get_current_logger()->log(LogLevel::INFO, "window already setup, probably you are mixing old and new style setup");
             afw_globalc::get_current_logger()->log(LogLevel::INFO, "call only ofCreateWindow(settings) or ofSetupOpenGL(...)");
             afw_globalc::get_current_logger()->log(LogLevel::INFO, "calling window->setup() after ofCreateWindow() is not necessary and won't do anything");

             //std::cout << "window already setup, probably you are mixing old and new style setup" << std::endl;
             //std::cout << "call only ofCreateWindow(settings) or ofSetupOpenGL(...)" << std::endl;
             //std::cout << "calling window->setup() after ofCreateWindow() is not necessary and won't do anything" << std::endl;
             return false;
         }

         define_window_proprties(title, position, width, height);
         //define_window_settings(settings);

         application = nullptr;
         window_ptr  = nullptr;

         return true;
     }

     void define_window_settings(ogl_glfw_window_settings_struct glfw_window_settings) {
         settings = glfw_window_settings;
     }

    void define_glfw_settings() override {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RED_BITS, settings.redBits);
        glfwWindowHint(GLFW_GREEN_BITS, settings.greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, settings.blueBits);
        glfwWindowHint(GLFW_ALPHA_BITS, settings.alphaBits);
        glfwWindowHint(GLFW_DEPTH_BITS, settings.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, settings.stencilBits);
        glfwWindowHint(GLFW_STEREO, settings.stereo);
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

        glfwWindowHint(GLFW_AUX_BUFFERS, settings.doubleBuffering ? 1 : 0);

        glfwWindowHint(GLFW_SAMPLES, settings.numSamples);
        glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
        glfwWindowHint(GLFW_DECORATED, settings.decorated);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glVersionMinor);

        if (settings.glVersionMajor >= 3) {
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#if (GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR > 2) || (GLFW_VERSION_MAJOR > 3 )
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparent);
#endif
        }
    }

    void inititialise_window_frame_buffer() override {
        int window_w;
        int window_h;
        glfwGetFramebufferSize(get_window_ptr(), &window_w, &window_h);
        glfwMakeContextCurrent(get_window_ptr());
        glViewport(0, 0, window_w, window_h);

        prev_width  = window_w;
        prev_height = window_h;
    }

    //void perform_window_enterd_action() override {

    //}

    void perform_window_move_action() override {
        //glm::ivec2 cwinpos = {};
        //cwinpos = get_window_position();
        set_position(get_window_position());
        glfw_window_event->end_window_moved();// perform end of window move event action 
    }

    void perform_window_resize_action() override{
                glm::ivec2 window_size = window_resize(); // get window size from resize window event
//printf("window resize callback 000 x : %i :: y : %i\n", window_w, window_h);
//printf("window resize callback 001 x : %i :: y : %i\n", window_size.x, window_size.y);
                glfw_window_event->end_window_resized(); // perform end of window resize event action 

                int resized_width  = window_size.x;
                int resized_height = window_size.y;

//printf("window resize callback 003 prev_width : %i : prev_height : %i \n", prev_width, prev_height);

                float x_ratio = (float)resized_width / (float)prev_width;
                float y_ratio = (float)resized_height / (float)prev_height;

                int new_width  = prev_width;
                int new_height = prev_height;

                if (window_size.x != prev_width && window_size.y == prev_height) {
                    new_width  = window_size.x;
                    new_height = (int)((float)prev_height * x_ratio);
                } else {

                    if (window_size.x == prev_width && window_size.y != prev_height) {
                        new_width  = (int)((float)prev_width * y_ratio);
                        new_height = window_size.y;
                    } else {

                        if (window_size.x != prev_width && window_size.y != prev_height) {
                            // find larger of resized window changes
                            if (x_ratio > y_ratio) {
                                new_width  = window_size.x;
                                new_height = (int)((float)prev_height * x_ratio);
                            } else {
                                new_width  = (int)((float)prev_width * y_ratio);
                                new_height = window_size.y;
                            }
                        }
                    }
                }

//printf("window resize callback 004 x : %f :: y : %f\n", x_ratio, y_ratio);
//printf("window resize callback 005 x : %i :: y : %i : new_width : %i : new_ y %i\n",prev_width,prev_height, new_width, new_height);

                prev_width  = new_width;
                prev_height = new_height;
                
                resize_window(new_width,new_height);
    }



    ogl_glfw_window_settings_struct settings;
protected:
    void setVerticalSync(bool bVerticalSync) {
        if (bVerticalSync) {
            glfwSwapInterval(1);
        }
        else {
            glfwSwapInterval(0);
        }
    }

    int prev_width  = 0;
    int prev_height = 0;
private:

};