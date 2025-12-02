#pragma once

#include <FrameWork/AFW/Window/afw_window_base.h>

struct ogl_window_settings_struct : public window_properties_struct {

	void setGLVersion(int major, int minor) {
		glVersionMajor = major;
		glVersionMinor = minor;
	}

	int glVersionMajor;
	int glVersionMinor;

};



