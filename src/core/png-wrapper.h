#pragma once

#include "types.h" // Color, Point, ImageData

/**
 * @param alpha defines if output image should have an alpha channel
 * specifies whether to save it with a bit depth of 24 bpp or 32 bpp (bits per pixel)
 * 32 bpp images use the last 8 bits to store the alpha channel
 */
void write_as_png(const char* const filename, const ImageData &data, bool alpha);
void write_as_png(const char* const filename, const Color *colors, uint width, uint height, bool alpha);

std::unique_ptr<ImageData> read_as_png(const char* const filename);