#ifndef _CODEBASE_DATASTRUCT__HASH_H_
#define _CODEBASE_DATASTRUCT__HASH_H_

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const unsigned short *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((unsigned int)(((const unsigned char *)(d))[1])) << 8)\
                      +(unsigned int)(((const unsigned char *)(d))[0]) )
#endif

unsigned int elfhash(const void* key, int key_len);
unsigned int myhash(const void* key, int key_len);
unsigned int superFastHash (const void* key, int key_len);
unsigned int additiveHash(const void* key, int key_len);
unsigned int javahash(const void* key, int key_len);
unsigned int RSHash(const void *key, int key_len);
unsigned int JSHash(const void *key, int key_len);
unsigned int PJWHash(const void *key, int key_len);
unsigned int BKDRHash(const void *key, int key_len);
unsigned int SDBMHash(const void *key, int key_len);
unsigned int Time33Hash(const void *key, int key_len);
unsigned int DJBHash(const void *key, int key_len);
unsigned int APHash(const void *key, int key_len);
unsigned int calc_hashnr (const void* key, int key_len);
unsigned int calc_hashnr1(const void* key, int key_len);
unsigned int quick_hash(const void *key, int length);

#endif /* _CODEBASE_DATASTRUCT__HASH_H_ */
