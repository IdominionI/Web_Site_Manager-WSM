#pragma once

#include <string>
#include <vector>
#include <utility>
#include <filesystem>

#include <Framework/AFW/Tools/afw_string_tools.h>
#include <Framework/AFW/Tools/afw_dialogs.h>
#include <Framework/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include "Source/File_manager/ws_tree_manager.h"

typedef  std::pair<std::string, std::filesystem::path>  wsm_project_data_type;

#define WSM_PROJECT_LIST_FILE_PATHNAME     "wsm_projects_list.txt"
#define WSM_APPLICATION_LIST_FILE_PATHNAME "wsm_application_list.txt"

struct wsm_projects_data_struct_type {
	std::vector<wsm_project_data_type>  wsm_projects;
	int current_selected_project = -1;

	bool add_wsm_project(std::string project_label, std::filesystem::path project_path) {
		wsm_project_data_type project_to_add;
		project_to_add.first  = project_label;
		project_to_add.second = project_path;

		for (wsm_project_data_type wsm_project : wsm_projects) {
			if (project_to_add.second == wsm_project.second) {
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "add_wsm_project : Unable to add web site project with pathname : " + project_path.string() + "\n Project already defined. \n");
				return false;
				break;
			}
		}

		wsm_projects.push_back(project_to_add);

		return true;
	}

	bool add_wsm_project(std::filesystem::path project_path) {
		return add_wsm_project(project_path.filename().string(), project_path);
	}

	void remove_wsm_project(int wsm_project_index_id) {
		if(wsm_project_index_id > -1 && wsm_project_index_id < wsm_projects.size())
			wsm_projects.erase(wsm_projects.begin() + wsm_project_index_id);

		wsm_projects.shrink_to_fit();
	}

	bool save_wsm_projects(int current_selected_project) {
		std::filesystem::path project_listings_path = WSM_PROJECT_LIST_FILE_PATHNAME;

		std::ofstream projects_listing_file;

		projects_listing_file.open(project_listings_path, std::ios::trunc);// Overwrite existing projects file
		//projects_listing_file.open(WSM_PROJECT_LIST_FILE_PATHNAME, std::ios::trunc);// Overwrite existing projects file // This does not work

		if (!projects_listing_file) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to save projects listing  : Could not open/create list of projects file for writing to. \n");
			return false;
		}
		else
			if (!projects_listing_file.is_open()) return false;

std::cout<<"save_wsm_projects 111:"<< current_selected_project << "\n";
		projects_listing_file << current_selected_project << "\n";
//std::cout<<"save_wsm_projects 222:"<< wsm_projects.size() << "\n";
		for (wsm_project_data_type wsm_project : wsm_projects) {
//std::cout<<"save_wsm_projects 333:"<< wsm_project.first << "|" << wsm_project.second.string().c_str() << "\n";

			// Replace forward slashes with backslashes in the path name as they cause problems 
			std::string proj_path_name = wsm_project.second.string().c_str();
			FW::stringtools::replaceInPlace(proj_path_name, "\\", "/");

			//projects_listing_file << wsm_project.first << "|" << wsm_project.second.string().c_str() << "\n";
			projects_listing_file << wsm_project.first << "|" << proj_path_name << "\n";
		}

		// Close the file to free up resources.
		projects_listing_file.close();

		return true;
	}
};


class wsm_application_class {
public:
	wsm_application_class() {}
	~wsm_application_class() {}


	glfw_openGL_window_class *glfw_window = nullptr;

	bool wsm_initialise_project_listing() {
		std::filesystem::path project_listings_path = WSM_PROJECT_LIST_FILE_PATHNAME;
//std::cout<<"wsm_initialise_project_listing 111\n";
		if (!std::filesystem::exists(project_listings_path)) {
			return create_wsm_projects_listing_file();
		}
//std::cout<<"wsm_initialise_project_listing 222\n";
		return read_wsm_projects_listing_file(project_listings_path);
	}

	bool wsm_initialise_application_listing() {
		std::filesystem::path application_listings_path = WSM_APPLICATION_LIST_FILE_PATHNAME;

		if (!std::filesystem::exists(application_listings_path)) {
			return create_wsm_projects_listing_file();
		}

		return read_wsm_application_listing_file(application_listings_path);
	}

	void display_wsm_main_gui_panel() {
//std::cout<<"display_wsm_main_gui_panel 000:"<< selected_wsm_project << "\n";		
		// Display main window menu bar
		if (ImGui::BeginMainMenuBar()) {// UnComment out to have menu in application main window

			if (ImGui::BeginMenu("Project")) {
//printf("display_tree_branch :BeginMenu: %s : %i \n", child_tree_branch->label.c_str(), i);

				if (ImGui::MenuItem("Add project")) {
//printf("Add to selection :MenuItem: \n");
					add_project();
				}

				if (ImGui::MenuItem("Remove project")) {
//printf("Remove from selection :MenuItem:\n");
					open_remove_popup = true;
				}

				if (ImGui::MenuItem("Edit project label")) {
//printf("Edit selection :MenuItem:\n");
					open_edit_project_label_popup = true;
				}

				if (ImGui::MenuItem("Save and Exit")) {
//printf("Exit WSM :MenuItem:\n");
					exit_WSM();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Preferences")) {
				if (ImGui::BeginMenu("Define WSM Media Apps ...")) {
					if (ImGui::MenuItem("html editor")) {
//printf("Add to selection :MenuItem: \n");
						define_html_editor();
					}

					if (ImGui::MenuItem("image editor")) {
//printf("Add to selection :MenuItem: \n");
						define_image_editor();
					}

					if (ImGui::MenuItem("text editor")) {
//printf("Add to selection :MenuItem: \n");
						define_text_editor();
					}

					if (ImGui::MenuItem("media player")) {
//printf("Add to selection :MenuItem: \n");
						define_media_player();
					}

					// Add other web site media applications to view/edit WSM files here

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();// UnComment out to have menu in application main window
		}// UnComment out to have menu in application main window

		if (open_remove_popup) { // Cannot display Model popups within a menu call
			remove_project();
		}

		if (open_edit_project_label_popup) { // Cannot display Model popups within a menu call
			edit_project_label();
		}

//std::cout<<"display_wsm_main_gui_panel 111:"<< selected_wsm_project << "\n";

		if (selected_wsm_project < 0 || selected_wsm_project >= wsm_projects_data.wsm_projects.size()) return;

		// ############# Display Web Site Manager window and widgets ##################
		ImGui::Begin("Web Site Manager", nullptr, ImGuiWindowFlags_MenuBar);// Critical to have ImGuiWindowFlags_MenuBar to enable menu bar to be displayed
			static int item_selected_idx; // Here we store selection data as an index.


//std::cout<<"display_wsm_main_gui_panel 222:"<< selected_wsm_project << "\n";
			//if (selected_wsm_project < 0 && wsm_projects_data.wsm_projects.size() > 0)
			//	item_selected_idx = 0;
			//else
				item_selected_idx = selected_wsm_project;


			if (ImGui::BeginMenuBar()) {// UnComment out to have menu in application main window;
				if (ImGui::BeginMenu("Actions")) {
					if (ImGui::MenuItem("Clear all selected items")) {
//printf("Clear all selected items :MenuItem:\n");
						clear_all_selected_items();

					}

					if (ImGui::MenuItem("Delete all selected items")) {
//printf("Delete all selected items :MenuItem:\n");
						delete_all_selected_items();

					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();// UnComment out to have menu in application main window
			}// UnComment out to have menu in application main window

			
			// --------- Display Web Site Manager project selection combo widget ---------------

			combo_flags = ImGuiComboFlags_WidthFitPreview;// Apparently need this to be reset every display cycle or will not work
			
			// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
			const char* combo_preview_value = wsm_projects_data.wsm_projects[item_selected_idx].first.c_str();
			if (ImGui::BeginCombo("Select Web Site Project", combo_preview_value, combo_flags))
			{
				for (int n = 0; n < wsm_projects_data.wsm_projects.size(); n++)
				{
					const bool is_selected = (item_selected_idx == n);

//std::cout << "display_wsm_main_gui_panel 02 "<< wsm_projects_data.wsm_projects[n].first << " :: " << "\n";
					if (ImGui::Selectable(wsm_projects_data.wsm_projects[n].first.c_str(), is_selected)) {
						item_selected_idx = n;
						item_changed      = true;
						projects_changed  = true;
						prev_selected_wsm_project = selected_wsm_project; // ++++++
						selected_wsm_project = n;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// --------- Display Web site tree of directories and files --------------

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::SeparatorText("Selected Web Site Directory/File Tree Structure");
			ImGui::Separator();
			ImGui::NewLine();

			if (item_selected_idx > -1 ) {
				if (item_changed || first_run) {
					if (std::filesystem::exists(wsm_projects_data.wsm_projects[item_selected_idx].second)) {
						if (!first_run)
							ws_tree_manager.delete_ws_tree(false);
						else
							first_run = false;

						ws_tree_manager.create_ws_tree(wsm_projects_data.wsm_projects[item_selected_idx].first, wsm_projects_data.wsm_projects[item_selected_idx].second);
					}
					else {
						afw_globalc::get_current_logger()->log(LogLevel::INFO, "Unable to initialise web site : " + wsm_projects_data.wsm_projects[item_selected_idx].second.string() + "\nLocation Does not exist.\n");
						projects_changed = false;// ++++++
						selected_wsm_project = prev_selected_wsm_project;// ++++++
					}

					item_changed = false;
				}
			
				ws_tree_manager.display_ws_tree();

				change_tree_label(wsm_projects_data.wsm_projects[item_selected_idx]);
			}


		ImGui::End();// Comment out to have menu in application main window
	}

	void exit_WSM() {
		printf("Exit WSM \n");
		// save defined project definitions
		if (projects_changed)	  wsm_projects_data.save_wsm_projects(selected_wsm_project);
		if (applications_changed) save_wsm_applications();

		if (glfw_window)	glfwSetWindowShouldClose(glfw_window->get_window_ptr(), true);
	}

protected:
	bool add_project() {
		char const* directory_pathname_c = vwDialogs::select_directory("F:");

		std::string directory_pathname_s = directory_pathname_c;

		if (!directory_pathname_s.empty()) {
			std::filesystem::path directory_pathname = directory_pathname_s;

			if (!std::filesystem::exists(directory_pathname)) {
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "Unable to initialise web site at : " + directory_pathname_s + "\nLocation Does not exist.\n");
				return false;
			}
			else {
//printf("expression_file_pathname != NULL %s \n", directory_pathname);

				wsm_projects_data.add_wsm_project(directory_pathname);
				projects_changed = true;
				if(wsm_projects_data.wsm_projects.size()==1) selected_wsm_project = 0;
			}

		}

		return true;
	}

	void remove_project() {

//printf("remove_project 00000 \n");

		if (wsm_projects_data.wsm_projects.size() < 1) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "No projects defined to remove. \n");
			return;
		}

		ImGui::OpenPopup("Select Web Site Project to Remove");

		if (ImGui::BeginPopupModal("Select Web Site Project to Remove", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
//printf("remove_project 11111 \n");
			if (wsm_projects_data.wsm_projects.size() < 1) {
				ImGui::Text("No projects to remove !");
				ImGui::NewLine();
				if (ImGui::Button("EXIT", ImVec2(120, 0))) {
					open_remove_popup = false;
					ImGui::CloseCurrentPopup();
				}
			}

			static int item_selected_to_remove_idx = 0; // Here we store our selection data as an index.

			// --------- Display Web Site Manager project selection combo widget ---------------

			combo_flags = ImGuiComboFlags_WidthFitPreview;// Apparently need this to be reset every display cycle or will not work

			// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
			const char* combo_preview_value = wsm_projects_data.wsm_projects[item_selected_to_remove_idx].first.c_str();
			if (ImGui::BeginCombo("Select Web Site Project", combo_preview_value, combo_flags))
			{
				for (int n = 0; n < wsm_projects_data.wsm_projects.size(); n++)
				{
					const bool is_selected = (item_selected_to_remove_idx == n);
					//bool is_selected = n;

//std::cout << "remove_project 02 "<< wsm_projects_data.wsm_projects[n].first << " :: " << is_selected << "\n";
					if (ImGui::Selectable(wsm_projects_data.wsm_projects[n].first.c_str(), is_selected)) {
//std::cout << "remove_project 03 " << item_selected_to_remove_idx << "\n";
						item_selected_to_remove_idx = n;
//std::cout << "remove_project 04 " << item_selected_to_remove_idx << "\n";
						//item_changed = true;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

//std::cout << "remove_project 05AS \n";

			std::string selected_project_to_remove = wsm_projects_data.wsm_projects[item_selected_to_remove_idx].first + " : " + wsm_projects_data.wsm_projects[item_selected_to_remove_idx].second.string();

//std::cout << "remove_project 05B \n";

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Text("Selected project to remove");
			ImGui::Text(selected_project_to_remove.c_str());
			ImGui::Separator();
			ImGui::NewLine();

//std::cout << "remove_project 06 : " << item_selected_to_remove_idx  << ":" << selected_wsm_project << "\n";

			if (ImGui::Button("Remove")) {
				wsm_projects_data.remove_wsm_project(item_selected_to_remove_idx);
				if (item_selected_to_remove_idx == selected_wsm_project) { // Nominated to delete currently selected project displayed in the tree display panel
					ws_tree_manager.delete_ws_tree(true);
					selected_wsm_project = -1;

					ws_tree_manager.ws_tree.label = " Undefined";
					ws_tree_manager.root_path = "";
					ws_tree_manager.ws_tree.item_path = "";
				}

				projects_changed = true;

				open_remove_popup = false;
				ImGui::CloseCurrentPopup();
			}

//std::cout << "remove_project 07 \n";
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				open_remove_popup = false;
				ImGui::CloseCurrentPopup();
			}

//std::cout << "remove_project 08 \n";
			ImGui::EndPopup();
		}
	}

	void edit_project_label() {
		ImGui::OpenPopup("Edit web Site Label");

			if (ImGui::BeginPopupModal("Edit web Site Label", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
//printf("remove_project 11111 \n");
				if (wsm_projects_data.wsm_projects.size() < 1) {
					ImGui::Text("No projects to remove !");
					ImGui::NewLine();
					if (ImGui::Button("EXIT", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
					}
				} else {
					if (selected_project_label_to_edit.empty()) 
						selected_project_label_to_edit = wsm_projects_data.wsm_projects[0].first;
					}

				// --------- Display Web Site Manager project selection combo widget ---------------

				combo_flags = ImGuiComboFlags_WidthFitPreview;// Apparently need this to be reset every display cycle or will not work

				// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
				const char* combo_preview_value = wsm_projects_data.wsm_projects[item_selected_to_change_idx].first.c_str();
				if (ImGui::BeginCombo("Select Web Site Project to change label", combo_preview_value, combo_flags))
				{
					for (int n = 0; n < wsm_projects_data.wsm_projects.size(); n++)
					{
						const bool is_selected = (item_selected_to_change_idx == n);

						//std::cout << "display_wsm_main_gui_panel 02 "<< wsm_projects_data.wsm_projects[n].first << " :: " << "\n";
						if (ImGui::Selectable(wsm_projects_data.wsm_projects[n].first.c_str(), is_selected)) {
							item_selected_to_change_idx = n;
							item_changed = true;
							selected_project_label_to_edit = wsm_projects_data.wsm_projects[item_selected_to_change_idx].first; // Does not work
//printf("HERE 0000000 %i : %i :: %s\n",n, item_selected_to_change_idx, old_selected_project_to_edit.c_str());
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected) {
							ImGui::SetItemDefaultFocus();
							selected_project_label_to_edit = wsm_projects_data.wsm_projects[item_selected_to_change_idx].first; // Does not work
//printf("HERE 111111 : %i  : % i :: %s\n",n, item_selected_to_change_idx, old_selected_project_to_edit.c_str());
						}
					}
					ImGui::EndCombo();
				}

				float y_pos = 50.0f;

				ImGui::SetCursorPosY(y_pos);
				ImGui::NewLine();
				ImGui::Separator();

				// Cannot use a  selected_project_label_to_edit that is aaaigned a the value of  wsm_projects_data.wsm_projects[item_selected_to_change_idx].first
				// in any way to later change the value of wsm_projects_data.wsm_projects[item_selected_to_change_idx].first = selected_project_label_to_edit
				// A ImGui thing it seems !!!!!!!
				ascii_text_input("Project Label", selected_project_label_to_edit, 50, y_pos + 30.0f, 50);
				//ascii_text_input("Project Label", wsm_projects_data.wsm_projects[item_selected_to_change_idx].first, 50, y_pos + 30.0f, 50);

				ImGui::SetCursorPosY(y_pos + 60.0f);

				ImGui::Separator();
				ImGui::NewLine();

				if (ImGui::Button("Change") && !selected_project_label_to_edit.empty()) {

//printf("BEFORE CHANGE label %s :: %s :: %s\n", wsm_projects_data.wsm_projects[item_selected_to_change_idx].first.c_str(), wsm_projects_data.wsm_projects[item_selected_to_change_idx].second.string().c_str(), old_selected_project_to_edit.c_str());
		
					// The only way to get wsm_projects_data.wsm_projects[item_selected_to_change_idx].first assigned to the current value of selected_project_label_to_edit  !!!!!
					// wsm_projects_data.wsm_projects[item_selected_to_change_idx].first = selected_project_label_to_edit; 
					// or  wsm_projects_data.wsm_projects[item_selected_to_change_idx].first.assign(selected_project_label_to_edit); gives icorrect previous original assigned value of wsm_project.first 
					// !!!!! Wierd and Crazy and so unintuitive result that is a major error prone bug :: ImGui related ???? !!!!!!
					// Took over 3 hours of looking for alternative methods before figuring this out
					
					wsm_projects_data.wsm_projects[item_selected_to_change_idx].first = std::string(selected_project_label_to_edit);
					old_selected_project_to_edit = wsm_projects_data.wsm_projects[item_selected_to_change_idx].first.c_str();// old_selected_project_to_edit defined outside function only way to work

					change_tree_label(wsm_projects_data.wsm_projects[item_selected_to_change_idx]);

//printf("AFTER CHANGE label : %s :: %s\n", selected_project_label_to_edit.c_str(), old_selected_project_to_edit.c_str());

					open_edit_project_label_popup = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
//printf("BEFORE Cancel lable %s :: %s\n", wsm_projects_data.wsm_projects[item_selected_to_change_idx].first.c_str(), old_selected_project_to_edit.c_str());
					if(!old_selected_project_to_edit.empty()) wsm_projects_data.wsm_projects[item_selected_to_change_idx].first = old_selected_project_to_edit;
//printf("AFTER Cancel lable %s :: %s\n", wsm_projects_data.wsm_projects[item_selected_to_change_idx].first.c_str(), old_selected_project_to_edit.c_str());

					open_edit_project_label_popup = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
	}

	void change_tree_label(wsm_project_data_type wsm_project) {
//printf("change_tree_label 000 %s :: %s :: %s\n", wsm_project.first.c_str(), wsm_project.second.string().c_str(), ws_tree.ws_tree.item_path.string().c_str());
//printf("change_tree_label 111 %s \n", ws_tree.ws_tree.label.c_str());

		if (ws_tree_manager.ws_tree.item_path == wsm_project.second){
			// The only way to get ws_tree.ws_tree.label assigned to the current value of wsm_project.first  !!!!!
			// ws_tree.ws_tree.label = wsm_project.first; or ws_tree.ws_tree.label.assign(wsm_project.first); gives icorrect previous original assigned value of wsm_project.first 
			// !!!!! Wierd and Crazy and so unintuitive result that is a major error prone bug :: ImGui related ???? !!!!!! 
			ws_tree_manager.ws_tree.label = std::string(wsm_project.first);
			
//printf("change_tree_label 222 %s :: %s \n", ws_tree.ws_tree.label.c_str(), wsm_project.first.c_str());
		}
	}

	bool read_wsm_projects_listing_file(std::filesystem::path project_listings_path) {
		std::fstream projects_listing_file;

		projects_listing_file.open(project_listings_path, std::ios::in);

		if (!projects_listing_file)
			return false;
		else
			if (!projects_listing_file.is_open()) return false;

//std::cout << "read_wsm_projects_listing_file 03\n";
		std::string working_model_string = FW::filetools::read_all(projects_listing_file);
		projects_listing_file.close();

//std::cout << "read_wsm_projects_listing_file 03\n";
		lines = FW::stringtools::split(working_model_string, '\n');
//std::cout << "define_html_file_links 04\n";		
		line_number = 0;

//std::cout << "read_wsm_projects_listing_file 05\n";
		return define_wsm_projects_list();
	}

	bool define_wsm_projects_list() {
		if (lines.size() < 1) {
			selected_wsm_project = -1;
			return false;
		}

		selected_wsm_project = std::stoi(lines[0]);// last saved current selected wsm project index number 

		for (size_t i = 1; i < lines.size(); i++) {

//std::cout << "define_wsm_projects_list 01 "<< lines[i] << "\n";
			std::vector<std::string> line = FW::stringtools::split(lines[i], '|');

//std::cout << "define_wsm_projects_list 02 "<< line[0] << " :: " << line[1] << "\n";
			if(!line[0].empty() && !line[1].empty())
				wsm_projects_data.add_wsm_project(line[0], line[1]);
		}

		return true;
	}

	bool read_wsm_application_listing_file(std::filesystem::path application_listings_path) {
		std::fstream application_listing_file;

		application_listing_file.open(application_listings_path, std::ios::in);

		if (!application_listing_file)
			return false;
		else
			if (!application_listing_file.is_open()) return false;

//std::cout << "read_wsm_application_listing_file 03\n";
		std::string working_model_string = FW::filetools::read_all(application_listing_file);
		application_listing_file.close();

//std::cout << "read_wsm_application_listing_file 03\n";
		lines = FW::stringtools::split(working_model_string, '\n');
//std::cout << "define_html_file_links 04\n";		
		line_number = 0;

//std::cout << "read_wsm_application_listing_file 05\n";
		return define_wsm_application_list();
	}

	bool define_wsm_application_list() {
		for (size_t i = 0; i < lines.size(); i++) {

//std::cout << "define_wsm_application_list 01 "<< lines[i] << "\n";
			std::vector<std::string> line = FW::stringtools::split(lines[i], '|');

//std::cout << "define_wsm_application_list 02 "<< line[0] << " :: " << line[1] << "\n";
			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "IMAGES")) {
				ws_tree_manager.set_image_editor(line[1]);
				continue;
			}

			// Will not have this defined as to open view html file will use system default browser firefox
			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "BROWSER")) {
				ws_tree_manager.set_html_browser(line[1]);
				continue;
			}

			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "HTML_EDITOR")) {
				ws_tree_manager.set_html_editor(line[1]);
				continue;
			}

			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "TEXT")) {
				ws_tree_manager.set_text_editor(line[1]);
				continue;
			}

			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "VIDEO")) {
				ws_tree_manager.set_media_editor(line[1]);
				continue;
			}

			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "AUDIO")) {
				ws_tree_manager.set_audio_editor(line[1]);
				continue;
			}

			if (FW::stringtools::contains(FW::stringtools::toUpperCase(line[0]), "PDF")) {
				ws_tree_manager.set_pdf_editor(line[1]);
			}
		}

		return true;
	}

	bool create_wsm_projects_listing_file() {
		std::ofstream file;

		// Opening file WSM_PROJECT_LIST_FILE_PATHNAME in write mode.
		file.open(WSM_PROJECT_LIST_FILE_PATHNAME);
		//file.open(WSM_PROJECT_LIST_FILE_PATHNAME, std::ios::out);

		// Check if the file was successfully created.
		if (!file.is_open()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to initialise WSM  : Could not open/create list of projects file. \n");
			return false;
		}

		file << "-1";

		// Close the file to free up resources.
		file.close();

		return true;
	}

	// -----------------------------------------

	void define_html_editor() {
printf("define_html_editor \n");

		std::filesystem::path html_editor = get_executable_file_pathname();

		if (!html_editor.empty()) {
			ws_tree_manager.set_html_editor(html_editor);
			applications_changed = true;
		}
	}

	void define_image_editor() {
//printf("define_image_editor 0000 \n");
		std::filesystem::path image_editor = get_executable_file_pathname();

		if (!image_editor.empty()) {
			ws_tree_manager.set_image_editor(image_editor);
			applications_changed = true;
		}

	}

	void define_text_editor() {
printf("define_image_editor \n");

		std::filesystem::path text_editor = get_executable_file_pathname();

		if (!text_editor.empty()) {
			ws_tree_manager.set_text_editor(text_editor);
			applications_changed = true;
		}
	}

	void define_media_player() {
printf("define_image_editor \n");

		std::filesystem::path media_editor = get_executable_file_pathname();

		if (!media_editor.empty()) {
			ws_tree_manager.set_media_editor(media_editor);
			applications_changed = true;
		}
	}

	// -----------------------------------------
	void clear_all_selected_items() {
printf("clear_all_selected_items \n");
		ws_tree_manager.tree_selection.clear_selection();
	}

	void delete_all_selected_items() {
printf("delete_all_selected_items \n");

		for(ws_tree_node_item_struct_type *tree_node : ws_tree_manager.tree_selection.tree_node_selection){
			// Not yet implemented

printf("delete_all_selected_items 1111 %s \n", tree_node->item_path.string().c_str());
		}

	}

	bool save_wsm_applications() {
		std::filesystem::path application_listings_path = WSM_APPLICATION_LIST_FILE_PATHNAME;

		std::ofstream application_listing_file;

		application_listing_file.open(application_listings_path, std::ios::trunc);// Overwrite existing projects file

		if (!application_listing_file) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to save projects application listing  : Could not open/create list of applications file for writing to. \n");
			return false;
		}
		else
			if (!application_listing_file.is_open()) return false;

		application_listing_file << "IMAGES|" << ws_tree_manager.image_editor_pathname << "\n";
		application_listing_file << "BROWSER|" << ws_tree_manager.browser_pathname << "\n";
		application_listing_file << "HTML_EDITOR|" << ws_tree_manager.html_editor_pathname   << "\n";
		application_listing_file << "TEXT|"	<< ws_tree_manager.text_editor_pathname   << "\n";
		application_listing_file << "VIDEO|" << ws_tree_manager.media_editor_pathname  << "\n";
		application_listing_file << "AUDIO|" << ws_tree_manager.audio_editor_pathname  << "\n";
		application_listing_file << "PDF|" << ws_tree_manager.pdf_viewer_pathname    << "\n";

		// Close the file to free up resources.
		application_listing_file.close();

		return true;
	}

	// --------------------------------------------------------

	std::filesystem::path get_executable_file_pathname() {
//printf("get_executable_file_pathname 0000 \n");

		std::filesystem::path image_editor = "";

		char const *extension[] = {"*.exe"};

		char const* image_editor_c = vwDialogs::open_file(nullptr, extension,1);

		if (!image_editor_c) return image_editor;;

//printf("get_executable_file_pathname 1111 %s \n", image_editor_c);

		std::string image_editor_s = image_editor_c;

//printf("get_executable_file_pathname 2222 %s \n", image_editor_s.c_str());

		if (!image_editor_s.empty()) {
			image_editor = image_editor_s;

//printf("get_executable_file_pathname 3333 %s \n", image_editor.string().c_str());

			if (!std::filesystem::exists(image_editor)) {
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "Unable to use image Viewer/Editor : " + image_editor_s + "\nFile Does not exist.\n");
				image_editor = "";
			}
			//else {
//printf("get_executable_file_pathname 4444 :: expression_file_pathname != NULL %s \n", image_editor.string().c_str());

				return image_editor;

			//}

		}
	}

private:
	wsm_projects_data_struct_type wsm_projects_data;
	
	ws_tree_manager_class ws_tree_manager;

	std::vector<std::string> lines = {};

	int line_number = 0;
	int selected_wsm_project      = -1;
	int prev_selected_wsm_project = -1;

	ImGuiComboFlags combo_flags = 0;
	bool item_changed				   = false;
	bool first_run					   = true;
	bool open_remove_popup			   = false;
	bool open_edit_project_label_popup = false;

	bool projects_changed	  = false;
	bool applications_changed = false;

	std::string old_selected_project_to_edit = "";
	std::string selected_project_label_to_edit = "";
	int item_selected_to_change_idx = 0; // Here we store our selection data as an index.
};