#ifndef IMAGE_H
#define IMAGE_H

#include <png.h>
#include <stdint.h>

typedef struct Image {
  int width;
  int height;
  png_bytepp pixels;
} Image;

Image *image_new(int width, int height);
void image_set_pixel(Image *image, int x, int y, uint8_t r, uint8_t g,
                     uint8_t b);
void image_write_png(Image *image, const char *path,
                     png_write_status_ptr write_callback);
void image_free(Image *image);

#endif
