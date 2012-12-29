#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fdict/wordbase.h>
#include <fdict/libfdict.h>
#include <fdict/fdict.h>

int main(int argv, char** argc) {
  printf("uint8:%ld uint16:%ld uint32:%ld uint64:%ld double:%ld\n",
	 sizeof(uint8)*8,
	 sizeof(uint16)*8,
	 sizeof(uint32)*8,
	 sizeof(uint64)*8,
	 sizeof(double)*8);

  printf("struct field_s:%ldbyte %ldbit\n",
	 sizeof(struct field_s), sizeof(struct field_s)*8);

  printf("struct record_s:%ldbyte %ldbit\n",
	 sizeof(struct record_s), sizeof(struct record_s)*8);

  return 0;
}

