#pragma once

#include <Windows.h>

//#include <thread>// +++++

#include "Source/Tree_Framework/ws_tree.h"

class ws_tree_manager_class {
public:
	ws_tree_manager_class() {}
	~ws_tree_manager_class() {}

	ws_tree_class ws_tree;

	tree_selection_struct_type    tree_selection = {};

	std::filesystem::path root_path = INVALID_PATH;

		// These \\ needed in pathname because of the stupid windows use of \ to define diectory pathnames which is also used to 
	// define literal text to be used in strings of commands where a special command or parameter are set in a command string
	//std::filesystem::path image_editor_pathname = "C:\\Program Files\\XnViewMP\\xnviewmp.exe";
	//std::filesystem::path image_editor_pathname = "C:/Program Files/XnViewMP/xnviewmp.exe";// This works so use this instead of \\ directory file deliminator
	std::filesystem::path image_editor_pathname = "";// This works so use this instead of \\ directory file deliminator
	
	std::filesystem::path browser_pathname		= "";// This works so use this instead of \\ directory file deliminator
	std::filesystem::path html_editor_pathname  = "";// This works so use this instead of \\ directory file deliminator
	std::filesystem::path text_editor_pathname  = "";// This works so use this instead of \\ directory file deliminator
	std::filesystem::path media_editor_pathname = "";// This works so use this instead of \\ directory file deliminator
	std::filesystem::path audio_editor_pathname = "";// This works so use this instead of \\ directory file deliminator
	std::filesystem::path pdf_viewer_pathname   = "";// This works so use this instead of \\ directory file deliminator

	// ----------------------------
	bool create_ws_tree(std::string label, std::filesystem::path ws_root_path) {
		if(!ws_tree.create_root_tree(label, ws_root_path)) return false;

		root_path = ws_root_path;

		// In trying to use a thread so as to dipalay a message while creating web site tree
		// get compile error C3867 nonstandard syntax; use '&' to create a pointer to member 
		// despite doing exactly that !!! Cannot use. Not fist time have this problem. std::thread useless ???
		//std::thread define_html_links_thread(define_all_html_nodes_links,&ws_tree);

		afw_globalc::get_current_logger()->log(LogLevel::INFO, "\n!!!!! CREATING WEB SITE TREE. !!!!!\n!!!! PLEASE WAIT. !!!!!\nTHIS MAY TAKE SOME TIME\nDEPENDING ON WEB SITE SIZE\n");

		define_all_html_nodes_links(&ws_tree); // &ws_tree is address to tree root node

		afw_globalc::get_current_logger()->log(LogLevel::INFO, "#### CREATING WEB SITE TREE COMPLETED. #####\n");


		//output_html_nodes_links(&ws_tree); // For testing only : comment out when not in use
	}

	void delete_ws_tree(bool delete_disc_directory) {
		ws_tree.delete_ws_tree(delete_disc_directory);
	}

	// ----------------------------
	bool display_ws_tree() {
		if (ws_tree.item_path.empty()) {
			//afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "display_root_tree ::  Root tree path not defined");
			return false;
		}
		if (ws_tree.tree_node_item_type != afw_tree_node_item_type_enum::root_tree_node) {
			//afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "display_root_tree ::  Root tree item type not valid");
			return false;
		}

		//ImGui::Begin(ws_tree.window_label.c_str());// A ImGui main UI containner window to hold all other UI panels. Uncomment if want tree displayed in a stand alone window
		std::string label = ws_tree.label;

		if (ImGui::TreeNode(label.c_str())) {
			display_tree_branch(&ws_tree);
			ImGui::TreePop();
		}
		//ImGui::End(); //Uncomment if want tree displayed in a stand alone window

		return true;
	}


	bool display_tree_branch(ws_tree_class* tree_branch) {
		if (!tree_branch) {
			//afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "display_tree_branch ::  Tree branch not defined ie nullptr.");
			return false;
		}

		if (tree_branch->item_path.empty()) {
			//afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "display_tree_branch ::  Root tree path not defined");
			return false;
		}

		if (tree_branch->tree_node_item_type == afw_tree_node_item_type_enum::undefined) {
			//afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "display_tree_branch ::  Tree Branch item type not defined");
			return false;
		}

		if (tree_branch->tree_node_item_type == afw_tree_node_item_type_enum::root_tree_node) {
			if (ImGui::GetIO().KeyCtrl && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
//printf("display_tree_branch :move to root tree branch: %s :: %s \n", tree_branch->label.c_str(),tree_branch->item_path.string().c_str());
				ws_tree.perform_selection_move(tree_branch, tree_selection); // testing only : in future will have menu selection to move, delete, copy, paste rename and other tasks
				tree_selection.clear_selection();
			}
		}

		// Tree branch menus and functions
		for (int i = 0; i < tree_branch->branches.size(); i++) {
			ws_tree_class *child_tree_branch = tree_branch->branches[i];
			std::string label = child_tree_branch->icon_id + child_tree_branch->label;

			ImGuiTreeNodeFlags branch_node_flags = branch_base_node_flags;
			if (child_tree_branch->selected) branch_node_flags |= ImGuiTreeNodeFlags_Selected;

			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)child_tree_branch->tree_node_id, branch_node_flags, label.c_str());
			// ----------------- BRANCH SELECTION --------------
			if (!ImGui::GetIO().KeyCtrl && selected_tree_branch_label()) {
				if (tree_selection.is_currently_selected(child_tree_branch))
					tree_selection.remove_tree_node_selection(child_tree_branch);
				else {
					tree_selection.clear_selection();
					tree_selection.add_tree_node_selection(child_tree_branch);
				}
//printf("display_tree_branch : single selected tree_branch  : %i : %s : %s \n", i , child_tree_branch->label.c_str(),  child_tree_branch->item_path.extension().string().c_str());
			}

			if (ImGui::GetIO().KeyCtrl && selected_tree_branch_label()) {
				if (tree_selection.is_currently_selected(child_tree_branch))
					tree_selection.remove_tree_node_selection(child_tree_branch);
				else
					tree_selection.add_tree_node_selection(child_tree_branch);
//printf("display_tree_branch : multiple selected branch: %s : %i \n", child_tree_branch->label.c_str(), i);
			}
			// -----------------------------------------------------

			std::string tree_branch_popup_id = std::to_string(child_tree_branch->tree_node_id);

			if (ImGui::BeginPopupContextItem(tree_branch_popup_id.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
				selected_menu_branch_index = child_tree_branch->tree_node_id;// Need child_tree_branch->tree_node_id in this to prevent functions being executed for every tree leaf item
				action_tree_node = child_tree_branch;

//printf("display_tree_branch :IsItemClicked: %s : %i \n", child_tree_branch->label.c_str(), i);
				if (ImGui::BeginMenu("Tree branch ...")) {
//printf("display_tree_branch :BeginMenu: %s : %i \n", child_tree_branch->label.c_str(), i);

					if (ImGui::BeginMenu("Select ...")) {
						ImGui::SeparatorText("Selection ");
						if (ImGui::MenuItem("Add branch to selection")) {
//printf("Add Branch :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							add_branch_to_selection(child_tree_branch);
						}

						if (ImGui::MenuItem("Remove branch from selection")) {
//printf("Remove Branch :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							remove_branch_from_selection(child_tree_branch);
						}

						if (ImGui::MenuItem("Unlinked branch Files")) {
//printf("Select Unlinked Files :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							select_branch_unlinked_files(child_tree_branch);
						}

						if (ImGui::MenuItem("Image Files")) {
//printf("Select Image Files :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							select_branch_leaf_items(child_tree_branch, afw_file_item_category_type_enum::image);
						}

						if (ImGui::MenuItem("Media Files")) {
//printf("Select Media Files :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							select_branch_leaf_items(child_tree_branch, afw_file_item_category_type_enum::media);
						}

						// !!!!! Add more file types to select here !!!!!!
						if (ImGui::MenuItem("Invert selected items")) {
//printf("Invert branch items :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							invert_tree_branch_selection(child_tree_branch);
						}

						if (ImGui::MenuItem("Deselect branch items")) {
//printf("Deselect branch items :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							deselect_all_branch_items(child_tree_branch);
						}

						if (ImGui::MenuItem("Delete branch selected items")) {
//printf("Delete branch selected items :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							delete_selected_branch_items(child_tree_branch);
						}

						ImGui::EndMenu();
					}

					ImGui::SeparatorText("Action");
					if (tree_selection.tree_node_selection.size() > 0) {

						if (ImGui::MenuItem("Move selected items to and update")) {
//printf("Move selected items and update :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
							selected_dir_pathname = child_tree_branch->item_path;
							perform_selection_move_and_update = true;
						}
						ImGui::Separator();
					}

					if (ImGui::MenuItem("Rename Branch")) {
						tree_selection.clear_selection();
						tree_selection.add_tree_node_selection(tree_branch);
//printf("Rename selected items :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
						selected_dir_pathname = child_tree_branch->item_path;
						perform_rename_branch = true;
					}

					if (ImGui::MenuItem("Refresh Branch Contents")) {
						tree_selection.clear_selection();
						tree_selection.add_tree_node_selection(tree_branch);
//printf("Refresh :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
						//selected_dir_pathname = child_tree_branch->item_path;
						perform_refresh_branch = true;
					}

					if (ImGui::MenuItem("Create Branch")) {
//printf("Create tree branch items :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
						//perform_create_ws_tree_branch(child_tree_branch);
						perform_create_branch = true;
					}

					if (ImGui::MenuItem("Delete Branch")) {
//printf("Delete selected items :MenuItem: %s : %i \n", child_tree_branch->label.c_str(), i);
						//selected_dir_pathname = child_tree_branch->item_path;
						perform_delete_branch = true;
					}

					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

			if (perform_selection_move_and_update && selected_menu_branch_index == child_tree_branch->tree_node_id) { // Need i in this to prevent this function being executed for every tree leaf item
//printf("display_tree_branch :move to child branch 000: %s : %i \n", child_tree_branch->label.c_str(), i);
				ws_tree.perform_selection_move(child_tree_branch, tree_selection);
				tree_selection.clear_selection();
				perform_selection_move_and_update = false;
			}

			if (perform_rename_branch && selected_dir_pathname == child_tree_branch->item_path) { // Need i in this to prevent this function being executed for every tree leaf item
//printf("display_tree_branch :Rename selected branch 000: %s : %i \n", child_tree_branch->label.c_str(), i);
				rename_tree_branch(child_tree_branch); 
				tree_selection.clear_selection();
				//perform_rename_branch = false;
			}

			// !!!!!!!!!!!!!!!!!!!!!!!!!
			if (perform_refresh_branch && selected_menu_branch_index == child_tree_branch->tree_node_id) { // Need i in this to prevent this function being executed for every tree leaf item
//printf("display_tree_branch :Refresh selected branch 000: %s : %i \n", child_tree_branch->label.c_str(), i);
				ws_tree.refresh_tree_branch(child_tree_branch); 
				tree_selection.clear_selection();
				perform_refresh_branch = false;
			}

			if (perform_create_branch && selected_menu_branch_index == child_tree_branch->tree_node_id) { // Need i in this to prevent this function being executed for every tree leaf item
//printf("display_tree_branch :create tree branch 000: %s : %i : %i \n", child_tree_branch->label.c_str(), i, tree_branch->branches.size());
				perform_create_ws_tree_branch(child_tree_branch);
				//perform_create_branch = false;
			}

			if (perform_delete_branch && selected_menu_branch_index == child_tree_branch->tree_node_id) { // Need i in this to prevent this function being executed for every tree leaf item
//printf("display_tree_branch :delete selected branch 000: %s : %i : %i \n", child_tree_branch->label.c_str(), i, tree_branch->branches.size());
				ws_tree.delete_ws_tree_branch(child_tree_branch,true);
				tree_selection.clear_selection();
				perform_delete_branch = false;
			}
			// !!!!!!!!!!!!!!!!!!!!!!!!!


			if (ImGui::GetIO().KeyCtrl && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
//printf("display_tree_branch :move to child branch 111: %s : %i \n", child_tree_branch->label.c_str(), i);
				ws_tree.perform_selection_move(child_tree_branch, tree_selection); // testing only : in future will have menu selection to move, delete, copy, paste rename and other tasks
				tree_selection.clear_selection();
			}

			if (node_open) {
				display_tree_branch(child_tree_branch);
				ImGui::TreePop();
				//node_open = false;
			}
		}

		// Tree leaf menus and functions
		for (int j = 0; j < tree_branch->leaves.size(); j++) {
			ws_tree_node_item_struct_type *child_tree_leaf = tree_branch->leaves[j];// Need reference to update child tree leaf to and from links 
			std::string label = child_tree_leaf->icon_id + child_tree_leaf->label;

			ImGuiTreeNodeFlags leaf_node_flags = leaf_base_node_flags;
			if (child_tree_leaf->selected) leaf_node_flags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx((void*)(intptr_t)child_tree_leaf->tree_node_id, leaf_node_flags, label.c_str());



			// ----------------- LEAF SELECTION --------------

			if (!ImGui::GetIO().KeyCtrl && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
				if (tree_selection.is_currently_selected(child_tree_leaf))
					tree_selection.remove_tree_node_selection(child_tree_leaf);
				else {
					tree_selection.clear_selection();
					tree_selection.add_tree_node_selection(child_tree_leaf);
				}
printf("display_tree_branch : single selected leaf  : %s : %i : %s \n", child_tree_leaf->label.c_str(), j, child_tree_leaf->item_path.extension().string().c_str());
				selected_menu_item_index = -1;
			}

			if (ImGui::GetIO().KeyCtrl && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
				if (tree_selection.is_currently_selected(child_tree_leaf))
					tree_selection.remove_tree_node_selection(child_tree_leaf);
				else
					tree_selection.add_tree_node_selection(child_tree_leaf);
printf("display_tree_branch : multiple selected leaf : %s : %i : %s \n", child_tree_leaf->label.c_str(), j, child_tree_leaf->item_path.extension().string().c_str());
				selected_menu_item_index = -1;
			}
			// -----------------------------------------------------

			std::string tree_item_popup_id = std::to_string(child_tree_leaf->tree_node_id);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {// Must have ImGui::IsItemHovered() or the first leaf of the banch is selected
				tree_selection.clear_selection();
				tree_selection.add_tree_node_selection(child_tree_leaf);
printf("Double click selection 000 : MenuItem: %s : %i \n", child_tree_leaf->label.c_str(), j);
//printf("Double click selection 111 : MenuItem: %s : %i \n", tree_selection.tree_node_selection[0]->label.c_str(), i);
				open_item_application(tree_selection);
				selected_menu_item_index = -1;
				break; // This is needed or will get open_item_application(tree_selection) executed for the number of leaf entries in in the tree branch
			}

			if (ImGui::BeginPopupContextItem(tree_item_popup_id.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
				selected_menu_item_index = child_tree_leaf->tree_node_id;// Need child_tree_leaf->tree_node_id in this to prevent functions being executed for every tree leaf and tree branch item
				action_tree_node = child_tree_leaf;
//printf("display_tree_branch :IsItemClicked: %s : %i \n", child_tree_leaf->label.c_str(), j);
				if (ImGui::BeginMenu("Tree node ...")) {
//printf("display_tree_branch :BeginMenu: %s : %i \n", child_tree_branch->label.c_str(), i);
					ImGui::SeparatorText("Selection");
					if (ImGui::MenuItem("Add to selection")) {
//printf("Add to selection :MenuItem: %s : %i \n", child_tree_leaf->label.c_str(), j);
						tree_selection.add_tree_node_selection(child_tree_leaf);
					}

					if (ImGui::MenuItem("Remove from selection")) {
//printf("Remove from selection :MenuItem: %s : %i \n", child_tree_leaf->label.c_str(), j);
						tree_selection.remove_tree_node_selection(child_tree_leaf);
					}

					//if (tree_selection.tree_node_selection.size() > 0) {
						ImGui::SeparatorText("Action");

						if (ImGui::MenuItem("Delete selected item")) {
//printf("Delete selected item :MenuItem: %i \n", j);
							ws_tree.delete_branch_leaf(tree_branch, child_tree_leaf->tree_node_id,true);

						}

						if (ImGui::MenuItem("Rename item")) {
//printf("Rename selected items :MenuItem: %i \n", j);
							open_rename_branch_leaf_popup = true;
						}

						if (ImGui::MenuItem("Make a Copy of item")) {
//printf("Make a copy of selected items :MenuItem: %i \n",j);
							ws_tree.create_copy_of_leaf(tree_branch, child_tree_leaf->tree_node_id);
						}


						if (ImGui::MenuItem("Update html links")) {
printf("Update html links: %i \n", j);
							ws_tree.update_edited_html_file_leaf_links(child_tree_leaf); // Needs testing
						}

						ImGui::Separator();

						std::string extension =  FW::stringtools::toLowerCase(child_tree_leaf->item_path.extension().string());

						if (FW::stringtools::contains(extension, "htm")) {
							if (ImGui::BeginMenu("Web Page Clear ...")) {
								if (ImGui::MenuItem("All external html links")) {
//printf("Clear all external links :MenuItem: %i \n",j);
									ws_html_manager.clear_html_links(child_tree_leaf->item_path,true);
									ws_tree.update_edited_html_file_leaf_links(child_tree_leaf);
									//break;// ImGui does not like this
								}

								if (ImGui::MenuItem("All html links")) {
//printf("Clear all links :MenuItem: %i \n", j);
									ws_html_manager.clear_html_links(child_tree_leaf->item_path);
									ws_tree.update_edited_html_file_leaf_links(child_tree_leaf);
								}


								if (ImGui::MenuItem("All broken links (html and source)")) {
									//not yet implemented
printf("Clear all broken links :MenuItem: %i \n", j);
								}

								ImGui::EndMenu();
							}

							if (ImGui::BeginMenu("Web Page Display ...")) {
								if (ImGui::MenuItem("In web browser")) {
//printf("In web browser :MenuItem: %i :: %s\n", i, child_tree_leaf->item_path.string().c_str());
									open_html_browser_viewer(browser_pathname, child_tree_leaf->item_path);
									//break;// Including this crashes application// Need this to prevent this function being executed for every tree leaf item
								}

								if (ImGui::MenuItem("List all external links")) {
//printf("List all external links :MenuItem: %i :: %s\n", i, child_tree_leaf->item_path.string().c_str());
									open_external_links_list_popup = true;
								}

								if (ImGui::MenuItem("List all files this links to")) {
//printf("List of all files this links to :MenuItem: %i \n", i);
									open_internal_to_links_list_popup = true;	
								}

								if (ImGui::MenuItem("List all links to this file01")) {
//printf("List of all links to this file :MenuItem: %i \n", i);
									open_internal_from_links_list_popup = true;
								}

								if (ImGui::MenuItem("List all broken links")) {
//printf("List all broken links :MenuItem: %i \n", i);
									open_broken_links_list_popup = true;	
								}

								ImGui::EndMenu();
							}

							//Place other html web page context menus here

						} else {
								if (ImGui::MenuItem("List all links to this file")) {
//printf("List of all links to this file :MenuItem: %i :: %i \n", selected_menu_item_index, j);
									open_internal_from_links_list_popup = true;
								}

								//Place other non html web page context menus or menu items here
						}



						ImGui::Separator();
/*
						if (ImGui::BeginMenu("Open With ...")) {
printf("Open With :Menu: %i \n", i);

							if (ImGui::MenuItem("Test Open Menu Item")) {
printf("Test Open Menu Item :MenuItem: %i \n", i);
							}

							ImGui::EndMenu();
						}
*/
					//}


					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}


			// #################### TESTING BLOCK TO BE INTEGRATED WHEN PROJECT STARTED UP OR UPDATED ######################
			//if (ImGui::GetIO().KeyCtrl && ImGui::IsItemClicked(ImGuiMouseButton_Middle)) { // testing only : in future will have menu selection to move, delete, copy, paste rename and other tasks
			//	define_leaf_html_node_links(tree_branch,child_tree_leaf);
//printf("display_tree_branch html_nodes 00 \n");
//printf("display_tree_branch html_nodes : %i \n", html_nodes.size());
//printf("display_tree_branch html_nodes : %i :: %s\n", html_nodes.size(), html_nodes[0].item_path.string().c_str());

			//}

			if (open_external_links_list_popup && selected_menu_item_index == child_tree_leaf->tree_node_id) { // Need i in this to prevent this function being executed for every tree leaf item
				list_all_external_links(child_tree_leaf);
			}

			if (open_internal_from_links_list_popup && selected_menu_item_index == child_tree_leaf->tree_node_id) {
//std::cout << "list_all_internal_from_links " <<  j << "\n";
				list_all_internal_from_links(child_tree_leaf);
			}

			if (open_internal_to_links_list_popup && selected_menu_item_index == child_tree_leaf->tree_node_id) {
//std::cout << "list_all_internal_from_links " <<  j << "\n";
				list_all_internal_to_links(child_tree_leaf);
			}

			if (open_broken_links_list_popup && selected_menu_item_index == child_tree_leaf->tree_node_id) {
				list_all_broken_links(child_tree_leaf);
			}

			if(open_rename_branch_leaf_popup && selected_menu_item_index == child_tree_leaf->tree_node_id) {
//std::cout << "list_all_internal_from_links " << child_tree_leaf->item_path.string() << " : " << child_tree_leaf->tree_node_id << "\n";
				rename_branch_leaf_filename(child_tree_leaf);
			}
		}

		return true;
	}

	// --------------------------------------------------------------------------------------------------------


	void define_all_html_nodes_links(ws_tree_class *tree_branch) {
		for (ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves) { // CRITICAL : Need leaf variable as a reference so updates to links vector arrays is to be saved 
			ws_tree.define_html_tree_leaf_links(leaf);
		}

		for (ws_tree_class* branch : tree_branch->branches) {
			define_all_html_nodes_links(branch);
		}
	}

	// For testing 
	void output_html_nodes_links(ws_tree_class* tree_branch) {
		for (ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves) {

			for (ws_tree_node_item_struct_type *internal_link_from : leaf->internal_links_from) {
				printf("output_html_nodes_links : internal_link_from : %s : %s \n", leaf->item_path.string().c_str(), internal_link_from->item_path.string().c_str());
			}

			for (ws_tree_node_item_struct_type* internal_link_to : leaf->internal_links_to) {
				printf("output_html_nodes_links : internal_link_to : %s : %s\n", leaf->item_path.string().c_str(), internal_link_to->item_path.string().c_str());
			}

			//for (std::filesystem::path external_link_to : leaf.ws_tree_node_links.external_links_to) {
			//	printf("output_html_nodes_links : external_link_to : %s \n", external_link_to.string().c_str());
			//}

			if (ws_tree.is_web_page(leaf)) {

				if (leaf->internal_links_from.empty())
					printf("output_html_nodes_links : internal_link_from empty : %s \n", leaf->item_path.string().c_str());

				if (leaf->internal_links_to.empty())
					printf("output_html_nodes_links : internal_links_to empty : %s \n", leaf->item_path.string().c_str());
			}
		}

		for (ws_tree_class* branch : tree_branch->branches) {
			output_html_nodes_links(branch);
		}
	}

	// ---------------------- BRANCH SELECTION FUNCTIONS ----------------------------

	// Select all unlinked files that exist in a tree branch with option to add to existing selection list
	// NOTE : Sub-branches are not included but a recursive algorithm option can be created or added to do this.
	void select_branch_unlinked_files(ws_tree_class *tree_branch, bool add_to_selection = false){
		if(!add_to_selection) tree_selection.clear_selection();

		for(ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves){
			if (leaf->internal_links_from.empty() && leaf->internal_links_to.empty()) { //leaf has no links
				bool selected = false;
				//if (add_to_selection) { //if adding to selection check not already seleced
					for(ws_tree_node_item_struct_type *selected_item : tree_selection.tree_node_selection){
						if (selected_item->item_path == leaf->item_path) {
							selected = true;
							break;
						}
					}
				//}
				if (!selected) tree_selection.add_tree_node_selection(leaf);
			}
		}

		// !!!!!!!!! RECURSIVE ALGOROTHIM TO ADD ALL SUB BRANCH TREE ITEMS SHOULD LOOK LIKE THIS !!!!!!!!!!
		//for (ws_tree_class* child_tree_branch : tree_branch->branches) {
		//	select_branch_unlinked_files(child_tree_branch, true);
		//}
	}

	void select_branch_leaf_items(ws_tree_class *tree_branch, afw_file_item_category_type_enum item_category, bool add_to_selection = false) {
		if(!add_to_selection) tree_selection.clear_selection();

		for(ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves){
//printf("display_tree_branch : select_branch_leaf_items : %s  \n", leaf->label.c_str());
//if(leaf->afw_file_item_category_type == afw_file_item_category_type_enum::image)
//printf("display_tree_branch : afw_file_item_category_type_enum::image : %s  \n", leaf->label.c_str());

			if (leaf->afw_file_item_category_type == item_category) { //leaf is of catergory to be selected
				bool selected = false;
				//if (add_to_selection) { //if adding to selection check not already seleced
					for(ws_tree_node_item_struct_type *selected_item : tree_selection.tree_node_selection){
						if (selected_item->item_path == leaf->item_path) {
							selected = true;
							break;
						}
					}
				//}
				if (!selected) tree_selection.add_tree_node_selection(leaf);
			}
		}

		// !!!!!!!!! RECURSIVE ALGOROTHIM TO ADD ALL SUB BRANCH TREE ITEMS SHOULD LOOK LIKE THIS !!!!!!!!!!
		//for (ws_tree_class* child_tree_branch : tree_branch->branches) {
		//	select_branch_leaf_items(child_tree_branch,item_category, true);
		//}
	}

	void deselect_all_branch_items(ws_tree_class *tree_branch) {
		for(ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves){
			tree_selection.remove_tree_node_selection(leaf);
		}

		// !!!!!!!!! RECURSIVE ALGOROTHIM TO ADD ALL SUB BRANCH TREE ITEMS SHOULD LOOK LIKE THIS !!!!!!!!!!
		//for (ws_tree_class* child_tree_branch : tree_branch->branches) {
		//	deselect_all_branch_items(child_tree_branch);
		//}
	}

	void delete_selected_branch_items(ws_tree_class *tree_branch) {
		for (int leafi = tree_branch->leaves.size()-1; leafi > -1; leafi--) {
			ws_tree_node_leaf_struct_type* leaf = tree_branch->leaves[leafi];
			if (leaf->selected) {
				ws_tree.delete_branch_leaf(tree_branch, leaf->tree_node_id, true);
				//ws_tree.delete_branch_leaf(tree_branch, leaf->tree_node_id, false);
				tree_selection.remove_tree_node_selection(leaf);
			}	
		}
	}

	void invert_tree_branch_selection(ws_tree_class *tree_branch) {
		tree_selection_struct_type branch_tree_invert_selection;
		 
		// Create list of unselected items  in branch
		for (ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves) {
			if (!leaf->selected) {
				branch_tree_invert_selection.add_tree_node_selection(leaf);
			}
		}

		// Remove currently selected branch items from selection list
		for (ws_tree_node_leaf_struct_type *leaf : tree_branch->leaves) {
			for (int i = tree_selection.tree_node_selection.size() - 1; i > -1; i--){
				if (leaf->tree_node_id == tree_selection.tree_node_selection[i]->tree_node_id) {
//printf("invert_tree_branch_selection :: %s : %s \n", leaf->item_path.string().c_str(), tree_selection.tree_node_selection[i]->item_path.string().c_str());
					if (leaf->selected) {
//printf("invert_tree_branch_selection AAAA:: %s : %s \n", leaf->item_path.string().c_str());
						tree_selection.remove_tree_node_selection(leaf);
					}
				}
			}
		}

		// Add unselected branch items to selection list
		for (int i = 0;i< branch_tree_invert_selection.tree_node_selection.size(); i++) {
//printf("invert_tree_branch_selection 000 :: %s  \n", branch_tree_invert_selection.tree_node_selection[i]->item_path.string().c_str());
			tree_selection.add_tree_node_selection(branch_tree_invert_selection.tree_node_selection[i]);
//printf("invert_tree_branch_selection 000AAA :: %i  \n", tree_selection.tree_node_selection.size());
		}

//printf("invert_tree_branch_selection 111!!!! :: \n");
//printf("invert_tree_branch_selection 111AAA :: %i  \n", tree_selection.tree_node_selection.size());
//
//		for (int i = 0; i< tree_selection.tree_node_selection.size(); i++) {
//printf("invert_tree_branch_selection 111 :: %s  \n", tree_selection.tree_node_selection[i]->item_path.string().c_str());
//		}

	}

	void add_branch_to_selection(ws_tree_class *tree_branch) {
		if (!tree_selection.is_currently_selected(tree_branch)) tree_selection.add_tree_node_selection(tree_branch);
printf("display_tree_branch : add_branch_to_selection : %s  \n", tree_branch->label.c_str());
	}

	void remove_branch_from_selection(ws_tree_class *tree_branch) {
		if (tree_selection.is_currently_selected(tree_branch)) tree_selection.remove_tree_node_selection(tree_branch);
printf("display_tree_branch : remove_branch_from_selection : %s  \n", tree_branch->label.c_str());
	}

	// ------------------------------------------------------------------------------
	
	void list_all_external_links(ws_tree_node_item_struct_type *leaf_node) {
		ws_tree_node_leaf_struct_type *leaf = static_cast<ws_tree_node_leaf_struct_type*>(leaf_node);
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

//printf("list_all_external_links 000 : %i : %s\n", leaf->external_links_to.size(),leaf->item_path.string().c_str());
//printf("list_all_external_links 000 s\n");

		if(ImGui::Begin("External links",nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)){
			std::string listbox_label = leaf->item_path.filename().string() + " External Links";

			static float scroll_to_off_px = 0.0f;
			static float scroll_to_pos_px = 200.0f;

			if (ImGui::BeginChild(listbox_label.c_str(), ImVec2(750, 200.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_None)) {
				ImGui::SetScrollY(scroll_to_off_px);

				ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, 0.0f);

				for (int item = 0; item < leaf->external_links_to.size(); item++){
					ImGui::Text(leaf->external_links_to[item].string().c_str(), item);
				}

				float scroll_y = ImGui::GetScrollY();
				float scroll_max_y = ImGui::GetScrollMaxY();
				ImGui::EndChild();
			}

			if (ImGui::Button("Exit List External Links", ImVec2(200, 0))) {
				open_external_links_list_popup = false;
			}

			ImGui::End();
		}
	}

	void list_all_internal_from_links(ws_tree_node_item_struct_type *leaf_node) {
		ws_tree_node_leaf_struct_type *leaf = static_cast<ws_tree_node_leaf_struct_type*>(leaf_node);
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

//printf("list_all_internal_from_links 000 : %i : %s\n", leaf->internal_links_from.size(),leaf->item_path.string().c_str());
//printf("list_all_internal_from_links 000 s\n");

//		int k = 0;

		if(ImGui::Begin("From links",nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)){
			//std::string listbox_label = leaf->item_path.filename().string() + " From Links";
			std::string listbox_label = "test From Links";

			static float scroll_to_off_px = 0.0f;
			static float scroll_to_pos_px = 200.0f;

			if (ImGui::BeginChild(listbox_label.c_str(), ImVec2(750, 200.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_None)) {
				ImGui::SetScrollY(scroll_to_off_px);

				ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, 0.0f);

				for (int item = 0; item < leaf->internal_links_from.size(); item++){
					ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_from[item];
					ImGui::Text(tree_node_item->item_path.string().c_str(), item);
				}

				float scroll_y = ImGui::GetScrollY();
				float scroll_max_y = ImGui::GetScrollMaxY();
				ImGui::EndChild();
			}


//			k = k + 1;
//std::cout << "list_all_internal_from_links " <<  k << "\n";
			if (ImGui::Button("Exit List From Links", ImVec2(200, 0))) {
				open_internal_from_links_list_popup = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Add links to selection##iflats", ImVec2(200, 0))) {
				for (int item = 0; item < leaf->internal_links_from.size(); item++) {
					ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_from[item];
					if (!tree_selection.is_currently_selected(tree_node_item)) tree_selection.add_tree_node_selection(tree_node_item);
				}
				//open_internal_from_links_list_popup = false;
			}

			ImGui::End();
		}
	}

	void list_all_internal_to_links(ws_tree_node_item_struct_type *leaf_node) {
		ws_tree_node_leaf_struct_type *leaf = static_cast<ws_tree_node_leaf_struct_type*>(leaf_node);

//printf("list_all_internal_to_links 000 : %i : %s\n", leaf->external_links_to.size(),leaf->item_path.string().c_str());
//printf("list_all_internal_to_links 000 s\n");

		if(ImGui::Begin("to links",nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)){
			std::string listbox_label = leaf->item_path.filename().string() + " to Links";

			static float scroll_to_off_px = 0.0f;
			static float scroll_to_pos_px = 200.0f;

			if (ImGui::BeginChild(listbox_label.c_str(), ImVec2(750, 200.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_None)) {
				ImGui::SetScrollY(scroll_to_off_px);

				ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, 0.0f);

				for (int item = 0; item < leaf->internal_links_to.size(); item++){
					ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_to[item];
					ImGui::Text(tree_node_item->item_path.string().c_str(), item);
				}

				float scroll_y     = ImGui::GetScrollY();
				float scroll_max_y = ImGui::GetScrollMaxY();
				ImGui::EndChild();
			}

			if (ImGui::Button("Exit List to Links", ImVec2(200, 0))) {
				open_internal_to_links_list_popup = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Add links to selection##itlats", ImVec2(200, 0))) {
//printf("Add links to selection 000\n");
				for (int item = 0; item < leaf->internal_links_to.size(); item++) {
					ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_to[item];
//printf("Add links to selection 111 : %s\n", tree_node_item->item_path.string().c_str());
					if (!tree_selection.is_currently_selected(tree_node_item)) tree_selection.add_tree_node_selection(tree_node_item);
				}
				//open_internal_from_links_list_popup = false;
			}

			ImGui::End();
		}
	}

	void list_all_broken_links(ws_tree_node_item_struct_type *leaf_node) {
		ws_tree_node_leaf_struct_type* leaf = static_cast<ws_tree_node_leaf_struct_type*>(leaf_node);
		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

//printf("list_all_broken_links 000 : %i : %s\n", leaf->external_links_to.size(),leaf->item_path.string().c_str());
//printf("list_all_broken_links 000 s\n");

		if (ImGui::Begin("Broken links", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) {
			std::string listbox_label = leaf->item_path.filename().string() + " Broken Links";

			static float scroll_to_off_px = 0.0f;
			static float scroll_to_pos_px = 200.0f;

			if (ImGui::BeginChild(listbox_label.c_str(), ImVec2(750, 200.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_None)) {
				ImGui::SetScrollY(scroll_to_off_px);

				ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, 0.0f);

				for (int item = 0; item < leaf->internal_links_to.size(); item++) {
					ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_to[item];
					if(!std::filesystem::exists(tree_node_item->item_path))
						ImGui::Text(tree_node_item->item_path.string().c_str(), item);
				}

				float scroll_y = ImGui::GetScrollY();
				float scroll_max_y = ImGui::GetScrollMaxY();
				ImGui::EndChild();
			}

			if (ImGui::Button("Exit List of broken Links", ImVec2(200, 0))) {
				open_broken_links_list_popup = false;
			}

			ImGui::End();
		}
	}

	// ------------------------------------------------------------------------------

	void set_image_editor(std::filesystem::path image_editor_path) {
		std::string image_editor_pathname_s = image_editor_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "/");

		image_editor_pathname = image_editor_pathname_s;

printf("set_image_editor 000 :  %s :: %s :: %s\n", image_editor_path.string().c_str(), image_editor_pathname_s.c_str(), image_editor_pathname.string().c_str());

	}

	void set_html_editor(std::filesystem::path html_editor_path) {
		std::string html_editor_pathname_s = html_editor_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(html_editor_pathname_s, "\\", "/");

		html_editor_pathname = html_editor_pathname_s;

printf("set_html_editor 000 :  %s :: %s :: %s\n", html_editor_pathname.string().c_str(), html_editor_pathname_s.c_str(), html_editor_path.string().c_str());

	}

	void set_html_browser(std::filesystem::path browser_path) {
		std::string browser_pathname_s = browser_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(browser_pathname_s, "\\", "/");

		browser_pathname = browser_pathname_s;

printf("set_html_browser 000 :  %s :: %s :: %s\n", browser_pathname.string().c_str(), browser_pathname.c_str(), browser_path.string().c_str());

	}

	void set_text_editor(std::filesystem::path text_editor_path) {
		std::string text_editor_pathname_s = text_editor_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(text_editor_pathname_s, "\\", "/");

		text_editor_pathname = text_editor_pathname_s;

printf("set_image_editor 000 :  %s :: %s :: %s\n", text_editor_pathname.string().c_str(), text_editor_pathname_s.c_str(), text_editor_path.string().c_str());

	}

	void set_media_editor(std::filesystem::path media_editor_path) {
		std::string media_editor_pathname_s = media_editor_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(media_editor_pathname_s, "\\", "/");

		media_editor_pathname = media_editor_pathname_s;

printf("set_image_editor 000 :  %s :: %s :: %s\n", media_editor_pathname.string().c_str(), media_editor_pathname_s.c_str(), media_editor_path.string().c_str());

	}

	void set_audio_editor(std::filesystem::path audio_editor_path) {
		std::string audio_editor_pathname_s = audio_editor_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(audio_editor_pathname_s, "\\", "/");

		audio_editor_pathname = audio_editor_pathname_s;

printf("set_audio_editor 000 :  %s :: %s :: %s\n", audio_editor_pathname.string().c_str(), audio_editor_pathname_s.c_str(), audio_editor_path.string().c_str());

	}

	void set_pdf_editor(std::filesystem::path pdf_editor_path) {
		std::string pdf_editor_pathname_s = pdf_editor_path.string();
		//FW::stringtools::replaceInPlace(image_editor_pathname_s, "\\", "\\\\");
		FW::stringtools::replaceInPlace(pdf_editor_pathname_s, "\\", "/");

		pdf_viewer_pathname = pdf_editor_pathname_s;

printf("set_pdf_editor 000 :  %s :: %s :: %s\n", pdf_viewer_pathname.string().c_str(), pdf_editor_pathname_s.c_str(), pdf_editor_path.string().c_str());

	}

	// ------------------------------------------------------------------------------
	void perform_create_ws_tree_branch(ws_tree_class *parent_branch) {
		if (!parent_branch) return;

		if (ImGui::Begin("Create file Directory", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) {
//printf("rename_tree_branch !!!!!! 111\n");

			float x_pos = 10.0f;
			float y_pos = 40.0f;
			int   name_length = 100;

			ascii_text_input("New Directory Name", new_dir_name_s, x_pos, y_pos, name_length);

			y_pos += 60.0f;

			if (ex_button("Create##ctbcb", x_pos, y_pos, 100.0f, 30.0f)) {
				std::string new_branch_name = new_dir_name_s.c_str();
//std::cout << "perform_create_ws_tree_branch : " << parent_branch->item_path.string() << " : " << new_branch_name << "\n";
				ws_tree.create_ws_tree_branch(parent_branch, new_branch_name);

				perform_create_branch = false;
			}

			if (ex_button("Cancel##ctbnb", x_pos + 250, y_pos, 100.0f, 30.0f)) {
				perform_create_branch = false;
			}
//printf("rename_tree_branch !!!!!! EEEEE\n");
			ImGui::End();
		}

	}


	void rename_tree_branch(ws_tree_class *tree_branch) {
//printf("rename_tree_branch !!!!!! 000\n");
		if (ImGui::Begin("Rename file Directory", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) {
//printf("rename_tree_branch !!!!!! 111\n");

			std::string old_filename_s = tree_branch->item_path.filename().string();

			new_dirname_s.resize(MAX_NUMBER_FILENAME_CHARACTERS); // Need to do this otherwise directory name is concatonated if new dir filename is longer than old

			if (change_dirname) {
				new_dirname_s = old_filename_s;// This needs to not be local so as to ensure it is not re-initialised on each call to this function that ImGui does 
				change_dirname = false;
			}

			float x_pos = 10.0f;
			float y_pos = 40.0f;
			int   name_length = 100;
			if (old_filename_s.length() > name_length) {
				name_length = old_filename_s.length() + 10;
			}

			text("Old Filename : " + old_filename_s, x_pos, y_pos);

			y_pos += 40.0f;

			ascii_text_input("New Directory Name", new_dirname_s, x_pos, y_pos, name_length);
			//y_pos += 20.0f;
			//text("Must\nPress Enter\nto change", x_pos + 300, y_pos);

			y_pos += 60.0f;

			if (ex_button("Rename##rtbrb", x_pos, y_pos, 100.0f, 30.0f)) {
				change_branch_filename(tree_branch, old_filename_s);

				perform_rename_branch = false;
				change_dirname = true;
			}

			if (ex_button("Cancel##rtbcb", x_pos + 250, y_pos, 100.0f, 30.0f)) {
				perform_rename_branch = false;
				change_dirname = true;
			}
//printf("rename_tree_branch !!!!!! EEEEE\n");
			ImGui::End();
		}
	}

	void change_branch_filename(ws_tree_class *branch_node, std::string old_filename_s) {
		std::string old_pathname_s = branch_node->item_path.string();

printf("change_branch_filename 000  Before : %s : %s : %s \n", new_dirname_s.c_str(), branch_node->item_path.string().c_str(), branch_node->label.c_str());

		branch_node->item_path.remove_filename();
		int fns = FW::stringtools::number_characters_in_string(new_dirname_s);
		int pns = FW::stringtools::number_characters_in_string(branch_node->item_path.string());

//printf("change_branch_filename 000AAA  : %i : %i : %i \n", branch_node->item_path.string().length(), new_filename_s.length(), new_filename_s.size());
//printf("change_branch_filename 000BBB  : %i : %i : %i : %i\n", fns, pns, new_filename_s.length(), new_filename_s.size());

		branch_node->item_path.string().resize(pns+fns+1);   // Need to do this otherwise full pathname is concatonated if new dir filename is longer than old. If does not work use above line
		branch_node->item_path /= new_dirname_s;

		//branch_node->label.resize(fns + 1);
		//branch_node->label     = TREE_NODE_PADDING + branch_node->item_path.filename().string;// This is to display file name only 
		branch_node->label.resize(pns + fns + 1);// Need to do this otherwise full pathname is concatonated if new dir filename is longer than old. If does not work use above line
		branch_node->label     = TREE_NODE_PADDING + branch_node->item_path.string();

printf("change_branch_filename 111ZZZ After  : %s  \n", branch_node->item_path.string().c_str());

		std::filesystem::rename(old_pathname_s, branch_node->item_path);// Rename directory that tree node represents; Uncomment when ready to implement

		//Update child branch and leaf pathnames and links
		update_child_branch_and_leaf_pathnames(branch_node, old_filename_s);

		// Update branch leaves links
		update_branch_leaves_pathname(branch_node);
	}

	void update_child_branch_and_leaf_pathnames(ws_tree_class *branch_node, std::string old_filename_s) {//Needs more testing
		std::string parent_path = branch_node->item_path.string().c_str(); // Critical : when ever assigning a std::filesystem::path to a string variable, 
																		   // need to assign as string().c_str() or the string variable will not be able to
																		   // be operated on prperly or as expected !!!!!!!!!
																		   // Wasted hours before finding this 

printf("update_child_branch_and_leaf_pathnames 000 : %s \n", parent_path.c_str());

		for (int b=0; b < branch_node->branches.size(); b++) {
			ws_tree_class *child_branch = branch_node->branches[b];
			std::string child_branch_filename_s = child_branch->item_path.filename().string().c_str();

printf("update_child_branch_and_leaf_pathnames 111 BEFORE : %s  : %s\n", child_branch->item_path.string().c_str(), old_filename_s.c_str());

			int prev_path_size = FW::stringtools::number_characters_in_string(child_branch->item_path.string());
			child_branch->previous_item_path.string().resize(prev_path_size);
			child_branch->previous_item_path = child_branch->item_path.string().c_str();
printf("update_child_branch_and_leaf_pathnames AAA : %s :%s \n", child_branch->previous_item_path.string().c_str(), child_branch_filename_s.c_str());

			int dir_name_size    = FW::stringtools::number_characters_in_string(child_branch_filename_s);
			int parent_path_size = FW::stringtools::number_characters_in_string(branch_node->item_path.string());

			child_branch->item_path.clear();
			child_branch->item_path.string().resize(parent_path_size + dir_name_size +1);

			std::string s = parent_path + "/" + child_branch_filename_s;// parent_path and child_branch_filename_s must be assigned as .c_srt() 
																		// when converting from filesystem::path or string functions or assignments like
																		// this one will not work as expected or at all. !!!!!!!

printf("update_child_branch_and_leaf_pathnames 222!!!!!!: %s \n", s.c_str());

			child_branch->item_path.assign(s);

			//child_branch->label.resize(fns + 1);
			//child_branch->label     = TREE_NODE_PADDING + branch_node->item_path.filename().string;// This is to display file name only 
			child_branch->label.resize(parent_path_size + dir_name_size + 1);// Need to do this otherwise full pathname is concatonated if new dir filename is longer than old. If does not work use above line
			child_branch->label = TREE_NODE_PADDING + child_branch->item_path.string();

printf("update_child_branch_and_leaf_pathnames 222 AFTER : %s : %s : %s :%i : %i : %i\n", child_branch->previous_item_path.string().c_str(), child_branch_filename_s.c_str(), child_branch->item_path.string().c_str(), parent_path_size, dir_name_size, child_branch->item_path.string().size());
printf("update_child_branch_and_leaf_pathnames 222A AFTER : %s \n", s.c_str());

			update_branch_leaves_pathname(child_branch);

			update_child_branch_and_leaf_pathnames(child_branch, old_filename_s);
		}
	}

	void update_branch_leaves_pathname(ws_tree_class *branch_node) {
		std::string parent_path = branch_node->item_path.string().c_str(); // Critical : when ever assigning a std::filesystem::path to a string variable, 
																		   // need to assign as string().c_str() or the string variable will not be able to
																		   // be operated on prperly or as expected !!!!!!!!!
																		   // Wasted hours before finding this 
printf("update_branch_leaves_pathname  BL000 : %s \n", parent_path.c_str());


		for (int i = 0; i < branch_node->leaves.size(); i++) {
			ws_tree_node_item_struct_type* leaf_node = branch_node->leaves[i];
			std::string leaf_filename_s = leaf_node->item_path.filename().string().c_str();

printf("update_branch_leaves_pathname BL111 BEFORE : %s \n", leaf_node->item_path.string().c_str());

			int leaf_filename_size = FW::stringtools::number_characters_in_string(leaf_filename_s);
			int parent_path_size   = FW::stringtools::number_characters_in_string(parent_path);

			leaf_node->previous_item_path = leaf_node->item_path;

			leaf_node->item_path.clear();
			leaf_node->item_path.string().resize(parent_path_size + leaf_filename_size +1);

			leaf_node->item_path = std::string(parent_path + "/" + leaf_filename_s);// parent_path and child_branch_filename_s must be assigned as .c_srt() 
																					// when converting from filesystem::path or string functions or assignments like
																					// this one will not work as expected or at all. !!!!!!!

printf("update_branch_leaves_pathname BL222 AFTER : %s : %s : %s\n", leaf_node->label.c_str(), leaf_node->previous_item_path.string().c_str(), leaf_node->item_path.string().c_str());

			ws_tree_node_leaf_struct_type *leaf= static_cast<ws_tree_node_leaf_struct_type *>(leaf_node);

			for (int item = 0; item < leaf->internal_links_from.size(); item++){
				ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_from[item];
				ws_tree_class *root_node =  ws_tree.get_tree_root_node(leaf_node->node_item_tree_branch);
				std::filesystem::path root_pathname = root_node->item_path;

printf("rename_branch_leaf_filename 222  : %s : %s : %s \n", root_pathname.string().c_str(), tree_node_item->item_path.string().c_str(), leaf_node->previous_item_path.string().c_str());

				ws_html_manager.update_web_page_file_link(root_pathname, tree_node_item->item_path, leaf_node->previous_item_path, leaf_node->item_path);// not yet tested; Uncomment when ready to implement
				//ws_html_manager.update_web_page_file_link(root_pathname, tree_node_item->item_path, old_filename_s, new_item_path_s);// not yet tested; Uncomment when ready to implement
			}
		}
	}

	void rename_branch_leaf_filename(ws_tree_node_item_struct_type *leaf_node) {
		ws_tree_node_leaf_struct_type *leaf = static_cast<ws_tree_node_leaf_struct_type*>(leaf_node);

//printf("rename_branch_leaf_filename 000 : %i : %s\n", leaf->external_links_to.size(),leaf->item_path.string().c_str());

		if(ImGui::Begin("Rename Leaf filename##rbl",nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)){
			std::string old_filename_s = leaf->item_path.filename().string();

			new_filename_s.resize(MAX_NUMBER_FILENAME_CHARACTERS); // Need to do this otherwise directory name is concatonated if new dir filename is longer than old

			if (change_filename) {
				new_filename_s = old_filename_s;// This needs to not be local so as to ensure it is not re-initialised on each call to this function that ImGui does 
				change_filename = false;
			}

			float x_pos = 10.0f;
			float y_pos = 40.0f;
			int   name_length = 100;
			if (old_filename_s.length() > name_length) {
				name_length = old_filename_s.length() + 10;
			}

			text("Old Filename : " + old_filename_s, x_pos, y_pos);
			
			y_pos += 40.0f;

			ascii_text_input("New Filename", new_filename_s, x_pos,y_pos, name_length);
			//y_pos += 20.0f;
			//text("Press Enter\nto change", x_pos+300, y_pos);

			y_pos += 60.0f;

			if (ex_button("Rename file##rblrb", x_pos, y_pos,100.0f, 30.0f)) {
				change_branch_leaf_filename(leaf_node, leaf);

				open_rename_branch_leaf_popup = false;
				change_filename = true;
			}

			if (ex_button("Cancel##rblcb", x_pos + 250, y_pos,100.0f, 30.0f)) {
				open_rename_branch_leaf_popup = false;
				change_filename = true;
			}

			ImGui::End();
		}
	}

	void change_branch_leaf_filename(ws_tree_node_item_struct_type *leaf_node,ws_tree_node_leaf_struct_type *leaf) {//Needs more testing
		//std::string old_pathname_s = leaf->item_path.string();
		std::filesystem::path old_pathname = leaf->item_path;

printf("rename_branch_leaf_filename 000  Before : %s : %s : %s\n", new_filename_s.c_str(), leaf->item_path.string().c_str(), leaf_node->label.c_str());

		leaf_node->item_path.remove_filename();
		int fns = FW::stringtools::number_characters_in_string(new_filename_s);
		int pns = FW::stringtools::number_characters_in_string(leaf_node->item_path.string());

		leaf_node->item_path.string().resize(pns + fns + 1);   // Need to do this otherwise full pathname is concatonated if new dir filename is longer than old. If does not work use above line
		leaf_node->item_path /= new_filename_s;

		leaf_node->label     = TREE_NODE_PADDING + new_filename_s;

		//std::filesystem::rename(old_pathname_s, leaf_node->item_path);// Rename file that tree leaf represents; Uncomment when ready to implement
		std::filesystem::rename(old_pathname, leaf_node->item_path);// Rename file that tree leaf represents; Uncomment when ready to implement

printf("rename_branch_leaf_filename 111 After : %s : %s : %s\n", leaf->item_path.string().c_str(), leaf_node->label.c_str());

		for (int item = 0; item < leaf->internal_links_from.size(); item++){
			ws_tree_node_item_struct_type *tree_node_item = leaf->internal_links_from[item];
			ws_tree_class *root_node =  ws_tree.get_tree_root_node(leaf_node->node_item_tree_branch);
			std::filesystem::path root_pathname = root_node->item_path;

printf("rename_branch_leaf_filename 222  : %s : %s : %s \n", root_pathname.string().c_str(), tree_node_item->item_path.string().c_str(), old_pathname.string().c_str());

			ws_html_manager.update_web_page_file_link(root_pathname, tree_node_item->item_path, old_pathname, leaf_node->item_path);// not yet tested; Uncomment when ready to implement
			//ws_html_manager.update_web_page_file_link(root_pathname, tree_node_item->item_path, old_filename_s, new_item_path_s);// not yet tested; Uncomment when ready to implement
		}
	}


	//int number_characters_in_string(std::string string) {
	//	std::vector<char> char_vector = FW::stringtools::toCharArray(string);
	//	
	//	for (int i = 0; i < char_vector.size(); i++) {
	//		if (char_vector[i] == '\0') {
	//			return i;
	//			break;
	//		}
	//	}
	//	
	//	return char_vector.size();
	//}

protected:
	//if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
	//{
	//}

	// Select tree branch by selecting tree brach label and not by clicking on the tree open close arrow button.
	// If this is not inplemented then tree branch will be selected when opening/closing tree brach wich is not desired.

	bool selected_tree_branch_label() {
		if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
			return true;
		else
			return false;
	}

	void open_item_application(tree_selection_struct_type tree_selection) {
		if (tree_selection.tree_node_selection.size() < 1) return;

		ws_tree_node_item_struct_type *tree_node_leaf = tree_selection.tree_node_selection[0];

		std::string extension = tree_node_leaf->item_path.extension().string();

printf("open_item_application 000 :  %s \n", tree_node_leaf->item_path.string().c_str());

		afw_file_item_category_type_enum item_category_type = get_item_category_type(extension);

		switch (item_category_type) {
			case afw_file_item_category_type_enum::image : open_application_editor_viewer(image_editor_pathname,tree_node_leaf->item_path); break;
			//case afw_file_item_category_type_enum::page  : open_application_editor_viewer(html_editor_pathname,tree_node_leaf->item_path); break;
			case afw_file_item_category_type_enum::page  : open_html_browser_viewer(html_editor_pathname,tree_node_leaf->item_path); break;
			case afw_file_item_category_type_enum::pageformat: open_application_editor_viewer(text_editor_pathname,tree_node_leaf->item_path); break;
			case afw_file_item_category_type_enum::media : open_application_editor_viewer(media_editor_pathname,tree_node_leaf->item_path); break;
			case afw_file_item_category_type_enum::audio : open_application_editor_viewer(audio_editor_pathname,tree_node_leaf->item_path); break;
			case afw_file_item_category_type_enum::pdf   : open_application_editor_viewer(pdf_viewer_pathname,tree_node_leaf->item_path); break;
		}
	}

	afw_file_item_category_type_enum get_item_category_type(std::string extension) {
		return ws_tree.get_item_category_type(extension);
	}

	void open_application_editor_viewer(std::filesystem::path editor_pathname, std::filesystem::path file_pathname) {
		// This works so need to folow this form of defining the string to be used in the std::system(); command
		//std::string system_command = "\"C:\\Program Files\\XnViewMP\\xnviewmp.exe \" F:\\Web_downloads\\ESA\\Unfinished\\Arcadia_Planitia\\main_files\\Media_card_medium.jpg";

		// "\"" defines string to have a quote in it to pass to the windows command as windows allows spaces to be used in path and file names
		// but when encountered in a command line that space is assumed to be a deliminator between command and parameter definitions. Another stupid windows feature
		std::string system_command = "\"" + editor_pathname.string() + " \" " + file_pathname.string();// This works
												 // This space is critical ^ or sting command will generate file not found error !!!!!

printf("open_image_editor 111 :  %s \n", system_command.c_str());
		std::system(system_command.c_str());
	}

	void open_html_browser_viewer(std::filesystem::path html_editor_pathname, std::filesystem::path file_pathname) {
		std::string browser_pathname_s = FW::stringtools::toLowerCase(html_editor_pathname.string());
		std::string system_command;
		//FW::stringtools::replaceInPlace(s, "\\", "/");

		if (FW::stringtools::contains(browser_pathname_s, "firefox") || FW::stringtools::contains(browser_pathname_s, "bluegriffon")) {
			system_command = "\"" + html_editor_pathname.string() + " \" file:\\" + file_pathname.string();// This works
											  // This space is critical ^ or sting command will generate file not found error !!!!!
		} else{ // Not sure if other browsers or html editors would have specific line command structure to open html in browaser as firefox does
			system_command = "\"" + html_editor_pathname.string() + " \" " + file_pathname.string();// This works
											  // This space is critical ^ or sting command will generate file not found error !!!!!		
		}

printf("open_image_editor 111 :  %s \n", system_command.c_str());
		std::system(system_command.c_str());
	}

private:


	int selected = -1;

	//tree_selection_struct_type    tree_selection = {};
	ws_tree_node_item_struct_type drop_selection;

	bool drag_drop_source_file = false;// Flag to indicate if tree item selecected to drag and drop is a file.
	bool drag_drop_source_dir = false;// Flag to indicate if tree item selecected to drag and drop is a file.

	ImGuiTreeNodeFlags branch_base_node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanLabelWidth;
	ImGuiTreeNodeFlags leaf_base_node_flags   = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanLabelWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	bool open_external_links_list_popup		 = false;
	bool open_internal_from_links_list_popup = false;
	bool open_internal_to_links_list_popup   = false;
	bool open_broken_links_list_popup		 = false;
	bool open_rename_branch_leaf_popup		 = false;

	bool perform_selection_move_and_update   = false;
	bool perform_rename_branch				 = false;
	bool perform_refresh_branch				 = false;
	bool perform_create_branch				 = false;
	bool perform_delete_branch				 = false;

	bool exit_create_root_tree = false;

	int selected_menu_branch_index = -1;
	int selected_menu_item_index   = -1;

	std::filesystem::path selected_dir_pathname = "";
	
	ws_html_manager_class ws_html_manager;

	std::string new_filename_s = "";
	std::string new_dirname_s = "";


	std::string new_dir_name_s = "directory";

	bool change_filename = true;
	bool change_dirname  = true;

	ws_tree_node_item_struct_type *action_tree_node = nullptr;
};