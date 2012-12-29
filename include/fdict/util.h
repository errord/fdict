#ifndef _FDICT_UTIL_H_
#define _FDICT_UTIL_H_

/* Version */
#define MAKE_VERSION(major, minor) (char)(major<<4 | minor)
#define MAJOR_VERSION(ver) (ver>>4 & 0x0F)
#define MINOR_VERSION(ver) (ver & 0x0F)

#define TYPE_TABLE_HEAD(name) static const char *name[] = {"unknow",
#define TYPE_TABLE_END NULL};

void logging(const char *fmt, ...);
const char* type_to_string(const char **type_table, int type);
int string_to_type(const char **type_table, const char *type_string);

size_t gbk_strlen(const char *str);
size_t fdstrlen(enum word_encode encode, const char *str);

#endif /* _FDICT_UTIL_H_ */
