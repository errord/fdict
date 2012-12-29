/*
 * clib -- libhash linehash
 *
 */
#ifndef _CLIB_LIBHASH_LINEHASH_H_
#define _CLIB_LIBHASH_LINEHASH_H_

#define HASH RSHash

#define MEMOUT CLIBMEMOUT
#define MEMOUT_EXIT(ret) CLIBMEMOUT_EXIT(ret)
/* define memory allocate */
#define MALLOC CLIBMALLOC
#define FREE CLIBFREE

struct ht_node_s
{
	struct ht_node_s* list_next;
	struct ht_node_s* next;
	int value;
	unsigned int hashcode;
	char key[0];
};
typedef struct ht_node_s ht_node;

struct hashtable_s
{
	unsigned int size;     /* hash table node size */
	unsigned int htsize;   /* hashtable slot size */
	unsigned int maxvalue; /* max value */
	ht_node** hashtable;
};
typedef struct hashtable_s hashtable;

/* create hash table */
hashtable* create_hashtable(unsigned int hashtable_size);

/*
 * destroy hash table 
 * free all ht_node and hashtable, but ht_node within memory not free
 */
void destroy_hashtable(hashtable* ht);

/*
 * clear hashtable
 * free all ht_node, but ht_node within memory no free
 */
void ht_clear(hashtable* ht);

/* set value to hash table */
void ht_set(hashtable* ht, const char* key, unsigned int value);

/*
 * get value from hash table
 * return set value if key exist, other return 0
 */
unsigned int ht_get(hashtable* ht, const char* key);

/*
 * key is exist
 * return 1 if key exist, other return 0
 */
int ht_key_exist(hashtable* ht, const char* key);

/*
 * get hash table size
 */
int ht_size(hashtable* ht);

/*
 * get hash table node list
 */
ht_node* ht_get_node_list(hashtable* ht);

/*
 * sort hashtable return sorted list
 */
ht_node* ht_sort(hashtable* ht, char order);

/*
 * out hash table to file or terminal, content is text
 * print to stdout if out_file_name is null
 * 0 if error, other success
 */
int ht_out_text(hashtable* ht, const char* out_file_name);

/*
 * resume hash table from text
 * NULL if error, other hash table
 */
hashtable* ht_resume_text(const char* in_file_name);

/*
 * out hash table list to file or terminal, content is text
 * print to stdout if out_file_name is null
 * 0 if error, other success
 */
int ht_list_out_text(ht_node* listhead, const char* out_file_name);

#endif /* _CLIB_LIBHASH_LINEHASH_H_ */

