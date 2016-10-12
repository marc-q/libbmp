/* Copyright 2016 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include <stdlib.h>
#include "libbmp.h"

/* BMP_HEADER */

void bmp_header_init_df (bmp_header *header, const int width, const int height)
{
	/* Init a bmp_header with the default values. */
	header->bfSize = (sizeof (bmp_pixel) * width * height) + (sizeof (unsigned char) * BMP_GET_PADDING (width) * height);
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

int bmp_header_write (const bmp_header *header, FILE *img_file)
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

int bmp_header_read (bmp_header *header, FILE *img_file)
{
	unsigned short magic;
	
	if (img_file == NULL)
	{
		/* ERROR: No file opened! */
		return -1;
	}
	
	/* Check if its an bmp file by comparing the magic nbr: */
	fread (&magic, sizeof (magic), 1, img_file);
	
	if (magic != BMP_MAGIC)
	{
		/* ERROR: Not an BMP file! */
		return -2;
	}
	
	fread (header, sizeof (bmp_header), 1, img_file);
	return 0;
}

/* BMP_PIXEL */

void bmp_pixel_init (bmp_pixel *pxl, const unsigned char red, const unsigned char green, const unsigned char blue)
{
	pxl->red = red;
	pxl->green = green;
	pxl->blue = blue;
}

/* BMP_IMG */

void bmp_img_alloc (bmp_img *img)
{
	int y;
	
	/* Allocate the required memory for the pixels: */
	img->img_pixels = (bmp_pixel**) malloc (sizeof (bmp_pixel*) * img->img_header.biHeight);
	
	for (y = 0; y < img->img_header.biHeight; y++)
	{
		img->img_pixels[y] = (bmp_pixel*) malloc (sizeof (bmp_pixel) * img->img_header.biWidth);
	}
}

void bmp_img_init_df (bmp_img *img, const int width, const int height)
{
	/* INIT the header with default values: */
	bmp_header_init_df (&img->img_header, width, height);
	/* INIT the img: */
	bmp_img_alloc (img);
}

void bmp_img_free (bmp_img *img)
{
	int y;
	
	for (y = 0; y < img->img_header.biHeight; y++)
	{
		free (img->img_pixels[y]);
	}
	free (img->img_pixels);
}

int bmp_img_write (const bmp_img *img, const char *filename)
{
	unsigned int x, y;
	unsigned char padding;
	FILE *img_file;
	
	img_file = fopen (filename, "wb");
	
	if (img_file == NULL)
	{
		/* ERROR: File could'nt be opened! */
		return -1;
	}
	else if (bmp_header_write (&img->img_header, img_file) != 0)
	{
		/* ERROR: Could'nt write the header! */
		fclose (img_file);
		return -2;
	}
	
	/* Write the content: */
	padding = '\0';
	
	for (y = 0; y < img->img_header.biHeight; y++)
	{
		/* Write a whole row of pixels to the file: */
		fwrite (img->img_pixels[y], sizeof (bmp_pixel) * img->img_header.biWidth, 1, img_file);
		
		/* Write the padding for the row! */
		for (x = 0; x < BMP_GET_PADDING (img->img_header.biWidth); x++)
		{
			fwrite (&padding, sizeof (padding), 1, img_file);
		}
	}
	
	/* NOTE: All good! */
	fclose (img_file);
	return 0;
}

int bmp_img_read (bmp_img *img, const char *filename)
{
	int y;
	FILE *img_file;
	
	img_file = fopen (filename, "rb");
	
	if (img_file == NULL)
	{
		/* ERROR: File could'nt be opened! */
		return -1;
	}
	else if (bmp_header_read (&img->img_header, img_file) != 0)
	{
		/* ERROR: Could'nt read the image header! */
		fclose (img_file);
		return -2;
	}
	
	bmp_img_alloc (img);
	
	// TODO: Implement a way to read backwards to be compatible with negative values for biHeight!
	for (y = 0; y < img->img_header.biWidth; y++)
	{
		/* Read a whole row of pixels from the file: */
		fread (img->img_pixels[y], sizeof (bmp_pixel) * img->img_header.biWidth, 1, img_file);
		
		/* Skip the padding: */
		fseek (img_file, sizeof (unsigned char) * BMP_GET_PADDING (img->img_header.biWidth), SEEK_CUR);
	}
	
	fclose (img_file);
	return 0;
}
