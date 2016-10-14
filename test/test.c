/* Copyright 2016 Marc Volker Dickmann */
/* Project: LibBMP */
#include <stdio.h>
#include "../libbmp.h"

#define BMP_TEST_PASSED 1
#define BMP_TEST_FAILED 0

/* Utils */

static void bmp_test_print_summary (const int points, const int points_max)
{
	printf ("\n\nPoints\t%i/%i\n", points, points_max);
	printf ("Failed\t%i\n", points_max - points);
}

static void bmp_test_print_passed (const char *name)
{
	printf ("%s\t\tPASSED!\n", name);
}

static void bmp_test_print_failed (const char *name)
{
	printf ("%s\t\tFAILED!\n", name);
}

/* MACROS */

static int bmp_test_get_padding (void)
{
	if (BMP_GET_PADDING (1) == 3 &&
	    BMP_GET_PADDING (2) == 2 &&
	    BMP_GET_PADDING (3) == 1 &&
	    BMP_GET_PADDING (4) == 0 &&
	    BMP_GET_PADDING (5) == 3 &&
	    BMP_GET_PADDING (6) == 2 &&
	    BMP_GET_PADDING (7) == 1 &&
	    BMP_GET_PADDING (8) == 0)
	{
		bmp_test_print_passed ("BMP_GET_PADDING");
		return BMP_TEST_PASSED;
	}
	
	bmp_test_print_failed ("BMP_GET_PADDING");
	return BMP_TEST_FAILED;
}

int main (int argc, char *argv[])
{
	int points;
	
	points = 0;
	
	printf ("LibBMP-Test v. 0.0.1 A (C) 2016 Marc Volker Dickmann\n\n");
	
	points += bmp_test_get_padding ();
	
	bmp_test_print_summary (points, 1);
	
	return 0;
}
