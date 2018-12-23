#pragma once

//#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>


class cPNG {
public:
	cPNG() : isLoaded(false) {
	}

	bool isLoaded;
	int x, y;

	int width, height;
	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	void read_png_file(char* file_name);
	void write_png_file(char* file_name);
	void process_file(void);
	
	
};
