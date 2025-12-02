#pragma once
#include <string>

#include <FrameWork/AFW/Common/afw_definitions.h>


class afw_app_base_class {
public:
	afw_app_base_class() {}
	virtual ~afw_app_base_class() {}

	
	// -----------------------------------

	int         app_id   = INVALID_ID;
	std::string app_name = "app";


	virtual void setup() = 0;
	virtual void run() {
		setup();
		run_loop();
		close();
	};

	virtual void run_loop() = 0;

	virtual void update(){}
	//virtual void draw(){}
	virtual void close() = 0;

	virtual void windowResized(int w, int h) {}

	virtual void keyPressed(int key) {}
	virtual void keyReleased(int key) {}

	/// \brief Called on the active window when the mouse is moved
	virtual void mouseMoved(int x, int y) {}

	/// \brief Called on the active window when the mouse is dragged, i.e.
	/// moved with a button pressed
	virtual void mouseDragged(int x, int y, int button) {}

	/// \brief Called on the active window when a mouse button is pressed
	virtual void mousePressed(int x, int y, int button){}

	/// \brief Called on the active window when a mouse button is released
	virtual void mouseReleased(int x, int y, int button) {}

	/// \brief Called on the active window when the mouse wheel is scrolled
	virtual void mouseScrolled(int x, int y, float scrollX, float scrollY){}

	/// \brief Called on the active window when the mouse cursor enters the
	/// window area
	///
	/// Note that the mouse coordinates are the last known x/y before the
	/// event occurred, i.e. from the previous frame
	virtual void mouseEntered(int x, int y){}

	/// \brief Called on the active window when the mouse cursor leaves the
	/// window area
	///
	/// Note that the mouse coordinates are the last known x/y before the
	/// event occurred, i.e. from the previous frame
	virtual void mouseExited(int x, int y){}

	//virtual void dragEvent(ofDragInfo dragInfo){}
	//virtual void gotMessage(ofMessage msg){}

	//int mouseX = 0, mouseY =0;			// for processing heads

	//virtual void setup(ofEventArgs& args){}
	//virtual void update(ofEventArgs& args){}
	////virtual void draw(ofEventArgs& args){}
	//virtual void exit(ofEventArgs& args){}

	//virtual void windowResized(ofResizeEventArgs& resize){}

	//virtual void keyPressed(ofKeyEventArgs& key){}
	//virtual void keyReleased(ofKeyEventArgs& key){}

	//virtual void mouseMoved(ofMouseEventArgs& mouse){}
	//virtual void mouseDragged(ofMouseEventArgs& mouse){}
	//virtual void mousePressed(ofMouseEventArgs& mouse){}
	//virtual void mouseReleased(ofMouseEventArgs& mouse){}
	//virtual void mouseScrolled(ofMouseEventArgs& mouse){}
	//virtual void mouseEntered(ofMouseEventArgs& mouse){}
	//virtual void mouseExited(ofMouseEventArgs& mouse){}
	//virtual void dragged(ofDragInfo& drag){}
	//virtual void messageReceived(ofMessage& message){}


protected:

private:

};

/*
	// -----------------------------------
	// Callbacks
	std::function<void(int, int, int, int)> key_callback;
	std::function<void(int, int)>           window_size_callback;
	std::function<void(double, double)>     mouse_call_back;

	std::function<void(int button, int state, int mods)>mouse_cb;
	std::function<void(double x, double y)>motion_cb;
	std::function<void(int entered)>entry_cb;
	std::function<void(int key, int scancode, int action, int mods)>keyboard_cb;
	std::function<void(uint32_t key)>char_cb;
	std::function<void(int w, int h)>resize_cb;
	std::function<void(int w, int h)>framebuffer_size_cb;
	std::function<void(GLFWwindow* windowP_)>exit_cb;
	std::function<void(double x, double y)>scroll_cb;
	std::function<void(int numFiles, const char** dropString)>drop_cb;
	std::function<void(int errorCode, const char* errorDescription)>error_cb;

	void initialise_callbacks() {
		set_window_size_callback(std::bind(&Application::window_resize_callback, this, std::placeholders::_1, std::placeholders::_2));

		set_mouse_callback(std::bind(&Application::mouse_callback, this, std::placeholders::_1, std::placeholders::_2));

		set_key_callback(std::bind(&Application::keyboard_callback,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4));
	}

	inline void set_key_callback(std::function<void(int, int, int, int)> callback)
	{
		key_callback = callback;
	}

	inline void set_window_size_callback(std::function<void(int, int)> callback)
	{
		window_size_callback = callback;
	}

	inline void set_mouse_callback(std::function<void(double, double)> callback)
	{
		mouse_call_back = callback;
	}
*/