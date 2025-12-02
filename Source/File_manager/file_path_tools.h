#pragma once

#include <filesystem>

#include <string>
#include <vector>

#include <Framework/AFW/Tools/afw_string_tools.h>

class file_path_tools_class {
public:
	file_path_tools_class() {}
	~file_path_tools_class() {}

	// Get relative pathname of file located at absolute_file_to_pathname from file located at absolute_origin_from_pathname
	// Critical pre-requisite is that absolute_file_to_pathname and absolute_origin_from_pathname have as stated an absolute file pathname to each file
	static std::string get_relative_pathname(std::filesystem::path absolute_origin_from_pathname, std::filesystem::path absolute_file_to_pathname, std::filesystem::path root_pathname) {
		std::string root_pathname_s                 = root_pathname.string().c_str();
		std::string absolute_origin_from_pathname_s = absolute_origin_from_pathname.parent_path().string().c_str();
		std::string absolute_file_to_pathname_s		= absolute_file_to_pathname.parent_path().string().c_str();
		std::string relative_path					= "";
	
		FW::stringtools::replaceInPlace(root_pathname_s, "\\", "/");
		FW::stringtools::replaceInPlace(absolute_origin_from_pathname_s, "\\", "/");
		FW::stringtools::replaceInPlace(absolute_file_to_pathname_s, "\\", "/");

		std::vector<std::string> origin_from_path_segments;
		std::vector<std::string> file_to_path_segments;

		if (absolute_origin_from_pathname_s.find(root_pathname_s) != std::string::npos) {
//std::cout << "get_relative_pathname 01AAA : web_page_file_pathname_st " << " :" << root_pathname_s << ":\n";
			replacefirst(absolute_origin_from_pathname_s, root_pathname_s, "");
		}
//std::cout << "get_relative_pathname 02 : after absolute_source_pathname_s " << absolute_source_pathname_s << "\n";

//std::cout << "get_relative_pathname 03 : before web_page_file_pathname_s " << web_page_file_pathname_s << " :" << root_pathname_s << ":\n";

		if (absolute_file_to_pathname_s.find(root_pathname_s) != std::string::npos) {
//std::cout << "get_relative_pathname 03AAA : web_page_file_pathname_s " << web_page_file_pathname_s << " :" << root_pathname_s << ":\n";
			replacefirst(absolute_file_to_pathname_s, root_pathname_s, "");
		}
//std::cout << "get_relative_pathname 04 : after absolute_destination_pathname_s " << absolute_destination_pathname_s << "\n";

		origin_from_path_segments = FW::stringtools::split(absolute_origin_from_pathname_s, '/');
		file_to_path_segments = FW::stringtools::split(absolute_file_to_pathname_s, '/');

		// ++++++++++++++
		// Take account of common path from root path :: NEED TO KEEP AN EYE AND TEST ALL POSSIBLE VARIATIONS OF PATHNAMES
		int start_index = 0;// Because first segment is a null string : ie of value "" : Need to keep an eye that this does not change
		for (int k = 1; k < origin_from_path_segments.size(); k++) {
			if (k < file_to_path_segments.size()) {
std::cout << "get_relative_pathname 04AAA : relative_path :" << origin_from_path_segments[k] << ":" << file_to_path_segments[k] << ": "  << std::to_string(k) << " : " << std::to_string(start_index) << "\n";
				if (origin_from_path_segments[k] == file_to_path_segments[k]) {
					start_index++;
				} else
					break;
			}
		}
std::cout << "get_relative_pathname 04BBB : relative_path :" << std::to_string(start_index) << "\n";
		// ++++++++++++++

		int relative_number = 1;// Because first segment is a null string : ie of value "" : Need to keep an eye that this does not change
		for (int i = origin_from_path_segments.size() - 1; i > start_index; i--) {// +++++
		//for (int i = origin_from_path_segments.size() - 1; i > 0; i--) {
			if (relative_number < file_to_path_segments.size()) {
std::cout << "get_relative_pathname 05 : relative_path :" << origin_from_path_segments[i] << ":" << file_to_path_segments[relative_number] << ": "  << std::to_string(i) << " : " << std::to_string(relative_number) << "\n";
				if (origin_from_path_segments[i] != file_to_path_segments[relative_number]) {
					relative_path += "../";
std::cout << "get_relative_pathname 05AAA : relative_path " << relative_path << "\n";
				}
				else {
					relative_number += 1;
				}
			}
		}

		//for (int j = relative_number; j < file_to_path_segments.size(); j++) {
		for (int j = relative_number+ start_index; j < file_to_path_segments.size(); j++) {// +++++
			relative_path += file_to_path_segments[j] + "/";
std::cout << "get_relative_pathname 06 : relative_path " << relative_path << "\n";
		}

		relative_path += absolute_file_to_pathname.filename().string();

std::cout << "get_relative_pathname 07 : relative_path " << relative_path << "\n";
		return relative_path;

	}

	// get absolute pathname of a link that exists within the web page file of pathname web_page_file_pathname
	// Need to move this to a class that handles such function that can be included
	// Seems to work but need to keep an eye on it as std::string and std::filesystem::path have incosistentcies 
	// of string handling with other string functions
	static std::string get_absolute_pathname(std::string relative_path, std::filesystem::path web_page_file_pathname) {
		std::string web_page_file_pathname_s = web_page_file_pathname.string().c_str();
		std::string absolute_path = "";

		FW::stringtools::replaceInPlace(relative_path, "\\", "/");
		FW::stringtools::replaceInPlace(web_page_file_pathname_s, "\\", "/");

		std::vector<std::string> relative_link_path_segments;
		relative_link_path_segments = FW::stringtools::split(relative_path, '/');

		std::vector<std::string>  web_page_file_path_segments;
		web_page_file_path_segments = FW::stringtools::split(web_page_file_pathname_s, '/');

		//int pos = web_page_file_pathname_s.rfind("/");
		//web_page_file_pathname_s.resize(pos);

		int end_web_page_segment			 = web_page_file_path_segments.size()-1;
		int start_relative_link_path_segment = 0;

		for (int i = 0; i < relative_link_path_segments.size(); i++) {
			if (relative_link_path_segments[i] == "..") {
				if (end_web_page_segment == 0) {
				//if (web_page_file_pathname_s.size() == 0) {
					// Have an error incorect web_page_file_pathname
				}
				end_web_page_segment -= 1;
				start_relative_link_path_segment += 1;
				//int pos = web_page_file_pathname_s.rfind("/");
				//web_page_file_pathname_s.resize(pos);
			}
		}

		for (int j = 0; j < end_web_page_segment; j++) {
			absolute_path.append(web_page_file_path_segments[j].c_str());
			absolute_path.append("/");
		}

		for (int k = start_relative_link_path_segment; k < relative_link_path_segments.size(); k++) {
			absolute_path.append(relative_link_path_segments[k].c_str());
			if(k< relative_link_path_segments.size()-1) absolute_path.append("/");
		}

//std::cout << "get_relative_pathname 06 : " << web_page_file_pathname_s << " : " << relative_path << " : " << absolute_path << "\n";

		//absolute_path = web_page_file_pathname_s + "/" + relative_path;

		return absolute_path;
	}

	static std::string get_file_extension(std::filesystem::path tree_node) {
		return  tree_node.extension().string();
	}

	static bool is_absolute_path(std::string path) {

		if (FW::stringtools::contains(path, ":")) { // Have a path with an absolute file pathname 
			return true;
		}
		return false;
	}

	static bool is_absolute_path(std::filesystem::path path) {
		return is_absolute_path(path.string());
	}

	static bool is_relative_path(std::filesystem::path file_pathname, std::filesystem::path root_pathname) {
		if (FW::stringtools::contains(file_pathname.string(), root_pathname.string()) || FW::stringtools::contains(file_pathname.string(), ":")) return false;

		return true;
	}

	bool is_external_path(std::string path, std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		if (FW::stringtools::contains(path, ":")) {						  // Have a link with an absolute file pathname 
			if (!FW::stringtools::contains(path, root_pathname.string())) // If pathname does not have the html file web project root pathname have external link
				return true;
		}

		// !!!!!!!!!!!!!!!!!! This needs to be tested !!!!!!!!!!!!!!!!!!!!
		if (FW::stringtools::contains(path, "..")) {// Have a link with a relative file pathname at least one directory level above
			std::string t_path = file_pathname.string();

			// May need function to remove any padding to be sure
			int last_pos = path.rfind("..") + 3;
			int number_directory_levels = last_pos / 3;

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

	bool is_internal_path(std::string link, std::filesystem::path root_pathname, std::filesystem::path file_pathname) {
		return !is_external_path(link, root_pathname, file_pathname);
	}

protected:

	static bool replacefirst(std::string& source, const std::string& search, const std::string& replace) {
		size_t pos = source.find(search);

		if (pos == std::string::npos) return false;

		source.replace(pos, search.length(), replace);
		return true;
	}


private:
};