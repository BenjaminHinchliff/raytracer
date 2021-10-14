#ifndef PNG_WRITE_H
#define PNG_WRITE_H

#include <png.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool write_png_file(const char *filename, int width, int height,
                    int num_channels, png_bytepp rows);

#ifdef __cplusplus
}
#endif

#endif
