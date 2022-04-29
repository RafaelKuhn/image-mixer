<h1 align="center">Image Mixer</h1>
<p align="center">
C++ terminal application for Windows to mix and demix rgb (+a) color channels
<h2 align="center"> TODO: COOL IMAGE HERE<h1>
</p>

## DEMIX PROGRAM
Segregates an image into three others, comprising of the image's three color channels<br>
(can also be configured to work with alpha)<br>

- **Examples**<br>
	- `$ ./demix "img.png"` will create files "img-r.png", "img-g.png", and "img-b.png"<br>
	- `$ ./demix -a "img.png"` will create files "img-r.png", "img-g.png", "img-b.png", and "img-a".png"<br>

- **Output Modes**<br>
	- **RGB**: each color channel will create an image with only that channel in the image, a "0x88FFcc" pixel in the red image will become "0x880000", discards all alpha in these images<br>
	if used with -a (alpha), will also create a white image with the alpha data as its alpha channel.

	- **GREY**: each channel will create an image with that channel represented in greyscale, a "0x88FFcc" pixel in the red image will become "0x88"<br>
	if used with -a (alpha) option, will create a greyscale representation of the input image's alpha channel.
	
		(default mode is RGB)

- **Args**<br>
	- -m: specify mode ~~(case insensitive)~~<br>
	modes: `rgb`, `grey`<br>
	if not specified, will use `rgb` mode<br>
	examples: `-m grey`, `-m rgb`
	- -a: include alpha channel<br>
	also creates "img-a.ext", which has the alpha data, the rgb data is filled with white pixels (0xFFFFFF) as most image image viewers display background as black.<br>
	generated R-G-B images have all alphas filled with "FF" (meaning they're opaque)
	- last argument without a prefix is always the path to the image to be demixed.<br>
	examples: `./demix -a "C:/users/anon/landscape.jpg" -m rgb`, `./demix  "my-image.png"`
## MIX PROGRAM

Creates an image comprised of three others, taking the red, green, and blue channels of, respectively, the first, second and third image and creating one image comprising the three<br>
(can also be configured to work with alpha)<br>

- **Notes**<br>
	- The size of the remaning image is comprised of the biggest width and height among the specified images
	- Default output image is named "output.png", name and format (extension) can be overriden with `-o` argument<br>

- **Examples**<br>
	- `$ ./mix "r.png" "g.png" "blue.png"` will create the file "output.png" using the images' red, green, and blue channels as color channels in the output image.<br>
	- `$ ./mix -o "what-have-i-done.bmp" "r.jpg" "g.bmp" "b.png" "stencil.png"` will create the file "what-have-i-done.bmp" (already encoded as BMP) using the images' red, green, blue, and alpha channels as color channels in the output image.<br>
	- ~~`$ ./mix -m ADD -o "starfield.png" "field.jpg" "star-particles.png"` will perform an additive blending between "field.jpg" colors and "star-particles.png" and save it as the file "starfield.png" (already encoded as PNG)~~.<br>

- **Default Modes** (check Modes below)<br>
	- if 1 image specified, default mode is ~~PRUNE~~ ERROR<br>
	- if 2 or 3 images specified, default mode is RGB<br>
	- if 4 images specified, mode is RGB with -a applied<br>
	- if more than 4 images specified, the program will encrypt your operating system (just kidding, it will throw an error)<br>
	
- **Modes**<br>
	- *RGB*: uses the images' red, green, and blue channels as color channels in the output image (if only two images specified, will only fill red and green channels, blue will be zero)<br>

	- ~~*PRUNE*: cleans "visible trash" data from image, like an alpha channel comprised of only "FF"s in a png, if all the pixels have "FF" in alpha, that means it shouldn't be spending space using an alpha channel<br>
	also, if alpha is 0 at certain pixel, we can null the color data from there (as 0x000000), so the image formats (png and jpeg) may have a chance at optimizing them~~<br>

	- ~~*ADD*: will perform additive blending, just adding all color channels of the images together~~<br>
	- ~~*MUL*: will perform multiplicative blending, multiplying the first image by the second~~

- **args**<br>
	- -a: use alpha channel<br>
	includes an alpha channel at the resulting image (if fourth input image is not specified, fills it with "FF").
	- -o: overrides output file name, encodes the output using the new filename's extension<br>
	example: `-o "new-file-name.png"`, will generate a .png image (already with the `png` encoding and extension), the extension must be supported (PNG, JPG, BMP, ~~PNM (see PNM variants)~~).<br>
	- last three arguments without prefixes will become the paths to the images to be mixed, separated by a space character<br>
	examples:<br>
	`"r.jpg" "g.jpg" "b.jpg" -o "rgb.bmp"`<br>
	`-a -o "mixed.png" "r.png" "g.png" "b.png" "stencil.png"`<br>

## BUILD
Program can be built using gcc (g++) for Mingw-w64, compiled libs (libpng, zlib and libjpeg) are already in the project for laziness sake.
- run `$ make` to compile
- will generate files: `mix` and `demix`

## TODOS:
* ~~support read/write of pnm images (.pbm, .pgm, .ppm, and .pam) (checking if it's worth it)~~
* support read/write of jpeg images (.jpg, .jpeg)
* alpha handling
	* ~~for pnm (.pam only)~~
	* for png
	* for bmp
* makefile targets and incremental build
* demix
	* file format parsing
	* algorithm
* mix
	* args parsing
	* file format parsing
	* algorithm

## TO-DONES:
* read/write images as windows bitmap (.bmp)
* read/write images as png (.png)
* demix
	* args parsing

<br>
(this doesn't work on Big-endian IBM mainframes with retarded byte parsing lol)