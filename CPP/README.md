This is the C++ version of libbmp.   
Instead of just using C code inside a wrapper, it is rewritten in native C++.

# Example: Checkerboard-pattern

```C++
#include <iostream>
#include "lib/libbmp.h"

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
