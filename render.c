#include <stdio.h>
#include <stdlib.h>

#include "libbmp/libbmp.h"

struct color {
  uint8_t r, g, b;
};

static void draw_hline(bmp_img *img, int x1, int x2, int y, struct color col) {
  if (x2 < x1) {
    int temp = x1;
    x1 = x2;
    x2 = temp;
  }

  for (int x = x1; x <= x2; x++) {
    bmp_pixel_init(&img->img_pixels[y][x], col.r, col.g, col.b);
  }
}

static void draw_vline(bmp_img *img, int x, int y1, int y2, struct color col) {
  if (y2 < y1) {
    int temp = y1;
    y1 = y2;
    y2 = temp;
  }

  for (int y = y1; y <= y2; y++) {
    bmp_pixel_init(&img->img_pixels[y][x], col.r, col.g, col.b);
  }
}

int main(int argc, char **argv) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s [colors] [layout] [target]\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *colormap = fopen(argv[1], "r");
  int num_colors;
  fscanf(colormap, "%d", &num_colors);

  struct color *colors = calloc(num_colors, sizeof(struct color));

  for (int i = 0; i < num_colors; i++) {
    fscanf(colormap, "%d %d %d", &colors[i].r, &colors[i].g, &colors[i].b);
  }
  fclose(colormap);

  FILE *layout = fopen(argv[2], "r");
  if (!layout) {
    fprintf(stderr, "Failed to open layout file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  float width, height;
  fscanf(layout, "%f %f", &width, &height);

  bmp_img img;
  bmp_img_init_df(&img, (int)width + 1, (int)height + 1);
  printf("Initialized image of dimensions %dx%d\n", (int)width, (int)height);
  for (size_t i = 0; i < width; i++) {
    for (size_t j = 0; j < height; j++) {
      bmp_pixel_init(&img.img_pixels[j][i], 255, 255, 255);
    }
  }

  int id;
  float x1, y1, x2, y2;
  while (fscanf(layout, "%d %f %f %f %f", &id, &x1, &y1, &x2, &y2) != -1) {
    printf("Drawing node %d\n", id);
    draw_hline(&img, x1, x2, y1, colors[id % num_colors]);
    draw_hline(&img, x1, x2, y2, colors[id % num_colors]);

    draw_vline(&img, x1, y1, y2, colors[id % num_colors]);
    draw_vline(&img, x2, y1, y2, colors[id % num_colors]);
  }

  bmp_img_write(&img, argv[3]);
  bmp_img_free(&img);

  return 0;
}
