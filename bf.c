#include "bv.h"
#include "city.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BloomFilter {

  int N_HASHES;
  uint64_t salts[5];
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;

} BloomFilter;
typedef struct BitVector {
  uint32_t length;
  uint64_t *vector;

} BitVector;
static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e,
                                   0x50d8bb08de3818df, 0x272347aea4045dd5,
                                   0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->n_keys = bf->n_hits = 0;
    bf->n_misses = bf->n_bits_examined = 0;
    for (int i = 0; i < 5; i++) {
      bf->salts[i] = default_salts[i];
    }
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  //printf("made bloom filter\n");
  return bf;
}

void bf_delete(BloomFilter **bf) {

  // printf("deleting\n");

  // bv_print((*bf)->filter);

  // BitVector *test = (*bf)->filter;

  // bv_print(test);

  bv_delete(&((*bf)->filter));
  free(*bf);
  *bf = NULL;
}

uint32_t bf_size(BloomFilter *bf) { return (bf->filter->length); }

void bf_insert(BloomFilter *bf, char *oldspeak) {

  uint64_t hashed;
  bf->n_keys++;
  for (int s = 0; s < 5; s++) {

    // printf("hasing:%s\n",oldspeak);
    hashed = hash(default_salts[s], oldspeak);
    // printf("Hashed: %lu\n", hashed);

    uint32_t bit = hashed % bf_size(bf);
    // printf("setting bit %u\n",bit);
    bv_set_bit(bf->filter, bit);
  }
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {

  uint64_t hashed;
  uint8_t bit;

  for (int s = 0; s < 5; s++) {

    bf->n_bits_examined++;
    hashed = hash(default_salts[s], oldspeak);
    uint32_t location = hashed % bf_size(bf);

    bit = bv_get_bit(bf->filter, location);

    if (bit == 0) {
      bf->n_misses++;
      return false;
    }
  }

  bf->n_hits++;
  return true;
}

uint32_t bf_count(BloomFilter *bf) {

  uint8_t bit;
  uint32_t total = 0;
  for (uint32_t i = 0; i < bf_size(bf); i++) {
    bit = bv_get_bit(bf->filter, i);
    if (bit == 1) {
      total++;
    }
  }
  return total;
}

void bf_print(BloomFilter *bf) {

  bv_print(bf->filter);
  return;
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {

  *nk = (bf->n_keys);

  *nh = (bf->n_hits);

  *nm = (bf->n_misses);

  *ne = (bf->n_bits_examined);
  return;
}
