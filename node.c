#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int length(char *word) {
  int i = 0;

  while (word[i] != 0) {
    i++;
  }
  return i;
}

static char *copy(char *source) {

  if (source == NULL) {
    return NULL;
  }

  int l = length(source) + 1;

  char *cpy = calloc(l, sizeof(char));
  memcpy(cpy, source, l);
  return cpy;
}

Node *node_create(char *oldspeak, char *newspeak) {

  Node *n = (Node *)malloc(sizeof(Node));

  n->oldspeak = copy(oldspeak);
  n->newspeak = copy(newspeak);

  n->next = NULL;
  n->prev = NULL;

  return n;
}

void node_delete(Node **n) {
  free((*n)->oldspeak);
  free((*n)->newspeak);

  free(*n);
  *n = NULL;
}

void node_print(Node *n) {
  if (n->oldspeak != NULL && n->newspeak != NULL) {
    printf("%s -> %s\n", n->oldspeak, n->newspeak);
  }
  if (n->oldspeak != NULL && n->newspeak == NULL) {
    printf("%s\n", n->oldspeak);
  }
}
