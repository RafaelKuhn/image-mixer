// TODO: make targets
// TODO: -t opt to measure time
// TODO: read image based on it's extension

/* TODO: OPTIMIZE PNG!!!
took
real    1m36.803s
user    1m33.312s
sys     0m0.530s
to read 8k png  */;

// TODO: option to demix to yUV (could use png lib)
#include <iostream>
#include <memory>

#include <vector>

#include "png-wrapper.h"
#include "jpeg-wrapper.h"

#include "timer.h"


using std::cout;

void clone_red(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			Color col(0, 0, 0);
			col.r = base.colors[index].r;

			dest.colors[index] = col;
		}
	}
}

void clone_green(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			Color col(0, 0, 0);
			col.g = base.colors[index].g;

			dest.colors[index] = col;
		}
	}
}

void clone_blue(const ImageData& base, ImageData& dest)
{
	int height = base.get_height();
	int width = base.get_width();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			Color col(0, 0, 0);
			col.b = base.colors[index].b;

			dest.colors[index] = col;
		}
	}
}

enum Mode { RGB, GREY };
struct Settings {
	bool has_alpha = false;
	Mode color_mode;
	std::string image_path;
};

std::vector<std::string> create_str_vect_of_args(int argc, char* argv[])
{
	std::vector<std::string> vect;

	for (int i = 1; i < argc; ++i) {
		vect.push_back(std::string(argv[i]));
	}

	return vect;
}


Settings create_settings_from_args(std::vector<std::string> args)
{
	if (args.size() == 0) {
		std::cerr << "[error] no arguments!\n";
		exit(1);
	}
	// if (args.size() > 4) {
	// 	std::cerr << "[error] too many arguments!\n";
	// 	exit(1);
	// }

	// defaults
	Settings settings;
	settings.has_alpha = false;
	settings.color_mode = RGB;
	settings.image_path = "";

	int non_opt_args_amount = 0;

	for (auto arg_ptr = args.begin(); arg_ptr != args.end(); arg_ptr++) {
		std::string current = *arg_ptr; // cout << "------->: " << current << "\n";

		bool is_arg_not_an_opt = current[0] != '-';
		if (is_arg_not_an_opt) {
			if (non_opt_args_amount > 0) {
				std::cerr << "[error] multiple paths informed!\n";
				cout << "can't decide between \"" << settings.image_path << "\" and \"" << current << "!\n";
				exit(1);
			}
			non_opt_args_amount++;
			settings.image_path = current;
			continue;
		}

		if (current.size() > 2) {
			std::cerr << "[error] opt argument not found: " << current;
			cout << "opt arguments are -a and -m!\n";
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
				std::cerr << "[error] mode not found!\n";
				cout << "modes are \"rgb\" and \"grey\"\n";
				exit(1);
			}

			std::string mode = *(++arg_ptr);
			if (mode == std::string("grey") || mode == std::string("gray")) {
				settings.color_mode = GREY;

			} else if (mode == std::string("rgb")) {
				settings.color_mode = RGB;

			} else {
				std::cerr << "[error] mode not found: " << mode;
				cout << "\nmodes are \"rgb\" and \"grey\"\n";
				exit(1);
			}
			continue;
		}

		std::cerr << "[error] optional argument \"" << current << "\" not found!\n";
		exit(1);
	}

	if (non_opt_args_amount == 0) {
		std::cerr << "[error] you must specify path to the image!\n";
		exit(1);
	}

	return settings;
}

int main(int argc, char* argv[])
{
	using std::string; using std::make_unique; using std::unique_ptr;

	auto arg_vect = create_str_vect_of_args(argc, argv);
	Settings settings = create_settings_from_args(arg_vect);

	cout << "[settings]\nalpha : " << (settings.has_alpha == 1 ? "true" : "false") << "\n";
	cout << "mode  : " << (settings.color_mode == 0 ? "rgb" : "grey") << "\n";
	cout << "path  : " << settings.image_path.data() << "\n";

	string basename = settings.image_path;
	cout << "[demix] reading path " << basename << "\n";

				Timer::get_instance().start_measure();
	unique_ptr<ImageData> original_img = read_as_png(basename.data());
				Timer::get_instance().print_measure("time reading img: ");
	
				Timer::get_instance().start_measure();
	auto temp_img = make_unique<ImageData>(original_img->get_width(), original_img->get_height()); // unique_ptr<ImageData> cloned_img(original_img->clone()); // clones it
				Timer::get_instance().print_measure("time cloning img: ");		

	
	cout << "[demix] generating red\n";
	string r_image_path = string(basename).replace(basename.find(".png"), sizeof(".png"), "-r.jpg");
				Timer::get_instance().start_measure();
	clone_red(*original_img, *temp_img);
	write_as_jpeg(r_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
				Timer::get_instance().print_measure("time cloning red:   ");
	cout << "[demix] red saved at   \"" << r_image_path <<  "\"\n";


	cout << "[demix] generating green\n";
	string g_image_path = string(basename).replace(basename.find(".png"), sizeof(".png"), "-g.jpg");
				Timer::get_instance().start_measure();
	clone_green(*original_img, *temp_img);
	write_as_jpeg(g_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
				Timer::get_instance().print_measure("time cloning green: ");
	cout << "[demix] green saved at  \"" << g_image_path <<  "\"\n";

	cout << "[demix] generating blue\n";
	string b_image_path = string(basename).replace(basename.find(".png"), sizeof(".png"), "-b.jpg");
				Timer::get_instance().start_measure();
	clone_blue(*original_img, *temp_img);
	write_as_jpeg(b_image_path.data(), *temp_img, 80, SUBSAMPLING_420);
				Timer::get_instance().print_measure("time cloning blue:  ");
	cout << "[demix] blue saved at \"" << b_image_path <<  "\"\n";
}