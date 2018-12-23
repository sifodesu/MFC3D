#include "cPNG.h"


void cPNG::read_png_file(char* file_name)
{
	char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	
	fread(header, 1, 8, fp);
	


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);
	

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	
	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (y = 0; y < height; y++)
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));

	png_read_image(png_ptr, row_pointers);

	fclose(fp);
	isLoaded = true;
}


void cPNG::write_png_file(char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	

	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);
	
	png_init_io(png_ptr, fp);


	/* write header */
	
	png_set_IHDR(png_ptr, info_ptr, width, height,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	

	png_write_image(png_ptr, row_pointers);


	/* end write */
	

	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	for (y = 0; y < height; y++)
		free(row_pointers[y]);
	free(row_pointers);

	fclose(fp);
}


void cPNG::process_file(void)
{
	
	for (y = 0; y < height; y++) {
		png_byte* row = row_pointers[y];
		for (x = 0; x < width; x++) {
			png_byte* ptr = &(row[x * 4]);
			printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
				x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

			/* set red value to 0 and green value to the blue one */
			ptr[0] = 0;
			ptr[1] = ptr[2];
		}

	}
}