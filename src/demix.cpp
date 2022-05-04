// TODO: make targets
// TODO: -t opt to measure time
// TODO: option to demix to yUV (could use png lib)

#include <iostream>
#include <memory>

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <functional>

#include "bmp.h"
#include "png-wrapper.h"
#include "jpeg-wrapper.h"

#include "timer.h"


std::string append_before_extension(const std::string &path, const std::string &ext, const char* appended)
{
	size_t replace_start = path.length() - ext.length() - 1;
	size_t ext_size = sizeof(ext);
	std::string new_path_ending = std::string(appended).append(".").append(ext);
	
	std::string new_path = std::string(path).replace(replace_start, ext_size, new_path_ending);

	return new_path;
}

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

void clone_only_red(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			Color col(base.colors[index].r, 0, 0);

			dest.colors[index] = col;
		}
	}
}
void clone_only_green(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			Color col(0, base.colors[index].g, 0);

			dest.colors[index] = col;
		}
	}
}
void clone_only_blue(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			Color col(0, 0, base.colors[index].b);

			dest.colors[index] = col;
		}
	}
}

void distribute_red(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			int channel = base.colors[index].r;
			Color col(channel, channel, channel);

			dest.colors[index] = col;
		}
	}
}
void distribute_green(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			int channel = base.colors[index].g;
			Color col(channel, channel, channel);

			dest.colors[index] = col;
		}
	}
}
void distribute_blue(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			int channel = base.colors[index].b;
			Color col(channel, channel, channel);

			dest.colors[index] = col;
		}
	}
}

enum Mode { RGB, GREY };
struct Settings {
	std::string image_path;
	std::string image_extension;
	Mode color_mode;
	bool has_alpha = false;
};

// TODO: demix function could take 'read function' and 'write function'
// or polymorphism: class receives a 'read method' and 'write method'
void demix_png(const Settings &settings)
{
	using std::cout;

	cout << "[demix] reading file as \"" << settings.image_path << "\" as " << settings.image_extension << "\n";
	auto image_data = read_as_png(settings.image_path.data());
	if (!image_data) { exit(1); }
	
	auto temp_img = std::make_unique<ImageData>(image_data->get_width(), image_data->get_height());

	if (settings.color_mode == GREY) {
		std::string red_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wr");
		distribute_red(*image_data, *temp_img);
		write_as_png(red_image_path.data(), *temp_img, settings.has_alpha);
		cout << "[demix] red saved at \"" << red_image_path <<  "\"\n";

		std::string green_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wg");
		distribute_green(*image_data, *temp_img);
		write_as_png(green_image_path.data(), *temp_img, settings.has_alpha);
		cout << "[demix] green saved at \"" << green_image_path <<  "\"\n";

		std::string blue_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wb");
		distribute_blue(*image_data, *temp_img);
		write_as_png(blue_image_path.data(), *temp_img, settings.has_alpha);
		cout << "[demix] blue saved at  \"" << blue_image_path <<  "\"\n";
	} else {
		std::string red_image_path = append_before_extension(settings.image_path, settings.image_extension, "-r");
		clone_only_red(*image_data, *temp_img);
		write_as_png(red_image_path.data(), *temp_img, settings.has_alpha);
		cout << "[demix] red saved at \"" << red_image_path <<  "\"\n";

		std::string green_image_path = append_before_extension(settings.image_path, settings.image_extension, "-g");
		clone_only_green(*image_data, *temp_img);
		write_as_png(green_image_path.data(), *temp_img, settings.has_alpha);
		cout << "[demix] green saved at \"" << green_image_path <<  "\"\n";

		std::string blue_image_path = append_before_extension(settings.image_path, settings.image_extension, "-b");
		clone_only_blue(*image_data, *temp_img);
		write_as_png(blue_image_path.data(), *temp_img, settings.has_alpha);
		cout << "[demix] blue saved at  \"" << blue_image_path <<  "\"\n";
	}
}

void demix_bmp(const Settings &settings)
{
	using std::cout;

	std::cout << "[demix] reading file as \"" << settings.image_path << "\" as " << settings.image_extension << "\n";
	auto image_data = read_as_bmp(settings.image_path.data());
	if (!image_data) { exit(1); }
	
	auto temp_img = std::make_unique<ImageData>(image_data->get_width(), image_data->get_height());

	if (settings.color_mode == GREY) {
		std::string red_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wr");
		distribute_red(*image_data, *temp_img);
		write_as_bmp(red_image_path.data(), *temp_img);
		cout << "[demix] red saved at \"" << red_image_path <<  "\"\n";

		std::string green_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wg");
		distribute_green(*image_data, *temp_img);
		write_as_bmp(green_image_path.data(), *temp_img);
		cout << "[demix] green saved at \"" << green_image_path <<  "\"\n";

		std::string blue_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wb");
		distribute_blue(*image_data, *temp_img);
		write_as_bmp(blue_image_path.data(), *temp_img);
		cout << "[demix] blue saved at  \"" << blue_image_path <<  "\"\n";
	} else {
		std::string red_image_path = append_before_extension(settings.image_path, settings.image_extension, "-r");
		clone_only_red(*image_data, *temp_img);
		write_as_bmp(red_image_path.data(), *temp_img);
		cout << "[demix] red saved at \"" << red_image_path <<  "\"\n";

		std::string green_image_path = append_before_extension(settings.image_path, settings.image_extension, "-g");
		clone_only_green(*image_data, *temp_img);
		write_as_bmp(green_image_path.data(), *temp_img);
		cout << "[demix] green saved at \"" << green_image_path <<  "\"\n";

		std::string blue_image_path = append_before_extension(settings.image_path, settings.image_extension, "-b");
		clone_only_blue(*image_data, *temp_img);
		write_as_bmp(blue_image_path.data(), *temp_img);
		cout << "[demix] blue saved at  \"" << blue_image_path <<  "\"\n";
	}
}

void demix_jpeg(const Settings &settings)
{
	using std::cout;

	cout << "[demix] reading file as \"" << settings.image_path << "\" as " << settings.image_extension << "\n";
	auto image_data = read_as_jpeg(settings.image_path.data());
	if (!image_data) { exit(1); }
	
	auto temp_img = std::make_unique<ImageData>(image_data->get_width(), image_data->get_height());

	if (settings.color_mode == GREY) {
		std::string red_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wr");
		distribute_red(*image_data, *temp_img);
		write_as_jpeg(red_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
		cout << "[demix] red saved at \"" << red_image_path <<  "\"\n";

		std::string green_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wg");
		distribute_green(*image_data, *temp_img);
		write_as_jpeg(green_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
		cout << "[demix] green saved at \"" << green_image_path <<  "\"\n";

		std::string blue_image_path = append_before_extension(settings.image_path, settings.image_extension, "-wb");
		distribute_blue(*image_data, *temp_img);
		write_as_jpeg(blue_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
		cout << "[demix] blue saved at  \"" << blue_image_path <<  "\"\n";
	} else {
		std::string red_image_path = append_before_extension(settings.image_path, settings.image_extension, "-r");
		clone_only_red(*image_data, *temp_img);
		write_as_jpeg(red_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
		cout << "[demix] red saved at   \"" << red_image_path <<  "\"\n";

		std::string green_image_path = append_before_extension(settings.image_path, settings.image_extension, "-g");
		clone_only_green(*image_data, *temp_img);
		write_as_jpeg(green_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
		cout << "[demix] green saved at \"" << green_image_path <<  "\"\n";

		std::string blue_image_path = append_before_extension(settings.image_path, settings.image_extension, "-b");
		clone_only_blue(*image_data, *temp_img);
		write_as_jpeg(blue_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
		cout << "[demix] blue saved at  \"" << blue_image_path <<  "\"\n";
	}
}

std::map<std::string, std::function<void(Settings)>> demix_functions_by_extension = {
	{ "jpg",  demix_jpeg },
	{ "jpeg", demix_jpeg },
	{ "png",  demix_png },
	{ "bmp",  demix_bmp }
};

void exit_if_invalid_extension(const std::string &ext)
{
	bool is_extension_invalid = demix_functions_by_extension.find(ext) == demix_functions_by_extension.end();
	if (is_extension_invalid) {
		std::cerr << "[error] invalid extension: \"" << ext << "\"\nvalid extensions are: ";
		for (auto const& [key, val] : demix_functions_by_extension) {
			std::cerr << key << " ";
		}
		std::cerr << "\n";
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
	settings.has_alpha = false;
	settings.color_mode = RGB;

	int non_opt_args_amount = 0;
	for (auto arg_ptr = args.begin(); arg_ptr != args.end(); ++arg_ptr) {
		std::string current = *arg_ptr;

		bool is_arg_not_an_opt = current[0] != '-';
		if (is_arg_not_an_opt) {
			if (non_opt_args_amount > 0) {
				cerr << "[error] multiple paths informed!\n";
				cerr << "can't decide between \"" << settings.image_path << "\" and \"" << current << "!\n";
				exit(1);
			}
			non_opt_args_amount++;
			settings.image_path = current;
			continue;
		}

		if (current.size() > 2) {
			cerr << "[error] opt argument too long!: " << current;
			cerr << "opt arguments have just one character, like -a or -m!\n";
			exit(1);
		}

		char opt_char = current[1];

		// check for -a
		if (opt_char == 'a') {
			settings.has_alpha = true;
			continue;
		}
		
		// check for -m (if found, uses next iteration as value for -m)
		if (opt_char == 'm') {
			// check if -m is the last argument
			if (current == args.back()) {
				cerr << "[error] mode not specified!\n";
				cerr << "try \"-m rgb\" or \"-m grey\" \n";
				exit(1);
			}

			std::string mode = *(++arg_ptr);
			
			if (mode == "grey" || mode == "gray") {
				settings.color_mode = GREY;

			} else if (mode == "rgb") {
				settings.color_mode = RGB;

			} else {
				cerr << "[error] specified mode not found: " << mode;
				cerr << "\nmodes are \"rgb\" and \"grey\"\n";
				exit(1);
			}
			continue;
		}

		cerr << "[error] optional argument \"" << current << "\" not found!\n";
		exit(1);
	}

	if (non_opt_args_amount == 0) {
		cerr << "[error] you must specify path to the image!\n";
		exit(1);
	}

	settings.image_extension = get_extension(settings.image_path);
	exit_if_invalid_extension(settings.image_extension);

	return settings;
}

int main(int argc, char* argv[])
{
	Settings settings = create_settings_from_args(argc, argv);

	auto demix_function = demix_functions_by_extension.at(settings.image_extension);

	demix_function(settings);
}