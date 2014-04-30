/*
** bignum.c
**
** Copyright (c) h2so5 2014
**
** See Copyright Notice in LICENSE
*/

#include "bignum.h"
#include <string.h>

Bignum *bignum_alloc(mrb_state *mrb, const void *data, int length)
{
  Bignum *b = (Bignum*)mrb_malloc(mrb, sizeof(Bignum));
  b->mrb = mrb;
  b->len = length;
  b->data = (unsigned char*)malloc(length);
  memcpy(b->data, data, length);
  return b;
}
 
Bignum *bignum_alloc_char(mrb_state *mrb, unsigned char c)
{
  Bignum *b = (Bignum*)mrb_malloc(mrb, sizeof(Bignum));
  b->mrb = mrb;
  b->len = 1;
  b->data = (unsigned char*)malloc(1);
  b->data[0] = c;
  return b;
}
 
void bignum_free(Bignum *b)
{
  if (b) mrb_free(b->mrb, b->data);
  mrb_free(b->mrb, b);
}

static void bignum_extend(Bignum *b)
{
  b->len *= 2;
  b->data = (unsigned char*)mrb_realloc(b->mrb, b->data, b->len);
  memcpy(b->data + b->len / 2, b->data, b->len / 2);
  memset(b->data, 0, b->len / 2);
}

static int bignum_get_bit(const Bignum *b, int pos)
{
  int rpos = b->len * 8 - pos - 1;
  if (rpos < 0) return 0;
  return !!(b->data[rpos / 8] & (0x80 >> (rpos % 8)));
}
 
static void bignum_set_bit(Bignum *b, int pos, int value)
{
  int rpos = b->len * 8 - pos - 1;
  if (rpos < 0) {
    bignum_extend(b);
    rpos = b->len * 8 - pos - 1;
  }
  b->data[(rpos / 8)] &= ~(0x80 >> (rpos % 8));
  if (value) b->data[rpos / 8] |= (0x80 >> (rpos % 8));
}
 
int bignum_div(Bignum *b, int rhs)
{
  unsigned int remainder = 0;
  int i;
  
  for (i = b->len * 8 - 1; i >= 0; --i) {
    int bit = bignum_get_bit(b, i);
    remainder <<= 1;
    remainder += bit;
    if (remainder >= rhs) {
      remainder -= rhs;
      bignum_set_bit(b, i, 1);
    }
    else {
      bignum_set_bit(b, i, 0);
    }
  }
  return remainder;
}
 
void bignum_add(Bignum *lb, const Bignum *rb)
{
  int i;
  int carry = 0;
  
  for (i = 0; i < rb->len * 8 || carry; ++i) {
    int sum = bignum_get_bit(lb, i) + bignum_get_bit(rb, i) + carry;
    carry = (sum >= 2);
    bignum_set_bit(lb, i, sum % 2);
    
    if (carry && i == lb->len * 8 - 1) {
      bignum_extend(lb);
    }
  }
}
 
int bignum_is_zero(const Bignum *b)
{
  int i;
  for (i = 0; i < b->len; ++i) {
    if (b->data[i] != 0) return 0;
  }
  return 1;
}
 
int bignum_length(const Bignum *b)
{
  int max = b->len;
  int i;
  for (i = 0; i < b->len; ++i) {
    if (b->data[i] != 0) return max - i;
  }
  return max;
}
