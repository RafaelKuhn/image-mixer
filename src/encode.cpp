#include <iostream>

#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "bmp.h"
#include "png-wrapper.h"
#include "jpeg-wrapper.h"

// TODO: compile only once
std::string append_before_extension(const std::string &path, const std::string &ext, const char* appended)
{
	size_t replace_start = path.length() - ext.length() - 1;
	size_t ext_size = sizeof(ext);
	std::string new_path_ending = std::string(appended).append(".").append(ext);
	
	std::string new_path = std::string(path).replace(replace_start, ext_size, new_path_ending);

	return new_path;
}

// TODO: compile only once
std::string get_extension(const std::string &path)
{
	using std::cout;
	using std::cerr; using std::stringstream; using std::string;

	size_t index_of_last_dot = path.find_last_of('.');
	if (index_of_last_dot == string::npos) {
		cerr << "[error] file at path \"" << path << "\" has no extension\n";
		exit(1);
	}

	stringstream ss;
	int path_len = path.length();
	for (int i = index_of_last_dot+1; i < path_len; ++i) {
		ss << path[i];
	}

	string ext = ss.str();

	return ext;
}

struct Settings {
	std::string image_path;
	std::string image_extension;
	std::string output_extension;
};

bool is_a_valid_extension(const std::string &ext)
{
	return (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "bmp");
	// bool is_extension_invalid = demix_functions_by_extension.find(ext) == demix_functions_by_extension.end();
	// if (is_extension_invalid) {
	// 	std::cerr << "[error] invalid extension: \"" << ext << "\"\nvalid extensions are: ";
	// 	for (auto const& [key, val] : demix_functions_by_extension) {
	// 		std::cerr << key << " ";
	// 	}
	// 	std::cerr << "\n";
	// 	exit(1);
	// }
}

void exit_if_invalid_extension(const std::string &ext)
{
	if (!is_a_valid_extension(ext)) {
		std::cerr << "[error] invalid extension: " << ext << "\n";
		exit(1);
	}
}


Settings create_settings_from_args(int argc, char* argv[])
{
	using std::cerr;

	// creates a vector of string from argv
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));

	if (args.size() == 0) {
		cerr << "[error] no arguments!\n";
		exit(1);
	}

	Settings settings;
	// defaults
	settings.image_path = "";
	settings.image_extension = "";
	settings.output_extension = "";

	int non_opt_args_amount = 0;
	for (auto arg_ptr = args.begin(); arg_ptr != args.end(); ++arg_ptr) {
		std::string current = *arg_ptr;
		
		// handle opts (-q, -r, -o)
		bool is_arg_an_opt = current[0] == '-';
		if (is_arg_an_opt) {
			if (current == args.back()) {
				cerr << "[error] flag " << current << " has no value\n";
				exit(1);
			}

			++arg_ptr;
			continue;
		}

		// handle non-opts
		non_opt_args_amount++;
		if (non_opt_args_amount >= 3) {
			cerr << "[error] multiple paths informed!\n";
			exit(1);
		}

		// check if arg is just a "jpeg", "png", bmp...
		if (is_a_valid_extension(current)) {
			settings.output_extension = current;
			continue;
		}

		settings.image_path = current;

	}

	if (settings.image_path == "") {
		cerr << "[error] path not found \n";
		exit(1);
	}

	settings.image_extension = get_extension(settings.image_path);
	exit_if_invalid_extension(settings.image_extension);

	if (settings.output_extension == "") {
		std::cout << "[encode] output format deduced to be same as input, " << settings.image_extension << "\n";
		settings.output_extension = settings.image_extension;
	}

	return settings;
}

// TODO: make this shared across files, image-reader.cpp and image-writer.cpp
// read-settings, write-settings
class GenericReader {
public:
	virtual std::unique_ptr<ImageData> read(const Settings &settings) = 0;
};
class JPEGReader : public GenericReader {
public:
	std::unique_ptr<ImageData> read(const Settings &settings) {
		std::cout << "reading as jpeg \n";
		return nullptr;
	}
};
class PNGReader : public GenericReader {
public:
	std::unique_ptr<ImageData> read(const Settings &settings) {
		std::cout << "reading as png \n";
		return nullptr;
	}
};
class BMPReader : public GenericReader {
public:
	std::unique_ptr<ImageData> read(const Settings &settings) {
		std::cout << "reading as bmp \n";
		return nullptr;
	}
};

std::unique_ptr<GenericReader> get_read_method(const Settings &settings)
{
	if (settings.image_extension == "jpeg" || settings.image_extension == "jpg") {
		return std::make_unique<JPEGReader>();
	}

	if (settings.image_extension == "png") {
		return std::make_unique<PNGReader>();
	}

	if (settings.image_extension == "bmp") {
		return std::make_unique<BMPReader>();
	}

	exit(1);
}

class GenericWriter {
public:
	virtual std::unique_ptr<ImageData> write(const Settings &settings) = 0;
};
class JPEGWriter : public GenericWriter {
public:
	std::unique_ptr<ImageData> write(const Settings &settings) {
		std::cout << "writing as jpeg \n";
		return nullptr;
	}
};
class PNGWriter : public GenericWriter {
public:
	std::unique_ptr<ImageData> write(const Settings &settings) {
		std::cout << "writing as png \n";
		return nullptr;
	}
};
class BMPWriter : public GenericWriter {
public:
	std::unique_ptr<ImageData> write(const Settings &settings) {
		std::cout << "writing as bmp \n";
		return nullptr;
	}
};

std::unique_ptr<GenericWriter> get_write_method(const Settings &settings)
{
	if (settings.output_extension == "jpeg" || settings.output_extension == "jpg") {
		return std::make_unique<JPEGWriter>();
	}

	if (settings.output_extension == "png") {
		return std::make_unique<PNGWriter>();
	}

	if (settings.output_extension == "bmp") {
		return std::make_unique<BMPWriter>();
	}

	exit(1);
}


int main(int argc, char* argv[])
{
	Settings settings = create_settings_from_args(argc, argv);

	std::unique_ptr<GenericReader> read_method = get_read_method(settings);
	auto img_data = read_method->read(settings);

	std::unique_ptr<GenericWriter> write_method = get_write_method(settings);
	write_method->write(settings);	

}