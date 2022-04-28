#include <iostream>
#include "libbmp.h"

#define ImageWidth		1055
#define ImageHeight		1011
#define CheckWidth		16
#define CheckerHeight	16

int
write_test()
{
	BmpImg img (ImageWidth, ImageHeight);
	RGB_t light = RGB_t(255,128,255);
	RGB_t dark = RGB_t(0,0,0);

	for (int y = ImageHeight-1; y >= 0; y--)
	{
		for (int x = ImageWidth-1; x >= 0; x--)
		{
			if ((y % (CheckerHeight*2) < CheckerHeight && x % (CheckWidth*2) < CheckWidth) ||
			    (y % (CheckerHeight*2) >= CheckerHeight && x % (CheckWidth*2) >= CheckWidth))
			{
				img.set_pixel (x, y, light);
			}
			else{
				img.set_pixel (x, y, dark);
			}
		}
	}
	// 
	img.write ("checker24bpp.bmp");
	// 
	img.set_pallete(0, dark);
	img.set_pallete(1, light);
	img.bilevel(true);
	img.write ("checker1bpp.bmp");
	
	return 0;
}

int
read_test()
{
	BmpImg img;

	img.read ("checker1bpp.bmp");
	img.bilevel(false);
	img.write ("checker1bpp_copy_as_24bpp.bmp");
	return 0;
}


int
main ()
{
	write_test();
	read_test();
	return 0;
}

