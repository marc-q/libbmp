# libbmp
A simple Bitmap (BMP) library written in C without dependencies.  
For a native C++ version look inside the `CPP` folder.

### Example: Checkerboard-pattern

```C
#include <stdio.h>
#include "libbmp.h"

int
main ()
{
	bmp_img img;
	bmp_img_init_df (&img, 512, 512);
	
	// Draw a checkerboard pattern:
	for (size_t y = 0, x; y < 512; y++)
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
	return 0;
}
```

### Important Notes

* Since the C and C++ version have many things in common, both of them are maintained in a single repository.  
To report language specific things, make use of the `C` or `C++` label.
