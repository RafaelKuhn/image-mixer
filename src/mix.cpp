#include <iostream>

#include <vector>
#include <string>

#include "path-utils.h"
#include "image-handler.h"

struct MixSettings {
	std::string r_input_path;
	std::string r_input_ext;
	
	std::string g_input_path;
	std::string g_input_ext;
	
	std::string b_input_path;
	std::string b_input_ext;
	
	bool has_alpha;
	std::string a_input_path;
	std::string a_input_ext;

	std::string output_path;
	std::string output_extension;
	
};

MixSettings create_settings_from_args(int argc, char* argv[])
{
	using std::cout; using std::cerr;

	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));

	if (args.size() == 0) {
		cerr << "[error] no arguments!\n";
		exit(1);
	}

	MixSettings settings;
	// defaults
	settings.has_alpha = false;
	settings.output_path = "output.png";
	settings.output_extension = "png";

	int paths_amount = 0;
	int non_skipped_paths_amount = 0;
	for (auto arg_ptr = args.begin(); arg_ptr != args.end(); ++arg_ptr) {
		std::string current = *arg_ptr;

		// handle opts (-o)
		bool is_arg_an_opt = current[0] == '-';
		if (is_arg_an_opt) {
			char opt_char = current[1];
			
			if (opt_char == 'h') {
				// TODO: implement help and remove this
				std::cerr << "[dev] help not implemented, only you can help yourself now (read the readme ;-)\n";
				exit(1);
				//
			}

			if (opt_char == 'o') {
				if (current == args.back()) {
					cerr << "[error] -o arg without output path, try -o \"my-output.png\"\n";
					exit(1);
				}

				std::string out_filename = *(++arg_ptr);
				settings.output_path = out_filename;
				settings.output_extension = get_extension(out_filename);
				continue;
			}

			cerr << "[error] optional argument \"" << opt_char << "\" not found!\n";
			exit(1);
		}

		// arguments after opts are image paths (to take red, green, blue and alpha, respectively)
		paths_amount++;

		bool arg_starts_with_underscore = current[0] == '_';
		if (arg_starts_with_underscore) {
			const char* current_channel =
				paths_amount == 1 ? "red"   :
				paths_amount == 2 ? "green" :
				paths_amount == 3 ? "blue"  : "alpha";

			cout << "[mix] " << current_channel << " channel will be skipped (filled with zeros)\n";
			continue;
		}

		non_skipped_paths_amount++;

		if (paths_amount == 1) {
			settings.r_input_path = current;
			settings.r_input_ext = get_extension(current);
			if (!is_a_valid_extension(settings.r_input_ext)) {
				cerr << "[error] unsupported extension on 1st image: " << settings.a_input_ext << "\n";
				exit(1);
			}
			continue;
		}
		else if (paths_amount == 2) {
			settings.g_input_path = current;
			settings.g_input_ext = get_extension(current);
			if (!is_a_valid_extension(settings.g_input_ext)) {
				cerr << "[error] unsupported extension on 2nd image: " << settings.a_input_ext << "\n";
				exit(1);
			}
			continue;
		}
		else if (paths_amount == 3) {
			settings.b_input_path = current;
			settings.b_input_ext = get_extension(current);
			if (!is_a_valid_extension(settings.b_input_ext)) {
				cerr << "[error] unsupported extension on 3rd image: " << settings.a_input_ext << "\n";
				exit(1);
			}
			continue;
		}
		else if (paths_amount == 4) {
			// TODO: implement alpha and remove these
			std::cerr << "[dev] alpha support is under development, we don't handle it well yet 8)\n";
			exit(1);
			//
			settings.has_alpha = true;
			settings.a_input_path = current;
			settings.a_input_ext = get_extension(current);
			if (!is_a_valid_extension(settings.a_input_ext)) {
				cerr << "[error] unsupported extension on 4th image: " << settings.a_input_ext << "\n";
			}
			continue;
		}
		else {
			cerr << "[error] more than 4 files specified, our images have a max of only 4 color channels!\n";
			exit(1);
		}
	}

	if (non_skipped_paths_amount == 0) {
		std::cerr << "[error] at least one image path is needed to mix\n";
		exit(1);
	}

	if (!is_a_valid_extension(settings.output_extension)) {
		std::cerr << "[error] invalid extension: " << settings.output_extension << "\n";
		exit(1);
	}

	settings.output_extension = change_to_jpg_if_jpeg(settings.output_extension);

#ifdef DEBUG_MODE
	cout << "\napp settings: \n";
	cout << "r input path "  << (settings.r_input_path == "" ? "NULL" : ("\""+settings.r_input_path+"\"")) << " with ext: " << settings.r_input_ext << "\n";
	cout << "g input path "  << (settings.g_input_path == "" ? "NULL" : ("\""+settings.g_input_path+"\"")) << " with ext: " << settings.g_input_ext << "\n";
	cout << "b input path "  << (settings.b_input_path == "" ? "NULL" : ("\""+settings.b_input_path+"\"")) << " with ext: " << settings.b_input_ext << "\n";
	cout << "has alpha? "  << (settings.has_alpha ? "true" : "false") << "\n";
	cout << "a input path "  << (settings.a_input_path == "" ? "NULL" : ("\""+settings.a_input_path+"\"")) << " with ext: " << settings.a_input_ext << "\n";
	cout << "out path "  << settings.output_path << "\n";
	cout << "out ext "  << settings.output_extension << "\n";
#endif

	return settings;
}

int get_max_width_of(ImageData* r_ptr, ImageData* g_ptr, ImageData* b_ptr, ImageData* a_ptr)
{
	int max_width = 0;
	
	int current_img_width = 0;
	if (r_ptr) {
		current_img_width = r_ptr->get_width();
		if (current_img_width > max_width) {
			max_width = current_img_width;
		}
	}
	if (g_ptr) {
		current_img_width = g_ptr->get_width();
		if (current_img_width > max_width) {
			max_width = current_img_width;
		}
	}
	if (b_ptr) {
		current_img_width = b_ptr->get_width();
		if (current_img_width > max_width) {
			max_width = current_img_width;
		}
	}
	if (a_ptr) {
		current_img_width = a_ptr->get_width();
		if (current_img_width > max_width) {
			max_width = current_img_width;
		}
	}

	return max_width;
}

int get_max_height_of(ImageData* r_ptr, ImageData* g_ptr, ImageData* b_ptr, ImageData* a_ptr)
{
	int max_height = 0;
	
	int current_img_height = 0;
	if (r_ptr) {
		current_img_height = r_ptr->get_height();
		if (current_img_height > max_height) {
			max_height = current_img_height;
		}
	}
	if (g_ptr) {
		current_img_height = g_ptr->get_height();
		if (current_img_height > max_height) {
			max_height = current_img_height;
		}
	}
	if (b_ptr) {
		current_img_height = b_ptr->get_height();
		if (current_img_height > max_height) {
			max_height = current_img_height;
		}
	}
	if (a_ptr) {
		current_img_height = a_ptr->get_height();
		if (current_img_height > max_height) {
			max_height = current_img_height;
		}
	}

	return max_height;
}

void aggregate_red(const ImageData &src, ImageData &dest)
{
	int src_width  = src.get_width();
	int src_height = src.get_height();

	int dest_width  = dest.get_width();

	for (int y = 0; y < src_height; ++y) {
		for (int x = 0; x < src_width; ++x) {
			int src_index = y * src_width + x;
			int dest_index = y * dest_width + x;
			dest.colors[dest_index].r = src.colors[src_index].r;
		}
	}
}
void aggregate_green(const ImageData &src, ImageData &dest)
{
	int src_width  = src.get_width();
	int src_height = src.get_height();

	int dest_width  = dest.get_width();

	for (int y = 0; y < src_height; ++y) {
		for (int x = 0; x < src_width; ++x) {
			int src_index = y * src_width + x;
			int dest_index = y * dest_width + x;
			dest.colors[dest_index].g = src.colors[src_index].g;
		}
	}
}
void aggregate_blue(const ImageData &src, ImageData &dest)
{
	int src_width  = src.get_width();
	int src_height = src.get_height();

	int dest_width  = dest.get_width();

	for (int y = 0; y < src_height; ++y) {
		for (int x = 0; x < src_width; ++x) {
			int src_index = y * src_width + x;
			int dest_index = y * dest_width + x;
			dest.colors[dest_index].b = src.colors[src_index].b;
		}
	}
}

// TODO: aggregate alpha

int main(int argc, char *argv[])
{
	MixSettings settings = create_settings_from_args(argc, argv);

	std::unique_ptr<ImageData> red_image_data_ptr = nullptr;
	std::unique_ptr<ImageData> green_image_data_ptr = nullptr;
	std::unique_ptr<ImageData> blue_image_data_ptr = nullptr;
	
	std::unique_ptr<ImageData> alpha_image_data_ptr = nullptr;
	
	if (!settings.r_input_path.empty()) {
		read_function read_red_image = get_read_function(settings.r_input_ext);
		red_image_data_ptr = read_red_image(settings.r_input_path.data());
		exit_if_nullptr(red_image_data_ptr);
	}

	if (!settings.g_input_path.empty()) {
		read_function read_green_image = get_read_function(settings.g_input_ext);
		green_image_data_ptr = read_green_image(settings.g_input_path.data());
		exit_if_nullptr(green_image_data_ptr);
	}

	if (!settings.b_input_path.empty()) {
		read_function read_blue_image = get_read_function(settings.b_input_ext);
		blue_image_data_ptr = read_blue_image(settings.b_input_path.data());
		exit_if_nullptr(blue_image_data_ptr);
	}

	// TODO: handle alpha
	// if (!settings.a_input_path.empty()) {
	// 	read_function read_alpha_image = get_read_function(settings.a_input_ext);
	// 	alpha_image_data_ptr = read_alpha_image(settings.a_input_path.data());
	// 	exit_if_nullptr(alpha_image_data_ptr);
	// }

	WriteSettings write_settings(settings.output_path, settings.has_alpha);
	write_function write_image = get_write_function(settings.output_extension);

	int max_width  = get_max_width_of(red_image_data_ptr.get(), green_image_data_ptr.get(), blue_image_data_ptr.get(), alpha_image_data_ptr.get());
	int max_height = get_max_height_of(red_image_data_ptr.get(), green_image_data_ptr.get(), blue_image_data_ptr.get(), alpha_image_data_ptr.get());
	
	std::cout << "[mix] creating image with width " << max_width << " and height " << max_height << "\n";
	auto mixed_image = std::make_unique<ImageData>(max_width, max_height);
	
	if (red_image_data_ptr) aggregate_red(*red_image_data_ptr, *mixed_image);
	if (green_image_data_ptr) aggregate_green(*green_image_data_ptr, *mixed_image);
	if (blue_image_data_ptr) aggregate_blue(*blue_image_data_ptr, *mixed_image);
	// aggregate_alpha(*alpha_image_data_ptr, *mixed_image);
	
	std::cout << "[mix] saving to \"" << settings.output_path << "\"\n";
	write_image(write_settings, *mixed_image);
}