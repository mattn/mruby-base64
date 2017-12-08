#include <mruby.h>
#include <mruby/string.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#endif
#include <ctype.h>

const char* base64_chars = 
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";
#define is_base64(c) ( \
    isalnum((unsigned char)c) || \
    ((unsigned char)c == '+') || \
    ((unsigned char)c == '/'))

#define is_newline(c) ( \
    ((unsigned char)c == '\n') || \
    ((unsigned char)c == '\r'))

static unsigned char*
base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len, unsigned int *out_len) {
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3] = {0};
  unsigned char char_array_4[4] = {0};

  unsigned char* p = malloc(in_len * 3 + 1);
  unsigned char* top = p;
  *out_len = 0;
  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        *p++ = base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      *p++ = base64_chars[char_array_4[j]];

    while((i++ < 3))
      *p++ = '=';
  }
  *p = 0;
  *out_len = p - top;

  return top;
}

static unsigned char*
base64_decode(unsigned char const* bytes_to_decode, unsigned int in_len, unsigned int *out_len) {
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4] = {0};
  unsigned char char_array_3[3] = {0};

  unsigned char* p = malloc(in_len * 3 + 1);
  unsigned char* top = p;
  *out_len = 0;
  while (in_len-- && ( bytes_to_decode[in_] != '=') && (is_base64(bytes_to_decode[in_]) || is_newline(bytes_to_decode[in_]))) {
    if (!is_newline(bytes_to_decode[in_]))
      char_array_4[i++] = bytes_to_decode[in_];
    in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = strchr(base64_chars, char_array_4[i]) - base64_chars;

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        *p++ = char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = strchr(base64_chars, char_array_4[j]) - base64_chars;

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++)
      *p++ = char_array_3[j];
  }
  *p = 0;
  *out_len = p - top;

  return top;
}

static mrb_value
mrb_base64_encode(mrb_state *mrb, mrb_value self)
{
  mrb_value arg = mrb_nil_value();
  unsigned char* p;
  unsigned int len = 0;
  mrb_value str;

  mrb_get_args(mrb, "S", &arg);
  p = base64_encode((unsigned char*) RSTRING_PTR(arg), RSTRING_LEN(arg), &len);
  str = mrb_str_new(mrb, (char*) p, (size_t) len);
  free(p);
  return str;
}

static mrb_value
mrb_base64_decode(mrb_state *mrb, mrb_value self)
{
  mrb_value arg = mrb_nil_value();
  unsigned char* p;
  unsigned int len = 0;
  mrb_value str;

  mrb_get_args(mrb, "S", &arg);
  p = base64_decode((unsigned char*) RSTRING_PTR(arg), RSTRING_LEN(arg), &len);
  str = mrb_str_new(mrb, (char*) p, (size_t) len);
  free(p);
  return str;
}

void
mrb_mruby_base64_gem_init(mrb_state* mrb) {
  struct RClass* _class_base64 = mrb_define_module(mrb, "Base64");
  mrb_define_class_method(mrb, _class_base64, "encode", mrb_base64_encode, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, _class_base64, "decode", mrb_base64_decode, MRB_ARGS_REQ(1));
}

void
mrb_mruby_base64_gem_final(mrb_state* mrb) {
}

/* vim:set et ts=2 sts=2 sw=2 tw=0: */
