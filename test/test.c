/* Copyright 2016 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include <stdlib.h>
#include "../libbmp.h"

static void bmp_test_write (int width, int height)
{
	int x, y;
	bmp_img img;
	
	bmp_img_init_df (&img, width, -height);

	/* Draw a checkerboard pattern: */
	for (y = 0; y < height; y++)
	{	
		for (x = 0; x < width; x++)
		{
			if ((y % 128 < 64 && x % 128 < 64) ||
			    (y % 128 >= 64 && x % 128 >= 64))
			{
				bmp_pixel_init (&img.img_pixels[y][x], 250, 250, 250);
			}
			else
			{
				bmp_pixel_init (&img.img_pixels[y][x], 0, 0, 0);
			}
		}
	}
	
	bmp_img_write (&img, "test.bmp");
	bmp_img_free (&img);
}

static void bmp_test_read (void)
{
	int x, y;
	bmp_img img;
	
	bmp_img_read (&img, "test.bmp");
	img.img_header.biHeight = abs (img.img_header.biHeight);
	
	for (y = 0; y < img.img_header.biHeight; y++)
	{
		for (x = 0; x < img.img_header.biWidth; x++)
		{
			if (img.img_pixels[y][x].red == 250 &&
			    img.img_pixels[y][x].green == 250 &&
			    img.img_pixels[y][x].blue == 250)
			{
				bmp_pixel_init (&img.img_pixels[y][x], 250, 120, 120);
			}
		}
	}
	
	bmp_img_write (&img, "test2.bmp");
	bmp_img_free (&img);
}

int main (int argc, char *argv[])
{
	int i;
	
	printf ("LibBMP-Test v. 0.0.1 A (C) 2016 Marc Volker Dickmann\n\n");
	
	for (i = 0; i < 4; i++)
	{
		bmp_test_write (512 + i, 512);
		bmp_test_read ();
	}
	
	return 0;
}
