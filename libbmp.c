/* Copyright 2016 - 2017 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libbmp.h"

/* BMP_HEADER */

void bmp_header_init_df (bmp_header *header, const int width, const int height)
{
	/* Init a bmp_header with the default values. */
	header->bfSize = (sizeof (bmp_pixel) * width * abs (height)) + (sizeof (unsigned char) * BMP_GET_PADDING (width) * abs (height));
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
		/* ERROR: bmp_header not initialized! */
		return BMP_HEADER_NOT_INITIALIZED; 
	}
	else if (img_file == NULL)
	{
		/* ERROR: No file opened! */
		return BMP_FILE_NOT_OPENED;
	}
	
	magic = BMP_MAGIC;
	
	fwrite (&magic, sizeof (magic), 1, img_file);
	/* Use the type instead of the variable because its a pointer! */
	fwrite (header, sizeof (bmp_header), 1, img_file);
	
	/* NOTE: All good! */
	return BMP_OK;
}

int bmp_header_read (bmp_header *header, FILE *img_file)
{
	unsigned short magic;
	
	if (img_file == NULL)
	{
		/* ERROR: No file opened! */
		return BMP_FILE_NOT_OPENED;
	}
	
	/* Check if its an bmp file by comparing the magic nbr: */
	fread (&magic, sizeof (magic), 1, img_file);
	
	if (magic != BMP_MAGIC)
	{
		/* ERROR: Not an BMP file! */
		return BMP_INVALID_FILE;
	}
	
	fread (header, sizeof (bmp_header), 1, img_file);
	return BMP_OK;
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
	img->img_pixels = (bmp_pixel**) malloc (sizeof (bmp_pixel*) * abs (img->img_header.biHeight));
	
	for (y = 0; y < abs (img->img_header.biHeight); y++)
	{
		img->img_pixels[y] = (bmp_pixel*) malloc (sizeof (bmp_pixel) * img->img_header.biWidth);
	}
}

void bmp_img_init_df (bmp_img *img, const int width, const int height)
{
	/* INIT the header with default values: */
	bmp_header_init_df (&img->img_header, width, height);
	/* ALLOCATE the img: */
	bmp_img_alloc (img);
}

void bmp_img_free (bmp_img *img)
{
	int y;
	
	for (y = 0; y < abs (img->img_header.biHeight); y++)
	{
		free (img->img_pixels[y]);
	}
	free (img->img_pixels);
}

int bmp_img_write (const bmp_img *img, const char *filename)
{
	int y, offset;
	unsigned char padding[3];
	FILE *img_file;
	
	img_file = fopen (filename, "wb");
	
	if (img_file == NULL)
	{
		/* ERROR: File could'nt be opened! */
		return BMP_FILE_NOT_OPENED;
	}
	
	/* NOTE: This way the correct error code could be returned. */
	y = bmp_header_write (&img->img_header, img_file);
	
	if (y != BMP_OK)
	{
		/* ERROR: Could'nt write the header! */
		fclose (img_file);
		return y;
	}
	
	/* Select the mode (bottom-up or top-down): */
	offset = 0;
	
	if (img->img_header.biHeight > 0)
	{
		offset = abs (img->img_header.biHeight) - 1;
	}
	
	/* Write the content: */
	memset (padding, '\0', sizeof (unsigned char) * 3);
	
	for (y = 0; y < abs (img->img_header.biHeight); y++)
	{
		/* Write a whole row of pixels to the file: */
		fwrite (img->img_pixels[abs (offset - y)], sizeof (bmp_pixel), img->img_header.biWidth, img_file);
		
		/* Write the padding for the row! */
		fwrite (padding, sizeof (unsigned char), BMP_GET_PADDING (img->img_header.biWidth), img_file);
	}
	
	/* NOTE: All good! */
	fclose (img_file);
	return BMP_OK;
}

int bmp_img_read (bmp_img *img, const char *filename)
{
	int y;
	long seek_offset;
	FILE *img_file;
	
	img_file = fopen (filename, "rb");
	
	if (img_file == NULL)
	{
		/* ERROR: File could'nt be opened! */
		return BMP_FILE_NOT_OPENED;
	}
	
	/* NOTE: This way the correct error code could be returned. */
	y = bmp_header_read (&img->img_header, img_file);
	
	if (y != BMP_OK)
	{
		/* ERROR: Could'nt read the image header! */
		fclose (img_file);
		return y;
	}
	
	bmp_img_alloc (img);
	
	/* Select the mode (bottom-up or top-down): */
	seek_offset = sizeof (unsigned char) * BMP_GET_PADDING (img->img_header.biWidth);
	
	if (img->img_header.biHeight > 0)
	{
		/* For the first row it seek's one row from the SEEK_END. */
		fseek (img_file, (sizeof (bmp_pixel) * img->img_header.biWidth * -1) - seek_offset, SEEK_END);
		seek_offset = (sizeof (bmp_pixel) * img->img_header.biWidth * -2) - seek_offset;
	}
	
	/* Read the content: */
	for (y = 0; y < abs (img->img_header.biHeight); y++)
	{
		/* Read a whole row of pixels from the file: */
		fread (img->img_pixels[y], sizeof (bmp_pixel), img->img_header.biWidth, img_file);
		
		/* Skip the padding: */
		fseek (img_file, seek_offset, SEEK_CUR);
	}
	
	/* NOTE: All good! */
	fclose (img_file);
	return BMP_OK;
}
