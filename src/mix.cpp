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

	// creates a vector of string from argv
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
	for (auto arg_ptr = args.begin(); arg_ptr != args.end(); ++arg_ptr) {
		std::string current = *arg_ptr;

		// handle opts (-o)
		bool is_arg_an_opt = current[0] == '-';
		if (is_arg_an_opt) {
			char opt_char = current[1];
			
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

		// arguments after opts are image paths (for red, green, blue and alpha, respectively)
		paths_amount++;

		bool arg_starts_with_underscore = current[0] == '_';
		if (arg_starts_with_underscore) {
			cout << "[mix] channel taken from path \"" << current << "\" will be filled with zeros (arg starts with _)\n";
			continue;
		}

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
			settings.has_alpha = true;
			settings.a_input_path = current;
			settings.a_input_ext = get_extension(current);
			if (!is_a_valid_extension(settings.a_input_ext)) {
				cerr << "[error] unsupported extension on 4th image: " << settings.a_input_ext << "\n";
			}
			continue;
		}
		else {
			cerr << "[error] more than 4 files specified, our images have max of only 4 color channels!\n";
			exit(1);
		}
	}

	if (!is_a_valid_extension(settings.output_extension)) {
		std::cerr << "[error] invalid extension: " << settings.output_extension << "\n";
		exit(1);
	}

	settings.output_extension = change_to_jpg_if_jpeg(settings.output_extension);

// #ifdef DEBUG_MODE
	cout << "\napp settings: \n";
	cout << "r input path "  << (settings.r_input_path == "" ? "NULL" : ("\""+settings.r_input_path+"\"")) << " with ext: " << settings.r_input_ext << "\n";
	cout << "g input path "  << (settings.g_input_path == "" ? "NULL" : ("\""+settings.g_input_path+"\"")) << " with ext: " << settings.g_input_ext << "\n";
	cout << "b input path "  << (settings.b_input_path == "" ? "NULL" : ("\""+settings.b_input_path+"\"")) << " with ext: " << settings.b_input_ext << "\n";
	cout << "has alpha? "  << (settings.has_alpha ? "true" : "false") << "\n";
	cout << "a input path "  << (settings.a_input_path == "" ? "NULL" : ("\""+settings.a_input_path+"\"")) << " with ext: " << settings.a_input_ext << "\n";
	cout << "out path "  << settings.output_path << "\n";
	cout << "out ext "  << settings.output_extension << "\n";
// #endif

	return settings;
}

int main(int argc, char *argv[])
{
	MixSettings settings = create_settings_from_args(argc, argv);
}