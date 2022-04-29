#include <iostream>
#include <fstream>

#include <cstring>

#include "jpeg-wrapper.h"


void write_as_jpeg(const char* const filename, const ImageData &data, int j_ql, int ch_ss)
{
	write_as_jpeg(filename, data.colors, data.get_width(), data.get_height(), j_ql, ch_ss);
}

void write_as_jpeg(const char* const filename, const Color *data, uint w, uint h, int j_ql, int ch_ss)
{
	tjhandle jpeg_handle = tjInitCompress();

	if (jpeg_handle == NULL) {
		std::cerr << "TJ Error: " << tjGetErrorStr() << " UNABLE TO INIT TJ Compressor Object\n";
		return;
	}

	// TODO: greyscale jpeg support
	// int pixelFormat = TJPF_GRAY;
	// int ch_ss = TJSAMP_GRAY;
	// int color_channel_amount = 1;
	
	int color_channel_amount = 3;
	int pixel_format = TJPF_RGB;

	auto input_image_buffer_ptr = std::make_unique<unsigned char[]>(w * h * color_channel_amount);
	for (uint y = 0; y < h; y++) {
		for (uint x = 0; x < w; x++) {
			int index = y * w + x;

			input_image_buffer_ptr[index * color_channel_amount + 0] = data[index].r;
			input_image_buffer_ptr[index * color_channel_amount + 1] = data[index].g;
			input_image_buffer_ptr[index * color_channel_amount + 2] = data[index].b;
		}
	}
	
	unsigned long jpeg_size = 0;
	unsigned char* compressed_jpeg_buffer = NULL;
	int tj_stat = tjCompress2(jpeg_handle, input_image_buffer_ptr.get(), w, 0, h, pixel_format, &compressed_jpeg_buffer, &jpeg_size, ch_ss, j_ql, 0);
	if (tj_stat != 0) {
		std::cerr << "TurboJPEG Error: " << tjGetErrorStr() << " UNABLE TO COMPRESS JPEG IMAGE\n";
		tjDestroy(jpeg_handle);
		return;
	}

	std::ofstream file(filename, std::ios_base::binary);
	if (!file) {
		std::cerr << "[os error] could not open file: " << std::strerror(errno) << "\n";
		return;
	}

	file.write(reinterpret_cast<const char*>(compressed_jpeg_buffer), jpeg_size);
	
	tjDestroy(jpeg_handle);
}

std::unique_ptr<ImageData> read_as_jpeg(const char* const filename)
{
	std::ifstream file(filename, std::ios_base::binary);
	if (file.fail()) {
		std::cerr << "error: file not found!\n";
		return nullptr;
	}
	
	file.seekg(0, file.end);
	int file_size = file.tellg();
	
	file.seekg(0, file.beg);

 	//TODO: make unique
	// unsigned char* compressed_image_data = new unsigned char[file_size];
	auto compressed_image_data_ptr = std::make_unique<unsigned char[]>(file_size);
	file.read((char*)compressed_image_data_ptr.get(), file_size);

	int width = 0;
	int height = 0;
	int jpeg_subsamp = 0;
	int bytes_per_px = 3;

	tjhandle _jpegDecompressor = tjInitDecompress();
	tjDecompressHeader2(_jpegDecompressor, compressed_image_data_ptr.get(), file_size, &width, &height, &jpeg_subsamp);

	auto output_buffer_ptr = std::make_unique<unsigned char[]>(width * height * bytes_per_px);

	int status = tjDecompress2(_jpegDecompressor, compressed_image_data_ptr.get(), file_size, output_buffer_ptr.get(), width, 0 /* pitch */, height, TJPF_RGB, TJFLAG_FASTDCT);
	if (status != 0) {
		std::cerr << "TurboJPEG Error: " << tjGetErrorStr2(_jpegDecompressor) << "\n";
	}
	
	auto img_data = std::make_unique<ImageData>(width, height);

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
			
			int r = output_buffer_ptr[xy_index * bytes_per_px + 0];
			int g = output_buffer_ptr[xy_index * bytes_per_px + 1];
			int b = output_buffer_ptr[xy_index * bytes_per_px + 2];

			Color col(r, g, b);
			img_data->colors[xy_index] = col;
		}
	}

	tjDestroy(_jpegDecompressor);
	
	return img_data;
}