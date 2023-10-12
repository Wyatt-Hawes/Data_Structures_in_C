#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "ll.h"
#include "messages.h"
#include "node.h"
#include "parser.h"

void print_usage() {

  fprintf(stderr,
      "Usage: ./banhammer [options]\n"
"  ./banhammer will read in words from stdin, identify any badspeak or old speak and output an\n"
"  appropriate punishment message. The badspeak and oldspeak (with the newspeak translation)\n"
"  that caused the punishment will be printed after the message. If statistics are enabled\n"
"  punishment messages are suppressed and only statistics will be printed.\n"
"    -t <ht_size>: Hash table size set to <ht_size>. (default: 10000)\n"
"    -f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\n"
"    -s          : Enables the printing of statistics.\n"
"    -m          : Enables move-to-front rule.\n"
"    -h          : Display program synopsis and usage.\n");
}

int length(char *word) {
  int i = 0;

  while (word[i] != 0) {
    i++;
  }
  return i;
}
/*
typedef struct BloomFilter {

  int N_HASHES;
  uint64_t salts[5];
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;

} BloomFilter;
*/
int main(int argc, char **argv) {

  uint32_t hash_size = 10000;
  uint32_t bloom_size = 1 << 19;
  bool mtf = false;
  bool statistics = false;

  int value;

  while ((value = getopt(argc, argv, "t:f:msh")) != -1) {

    switch (value) {

    case 't':
      if (atol(optarg) < 1){
      
      	fprintf(stderr, "./banhammer: Invalid hash table size.\n");
      	return 1;
      }
      hash_size = atol(optarg);
      break;

    case 'f':
    	if (atol(optarg)< 1) {
    	fprintf(stderr, "./banhammer: Invalid bloom filter table size.\n");
    	return 1;
 	 }
      bloom_size = atol(optarg);
      break;

    case 'm':
      mtf = true;
      break;

    case 's':
      statistics = true;
      break;

    case 'h':
      // Print usage and program synopsis
      print_usage();
      return 0;
      break;
    default:
      // printf("invalid input");
      print_usage();
      return 1;
      break;
    }
  }
  //printf("H size: %d\n", hash_size);
  if (hash_size < 1) {
    //printf("inside\n");
    fprintf(stderr, "./banhammer: Invalid hash table size.\n");
    return 1;
  }

  if (bloom_size < 1) {
    fprintf(stderr, "./banhammer: Invalid bloom filter table size.\n");
    return 1;
  }

  char word[MAX_PARSER_LINE_LENGTH + 1];

  BloomFilter *bf = bf_create(bloom_size);
  HashTable *ht = ht_create(hash_size, mtf);
  //printf("made bloom filter\n");
  FILE *b = fopen("badspeak.txt", "r");
  if (b == NULL) {
    fprintf(stderr, "banhammer: Couldn't open badspeak.txt to read bad speak.\n"
                    ": No such file or directory\n");
    return 1;
  }

  Parser *bad = parser_create(b);
  FILE *new = fopen("newspeak.txt", "r");

  if (new == NULL) {
    fprintf(stderr, "banhammer: Couldn't open newspeak.txt to read old speak "
                    "and new speak translation.\n"
                    ": No such file or directory\n");
    return 1;
  }

  // next_word is using fgets()
  while (next_word(bad, word)) {

    bf_insert(bf, word);
    ht_insert(ht, word, NULL);
  }

  while (fgets(word, MAX_PARSER_LINE_LENGTH, new)) {

    int len = length(word);
    int i;
    word[len - 1] = 0;
    for (i = 0; i < len; i++) {
      if (word[i] == ' ') {
        word[i] = 0;
        break;
      }
    }

    bf_insert(bf, word);

    if (i == len) {
      ht_insert(ht, word, NULL);
    } else {

      ht_insert(ht, word, &word[i + 1]);
    }
  }
  fclose(new);

  Parser *input = parser_create(stdin);

  // Thoughtcrime is when there is NO NEWSPEAK TRANSLATION
  LinkedList *thoughtcrime = ll_create(mtf);
  LinkedList *rightspeak = ll_create(mtf);

  while (next_word(input, word)) {

    // Is word in the bloom filter?
    if (!bf_probe(bf, word)) {
      continue;
    }

    Node *n = ht_lookup(ht, word);
	//printf("%p\n", (void *)n);
    // Checking for false positive in bloom filter
    if (n == NULL) {
      continue;
    }
    //printf("%p\n", (void *)n);
    // Checking if there is NO newspeak translation (thoughtcrime)
    if (n->newspeak == NULL) {

      ll_insert(thoughtcrime, n->oldspeak, NULL);
      continue;
    }

    ll_insert(rightspeak, n->oldspeak, n->newspeak);
    continue;
  }
 // printf("out\n");
  /*
  printf("====================\n");
  printf("thoughtcrime:\n");
  ll_print(thoughtcrime);

  printf("====================\n");

  printf("rightspeak:\n");
  ll_print(rightspeak);
  */

  if (statistics) {

    	uint32_t bf_keys = 0;
    	uint32_t bf_hits = 0;
    	uint32_t bf_misses = 0;
    	uint32_t bf_bits_examined = 0;
    	bf_stats(bf, &bf_keys, &bf_hits, &bf_misses, &bf_bits_examined);

    	uint32_t ht_keys = 0;
    	uint32_t ht_hits = 0;
    	uint32_t ht_misses = 0;
    	uint32_t ht_bits_examined = 0;
    	ht_stats(ht, &ht_keys, &ht_hits, &ht_misses, &ht_bits_examined);
    
    	float b_per_m;
    	if (bf_misses > 0){
    	b_per_m =
    	    (float)(bf_bits_examined - (5 * bf_hits)) / ((float)bf_misses);
    	}else {b_per_m = 0;}
    	
    	float false_p;
    	if (bf_hits >0){
    	false_p = (float)ht_misses / (float)bf_hits;
    	} else {false_p = 0;}
    	
    	float bloom_load = (float)bf_count(bf) / (float)bf_size(bf);
    	
    	float avg_seek_len;
    	if (ht_hits > 0 || ht_misses > 0){
    	
    	avg_seek_len =
     	   ((float)ht_bits_examined) / (((float)ht_hits + (float)ht_misses));
	} else {avg_seek_len = 0;}
    	fprintf(stdout, "ht keys: %u\n"
		"ht hits: %u\n"
		"ht misses: %u\n"
		"ht probes: %u\n"
		"bf keys: %u\n"
		"bf hits: %u\n"
		"bf misses: %u\n"
		"bf bits examined: %u\n"
		"Bits examined per miss: %.6f\n"
		"False positives: %.6f\n"
		"Average seek length: %.6f\n"
		"Bloom filter load: %.6f\n",
 
           ht_keys, ht_hits, ht_misses, ht_bits_examined,

           bf_keys, bf_hits, bf_misses, bf_bits_examined,

           b_per_m, false_p, avg_seek_len, bloom_load);

    // printf("examined:%d  hits:%d misses:%d\n", ht_bits_examined, ht_hits,
    // ht_misses);

    // printf("BF: K:%u H:%u M:%u E:%u\n", bf_keys, bf_hits, bf_misses,
    // bf_bits_examined); printf("HT: K:%u H:%u M:%u E:%u\n", ht_keys, ht_hits,
    // ht_misses, ht_bits_examined);
  }

  // If both lists filled, mixspeak
  if (!statistics) {
    if (ll_length(thoughtcrime) > 0 && ll_length(rightspeak) > 0) {
      printf("%s", mixspeak_message);
      ll_print(thoughtcrime);
      ll_print(rightspeak);
    }

    // If Only thoughtcrime, badspeak message
    else if (ll_length(thoughtcrime) > 0) {
      printf("%s", badspeak_message);
      ll_print(thoughtcrime);
    } else if (ll_length(rightspeak)) {
      printf("%s", goodspeak_message);
      ll_print(rightspeak);
    }
  }

  // If only rightspeak, goodspeak message

  // node_print(ht_lookup(ht, "hand"));

  ll_delete(&thoughtcrime);
  ll_delete(&rightspeak);
  ht_delete(&ht);
  bf_delete(&bf);

  parser_delete(&input);
  parser_delete(&bad);
  return 0;
}
