#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <FrameWork/AFW/Common/afw_definitions.h>

/// \brief Used to represent the available windowing modes for the application.
//enum class window_mode_enum { windowed, fullscreen, minimised };

enum class window_graphic_api_enum { undefined, opengl, vulkan};// add others !!!
enum class window_api_enum { undefined, glfw}; // add others !!!

#define WINDOW_CATEGORY_UNDEFINED 0
#define WINDOW_CATEGORY_GUI       10
#define WINDOW_CATEGORY_GRAPHICS  11
// add more categories here

// Struct to store properties of a winddow for easy reference

class application_base_class;
class ofCoreEvents;

struct window_properties_struct {
	window_graphic_api_enum window_graphic_api = window_graphic_api_enum::undefined;
	window_api_enum window_api = window_api_enum::undefined;
	int window_category = WINDOW_CATEGORY_UNDEFINED;

	
	std::string title     = "window";
	glm::vec2   position  = { 100,100 };
	int			width     = 1000;
	int			height    = 600;
	float		opacity   = 1.0;
	bool		wpresizable = true;
	bool		visible   = true;
	
	
	glm::vec3 background_color = { 0.0f, 0.0f, 0.0f };

//	window_mode_enum mode = window_mode_enum::windowed;
	
	// Set functions
	void set_position( glm::ivec2 m_position) { this->position = m_position; }
	void set_width(int m_width)  { this->width = m_width; }
	void set_height(int m_height) { this->height = m_height; }
	void set_size(int m_width,int m_height) {
		set_width(m_width);
		set_height(m_height);
	}
//	void set_mode(window_mode_enum m_mode) { this->mode = m_mode; }

	void set_title(std::string m_title) { title = m_title; }
	void set_position(int x,int y) { set_position(glm::vec2{ x, y }); }
	void set_size(glm::ivec2 m_size) { set_size(m_size.x, m_size.y); }
	void set_opacity(float m_opacity) { opacity = m_opacity; }
	void set_window_graphic_api(window_graphic_api_enum m_window_graphic_api) { window_graphic_api = m_window_graphic_api; }
	void set_window_api(window_api_enum m_window_api) { window_api = m_window_api; }
	void set_window_category(int category) { window_category = category; }
	void set_background_color(glm::vec3 bg_color) { background_color = bg_color; }

	void define_window_proprties(std::string title, glm::vec2 position, int width, int height) {
		set_title(title);
		set_position(position);
		set_width(width);
		set_height(height);
	}

	// get functions
	std::string get_title()	   { return title; }
	glm::ivec2  get_position() { return position; }
	int         get_width()    { return width; }
	int         get_height()   { return height; }
	float       get_opacity()  { return opacity; }
	glm::ivec2  get_size()     { return { width, height }; }
	glm::vec3   get_background_color() { return background_color; }
//	window_mode_enum get_mode() { return mode; }

	window_graphic_api_enum get_window_graphic_api(){ return window_graphic_api; }
	window_api_enum			get_window_api()		{ return window_api; }
	int						get_window_category()	{ return window_category; }

};

// window base class that defies common functions that any type of window must have defined 



template <class T>
class afw_window_base_class : public window_properties_struct {
public:
	afw_window_base_class() {}
	~afw_window_base_class() {}
	// get window ptr function of form in derived class by casting to correct data type
	//  <data type> *get_window_ptr() { return static_cast<data_type*> (window_ptr) ; }
	//int * get_window_ptr() { return static_cast<int*> (window_ptr); }

	// -------------------------------------------------------------
	int  id     = INVALID_ID;
	bool active = false;

	virtual T* create_window() { return nullptr; };
	virtual T* create_window(window_properties_struct window_properties) { return nullptr; };
	virtual T* create_window(std::string m_title = "window", glm::vec2 m_position = { 200,200 }, int m_width = 600, int m_height = 600) { return nullptr; };

	T   *get_window_ptr() { return window_ptr; }
	bool valid_window() { if (window_ptr != nullptr) return true; else return false; }

	virtual void poll_events() = 0;

	int						get_window_id() { return id; }
	window_graphic_api_enum get_window_graphic_api() { return window_graphic_api; }
	window_api_enum         get_window_api() {return window_api;}
	int						get_window_category() { return window_category; }

	// Set window attribute/property functions that must exist for all windows
	virtual void set_window_position(glm::ivec2 m_position)		 = 0;
	virtual void set_window_width(int m_width)					 = 0;
	virtual void set_window_height(int m_height)				 = 0;
	virtual void set_window_size(int m_width, int m_height)		 = 0;
	virtual void set_window_title(std::string m_title)			 = 0;
	virtual void set_window_position(int x, int y)				 = 0;
	virtual void set_window_size(glm::ivec2 m_size)				 = 0;
	virtual void set_window_opacity(float m_opacity)			 = 0;
	virtual void set_window_background_color(glm::vec3 bg_color) = 0;
	//virtual void set_window_mode(window_mode_enum m_mode) { set_mode(m_mode); }

	void define_window_settings(std::string title, glm::vec2 position, int width, int height) {
		set_window_title(title);
		set_window_position(position);
		set_window_width(width);
		set_window_height(height);
	}

	// get window attribute/property functions that must exist for all windows
	virtual glm::vec2	get_window_position()		  = 0;
	virtual int			get_window_width()			  = 0;
	virtual int			get_window_height()			  = 0;
	virtual glm::vec2	get_window_size()			  = 0;
	virtual std::string get_window_title()			  = 0;
	virtual float		get_window_opacity()		  = 0;
	virtual glm::vec3	get_window_background_color() { return get_background_color(); };
	//virtual window_mode_enum get_window_mode() { return get_mode(); }

	// Window optional action functions that exist for all windows
	virtual void hide_window(){}
	virtual void show_window(){}
	virtual void focus_window(){}
	virtual void maximise_window() {}
	virtual void minimise_window() {}

	virtual void resize_window(int width, int height){}
	virtual void resize_window(glm::ivec2){}
	virtual void move_window(int x, int y){}
	virtual void move_window(glm::ivec2){}

	// Window query functions that must exist for all windows
	virtual bool is_window_resizable()   = 0;
	virtual bool is_window_full_screen() = 0;
	virtual bool is_window_visible()     = 0;
	virtual bool is_window_hovered()     = 0;

	// window callback functions
	virtual void on_window_resized() {}
	virtual void on_window_focused() {}
	virtual void on_window_close()   {}
	virtual void on_window_entered() {}

protected:
	T *window_ptr = nullptr;

	application_base_class *application=nullptr;
	//ofCoreEvents		   *coreEvents;

	//virtual bool create_window()  = 0;
	//virtual void destroy_window() = 0;

private:
	
};