#ifndef _FDICT_MEMORY_H_
#define _FDICT_MEMORY_H_

#define fdmalloc(var, type, size) var = (type)malloc(size);	\
  if (!var) {							\
    printf("Out Of Memory\n");					\
    exit(0);							\
  }

#define fdmalloc0(var, type, size) fdmalloc(var, type, size);	\
  memset(var, 0, size)

#define fdfree(var) free(var)

#endif /* _FDICT_MEMORY_H_ */
