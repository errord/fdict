#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fdict/wordbase.h>
#include <fdict/utf.h>
#include <fdict/util.h>

void logging(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  printf(fmt, ap);
  va_end(ap);
}

const char* type_to_string(const char **type_table, int type)
{
  return type_table[type];
}

int string_to_type(const char **type_table, const char *type_string)
{
  int i = 0;
  while (type_table[i]) {
    if (!strcmp(type_table[i], type_string))
      return i;
    i++;
  }
  return 0;
}

size_t gbk_strlen(const char *str)
{
  size_t l;
  while (*str) {
    if ((unsigned char)*str > 0x80)
      str++;
    l++;
  }
  return l;
}

size_t fdstrlen(enum word_encode encode, const char *str)
{
  switch (encode) {
  case gbk:
    return gbk_strlen(str);
  case utf8:
    return utf8len(str);
  case utf8_short:
    return utf16len(str);
  }
  return 0;
}

