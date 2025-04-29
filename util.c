#include "hw14.h"

#include <ctype.h>
#include <stdlib.h>
#include <strings.h>

static void insert_tail(struct DOMNodeList **list_ptr,
                        struct DOMNodeList *new_node) {
  if (*list_ptr == NULL) {
    *list_ptr = new_node;
    return;
  }

  struct DOMNodeList *cur;
  for (cur = *list_ptr; cur->next != NULL; cur = cur->next)
    ;
  cur->next = new_node;
}

static void add_child(struct DOMNode *parent, struct DOMNode *child) {
  struct DOMNodeList *new_node = malloc(sizeof(struct DOMNodeList));
  new_node->node = child;
  new_node->next = NULL;
  insert_tail(&parent->children, new_node);
}

static void skip_whitespace(struct stream *s) {
  while (isspace(s->text[s->pos]))
    s->pos++;
}

static int read_int(struct stream *s) {
  int read = 0;
  while (isdigit(s->text[s->pos])) {
    read *= 10;
    read += s->text[s->pos++] - 48;
  }
  return read;
}

static float read_float(struct stream *s) {
  float top = read_int(s);
  if (s->text[s->pos] != '.') {
    return top;
  }

  size_t old_pos = ++s->pos;
  float bottom = read_int(s);
  size_t num_digits = s->pos - old_pos;

  for (size_t i = 0; i < num_digits; i++) {
    bottom /= 10;
  }

  return top + bottom;
}

struct DOMNode *load_tree(struct stream *s) {

  if (s->text[s->pos] != '(') {
    fprintf(stderr, "No tree found in %s\n", s->text + s->pos);
    exit(1);
  }
  s->pos++;
  skip_whitespace(s);

  struct DOMNode *root = malloc(sizeof(struct DOMNode));
  root->children = NULL;

  root->id = read_int(s);
  skip_whitespace(s);

  if (!strncasecmp(s->text + s->pos, "horiz", 5)) {
    root->layout_direction = LAYOUT_HORIZ;
    s->pos += 5;
  } else if (!strncasecmp(s->text + s->pos, "vert", 4)) {
    root->layout_direction = LAYOUT_VERT;
    s->pos += 4;
  } else if (!strncasecmp(s->text + s->pos, "none", 4)) {
    root->layout_direction = LAYOUT_NONE;
    s->pos += 4;
  } else {
    fprintf(stderr, "Unrecognized layout for node ID %d\n", root->id);
    free(root);
    exit(1);
  }
  skip_whitespace(s);

  root->margin = read_float(s);
  skip_whitespace(s);

  while (s->text[s->pos] == '(') {
    struct DOMNode *child = load_tree(s);
    add_child(root, child);
    skip_whitespace(s);
  }

  if (s->text[s->pos] != ')') {
    fprintf(stderr, "Missing `)` at position %zu: %s\n", s->pos,
            s->text + s->pos);
    exit(1);
  }
  s->pos++;
  return root;
}
