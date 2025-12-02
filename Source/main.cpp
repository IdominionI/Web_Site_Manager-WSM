
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include <Framework/AFW/Application/afw_global.h>
#include <FrameWork/AFW/Application/afw_app_base.h>

#include <FrameWork/OGLFW/Window/glfw_ogl_window.h>
#include <FrameWork/OGLFW/imGui/oglfw_imgui_glfw.h>

//#include "GUI/ws_tree.h"
#include "Application/wsm_application.h"

//#include "File_manager/ws_tree_manager.h"

#include <glm/glm.hpp>

class app_main_class : public afw_app_base_class {
public:
    app_main_class() {}
    ~app_main_class() {}

    glfw_openGL_window_class *create_glfw_window() {
        glfw_openGL_window_class *glfw_window = new glfw_openGL_window_class();
        glfw_window->settings.setGLVersion(4, 6);
        if (!glfw_window->create_window("Web Site Manager", { 200.0f,200.0f }, 1200, 800)) {
            delete glfw_window;
            return nullptr;
        }
        return glfw_window;
    }

    void  define_logger() {
        logger = new afw_Logger_class("logger.txt");
        afw_globalc::set_current_logger(logger);
    }

    void setup() override {
        define_logger();

        glfw_window = create_glfw_window();
        if (!glfw_window) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to initialize/create OpenGL GLFW! : Application Aborted");
            exit(0);
        }

        //initialise imgui
        oglfw_ImGui.ImGui_init(glfw_window->get_window_ptr());

        //log_panel = new log_panel_class();
        //if (log_panel == NULL) {
        //    afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "No Applicaton Logger Defined : Cannot perform application");
        //    return;
        //}


        // This causes render problems and need to be left commented out until debuged
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // Set up the fonts file to use to display text 
        // and the awesome icon font file to display icons since ImGui is not designed
        // to have icon images imported and displayed as part of the GUI !!!!!!!
        ImFontConfig cfg;
        cfg.OversampleH = 3;
        cfg.OversampleV = 1;
        cfg.PixelSnapH = true;
        //cfg.GlyphExtraSpacing.x = 0.0f;// Depreciated
        //cfg.GlyphExtraSpacing.y = 0.0f;// Depreciated
        cfg.GlyphExtraAdvanceX = 0.0f; // New to ImGui to replace GlyphExtraSpacing
        cfg.RasterizerMultiply = 1.25f;

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        ImFont* mainFont = io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 13.0f, &cfg);
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphOffset = ImVec2(0, 1);
        ImFont* iconFont = io.Fonts->AddFontFromFileTTF("Fonts/fontawesome_6_solid.otf", 13.0f + 1.0, &icons_config, icons_ranges);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//std::cout<<"setup 111\n";
        wsm_application.wsm_initialise_project_listing();
//std::cout<<"setup 222\n";
        wsm_application.wsm_initialise_application_listing();
//std::cout<<"setup 222\n";
        wsm_application.glfw_window = glfw_window;
    }



    void run_loop() override {
        //ws_tree.create_ws_tree("ESA","F:Web_downloads/ESA/Unfinished");// Testing only : delete comment out when not in use

        while (!glfwWindowShouldClose(glfw_window->get_window_ptr()))
        {
//printf("here\n");

            // IO
            glfw_window->poll_events();

            if (glfwWindowShouldClose(glfw_window->get_window_ptr())) {
                afw_globalc::get_current_logger()->log(LogLevel::INFO, "EXITED APPLICATION WITHOUT SAVING CHANGES!!!!\n");
                //wsm_application.exit_WSM();// If want to save before exiting application.
            }

            ///Process user input, in this case if the user presses the 'esc' key
            ///to close the application
            gflw_event.glfw_key_event    = glfw_window->glfw_key_event;
            gflw_event.glfw_mouse_event  = glfw_window->glfw_mouse_event;
            gflw_event.glfw_cursor_event = glfw_window->glfw_cursor_event;


            // Update any changes to the window framebuffer to be used in rendering the scene
            // Code here in its raw form rather than as a function in class glfw_ogl_window so as to compare
            // with any opengl or glfw code that may be referenced as an example or modification
            glfwGetFramebufferSize(glfw_window->get_window_ptr(), &glfw_window->window_w, &glfw_window->window_h);
            glViewport(0, 0, glfw_window->window_w, glfw_window->window_h);

            // +++++++

           // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClearColor(background_color.r, background_color.g, background_color.b, background_color.a );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            
            // !!!!!!!!!!!!!!! Window callback actions must be performed before ImGui rendering !!!!!!!!!!!!!

            //if (entered_window(glfw_window)) {
            //    printf("run loop entered_window\n");
            //    glfw_window->window_entered = true;
            //    glfw_window->glfw_cursor_event->reset_cursor_enter_event();
            //} else
            //    glfw_window->window_entered = false;

            //if (glfw_window->is_window_hovered()) {
            //    glm::vec2 window_pos = get_cursor_position(glfw_window);
            //    printf("cursor position callback x : %f :: y : %f\n", window_pos.x, window_pos.y);
            //}


            //if (window_is_resized(active_window)) {
            //    active_window->perform_window_resize_action();
            //}

            //if (window_is_moved(active_window)) {
            //    active_window->perform_window_move_action();
            //}

            //if (window_focused(glfw_window)) {
            //    //printf("run loop window_focused\n");
            //    //active_window = glfw_window;
            //    glfw_window->window_active = true;
            //} else
            //    glfw_window->window_active = false;

            //if (window_maximized(glfw_window)) {
            //    printf("run loop window_maximized %i\n",glfw_window->glfw_window_event->window_maximized);
            //}
//printf("before02\n");
            oglfw_ImGui.ImGui_pre_render();
//printf("After02\n");
            draw_UI();// This must be placed before glfwSwapBuffers
//printf("After03\n");        
            oglfw_ImGui.ImGui_post_render(glfw_window);
//printf("After04\n");
            glfwSwapBuffers(glfw_window->get_window_ptr());// THis must be presentc
        }
    }

    void draw_UI() {
        // Define interface style parameters each render pass so as
        // to be able to change interface style or style components
        //tron_style();

        /*
        enum ImGuiTreeNodeFlags_
        {
            ImGuiTreeNodeFlags_None                 = 0,
            ImGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
            ImGuiTreeNodeFlags_Framed               = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
            ImGuiTreeNodeFlags_AllowOverlap         = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
            ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
            ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
            ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
            ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Open on double-click instead of simple click (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
            ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Open when clicking on the arrow part (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
            ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
            ImGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow. IMPORTANT: node can still be marked open/close if you don't set the _Leaf flag!
            ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding() before the node.
            ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line without using AllowOverlap mode.
            ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (cover the indent area).
            ImGuiTreeNodeFlags_SpanLabelWidth       = 1 << 13,  // Narrow hit box + narrow hovering highlight, will only cover the label text.
            ImGuiTreeNodeFlags_SpanAllColumns       = 1 << 14,  // Frame will span all columns of its container table (label will still fit in current column)
            ImGuiTreeNodeFlags_LabelSpanAllColumns  = 1 << 15,  // Label will span all columns of its container table
            //ImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 16,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
            ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 17,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
            ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,

            #ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                ImGuiTreeNodeFlags_AllowItemOverlap     = ImGuiTreeNodeFlags_AllowOverlap,  // Renamed in 1.89.7
                ImGuiTreeNodeFlags_SpanTextWidth        = ImGuiTreeNodeFlags_SpanLabelWidth,// Renamed in 1.90.7
            #endif
        };
        */

        /*
            Lines 3186 - 3412 imgui_demo.cpp for Multiselect

            // [SECTION] DemoWindowWidgetsTreeNodes() Lines 3990 - 4120 imgui_demo.cpp
        */


        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());// To have the main glfw window viewport as the main docking node

        wsm_application.display_wsm_main_gui_panel();
 
        //ws_tree.display_ws_tree();

        // Following uncommented when investigating ImGui widget examples
        //ImGui::Begin;
        //bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);
        //ImGui::End;
        // ----------------------------------
        ImGui::Render();

    }

    void close() override {
        oglfw_ImGui.ImGui_end();

        if (glfw_window) glfw_window->close();
        glfwTerminate();
    }


    gflw_event_struct_type gflw_event;

    glm::vec4 background_color = { 0.0f,0.0f,0.0f,1.0f };

private:
    glfw_openGL_window_class* glfw_window = nullptr;

    oglFW_ImGui_GLFW_class oglfw_ImGui;
    
    afw_Logger_class* logger = nullptr;

    // UI components
    //log_panel_class* log_panel = nullptr;

    wsm_application_class wsm_application;
};

int main() {
    app_main_class* app = new app_main_class;

    app->run();

    exit(0);

}