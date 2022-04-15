#include <iostream>  // cout
#include <memory>    // unique_ptr
 
#include <string>    // string, stringstream, to_string

#include "bmp-types.h"   // Color, Point, ImageData
#include "bmp.h"         // read_as_bmp, write_as_bmp
#include "png-wrapper.h" // read_as_png, write_as_png

#include <fstream>   // ofstream

using std::cout;


#ifdef DEBUG_MODE
std::ostream &operator << (std::ostream &out, const Color &col)
{
	out << "[" << (uint)(uchar)col.r << "," << (uint)(uchar)col.g << "," << (uint)(uchar)col.b << "]";
	return out;
}
#endif


void draw_uv_gradient(Color* output, int width, int height)
{
#ifdef DEBUG_MODE
	cout << "drawing uv gradient on canvas with width " << width << ", height " << height << "\n\n";
#endif
	int _height = (height == 1) ? 2 : height;
	int _width = (width == 1) ? 2 : width;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			output[y * width + x] = { (x * 255)/(_width-1), (y * 255)/(_height-1), 0 };
		}
	}
}

void draw_circle(Color* output, Point<int> center, int radius, int width, int height)
{
#ifdef DEBUG_MODE
	cout << "drawing circle with radius " << radius << ", center at " << center.x << "," << center.y << "\n\n";
	cout << "on canvas with width " << width << ", height " << height << "\n";
#endif
	float radiusf = (float)radius;

	float i, j;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			// by summing 0.5, we calculate the distance from the center of the pixel, not the top left corner of it
			i = x + 0.5f - center.x;
			j = y + 0.5f - center.y;
			
			// x^2 + y^2 > radius^2
			if (i*i + j*j < radiusf*radiusf)
				output[y * width + x] = { 255, 0, 0 };

			else
				output[y * width + x] = { 0, 0, 0 };
		}
	}
}


// TODO: move to color implementation
#ifdef DEBUG_MODE
void print_color_data_by_ref(std::unique_ptr<Color[]> &data, int width, int height)
{
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Color el = data[y * width + x];
			cout << "[" << el.r << "," << el.g << "," << el.b << "] ";
		}
		cout << "\n";
	}
}
#endif

int main(int argc, const char* argv[])
{
	if (argc != 4) {
		cout << "this needs 3 arguments, 1st is file to be read (as png)\n";
		cout << "2 is path to save the read file as png\n";
		cout << "3 is path to save the read file as bmp\n";
		exit(1);
	}

	int width = 426;
	int height = 240;

	// create a 240p uv gradient and write it into an image file (argv[1])
	// {
	// 	std::unique_ptr<Color[]> arr = std::make_unique<Color[]>(height * width);

	// 	draw_uv_gradient(arr.get(), width, height);
	// 	write_as_png(argv[1], arr.get(), width, height);
	// }

	// read the gradient from disc and write it as a copy
	{
		std::unique_ptr<ImageData> data = read_as_png(argv[1]);

		if (data) {
			write_as_png(argv[2], *data);
			write_as_bmp(argv[3], *data);
		}
	}

	// width  = 256;
	// height = 144;

	// // create a 144p circle and write it into an image file
	// {
	// 	auto img_data = std::make_unique<ImageData>(width, height);
		
	// 	Point<int> center = { width/2, height/2 };
	// 	int radius = height/2;
	// 	draw_circle(img_data->colors, center, radius, img_data->get_width(), img_data->get_height());

	// 	write_as_bmp("bmp/circle.bmp", img_data->colors, width, height);
	// }
	
	// std::cin.get();
	cout << "program ended! big success!\n";
}