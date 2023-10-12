#include "ll.h"
#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern uint64_t seeks; // Number of seeks performed.
// extern uint64_t links; // Number of links traversed.

uint64_t seeks;
uint64_t links;

typedef struct LinkedList {
  uint32_t length;
  Node *head;
  Node *tail;
  bool mtf;
} LinkedList;

static int compare(char *word1, char *word2) {

  if (word1 == NULL || word2 == NULL) {
    return 1;
  }

  if (sizeof(word1) != sizeof(word2)) {
    return 1;
  }

  int i = 0;
  while (word1[i] != 0 || word2[i] != 0) {

    if (word1[i] != word2[i]) {
      return 1;
    }
    i++;
  }
  return 0;
}

LinkedList *ll_create(bool mtf) {

  LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));

  ll->mtf = mtf;
  ll->length = 0;

  Node *sentinel_head = node_create("HEAD", NULL);
  Node *sentinel_tail = node_create("TAIL", NULL);

  sentinel_head->prev = sentinel_tail;
  sentinel_tail->next = sentinel_head;

  ll->head = sentinel_head;
  ll->tail = sentinel_tail;

  return ll;
}

void ll_delete(LinkedList **ll) {

  Node *current = (*ll)->head;
  Node *next;
  while (current->prev != NULL) {
    // printf("looping\n");
    next = current->prev;
    node_delete(&current);
    current = next;
  }
  node_delete(&(*ll)->tail);
  free(*ll);
  *ll = NULL;
}

uint32_t ll_length(LinkedList *ll) { return ll->length; }

Node *ll_lookup(LinkedList *ll, char *oldspeak) {

  // performing seek, increase
  seeks++;
  if (ll == NULL){
  	return NULL;
  }
  // Previous
  Node *current = ll->head->prev;
  Node *next;
  //printf("1\n");
  // printf("\n");
  // links--;
  while (current != ll->tail) {

    // traversing a link
    // links++;
    links++;
    next = current->prev;
//printf("2\n");
    // Using my own compare
    if (compare(oldspeak, current->oldspeak) == 0) {

      // Move node to the front if mtf = true
      // printf("Lookup: %s exists\n", oldspeak);

      if (ll->mtf) {

        // These two nodes need to get connected
        // printf("moving to front\n");

        // Fixing hole after pulling node out
        Node *after = current->prev;
        Node *before = current->next;

        before->prev = after;
        after->next = before;

        Node *before_head = ll->head->prev;

        // inserting node at front
        ll->head->prev = current;
        current->next = ll->head;
        current->prev = before_head;
        before_head->next = current;
      }
      return current;
    }
    current = next;
    // printf("---------------\n");
  }
  //printf("4\n");
  return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {

  // printf("1\n");
  uint32_t oldlink = links;
  if (ll_lookup(ll, oldspeak) != NULL) {
    links = oldlink;
    // printf("Insert: Node exists already\n");
    return;
  }
  links = oldlink;
  // printf("storing head and after head\n");
  Node *head = ll->head;
  Node *after_head = head->prev;

  // node_print(after_head);
  // printf("creating insert node\n");
  Node *insert = node_create(oldspeak, newspeak);

  // setting vars
  insert->next = head;
  insert->prev = after_head;

  head->prev = insert;
  after_head->next = insert;

  ll->length++;
}

void ll_print(LinkedList *ll) {

  Node *current = ll->head->prev;
  Node *next;
  // printf("(");
  while (current->prev != NULL) {
    next = current->prev;
    node_print(current);
    current = next;
  }
  // printf("Length:%d\n", ll->length);
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {

  *n_seeks = seeks;
  *n_links = links;
}
