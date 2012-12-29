#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <fdict/clib.h>
#include <fdict/libdsout.h>
#include <fdict/wordimage.h>
#include <fdict/trie.h>

/*
 * TODO:
 *
 *              [1]
 *          _____|______  
 *         /   /   |    \  
 *       [2] [3]  [4]  [10]
 *     ___|___   __|__  |                
 *     |     |   |   |   \   
 *    [5]   [6] (7) [9]  [8]           
 *                      
 * 
*/
int trie_out_text(struct trie_s* trie, const char* out_file_name)
{
  DSOUT_DEFINE;

  if (trie == NULL)
    return 0;
  
  DSOUT_OPEN_OUTFILE(out_file_name);
  buff[0] = '\0';
  DSOUT_CLOSE_OUTFILE;
  return 1;
}
