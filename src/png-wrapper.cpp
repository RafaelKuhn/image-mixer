#include <memory>
// TODO: remove aborts, change to real messages
#include <stdlib.h>

#include "bmp-types.h" // Color, Point, ImageData

#include "png-wrapper.h"
#include "png.h"

// TODO DEBUG
// #include <iostream>

std::unique_ptr<ImageData> read_as_png(const char* const filename)
{
	FILE *fp = fopen(filename, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	
	if (!info) abort();
	if (setjmp(png_jmpbuf(png))) abort();
	png_init_io(png, fp);

	png_read_info(png, info);

	int width  = png_get_image_width(png, info);
	int height = png_get_image_height(png, info);
	unsigned char color_type = png_get_color_type(png, info);
	unsigned char bit_depth  = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt
	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// TODO: try to prevent pnglib from reading unnecessary alpha bytes
	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	auto image_data = std::make_unique<ImageData>(width, height);

	int bytes_per_px = 4;
	auto rgba_row = std::make_unique<uint8_t[]>(width * bytes_per_px);
	
	for (int y = 0; y < height; ++y) {
		// raw row of color bytes
		png_read_row(png, rgba_row.get(), NULL);
		// int r = rgba_row[0]; int g = rgba_row[1]; int b = rgba_row[2];
		// std::cout << "row " << y << " starts with " << (uint)r << " " << (uint)g << " " << (uint)b << " \n";
		for (int x = 0; x < width; ++x) {
			// 4 is bytes_per_px
			int r = rgba_row[x*4 + 0];
			int g = rgba_row[x*4 + 1];
			int b = rgba_row[x*4 + 2];
			// TODO: handle alpha
			// int a = argb_row[x*4+3]
			
			Color col(r, g, b);

			// to read y inverted:
			// image_data->colors[(height - y - 1) * width + x] = col;
			image_data->colors[y * width + x] = col;
		}
	}

	fclose(fp);
	png_destroy_read_struct(&png, &info, NULL);

	return image_data;
}

void write_as_png(const char* const filename, const ImageData &data)
{
	write_as_png(filename, data.colors, data.get_width(), data.get_height());
}

// "char const * const filename" also works for "const pointer of const data", lol
void write_as_png(const char* const filename, Color *colors, uint width, uint height)
{
	FILE *fp = fopen(filename, "wb");
	if(!fp) abort();

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();
	
	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		png,
		info,
		width, height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//   png_set_filler(png, 0, 1);

	int bytes_per_px = 4;
	auto rgba_row = std::make_unique<uchar[]>(width * bytes_per_px);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			// 4 is bytes_per_px
			rgba_row[x*4 + 0] = colors[index].r;
			rgba_row[x*4 + 1] = colors[index].g;
			rgba_row[x*4 + 2] = colors[index].b;
			rgba_row[x*4 + 3] = (uchar)255;
			
			// TODO: handle alpha
			//rgba_row[x * 3] = colors[index].a;
		}

		png_write_row(png, rgba_row.get());
	}

	png_write_end(png, NULL);

	fclose(fp);
	png_destroy_write_struct(&png, &info);
}