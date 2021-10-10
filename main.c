#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, char **argv) {
  const int image_width = 256;
  const int image_height = 256;
  const int num_channels = 3;

  uint8_t image[image_width * image_height * num_channels];

  for (int i = image_height - 1; i >= 0; i -= 1) {
    for (int j = 0; j < image_width; j += 1) {
      double r = (double)j / (double)(image_width - 1);
      double g = (double)i / (double)(image_height - 1);
      double b = 0.25;

      uint8_t ir = (int)(r * 255.999);
      uint8_t ig = (int)(g * 255.999);
      uint8_t ib = (int)(b * 255.999);

      int offset = (image_height * (image_height - i - 1) + j) * num_channels;
      image[offset] = ir;
      image[offset + 1] = ig;
      image[offset + 2] = ib;
    }
  }

  stbi_write_png("out.png", image_width, image_height, num_channels, image, image_width * num_channels);

  return 0;
}
