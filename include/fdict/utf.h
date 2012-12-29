#ifndef _UTF_H_
#define _UTF_H_

#define int2char(i) ((char*)&i)

size_t utf8len(const char *s);
size_t utf16len(const char *s);

/*
  return index char on utf8 string s
  return 0 then error, other return char
 */
int utf8char(const char *s, int idx);

unsigned short utf8_to_utf16(int utf8);

#endif /* _UTF_H_ */
