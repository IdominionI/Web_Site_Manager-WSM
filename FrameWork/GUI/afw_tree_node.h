#pragma once

#include <string>
//#include <vector>

//#include <imgui.h>

//#include <FrameWork/AFW/Common/afw_definitions.h>
//#include <FrameWork/AFW/Tools/afw_id_key_manager.h>

//#define TREE_NODE_ITEM_DIRECTORY      00
//#define TREE_NODE_ITEM_DIRECTORY_OPEN 01

//#define TREE_NODE_ITEM_TYPE_UNDEFINED -1
// 
//#define TREE_NODE_ITEM_TYPE_HTML       10
//#define TREE_NODE_ITEM_TYPE_PHP        11
// 
//#define TREE_NODE_ITEM_TYPE_JPG        20
//#define TREE_NODE_ITEM_TYPE_PNG        21
//#define TREE_NODE_ITEM_TYPE_GIF        22
//#define TREE_NODE_ITEM_TYPE_WEB        23
//#define TREE_NODE_ITEM_TYPE_TIF        24
//#define TREE_NODE_ITEM_TYPE_SVG        25
// 
//#define TREE_NODE_ITEM_TYPE_JS         30
//#define TREE_NODE_ITEM_TYPE_JAVE       31
//#define TREE_NODE_ITEM_TYPE_CLASS      32
// 
//#define TREE_NODE_ITEM_TYPE_MPEG       40
//#define TREE_NODE_ITEM_TYPE_FLT        41
//#define TREE_NODE_ITEM_TYPE_MOV        42
// 
//#define TREE_NODE_ITEM_TYPE_MP3        50

//#define WEB_LINK_TYPES_HTTP			100
//#define WEB_LINK_TYPES_HTTPS			101

enum class afw_file_item_category_type_enum{ directory,page,pageformat,image,script,media,audio,pdf,code,undefined};
//enum class afw_file_page_item_type_enum{html,htm,php,undefined};
//enum class afw_file_format_item_type_enum{css, undefined};
//enum class afw_file_image_item_type_enum{jpg,png,gif,web,tif,undefined};
//enum class afw_file_script_item_type_enum{js,jave,_class, undefined};
//enum class afw_file_media_item_type_enum{mpeg,flt,mov,mp3, undefined};

enum class afw_tree_node_item_type_enum {root_tree_node,tree_node, tree_leaf,undefined};

struct afw_tree_node_item_base_struct_type {
	afw_tree_node_item_type_enum tree_node_item_type = afw_tree_node_item_type_enum::undefined;

	std::string label   = " Item label";
	std::string icon_id = "";

	id_type tree_node_id = INVALID_ID;

	bool selected = false;
};
/*
template<class TNIDT>
class afw_tree_node_base_class {
public:
	afw_tree_node_base_class() {}
	~afw_tree_node_base_class() {}

	id_type id = INVALID_ID;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

	afw_tree_node_base_class *parent = nullptr;

	std::vector<TNIDT> items = {};

	virtual bool add_tree_node_item(TNIDT afw_tree_node_item) { return false; }
	virtual bool copy_tree_node_item(TNIDT afw_tree_node_item) { return false; }
	virtual bool delete_tree_node_item(TNIDT afw_tree_node_item) { return false; }
	virtual bool paste_tree_node_item(TNIDT afw_tree_node_item) { return false; }
	virtual bool move_tree_node_item(TNIDT afw_tree_node_item) { return false; }


	virtual bool rename_tree_node_item(TNIDT afw_tree_node_item) { return false; }
	
	virtual bool display_tree_node_item(TNIDT afw_tree_node_item) { return false; }

protected:

private:
	id_key_manager_class<id_type> tree_node_id_key;

};
*/
//template<class TNIDT>
//class afw_tree_base_class {
//public:
//	afw_tree_base_class() {};
//	~afw_tree_base_class() {};
//
//	//ImGuiTreeNodeFlags flags = ;
//
//	std::string tree_root_label = "Tree";
//	std::string window_label    = "Window Label";
//
//	afw_tree_node_base_class<TNIDT> tree;
//
//	virtual bool create_tree(std::string label) { tree_root_label = label; return true; }
//	virtual bool delete_tree() { return false; }
//
//	virtual bool display_root_tree() {
//		ImGui::Begin(window_label.c_str());// A ImGui main UI containner window to hold all other UI panels     
//			
//			if (ImGui::TreeNode(tree_root_label.c_str())) {
//				display_tree_nodes();
//
//				ImGui::TreePop();
//			}
//
//		ImGui::End();
//		return true;
//	}
//
//	virtual bool display_tree_nodes() {
//		ImGui::Text("Item A");
//		return false;
//	}
//
//protected:
//
//private:
//
//};