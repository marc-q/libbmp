#ifndef __LIBBMP_H__
#define __LIBBMP_H__

#define BMP_MAGIC 19778

#define BMP_GET_PADDING(a) ((a) % 4)

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

/* BMP_HEADER */
void bmp_header_init_df (bmp_header*, int, int);
int bmp_header_write (bmp_header*, FILE*);

/* BMP_PIXEL */
void bmp_pixel_init (bmp_pixel*, unsigned char, unsigned char, unsigned char);

/* BMP_MISC */
int bmp_img_write (char*, bmp_pixel**, int, int);

#endif /* __LIBBMP_H__ */
