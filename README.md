<h1 align="center">Image Mixer</h1>
<p align="center">
Simple terminal applications in C++ to mix and demix rgb (+a) color channels<br>
Mix | Demix | Encode
</p>
<h2 align="center"> TODO: COOL IMAGE HERE</h2>

Supported image formats: **png, jpeg & bmp**

## DEMIX PROGRAM
Segregates an image into three others, comprising of the image's three color channels.  
(can also be work with alpha)  

- **Examples**  
	- `$ demix "img.png"` will create files `img-r.png`, `img-g.png`, and `img-b.png`  
	- `$ demix -a img.png` will create files `img-r.png`, `img-g.png`, `img-b.png`, and `img-a.png`  

- **Input**
	- Path to the file to be demixed

- **Options / Flags**
	- `-g`: use `GREY` output mode, see Output Modes below  
	if not supplied, will use `RGB` mode  
	- `-a`: include alpha channel  
	also creates "img-a.ext", which has the alpha data, the rgb data is filled with white pixels (0xFFFFFF) as most image image viewers display background as black
	- `-h`: print help

- **Output Modes**  
	- **RGB**: each color channel will create an image with only that channel in the image, a "0x88FFcc" pixel will become, in the "-r" image, "0x880000", discards all alpha in these images  
	if used with -a (alpha), will also create a white image with the alpha data as its alpha channel

	- **GREY**: each channel will create an image with that channel represented in greyscale, a "0x88FFcc" pixel in the red image will become "0x88"  
	if used with -a (alpha) option, will create a grey representation of the input image's alpha channel
	
		(default mode is RGB)

## MIX PROGRAM
Creates an image comprised of three others, taking the red, green, and blue channels of, respectively, the first, second and third arguments and using them as the RGB channels of the new image.  
(can also work with alpha)  

- **Examples**  
	- `$ mix "r.png" "g.png" "blue.png"` will create the file `output.png` using the images' red, green, and blue channels as color channels in the output image  
	- `$ mix -o "what-have-i-done.bmp" "r.jpg" "g.bmp" "b.png" "stencil.png"` will create the file "what-have-i-done.bmp" (already encoded as BMP) using the images' red, green, blue, and alpha channels as color channels in the output image  

- **Input**
	- Path to the files to be mixed, respectively R, G, B and A  

- **Options / Flags**  
	- `-o`: output file name and format, encodes the output using the new filename's extension  
	example:  
	`$ mix -o "new-file-name.jpg"` will generate a `jpeg` image (already with the correct encoding and extension)  
	the extension must be supported (check header of this)  
	- `-h`: print help

- **Default Behavior**  
	- if one of the inputs is or starts with "_", that channel will be skipped (filled with zeros)  
	example:  
	`$ mix img.png _ img.png` will create an image with only the img's red and blue channels, filling green with zeros
	- if no output is informed, the default image will be a `PNG` named `output.png`
	- if 4 images are specified, will use the fourth image's alpha in the output image  
	when less than 4 images are specified, output image doesn't need to have an alpha channel at all
	- if more than 4 images are specified, the program will encrypt all your files and corrupt your operating system  
	(just kidding, it will throw an error)  
	- in `jpeg` outputs, quality will be `100` and chrominance subsampling `4:4:4` (can be changed with encode program)

- **Notes**
	- The dimensions of the remaning image are the biggest width and height among the specified images  
	- Default output image is named "output.png", name and format (extension) can be overriden with `-o` argument  

## ENCODE PROGRAM
This just changes the image format into another (can be used to remove alpha channels or mess with jpeg settings)  

- **Examples**  
	- `$ encode jpeg "img.png"` will create file "img.jpg"  
	changing input image's format to `jpeg`
	- `$ encode -a "img.png"` will create file "img.jpg"  
	changing input image's format to `jpeg`
	- `$ encode -q 80 img.jpg` will create file "output.jpg"  
	-q option is used to reduce jpeg quality (0 to 100)
	- `$ encode -r img.png` will create file "output.png"  
	-r option is used to remove the alpha channel from pngs and bmps, turning a 32bpp image into a 24bpp one
	- `$ encode bmp "cool-photo.jpg" -o "legacy-photo.bmp"` will create file "legacy-photo.bmp"

- **Input**
	- Output format, example: "jpeg", "png", "bmp"
	- Input file

- **Options / Flags**  
	- `-o`: output file  
	if you choose to save a "jpeg" encoded image with "-o image.bmp", that's your problem  
	- `-r`: remove alpha from file (png | bmp output only)  
	if the input is a JPEG, this will be on by default, jpegs don't have alpha, so there's no reason to create a png with alpha from them  
	- `-q`: change the jpeg quality, from 0 to 100 (jpeg output only)  
	default is 80
	- `-c`: chrominance subsampling, see table below (jpeg output only)  
	default is 420

- **Chrominance Subsampling**  
This changes the "chroma color precision" of the jpeg, reducing file size  
	| value       | Proportion |
	| ----------- | ----------- |
	| 444         | 4:4:4 (don't mess with colors)|
	| 422         | 4:2:2 (reduces precision to half)|
	| 420         | 4:2:0 (reduces precision to a quarter, horizontally)|
	| 411         | 4:1:1 (reduces precision to a quarter, vertically)|


## DEPENDENCIES
- libturbojpeg
- libpng
- zlibstd (libpng depends on zlib, I use zlibstd because it's faster)<br>
also depends on a small bmp lib, included as a submodule (homemade)

## BUILD
This is built using Make and G++  
First download the dependencies with the appropriate package manager  
> For an example, in Mingw-w64 under MSYS2, run:  
`$ pacman -S mingw64/mingw-w64-x86_64-libpng`  
`$ pacman -S mingw64/mingw-w64-x86_64-libjpeg-turbo`  
`$ pacman -S msys/libzstd`  

With the dependencies installed and gcc include path configured, run:  
`$ make`  

will generate files: `mix`, `demix` and `encode`  
and on Windows, its `.exe` variants

## INSTALL
`$ make install`  
Will build all the stuff (if not already built) and dump it into `/usr/bin` folder to be available globally  

`$ make uninstall`  
Will delete the stuff from there


## POSSIBLE TODOS:
> not that they will ever be done
- alpha handling in programs for supported formats
- new program: messer, messes with the image (lol)  
could be used to crop, reescale, rotate, warp and shit    
- help `-h` option in programs (until then, only me, this readme, can help you ;-)
- file support
	- webp images (.webp)
	- pnm images (.pbm, .pgm, .ppm, and .pam) (checking if it's worth it)
	- encode output as grayscale (actual 8bit grayscale, not grey representation)


## PS
this doesn't work on Big-endian IBM mainframes
