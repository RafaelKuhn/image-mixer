#include "image-handler.h"

#include <memory>
#include <string>
#include <functional>

#include <iostream>

#include "types.h"
#include "jpeg-wrapper.h"
#include "png-wrapper.h"
#include "bmp.h"


/**
 * Read
 */

using read_function = std::function<std::unique_ptr<ImageData>(const char*)>;

read_function get_read_function(const std::string &file_ext)
{
	if (file_ext == "jpg" || file_ext == "jpeg") {
		return read_as_jpeg;
	}
	else if (file_ext == "png") {
		return read_as_png;
	}
	else if (file_ext == "bmp") {
		return read_as_bmp;
	}
	
	std::cerr << "fatal error, unimplemented extension: " << file_ext; exit(1);
}


/**
 * Write
 */

WriteSettings::WriteSettings(const std::string &path, bool alpha, int jpeg_q, int chroma_ss_repr)
{
	this->file_name = path;
	
	this->should_write_with_alpha = alpha;
	
	this->jpeg_quality = jpeg_q;
	this->jpeg_chrominance_subsampling = int_representation_to_chroma_ss(chroma_ss_repr);

#ifdef DEBUG_MODE
	std::cout << "\ncreated write settings with: \n";
	std::cout << "path: " << this->file_name << "\n";
	std::cout << "alpha: " << this->should_write_with_alpha << "\n";
	std::cout << "jpeg ql: " << this->jpeg_quality << "\n";
	std::cout << "jpeg ch_ss: " << this->jpeg_chrominance_subsampling << "\n\n";
#endif
}

void write_as_jpeg(const WriteSettings& settings, const ImageData &data)
{
	write_as_jpeg(settings.file_name.data(), data, settings.jpeg_quality, settings.jpeg_chrominance_subsampling);
}

void write_as_png(const WriteSettings& settings, const ImageData &data)
{
	write_as_png(settings.file_name.data(), data, settings.should_write_with_alpha);
}

void write_as_bmp(const WriteSettings& settings, const ImageData &data)
{
	write_as_bmp(settings.file_name.data(), data);
}

using write_function = std::function<void(const WriteSettings&, const ImageData&)>;
using write_function_ptr = void (*)(const WriteSettings&, const ImageData&);

write_function get_write_function(const std::string &file_ext)
{
	// static cast is needed here to disambiguate between the
	// write_as... in this file and its other overloads
	if (file_ext == "jpg" || file_ext == "jpeg") {
		return static_cast<write_function_ptr>(write_as_jpeg);
	}
	else if (file_ext == "bmp") {
		return static_cast<write_function_ptr>(write_as_bmp);
	}
	else if (file_ext == "png") {
		return static_cast<write_function_ptr>(write_as_png);
	}

	std::cerr << "fatal error: invalid extension: \"" << file_ext << "\", should be validated first!\n";
	exit(1);
}


/**
 * Misc
 */

constexpr CHROMINANCE_SUBSAMPLING int_representation_to_chroma_ss(int ch_ss)
{
	switch(ch_ss) {
	case 411:
		return SUBSAMPLING_411;
	case 420:
		return SUBSAMPLING_420;
	case 422:
		return SUBSAMPLING_422;
	case 444:
	default:
		return SUBSAMPLING_444;
	}
}

bool is_chroma_subsampling_representation_valid(const int &chroma_subsampling)
{
	bool is_valid = chroma_subsampling == 444 || chroma_subsampling == 422 ||
	                chroma_subsampling == 420 || chroma_subsampling == 411;
	
	return is_valid;
}

bool is_quality_valid(const int &quality)
{
	bool is_valid = quality <= 100 && quality >= 0;
	
	return is_valid;
}

bool is_a_valid_extension(const std::string &ext)
{
	return (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "bmp");
}

void exit_if_nullptr(std::unique_ptr<ImageData> &ptr) {
	if (!ptr) {
		exit(1);
	}
}