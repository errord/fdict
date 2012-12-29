/*
 *
 * clib -- libcheck
 * test check
 *
 */
#ifndef _CLIB_LIBCHECK_H_
#define _CLIB_LIBCHECK_H_

void check_start(const char* info);
void check(const char* info, int b, const char* file, int line);
void check_end();
#define CHECK(i, b) check(i, b, __FILE__, __LINE__)

#endif /* _CLIB_LIBCHECK_H_ */

