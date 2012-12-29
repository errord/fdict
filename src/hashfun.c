#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/hashfun.h>

unsigned int elfhash(const void* key, int key_len)
{   
  unsigned char* pkey = (unsigned char*)key;
  unsigned long t=0,g;
  int i;
    
  for (i = 0; i < key_len; i++) {
    t=(t<<4) + pkey[i];
    g=t & 0xf0000000L;
    if(g) t^=g>>24;
    t &=~g;
  }
  return (t & 0x7FFFFFFF);
}

unsigned int myhash(const void* key, int key_len)
{
  unsigned char* pkey = (unsigned char*)key;
  unsigned int h = 0;
  int i;
  
  for (i = 0; i < key_len; i++) {
    h = 5*h + pkey[i];
  }
  return h;
}


unsigned int superFastHash(const void *key, int key_len)
{
  unsigned char* pkey = (unsigned char*)key;
  unsigned int hash = key_len, tmp;
  int rem;

  if (key_len <= 0 || pkey == NULL) return 0;

  rem = key_len & 3;
  key_len >>= 2;

  /* Main loop */
  for (;key_len > 0; key_len--) {
    hash  += get16bits (pkey);
    tmp    = (get16bits (pkey+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    pkey  += 2*sizeof (unsigned short);
    hash  += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (pkey);
    hash ^= hash << 16;
    hash ^= pkey[sizeof (unsigned short)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (pkey);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *pkey;
    hash ^= hash << 10;
    hash += hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

unsigned int additiveHash(const void* key, int key_len)
{  
  unsigned char* pkey = (unsigned char*)key;
  unsigned int hash;
  int i;
  for (hash = key_len, i = 0; i < key_len; i++)
    hash += pkey[i];
  
  return hash; 
}

unsigned int javahash(const void* key, int key_len)
{
  unsigned char* pkey = (unsigned char*)key;
  unsigned int h = 0;
  unsigned int off = 0;
  int i;
  
  for (i = 0; i < key_len; i++)
    h = 31 * h + pkey[off++];

  return h;
}


// RS Hash Function
unsigned int RSHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int b = 378551;
  unsigned int a = 63689;
  unsigned int hash = 0;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash = hash * a + (*pKey);
      a *= b;
    }

  return hash;
} 
 
// JS Hash Function
unsigned int JSHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int hash = 1315423911;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash ^= ((hash << 5) + (*pKey) + (hash >> 2));
    }

  return hash;
}
 
// P.J.Weinberger Hash Function
unsigned int PJWHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
  unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt * 3) / 4);
  unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);

  unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << \
    (BitsInUnignedInt - OneEighth);
  unsigned int hash             = 0;
  unsigned int test             = 0;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash = (hash << OneEighth) + (*(pKey));
      if ((test = hash & HighBits) != 0)
        {
          hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

  return hash;
} 

// BKDR Hash Function
unsigned int BKDRHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int seed = 131;  //  31 131 1313 13131 131313 etc..
  unsigned int hash = 0;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash = hash * seed + (*pKey);
    }

  return hash;
}

// SDBM Hash Function
unsigned int SDBMHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int hash = 0;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash = (*pKey) + (hash << 6) + (hash << 16) - hash;
    }

  return hash;
}

unsigned int Time33Hash(const void *key, int key_len)
{
  unsigned int nHash;
  unsigned char *pKey;
  unsigned char *pEnd;

  nHash = 0;
  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
	{
      nHash = (nHash << 5) + nHash + (*pKey);
	}

  return nHash;
}

// DJB Hash Function
unsigned int DJBHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int hash = 5381;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash += (hash << 5) + (*pKey);
    }

  return hash;
}

// AP Hash Function
unsigned int APHash(const void *key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  int i;
  unsigned int hash = 0;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key, i=0; pKey != pEnd; pKey++, i++)
    {
      if ((i & 1) == 0)
        {
          hash ^= ((hash << 7) ^ (*pKey) ^ (hash >> 3));
        }
      else
        {
          hash ^= (~((hash << 11) ^ (*pKey) ^ (hash >> 5)));
        }
    }

  return hash;
}

unsigned int calc_hashnr (const void* key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int nr = 1, nr2 = 4;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      nr ^= (((nr & 63) + nr2) * (*pKey)) + (nr << 8);
      nr2 += 3;
    }

  return nr;
}

unsigned int calc_hashnr1(const void* key, int key_len)
{
  unsigned char *pKey;
  unsigned char *pEnd;
  unsigned int hash = 0;

  pEnd = (unsigned char *)key + key_len;
  for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
      hash *= 16777619;
      hash ^= *pKey;
    }
  return hash;
}

#define mix(a,b,c)                              \
  {                                             \
    a -= b; a -= c; a ^= (c>>13);               \
    b -= c; b -= a; b ^= (a<<8);                \
    c -= a; c -= b; c ^= (b>>13);               \
    a -= b; a -= c; a ^= (c>>12);               \
    b -= c; b -= a; b ^= (a<<16);               \
    c -= a; c -= b; c ^= (b>>5);                \
    a -= b; a -= c; a ^= (c>>3);                \
    b -= c; b -= a; b ^= (a<<10);               \
    c -= a; c -= b; c ^= (b>>15);               \
  }

/* the key */
/* the length of the key */
/* the previous hash, or an arbitrary value */
unsigned int quick_hash(const void *key, int length)
{
  register unsigned int a,b,c,len, initval = 1000;
  unsigned char* k = (unsigned char*)key;
  
  /* Set up the internal state */
  len = length;
  a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
  c = initval;         /* the previous hash value */

  /*---------------------------------------- handle most of the key */
  while (len >= 12)
    {
      a += (k[0] +((unsigned int)k[1]<<8) +((unsigned int)k[2]<<16) +((unsigned int)k[3]<<24));
      b += (k[4] +((unsigned int)k[5]<<8) +((unsigned int)k[6]<<16) +((unsigned int)k[7]<<24));
      c += (k[8] +((unsigned int)k[9]<<8) +((unsigned int)k[10]<<16)+((unsigned int)k[11]<<24));
      mix(a,b,c);
      k += 12; len -= 12;
    }

  /*------------------------------------- handle the last 11 bytes */
  c += length;
  switch(len)              /* all the case statements fall through */
    {
    case 11: c+=((unsigned int)k[10]<<24);
    case 10: c+=((unsigned int)k[9]<<16);
    case 9 : c+=((unsigned int)k[8]<<8);
      /* the first byte of c is reserved for the length */
    case 8 : b+=((unsigned int)k[7]<<24);
    case 7 : b+=((unsigned int)k[6]<<16);
    case 6 : b+=((unsigned int)k[5]<<8);
    case 5 : b+=k[4];
    case 4 : a+=((unsigned int)k[3]<<24);
    case 3 : a+=((unsigned int)k[2]<<16);
    case 2 : a+=((unsigned int)k[1]<<8);
    case 1 : a+=k[0];
      /* case 0: nothing left to add */
    }
  mix(a,b,c);
  /*-------------------------------------------- report the result */
  return c;
}
