#ifndef __LIBBMP_H__
#define __LIBBMP_H__

#include <vector>

#define BMP_MAGIC 19778

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
		
		void set_pixel (const int x,
		                const int y,
		                const unsigned char r,
		                const unsigned char g,
		                const unsigned char b);
		
		unsigned char red_at (const int x,
		                      const int y);
		unsigned char green_at (const int x,
		                        const int y);
		unsigned char blue_at (const int x,
		                       const int y);
		
		
		void write (const int row,
		            std::ofstream& f);
		
		void read (const int row,
		           std::ifstream& f);
	private:
		size_t len_row;
		size_t len_pixel = 3;
		
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
	private:
		// Use a struct to read this in one call
		struct
		{
		unsigned int bfSize = 0;
		unsigned int bfReserved = 0;
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
