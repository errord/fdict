/*
 * libhash
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include <fdict/clib.h>
#include <fdict/libdsout.h>
##include "../hashfun.h"
#include <fdict/linehash.h>

hashtable* create_hashtable(unsigned int hashtable_size)
{
	hashtable* ht;
	ht_node** pphn;
	unsigned int i;
  
	ht = (hashtable*)MALLOC(sizeof(hashtable));
	if (ht == NULL)
	{
		MEMOUT;
		MEMOUT_EXIT(NULL);
	}
	ht->hashtable = (ht_node**)MALLOC(sizeof(ht_node) * hashtable_size);
	ht->size = 0;
	ht->htsize = hashtable_size;
	ht->maxvalue = 0;
	pphn = ht->hashtable;
	for(i = 0; i < hashtable_size; i++)
		pphn[i] = NULL;
	return ht;
}


void destroy_hashtable(hashtable* ht)
{
	int i;
	int ht_size;
	ht_node** pphn;
	ht_node* hn;
	ht_node* freehn;
	if (ht == NULL)
		return;
  
	ht_size = ht->htsize;
	pphn = ht->hashtable;
	for (i = 0; i < ht_size; i++)
	{
		hn = pphn[i];
		if (hn == NULL)
			continue;
		while (hn != NULL)
		{
			freehn = hn;
			hn = hn->next;
			FREE(freehn);
		}
		pphn[i] = NULL;
	}
	ht->size = 0;
	ht->htsize = 0;
	ht->maxvalue = 0;
	FREE(ht->hashtable);
	FREE(ht);
}

void ht_clear(hashtable* ht)
{
	int i;
	int ht_size;
	ht_node** pphn;
	ht_node* hn;
	ht_node* freehn;
	if (ht == NULL)
		return;
  
	ht_size = ht->htsize;
	pphn = ht->hashtable;
	for (i = 0; i < ht_size; i++)
	{
		hn = pphn[i];
		if (hn == NULL)
			continue;
		while (hn != NULL)
		{
			freehn = hn;
			hn = hn->next;
			FREE(freehn);
		}
		pphn[i] = NULL;
	}
	ht->size = 0;
	ht->maxvalue = 0;
	FREE(ht->hashtable);
}

static ht_node* ht_node_create(int hashcode, const char* key, int value)
{
	ht_node* node;

	node = (ht_node*)MALLOC(sizeof(ht_node) + (sizeof(char)*(strlen(key)+1)));
	if (node == NULL)
	{
		MEMOUT;
		MEMOUT_EXIT(NULL);
	}
	node->hashcode = hashcode;
	strcpy(node->key, key);
	node->value = value;
	node->next = NULL;
	node->list_next = NULL;
	return node;
}

void ht_set(hashtable* ht, const char* key, unsigned int value)
{
	unsigned int hashcode;
	int keyindex;
	ht_node* node;
	ht_node* insert;
	
	hashcode = HASH(key, strlen(key));
	keyindex = hashcode % ht->htsize;
	insert = ht->hashtable[keyindex];
	if (insert == NULL)
	{
		node = ht_node_create(hashcode, key, value);
		if (node == NULL)
			return;
		ht->hashtable[keyindex] = node;
		ht->size++;
	}
	else
	{
		while ((insert->next != NULL) &&
		       !(insert->hashcode == hashcode && strcmp(insert->key, key) == 0))
			insert = insert->next;
		if (insert->hashcode == hashcode && strcmp(insert->key, key) == 0)
		{
			insert->value = value;
		}
		else
		{
			node = ht_node_create(hashcode, key, value);
			if (node == NULL)
				return;
			insert->next = node; 
			ht->size++;
		}
	}
	if (value > ht->maxvalue)
		ht->maxvalue = value;
}

unsigned int ht_get(hashtable* ht, const char* key)
{
	unsigned int hashcode;
	int keyindex;
	ht_node* node;
  
	hashcode = HASH(key, strlen(key));
	keyindex = hashcode % ht->htsize;
	node = ht->hashtable[keyindex];
	if (node == NULL)
		return 0;
	while(node != NULL)
	{
		if ((node->hashcode == hashcode) && 
		    (strcmp(key, node->key) == 0))
			return node->value;
		node = node->next;
	}
	return 0;
}

int ht_key_exist(hashtable* ht, const char* key)
{
	unsigned int hashcode;
	int keyindex;
	ht_node* node;
  
	hashcode = HASH(key, strlen(key));
	keyindex = hashcode % ht->htsize;
	node = ht->hashtable[keyindex];
	if (node == NULL)
		return 0;
	while(node != NULL)
	{
		if ((node->hashcode == hashcode) && 
		    (strcmp(key, node->key) == 0))
			return 1;
	}
	return 0;
}

int ht_size(hashtable* ht)
{
	return ht->size;
}

ht_node* ht_get_node_list(hashtable* ht)
{
	ht_node* head = NULL;
	ht_node* cur_node = NULL;
	ht_node* node;
	int i;
	int ht_size;
  
	ht_size = ht->htsize;
	for (i = 0; i < ht_size; i++)
	{
		node = ht->hashtable[i];
		if (node == NULL)
			continue;
      
		if (head == NULL)
			head = cur_node = node;
		else
			cur_node->list_next = node;
      
		while (node->next != NULL)
		{
			node->list_next = node->next;
			node = node->next;
		}
		cur_node = node;
	}
	return head;
}

ht_node* ht_sort(hashtable* ht, char order)
{
	struct sort_slot_node
	{
		ht_node* head;
		ht_node* end;
	};
	struct sort_slot_node* sorttable;
	ht_node* listhead = NULL;
	ht_node* head;
	ht_node* end;
	ht_node* node;
	ht_node* sort_node;
	unsigned int index;
	int i;
	int ht_size;
	int msize;
  
	msize = sizeof(struct sort_slot_node) * (ht->maxvalue + 1);
	sorttable = (struct sort_slot_node*)MALLOC(msize);
	if (sorttable == NULL)
		return NULL;
	memset(sorttable, 0, msize);
  
	ht_size = ht->htsize;
	/* iterative all slot from hashtable */
	for (i = 0; i < ht_size; i++)
	{
		node = ht->hashtable[i];

		/* iterative all node from hashtable slot */
		while (node != NULL)
		{
			/* empty sort table slot
			 * insert to sort table slot 
			 */
			if (sorttable[(unsigned int)node->value].head == NULL)
			{
				node->list_next = NULL;
				sorttable[(unsigned int)node->value].head = 
					sorttable[(unsigned int)node->value].end = node;
			}
			/* insert node to list head  */
			else
			{
				sort_node = sorttable[(unsigned int)node->value].head;
				node->list_next = sort_node;
				sorttable[(unsigned int)node->value].head = node;
              
				/* to list end , insert to end
				   sort_node = sorttable[node->value].head;
				   while (sort_node->list_next != NULL)
				   sort_node = sort_node->list_next;
              
				   sort_node->list_next = node;
				   sorttable[node->value].end = node;
				*/
			}
			node = node->next;
		}
	}
  
	/* make sorted list */
	if (order == '>')
	{
		index = ht->maxvalue;
		end = NULL;
		while (index > 0)
		{
			head = sorttable[index].head;
			if (head == NULL)
			{
				index--;
				continue;
			}
          
			if (listhead == NULL)
				listhead = head;
			if (end != NULL)
				end->list_next = head;
			end = sorttable[index].end;
			index--;
		}
	}
	else if (order == '<')
	{
		index = 0;
		end = NULL;
		while (index <= ht->maxvalue)
		{
			head = sorttable[index].head;
			if (head == NULL)
			{
				index++;
				continue;
			}
          
			if (listhead == NULL)
				listhead = head;
			if (end != NULL)
				end->list_next = head;
			end = sorttable[index].end;
			index++;
		}
	}
  
	free(sorttable);
	return listhead;
}

int ht_out_text(hashtable* ht, const char* out_file_name)
{
	DSOUT_DEFINE;
	ht_node* node;
	int i;
	int ht_size;
  
	if (ht == NULL)
		return 0;
  
	DSOUT_OPEN_OUTFILE(out_file_name);
  
	sprintf(DSOUT_BUFF, "HashTable:\n");
	sprintf(DSOUT_CBUFF, "  HashTable Slot Size: %d\n", ht->htsize);
	sprintf(DSOUT_CBUFF, "  HashTable Node Size: %d\n", ht->size);
	sprintf(DSOUT_CBUFF, "  HashTable Max Value: %d\n", ht->maxvalue);
	DSOUT_INFO(buff);

	ht_size = ht->htsize;
	for (i = 0; i < ht_size; i++)
	{
		node = ht->hashtable[i];
		if (node == NULL)
			continue;

		sprintf(DSOUT_BUFF, "\t[%d]", i);
		DSOUT_INFO(buff);

		while (node != NULL)
		{
			sprintf(DSOUT_BUFF, " -> (k:%s => v:%d)", node->key, node->value);
			DSOUT_INFO(buff);
			node = node->next;
		}
		sprintf(DSOUT_BUFF, "\n");
		DSOUT_INFO(buff);
	}
  
	DSOUT_CLOSE_OUTFILE;
  
	return 1;
}

static char* get_k_v(const char* buff, char* k, char* v)
{
	char* sp;
	char* ep;
	int c;
  
	/* find k */
	sp = strstr(buff, "(k:");
	if (sp == NULL)
		return NULL;
	sp += 3;
	/* move k */
	ep = strstr(sp, " => ");
	c = ep - sp;
	memcpy(k, sp, c);
	k[c] = '\0';
	/* find v */
	sp = strstr(ep, "v:");
	sp += 2;
	/* move v */
	ep = strstr(sp, ")");
	c = ep - sp;
	memcpy(v, sp, c);
	v[c] = '\0';
	return ep;
}


hashtable* ht_resume_text(const char* in_file_name)
{
	hashtable* ht = NULL;
	FILE* fd;
	char buff[1024000];
	char kbuf[1024];
	char vbuf[1024];
	char* p = NULL;
	char* fp = NULL;
	int slot;
	int v;
  
	if (in_file_name == NULL)
		return NULL;
  
	fd = fopen(in_file_name, "r");
	if (fd == NULL)
		return NULL;

	p = fgets(buff, 1024000, fd);
	if (p == NULL)
		goto ret;
  
	if (strstr(p, "HashTable:") == NULL)
		goto ret;
  
	p = fgets(buff, 1024000, fd);
	fp = strstr(p, "HashTable Slot Size: ");
	if (fp == NULL)
		goto ret;
  
	/* get slot size */
	fp = p = fp + strlen("HashTable Slot Size: ");
	while (*p!='\0' && *p!='\n')
		p++;
	*p = '\0';
	slot = atoi(fp);

	p = fgets(buff, 1024000, fd);
	if (strstr(p, "HashTable Node Size: ") == NULL)
		goto ret;
	p = fgets(buff, 1024000, fd);
	if (strstr(p, "HashTable Max Value: ") == NULL)
		goto ret;
	ht = create_hashtable(slot);
	while( (p = fgets(buff,  1024000, fd)) != NULL)
	{
		fp = buff;
		while ( (fp = get_k_v(fp, kbuf, vbuf)) != NULL)
		{
			v = atoi(vbuf);
			ht_set(ht, kbuf, v);
		}
	}
ret:
	fclose(fd);
	return ht;
}


int ht_list_out_text(ht_node* listhead, const char* out_file_name)
{
	DSOUT_DEFINE;
	int count = 0;
  
	if (listhead == NULL)
		return 0;

	DSOUT_OPEN_OUTFILE(out_file_name);

	sprintf(DSOUT_BUFF, "HashTable List:\n");
	DSOUT_INFO(DSOUT_BUFF);

	while(listhead != NULL)
	{
		sprintf(DSOUT_BUFF, "\t[%d] -> (k:%s => v:%d)\n", count, listhead->key, listhead->value);
		DSOUT_INFO(DSOUT_BUFF);
		listhead = listhead->list_next;
		count++;
	}
  
	DSOUT_CLOSE_OUTFILE;
	return 1;
}
