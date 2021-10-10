#include <stdio.h>

int main(int argc, char **argv) {
  const int image_width = 256;
  const int image_height = 256;

  printf("P3\n");
  printf("%d %d\n", image_width, image_height);
  printf("255\n");

  for (int i = image_height - 1; i >= 0; i -= 1) {
    for (int j = 0; j < image_width; j += 1) {
      double r = (double)j / (double)(image_width - 1);
      double g = (double)i / (double)(image_height - 1);
      double b = 0.25;

      int ir = (int)(r * 255.999);
      int ig = (int)(g * 255.999);
      int ib = (int)(b * 255.999);

      printf("%d %d %d\n", ir, ig, ib);
    }
  }

  return 0;
}
