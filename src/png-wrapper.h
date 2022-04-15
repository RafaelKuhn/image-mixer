#pragma once

#include "bmp-types.h" // Color, Point, ImageData

std::unique_ptr<ImageData> read_as_png(const char* const filename);

void write_as_png(const char* const filename, Color *colors, uint width, uint height);
void write_as_png(const char* const filename, const ImageData &data);