/* Copyright 2016 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include "../libbmp.h"

static void bmp_test_write_bw (void)
{
	int x, y;
	bmp_img img;
	
	bmp_img_init_df (&img, 512, 512);

	/* Draw a checkerboard pattern: */
	for (y = 0; y < 512; y++)
	{	
		for (x = 0; x < 512; x++)
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

int main (int argc, char *argv[])
{
	printf ("LibBMP-Test v. 0.0.1 A (C) 2016 Marc Volker Dickmann\n\n");
	
	bmp_test_write_bw ();
	
	return 0;
}
