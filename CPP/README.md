This is the C++ version of libbmp.   
Instead of just using C code inside a wrapper, it is rewritten in native C++.

### Example: Checkerboard-pattern

```C++
#include <iostream>
#include "libbmp.h"

int
main ()
{
	BmpImg img (512, 512);
	
	// Draw a checkerboard pattern:
	// Use negative loops to improve performance
	for (int y = 511, x; y >= 0; y--)
	{
		for (x = 511; x >= 0; x--)
		{
			if ((y % 128 < 64 && x % 128 < 64) ||
			    (y % 128 >= 64 && x % 128 >= 64))
			{
				img.set_pixel (x, y, 255, 255, 255);
			}
		}
	}
	
	img.write ("test.bmp");
	return 0;
}
```

### Example: Counting black and white pixels

```C++
#include <iostream>
#include "libbmp.h"

int
main ()
{
	BmpImg img;
	
	img.read ("test.bmp");
	
	// n - 1 because we use negative loops.
	const int width = img.get_width () - 1;
	const int height = img.get_height () - 1;
	int amnt_b = 0, amnt_w = 0;
	
	// Count the amount of black and white pixels:
	// Use negative loops to improve performance
	for (int y = height, x; y >= 0; y--)
	{
		for (x = width; x >= 0; x--)
		{
			if (img.red_at (x, y) < 128 &&
			    img.green_at (x, y) < 128 &&
			    img.blue_at (x, y) < 128)
			{
				amnt_b++;
			}
			else
			{
				amnt_w++;
			}
		}
	}
	
	std::cout << "Amount of black pixels:\t" << amnt_b << "\n";
	std::cout << "Amount of white pixels:\t" << amnt_w << "\n";
	return 0;
}
```
