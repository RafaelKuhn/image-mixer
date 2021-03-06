#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "image-handler.h"
#include "path-utils.h"


struct EncodeSettings {
	std::string input_path;
	std::string input_extension;
	std::string output_path;
	std::string output_extension;
	bool remove_alpha;
	int jpeg_quality;
	int chroma_subsampling;
};

EncodeSettings create_settings_from_args(int argc, char* argv[])
{
	using std::cout; using std::cerr;

	// creates a vector of string from argv
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(std::string(argv[i]));

	if (args.size() == 0) {
		cerr << "[error] no arguments!\n";
		exit(1);
	}

	EncodeSettings settings;
	// defaults
	settings.input_path = "";
	settings.input_extension = "";
	settings.output_path = "";
	settings.output_extension = "";
	settings.remove_alpha = false;
	settings.jpeg_quality = 80;
	settings.chroma_subsampling = 420;

	int non_opt_args_amount = 0;
	for (auto arg_ptr = args.begin(); arg_ptr != args.end(); ++arg_ptr) {
		std::string current = *arg_ptr;
		
		// handle opts (-q, -r, -o)
		bool is_arg_an_opt = current[0] == '-';
		if (is_arg_an_opt) {
			if (current.length() > 2) {
				cerr << "[error] opt argument too long!: " << current;
				cerr << "in this program, opt args have just one character, like -a or -m!\n";
				exit(1);
			}
			char opt_char = current[1];
			
			// args that don't require a value
			if (opt_char == 'h') {
				// TODO: implement help and remove this
				std::cerr << "[dev] help not implemented, only you can help yourself now (read the readme ;-)\n";
				exit(1);
				//
			}
			
			if (opt_char == 'r') {
				bool is_output_jpeg = settings.output_extension == "jpg" || settings.output_extension == "jpeg";
				if (is_output_jpeg) {
					cerr << "[error] can't remove alpha from jpeg, jpegs don't have alpha!\n";
					exit(1);
				}
				settings.remove_alpha = true;
				continue;
			}

			// args that require a value
			if (opt_char == 'o') {
				if (current == args.back()) {
					cerr << "[error] flag " << current << " requires a value\n";
					exit(1);
				}
				std::string arg_after_current = *(++arg_ptr);
				settings.output_path = arg_after_current;
				continue;
			}

			bool is_output_png_or_bmp = settings.output_extension == "png" || settings.output_extension == "bmp";
			if (opt_char == 'q') {
				if (is_output_png_or_bmp) {
					cerr << "[error] can't change quality of pngs or bmps, jpeg only option!\n";
					exit(1);
				}
				if (current == args.back()) {
					cerr << "[error] flag " << current << " requires a value\n";
					exit(1);
				}

				std::string arg_after_current = *(++arg_ptr);
				int jpeg_quality_from_arg = ensure_parse_int(arg_after_current);
				if (!is_quality_valid(jpeg_quality_from_arg)) {
					cerr << "[error] invalid jpeg quality!\n";
					cerr << "must be a number between 0 and 100\n";
					exit(1);
				}
				
				settings.jpeg_quality = jpeg_quality_from_arg;
				continue;
			}

			if (opt_char == 'c') {
				if (is_output_png_or_bmp) {
					cerr << "[error] can't change chrominance subsampling of pngs or bmps, jpeg only option!\n";
					exit(1);
				}
				if (current == args.back()) {
					cerr << "[error] flag " << current << " requires a value\n";
					exit(1);
				}
				
				std::string arg_after_current = *(++arg_ptr);
				int chroma_subs_from_arg = ensure_parse_int(arg_after_current);
				if (!is_chroma_subsampling_representation_valid(chroma_subs_from_arg)) {
					cerr << "[error] invalid chrominance subsampling!\n";
					cerr << "options are: 444, 422, 420 & 411\n";
					exit(1);
				}

				settings.chroma_subsampling = chroma_subs_from_arg;
				continue;
			}

			cerr << "[error] optional argument \"" << opt_char << "\" not found!\n";
			exit(1);
		}

		// handle non-opts
		non_opt_args_amount++;
		if (non_opt_args_amount >= 3) {
			cerr << "[error] multiple paths informed!\n";
			exit(1);
		}

		// check if that arg is just a "jpeg", "png", bmp...
		if (is_a_valid_extension(current)) {
			settings.output_extension = current;
			continue;
		}

		settings.input_path = current;
	}
	// end of for loop over all args

	if (settings.input_path == "") {
		cerr << "[error] path not found \n";
		exit(1);
	}
	
	settings.input_extension = get_extension(settings.input_path);
	if (!is_a_valid_extension(settings.input_extension)) {
		std::cerr << "[error] invalid extension: " << settings.input_extension << "\n";
		exit(1);
	}

	bool output_path_not_informed = settings.output_path == "";
	bool output_format_not_informed = settings.output_extension == "";
	if (output_path_not_informed && output_format_not_informed) {
		settings.output_path = std::string("output.").append(settings.input_extension);
		settings.output_extension = settings.input_extension;
		std::cout << "[encode] neither output path nor format informed, saving to \"" << settings.output_path << "\"\n";
	}
	else if (output_path_not_informed) {
		settings.output_path = replace_extension(settings.input_path, settings.output_extension);
		std::cout << "[encode] output file name deduced to be same as input, with specified extension: \"" << settings.output_path << "\"\n";
	}
	else if (output_format_not_informed) {
		settings.output_extension = get_extension(settings.output_path);
		std::cout << "[encode] output format deduced to be the extension of \"-o\", saving to \"" << settings.output_path << "\"\n";
	}

	// if input is a jpeg, output doesn't need to have alpha
	bool is_input_jpeg = settings.input_extension == "jpg" || settings.input_extension == "jpeg";
	if (is_input_jpeg) {
		settings.remove_alpha = true;
	}
	// if input and output path are the same, change output to "output.smth" to prevent overriding an image
	if (settings.output_path == settings.input_path) {
		settings.output_path = std::string("output.").append(settings.input_extension);
		settings.output_extension = settings.input_extension;
		std::cout << "[encode] input and output path can't be same, output path will be " << settings.output_path << "\n";
	}

	// change any extension "jpeg" to a "jpg", because 4 letter extensions are fucking ugly
	settings.input_extension  = change_to_jpg_if_jpeg(settings.input_extension);
	settings.output_extension = change_to_jpg_if_jpeg(settings.output_extension);

#ifdef DEBUG_MODE
	cout << "\napp settings: ";
	cout << "\ninput path "  << settings.input_path;
	cout << "\ninput ext "  << settings.input_extension;
	cout << "\noutput path "  << settings.output_path;
	cout << "\noutput ext "  << settings.output_extension;
	cout << "\njpeg q "  << settings.jpeg_quality;
	cout << "\nch_ss "  << settings.chroma_subsampling;
	cout << "\nremove a "  << settings.remove_alpha << "\n\n";
#endif

	return settings;
}


int main(int argc, char* argv[])
{
	EncodeSettings settings = create_settings_from_args(argc, argv);

	read_function read_image = get_read_function(settings.input_extension);
	std::unique_ptr<ImageData> img_data = read_image(settings.input_path.data());
	exit_if_nullptr(img_data);

	WriteSettings write_settings = WriteSettings(settings.output_path, !settings.remove_alpha, settings.jpeg_quality, settings.chroma_subsampling);

	write_function write_image = get_write_function(settings.output_extension);
	std::cout << "[encode] writing to \"" << write_settings.file_name << "\"\n";
	write_image(write_settings, *img_data);
}