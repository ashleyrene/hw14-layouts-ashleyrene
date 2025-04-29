#include "hw14.h"

#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  if (argc < 5) {
    fprintf(stderr, "Usage: %s [input] [output] [width] [height]\n", argv[0]);
    return EXIT_FAILURE;
  }

  float width = atof(argv[3]);
  float height = atof(argv[4]);

  if (width == 0) {
    fprintf(stderr, "Invalid width: %s\n", argv[3]);
    return EXIT_FAILURE;
  }

  if (height == 0) {
    fprintf(stderr, "Invalid height: %s\n", argv[4]);
    return EXIT_FAILURE;
  }

  FILE *source = fopen(argv[1], "r");
  if (!source) {
    fprintf(stderr, "Failed to open input file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  char buf[2048] = {0};
  fread(buf, sizeof(char), 2048, source);
  fclose(source);

  struct stream s = {.text = buf, .pos = 0, .length = strlen(buf)};
  struct DOMNode *root = load_tree(&s);

  FILE *target = fopen(argv[2], "w");
  if (!target) {
    fprintf(stderr, "Failed to open output file %s\n", argv[2]);
    return EXIT_FAILURE;
  }

  layout(root, width, height, target);
  fclose(target);

  return 0;
}
