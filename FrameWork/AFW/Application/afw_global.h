#pragma once

// This include of glew must be performed before anything else so as 
// to prevent the error GL.h included before glew error.
// Why on earth have not the authors of glew not taken account of this
// in writting their glew code to avoid this most common of compilatioon
// errors in dealing with glew and glfw that both use gl.h ????????????????? !!!!!!!!!!!!
//#define GLEW_STATIC
//#include <GL/glew.h>

//#include <FrameWork/VW_framework/Types/vw_type_definitions.h>
//
#include <Framework/AFW/Tools/afw_id_key_manager.h>
//
//#include <Universal_FW/ImNodes/Kernal/node.h>
//
//#include "../3D/vw_camera.h" // ++++++++
//#include <Framework/OGLFW/Objects/oglfw_camera_base.h> // ++++++++

// Singleton class to store global variables and functions
// to be used through out the application

#include <FrameWork/AFW/Utils/afw_logger.h>

//#include <imgui.h>// ++++++

class afw_globalc {
public:
	afw_globalc(const afw_globalc&) = delete;

	static afw_globalc& get() {
		static afw_globalc instance;
		return instance;
	}

	static afw_Logger_class *get_current_logger() { return get().current_logger; }
	static void set_current_logger(afw_Logger_class* n) { get().current_logger = n; }

	//+++++++
	
	//static ImDrawList *get_current_ImGui_DrawList() { return get().current_ImGui_DrawList; }
	//static void set_current_ImGui_DrawList(ImDrawList *n) { get().current_ImGui_DrawList = n; }
	
	//+++++++


private:
	afw_globalc() {};

	afw_Logger_class *current_logger = nullptr;

	//ImDrawList *current_ImGui_DrawList = nullptr;


};
