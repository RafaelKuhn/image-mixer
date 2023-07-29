#pragma once

#include <memory>

#include "types.h"     // Color, ImageData
#include "turbojpeg.h" // TJSAMPS

#ifdef __TURBOJPEG_H__
enum CHROMINANCE_SUBSAMPLING {
	SUBSAMPLING_444 = TJSAMP_444,
	SUBSAMPLING_422 = TJSAMP_422,
	SUBSAMPLING_420 = TJSAMP_420,
	SUBSAMPLING_411 = TJSAMP_411, // (unoptimized by library)
	SUBSAMPLING_GRAY = TJSAMP_GRAY
};
#else
enum CHROMINANCE_SUBSAMPLING {
	SUBSAMPLING_444 = 0,
	SUBSAMPLING_422 = 1,
	SUBSAMPLING_420 = 2,
	SUBSAMPLING_411 = 5,
	SUBSAMPLING_GRAY = 3
};
#endif

/**
 * @param jpeg_quality jpeg_quality goes from 0 (worst) to 100 (best)
 * 90 will reduce significantly giving a high-quality image
 * 80 greatly reduces the size with almost no quality reduction
 * 70 and lower begins to show noticeable differences, but compresses even further
 * default is 100, recommended is 80
 * @param chrominance_subsampling how and how much
 * of the chroma component will be cut off the compressed image,
 * more details in turbojpeg header file,
 * default is SUBSAMPLING_444, recommended is SUBSAMPLING_420
 */
void write_as_jpeg(const char* const filename, const ImageData &data, int jpeg_quality = 100, int chrominance_subsampling = SUBSAMPLING_444);
void write_as_jpeg(const char* const filename, const Color *colors, uint width, uint height, int jpeg_quality = 100, int chrominance_subsampling = SUBSAMPLING_444);

std::unique_ptr<ImageData> read_as_jpeg(const char* const filename);