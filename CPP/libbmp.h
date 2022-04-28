#ifndef __LIBBMP_H__
#define __LIBBMP_H__

#include <vector>

#define BMP_MAGIC 19778
//#define BMP_MAGIC 'BM'

#define BMP_GET_PADDING(a) ((a) % 4)

//
// BmpError
//

enum BmpError
{
	BMP_FILE_NOT_OPENED = -4,
	BMP_HEADER_NOT_INITIALIZED,
	BMP_INVALID_FILE,
	BMP_ERROR,
	BMP_OK = 0
};

//
// RGB
//

class RGB_t
{
	public:
		RGB_t (void){}
		RGB_t (uint8_t red, uint8_t green, uint8_t blue)
		: r(red), g(green), b(blue){}

		uint8_t r;
		uint8_t g;
		uint8_t b;
};

//
// BmpPixbuf
//

class BmpPixbuf
{
	public:
		BmpPixbuf (void);
		BmpPixbuf (const int width,
		           const int height);
		~BmpPixbuf ();
		
		void init (const int width,
		           const int height);
		size_t get_rowwidth(void);
		void set_bitpersample(size_t);
		size_t get_bitpersample(void);
		void set_sampleperpixel(size_t);
		size_t get_sampleperpixel(void);
		uint8_t *get_pallete_address(void);
		void set_pallete(const int no,
						const RGB_t rgb);
		RGB_t get_pallete(const int no);
		void set_pixel (const int x,
		                const int y,
		                const RGB_t rgb);
		RGB_t get_pixel(const int x,
		                const int y);
		size_t write (const int row,
		            std::ofstream& f);
		size_t read (const int row,
		           std::ifstream& f);
	private:
		size_t len_width = 0;
		size_t len_height = 0; 
		size_t len_row = 0;
		size_t len_row_1bpp = 0;
		size_t len_row_24bpp = 0;
		size_t len_bitpersample = 8;
		size_t len_sampleperpixel = 3;
		uint8_t pallete[2][4] = {{0,0,0,0},{255,255,255,0}};
		std::vector<unsigned char> data;
};

//
// BmpImg
//

class BmpImg : public BmpPixbuf
{
	public:
		BmpImg (void);
		BmpImg (const int width,
		        const int height);
		~BmpImg ();
		
		enum BmpError write (const std::string& filename);
		enum BmpError read (const std::string& filename);
		
		int get_width (void);
		int get_height (void);

		void bilevel(bool);

	private:
		// Use a struct to read this in one call
		const uint16_t magic = BMP_MAGIC;
		struct
		{
		unsigned int bfSize = 0;
		uint8_t bfReserved[4] = {0};
		unsigned int bfOffBits = 54;
		unsigned int biSize = 40;
		int biWidth = 0;
		int biHeight = 0;
		unsigned short biPlanes = 1;
		unsigned short biBitCount = 24;
		unsigned int biCompression = 0;
		unsigned int biSizeImage = 0;
		int biXPelsPerMeter = 0;
		int biYPelsPerMeter = 0;
		unsigned int biClrUsed = 0;
		unsigned int biClrImportant = 0;
		} header;
};

#endif /* __LIBBMP_H__ */
