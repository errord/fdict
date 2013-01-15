#ifndef _UTF_H_
#define _UTF_H_

#define int2char(i) ((char*)&i)

size_t utf8len(const char *s);
size_t utf16len(const char *s);

/*
  get utf8 char
  return NULL then end, other return next char point
 */
const char* utf8char(const char *s, int *utf8);

/*
  get utf8 char at index
  return 0 then error, other return char
 */
int utf8char_at(const char *s, int idx);

unsigned short utf8_to_utf16(int utf8);

#endif /* _UTF_H_ */
