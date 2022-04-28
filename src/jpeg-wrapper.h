#include <memory>

#include "turbojpeg.h" // TJSAMPS
#include "types.h" // Color, Point, ImageData

/** 8 -> 8  */
#define SUBSAMPLING_444  TJSAMP_444
/** 8 -> 2,2 / 2,2  */
#define SUBSAMPLING_422  TJSAMP_422
/** 8 -> 4 , 4  */
#define SUBSAMPLING_420  TJSAMP_420
/** 8 -> 4 / 4 (unoptimized by library) */
#define SUBSAMPLING_411  TJSAMP_411
/** 0 */
#define SUBSAMPLING_GRAY TJSAMP_GRAY

/**
 * @param jpeg_quality jpeg_quality goes from 0 (worst) to 100 (best)
 * 90 will reduce significantly giving a high-quality image
 * 80 greatly reduces the size with almost no quality reduction
 * 70 and lower begins to show noticeable differences, but compresses even further
 * default is 100, recommended is 80
 * @param chrominance_subsampling chrominance subsampling, basically how and how much
 * of the chroma component will be cut off the compressed image
 * more details in turbojpeg header file
 * default is SUBSAMPLING_444, recommended is SUBSAMPLING_420
 */
void write_as_jpeg(const char* const filename, const ImageData &data, int jpeg_quality = 100, int chrominance_subsampling = SUBSAMPLING_444);
void write_as_jpeg(const char* const filename, const Color *colors, uint width, uint height, int jpeg_quality = 100, int chrominance_subsampling = SUBSAMPLING_444);

std::unique_ptr<ImageData> read_as_jpeg(const char* const filename);