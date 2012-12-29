#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/linehash.h>

int main()
{
  hashtable* ht;
  ht_node* head;
  int a;
  int b;
  char* c;
  char* d;
  
  ht = create_hashtable(100);
  ht_set(ht, "a", 1);
  ht_set(ht, "b", 2);
  a = ht_get(ht, "a");
  b = ht_get(ht, "b");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  ht_set(ht, "a", 10);
  a = ht_get(ht, "a");
  b = ht_get(ht, "b");
  c = (char*)ht_get(ht, "c");
  d = (char*)ht_get(ht, "国家叫什么啊?");
  printf("a=%d\n", a);
  printf("b=%d\n", b);
  printf("c=%s\n", c);
  printf("国家叫什么啊?=%s\n", d);
  head = ht_get_node_list(ht);
  while(head != NULL)
    {
      printf("key=%s, value=%d\n", head->key, head->value);
      head = head->list_next;
    }
  ht_out_text(ht, NULL);
  ht_out_text(ht, "htdump");
  head = ht_sort(ht, '<');
  ht_list_out_text(head, NULL);
  destroy_hashtable(ht);
  return 0;
}
