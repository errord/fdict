#include <stdlib.h>
#include <string.h>
#include <fdict/base_type.h>
#include <fdict/utf.h>
#include <fdict/ConvertUTF.h>

size_t utf8len(const char *s)
{
  size_t i = 0, j = 0;
  while (s[i]) {
    if ((s[i] & 0xc0) != 0x80) j++;
    i++;
  }
  return j;
}

size_t utf16len(const char *s)
{
  byte2 *b2 = (byte2*)s;
  size_t i;
  for (i = 0; b2[i]; ++i) ;
  return i;
}

const char* utf8char(const char *s, int *utf8)
{
  int t = 0;
  if (!*s)
    return NULL;
  /* 
     x & 1111 0000 = 1110 0000 
     1110 xxxx
     10xx xxxx
     10xx xxxx
  */
  if ((s[0] & 0x0F0) == 0x0E0) {
    t |= (s[0] & 0x0FF);
    t |= (s[1] & 0x0FF) << 8;
    t |= (s[2] & 0x0FF) << 16;
    s += 3;
  }
  /* 
     x & 1110 0000 = 1100 0000
     110x xxxx
     10xx xxxx
   */
  else if ((s[0] & 0x0E0) == 0x0C0) {
    t |= (s[0] & 0x0FF);
    t |= (s[1] & 0x0FF) << 8;
    s += 2;
  }
  /* 0xxx xxxx */
  else {
    t |= (s[0] & 0x0FF);
    s++;
  }
  *utf8 = t;
  return s;
}

int utf8char_at(const char *s, int idx)
{
  int i = 0, j = 0;
  int w = 0, f = 0;
  idx++;
  if (idx < 0)
    return 0;
  while (s[i]) {
    if ((s[i] & 0xc0) != 0x80) {
      j++;
    }
    if (j > idx)
      break;
    if (j == idx) {
      w |= (s[i] & 0x0FF) << f * 8;
      f++;
    }
    i++;
  }
  return w;
}

unsigned short utf8_to_utf16(int u8)
{
  const UTF8 *sutf8 = (UTF8*)&u8;
  const UTF8 *eutf8;
  UTF16 sutf16;
  UTF16 *psutf16 = (UTF16*)&sutf16;
  UTF16 *eutf16 = psutf16 + sizeof(UTF16);
  eutf8 = sutf8 + strlen(int2char(u8));
  ConvertUTF8toUTF16(&sutf8, eutf8, (UTF16**)&psutf16, eutf16, 0);
  return sutf16;
}
