#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fdict/utf.h>

int main(int argv, char** argc)
{
  char *s = "汉字中华人民共和国打分aa好的";
  int a, i;
  unsigned short u16;
  int ul;
  ul = utf8len(s);
  for (i = 0; i < ul; i++) {
    a = utf8char(s, i);
    u16 = utf8_to_utf16(a);
    printf("%s\tutf8: 0x%x utf16: 0x%x\n", int2char(a), a, u16);
  }
  return 0;
}
