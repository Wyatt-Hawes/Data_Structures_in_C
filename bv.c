#include "bv.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BitVector {
  uint32_t length;
  uint64_t *vector;

} BitVector;

BitVector *bv_create(uint32_t length) {

  int num_blocks = (length / 64) + 1;

  BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
  uint64_t *bit_array = calloc(num_blocks, sizeof(uint64_t));

  if (bit_array == NULL) {
    return NULL;
  }

  bv->length = length;
  bv->vector = bit_array;
  return bv;
}

void bv_delete(BitVector **bv) {
  free((*bv)->vector);
  free(*bv);
  *bv = NULL;
  return;
}

uint32_t bv_length(BitVector *bv) { return bv->length; }

void bv_set_bit(BitVector *bv, uint32_t i) {

  uint64_t key = 0x1;
  int block = i / 64;
  int bit = (i % 64);

  key = key << bit;
  bv->vector[block] = bv->vector[block] | key;
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
  // uint64_t last_zero; = 0xFFFFFFFFFFFFFFFE;
  // uint64_t fix; = 0xFFFFFFFFFFFFFFFF;
  int block = i / 64;
  int bit = (i % 64);

  uint64_t key = 0x1;

  key = ~(key << bit);
  // uint64_t fix = fix >> (64-(bit-1));
  // key = key | fix;
  bv->vector[block] = bv->vector[block] & key;
}
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {

  int block = i / 64;
  int bit = (i % 64);

  uint64_t result = bv->vector[block] << ((64 - 1) - bit);
  result = result >> 63;

  return (uint8_t)result;
}

void bv_print(BitVector *bv) {

  for (uint64_t i = 0; i < bv->length; i++) {
    uint8_t current_bit = bv_get_bit(bv, i);
    if (current_bit == 0) {
      printf("0");
    } else {
      printf("1");
    }
  }
  printf("\n");
}
