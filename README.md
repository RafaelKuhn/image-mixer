<h1 align="center">Image Mixer</h1>
<p align="center">
Simple terminal applications in C++ to mix and demix rgb (+a) color channels<br>
Mix | Demix | Encode
</p>
<h2 align="center"> TODO: COOL IMAGE HERE</h2>

Supported image formats: **png, jpeg & bmp**

## DEMIX PROGRAM
Segregates an image into three others, comprising of the image's three color channels.<br>
(can also be work with alpha)<br><br>

- **Examples**<br>
	- `$ ./demix "img.png"` will create files "img-r.png", "img-g.png", and "img-b.png"<br>
	- `$ ./demix -a "img.png"` will create files "img-r.png", "img-g.png", "img-b.png", and "img-a".png"<br>

- **Options / Flags**<br>
	- -m: specify output mode, see Output Modes below<br>
	modes: `rgb`, `grey`<br>
	if not specified, will use `rgb` mode<br>
	examples: `-m grey`, `-m rgb`
	- -a: include alpha channel<br>
	also creates "img-a.ext", which has the alpha data, the rgb data is filled with white pixels (0xFFFFFF) as most image image viewers display background as black
	- -h: print help

- **Output Modes**<br>
	- **RGB**: each color channel will create an image with only that channel in the image, a "0x88FFcc" pixel will become, in the "-r" image, "0x880000", discards all alpha in these images<br>
	if used with -a (alpha), will also create a white image with the alpha data as its alpha channel

	- **GREY**: each channel will create an image with that channel represented in greyscale, a "0x88FFcc" pixel in the red image will become "0x88"<br>
	if used with -a (alpha) option, will create a grey representation of the input image's alpha channel
	
		(default mode is RGB)

- **Input**
	- Path to the file to be demixed

## MIX PROGRAM
Creates an image comprised of three others, taking the red, green, and blue channels of, respectively, the first, second and third arguments and putting using them as the channels of the new image.<br>
(can also work with alpha)<br>

- **Notes**<br>
	- The size of the remaning image is comprised of the biggest width and height among the specified images
	- Default output image is named "mix-output.png", name and format (extension) can be overriden with `-o` argument<br>

- **Examples**<br>
	- `$ ./mix "r.png" "g.png" "blue.png"` will create the file "mix-output.png" using the images' red, green, and blue channels as color channels in the output image<br>
	- `$ ./mix -o "what-have-i-done.bmp" "r.jpg" "g.bmp" "b.png" "stencil.png"` will create the file "what-have-i-done.bmp" (already encoded as BMP) using the images' red, green, blue, and alpha channels as color channels in the output image<br>

- **Default Behavior**<br>
	- if one of the inputs is "_", that channel will be skipped<br>
	example: `$ ./mix img.png _ img.png` will create an image with only the red and blue channels, filling green with zeros
	- if 4 images are specified, will apply -a option and use the fourth image's alpha in the output
	- if 1, 2 or more than 4 images are specified, the program will delete all your files<br>(just kidding, it will throw an error)<br>

- **Options / Flags**<br>
	- -a: use alpha channel<br>
	includes an alpha channel at the resulting image (if fourth input image is not specified, fills it with "FF").
	- -o: overrides output file name, encodes the output using the new filename's extension<br>
	example: `-o "new-file-name.png"`, will generate a .png image (already with the `png` encoding and extension), the extension must be supported (PNG, JPG, BMP)<br>
	- -h: print help

- **Input**
	- Path to the files to be mixed, respectively R, G, B and A

## ENCODE PROGRAM
This just changes the image format into another (can be used to remove alpha channels or change internal image encoding)
- **Examples**<br>
	- `$ ./encode jpeg "img.png"` will create file "img.jpg"<br>
	translating input image's format to jpeg
	- `$ ./encode -q 80 img.jpg` will create file "output.jpg"<br>
	-q option is used to reduce jpeg quality (0 to 100)
	- `$ ./encode -r img.png` will create file "output.png"<br>
	-r option is used to remove the alpha channel from pngs and bmps, turning a 32bpp image into a 24bpp one
	- `$ ./encode bmp "cool-photo.jpg" -o "legacy-photo.bmp"` will create file "legacy-photo.bmp"

- **Options / Flags**<br>
	- -o: output file<br>
	default is output.\[ext\]
	- -r: remove alpha from file (png | bmp output only)<br>
	default is FALSE if input is a JPEG, otherwise TRUE
	- -q: change quality, from 0 to 100 (jpeg output only)<br>
	default is 80
	- -c: chrominance subsampling, see table below (jpeg output only)<br>
	default is 420

- **Chrominance Subsampling**<br>
This changes the "chroma color precision" of the jpeg, reduces chroma color precision in the yUV color representation, but reduces file size
	| value       | Proportion |
	| ----------- | ----------- |
	| 444         | 4:4:4 (don't mess with colors)|
	| 422         | 4:2:2 (reduces precision to half)|
	| 420         | 4:2:0 (reduces precision to a quarter, horizontally)|
	| 411         | 4:1:1 (reduces precision to a quarter, vertically)|

- **Input**
	- Output format, example: "jpeg", "png", "bmp"
	- Input file

## DEPENDENCIES
- libturbojpeg
- libpng
- zlibstd (libpng depends on zlib, I use zlibstd because it's faster)<br>
also depends on a small bmp lib, included as a submodule (homemade)

## BUILD
Programs Mix | Demix | Encode can be built using Make and gcc (g++)<br>
First download the dependencies with the appropriate package manager<br>
For an example, in Mingw-w64 under MSYS2:
- `$ pacman -S mingw64/mingw-w64-x86_64-libpng`
- `$ pacman -S mingw64/mingw-w64-x86_64-libjpeg-turbo`
- `$ pacman -S msys/libzstd`

With the dependencies installed and gcc include path configured:
- `$ make`
- will generate files: `mix.exe`, `demix.exe` and `encode.exe`

## POSSIBLE TODOS:
* file support
	* ~~pnm images (.pbm, .pgm, .ppm, and .pam)~~ (checking if it's worth it)
	* webp images (.webp)
	* output as grayscale (actual 8bit grayscale)
* alpha handling
	* for png
	* for bmp
* demix
	* (-a) option, alpha handling
* mix
	* args parsing
	* file format parsing
	* algorithm

## PS
(this doesn't work on Big-endian IBM mainframes with retarded byte parsing lol)