#include <stdio.h>
#include "../libbmp.h"

int
main (int argc, char *argv[])
{
	bmp_img img;
	bmp_img_init_df (&img, 512, 512);

	// Draw a checkerboard pattern:
	for (size_t y = 0; y < 512; y++)
	{
		for (size_t x = 0; x < 512; x++)
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
	return 0;
}
