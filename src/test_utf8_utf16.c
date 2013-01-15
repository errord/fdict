#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fdict/utf.h>

void test__utf8char_at(const char *s)
{
  int a, i;
  unsigned short u16;
  int ul;
  printf("test__utf8char_at:\n");
  ul = utf8len(s);
  for (i = 0; i < ul; i++) {
    a = utf8char_at(s, i);
    u16 = utf8_to_utf16(a);
    printf("\t%s\tutf8: 0x%x utf16: 0x%x\n", int2char(a), a, u16);
  }
}

void test__utf8char(const char *s)
{
  const char *p = s;
  int u8;
  unsigned short u16;
  printf("test__utf8char:\n");
  while ((p = utf8char(p, &u8))) {
    u16 = utf8_to_utf16(u8);
    printf("\t%s\tutf8: 0x%x utf16: 0x%x\n", int2char(u8), u8, u16);    
  }
}

int main(int argv, char** argc)
{
  char *s = "汉字中12华人民共和国打分aa好的";
  test__utf8char_at(s);
  test__utf8char(s);
  return 0;
}
