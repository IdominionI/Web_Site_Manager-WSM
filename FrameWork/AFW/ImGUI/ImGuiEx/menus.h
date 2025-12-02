#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>

#include <imgui.h>
#include <imgui_internal.h>

//#include <Universal_FW/Kernal/id_key_manager.h>
#include <Framework/AFW/Tools/afw_id_key_manager.h>

enum class menu_item_type_enum { label,item, option, seperator, parent };
enum class menu_type_enum { main, popup, menu  };

class imgui_menu_basis_class {
public:
	menu_type_enum menu_type = menu_type_enum::menu;
	std::string    menu_name = "menu";
	int            menu_id = -1;

	//imgui_menu_basis_class(std::string label = "menu") {
	//imgui_menu_basis_class(const char* label) {
		//menu_id   = _menu_id;
	//	menu_name = label;
	//}

	virtual void imgui_menu(std::string label = "menu", bool enabled = true) {
		if (ImGui::BeginMenu(label.c_str(), enabled)) {
			{
				if (ImGui::MenuItem("menu_item00")) { }

				ImGui::EndMenu();
			}
		}
	}

	virtual void imgui_context_popup_menu(std::string label = "context_popup") {
		if (ImGui::BeginPopupContextWindow(label.c_str(), ImGuiPopupFlags_MouseButtonRight)) {

			imgui_menu();

			ImGui::EndPopup();
		}
	}

	virtual void imgui_context_main_menu(std::string label = "context_popup") {
		if (ImGui::BeginMainMenuBar()) {

			imgui_menu();

			ImGui::EndMainMenuBar();
		}
	}

	// ****************** 
	// Menu functions to go here or are included from a seperate 
	// class or header file with functions defined
	// *********************
	

private:

};


class imgui_menu_manager_class {
public:
	~imgui_menu_manager_class(){
		menus.clear();
	}
	
	std::list <imgui_menu_basis_class > menus;


	//imgui_menu_basis_class *create_new_menu(std::string menu_name, menu_type_enum menu_type) {
	//	unsigned int menu_id = menu_id_key.get_available_id_key();

	//	imgui_menu_basis_class new_imgui_menu(menu_id,menu_name);
	//	new_imgui_menu.menu_type = menu_type;

	//	menus.push_back(new_imgui_menu);

	//	return &new_imgui_menu;
	//}

	void add_menu(imgui_menu_basis_class menu) {
		unsigned int menu_id = menu_id_key.get_available_id_key();
		menu.menu_id = menu_id;
		menus.push_back(menu);
	}

	imgui_menu_basis_class *get_menu(std::string menu_name) {
		for (imgui_menu_basis_class menu : menus) {
			if (menu.menu_name == menu_name) return &menu;
		}

		return NULL;
	}

	imgui_menu_basis_class *get_menu(unsigned int menu_id) {
		for (imgui_menu_basis_class menu : menus) {
			if (menu.menu_id == menu_id) return &menu;
		}

		return NULL;
	}


private:
	id_key_manager_class<unsigned int> menu_id_key;

};