#ifndef HW14
#define HW14

#include <stdio.h>

enum dir { LAYOUT_HORIZ, LAYOUT_VERT, LAYOUT_NONE };

struct DOMNode;
struct DOMNodeList {
  struct DOMNode *node;
  struct DOMNodeList *next;
};

struct DOMNode {
  int id;
  float margin;
  enum dir layout_direction;
  struct DOMNodeList *children;
};

void layout(struct DOMNode *root, float width, float height, FILE *target);
void free_DOMTree(struct DOMNode *root);

// UTILITY STUFF HERE
// DO NOT MODIFY
struct stream {
  char *text;
  size_t pos;
  size_t length;
};

struct DOMNode *load_tree(struct stream *s);

#endif // HW14
