/* Copyright 2016 - 2017 Marc Volker Dickmann
 * Project: LibBMP
 */
#include <fstream>
#include <cmath>
#include "libbmp.h"

//
// BmpPixbuf
//

BmpPixbuf::BmpPixbuf (void)
{
}

BmpPixbuf::BmpPixbuf (const int width,
                      const int height)
{
	init (width, height);
}

BmpPixbuf::~BmpPixbuf (void)
{
	data.clear ();
}

void
BmpPixbuf::init (const int width,
                 const int height)
{
	len_row = width * len_pixel;
	
	data.resize (height * len_row);
}

void
BmpPixbuf::set_pixel (const int x,
                      const int y,
                      const unsigned char r,
                      const unsigned char g,
                      const unsigned char b)
{
	const size_t index = (x * len_pixel) + (y * len_row);
	data[index] = b;
	data[index + 1] = g;
	data[index + 2] = r;
}

unsigned char
BmpPixbuf::red_at (const int x,
                   const int y)
{
	return data[(x * len_pixel) + (y * len_row) + 2];
}

unsigned char
BmpPixbuf::green_at (const int x,
                     const int y)
{
	return data[(x * len_pixel) + (y * len_row) + 1];
}

unsigned char
BmpPixbuf::blue_at (const int x,
                    const int y)
{
	return data[(x * len_pixel) + (y * len_row)];
}

void
BmpPixbuf::write (const int row,
                  std::ofstream& f)
{
	f.write (reinterpret_cast<char*> (&data[row * len_row]), len_row);
}

void
BmpPixbuf::read (const int row,
                 std::ifstream& f)
{
	f.read (reinterpret_cast<char*> (&data[row * len_row]), len_row);
}

//
// BmpImg
//

BmpImg::BmpImg (void)
{
}

BmpImg::BmpImg (const int width,
                const int height) : BmpPixbuf (width, std::abs (height))
{
	// INIT the header with default values
	header.bfSize = (3 * width + BMP_GET_PADDING (width)) 
	                * std::abs (height);
	header.biWidth = width;
	header.biHeight = height;
}

BmpImg::~BmpImg (void)
{
}

int
BmpImg::get_width (void)
{
	return header.biWidth;
}

int
BmpImg::get_height (void)
{
	return header.biHeight;
}

enum BmpError
BmpImg::write (const std::string& filename)
{
	// Open the image file in binary mode
	std::ofstream f_img (filename.c_str (), std::ios::binary);
	
	if (!f_img.is_open ())
		return BMP_FILE_NOT_OPENED;
	
	// Since an adress must be passed to fwrite, create a variable!
	const unsigned short magic = BMP_MAGIC;
	
	f_img.write (reinterpret_cast<const char*>(&magic), sizeof (magic));
	f_img.write (reinterpret_cast<const char*>(&header), sizeof (header));
	
	// Select the mode (bottom-up or top-down)
	const int h = std::abs (header.biHeight);
	const int offset = (header.biHeight > 0 ? 0 : h - 1);
	const int padding = BMP_GET_PADDING (header.biWidth);
	
	for (int y = h - 1; y >= 0; y--)
	{
		// Write a whole row of pixels into the file
		BmpPixbuf::write ((int)std::abs (y - offset), f_img);
		
		// Write the padding
		f_img.write ("\0\0\0", padding);
	}
	
	// NOTE: All good
	f_img.close ();
	return BMP_OK;
}

enum BmpError
BmpImg::read (const std::string& filename)
{	
	// Open the image file in binary mode
	std::ifstream f_img (filename.c_str (), std::ios::binary);
	
	if (!f_img.is_open ())
		return BMP_FILE_NOT_OPENED;
	
	// Since an adress must be passed to fread, create a variable!
	unsigned short magic;
	
	// Check if its an bmp file by comparing the magic nbr
	f_img.read(reinterpret_cast<char*>(&magic), sizeof (magic));
	
	if (magic != BMP_MAGIC)
	{
		f_img.close ();
		return BMP_INVALID_FILE;
	}
	
	// Read the header structure into header
	f_img.read (reinterpret_cast<char*>(&header), sizeof (header));
	
	// Select the mode (bottom-up or top-down)
	const int h = std::abs (header.biHeight);
	const int offset = (header.biHeight > 0 ? 0 : h - 1);
	const int padding = BMP_GET_PADDING (header.biWidth);
	
	// Allocate the pixel buffer
	BmpPixbuf::init (header.biWidth, h);
	
	for (int y = h - 1; y >= 0; y--)
	{
		// Read a whole row of pixels from the file
		BmpPixbuf::read ((int)std::abs (y - offset), f_img);
		
		// Skip the padding
		f_img.seekg (padding, std::ios::cur);
	}
	
	// NOTE: All good
	f_img.close ();
	return BMP_OK;
}
