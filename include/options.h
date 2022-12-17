#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct Options {
  char *world_path;
  char *output_path;
} Options;

Options *options_parse(int argc, char *argv[]);
void options_free(Options *options);

#endif
