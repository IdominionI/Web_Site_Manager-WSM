#pragma once

//#include <FrameWork/AFW/Events/afw_events.h>
//#include <unordered_map>

#include <vector>

//class app_base_class;
//class afw_window_base_class;
//class ofWindowSettings;

#include "afw_app_base.h"

class afw_app_manager_class {
public:
    afw_app_manager_class() {}

    virtual ~afw_app_manager_class() {}

	//virtual afw_window_base_class* create_window(std::string m_title = "window", glm::vec2 m_position = { 200,200 }, int m_width = 600, int m_height = 600)
	//{
	//	return nullptr;
	//}

	template<typename Window>
	void addWindow(Window *window) {
		allowMultiWindow = Window::allowsMultiWindow();
		if (Window::doesLoop()) {
			windowLoop = Window::loop;
		}
		if (Window::needsPolling()) {
			windowPollEvents = Window::pollEvents;
		}
		if (!allowMultiWindow) {
			windowsApps.clear();
		}
		windowsApps[window] = app_base_class;
		currentWindow = window;
		ofAddListener(window->events().keyPressed, this, &ofMainLoop::keyPressed);
	}

	// VWCUSTOM  - ADDED FUNCTION BEGIN
// ++++++++++++ Custom added functions +++++++++++
// !!!!!!!!!!!! This seems to Work !!!!!!!!!!!!!!
// Safely delete a ofAppBaseWindow and its associated OfBaseApp
// that also deletes any derived class created from them.
	template<typename Window>
	bool delete_window(Window *window) {
		auto index = windowsApps.find(window); // Find iterator index of window in running apps

		if (index == windowsApps.end()) return false; // window not found in running apps

		window->close();          // Close window and assiciated running app
		windowsApps.erase(index); // remove references to running app from mapping list 
		// so as no attempt is made to execute a non existing app in the main loop

		return true;
	}
	// VWCUSTOM  - ADDED FUNCTION END
	// ++++++++++++++++++++++++++++++++++++++++++++++

	//void run(afw_window_base_class* window, app_base_class* app) {

	//}
	
	bool add_app(afw_app_base_class* app) {
		if (app && !app_defined(app)) {
			define_app(app);
			return true;
		}
		return false;
	}

	void run(int app_id) {
		afw_app_base_class* app = get_app(app_id);

		if (app) {
			run(app);
		}
	}
	
	void run(afw_app_base_class *app) {
		if (app_defined(app)) {
			app->run();
		}
	}

	void runApp(afw_app_base_class *app) {
		if (!app_defined(app)) {
			define_app(app);
		}

		app->run();
	}

	bool app_defined(afw_app_base_class *app) {
		for (afw_app_base_class* wapp : windowsApps) {
			if (wapp->app_id == app->app_id) {
				return true;
			}
		}
		return false;
	}

	bool app_defined(int app_id) {
		for (afw_app_base_class* wapp : windowsApps) {
			if (wapp->app_id == app_id) {
				return true;
			}
		}
		return false;
	}

	afw_app_base_class* get_app(int app_id) {
		for (afw_app_base_class* wapp : windowsApps) {
			if (wapp->app_id == app_id) {
				return wapp;
			}
		}
		return nullptr;
	}

	bool close_app(int app_id) {
		for (afw_app_base_class* wapp : windowsApps) {
			if (wapp->app_id == app_id) {
				wapp->close();
			}
		}
	}

	bool close_all_apps() {
		for (afw_app_base_class* wapp : windowsApps) {
				wapp->close();
		}
	}

	//int loop();
	//void loopOnce();
	//void pollEvents();
	//void exit();
	//void shouldClose(int status);
	//std::shared_ptr<ofAppBaseWindow> getCurrentWindow();
	//void setCurrentWindow(const std::shared_ptr<ofAppBaseWindow>& window);
	//void setCurrentWindow(ofAppBaseWindow* window);
	//std::shared_ptr<ofBaseApp> getCurrentApp();
	//void setEscapeQuitsLoop(bool quits);

	//ofEvent<void> exitEvent;
	//ofEvent<void> loopEvent;

	//std::thread::id get_thread_id() { return thread_id; };
protected:

private:
	//std::thread::id thread_id{ std::this_thread::get_id() };

	//void keyPressed(ofKeyEventArgs& key);
	//std::vector<std::pair<afw_window_base_class*, app_base_class*>> windowsApps;
	std::vector<afw_app_base_class*> windowsApps;

	//std::unordered_map<afw_window_base_class*, app_base_class*> windowsApps;
	//bool bShouldClose;
	//afw_window_base_class *currentWindow;
	//int status;
	//bool allowMultiWindow;
	//std::function<void()> windowLoop;
	//std::function<void()> windowPollEvents;
	//bool escapeQuits;

	void define_app(afw_app_base_class *app) {
		// Assign an unique id for app here
		windowsApps.push_back(app);
	}
};