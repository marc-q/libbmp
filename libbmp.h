#ifndef __LIBBMP_H__
#define __LIBBMP_H__

#define BMP_MAGIC 19778

#define BMP_GET_PADDING(a) ((a) % 4)

enum
{
	BMP_FILE_NOT_OPENED = -3,
	BMP_HEADER_NOT_INITIALIZED,
	BMP_INVALID_FILE,
	BMP_OK = 0
};

struct _bmp_header
{
	unsigned int bfSize;
	unsigned int bfReserved;
	unsigned int bfOffBits;
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
};

typedef struct _bmp_header bmp_header;

struct _bmp_pixel
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

typedef struct _bmp_pixel bmp_pixel;

struct _bmp_img
{
	bmp_header img_header;
	bmp_pixel **img_pixels;
};

typedef struct _bmp_img bmp_img;

/* BMP_HEADER */
void bmp_header_init_df (bmp_header*, const int, const int);
int bmp_header_write (const bmp_header*, FILE*);
int bmp_header_read (bmp_header*, FILE*);

/* BMP_PIXEL */
void bmp_pixel_init (bmp_pixel*, const unsigned char, const unsigned char, const unsigned char);

/* BMP_IMG */
void bmp_img_alloc (bmp_img*);
void bmp_img_init_df (bmp_img*, const int, const int);
void bmp_img_free (bmp_img*);
int bmp_img_write (const bmp_img*, const char*);
int bmp_img_read (bmp_img*, const char*);

#endif /* __LIBBMP_H__ */
