/**
 * @file libbmp.cpp
 * @author Marc Volker Dickmann
 * @brief libbmp C++ source file
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2016 - 2017
 * 
 */

#include <stdlib.h>
#include <string.h>
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
	len_width = width;
	len_height = height;
	len_row_24bpp = 3 * len_width + BMP_GET_PADDING (len_width);
	len_row_1bpp = (((len_width + 7)/8 + 3)/4)*4;
	//printf("len24=%d len1=%d\n", len_row_24bpp, len_row_1bpp);
	len_row = len_row_24bpp;
	data.resize (len_row * len_height);
	//data.resize (len_width * len_height);
}
size_t
BmpPixbuf::get_rowwidth(void)
{
	if((len_bitpersample ==1)&&(len_sampleperpixel == 1))
		return len_row_1bpp;
	else
		return len_row_24bpp;
}
void
BmpPixbuf::set_bitpersample(size_t bitpersample)
{
	len_bitpersample = bitpersample;
}
size_t
BmpPixbuf::get_bitpersample(void)
{
	return len_bitpersample;
}
void
BmpPixbuf::set_sampleperpixel(size_t sampleperpixel)
{
	len_sampleperpixel = sampleperpixel;
}
size_t
BmpPixbuf::get_sampleperpixel(void)
{
	return len_sampleperpixel;
}
uint8_t*
BmpPixbuf::get_pallete_address(void)
{
	return &pallete[0][0];
}
void
BmpPixbuf::set_pallete(const int no,
				const RGB_t rgb)
{
	if ((no >= 0)&&(no <2)){
		pallete[no][0] = rgb.b;
		pallete[no][1] = rgb.g;
		pallete[no][2] = rgb.r;
		pallete[no][3] = 0;
	}
}
RGB_t
BmpPixbuf::get_pallete(const int no)
{
	RGB_t rgb = RGB_t(0,0,0);

	if ((no >= 0)&&(no <2)){
		rgb.b = pallete[no][0];
		rgb.g = pallete[no][1];
		rgb.r = pallete[no][2];
	}
	return rgb;
}

void
BmpPixbuf::set_pixel (const int x,
                      const int y,
                      const RGB_t rgb)
{
	const size_t index = (x * 3) + (y * len_row);
	data[index] = rgb.b;
	data[index + 1] = rgb.g;
	data[index + 2] = rgb.r;
}

RGB_t
BmpPixbuf::get_pixel(const int x,
                      const int y)
{
	RGB_t rgb = RGB_t(0,0,0);

	rgb.b = data[(x * 3) + (y * len_row)];
	rgb.g = data[(x * 3) + (y * len_row) + 1];
	rgb.r = data[(x * 3) + (y * len_row) + 2];
	return rgb;
}

size_t
BmpPixbuf::write (const int row,
                  std::ofstream& f)
{
	RGB_t rgb;
	unsigned char a;
	size_t index = 0;
	size_t len_row_local = get_rowwidth();
	char *image_buffer = new char[len_row_local];
	if ((image_buffer != NULL) && (len_row_local > 0)){
		memset(image_buffer, 0x0, len_row_local);
		for(size_t x = 0;x < len_width;x++){
			rgb = get_pixel(x, row);
			if ((len_bitpersample == 1)&&(len_sampleperpixel == 1)) {
				a = (rgb.r + rgb.g + rgb.b)/3;
				if (a > 127)
					image_buffer[index] |= (0x80 >> (x % 8));
				if ((x % 8) == 7)
					index++;
			}
			else {
				image_buffer[index++] = rgb.b;
				image_buffer[index++] = rgb.g;
				image_buffer[index++] = rgb.r;
			}
		}
		f.write (image_buffer, len_row_local);
		delete[] image_buffer;
	}
	return len_row_local;
}

size_t
BmpPixbuf::read (const int row,
                 std::ifstream& f)
{
	RGB_t rgb;
	unsigned char a;
	size_t index = 0;
	size_t len_row_local = get_rowwidth();
	char *image_buffer = new char[len_row_local];
	if ((image_buffer != NULL) && (len_row_local > 0)) {	
		f.read (image_buffer, len_row_local);
		for(size_t x = 0;x < len_width;x++){
			if ((len_bitpersample == 1)&&(len_sampleperpixel == 1)) {
				a = image_buffer[index] & (0x80 >> (x % 8));
				if (a)
					rgb = get_pallete(1);
				else
					rgb = get_pallete(0);
				if (x % 8 == 7)
					index++;				
			}
			else {
				rgb.b = image_buffer[index++];
				rgb.g = image_buffer[index++];
				rgb.r = image_buffer[index++];
			}
			set_pixel(x, row, rgb);
		}
		delete[] image_buffer;
	}
	return len_row_local;
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
	header.biWidth = width;
	header.biHeight = height;
	bilevel(false);
}

BmpImg::~BmpImg (void)
{
}

void
BmpImg::bilevel(bool BiLevel)
{
	if (BiLevel){
		header.biBitCount = 1;
		header.biClrUsed = 2;
		set_bitpersample(1);
		set_sampleperpixel(1);
	}
	else{
		header.biBitCount = 24;
		header.biClrUsed = 0;
		set_bitpersample(8);
		set_sampleperpixel(3);
	}
	header.bfOffBits = sizeof(magic) + sizeof (header) + header.biClrUsed*4;
	header.biSizeImage = BmpPixbuf::get_rowwidth() * std::abs (header.biHeight);
	header.bfSize = header.bfOffBits + header.biSizeImage;
	init(header.biWidth, header.biHeight);
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
	size_t index, padding;

	// Open the image file in binary mode
	std::ofstream f_img (filename.c_str (), std::ios::binary);
	
	if (!f_img.is_open ())
		return BMP_FILE_NOT_OPENED;
	
	// Since an adress must be passed to fwrite, create a variable!	
	f_img.write (reinterpret_cast<const char*>(&magic), sizeof (magic));
	f_img.write (reinterpret_cast<const char*>(&header), sizeof (header));
	if (header.biClrUsed > 0)
		f_img.write (reinterpret_cast<const char*>(get_pallete_address()), header.biClrUsed*4);
	
	// Select the mode (bottom-up or top-down)
	const int h = std::abs (header.biHeight);
	const int offset = (header.biHeight > 0 ? 0 : h - 1);
	//const int padding = BMP_GET_PADDING (header.biWidth);

	for (int y = h - 1; y >= 0; y--)
	{
		// Write a whole row of pixels into the file
		padding = BMP_GET_PADDING (index = BmpPixbuf::write ((int)std::abs (y - offset), f_img) );
		
		// Write the padding
		//if (padding > 0){
		//	f_img.write ("\0\0\0", 4 - padding);
		//	printf("%d idx=%d add 0 =%d\n", y, index, 4-padding);
		//}
	}

	// NOTE: All good
	f_img.close ();
	return BMP_OK;
}

enum BmpError
BmpImg::read (const std::string& filename)
{
	size_t padding;

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
	BmpPixbuf::init (header.biWidth, header.biHeight);
	
	// Select the mode (bottom-up or top-down)
	const int h = std::abs (header.biHeight);
	const int offset = (header.biHeight > 0 ? 0 : h - 1);
	//const int padding = BMP_GET_PADDING (header.biWidth);

	if (header.biBitCount == 1)
		bilevel(true);
	else if (header.biBitCount == 24)
		bilevel(false);
	else
		return BMP_INVALID_FILE;
	if (header.biClrUsed > 0)
		f_img.read (reinterpret_cast<char*>(get_pallete_address()), header.biClrUsed*4);

	// Allocate the pixel buffer
	BmpPixbuf::init (header.biWidth, h);
	
	for (int y = h - 1; y >= 0; y--)
	{
		// Read a whole row of pixels from the file
		padding = BMP_GET_PADDING (BmpPixbuf::read ((int)std::abs (y - offset), f_img) );
		
		// Skip the padding
		f_img.seekg (padding, std::ios::cur);
	}
	
	// NOTE: All good
	f_img.close ();
	return BMP_OK;
}

