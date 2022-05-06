#pragma once

#include <memory>
#include <string>
#include <functional>

#include "types.h"
#include "jpeg-wrapper.h"
#include "png-wrapper.h"
#include "bmp.h"

/**
 * Read
 */
using read_function = std::function<std::unique_ptr<ImageData>(const char*)>;

read_function get_read_function(const std::string &file_ext);

/**
 * Write
 */
struct WriteSettings {
	std::string file_name;
	bool should_write_with_alpha;
	int jpeg_quality;
	CHROMINANCE_SUBSAMPLING jpeg_chrominance_subsampling;

	WriteSettings(const std::string &path = "", bool alpha = false, int jpeg_q = 80, int chroma_ss_repr = 420);
};

using write_function = std::function<void(const WriteSettings&, const ImageData&)>;
using write_function_ptr = void (*)(const WriteSettings&, const ImageData&);

write_function get_write_function(const std::string &file_ext);

void write_as_jpeg(const WriteSettings& settings, const ImageData &data);
void write_as_png(const WriteSettings& settings, const ImageData &data);
void write_as_bmp(const WriteSettings& settings, const ImageData &data);

/**
 * Misc
 */
constexpr CHROMINANCE_SUBSAMPLING int_representation_to_chroma_ss(int chroma_subsampling_representation);

bool is_chroma_subsampling_representation_valid(const int &chroma_subsampling_representation);
bool is_quality_valid(const int &quality);

bool is_extension_valid(const std::string &ext);

void exit_if_nullptr(std::unique_ptr<ImageData> &ptr);