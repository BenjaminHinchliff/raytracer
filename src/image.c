#include "image.h"
#include "png_write.h"

#include <stdlib.h>

#define NUM_CHANNELS 3

Image *image_new(int width, int height) {
  Image *image = malloc(sizeof(Image));
  image->width = width;
  image->height = height;
  image->pixels = malloc(sizeof(png_bytep) * height);
  for (int i = 0; i < height; i++) {
    // NOTE: hardcoding rgb for now
    image->pixels[i] = malloc(sizeof(png_byte) * NUM_CHANNELS * width);
  }
  return image;
}

void image_set_pixel(Image *image, int x, int y, uint8_t r, uint8_t g,
                     uint8_t b) {
  x *= NUM_CHANNELS;
  image->pixels[y][x + 0] = r;
  image->pixels[y][x + 1] = g;
  image->pixels[y][x + 2] = b;
}

void image_write_png(Image *image, const char *path,
                     png_write_status_ptr write_callback) {
  write_png_file(path, image->width, image->height, image->pixels,
                 write_callback);
}

void image_free(Image *image) {
  if (image == NULL) {
    return;
  }

  for (int i = 0; i < image->height; i++) {
    free(image->pixels[i]);
  }
  free(image->pixels);
  free(image);
}
