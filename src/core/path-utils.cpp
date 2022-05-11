#include "path-utils.h"

#include <string>
#include <utility>

#include <iostream>
#include <sstream>

std::pair<std::string, std::string> split_path_in_last_dot_or_exit(const std::string &path)
{
	using std::cerr; using std::stringstream; using std::string;

	size_t index_of_last_dot = path.find_last_of('.');
	if (index_of_last_dot == string::npos) {
		cerr << "[error] file at path \"" << path << "\" has no extension\n";
		exit(1);
	}

	std::string path_wo_ext;
	std::string ext;

	// parse path before the last dot
	stringstream ss;
	for (size_t i = 0; i < index_of_last_dot; ++i) {
		ss << path[i];
	}
	path_wo_ext = ss.str();

	// parse path after the last dot
	ss = stringstream();
	int path_len = path.length();
	for (int i = index_of_last_dot+1; i < path_len; ++i) {
		ss << path[i];
	}
	ext = ss.str();

	return std::pair<std::string, std::string>(path_wo_ext, ext);
}
// TODO: check if compiles with const& structured binding
std::string get_extension(const std::string &path)
{
	auto [_, extension] = split_path_in_last_dot_or_exit(path);
	return extension;
}

std::string get_path_without_extension(const std::string &path)
{
	auto [path_wo_ext, _] = split_path_in_last_dot_or_exit(path);
	return path_wo_ext;
}

std::string replace_extension(const std::string &path, const std::string &new_ext)
{
	std::string output = get_path_without_extension(path)
	.append(".")
	.append(new_ext);

	return output;
}

std::string append_before_extension(const std::string &path, const char* appended)
{
	auto [path_wo_extension, ext] = split_path_in_last_dot_or_exit(path);

	size_t replace_start = path_wo_extension.length();
	size_t replace_end = ext.length() + 1;
	std::string new_path_ending = std::string(appended).append(".").append(ext);
	std::string new_path = std::string(path).replace(replace_start, replace_end, new_path_ending);

	return new_path;
}

// utils
std::string change_to_jpg_if_jpeg(const std::string &ext)
{
	if (ext == "jpeg") {
		return "jpg";
	}
	else {
		return ext;
	}
}

int ensure_parse_int(const std::string &input)
{
	int output = 0;
	try {
		output = std::stoi(input);
	}
	catch (std::exception &exp) {
		std::cerr << "[error] couldn't parse \"" << input << "\" to integer number!\n";
		exit(1);
	}

	return output;
}
// utils
// TODO: compile only once