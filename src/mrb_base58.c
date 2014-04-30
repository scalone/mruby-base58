/*
** mrb_base58.c
**
** Copyright (c) h2so5 2014
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "bignum.h"
#include <string.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

static struct RClass *base58;
static const char default_chars[] = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
 
static int base58_encoded_enough_length(int length)
{
  return length * 1.6; /* = 8.0 / floor(log2(58)) */
}
 
static int base58_encode_reverse(mrb_state *mrb, char *dst, const char *data, int length, const char *chars)
{
  Bignum *b = bignum_alloc(mrb, data, length);
  int index = 0;
  int i;
  
  do {
    dst[index] = chars[bignum_div(b, 58)];
    index++;
  } while (!bignum_is_zero(b));
  
  for (i = 0; i < length; ++i) {
    if (data[i] == 0) {
      dst[index] = chars[0];
      index++;
    }
    else {
      break;
    }
  }
  bignum_free(b);
  return index;
}
 
static int base58_decode(mrb_state *mrb, char *dst, const char *data, int length, const char *chars)
{
  int leading_zero = 0;
  int i;
  Bignum *result = bignum_alloc_char(mrb, 0);
  Bignum *base = bignum_alloc_char(mrb, 1);
  int b_length;

  for (i = 0; i < length; ++i) {
    if (data[i] == chars[0]) {
      leading_zero++;
    }
    else {
      break;
    }
  }
  
  memset(dst, 0, leading_zero);
  
  for (i = length - 1; i > leading_zero - 1; --i) {
    
    Bignum *f = bignum_alloc_char(mrb, 0);
    int j, c = -1;
    
    for (j = 0; j < 58; ++j) {
      if (data[i] == chars[j]) {
        c = j;
        break;
      }
    }
    
    if (c == -1) return -1;
    
    for (j = 0; j < c; ++j) bignum_add(result, base);
    for (j = 0; j < 58; ++j) bignum_add(f, base);
    
    bignum_free(base);
    base = f;
  }
  
  bignum_free(base);
  
  b_length = bignum_length(result);
  memcpy(dst + leading_zero, result->data + result->len - b_length , b_length);
  bignum_free(result);
  
  return leading_zero + b_length;
}

static mrb_value mrb_base58_encode(mrb_state *mrb, mrb_value self)
{
  const char *chars = default_chars;
  char *custom_chars;
  char *data;
  mrb_int data_len;
  mrb_value result;
  mrb_int result_len = 0;

  if (mrb_get_args(mrb, "s|z", &data, &data_len, &custom_chars) == 2) {
    if (strlen(custom_chars) != 58) mrb_raise(mrb, E_ARGUMENT_ERROR, "Base58 requires 58 symbols");
    chars = custom_chars;
  }

  result = mrb_str_buf_new(mrb, base58_encoded_enough_length(data_len));
  result_len = base58_encode_reverse(mrb, RSTRING_PTR(result), data, data_len, chars);
  result = mrb_str_resize(mrb, result, result_len);

  return mrb_funcall(mrb, result, "reverse", 0);
}

static mrb_value mrb_base58_decode(mrb_state *mrb, mrb_value self)
{
  const char *chars = default_chars;
  char *custom_chars;
  char *data;
  mrb_int data_len;
  mrb_value result;
  mrb_int result_len = 0;

  if (mrb_get_args(mrb, "s|z", &data, &data_len, &custom_chars) == 2) {
    if (strlen(custom_chars) != 58) mrb_raise(mrb, E_ARGUMENT_ERROR, "Base58 requires 58 symbols");
    chars = custom_chars;
  }

  result = mrb_str_buf_new(mrb, data_len);
  result_len = base58_decode(mrb, RSTRING_PTR(result), data, data_len, chars);

  if (result_len < 0) mrb_raise(mrb, E_ARGUMENT_ERROR, "invalid Base58 symbol");

  return mrb_str_resize(mrb, result, result_len);
}

void mrb_mruby_base58_gem_init(mrb_state *mrb)
{
    base58 = mrb_define_module(mrb, "Base58");
    mrb_define_module_function(mrb, base58, "encode", mrb_base58_encode, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
    mrb_define_module_function(mrb, base58, "decode", mrb_base58_decode, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
    DONE;
}

void mrb_mruby_base58_gem_final(mrb_state *mrb)
{
}

