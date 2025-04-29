#include "hw14.h"
#include <stdlib.h>

void compute_layout(struct DOMNode *node, float left, float top, float width, float height, FILE *target) {
  float margin_x = node->margin * width / 2;
  float margin_y = node->margin * height / 2;

  float inner_left = left + margin_x;
  float inner_top = top + margin_y;
  float inner_width = width - 2 * margin_x;
  float inner_height = height - 2 * margin_y;

  fprintf(target, "%d %.2f %.2f %.2f %.2f\n", node->id, left, top, left + width, top + height);

  // Count children
  int count = 0;
  struct DOMNodeList *curr = node->children;
  while (curr) {
    count++;
    curr = curr->next;
  }

  if (count == 0) return;

  float child_width = inner_width;
  float child_height = inner_height;

  if (node->layout_direction == LAYOUT_HORIZ) {
    child_width = inner_width / count;
  } else if (node->layout_direction == LAYOUT_VERT) {
    child_height = inner_height / count;
  }

  curr = node->children;
  for (int i = 0; i < count && curr; i++) {
    float child_left = inner_left;
    float child_top = inner_top;
    if (node->layout_direction == LAYOUT_HORIZ) {
      child_left = inner_left + i * child_width;
    } else if (node->layout_direction == LAYOUT_VERT) {
      child_top = inner_top + i * child_height;
    }
    compute_layout(curr->node, child_left, child_top, child_width, child_height, target);
    curr = curr->next;
  }
}

void layout(struct DOMNode *root, float width, float height, FILE *target) {
  fprintf(target, "%.2f %.2f\n", width, height);
  compute_layout(root, 0, 0, width, height, target);
}

void free_DOMTree(struct DOMNode *root) {
  if (!root) return;

  struct DOMNodeList *curr = root->children;
  while (curr) {
    struct DOMNodeList *next = curr->next;
    free_DOMTree(curr->node);
    free(curr);
    curr = next;
  }

  free(root);
}
