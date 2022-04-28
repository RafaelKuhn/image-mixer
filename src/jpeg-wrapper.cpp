#include <iostream>
#include <fstream>

#include <cstring>

#include "jpeg-wrapper.h"

void write_as_jpeg(const char* const fn, const ImageData &data, int jq, int ch_ss)
{
	write_as_jpeg(fn, data.colors, data.get_width(), data.get_height(), jq, ch_ss);
}

void write_as_jpeg(const char* const fn, const Color *cl, uint w, uint h, int ql, int ch_ss)
{
	tjhandle jpeg_handle = tjInitCompress();

	if (jpeg_handle == NULL) {
		const char *err = (const char *) tjGetErrorStr();
		std::cerr << "TJ Error: " << err << " UNABLE TO INIT TJ Compressor Object\n";
		return;
	}

	// TODO: greyscale jpeg support
	// int pixelFormat = TJPF_GRAY;
	// int ch_ss = TJSAMP_GRAY;
	// int color_channel_amount = 1;
	
	int color_channel_amount = 3;
	int pixel_format = TJPF_RGB;

	unsigned char* raw_image_buffer = new unsigned char[w * h * color_channel_amount];
	for (uint j = 0; j < h; j++) {
		for (uint i = 0; i < w; i++) {
			int index = j * w + i;
			raw_image_buffer[index * color_channel_amount + 0] = cl[index].r;
			raw_image_buffer[index * color_channel_amount + 1] = cl[index].g;
			raw_image_buffer[index * color_channel_amount + 2] = cl[index].b;
		}
	}
	
	unsigned long jpeg_size = 0;
	unsigned char* compressed_jpeg_buffer = NULL;
	int tj_stat = tjCompress2(jpeg_handle, raw_image_buffer, w, 0, h, pixel_format, &compressed_jpeg_buffer, &jpeg_size, ch_ss, ql, 0);
	if (tj_stat != 0) {
		const char* err = (const char *)tjGetErrorStr();
		std::cerr << "TurboJPEG Error: " << err << " UNABLE TO COMPRESS JPEG IMAGE\n";
		// std::cerr << "TurboJPEG Error: " << tjGetErrorStr() << " UNABLE TO COMPRESS JPEG IMAGE\n";
		tjDestroy(jpeg_handle);
		return;
	}

	// TODO: better C++ port, change to ofstream / try catch
	FILE *file = fopen(fn, "wb");
	if (!file) {
		std::cerr << "Could not open JPEG file: " << std::strerror(errno) << "\n";
		return;
	}

	int file_write_status = fwrite(compressed_jpeg_buffer, jpeg_size, 1, file);
	if (file_write_status < 1) {
		std::cerr << "Could not write JPEG file: " << std::strerror(errno) << "\n";
		return;
	}

	fclose(file);

	tjDestroy(jpeg_handle);
	free(raw_image_buffer);
}

std::unique_ptr<ImageData> read_as_jpeg(const char* const filename)
{
	std::ifstream file(filename, std::ios_base::binary);
	if (file.fail()) {
		std::cerr << "error: file not found!\n";
		exit(1);
	}
	
	file.seekg(0, file.end);
	int file_size = file.tellg();
	
	file.seekg(0, file.beg);

 	//TODO: make unique
	unsigned char* compressed_image_data = new unsigned char[file_size];
	file.read((char*)compressed_image_data, file_size);

	int width = 0;
	int height = 0;
	int jpeg_subsamp = 0;
	int bytes_per_px = 3;

	tjhandle _jpegDecompressor = tjInitDecompress();
	tjDecompressHeader2(_jpegDecompressor, compressed_image_data, file_size, &width, &height, &jpeg_subsamp);

	auto img_data = std::make_unique<ImageData>(width, height);

	// TODO: make unique and .get
	unsigned char* output_buffer = new unsigned char[width * height * bytes_per_px];

	int status = tjDecompress2(_jpegDecompressor, compressed_image_data, file_size, output_buffer, width, 0 /* pitch */, height, TJPF_RGB, TJFLAG_FASTDCT);
	if (status != 0) {
		std::cout << "TurboJPEG Error: " << tjGetErrorStr2(_jpegDecompressor) << "\n";
	}
#ifdef DEBUG_MODE
	std::cout << "png w: " << img_data->get_width() << "h: " << img_data->get_height() << "\n";
	std::cout << "header decompressed\n";
	std::cout << "bytes_per_px: " << bytes_per_px << "\n";
	std::cout << "subsampling: " << jpeg_subsamp << "\n";
	std::cout << "tj decompressed, status code:" << status << "\n"; // when this is -1, error
	std::cout << "status: " << tjGetErrorStr2(_jpegDecompressor) << "\n";
	std::cout << "size of decompressed image is " << (width*height*bytes_per_px) << " B | " << (width*height*bytes_per_px)/(float)1024 << " KB | " << (width*height*bytes_per_px)/(float)(1024*1024) << "MB \n";
#endif
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int xy_index = y * width + x;
			int r = output_buffer[xy_index * bytes_per_px + 0];
			int g = output_buffer[xy_index * bytes_per_px + 1];
			int b = output_buffer[xy_index * bytes_per_px + 2];

			Color col(r, g, b);
			img_data->colors[xy_index] = col;
		}
	}

	tjDestroy(_jpegDecompressor);
	
	delete[] compressed_image_data;
	delete[] output_buffer;
	
	return img_data;
}