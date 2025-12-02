#pragma once

//#include <filesystem>
#include <fstream>
//#include <string>
//#include <vector>

#include <Framework/AFW/Tools/afw_file_tools.h>
#include <Framework/AFW/Tools/afw_string_tools.h>

#include "file_path_tools.h"

#define OVER_WRITE_FUNCTION_FILE    100
#define READ_FUNCTION_FILE          101

#define HTML_LINK_HREF				"href"
#define HTML_LINK_SRC				"src"

#define HTML_LINK_ANCHOR_BEGIN		"<a"
#define HTML_LINK_ANCHOR_END		"a>"

class ws_html_manager_class {
public:
	ws_html_manager_class() {};
	~ws_html_manager_class() {};

	std::vector<std::filesystem::path> internal_links_from = {};
	std::vector<std::filesystem::path> internal_links_to = {};
	std::vector<std::filesystem::path> external_links_to = {};

	int  open_file_stream(std::filesystem::path filename, int write_mode) {
		if (write_mode == OVER_WRITE_FUNCTION_FILE) {
			stream.open(filename, std::ios::out | std::ios::out);

			if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}
	
	void close_file_stream() {
		stream.close();
	}

	bool open_file_to_read(std::filesystem::path filename) {
		stream.open(filename, std::ios::in);

		if (!stream)
			return false;
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void clear_file_buffer() {
		lines.clear();
		lines.shrink_to_fit();
	}

	bool define_html_file_links(std::filesystem::path file_pathname, std::filesystem::path root_pathname) {
//std::cout << "define_html_file_links : " + file_pathname.string() +"\n";

		if (file_pathname.empty() || !std::filesystem::exists(file_pathname)) {
//std::cout << "define_html_file_links : No file name defined or filepathname does not exist\n";
			return false;
		}

		//if(file_pathname.extension() != ".html")
//std::cout << "define_html_file_links 02\n";
		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
//std::cout << "define_html_file_links: working_model_file\n";
			return false;
		}

//std::cout << "define_html_file_links 03\n";
		std::string working_model_string = FW::filetools::read_all(working_model_file);
//std::cout << "define_html_file_links 03\n";
		lines = FW::stringtools::split(working_model_string, '\n');
//std::cout << "define_html_file_links 04\n";		
		line_number = 0;

//std::cout << "define_html_file_links 05\n";
		define_html_links(root_pathname, file_pathname);
//std::cout << "define_html_file_links 06\n";
		working_model_file.close();
		return true;
	}

	bool define_html_links(std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		std::string line;
		std::string link = "";

		while (line_number < lines.size()) {
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			scan_for_href_links(line, root_pathname, file_pathname);
			scan_for_src_links(line, root_pathname, file_pathname);

			line_number++;
		}

		return true;
	}

	bool scan_for_href_links(std::string line, std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		int start_pos = 0;
		int end_pos = 0;
		std::string link = "";

//std::cout << "scan_for_href_links 01" << line << "\n";
		// ######### href link #######
		int link_pos = line.find(HTML_LINK_HREF);// Find string position of href flag given by text defined by HTML_LINK_HREF

		if (link_pos == std::string::npos) return false;// href link flag not found

		start_pos = line.find("\"", link_pos)+1;// Find string position of first "
		if (start_pos == std::string::npos) return false;
		end_pos = line.find("\"", start_pos);// Find string position of next "
		if (end_pos == std::string::npos) return false;

//std::cout << "scan_for_href_links 02" << link_pos << ":" << start_pos << ":" << end_pos << "\n";
		link = line.substr(start_pos, end_pos - start_pos);// extract substring between quotes in the html line that gives link path name 
//std::cout << "scan_for_href_links 03" << link << "\n";		
		if (is_external_link(link, root_pathname, file_pathname)) {
			// Avoid duplicate references
			bool have_duplicate = false;
			for (std::filesystem::path external_links_to_path : external_links_to) {
				if (external_links_to_path.string() == link) {
					have_duplicate = true;
					break;
				}
			}

			if (!have_duplicate) {
				external_links_to.push_back(link);
			}
		} else {
			if (!is_self_section_page_link(link)) {// Have a link to a web site html or web page file
				// Avoid duplicate references
				bool have_duplicate = false;
				for (std::filesystem::path internal_links_to_path : internal_links_to) {
					if (internal_links_to_path.string() == link) {
						have_duplicate = true;
						break;
					}
				}

				if (!have_duplicate) {
					internal_links_to.push_back(link);
				}
			}
		}
			// other link types here
		// ##########################

		std::string new_line = line.substr(end_pos + 1);
		
		if(!new_line.empty()) scan_for_href_links(new_line, root_pathname, file_pathname);

		return true;
	}

	bool scan_for_src_links(std::string line, std::filesystem::path root_pathname,std::filesystem::path file_pathname) {
		int start_pos = 0;
		int end_pos = 0;
		std::string link = "";

		// ######### src link #######
		int link_pos = line.find(HTML_LINK_SRC);

		if (link_pos == std::string::npos) return false;

		start_pos = line.find("\"", link_pos)+1;
		if (start_pos == std::string::npos) return false;
		end_pos = line.find("\"", start_pos);
		if (end_pos == std::string::npos) return false;

		link = line.substr(start_pos, end_pos - start_pos);
//std::cout << "scan_for_src_links 000: " + link +"\n";

		if (is_external_link(link, root_pathname, file_pathname)) {
//std::cout << "scan_for_src_links 111 is_external_link  : " << file_pathname << " : "  <<  link  << "\n";
			// Avoid duplicate references
			bool have_duplicate = false;
			for (std::filesystem::path external_links_to_path : external_links_to) {
				if (external_links_to_path.string() == link) {
					have_duplicate = true;
					break;
				}
			}

			if (!have_duplicate) {
				external_links_to.push_back(link);
			}
		} else {
			if (!is_self_section_page_link(link)) {
//std::cout << "scan_for_src_links 222 !is_self_section_page_link : " << file_pathname << " : " << link << "\n";
				// Avoid duplicate references
				bool have_duplicate = false;
				for (std::filesystem::path internal_links_to_path : internal_links_to) {
					if (internal_links_to_path.string() == link) {
						have_duplicate = true;
						break;
					}
				}

				if (!have_duplicate) {
//std::cout << "scan_for_src_links 333 !is_self_section_page_link : " << file_pathname << " : " << link << "\n";
					internal_links_to.push_back(link);
				}
			}
		}

			// other link types here

		// ##########################

		std::string new_line = line.substr(end_pos + 1);

		if (!new_line.empty()) scan_for_href_links(new_line, root_pathname, file_pathname);

		return true;
	}

	bool is_self_section_page_link(std::string link) { // Test if link is a link to a section of current html page
		if (FW::stringtools::contains(link, "#")) {
			int start_pos = link.find("#");
			if (start_pos == 0) return true;
		}

		// May need above test modified or additional ones placed here
		
		return false;
	}

	bool define_html_buffer(std::filesystem::path new_web_page_file_pathname) {
std::cout << "define_html_buffer 00 : " << new_web_page_file_pathname.string() << "\n";

		lines.clear();

		if (new_web_page_file_pathname.empty() || !std::filesystem::exists(new_web_page_file_pathname)) {
			std::cout << "define_html_buffer 01: No file name defined or filepathname does not exist\n";
			return false;
		}

//std::cout << "update_html_file_links 02\n";
		std::fstream working_model_file(new_web_page_file_pathname, std::ios::in);

		if (!working_model_file) {
			std::cout << "define_html_buffer 02A: !working_model_file\n";
			return false;
		}

//std::cout << "update_html_file_links 03\n";
		std::string working_model_string = FW::filetools::read_all(working_model_file);
//std::cout << "update_html_file_links 03\n";
		lines = FW::stringtools::split(working_model_string, '\n');
//std::cout << "update_html_file_links 04\n";		
		line_number = 0;

		working_model_file.close();

		return true;
	}


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool update_html_file_links(std::filesystem::path previous_web_page_file_pathname, std::filesystem::path current_web_page_file_pathname, std::filesystem::path root_pathname, std::filesystem::path link_item_path) {
std::cout << "update_html_file_links 00 : " << previous_web_page_file_pathname.string() << " : " << current_web_page_file_pathname.string() << "\n";
std::cout << "update_html_file_links 01 : " << link_item_path.string() << " : " << root_pathname.string() << "\n";

		if (!std::filesystem::exists(current_web_page_file_pathname)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "update_html_links : Cannot update web page file\n" + current_web_page_file_pathname.string() + "\nlinks. It does not exist\n");
			return false;
		}

		std::string line;

		std::string previous_link_relative_pathname_s = file_path_tools_class::get_relative_pathname( previous_web_page_file_pathname, link_item_path, root_pathname);
		std::string current_link_relative_pathname_s  = file_path_tools_class::get_relative_pathname( current_web_page_file_pathname, link_item_path, root_pathname);

std::cout << "update_html_file_links 02 : " << previous_link_relative_pathname_s << " : " << current_link_relative_pathname_s << " : " << lines.size() <<"\n";

		line_number = 0;
		while (line_number < lines.size()) {
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			update_line_href_links(line, root_pathname, previous_link_relative_pathname_s, current_link_relative_pathname_s);

			line_number++;
		}

		return true;

	}
	
	// NOTE :!!!!! NOT YET TESTED !!!!!!!!!!!!!!!!!!!!
	bool update_html_file_from_links(std::filesystem::path previous_web_page_file_pathname, std::filesystem::path current_web_page_file_pathname, std::filesystem::path root_pathname, std::filesystem::path link_item_path) {
std::cout << "update_html_file_from_links 00 : " << previous_web_page_file_pathname.string() << " : " << current_web_page_file_pathname.string() << "\n";
std::cout << "update_html_file_from_links 01 : " << link_item_path.string() << " : " << root_pathname.string() << "\n";

		if (!std::filesystem::exists(current_web_page_file_pathname)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "update_html_file_from_links : Cannot update web page file\n" + current_web_page_file_pathname.string() + "\nlinks. It does not exist\n");
			return false;
		}

		if (!std::filesystem::exists(link_item_path)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "update_html_file_from_links : File pathname \n" + link_item_path.string() + "\n does not exist to save html updates to\n");
			return false;
		}

		if (is_web_page(link_item_path)) { // Have a selection tree leaf node that is a web page)

			if (!define_html_buffer(link_item_path)) {
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "update_html_file_from_links : Cannot define web page file\n" + link_item_path.string() + "\nto update\n");
				return false;
			}

			std::string line;

			std::string previous_link_relative_pathname_s = file_path_tools_class::get_relative_pathname(link_item_path, previous_web_page_file_pathname,  root_pathname);
			std::string current_link_relative_pathname_s  = file_path_tools_class::get_relative_pathname(link_item_path, current_web_page_file_pathname,  root_pathname);

std::cout << "update_html_file_from_links 02 : " << previous_link_relative_pathname_s << " : " << current_link_relative_pathname_s << " : " << lines.size() << "\n";

			line_number = 0;
			while (line_number < lines.size()) {
				line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
				update_line_href_links(line, root_pathname, previous_link_relative_pathname_s, current_link_relative_pathname_s);

				line_number++;
			}

			save_html_file_updates(link_item_path); // Uncomment when testing completed
		}

		return true;
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// #########################################################################################################

	bool save_html_file_updates(std::filesystem::path html_file_pathname) {
		if (html_file_pathname.empty() ) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "save_html_file_updates 00: No file name defined to save html updates to\n");
			return false;
		}

		if (!std::filesystem::exists(html_file_pathname)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "save_html_file_updates 00: File pathname \n" + html_file_pathname.string() + "\n does not exist to save html updates to\n");
			return false;
		}

		std::fstream html_output_file(html_file_pathname, std::ios::out | std::ios::trunc);//Open html file to overwrite existing contents

		if (!html_output_file.is_open()) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "save_html_file_updates 01 :: File\n" + html_file_pathname.string() + "\ncould not be opened to save html updates to.\n");
			return false;
		}

//std::cout << "save_html_file_updates 00 :" << html_file_pathname.string() << " : " << lines.size() << "\n";
		for (int i = 0; i < lines.size(); i++) {
//std::cout << "save_html_file_updates line :" << i << ": " << lines[i] << "\n";
			html_output_file << lines[i] << "\n";
		}

		html_output_file.close();
	}

	bool update_line_href_links(std::string line, std::filesystem::path root_pathname, std::string old_link_to_find, std::string new_link_to_change_to) {
		if (line.find(old_link_to_find) == std::string::npos) return false; // Line does not contain link to replace

		// ######### href link #######
		if (old_link_to_find == new_link_to_change_to) return false; // new link to change to allready applied +++++++

std::cout << "update_line_href_links : before " << line_number << " : " << line << "\n";

		FW::stringtools::replaceInPlace(line, old_link_to_find, new_link_to_change_to);

std::cout << "update_line_href_links : After " << line_number << " : " << line << "\n";

		lines[line_number] = line;

std::cout << "update_line_href_links : lines[line_number] " << line_number << " : " << lines[line_number] << "\n";

		return true;
	}

	// #########################################################################################################
	bool update_web_page_file_link(std::filesystem::path root_pathname,std::filesystem::path web_page_path_to_update, std::filesystem::path old_file_pathname, std::filesystem::path new_file_pathname) {
		if (!std::filesystem::exists(web_page_path_to_update)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "update_web_page_file_link : Cannot update web page file\n" + web_page_path_to_update.string() + "\nlinks. It does not exist\n");
			return false;
		}

		if (!define_html_buffer(web_page_path_to_update)) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "update_web_page_file_link : Cannot define web page file\n" + web_page_path_to_update.string() + "\nto update\n");
			return false;
		}

		std::string line;

		// Necessary to assign std::filesystem::path string as a c_str() as it can still retain unused capacity that is output as nulls when writing to a file
		// An old c leagcy thing exists in C++ ????
		std::string old_link_absolute_pathname = old_file_pathname.string().c_str();
		std::string new_link_absolute_pathname = new_file_pathname.string().c_str();

		std::string old_link_relative_pathname = "";
		std::string new_link_relative_pathname = "";

		if (is_internal_link(old_link_absolute_pathname, root_pathname, web_page_path_to_update)) {
			if (is_relative_link(old_link_absolute_pathname, root_pathname, web_page_path_to_update)) {
				old_link_absolute_pathname = file_path_tools_class::get_absolute_pathname(old_link_absolute_pathname, web_page_path_to_update);
			} else {
				old_link_absolute_pathname = old_file_pathname.string().c_str();
			}
		} else {
//std::cout << "update_web_page_file_link 02A old_link_to_find !is_internal_link !\n";	
			return false;
		}

//std::cout << "update_web_page_file_link 02 old_link_to_find absolute " << old_link_absolute_pathname << "\n";

		old_link_relative_pathname = file_path_tools_class::get_relative_pathname(web_page_path_to_update,old_link_absolute_pathname, root_pathname);
		new_link_relative_pathname = file_path_tools_class::get_relative_pathname(web_page_path_to_update,new_link_absolute_pathname, root_pathname);

//std::cout << "update_web_page_file_link 04 old_link_relative_pathname " << old_link_relative_pathname << "\n";
//std::cout << "update_web_page_file_link 05 new_link_relative_pathname " << new_link_relative_pathname << "\n";

		while (line_number < lines.size()) {
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			update_line_href_links(line, root_pathname, old_link_relative_pathname, new_link_relative_pathname);

			line_number++;
		}

//std::cout << "update_web_page_file_link 06 web_page_path_to_update %s " << web_page_path_to_update.string() << "\n";
		save_html_file_updates(web_page_path_to_update); // Uncomment when testing completed

		return true;
	}

	void display_html_buffer() {
		int line_numberB = 0;
		while (line_numberB < lines.size()) {
			std::cout << "display_html_buffer : " << line_numberB  << " : " << lines[line_numberB] << "\n";
			line_numberB++;
		}
	}

	bool is_relative_link(std::string link, std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		if (FW::stringtools::contains(link, root_pathname.string()) || FW::stringtools::contains(link, ":")) return false;

		return true;
	}

	bool is_external_link(std::string link, std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		for (int j = 0; j < external_link_types.size(); j++) {
			if (FW::stringtools::contains(link, external_link_types[j])) {
				return true;
			}
		}

		if (FW::stringtools::contains(link, ":")) {						 // Have a link with an absolute file pathname 
			if (!FW::stringtools::contains(link, root_pathname.string())) // If pathname does not have the html file web project root pathname have external link
				return true;
		}

		// !!!!!!!!!!!!!!!!!! This needs to be tested !!!!!!!!!!!!!!!!!!!!
		if (FW::stringtools::contains(link, "..")) {// Have a link with a relative file pathname at least one directory level above
			std::string t_path = file_pathname.string();

			// May need function to remove any padding to be sure
			int last_pos = link.rfind("..") + 3;
			int number_directory_levels = last_pos / 3;

			//FW::stringtools::replaceInPlace(t_path, "/", "|");
			FW::stringtools::replaceInPlace(t_path, "\\", "/");

			int d_pos = t_path.rfind("/");
			t_path.resize(d_pos + 1); // Directory path that file exists in
			for (int i = 0; i < number_directory_levels; i++) {
				int d_pos = t_path.rfind("/");
				t_path.resize(d_pos + 1);
			}

			if (!FW::stringtools::contains(t_path, root_pathname.string())) // If pathname does not have the html file web project root pathname have external link
				return true;
		}
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		return false;
	}

	bool is_internal_link(std::string link, std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		return !is_external_link(link, root_pathname, file_pathname);
	}

	bool is_web_page(std::filesystem::path file_path) {
		std::string extension = file_path.extension().string();

		if (FW::stringtools::contains(extension, "htm")) return true;

		return false;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool define_html_file_buffer(std::filesystem::path file_pathname) {
		if (!lines.empty()) return true; //Buffer already defined

//std::cout << "define_html_file_buffer : " + file_pathname.string() +"\n";

		if (file_pathname.empty() || !std::filesystem::exists(file_pathname)) {
//std::cout << "define_html_file_buffer : No file name defined or filepathname does not exist\n";
			return false;
		}

		//if(file_pathname.extension() != ".html")
//std::cout << "define_html_file_buffer 02\n";
		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
//std::cout << "define_html_file_buffer: working_model_file\n";
			return false;
		}

//std::cout << "define_html_file_buffer 03\n";
		std::string working_model_string = FW::filetools::read_all(working_model_file);
//std::cout << "define_html_file_buffer 03\n";
		lines = FW::stringtools::split(working_model_string, '\n');

		working_model_file.close();


		return true;
	}


	bool clear_html_links(std::filesystem::path file_pathname, bool external_links_only = false) {
//std::cout << "clear html imternal links 00\n";
		if (!define_html_file_buffer(file_pathname)) return false;

		line_number = 0;

//std::cout << "clear html imternal links 11\n";

		remove_html_links(external_links_only);

//std::cout << "clear html internal links 22\n";

		save_html_file_updates(file_pathname);
//std::cout << "clear html internal links 33\n";

		clear_file_buffer();

		return true;
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool remove_html_links(bool external_links=false){// Asumes no links are part of a comment block :: Note future need to accomadate this possibility
		std::string line;

		while (line_number < lines.size()) {
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());

//std::cout << "define_html_links 01" << lines[line_number] << "\n";

			if (FW::stringtools::contains(lines[line_number], HTML_LINK_ANCHOR_BEGIN)) { // update all these statements in c23 to lines[line_number].contains(HTML_LINK_ANCHOR_BEGIN) etc
				link_block_start_line = line_number;
				link_block_start_line_pos = lines[line_number].find(HTML_LINK_ANCHOR_BEGIN);

				if (FW::stringtools::contains(lines[line_number], HTML_LINK_ANCHOR_END)) {
					link_block_end_line = line_number;
					link_block_end_line_pos = lines[line_number].find(HTML_LINK_ANCHOR_END);
				}
				else {
					line_number++;
					if (!find_link_block_end()) return false;// Use if want to find html errors
				}
//std::cout << "define_html_links"<< " start anchor <a found at line " + std::to_string(line_number) + " of file " + html_file.string() + "\n";
				if(external_links) 
					define_external_link_to_delete();
				else
					define_link_to_delete();

//std::cout << "define_html_links AAA\n";
			}

			line_number++;
		}

		return true;
	}

	bool find_link_block_end() {

		std::string line;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while(!FW::stringtools::contains(lines[line_number], HTML_LINK_ANCHOR_END) && line_number < lines.size()){
			line_number++;
		}

		if (line_number >= lines.size() || !FW::stringtools::contains(lines[line_number], HTML_LINK_ANCHOR_END)) return false;

		link_block_end_line	 = line_number;
		link_block_end_line_pos = lines[line_number].find(HTML_LINK_ANCHOR_END);
		return true;
	}

	void define_link_to_delete() {
//std::cout <<"define_link01 : " << link_block_start_line << "::" << link_block_end_line << "\n";

		// This assumes only one  link is available per html anchor block
		// If not need to modify to take account of more than one  link
		for (int i = link_block_start_line; i <= link_block_end_line; i++) {
			lines[i] = remove_line_links(lines[i], 0);
		}
	}

	// following Seems to work except that deleting links is not a tidy process and junk html can be left behinde  
	std::string remove_line_links(std::string line, int start_loc) {
		std::string _link = "";
		std::string edited_line = line;

//std::cout << "remove_line_links 00 : " + edited_line + " : " + std::to_string(start_loc) + "\n";

			if (start_loc < edited_line.size()) {
				int start_pos = edited_line.find("href", start_loc);
				int end_pos   = edited_line.find(">", start_pos);

//std::cout << "remove_line_links 11 : " + edited_line + " : " + std::to_string(start_pos) + " : " + std::to_string(end_pos) + " : \n";

				if (start_pos != std::string::npos && end_pos != std::string::npos) { // Assumes all links have to be defined on the same line

					_link = edited_line.substr(start_pos - 1, end_pos - start_pos + 1);

//std::cout << "remove_line_links 44 before delete link: " + edited_line + " : " + _link + " : " + "\n";
					edited_line = FW::stringtools::replace(edited_line, _link, " ");
//std::cout << "remove_line_links 55 after delete link: " + edited_line + " : " + _link + " : " + "\n";
				} else 
					start_pos = end_pos;

//std::cout << "remove_line_links 66 : " + edited_line + " : " + std::to_string(start_pos) + " : " + "\n";

				if (start_pos < edited_line.size())
					edited_line = remove_line_links(edited_line, start_pos); // ensure to delete multiple  links on the same html line
			}

//std::cout << "remove_line_links 77 : " + edited_line  + "\n";

		return edited_line;
	}


	void define_external_link_to_delete() {
//std::cout <<"define_external_link01 : " << link_block_start_line << "::" << link_block_end_line << "\n";

		// This assumes only one external link is available per html anchor block
		// If not need to modify to take account of more than one external link
		for (int i = link_block_start_line; i <= link_block_end_line; i++) {
			for (int j = 0; j < external_link_types.size(); j++) {
				if (FW::stringtools::contains(lines[i], external_link_types[j])) {
//std::cout <<"define_external_link02 : " << lines[i] << ":" << external_link_types[j] << "\n";

					//int start_pos = lines[i].find("href");
					//int end_pos = lines[i].find(">", start_pos);

					//external_link = lines[i].substr(start_pos-1, end_pos-start_pos+1);
	
std::cout <<"define_external_link03 before delete link: " + lines[i] + "\n";
//std::cout <<"define_external_link03 before delete link: " + lines[i]+ " : " + external_link + " : " + "\n";
					//lines[i] = FW::stringtools::replace(lines[i], external_link," ");
					lines[i] = remove_line_external_links(lines[i],0, external_link_types[j]);
std::cout <<"define_external_link03 after delete link: " + lines[i] + "\n";
//std::cout <<"define_external_link03 after delete link: " + lines[i] + " : " + external_link + " : " + "\n";

					break;
				}
			}
			//if (external_link.length() > 0) break;
		}
	}

	// following Seems to work except that deleting links is not a tidy process and junk html can be left behinde  
	std::string remove_line_external_links(std::string line, int start_loc, std::string external_link_type) {
		std::string external_link = "";
		std::string edited_line = line;

//std::cout << "remove_line_external_links 00 : " + edited_line + " : " + std::to_string(start_loc) + " : " + external_link_type +" : ""\n";

		if (FW::stringtools::contains(edited_line, external_link_type)) {

			if (start_loc < edited_line.size()) {
				int start_pos = edited_line.find("href", start_loc);
				int end_pos = edited_line.find(">", start_pos);

//std::cout << "remove_line_external_links 11 : " + edited_line + " : " + std::to_string(start_pos) + " : " + std::to_string(end_pos) + " : " + external_link_type +" : ""\n";

				int external_link_pos = edited_line.find(external_link_type, start_pos);

//std::cout << "remove_line_external_links 22 :" + std::to_string(external_link_pos) +"\n";


				if (external_link_pos > start_pos && external_link_pos < end_pos) {

//std::cout << "remove_line_external_links 33 :" + std::to_string(external_link_pos) +"\n";

					external_link = edited_line.substr(start_pos - 1, end_pos - start_pos + 1);

//std::cout << "remove_line_external_links 44 before delete link: " + edited_line + " : " + external_link + " : " + "\n";
					edited_line = FW::stringtools::replace(edited_line, external_link, " ");
//std::cout << "remove_line_external_links 55 after delete link: " + edited_line + " : " + external_link + " : " + "\n";
				}
				else start_pos = end_pos;

//std::cout << "remove_line_external_links 66 : " + edited_line + " : " + std::to_string(start_pos) + " : " + "\n";

				if (start_pos < edited_line.size())
					edited_line = remove_line_external_links(edited_line, start_pos, external_link_type); // ensure to delete multiple external links on the same html line
			}
		}

//std::cout << "remove_line_external_links 77 : " + edited_line  + "\n";

		return edited_line;
	}


protected:
	int link_block_start_line     = -1;
	int link_block_start_line_pos = -1;
	int link_block_end_line	   = -1;
	int link_block_end_line_pos   = -1;

private:
	std::fstream stream;

	std::vector<std::string> external_link_types = { "https","ftp","tel","mailto","sms" };

	std::vector<std::string> lines = {};

	int line_number = 0;

	//int error_code = 0;
};