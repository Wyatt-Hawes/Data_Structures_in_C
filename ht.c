#include "city.h"
#include "ll.h"
#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTable {
  uint64_t salt;
  uint32_t size;
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_examined;
  bool mtf;
  LinkedList **lists;

} HashTable;

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

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {

  *nk = ht->n_keys;
  *nh = ht->n_hits;
  *nm = ht->n_misses;
  // printf("Examined is:%u", ht->n_examined);
  *ne = ht->n_examined;

  return;
}

HashTable *ht_create(uint32_t size, bool mtf) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht != NULL) {
    ht->mtf = mtf;
    ht->salt = 0x9846e4f157fe8840;
    ht->n_hits = ht->n_misses = ht->n_examined = 0;
    ht->n_keys = 0;
    ht->size = size;
    ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
    if (!ht->lists) {
      free(ht);
      ht = NULL;
    }
  }
  return ht;
}

void ht_delete(HashTable **ht) {

  for (uint32_t i = 0; i < (*ht)->size; i++) {

    // printf("%d\n", i);
    if ((*ht)->lists[i] != NULL) {

      ll_delete(&((*ht)->lists[i]));
    }
  }
  free((*ht)->lists);
  free(*ht);
  *ht = NULL;
  return;
}

uint32_t ht_size(HashTable *ht) { return ht->size; }

Node *ht_lookup(HashTable *ht, char *oldspeak) {
  
  if (ht == NULL){
    return NULL;
  }
  
  
  uint64_t hashed;
  uint64_t salt = ht->salt;

  hashed = (hash(salt, oldspeak)) % ht->size;
  /*
    // Hopefully setting current node to head node of lists[hash]
    LinkedList *ll = ht->lists[hashed];
    Node *current = ll->head;

    while (current != ll->tail) {
      ht->n_examined++;
      if (strcmp(current->oldspeak, oldspeak) == 0) {
        ht->n_hits++;
        return current;
      }
      current = current->prev;
    }
    ht->n_misses++;
    return NULL;
    */

  uint32_t oldseeks;
  uint32_t oldlinks;
  ll_stats(&oldseeks, &oldlinks);

  Node *n = ll_lookup(ht->lists[hashed], oldspeak);

  uint32_t links;
  uint32_t seeks;

  ll_stats(&seeks, &links);

  // printf();
  ht->n_examined += (links - oldlinks);
  if (n == NULL) {
    ht->n_misses++;
    return NULL;
  }

  ht->n_hits++;
  return n;
}


static Node *my_lookup(LinkedList *ll, char *oldspeak) {

  // performing seek, increase
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






void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {

  uint64_t hashed;
  hashed = hash(ht->salt, oldspeak) % ht->size;

  ht->n_keys++;
  if (ht->lists[hashed] == NULL) {

    ht->lists[hashed] = ll_create(ht->mtf);
  }
  
  if (my_lookup(ht->lists[hashed], oldspeak) != NULL){
  
  	ht->n_keys--;
  }

  ll_insert(ht->lists[hashed], oldspeak, newspeak);

  return;
}

uint32_t ht_count(HashTable *ht) {

  uint32_t count = 0;
  for (uint32_t i = 0; i < ht->size; i++) {

    if (ht->lists[i] != NULL) {
      count++;
    }
  }
  return count;
}

void ht_print(HashTable *ht) {

  // printf("Getting lists\n");
  LinkedList **all_lists = ht->lists;

  // printf("Getting first list\n");
  // LinkedList *current_list = all_lists[0];

  for (uint32_t i = 0; i < ht->size; i++) {

    LinkedList *current_list = all_lists[i];
    // printf("printing\n");

    // printf("%d===========================\n",i);
    if (current_list != NULL) {
      // printf("Is NULL\n");
      ll_print(current_list);
    }
  }
  return;
}
