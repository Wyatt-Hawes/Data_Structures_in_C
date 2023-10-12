#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"

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

int main() {

  // printf("hello");
  /*
  uint32_t length = 100;
  BitVector *bv = bv_create(length);


          for (uint32_t i = 0; i < length; i++){
                  bv_set_bit(bv, i);
                  if (i % 2 == 0){
                  bv_clr_bit(bv,i);
                  }
          }

  bv_clr_bit(bv,127);
  bv_print(bv);


  for(int i = 0; i < 20; i+=5){
          uint8_t bit = bv_get_bit(bv, i);
          printf("bit %d is %d\n", i,bit );
  }
  bv_delete(&bv);


  Node *n = node_create("old", "new");
  printf("%p\n",(void *)n);
  node_print(n);
  node_delete(&n);
  printf("%p\n",(void *)n);


  //printf("making list\n");
  struct LinkedList *ll = ll_create(true);
  //printf("insert1\n");
  ll_insert(ll, "0", "old1");
  //printf("insert2\n");
  ll_insert(ll, "1", NULL);
  ll_insert(ll, "2", "213zasdasd");
  ll_insert(ll, "3", "old1");

  ll_insert(ll, "4", NULL);
  ll_insert(ll, "5", "b");
  ll_insert(ll, "6", NULL);
  ll_insert(ll, "7", "d");


  //printf("printing\n");
  ll_print(ll);
  printf("----------------------------\n");
  ll_lookup(ll, "17");
  ll_print(ll);
  //node_print(ll->head);
  //printf("len: %d", ll->length);

  ll_delete(&ll);


  BloomFilter *bf = bf_create(128);
  bf_print(bf);

  char *m = "Hello World!";

  printf("inserting\n");
  bf_insert(bf, m);
  bf_print(bf);

  bool b;

  printf("probing\n");
  b = bf_probe(bf, m);

  if (b){
          printf("yes!\n");
  }

  bf_delete(&bf);
  */
  // TEST HASHTABLE//TEST HASHTABLE

  /*
  HashTable *ht = ht_create(10, false);

  //ht_print(ht);
  printf("inserting\n");
  ht_insert(ht, "Hello", "World");
  ht_insert(ht, "one", "World");
  ht_insert(ht, "two", "World");
  ht_insert(ht, "three", NULL);
  ht_insert(ht, "four", "World");
  ht_insert(ht, "five", NULL);
  ht_insert(ht, "six", NULL);

  ht_insert(ht, "Hello", "");
  ht_insert(ht, "Hello", NULL);

  //uint32_t my_size = ht->size;
  ht_print(ht);
  printf("\n\n----------------\n");
  Node *inside = ht_lookup(ht, "Hello");
  if(inside != NULL){
          printf("is inside\n");
          node_print(inside);
  }
  printf("C:%u\n",ht_size(ht));
  ht_delete(&ht);
  ht_print(ht);
  */

  /*
  FILE *my_file = fopen("test.txt", "r");


  Parser *p = parser_create(my_file);

 compare("hello", "hi");


  char word[1000];
	int i = 0;
  while(next_word(p, word)){
  	printf("%s ",word);
  	i++;
  	if(i % 10 == 0){
  	printf("\n");
  	}
  }
  printf("\n");

  
 */
  char *one = "onee";
  char *two = "onee";

  int size1 = sizeof(one);
  int size2 = sizeof(two);
  
  printf("%d:%d",size1,size2);

  int c = compare(one, two);
  if (compare(one, two) == 0) {
    printf("Equal!: %d\n", c);
  } else {
    printf("different%d\n", c);
  }

  return 0;
}
