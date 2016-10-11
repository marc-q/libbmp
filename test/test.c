/* Copyright 2016 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include <stdlib.h>
#include "../libbmp.h"

static void bmp_test_write_bw (void)
{
	int x, y;
	bmp_pixel **pxls;
	
	pxls = (bmp_pixel**) malloc (sizeof (bmp_pixel*) * 512);

	for (y = 0; y < 512; y++)
	{
		pxls[y] = (bmp_pixel*) malloc (sizeof (bmp_pixel) * 512);
		
		for (x = 0; x < 512; x++)
		{
			/* Draw a checkerboard pattern: */
			if ((y % 128 < 64 && x % 128 < 64) ||
			    (y % 128 >= 64 && x % 128 >= 64))
			{
				bmp_pixel_init (&pxls[y][x], 250, 250, 250);
			}
			else
			{
				bmp_pixel_init (&pxls[y][x], 0, 0, 0);
			}
		}
	}
	
	bmp_img_write ("test.bmp", pxls, 512, 512);

	for (y = 0; y < 512; y++)
	{
		free (pxls[y]);
	}
	free (pxls);
}

int main (int argc, char *argv[])
{
	printf ("LibBMP-Test v. 0.0.1 A (C) 2016 Marc Volker Dickmann\n\n");
	
	bmp_test_write_bw ();
	
	return 0;
}
