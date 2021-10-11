#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "color.h"
#include "vec3.h"

int main(int argc, char **argv) {
  const int image_width = 256;
  const int image_height = 256;
  const int num_channels = 3;

  uint8_t image[image_width * image_height * num_channels];

  for (int i = image_height - 1; i >= 0; i -= 1) {
    fprintf(stderr, "\rScanlines remaining: %d ", i);
    for (int j = 0; j < image_width; j += 1) {
      vec3 color = vec3_new((double)j / (double)(image_width - 1),
                            (double)i / (double)(image_height - 1), 0.25);

      uint8_t r, g, b;
      vec3_to_color(color, &r, &g, &b);

      int offset = (image_height * (image_height - i - 1) + j) * num_channels;
      image[offset] = r;
      image[offset + 1] = g;
      image[offset + 2] = b;
    }
  }

  stbi_write_png("out.png", image_width, image_height, num_channels, image,
                 image_width * num_channels);
  fprintf(stderr, "\nDone!\n");

  return 0;
}
