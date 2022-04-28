#pragma once

// this is compiled under bmp-lib submodule

typedef unsigned int uint;
typedef unsigned char uchar;


template <typename T>
struct Point {
	T x;
	T y;
};

/// Data container for rgb color
struct Color {
	char r{0}, g{0}, b{0};

	Color(); // TODO: should have this? could have maybe a 'to int' method
	Color(int r, int g, int b);
	Color(const char* const little_endian_3byte_bgr_arr);

};

/// Data container for image width, height and pointer to pixel colors
class ImageData {
private:
	uint _width;
	uint _height;

public:
	// TODO: could be a shared pointer, but too much overhead for my nooby c++
	Color* colors;
	
	ImageData(uint width, uint height);
	
	~ImageData();

	ImageData* clone() const;

	constexpr uint get_height() const {	return _height; }
	constexpr uint get_width() const { return _width; }

};

