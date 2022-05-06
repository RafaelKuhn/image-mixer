#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "image-handler.h"
#include "path-utils.h"

#include "timer.h"

// TODO: rename to demix settings
enum DemixMode { RGB, GREY };
struct DemixSettings {
	std::string input_path;
	std::string input_extension;
	DemixMode color_mode;
	bool has_alpha = false;
};

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


void exit_if_invalid_extension(const std::string &ext)
{
	bool is_ext_invalid = is_extension_valid(ext) == false;
	if (is_ext_invalid) {
		std::cerr << "[error] invalid extension: \"" << ext << "\"\nvalid extensions are: png, jpg & bmp\n";
		exit(1);
	}
}

DemixSettings create_settings_from_args(int argc, char* argv[])
{
	using std::cerr; using std::cout;

	// creates a vector of string from argv
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));

	if (args.size() == 0) {
		cerr << "[error] no arguments!\n";
		exit(1);
	}

	DemixSettings settings;
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
				cerr << "can't decide between \"" << settings.input_path << "\" and \"" << current << "!\n";
				exit(1);
			}
			non_opt_args_amount++;
			settings.input_path = current;
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

	settings.input_extension = get_extension(settings.input_path);
	exit_if_invalid_extension(settings.input_extension);

#ifdef DEBUG_MODE
	cout << "\napp settings: ";
	cout << "\ninput path? " << settings.input_path;
	cout << "\ninput ext? " << settings.input_extension;
	cout << "\ncolor mode? " << (settings.color_mode == GREY ? "grey" : "rgb");
	cout << "\ninclude alpha? " << (settings.has_alpha ? "true" : "false");
	cout << "\n";
#endif

	return settings;
}

int main(int argc, char* argv[])
{
	DemixSettings settings = create_settings_from_args(argc, argv);

	read_function read_image = get_read_function(settings.input_extension);
	auto image_data_ptr = read_image(settings.input_path.data());
	exit_if_nullptr(image_data_ptr);
	
	write_function write_image = get_write_function(settings.input_extension);
	WriteSettings write_settings = WriteSettings();
	
	auto temp_img_ptr = std::make_unique<ImageData>(image_data_ptr->get_width(), image_data_ptr->get_height());
	
	if (settings.color_mode == GREY) {
		write_settings.file_name = append_before_extension(settings.input_path, "-wr");
		std::cout << "writing red to \"" << write_settings.file_name << "\"\n";
		distribute_red(*image_data_ptr, *temp_img_ptr);
		write_image(write_settings, *temp_img_ptr);

		write_settings.file_name = append_before_extension(settings.input_path, "-wg");
		std::cout << "writing green to \"" << write_settings.file_name << "\"\n";
		distribute_green(*image_data_ptr, *temp_img_ptr);
		write_image(write_settings, *temp_img_ptr);

		write_settings.file_name = append_before_extension(settings.input_path, "-wb");
		std::cout << "writing blue to \"" << write_settings.file_name << "\"\n";
		distribute_blue(*image_data_ptr, *temp_img_ptr);
		write_image(write_settings, *temp_img_ptr);
	}
	else {
		write_settings.file_name = append_before_extension(settings.input_path, "-r");
		std::cout << "writing red to \"" << write_settings.file_name << "\"\n";
		clone_only_red(*image_data_ptr, *temp_img_ptr);
		write_image(write_settings, *temp_img_ptr);

		write_settings.file_name = append_before_extension(settings.input_path, "-g");
		std::cout << "writing green to \"" << write_settings.file_name << "\"\n";
		clone_only_green(*image_data_ptr, *temp_img_ptr);
		write_image(write_settings, *temp_img_ptr);

		write_settings.file_name = append_before_extension(settings.input_path, "-b");
		std::cout << "writing blue to \"" << write_settings.file_name << "\"\n";
		clone_only_blue(*image_data_ptr, *temp_img_ptr);
		write_image(write_settings, *temp_img_ptr);
	}
}