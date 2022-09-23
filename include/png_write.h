#ifndef PNG_WRITE_H
#define PNG_WRITE_H

#include <png.h>
#include <stdbool.h>

bool write_png_file(const char *filename, int width, int height,
                    png_bytepp rows, png_write_status_ptr callback);

#endif
