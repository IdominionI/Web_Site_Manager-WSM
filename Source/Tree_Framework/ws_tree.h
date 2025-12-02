#pragma once

#include <filesystem>
#include <sys/stat.h>

#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>
#include <FrameWork/AFW/Tools/afw_string_tools.h>

#include <FrameWork/GUI/afw_tree_node.h>

#include "Source/File_manager/html_manager.h"
#include "Source/File_manager/file_path_tools.h"

#define TREE_NODE_PADDING " "
#define INVALID_PATH	  ""

#define MAX_NUMBER_FILENAME_CHARACTERS 200
#define MAX_NUMBER_PATHNAME_CHARACTERS 500

class ws_tree_class;// Forward declaration
struct ws_tree_node_item_struct_type : public afw_tree_node_item_base_struct_type {
	std::filesystem::path item_path;
	std::filesystem::path previous_item_path = "";

	ws_tree_class *node_item_tree_branch = nullptr;

	afw_file_item_category_type_enum afw_file_item_category_type;

	int tree_item_type;
};

struct ws_tree_node_leaf_struct_type : public ws_tree_node_item_struct_type {
	std::vector <ws_tree_node_item_struct_type *> internal_links_from = {};
	std::vector <ws_tree_node_item_struct_type *> internal_links_to   = {};
	std::vector<std::filesystem::path>			  external_links_to   = {};

	bool update_link_path = false;
};

//enum class tree_leaves_sort_enum {label, size, type, date};

id_key_manager_class<unsigned int> tree_node_id_key;// Must have data type unsigned int and defined outside of tree class otherwise node id not assigned correctly 


struct tree_selection_struct_type {
	std::vector<ws_tree_node_item_struct_type*> tree_node_selection = {};

	void add_tree_node_selection(ws_tree_node_item_struct_type *tree_node) {
		if (is_currently_selected(tree_node)) return;
		if (is_within_a_currently_selected_directory(tree_node)) {
			tree_node->selected = false;
			return;
		}

		if (tree_node->tree_node_item_type == afw_tree_node_item_type_enum::tree_node) {
			remove_current_child_selections(tree_node);
		}

		tree_node->selected = true;
		tree_node_selection.push_back(tree_node);
	}

	void remove_tree_node_selection(ws_tree_node_item_struct_type *tree_node_to_remove) {
		for (int i = 0; i < tree_node_selection.size(); i++) {
			if (tree_node_to_remove->tree_node_id == tree_node_selection[i]->tree_node_id) {
				tree_node_to_remove->selected = false;
				tree_node_selection.erase(tree_node_selection.begin() + i);
				break;
			}
		}
	}

	void clear_selection() {
		for (ws_tree_node_item_struct_type *tree_node : tree_node_selection) {
			tree_node->selected = false;
		}
		tree_node_selection.clear();
	}

	bool is_currently_selected(ws_tree_node_item_struct_type *tree_node) {
		for (ws_tree_node_item_struct_type* selected_tree_node : tree_node_selection) {
//printf("is_currently_selected AAA : %s : %s \n", tree_node->item_path.string().c_str(), selected_tree_node->item_path.string().c_str());
			if (selected_tree_node->tree_node_id == tree_node->tree_node_id) {
//printf("is_currently_selected BBB : %i : %i \n", tree_node->tree_node_id, selected_tree_node->tree_node_id);
				return true;
				break;
			}
		}

		return false;
	}

	bool is_within_a_currently_selected_directory(ws_tree_node_item_struct_type* tree_node) {
		bool within_a_currently_selected_directory = false;

		for (ws_tree_node_item_struct_type *selected_tree_node : tree_node_selection) {
			std::string tree_node_item_path_s		   = tree_node->item_path.string();
			std::string selected_tree_node_item_path_s = selected_tree_node->item_path.string();

			FW::stringtools::replaceInPlace(tree_node_item_path_s, "\\", "/");
			FW::stringtools::replaceInPlace(selected_tree_node_item_path_s, "\\", "/");

//printf("tree_selection_struct_type : is_within_a_currently_selected_directory 00 : %s : %s \n", tree_node_item_path_s.c_str(), selected_tree_node_item_path_s.c_str());

			if (tree_node_item_path_s.find(selected_tree_node_item_path_s) != std::string::npos) {
//printf("tree_selection_struct_type : is_within_a_currently_selected_directory 01 : %s in direcory %s \n", tree_node_item_path_s.c_str(), selected_tree_node_item_path_s.c_str());
				within_a_currently_selected_directory = true;
				break;
			}
		}

//printf("tree_selection_struct_type : is_within_a_currently_selected_directory 02 not found in directory %s\n", tree_node->item_path.string().c_str());

		return within_a_currently_selected_directory;
	}

	void remove_current_child_selections(ws_tree_node_item_struct_type *tree_node) {
		if (tree_node->tree_node_item_type != afw_tree_node_item_type_enum::tree_node) {
			return;
		}

		std::vector<ws_tree_node_item_struct_type*> tree_nodes_to_remove = {};

		bool within_a_currently_selected_directory = false;

		for (size_t i = 0; i < tree_node_selection.size();i++) {
			ws_tree_node_item_struct_type *selected_tree_node = tree_node_selection[i];
			std::string tree_node_item_path_s				  = tree_node->item_path.string();
			std::string selected_tree_node_item_path_s		  = selected_tree_node->item_path.string();

			FW::stringtools::replaceInPlace(tree_node_item_path_s, "\\", "/");
			FW::stringtools::replaceInPlace(selected_tree_node_item_path_s, "\\", "/");

//printf("remove_current_child_selections :  00 : %s in %s :: %i :: %i\n", tree_node_item_path_s.c_str(), selected_tree_node_item_path_s.c_str(),i, tree_node_selection.size());

			if (selected_tree_node_item_path_s.find(tree_node_item_path_s) != std::string::npos) {
//printf("remove_current_child_selections : 01 : %s in selection %s \n", tree_node_item_path_s.c_str(), selected_tree_node_item_path_s.c_str());
				selected_tree_node->selected = false;
				tree_nodes_to_remove.push_back(selected_tree_node);
			}
		}

		for (size_t i = 0; i < tree_nodes_to_remove.size(); i++) {
			remove_tree_node_selection(tree_nodes_to_remove[i]);
		}

		tree_nodes_to_remove.clear();
	}
};

class ws_tree_class :public  ws_tree_node_item_struct_type {
public:
	ws_tree_class() {}
	~ws_tree_class() { delete_ws_tree(false); }

	//std::string window_label = "Window Label";

	std::filesystem::path parent_path = INVALID_PATH;

	ws_tree_class* parent_branch = nullptr;

	std::vector<ws_tree_node_leaf_struct_type*> leaves = {};
	std::vector<ws_tree_class*>					 branches = {};

	std::vector<std::string> image_extensions = { "jpg" ,"jpeg","gif","tif","png","webp","svg","bmp" };
	std::vector<std::string> html_extensions = { "htm" ,"html", "xhtm", "dhtml","php" };
	std::vector<std::string> text_extensions = { "asc","txt","csv" };
	std::vector<std::string> script_extensions = { "css" ,"js"};
	std::vector<std::string> media_extensions = { "mpeg","avi","webm","mp4","flv","swf"};
	std::vector<std::string> audio_extensions = { "weba","wav" };
	std::vector<std::string> pdf_extensions = { "pdf" };

	bool create_root_tree(std::string root_label, std::filesystem::path root_path) {
		if (label.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_root_tree :: Tree root label not defined");
			return false;
		}
		if (!std::filesystem::exists(root_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_root_tree :: Tree root path not defined");
			return false;
		}

		parent_path = INVALID_PATH; // to make sure root has no parent
		parent_branch = nullptr;	  // to make sure root has no parent

		label = TREE_NODE_PADDING + root_label;
		item_path = root_path;
		icon_id = ICON_FA_FOLDER;
		tree_node_item_type = afw_tree_node_item_type_enum::root_tree_node;
		tree_node_id = tree_node_id_key.get_available_id_key();
		node_item_tree_branch = this;

		//printf("create_root_tree tree_node_id %s : %i \n", tree_node.item_path.string().c_str(),tree_node.tree_node_id);
		return create_ws_contents(this);
	}

	ws_tree_class *create_tree_branch(ws_tree_class* parent_node, std::string label, std::filesystem::path branch_path, bool must_have_existing_dir = true) {
		ws_tree_class *tree_branch = new ws_tree_class;

		if (!tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_tree_branch :: Unable to create new tree branch");
			return nullptr;
		}
		if (label.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_tree_branch :: Tree branch label not defined");
			return nullptr;
		}
		if (!parent_node) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_tree_branch ::  Tree branch parent not defined");
			return nullptr;
		}

		if (must_have_existing_dir && !std::filesystem::exists(branch_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_tree_branch ::  Tree branch pathname does not exist");
			return nullptr;
		}

		parent_path = parent_node->item_path; // to make sure root has no parent

		tree_branch->label = TREE_NODE_PADDING + label;
		tree_branch->item_path = branch_path;
		tree_branch->icon_id = ICON_FA_FOLDER;
		tree_branch->tree_node_item_type = afw_tree_node_item_type_enum::tree_node;
		tree_branch->tree_node_id = tree_node_id_key.get_available_id_key();

		tree_branch->node_item_tree_branch = tree_branch;
		tree_branch->parent_branch = parent_node;
//printf("create_tree_branch tree_node_id %s : %i \n", tree_branch->item_path.string().c_str(), tree_branch->tree_node_id);

		return tree_branch;
	}

	bool create_ws_contents(ws_tree_class* parent_tree_node) {
		// This structure would distinguish a file from a
		// directory
		struct stat sb;

//printf("get_dir_contents : %s", path.c_str());

		// Looping until all the items of the directory are
		// exhausted
		for (const auto& entry : std::filesystem::directory_iterator(item_path)) {

			// Converting the path to const char * in the
			// subsequent lines
			std::filesystem::path outfilename = entry.path();
			std::string outfilename_str = outfilename.string();
			const char* path = outfilename_str.c_str();

			// Test whether the path points to a non-directory or not 
			if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) { // have a data file
//std::cout << path << std::endl;
				ws_tree_node_leaf_struct_type* ws_tree_node_item = new ws_tree_node_leaf_struct_type;

				ws_tree_node_item->label = TREE_NODE_PADDING + outfilename.filename().string();
				ws_tree_node_item->item_path = path;

				//ws_tree_node_item->icon_id = ICON_FA_FILE;
				ws_tree_node_item->icon_id = get_icon_id(outfilename.extension().string().c_str());
				ws_tree_node_item->tree_node_item_type = afw_tree_node_item_type_enum::tree_leaf;
				ws_tree_node_item->tree_node_id = tree_node_id_key.get_available_id_key();

				ws_tree_node_item->node_item_tree_branch = parent_tree_node->node_item_tree_branch;// +++++++

				std::string extension = ws_tree_node_item->item_path.extension().string();

//printf("create_ws_contents 000 :  %s \n", ws_tree_node_item->item_path.string().c_str());

				ws_tree_node_item->afw_file_item_category_type = get_item_category_type(extension);

//printf("create_ws_contents tree_node_id %s : %i \n", ws_tree_node_item.item_path.string().c_str(), ws_tree_node_item.tree_node_id);

				leaves.push_back(ws_tree_node_item);
			}
			else { // have a directory
				ws_tree_class* tree_branch = create_tree_branch(this, outfilename.string(), path);

				if (!tree_branch) return false;

				branches.push_back(tree_branch);

				tree_branch->create_ws_contents(tree_branch);
			}
		}
		return true;
	}

	void sort_dir_contents() {

	}

	void perform_selection_move(ws_tree_class* child_tree_branch, tree_selection_struct_type tree_selection) {
		tree_selection_struct_type  old_tree_selection = tree_selection;

printf("perform_selection_move 000 : %s :: %i\n", child_tree_branch->item_path.string().c_str(), tree_selection.tree_node_selection.size());

		for (int i = 0; i < tree_selection.tree_node_selection.size(); i++) {

			if (perform_move_task(static_cast<ws_tree_node_leaf_struct_type*>(tree_selection.tree_node_selection[i]), child_tree_branch)) { // Can only drag and drop a source file or directory into a target directory

				if (tree_selection.tree_node_selection[i]->tree_node_item_type == afw_tree_node_item_type_enum::tree_leaf) {
					ws_tree_node_leaf_struct_type* tree_node_leaf = static_cast<ws_tree_node_leaf_struct_type*>(tree_selection.tree_node_selection[i]);
					if (is_web_page(tree_selection.tree_node_selection[i]->item_path)) { // Have a selection tree leaf node that is a web page
printf("perform_selection_move 002 : %s ::\n", old_tree_selection.tree_node_selection[i]->item_path.string().c_str());
						web_page_update_to_links(tree_node_leaf, child_tree_branch);//UNCOMMENT WHEN TESTING OMPLETE
						web_page_update_from_links(tree_node_leaf, child_tree_branch);//UNCOMMENT WHEN TESTING OMPLETE :: NOTE : NOT YET TESTED !!!!
printf("perform_selection_move 003 : %s \n", tree_selection.tree_node_selection[i]->item_path.string().c_str());
					}
					else { // Have a selection tree leaf node that is not a web page
printf("perform_selection_move 004 : %s \n", tree_selection.tree_node_selection[i]->item_path.string().c_str());
						update_web_page_to_file_links(tree_node_leaf, child_tree_branch);
					}
					tree_node_leaf->previous_item_path.clear();// To avoid incorrect previous path value NEED TO BE SURE THIS IS NEEDED !!!!!!!
				}
				else {
					if (tree_selection.tree_node_selection[i]->tree_node_item_type == afw_tree_node_item_type_enum::tree_node) { // Have a selection tree node that is a tree branch or file system directory
						ws_tree_class* tree_branch = static_cast<ws_tree_class*>(tree_selection.tree_node_selection[i]);
						update_web_site_directory_links(tree_branch, tree_branch);
					}
				}
			}
		}
	}

	bool web_page_update_to_links(ws_tree_node_leaf_struct_type* tree_node_leaf, ws_tree_class* child_tree_branch) {
		if (!tree_node_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "web_page_update_to_links ::tree_node_leaf undefined. \n");
			return false;
		}

		if (!child_tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "web_page_update_to_links ::child_tree_branch undefined. \n");
			return false;
		}

		if (tree_node_leaf->previous_item_path.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "web_page_update_to_links ::tree_node_leaf->previous_item_path undefined. \n");
			return false;
		}

		std::filesystem::path current_web_page_file_pathname = tree_node_leaf->item_path;
		std::filesystem::path previous_web_page_file_pathname = tree_node_leaf->previous_item_path;
		ws_tree_class* root_node = get_tree_root_node(child_tree_branch);

printf("web_page_update_to_links 00 : %s :: %s \n", current_web_page_file_pathname.string().c_str(), previous_web_page_file_pathname.string().c_str());

		if (!ws_html_manager.define_html_buffer(current_web_page_file_pathname)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "web_page_update_to_links : Cannot define web page file\n" + current_web_page_file_pathname.string() + "\nto update\n");
			return false;
		}

		//for (ws_tree_node_leaf_struct_type *tree_node_leaf_link_to : static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_to)) {// This does not work
		for (size_t i = 0; i < tree_node_leaf->internal_links_to.size(); i++) {
			ws_tree_node_leaf_struct_type* tree_node_leaf_link_to = static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_to[i]);

printf("web_page_update_to_links 02A : %s :: %s \n", previous_web_page_file_pathname.string().c_str(), current_web_page_file_pathname.string().c_str());
printf("web_page_update_to_links 02B : %s :: %s \n", root_node->item_path.string().c_str(), tree_node_leaf_link_to->item_path.string().c_str());

			ws_html_manager.update_html_file_links(previous_web_page_file_pathname, current_web_page_file_pathname, root_node->item_path, tree_node_leaf_link_to->item_path);
		}

		ws_html_manager.save_html_file_updates(current_web_page_file_pathname); // Uncomment when testing completed

		//tree_node_leaf->previous_item_path.clear();// To avoid incorrect previous path value NEED TO BE SURE THIS IS NEEDED !!!!!!!

		return true;
	}

	bool web_page_update_from_links(ws_tree_node_leaf_struct_type* tree_node_leaf, ws_tree_class* child_tree_branch) {
		if (!tree_node_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "web_page_update_from_links ::tree_node_leaf undefined. \n");
			return false;
		}

		if (!child_tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "web_page_update_from_links ::child_tree_branch undefined. \n");
			return false;
		}

		if (tree_node_leaf->previous_item_path.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "web_page_update_from_links ::tree_node_leaf->previous_item_path undefined. \n");
			return false;
		}

		std::filesystem::path current_web_page_file_pathname = tree_node_leaf->item_path;
		std::filesystem::path previous_web_page_file_pathname = tree_node_leaf->previous_item_path;
		ws_tree_class* root_node = get_tree_root_node(child_tree_branch);

printf("update_web_page_to_file_links 00 : %s :: %s \n", current_web_page_file_pathname.string().c_str(), previous_web_page_file_pathname.string().c_str());

		//for (ws_tree_node_leaf_struct_type *tree_node_leaf_link_from : static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_from)) {// This does not work
		for (size_t i = 0; i < tree_node_leaf->internal_links_from.size(); i++) {
			ws_tree_node_leaf_struct_type* tree_node_leaf_link_from = static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_from[i]);

			// Get web page files that references old_file_pathname and needs to have this reference updated to new_file_pathname
printf("update_web_page_to_file_links 05A  : %s : %s : %s\n", tree_node_leaf_link_from->item_path.string().c_str(), previous_web_page_file_pathname.string().c_str(), current_web_page_file_pathname.string().c_str());

			ws_html_manager.update_html_file_from_links(previous_web_page_file_pathname, current_web_page_file_pathname, root_node->item_path, tree_node_leaf_link_from->item_path);
		}

	}

	bool update_web_page_to_file_links(ws_tree_node_leaf_struct_type* tree_node_leaf, ws_tree_class* child_tree_branch) {
		if (!tree_node_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_web_page_to_file_links ::tree_node_leaf undefined. \n");
			return false;
		}

		if (!child_tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_web_page_to_file_links ::child_tree_branch undefined. \n");
			return false;
		}

		if (tree_node_leaf->previous_item_path.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_web_page_to_file_links ::tree_node_leaf->previous_item_path undefined. \n");
			return false;
		}

		std::filesystem::path current_file_pathname = tree_node_leaf->item_path;
		std::filesystem::path previous_file_pathname = tree_node_leaf->previous_item_path;
		ws_tree_class* root_node = get_tree_root_node(child_tree_branch);

		printf("update_web_page_to_file_links 00 : %s :: %s \n", current_file_pathname.string().c_str(), previous_file_pathname.string().c_str());

		for (ws_tree_node_item_struct_type* internal_from_link : tree_node_leaf->internal_links_from) {
			// Get web page files that references old_file_pathname and needs to have this reference updated to new_file_pathname
			printf("update_web_page_to_file_links 05A  : %s : %s : %s\n", internal_from_link->item_path.string().c_str(), previous_file_pathname.string().c_str(), current_file_pathname.string().c_str());
			ws_html_manager.update_web_page_file_link(root_node->item_path, internal_from_link->item_path, previous_file_pathname, current_file_pathname);
		}

		//tree_node_leaf->previous_item_path.clear();// To avoid incorrect previous path value NEED TO BE SURE THIS IS NEEDED !!!!!!!

		return true;
	}

	bool update_web_site_directory_links(ws_tree_class* tree_branch, ws_tree_class* home_tree_branch) {
		//printf("update_web_site_directory_links file   : tree_branch %s  \n", tree_branch->item_path.string().c_str());

		for (ws_tree_node_item_struct_type* child_tree_leaf : tree_branch->leaves) {

			if (is_web_page(child_tree_leaf)) { // Have a selection tree leaf node that is a web page
printf("update_web_site_directory_links_002 : %s ::\n", child_tree_leaf->item_path.string().c_str());
				update_branch_web_page_to_links(child_tree_leaf, home_tree_branch);//UNCOMMENT WHEN TESTING OMPLETE:: NOTE : NOT YET TESTED !!!!
				update_branch_web_page_from_links(child_tree_leaf, home_tree_branch);//UNCOMMENT WHEN TESTING OMPLETE :: NOTE : NOT YET TESTED !!!!
printf("update_web_site_directory_links_003 : %s \n", child_tree_leaf->item_path.string().c_str());
			}
			else { // Have a selection tree leaf node that is not a web page
printf("update_web_site_directory_links 004 : %s \n", child_tree_leaf->item_path.string().c_str());
				update_branch_web_page_to_file_links(child_tree_leaf, home_tree_branch); //UNCOMMENT WHEN TESTING OMPLETE :: NOTE : NOT YET TESTED !!!!
			}

			//printf("update_web_site_directory_links_05A file   : source %s : destination %s \n", child_tree_leaf->item_path.string().c_str(), tree_branch->item_path.string().c_str());
			//printf("update_web_site_directory_links_05B branch : source %s : destination %s \n", tree_branch->node_item_tree_branch->item_path.string().c_str(), home_tree_branch->node_item_tree_branch->item_path.string().c_str());

		}

		for (ws_tree_class* child_tree_branch : tree_branch->branches) {
			update_web_site_directory_links(child_tree_branch, home_tree_branch);
		}

		for (ws_tree_node_item_struct_type* child_tree_leaf : tree_branch->leaves) {
			child_tree_leaf->previous_item_path.clear();// To avoid incorrect previous path value NEED TO BE SURE THIS IS NEEDED !!!!!!!
		}

		return true;
	}

	bool update_branch_web_page_to_links(ws_tree_node_item_struct_type* child_tree_leaf, ws_tree_class* home_tree_branch) {
		if (!child_tree_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_branch_web_page_to_links ::tree_node_leaf undefined. \n");
			return false;
		}

		if (!home_tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_branch_web_page_to_links ::child_tree_branch undefined. \n");
			return false;
		}

		if (child_tree_leaf->previous_item_path.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_branch_web_page_to_links ::tree_node_leaf->previous_item_path undefined. \n");
			return false;
		}

		ws_tree_node_leaf_struct_type* tree_node_leaf = static_cast<ws_tree_node_leaf_struct_type*>(child_tree_leaf);

		std::filesystem::path current_web_page_file_pathname = tree_node_leaf->item_path;
		std::filesystem::path previous_web_page_file_pathname = tree_node_leaf->previous_item_path;
		ws_tree_class* root_node = get_tree_root_node(home_tree_branch);
		bool update_child_tree_leaf = false;

		//printf("web_page_update_to_links 00 : %s :: %s \n", current_web_page_file_pathname.string().c_str(), previous_web_page_file_pathname.string().c_str());

		if (!ws_html_manager.define_html_buffer(current_web_page_file_pathname)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "web_page_update_to_links : Cannot define web page file\n" + current_web_page_file_pathname.string() + "\nto update\n");
			return false;
		}

		//for (ws_tree_node_leaf_struct_type *tree_node_leaf_link_to : static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_to)) {// This does not work
		for (size_t i = 0; i < tree_node_leaf->internal_links_to.size(); i++) {
			ws_tree_node_leaf_struct_type* tree_node_leaf_link_to = static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_to[i]);

			//printf("update_branch_web_page_to_links 02A : %s :: %s \n", previous_web_page_file_pathname.string().c_str(), current_web_page_file_pathname.string().c_str());
			//printf("update_branch_web_page_to_links 02B : %s :: %s \n", root_node->item_path.string().c_str(), tree_node_leaf_link_to->item_path.string().c_str());
			if (!is_local(tree_node_leaf_link_to, home_tree_branch)) { // Do not have a internal from link of the child tree leaf and the relative links to it need to be updated
				//printf("update_branch_web_page_to_links !is_local  : %s : %s : %s\n", tree_node_leaf_link_to->item_path.string().c_str(), previous_web_page_file_pathname.string().c_str(), current_web_page_file_pathname.string().c_str());
				ws_html_manager.update_html_file_links(previous_web_page_file_pathname, current_web_page_file_pathname, root_node->item_path, tree_node_leaf_link_to->item_path);
				update_child_tree_leaf = true;
			}
		}

		if (update_child_tree_leaf) {
			ws_html_manager.save_html_file_updates(current_web_page_file_pathname); // Uncomment when testing completed
		}

		//tree_node_leaf->previous_item_path.clear();// To avoid incorrect previous path value NEED TO BE SURE THIS IS NEEDED !!!!!!!

		return true;
	}

	bool update_branch_web_page_from_links(ws_tree_node_item_struct_type* child_tree_leaf, ws_tree_class* home_tree_branch) {
		if (!child_tree_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_branch_web_page_from_links ::tree_node_leaf undefined. \n");
			return false;
		}

		if (!home_tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_branch_web_page_from_links ::child_tree_branch undefined. \n");
			return false;
		}

		if (child_tree_leaf->previous_item_path.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_branch_web_page_from_links ::tree_node_leaf->previous_item_path undefined. \n");
			return false;
		}

		ws_tree_node_leaf_struct_type* tree_node_leaf = static_cast<ws_tree_node_leaf_struct_type*>(child_tree_leaf);

		std::filesystem::path current_web_page_file_pathname = tree_node_leaf->item_path;
		std::filesystem::path previous_web_page_file_pathname = tree_node_leaf->previous_item_path;
		ws_tree_class* root_node = get_tree_root_node(home_tree_branch);

		printf("update_branch_web_page_from_links 00 : %s :: %s \n", current_web_page_file_pathname.string().c_str(), previous_web_page_file_pathname.string().c_str());

		//for (ws_tree_node_leaf_struct_type *tree_node_leaf_link_from : static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_from)) {// This does not work
		for (size_t i = 0; i < tree_node_leaf->internal_links_from.size(); i++) {
			ws_tree_node_leaf_struct_type* tree_node_leaf_link_from = static_cast<ws_tree_node_leaf_struct_type*>(tree_node_leaf->internal_links_from[i]);

			// Get web page files that references old_file_pathname and needs to have this reference updated to new_file_pathname
			printf("update_branch_web_page_from_links 05A  : %s : %s : %s\n", tree_node_leaf_link_from->item_path.string().c_str(), previous_web_page_file_pathname.string().c_str(), current_web_page_file_pathname.string().c_str());

			if (!is_local(tree_node_leaf_link_from, home_tree_branch)) { // Do not have a internal from link of the child tree leaf and the relative links to it need to be updated
				printf("update_branch_web_page_to_links !is_local  : %s : %s : %s\n", tree_node_leaf_link_from->item_path.string().c_str(), previous_web_page_file_pathname.string().c_str(), current_web_page_file_pathname.string().c_str());
				ws_html_manager.update_html_file_from_links(previous_web_page_file_pathname, current_web_page_file_pathname, root_node->item_path, tree_node_leaf_link_from->item_path);
			}
		}

		return true;
	}

	bool update_branch_web_page_to_file_links(ws_tree_node_item_struct_type* child_tree_leaf, ws_tree_class* home_tree_branch) {
		if (!child_tree_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_web_page_to_file_links ::tree_node_leaf undefined. \n");
			return false;
		}

		if (!home_tree_branch) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_web_page_to_file_links ::child_tree_branch undefined. \n");
			return false;
		}

		if (child_tree_leaf->previous_item_path.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_web_page_to_file_links ::tree_node_leaf->previous_item_path undefined. \n");
			return false;
		}

		ws_tree_node_leaf_struct_type* tree_leaf = static_cast<ws_tree_node_leaf_struct_type*>(child_tree_leaf);

		std::filesystem::path current_file_pathname = tree_leaf->item_path;
		std::filesystem::path previous_file_pathname = tree_leaf->previous_item_path;
		ws_tree_class* root_node = get_tree_root_node(home_tree_branch);

		for (ws_tree_node_item_struct_type* internal_from_link : tree_leaf->internal_links_from) { // NOTE: !!!!! NEEDS TO BE TESTED !!!!!!!!!
			if (!is_local(internal_from_link, home_tree_branch)) { // Do not have a internal from link of the child tree leaf and the relative links to it need to be updated
				//printf("update_branch_web_page_to_file_links !is_local  : %s : %s : %s\n", internal_from_link->item_path.string().c_str(), previous_file_pathname.string().c_str(), current_file_pathname.string().c_str());
				ws_html_manager.update_web_page_file_link(root_node->item_path, internal_from_link->item_path, previous_file_pathname, current_file_pathname);
			}
			//printf("update_branch_web_page_to_file_links 00  : %s : %s : %s\n", internal_from_link->item_path.string().c_str(), previous_file_pathname.string().c_str(), current_file_pathname.string().c_str());
		}

		return true;
	}

	bool update_edited_html_file_leaf_links(ws_tree_node_item_struct_type* child_tree_leaf) {// this is to update html file links have changed via editing 
		if (!child_tree_leaf) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "update_html_leaf_links ::tree_node_leaf undefined. \n");
			return false;
		}

		ws_tree_node_leaf_struct_type* tree_leaf = static_cast<ws_tree_node_leaf_struct_type*>(child_tree_leaf);

		for (ws_tree_node_item_struct_type* internal_to_link_leaf_node : tree_leaf->internal_links_to) { // NOTE: !!!!! NEEDS TO BE TESTED !!!!!!!!!
			// go to each tree node that link points to and remove from this leaf node 

			ws_tree_node_leaf_struct_type* internal_to_link_leaf = static_cast<ws_tree_node_leaf_struct_type*> (internal_to_link_leaf_node);

			ws_tree_node_item_struct_type* from_link;
			for (size_t i = 0; i < internal_to_link_leaf->internal_links_from.size(); i++) {
				from_link = internal_to_link_leaf->internal_links_from[i];
				//std::cout << "update_edited_html_file_leaf_links 11 " << from_link->item_path.string() << "\n";
				if (from_link->item_path == child_tree_leaf->item_path) {
					//std::cout << "update_edited_html_file_leaf_links 22 Before :: " << internal_to_link_leaf->item_path.string() << " : " <<internal_to_link_leaf->internal_links_from.size() << "\n";
					internal_to_link_leaf->internal_links_from.erase(internal_to_link_leaf->internal_links_from.begin() + i);
					//std::cout << "update_edited_html_file_leaf_links 22 After :: " << internal_to_link_leaf->internal_links_from.size() << "\n";
				}
			}
		}

		// remove all internal to links
		tree_leaf->internal_links_to.clear();
		// remove all external to links
		tree_leaf->external_links_to.clear();

		// recreate all html file external and internal links for this leaf 
		define_html_tree_leaf_links(tree_leaf);
	}

	// Update a tree branch contents :: Relevent if the tree branch files and directory contents has been added to
	// or deleted externally outside the application functions :: !!!!!! NOT YET FULLY TESTED !!!!!!
	void refresh_tree_branch(ws_tree_class* child_tree_branch) {
		// This structure would distinguish a file from a
		// directory
		struct stat sb;

		std::vector< std::filesystem::path> files;
		std::vector< std::filesystem::path> directories;

		// Looping until all the items of the directory are
		// exhausted
		for (const auto& entry : std::filesystem::directory_iterator(child_tree_branch->item_path)) {

			// Converting the path to const char * in the
			// subsequent lines
			std::filesystem::path outfilename = entry.path();
			std::string outfilename_str = outfilename.string();
			const char* path = outfilename_str.c_str();

			// Test whether the path points to a non-directory or not 
			if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) { // have a data file
std::cout <<"refresh_tree_branch AAAAA FILE" << path << std::endl;
				files.push_back(outfilename);
			} else { // have a directory
std::cout <<"refresh_tree_branch BBBB DIRECTORY" << path << std::endl;
				directories.push_back(outfilename);
			}
		}

		// Check to see if have new files in the tree branch directory not defined as a tree branch leaf
		for (int i = 0; i < files.size(); i++) {
			std::filesystem::path outfilename = files[i];
			std::string outfilename_str = outfilename.string();
			const char* path = outfilename_str.c_str();

printf("refresh_tree_branch define new leaves 000 :  %s \n", outfilename.string().c_str());

			if (!leaf_defined_in_tree(outfilename, child_tree_branch)) { // Have a new tree leaf to create and define of a file that did not previously exist

				std::string file_name = outfilename.filename().string();
				std::string file_path_name = path;

printf("refresh_tree_branch define new leaves 111 :  %s \n", outfilename.string().c_str());
				ws_tree_node_leaf_struct_type* ws_tree_node_item = new ws_tree_node_leaf_struct_type;

				ws_tree_node_item->label = TREE_NODE_PADDING + outfilename.filename().string();
				ws_tree_node_item->item_path = path;

				//ws_tree_node_item->icon_id = ICON_FA_FILE;
				ws_tree_node_item->icon_id = get_icon_id(outfilename.extension().string().c_str());
				ws_tree_node_item->tree_node_item_type = afw_tree_node_item_type_enum::tree_leaf;
				ws_tree_node_item->tree_node_id = tree_node_id_key.get_available_id_key();

				ws_tree_node_item->node_item_tree_branch = child_tree_branch->node_item_tree_branch;// +++++++

				std::string extension = ws_tree_node_item->item_path.extension().string();

printf("refresh_tree_branch define new leaves 222 :  %s \n", ws_tree_node_item->item_path.string().c_str());

				ws_tree_node_item->afw_file_item_category_type = get_item_category_type(extension);

printf("refresh_tree_branch define new leaves 333 %s : %i \n", ws_tree_node_item->item_path.string().c_str(), ws_tree_node_item->tree_node_id);

				child_tree_branch->leaves.push_back(ws_tree_node_item);
			}
		}
printf("refresh_tree_branch CCCC\n");
		// Check to see if have new sub directories in the tree branch directory not defined as a tree branch
		for (int i = 0; i < directories.size(); i++) {
			std::filesystem::path outfilename = directories[i];
			std::string outfilename_str = outfilename.string();
			const char* path = outfilename_str.c_str();
printf("refresh_tree_branch define new child branch 000 :  %s \n", outfilename.string().c_str());
			if (!branch_defined_in_tree(outfilename, child_tree_branch)) {// Have a tree new tree branch to create and define
				ws_tree_class* tree_branch = create_tree_branch(child_tree_branch, outfilename.string(), path);

				if (tree_branch) {
					branches.push_back(tree_branch);
					tree_branch->refresh_tree_branch(tree_branch);
				}
			}
		}

printf("refresh_tree_branch DDDD\n");
		// Check to see if have tree branch leaves that no longer have a file associated with them
		// ie represent a file no longer present in the directory and remove them from tree branch leaves
		for (int i = child_tree_branch->leaves.size()-1; i > -1; i--) {
			if (!leaf_defined_in_files(child_tree_branch->leaves[i]->item_path, files)) {
printf("refresh_tree_branch DDDD 1111 %s \n", child_tree_branch->leaves[i]->item_path);
				delete_branch_leaf(child_tree_branch, child_tree_branch->leaves[i]->tree_node_id,false);
			}
		}
printf("refresh_tree_branch EEEE\n");
		// Check to see if have tree child branch that no longer have a directory associated with them
		// ie represent a sub directory that is no longer present in the directory and remove them from tree branch leaves
		for (int i = child_tree_branch->branches.size()-1; i > -1; i--) {
			if (!directory_defined_in_files(child_tree_branch->branches[i]->item_path, directories)) {
				//delete_child_branch(child_tree_branch);
			}
		}

	}

	std::string get_icon_id(std::string file_extension) {
		//std::string extension = FW::stringtools::toLowerCase(file_extension);

//std::cout<< "get_icon_id 000 : " << extension << "\n";
		afw_file_item_category_type_enum file_category_type = get_item_category_type(file_extension);

		switch (file_category_type) {
			case afw_file_item_category_type_enum::pageformat : {return ICON_FA_FILE_TEXT; break;}
			//case afw_file_item_category_type_enum::page :		{return ICON_FA_HTML5; break;}// This icon does not seem to work with imgui 
			case afw_file_item_category_type_enum::page :		{return ICON_FA_FILE_WORD_O; break;}// W for web page
			case afw_file_item_category_type_enum::image:		{return ICON_FA_FILE_IMAGE_O; break;}
			case afw_file_item_category_type_enum::audio:		{return ICON_FA_FILE_AUDIO_O; break;}
			case afw_file_item_category_type_enum::media:		{return ICON_FA_FILE_VIDEO_O; break;}
			case afw_file_item_category_type_enum::code:		{return ICON_FA_FILE_CODE_O; break;}
			case afw_file_item_category_type_enum::pdf:			{return ICON_FA_FILE_PDF_O; break;}
			case afw_file_item_category_type_enum::script :		{return ICON_FA_FILE_CODE_O; break;}
			case afw_file_item_category_type_enum::directory :	{return ICON_FA_FILE; break;}
		}
		
		return ICON_FA_FILE;

/*
#define ICON_FA_FILE_PDF_O "\xef\x87\x81"	// U+f1c1
#define ICON_FA_FILE_WORD_O "\xef\x87\x82"	// U+f1c2
#define ICON_FA_FILE_EXCEL_O "\xef\x87\x83"	// U+f1c3
#define ICON_FA_FILE_POWERPOINT_O "\xef\x87\x84"	// U+f1c4
#define ICON_FA_FILE_IMAGE_O "\xef\x87\x85"	// U+f1c5
#define ICON_FA_FILE_ARCHIVE_O "\xef\x87\x86"	// U+f1c6
#define ICON_FA_FILE_AUDIO_O "\xef\x87\x87"	// U+f1c7
#define ICON_FA_FILE_VIDEO_O "\xef\x87\x88"	// U+f1c8
#define ICON_FA_FILE_CODE_O "\xef\x87\x89"	// U+f1c9
*/
	}

	bool leaf_defined_in_files(std::filesystem::path item_path, std::vector< std::filesystem::path> files){
		bool tree_leaf_defined = false;
		for (int i = 0; i < files.size(); i++) {
			//if (item_path.string() == files[i].string()) {
			if (item_path == files[i]) {

//printf("leaf_defined_in_files 1111\n", item_path);
				tree_leaf_defined = true;
				break;
			}
		}

		return tree_leaf_defined;
	}

	bool directory_defined_in_files(std::filesystem::path item_path, std::vector< std::filesystem::path> directories) {
		bool tree_branch_defined = false;
		for (int i = 0; i < directories.size(); i++) {
			//if (item_path.string() == directories[i].string()) {
			if (item_path == directories[i]) {
				tree_branch_defined = true;
				break;
			}
		}

		return tree_branch_defined;
	}

	bool leaf_defined_in_tree(std::filesystem::path outfilename,ws_tree_class *child_tree_branch) {
		bool tree_leaf_defined = false;
		for (int i = 0; i < child_tree_branch->leaves.size(); i++) {
			//if (outfilename.string() == child_tree_branch->leaves[i]->item_path.string()) {
			if (outfilename == child_tree_branch->leaves[i]->item_path) {
				tree_leaf_defined = true;
				break;
			}
		}

		return tree_leaf_defined;
	}

	bool branch_defined_in_tree(std::filesystem::path outfilename, ws_tree_class* child_tree_branch) {
		bool tree_branch_defined = false;
		for (int i = 0; i < child_tree_branch->branches.size(); i++) {
			//if (outfilename.string() == child_tree_branch->branches[i]->item_path.string()) {
			if (outfilename == child_tree_branch->branches[i]->item_path) {
				tree_branch_defined = true;
				break;
			}
		}

		return tree_branch_defined;
	}

	bool is_local(ws_tree_node_item_struct_type *file_link,ws_tree_class *home_tree_branch) {
		std::string file_link_current_path   = file_link->item_path.string();
		std::string tree_branch_current_path = home_tree_branch->item_path.string();

		FW::stringtools::replaceInPlace(file_link_current_path, "\\", "/");
		FW::stringtools::replaceInPlace(tree_branch_current_path, "\\", "/");

//printf("is_local 00  : %s : %s : %s\n", file_link_current_path.c_str(), tree_branch_current_path.c_str());
		if (file_link_current_path.find(tree_branch_current_path) != std::string::npos) return true;


		return false;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	bool perform_move_task(ws_tree_node_leaf_struct_type* source, ws_tree_class* destination) { // Can only drag and drop a source file or directory into a target directory

printf("perform_move_task file01 \n");
printf("perform_move_task file   : source %s : destination %s \n", source->item_path.string().c_str(), destination->item_path.string().c_str());
printf("perform_move_task branch : source %s : destination %s \n", source->node_item_tree_branch->item_path.string().c_str(), destination->node_item_tree_branch->item_path.string().c_str());

		if (source->item_path == destination->item_path) return false;// To avoid accidently trying to move selected branch into itself 

		// Move tree branch leaf
		if (source->tree_node_item_type == afw_tree_node_item_type_enum::tree_leaf) return move_tree_branch_leaf(source, destination);

		// Move tree branch
		if (source->tree_node_item_type == afw_tree_node_item_type_enum::tree_node) return move_tree_branch(source, destination);

printf("perform_move_task POST file   : source %s : destination %s \n", source->item_path.string().c_str(), destination->item_path.string().c_str());
printf("perform_move_task POST branch : source %s : destination %s \n", source->node_item_tree_branch->item_path.string().c_str(), destination->node_item_tree_branch->item_path.string().c_str());

		return false;
	}

	bool move_tree_branch_leaf(ws_tree_node_leaf_struct_type *source, ws_tree_class* destination) {
		ws_tree_class*		  source_tree_branch  = source->node_item_tree_branch;
		//std::filesystem::path old_source_pathname = source->item_path;

//if (!source_tree_branch) printf("move_tree_branch_leaf 00 source_tree_branch == nullptr \n"); 
//else {
//	printf("move_tree_branch_leaf 00A source_tree_branch != nullptr \n");
//	printf("move_tree_branch_leaf  00B %s \n", source_tree_branch->tree_node.item_path.string().c_str());
//	if(source.node_item_tree_branch == destination.node_item_tree_branch) printf("source.node_item_tree_branch == destination.node_item_tree_branch \n");
//}

		int source_tree_leaf_index = find_tree_leaf(source->tree_node_id, source_tree_branch);

		if (source_tree_leaf_index < 0) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "move_tree_branch_leaf :: Could not find source tree index. Did not move !!!");
			return false;
		}

		std::filesystem::path old_item_path = source->item_path;
		std::filesystem::path new_item_path = destination->item_path.string() + "/" + old_item_path.filename().string();


		// !!!!!!!!!!!!!!! Uncomment following when not testing !!!!!!!!!!!!!!!!!!!!!
		if (std::filesystem::exists(new_item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "move_tree_branch_leaf :: Source file pathname " + old_item_path.string() + "\nhas file of same name in destination directory. Did not move !!!");
			return false;
		}

		// ###############
		// Move branch leaf file
		if (!std::filesystem::exists(old_item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "move_tree_branch_leaf :: Source file pathname " + old_item_path.string() + " to move does not exist\n");
			return false;
		}

		if (!std::filesystem::exists(destination->item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "move_tree_branch_leaf :: Destination file pathname " + destination->item_path.string() + " to move to does not exist\n");
			return false;
		}

		std::filesystem::rename(old_item_path, new_item_path);// Move source file to move with pathname old_item_path to destination new_item_pathname UNCOMMENT WHEN TESTING OMPLETE
		// ###############


printf("move_tree_branch_leaf #### PRE file   : source %s : destination %s \n", source->item_path.string().c_str(), destination->item_path.string().c_str());
		//update tree entries
		source->node_item_tree_branch = destination->node_item_tree_branch;

		// Update source pathname
		source->previous_item_path = source->item_path;
		source->item_path		   = new_item_path;

		// Add source leaf being moved to destination tree directory node of files or leaves :: Crtical to have this as last tree action to be performed
		destination->node_item_tree_branch->leaves.push_back(source);

printf("move_tree_branch_leaf #### POST file   : source %s : destination %s \n", source->item_path.string().c_str(), destination->item_path.string().c_str());
		source_tree_branch->leaves[source_tree_leaf_index] = nullptr;
		source_tree_branch->leaves.erase(source_tree_branch->leaves.begin() + source_tree_leaf_index);// Erase leaf reference in source tree node directory leaves list

		return true;
	}

	bool move_tree_branch(ws_tree_node_item_struct_type *source, ws_tree_class *destination) {
//printf("move_tree_branch 00 file : source %s : destination %s \n", source->item_path.string().c_str(), destination->item_path.string().c_str());
		ws_tree_class* source_parent_tree_branch = source->node_item_tree_branch->parent_branch;

		if (!source->node_item_tree_branch->parent_branch) return false; // cannot move tree root

		int source_tree_branch_index = find_tree_branch(source->tree_node_id, source_parent_tree_branch);

		if (source_tree_branch_index < 0) return false;

		std::filesystem::path old_item_path = source->item_path;
		std::filesystem::path new_item_path = destination->item_path.string() + "/" + old_item_path.filename().string();

//printf("move_tree_branch 01 file   : old_item_path %s : new_item_path %s \n", old_item_path.string().c_str(), new_item_path.string().c_str());

		if (std::filesystem::exists(new_item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "move_tree_branch :: Source directory pathname " + old_item_path.string() + "\nhas directory of same name in destination directory. Did not move !!!");
			return false;
		}

		// Move branch directory
		if (!std::filesystem::exists(old_item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "move_tree_branch_leaf :: Source directory pathname " + old_item_path.string() + " to move does not exist\n");
			return false;
		}

		if (!std::filesystem::exists(destination->item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "move_tree_branch_leaf :: Destination directory pathname " + destination->item_path.string() + " to move tp does not exist\n");
			return false;
		}

		std::filesystem::rename(old_item_path, new_item_path);// !!!!!!!!!! Uncomment when not testing with directory move or renaming !!!!!!!!!!!

		//update tree entries
		source->node_item_tree_branch->parent_branch = destination->node_item_tree_branch;

		// Update source branch pathname and child branches and file pathnames
		source->previous_item_path				 = source->item_path;
		source->item_path						 = new_item_path;
		source->node_item_tree_branch->item_path = new_item_path;
		source->node_item_tree_branch->label     = TREE_NODE_PADDING + new_item_path.string();

		// Add source branch being moved to destination tree directory node of files or leaves :: Crtical to have this as last tree action to be performed
		destination->node_item_tree_branch->branches.push_back(source->node_item_tree_branch);

		update_tree_branch_paths(source->node_item_tree_branch);// !!!!!!!!!! Uncomment when not testing with directory move or renaming !!!!!!!!!!!

		source_parent_tree_branch->branches[source_tree_branch_index] = nullptr;
		source_parent_tree_branch->branches.erase(source_parent_tree_branch->branches.begin() + source_tree_branch_index);

		return true;
	}

	void update_tree_branch_paths(ws_tree_class* tree_branch) {

		for (ws_tree_class *child_tree_branch : tree_branch->branches) {
			std::filesystem::path old_item_path = child_tree_branch->item_path;
			std::filesystem::path new_item_path = tree_branch->item_path.string() + "/" + old_item_path.filename().string();

			// Update source branch pathname and child branches and file pathnames
			child_tree_branch->previous_item_path				= old_item_path;
			child_tree_branch->item_path						= new_item_path;
			child_tree_branch->node_item_tree_branch->item_path = new_item_path;
			child_tree_branch->node_item_tree_branch->label		= TREE_NODE_PADDING + new_item_path.string();

			update_tree_branch_paths(child_tree_branch);
		}

		for (ws_tree_node_item_struct_type *child_tree_leaf : tree_branch->leaves) {
			child_tree_leaf->node_item_tree_branch = tree_branch->node_item_tree_branch;

			std::filesystem::path old_item_path = child_tree_leaf->item_path;
			std::filesystem::path new_item_path = tree_branch->item_path.string() + "/" + old_item_path.filename().string();

			child_tree_leaf->previous_item_path = old_item_path;
			child_tree_leaf->item_path			= new_item_path;

printf("update_tree_branch_paths file   : source %s : destination %s \n", child_tree_leaf->previous_item_path.string().c_str(), child_tree_leaf->item_path.string().c_str());
printf("update_tree_branch_paths branch : source %s : destination %s \n", tree_branch->previous_item_path.string().c_str(), tree_branch->item_path.string().c_str());

		}
	}

	// ------------------------------------------------------------------------------

	ws_tree_class* get_tree_node(std::filesystem::path tree_node_path, ws_tree_class* current_tree_node) {
		ws_tree_class* tree_node = nullptr;

		for (ws_tree_node_item_struct_type* leaf : current_tree_node->leaves) {
//printf("get_tree_node 00  : %s : %s : %s \n", current_tree_node->tree_node.item_path.string().c_str(), tree_node_path.string().c_str(), leaf.item_path.string().c_str());
			if (leaf->item_path == tree_node_path)
				return current_tree_node;
		}

		for (ws_tree_class* branch : current_tree_node->branches) {
			if ((tree_node = get_tree_node(tree_node_path, branch)))
				return tree_node;
		}

		return nullptr;
	}

	ws_tree_node_leaf_struct_type *get_tree_node_leaf_item(std::filesystem::path leaf_file_path){
		ws_tree_class *root = get_tree_root_node(this);

		return get_tree_node_leaf_item(root,leaf_file_path);
	}
	
	ws_tree_node_leaf_struct_type *get_tree_node_leaf_item(ws_tree_class *tree_node, std::filesystem::path leaf_file_path) {
		
		for (ws_tree_node_leaf_struct_type *leaf : tree_node->leaves) {
			if (leaf->item_path == leaf_file_path) return leaf;
		}

		for (ws_tree_class *branch : tree_node->branches) {
			ws_tree_node_leaf_struct_type *leaf = get_tree_node_leaf_item(branch, leaf_file_path);
			if (leaf) {
				return leaf;
				break;
			}
		}

		return nullptr;
	}
	
	id_type find_tree_leaf(id_type tree_node_id, ws_tree_class* ws_tree_class) {

printf("find_tree_leaf : tree_node_id to find ws_tree_class->leaves.size() %i : \n", ws_tree_class->leaves.size());

		for (int i = 0; i < ws_tree_class->leaves.size(); i++) {
			if (ws_tree_class->leaves[i]->tree_node_id == tree_node_id) {
printf("find_tree_leaf : tree_node_id to find %i :  tree_node_id %i \n", tree_node_id, ws_tree_class->leaves[i]->tree_node_id);
				return i;
			}
		}

		return -1;
	}

	id_type find_tree_leaf(std::filesystem::path tree_node_path, ws_tree_class *ws_tree_class) {

		printf("find_tree_leaf : tree_node_id to find ws_tree_class->leaves.size() %i : \n", ws_tree_class->leaves.size());

		for (int i = 0; i < ws_tree_class->leaves.size(); i++) {
			if (ws_tree_class->leaves[i]->item_path == tree_node_path) {
printf("find_tree_leaf : tree_node_path to find %s :  tree_node_path %s \n", tree_node_path.string().c_str(), ws_tree_class->leaves[i]->item_path.string().c_str());
				return i;
			}
		}

		return -1;
	}

	id_type find_tree_branch(id_type tree_node_id, ws_tree_class* ws_tree_class) {
		for (int i = 0; i < ws_tree_class->branches.size(); i++) {
			if (ws_tree_class->branches[i]->tree_node_id == tree_node_id)
				return i;
		}

		return -1;
	}

	ws_tree_class *get_tree_root_node(ws_tree_class* ws_tree) {
		ws_tree_class* root = nullptr;

		if (ws_tree->tree_node_item_type != afw_tree_node_item_type_enum::root_tree_node) {
			root = get_tree_root_node(ws_tree->parent_branch);
		}
		else
			root = ws_tree;
//printf("get_tree_root_node root %s \n", root->tree_node.item_path.string().c_str());

		return root;
	}

	// ----------------------------------------------------------------------------------------------------------------------------------
	bool is_html_file(std::filesystem::path tree_node_path) {
		std::string extension = FW::stringtools::toLowerCase(file_path_tools_class::get_file_extension(tree_node_path));

		if (extension == ".html" || extension == ".htm" || extension == ".xhtml" || extension == ".xps") return true;

		return false;
	}

	bool is_html_file(ws_tree_node_leaf_struct_type *tree_node_leaf) {
		return is_html_file(tree_node_leaf->item_path);
	}

	bool define_html_tree_leaf_links(ws_tree_node_leaf_struct_type *tree_node_leaf) {
//printf("ws_tree_node_links_struct_type::define_links 01\n");
		if (!is_html_file(tree_node_leaf->item_path)) return false;
//printf("ws_tree_node_links_struct_type::define_links 02\n");

		ws_html_manager_class ws_html_manager;

		if (!ws_html_manager.open_file_to_read(tree_node_leaf->item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "define_links :: Could not open html file " + tree_node_leaf->item_path.string());
			return false;
		}

		std::filesystem::path root_node_path = get_tree_root_node(this)->item_path.string();

//printf("ws_tree_node_links_struct_type::define_links 03\n");
		if (!ws_html_manager.define_html_file_links(tree_node_leaf->item_path, root_node_path)) return false;

//printf("ws_tree_node_links_struct_type::external_links 04 : %i\n", ws_html_manager.external_links_to.size());
		if (ws_html_manager.external_links_to.size() > 0) {
			tree_node_leaf->external_links_to = ws_html_manager.external_links_to;
//for (int i = 0; i < tree_node_leaf->external_links_to.size(); i++) {
//printf("ws_tree_node_links_struct_type::external_links 05 : %s :: %s\n", tree_node_leaf->item_path.string().c_str(), tree_node_leaf->external_links_to[i].string().c_str());
//}
		}

//printf("ws_tree_node_links_struct_type::internal_links 06 : %i\n", ws_html_manager.internal_links_to.size());
		for (std::filesystem::path internal_link_to_path : ws_html_manager.internal_links_to) {
			std::filesystem::path absolute_internal_link_to_path;

			if (!file_path_tools_class::is_absolute_path(internal_link_to_path))
				absolute_internal_link_to_path = file_path_tools_class::get_absolute_pathname(internal_link_to_path.string(), tree_node_leaf->item_path);
			else
				absolute_internal_link_to_path = internal_link_to_path;

//printf("ws_tree_node_links_struct_type::external_links 06 : %s :: %s : %s\n", internal_link_to_path.string().c_str(), tree_node_leaf->item_path.string().c_str(), absolute_internal_link_to_path.string().c_str());


			ws_tree_node_leaf_struct_type *internal_link_to = get_tree_node_leaf_item(absolute_internal_link_to_path);

			if (!internal_link_to) {
//printf("ws_tree_node_links_struct_type::!internal_link_to 07 : %s :: %s : %s\n", tree_node_leaf->item_path.string().c_str(), internal_link_to_path.string().c_str(), absolute_internal_link_to_path.string().c_str());
				// Uncomment when testing complete or to test data with errors
				//afw_globalc::get_current_logger()->log(LogLevel::INFO, "define_html_tree_leaf_links :: Internal link to file of pathname " + absolute_internal_link_to_path.string() + "\ncould not be found !!!");
			} else {
//printf("ws_tree_node_links_struct_type::internal_link_to 08 : %s :: %s : %s\n", tree_node_leaf->item_path.string().c_str(), internal_link_to_path.string().c_str(), absolute_internal_link_to_path.string().c_str());

				tree_node_leaf->internal_links_to.push_back(internal_link_to);
				define_link_from(internal_link_to, tree_node_leaf);
			}
		}

//for (int i = 0; i < tree_node_leaf->internal_links_to.size(); i++) {
//printf("ws_tree_node_links_struct_type::internal_links 09 : %s :: %s : %s\n", tree_node_leaf->item_path.string().c_str(), ws_html_manager.internal_links_to[i].string().c_str(), tree_node_leaf->internal_links_to[i]->item_path.string().c_str());
//}

		return true;
	}

	bool define_link_from(ws_tree_node_leaf_struct_type *tree_node_leaf_to_define_from_link, ws_tree_node_leaf_struct_type *tree_node_leaf_from) {

		for (ws_tree_node_item_struct_type *leaf_to_define_from_link_leaf : tree_node_leaf_to_define_from_link->internal_links_from) {
			if (leaf_to_define_from_link_leaf == tree_node_leaf_from) { // From link already exists for the tree_node_leaf_to_define_from_link an needs not be added
				return false;
				break;
			}
		}

		tree_node_leaf_to_define_from_link->internal_links_from.push_back(tree_node_leaf_from);  // From link does not exists for the tree_node_leaf_to_define_from_link. Add link entry to links from list

		return true;
	}
	
	// ----------------------------------------------------------------------------------------------------------------------------------

	// Yet to be tested
	void delete_ws_tree(bool delete_disc_directory) {
		for (int i = branches.size() - 1; i > 0; i--) {
			id_type free_id = branches[i]->tree_node_id;
			delete_ws_tree_branch(branches[i], delete_disc_directory);
			tree_node_id_key.assign_free_id_key(free_id);
		}

		for (int i = 0; i < leaves.size(); i++) {
			tree_node_id_key.assign_free_id_key(leaves[i]->tree_node_id);
		}

		leaves.clear();
		branches.clear();

//printf("delete_ws_tree root %s \n", (tree_node.item_path.parent_path().string() + "/" + tree_node.item_path.filename().string()).c_str());
	}

	void create_ws_tree_branch(ws_tree_class *parent_branch, std::string new_tree_branch_name) {
		if (!parent_branch) return;

		std::string new_directory_s = parent_branch->item_path.string() + "/" + new_tree_branch_name;
		//std::string new_directory_s = new_directory.string().c_str();

		FW::stringtools::replaceInPlace(new_directory_s, "\\", "/");

		std::filesystem::path new_directory = new_directory_s.c_str();


//std::cout << "create_ws_tree_branch 111: " << new_directory_s << " : " << new_directory.string() << "\n";
		ws_tree_class *new_branch =  create_tree_branch(parent_branch, new_directory_s, new_directory, false);

		if (new_branch) {
//std::cout << "create_ws_tree_branch 222: " << new_directory_s << " : " << parent_branch->branches.size() <<  "\n";
			parent_branch->branches.push_back(new_branch);
//std::cout << "create_ws_tree_branch 333: " << new_directory_s << " : " << parent_branch->branches.size() <<  "\n";
			std::filesystem::create_directory(new_directory);
		}
	}

	void delete_ws_tree_branch(ws_tree_class *tree_branch_to_delete, bool delete_disc_directory) {
		ws_tree_class* parent_branch = tree_branch_to_delete->parent_branch;
//printf("delete_ws_tree_branch 000 parent_branch %s \n", parent_branch->item_path.string().c_str());

		for (int i = 0; i < tree_branch_to_delete->branches.size(); i++) {
			delete_child_branch(tree_branch_to_delete->branches[i], delete_disc_directory);
		}

		for (int i = 0; i < tree_branch_to_delete->leaves.size(); i++) {
//printf("delete_ws_tree_branch 111 leaves %s \n", tree_branch_to_delete->leaves[i]->item_path.string().c_str());
			tree_node_id_key.assign_free_id_key(tree_branch_to_delete->leaves[i]->tree_node_id);
			delete(tree_branch_to_delete->leaves[i]);
			//delete_branch_leaf(tree_branch_to_delete, tree_branch_to_delete->leaves[i]->tree_node_id); Should this be used instead of above two lines
		}

		tree_branch_to_delete->leaves.clear();
		tree_branch_to_delete->branches.clear();

		if (parent_branch != nullptr) {
//printf("delete_ws_tree_branch 222AA branches %s \n", parent_branch->item_path.string().c_str());
			for (int i = 0; i < parent_branch->branches.size(); i++) {
//printf("delete_ws_tree_branch 222BB branches %s \n", parent_branch->branches[i]->item_path.string().c_str());
				if (parent_branch->branches[i]->tree_node_id == tree_branch_to_delete->tree_node_id) {
					parent_branch->branches.erase(parent_branch->branches.begin() + i);
					break;
				}
			}
		}

//printf("delete_ws_tree_branch 333 leaves %s \n", tree_branch_to_delete->item_path.string().c_str());
		if (delete_disc_directory) std::filesystem::remove_all(tree_branch_to_delete->item_path);// delete directory : Uncomment when want to implement

//printf("delete_ws_tree_branch 444 leaves %s \n", tree_branch_to_delete->item_path.string().c_str());
		delete(tree_branch_to_delete);
//printf("delete_ws_tree_branch 555 leaves %s \n", tree_branch_to_delete->item_path.string().c_str());

	}

	void delete_child_branch(ws_tree_class *tree_branch_to_delete, bool delete_disc_directory) {
//printf("delete_child_branch 000 child_branch %s \n", tree_branch_to_delete->item_path.parent_path().string().c_str());

		for (int i = tree_branch_to_delete->branches.size() - 1; i >= 0; i--) {
//printf("delete_ws_tree_branch  %i : %s \n", i, (tree_branch_to_delete->branches[i]->item_path.string().c_str() + '/' + tree_branch_to_delete->branches[i]->item_path.filename().string()).c_str());

			id_type free_id = tree_branch_to_delete->branches[i]->tree_node_id;
			delete_child_branch(tree_branch_to_delete->branches[i], delete_disc_directory);
			tree_node_id_key.assign_free_id_key(free_id);
		}

		for (int i = 0; i < tree_branch_to_delete->leaves.size(); i++) {
			//tree_node_id_key.assign_free_id_key(tree_branch_to_delete->leaves[i]->tree_node_id);
			//delete(tree_branch_to_delete->leaves[i]);
			delete_branch_leaf(tree_branch_to_delete, tree_branch_to_delete->leaves[i]->tree_node_id, delete_disc_directory); //Should this be used instead of above two lines
		}

		tree_branch_to_delete->leaves.clear();
		tree_branch_to_delete->branches.clear();

//printf("ws_tree_branch deleted %s deleted \n", (tree_branch_to_delete->item_path.string().c_str() + '/' + tree_branch_to_delete->item_path.filename().string()).c_str());
		tree_node_id_key.assign_free_id_key(tree_branch_to_delete->tree_node_id);
		
		//std::filesystem::remove_all(tree_branch_to_delete->item_path);// delete directory but suspect not needed here as delete is performed in delete_ws_tree_branch : Uncomment when want to implement
		
		delete tree_branch_to_delete;
	}

	void delete_branch_leaf(ws_tree_class *branch, unsigned int leaf_id, bool delete_item_from_disc) {
//std::cout << "delete_branch_leaf 00 : "<< branch->item_path.string() << " : " << leaf_id << " : \n";

		for (int i = 0; i < branch->leaves.size(); i++) {
			ws_tree_node_item_struct_type *branch_tree_leaf = branch->leaves[i];

//std::cout << "delete_branch_leaf 11 : "<< branch_tree_leaf->item_path.string() << " : " << branch_tree_leaf->tree_node_id << " : \n";

			if (branch_tree_leaf->tree_node_id == leaf_id) {
//std::cout << "delete_branch_leaf 22 : "<< branch_tree_leaf->item_path.string() << " : " << branch_tree_leaf->tree_node_id << " : " << leaf_id << "\n";

				// Have file delete function here
				ws_tree_node_leaf_struct_type *tree_leaf = static_cast<ws_tree_node_leaf_struct_type*>(branch_tree_leaf);

//std::cout << "delete_branch_leaf 33: "<< branch_tree_leaf->item_path.string() << " : " << tree_leaf->item_path.string() << " : " << leaf_id << "\n";

				for (size_t lfi = 0; lfi < tree_leaf->internal_links_from.size(); lfi++) { // Go through each link to this tree_leaf tree node that is from another tree leaf node
					ws_tree_node_item_struct_type *from_tree_leaf = tree_leaf->internal_links_from[lfi];
					ws_tree_node_leaf_struct_type *from_tree_leaf_to = static_cast<ws_tree_node_leaf_struct_type*>(from_tree_leaf);

					for (size_t j = 0; j < from_tree_leaf_to->internal_links_to.size(); j++) {// find the link to this tree_leaf tree node from the another tree leaf node to link list
						if (from_tree_leaf_to->internal_links_to[j]->item_path == branch_tree_leaf->item_path) {
							from_tree_leaf_to->internal_links_to.erase(from_tree_leaf_to->internal_links_to.begin() + j); // Erase the link to this this tree_leaf tree node
						}
					}
				}

//std::cout << "delete_branch_leaf 66: \n";

				for (size_t lti = 0; lti < tree_leaf->internal_links_to.size(); lti++) {// Go through each link from this tree_leaf tree node to that this is from another tree leaf node
					ws_tree_node_item_struct_type* to_tree_leaf = tree_leaf->internal_links_to[lti];
					ws_tree_node_leaf_struct_type* to_tree_leaf_from = static_cast<ws_tree_node_leaf_struct_type*>(to_tree_leaf);

					for (size_t j = 0; j < to_tree_leaf_from->internal_links_from.size(); j++) {// find the link from this tree_leaf tree node to the another tree leaf node to link list
						if (to_tree_leaf_from->internal_links_from[j]->item_path == branch_tree_leaf->item_path) {
							to_tree_leaf_from->internal_links_from.erase(to_tree_leaf_from->internal_links_from.begin() + j); // Erase the link from this this tree_leaf tree node
						}
					}
				}
				
//std::cout << "delete_branch_leaf 99: \n";

				//if (delete_item_from_disc) std::filesystem::remove(tree_leaf->item_path) ;// Delete file that tree leaf represents; Uncomment when ready to implement
				if (delete_item_from_disc) delete_file_to_recycle_bin(tree_leaf->item_path) ;// Delete file that tree leaf represents to windows recycle bin; Uncomment when ready to implement
				
				tree_node_id_key.assign_free_id_key(branch->leaves[i]->tree_node_id);
				delete(branch->leaves[i]);
				branch->leaves.erase(branch->leaves.begin() + i);
				break;
			}
		}
	}

	void delete_file_to_recycle_bin(std::filesystem::path item_path) {
		// File string must have an additional terminating null or have a problem that 
		// If file is restored from recycle bin, then the file cannot be deleted !!!!!
		std::string s = item_path.string().c_str();
		// Add additional null terminator to string of file pathname
		int s_size = s.size();
		s.resize(s_size+1);
		s[s_size] = '\0';

		// Define a wide string to be used in moving a file to the reycle bin
		// The wierdness of Microsoft and its use of wide strings to probably 
		// allow non ASCII character use in filenames.
		FW::stringtools::replaceInPlace(s, "\\", "/");
		std::wstring ws = std::wstring(s.begin(), s.end());

		// Define structure to perform action to move file to recycle bin
		SHFILEOPSTRUCT f = { 0 };
		f.hwnd = NULL;
		ZeroMemory(&f, sizeof(SHFILEOPSTRUCT));
		f.wFunc  = FO_DELETE;
		f.fFlags = FOF_ALLOWUNDO | FOF_NO_CONNECTED_ELEMENTS;// Must specify FOF_ALLOWUNDO to move the files to the Recycle Bin!

		f.pFrom = ws.c_str();
		f.pTo = NULL;
		f.fAnyOperationsAborted = FALSE;
		f.hNameMappings = NULL;
		f.lpszProgressTitle = NULL;
//std::cout << "delete_file_to_recycle_bin 00: " << s.c_str() << ":\n";
		SHFileOperation(&f);// Move file to recycle bin
	}

	bool create_copy_of_leaf(ws_tree_class *branch, unsigned int leaf_id) {

		for (int i = 0; i < branch->leaves.size(); i++) {
			ws_tree_node_item_struct_type* branch_tree_leaf = branch->leaves[i];

			if (branch_tree_leaf->tree_node_id == leaf_id) {
			
				// Have file delete function here
				ws_tree_node_leaf_struct_type *tree_leaf_to_copy = static_cast<ws_tree_node_leaf_struct_type*>(branch_tree_leaf);

				ws_tree_node_leaf_struct_type *tree_leaf_copy = new ws_tree_node_leaf_struct_type;

				tree_leaf_copy->item_path    = create_new_tree_leaf_item_pathname(tree_leaf_to_copy, branch);
				if(tree_leaf_copy->item_path.empty())  return false;

				tree_leaf_copy->label						= TREE_NODE_PADDING + tree_leaf_copy->item_path.filename().string();
				tree_leaf_copy->tree_node_id				= tree_node_id_key.get_available_id_key();
				tree_leaf_copy->icon_id						= tree_leaf_to_copy->icon_id;
				tree_leaf_copy->tree_item_type				= tree_leaf_to_copy->tree_item_type;
				tree_leaf_copy->tree_node_item_type			= tree_leaf_to_copy->tree_node_item_type;
				tree_leaf_copy->node_item_tree_branch       = tree_leaf_to_copy->node_item_tree_branch;
				tree_leaf_copy->afw_file_item_category_type = tree_leaf_to_copy->afw_file_item_category_type;

				for (size_t i = 0; i < tree_leaf_to_copy->internal_links_from.size(); i++) { // Go through each link to this tree_leaf tree node that is from another tree leaf node
					ws_tree_node_item_struct_type *from_tree_leaf = tree_leaf_to_copy->internal_links_from[i];
					tree_leaf_copy->internal_links_from.push_back(from_tree_leaf);

					ws_tree_node_leaf_struct_type *from_tree_leaf_to = static_cast<ws_tree_node_leaf_struct_type*>(from_tree_leaf);
					from_tree_leaf_to->internal_links_to.push_back(tree_leaf_copy);
				}

std::cout << "create_copy_of_leaf 66: \n";

				for (size_t i = 0; i < tree_leaf_to_copy->internal_links_to.size(); i++) {// Go through each link from this tree_leaf tree node to that this is from another tree leaf node
					ws_tree_node_item_struct_type *to_tree_leaf = tree_leaf_to_copy->internal_links_to[i];
					tree_leaf_copy->internal_links_to.push_back(to_tree_leaf);

					ws_tree_node_leaf_struct_type *to_tree_leaf_from = static_cast<ws_tree_node_leaf_struct_type*>(to_tree_leaf);
					to_tree_leaf_from->internal_links_from.push_back(tree_leaf_copy);
				}

				for (size_t i = 0; i < tree_leaf_to_copy->external_links_to.size(); i++) {// Go through each link from this tree_leaf tree node to that this is from another tree leaf node
					std::filesystem::path to_tree_leaf_external = tree_leaf_to_copy->external_links_to[i];
					tree_leaf_copy->external_links_to.push_back(to_tree_leaf_external);
				}

				branch->leaves.push_back(tree_leaf_copy);

				std::filesystem::copy(tree_leaf_to_copy->item_path, tree_leaf_copy->item_path);// Make a copy of file that tree leaf represents; Uncomment when ready to implement

				break;
			}
		}

		return true;
	}

	std::filesystem::path create_new_tree_leaf_item_pathname(ws_tree_node_leaf_struct_type *tree_leaf_to_copy,ws_tree_class *branch) {
		std::filesystem::path tree_leaf_copy_path_parent = tree_leaf_to_copy->item_path.parent_path().c_str();
		std::filesystem::path tree_leaf_to_copy_filename = tree_leaf_to_copy->item_path.filename().c_str();

		std::string tree_leaf_copy_filename_s           = tree_leaf_to_copy_filename.string().c_str();
		std::string tree_leaf_copy_filename_extension_s = tree_leaf_to_copy_filename.extension().string().c_str();

		int end_pos = tree_leaf_copy_filename_s.rfind(tree_leaf_copy_filename_extension_s);

		std::string copy_filename_prefix = tree_leaf_copy_filename_s.substr(0,end_pos);

		std::string copy_filename = copy_filename_prefix + "_copy" + tree_leaf_copy_filename_extension_s;
		std::string tree_leaf_copy_pathename_s = tree_leaf_copy_path_parent.string() + "/" + copy_filename;

		if(!std::filesystem::exists(tree_leaf_copy_pathename_s) && find_tree_leaf(tree_leaf_copy_pathename_s, branch) == -1)
		//if(find_tree_leaf(tree_leaf_copy_pathename_s, branch) == -1) // For testing only : comment out or delete when testing complete
			std::filesystem::path tree_leaf_copy_filename = copy_filename;
		else {
			int i = 0;
			while (std::filesystem::exists(tree_leaf_copy_pathename_s) && find_tree_leaf(tree_leaf_copy_pathename_s, branch) > -1) {
			//while (find_tree_leaf(tree_leaf_copy_pathename_s, branch) > -1) {// For testing only : comment out or delete when testing complete
				std::string copy_filename = copy_filename_prefix + "_copy" + std::to_string(i) + tree_leaf_copy_filename_extension_s;
				tree_leaf_copy_pathename_s = tree_leaf_copy_path_parent.string() + "/" + copy_filename;
				i += 1;
			}
		}

		std::filesystem::path tree_leaf_to_copy_pathname = tree_leaf_copy_pathename_s;

		return tree_leaf_to_copy_pathname;
	}


	// --------------------------------------------------------------------------------------------------------------------------------

	bool is_web_page(std::filesystem::path file_path) {
		std::string extension = file_path.extension().string();

		if (FW::stringtools::contains(extension, "htm")) return true;

		return false;
	}

	bool is_web_page(ws_tree_node_item_struct_type *leaf) {
		return is_web_page(leaf->item_path);
	}

	afw_file_item_category_type_enum get_item_category_type(std::string extension) {
		extension = FW::stringtools::toLowerCase(extension);

		for (std::string extension_i : image_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 000 :  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::image;
			}
		}

		for (std::string extension_i : html_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 111  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::page;
			}
		}

		for (std::string extension_i : text_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 222  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::pageformat;
			}
		}

		for (std::string extension_i : script_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 000 :  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::script;
			}
		}

		for (std::string extension_i : media_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 333 :  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::media;
			}
		}

		for (std::string extension_i : audio_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 444 :  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::audio;
			}
		}

		for (std::string extension_i : pdf_extensions) {
			if (FW::stringtools::contains(extension, extension_i)) {
				//printf("get_item_category_type 444 :  %s \n", extension.c_str());
				return afw_file_item_category_type_enum::pdf;
			}
		}

		return afw_file_item_category_type_enum::undefined;
	}

protected:

private:
	ws_html_manager_class ws_html_manager;

};
