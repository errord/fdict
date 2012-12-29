/*
 * clib -- libdsout
 * data struct out
 *
*/
#ifndef _CLIB_LIBDSOUT_H_
#define _CLIB_LIBDSOUT_H_


/*
 * outfile is NULL, out to terminal
 * function return typeis "int", and 0 == error
 *
 * DSOUT_DEFINE {at function declare start}
 * DSOUT_OPEN_OUTFILE {at function code start}
 * DSOUT_INF {at out info}
 * DSOUT_CLOSE_OUTFILE {at exit function}
 * 
 * DSOUT_BUFF {info buff}
 * DSOUT_CBUFF {info continue buff}
 * DSOUT_FD {file descriptor}
 * 
 * sprintf(DSOUT_BUFF, "hello") {out info to DSOUT_BUFF}
 *
*/

#define DSOUT_BUFF_SIZE 1024
#define DSOUT_DEFINE char buff[DSOUT_BUFF_SIZE];  \
  FILE* fd = NULL;
#define DSOUT_BUFF buff
#define DSOUT_CBUFF DSOUT_BUFF+strlen(DSOUT_BUFF)
#define DSOUT_FD fd

#define DSOUT_OPEN_OUTFILE(of)   if (of != NULL)                   \
    {                                                             \
      DSOUT_FD = fopen(of, "w");                                  \
      if (DSOUT_FD == NULL)                                       \
        return 0;                                                 \
    }

#define DSOUT_CLOSE_OUTFILE if (DSOUT_FD != NULL) fclose(DSOUT_FD)

#define DSTO_FILE if (DSOUT_FD != NULL) {
#define DSTO_STD                                \
  }                                             \
 else {
#define DSTO_END                                \
  }
#define DSOUTFILE(buff)   fwrite(buff, strlen(buff), sizeof(char), fd)
#define DSOUTSTD(buff) printf("%s", buff)
#define DSOUT_INFO(buff)  DSTO_FILE;              \
  DSOUTFILE(buff);                                \
  DSTO_STD;                                       \
  DSOUTSTD(buff);                                 \
  DSTO_END;

#endif /* _CLIB_LIBDSOUT_H_ */
