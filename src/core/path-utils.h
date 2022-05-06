#pragma once

#include <string>
#include <utility>

std::pair<std::string, std::string> split_path_in_last_dot(const std::string &path);

std::string get_extension(const std::string &path);
std::string get_path_without_extension(const std::string &path);

std::string replace_extension(const std::string &path, const std::string &new_ext);
std::string append_before_extension(const std::string &path, const char* appended);

std::string change_to_jpg_if_jpeg(const std::string &ext);

int ensure_parse_int(const std::string &input);