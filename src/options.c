#include "options.h"

#include <stdio.h>
#include <stdlib.h>

void print_usage(const char *exe) {
  fprintf(stderr, "%s [INPUT WORLD] [OUTPUT IMAGE]\n", exe);
}

Options *options_parse(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "invalid number of arguments\n");
    print_usage(argv[0]);
    return NULL;
  }

  Options *options = malloc(sizeof(*options));
  if (options == NULL) {
    return NULL;
  }

  options->world_path = argv[1];
  options->output_path = argv[2];

  return options;
}

void options_free(Options *options) { free(options); }
