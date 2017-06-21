/* Copyright 2016 - 2017 Marc Volker Dickmann
 * Project: LibBMP
 */
#include <stdio.h>
#include "../libbmp.h"

#define BMP_TEST_PASSED 1
#define BMP_TEST_FAILED 0

// Utils

static void
bmp_test_print_summary (const int points,
                        const int points_max)
{
	printf ("\n\nPoints\t%i/%i\n", points, points_max);
	printf ("Failed\t%i\n", points_max - points);
}

static void
bmp_test_print_passed (const char *name)
{
	printf ("%s\t\tPASSED!\n", name);
}

static void
bmp_test_print_failed (const char *name)
{
	printf ("%s\t\tFAILED!\n", name);
}

// MACROS

static int
bmp_test_get_padding (void)
{
	if (BMP_GET_PADDING (1) == 1 &&
	    BMP_GET_PADDING (2) == 2 &&
	    BMP_GET_PADDING (3) == 3 &&
	    BMP_GET_PADDING (4) == 0 &&
	    BMP_GET_PADDING (5) == 1 &&
	    BMP_GET_PADDING (6) == 2 &&
	    BMP_GET_PADDING (7) == 3 &&
	    BMP_GET_PADDING (8) == 0)
	{
		bmp_test_print_passed ("BMP_GET_PADDING");
		return BMP_TEST_PASSED;
	}
	
	bmp_test_print_failed ("BMP_GET_PADDING");
	return BMP_TEST_FAILED;
}

// Header

static int
bmp_test_header_size (void)
{
	// Note: Its 52 Bytes because the header doesn't include the magic field!
	if (sizeof (bmp_header) == 52)
	{
		bmp_test_print_passed ("header_size");
		return BMP_TEST_PASSED;
	}
	
	bmp_test_print_failed ("header_size");
	return BMP_TEST_FAILED;
}

static int
bmp_test_header_init_df (void)
{
	int passed = BMP_TEST_PASSED;
	bmp_header header;
	
	// Test positive height value:
	bmp_header_init_df (&header, 100, 100);
	
	if (header.bfSize != (sizeof (bmp_pixel) * 10000) ||
	    header.biWidth != 100 ||
	    header.biHeight != 100)
	{
		passed = BMP_TEST_FAILED;
	}
	
	// Test negative height value with padding:
	bmp_header_init_df (&header, 102, -100);
	
	if (header.bfSize != (sizeof (bmp_pixel) * 10200) + 200 ||
	     header.biWidth != 102 ||
	     header.biHeight != -100)
	{
		passed = BMP_TEST_FAILED;
	}
	
	// Return the result:
	if (passed == BMP_TEST_PASSED)
	{
		bmp_test_print_passed ("header_init_df");
		return BMP_TEST_PASSED;
	}

	bmp_test_print_failed ("header_init_df");
	return BMP_TEST_FAILED;
}

// Pixel

static int
bmp_test_pixel_init (void)
{
	bmp_pixel pxl;
	
	bmp_pixel_init (&pxl, 1, 250, 4);
	
	if (pxl.red == 1 &&
	    pxl.green == 250 &&
	    pxl.blue == 4)
	{
		bmp_test_print_passed ("pixel_init");
		return BMP_TEST_PASSED;
	}
	
	bmp_test_print_failed ("pixel_init");
	return BMP_TEST_FAILED;
}

int
main (int argc, char *argv[])
{
	int points = 0;
	
	printf ("LibBMP-Test v. 0.0.1 A (C) 2016 - 2017 Marc Volker Dickmann\n\n");
	
	points += bmp_test_get_padding ();
	
	points += bmp_test_header_size ();
	points += bmp_test_header_init_df ();
	
	points += bmp_test_pixel_init ();
	
	bmp_test_print_summary (points, 4);
	return 0;
}
