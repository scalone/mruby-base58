/*
** bignum.h
**
** Copyright (c) h2so5 2014
**
** See Copyright Notice in LICENSE
*/

#include <mruby.h>

typedef struct {
  unsigned char *data;
  int len;
  mrb_state *mrb;
} Bignum;

Bignum *bignum_alloc(mrb_state *mrb, const void *data, int length);
Bignum *bignum_alloc_char(mrb_state *mrb, unsigned char c);
void bignum_free(Bignum *b);

int bignum_div(Bignum *b, int rhs);
void bignum_add(Bignum *lb, const Bignum *rb);
int bignum_is_zero(const Bignum *b);
int bignum_length(const Bignum *b);

