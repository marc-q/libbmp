/* Copyright 2016 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include <stdlib.h>
#include "libbmp.h"

/* BMP_HEADER */

void bmp_header_init_df (bmp_header *header, int width, int height)
{
	/* Init a bmp_header with the default values. */
	header->bfSize = (sizeof (bmp_pixel) * (width * height)) + (BMP_GET_PADDING (width) * height);
	header->bfReserved = 0;
	header->bfOffBits = 54;
	header->biSize = 40;
	header->biWidth = width;
	header->biHeight = height;
	header->biPlanes = 1;
	header->biBitCount = 24;
	header->biCompression = 0;
	header->biSizeImage = 0;
	header->biXPelsPerMeter = 0;
	header->biYPelsPerMeter = 0;
	header->biClrUsed = 0;
	header->biClrImportant = 0;
}

int bmp_header_write (bmp_header *header, FILE *img_file)
{
	unsigned short magic;
	
	/* Check for errors: */
	if (header == NULL)
	{
		/* ERROR: bmp_header unitizalized! */
		return -1; 
	}
	else if (img_file == NULL)
	{
		/* ERROR: No file opened! */
		return -2;
	}
	
	magic = BMP_MAGIC;
	
	fwrite (&magic, sizeof (magic), 1, img_file);
	/* Use the type instead of the variable because its a pointer! */
	fwrite (header, sizeof (bmp_header), 1, img_file);
	
	/* NOTE: All good! */
	return 0;
}

/* BMP_PIXEL */

void bmp_pixel_init (bmp_pixel *pxl, unsigned char red, unsigned char green, unsigned char blue)
{
	pxl->red = red;
	pxl->green = green;
	pxl->blue = blue;
}

/* BMP_MISC */

int bmp_img_write (char *filename, bmp_pixel **pxls, int width, int height)
{
	unsigned int x, y;
	unsigned char padding;
	FILE *img_file;
	bmp_header img_header;
	
	img_file = fopen (filename, "wb");
	
	if (img_file == NULL)
	{
		/* ERROR: File could'nt be opened! */
		return -1;
	}
	
	/* Write the header: */
	bmp_header_init_df (&img_header, width, height);
	
	if (bmp_header_write (&img_header, img_file) != 0)
	{
		/* ERROR: Could'nt write the header! */
		fclose (img_file);
		return -2;
	}
	
	/* Write the content: */
	padding = '\0';
	
	for (y = 0; y < height; y++)
	{
		/* Write a whole row of pixels to the file: */
		fwrite (pxls[y], sizeof (bmp_pixel) * width, 1, img_file);
		
		/* Write the padding for the row! */
		for (x = 0; x < BMP_GET_PADDING (width); x++)
		{
			fwrite (&padding, sizeof (padding), 1, img_file);
		}
	}
	
	/* NOTE: All good! */
	fclose (img_file);
	return 0;
}
