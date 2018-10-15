#include "malloc.h"
#include <unistd.h>
#include <stdio.h>

int main() {

	char *a = memalloc(100);

	for(int i = 0; i < 26; i++)																										//filling it up with the alphabet
		a[i] = 'a'+i;

	printf("address of block a : %llu\n", (ULL) a - sizeof(struct block));
	printf("content of block a : %s\n", a);																				//printing the content of a

	a = remalloc((void*) a, 200);

	void *x = memalloc(100);

	printf("\nnew address of block a : %llu\n", (ULL) a - sizeof(struct block));

	memfree((void*) a);																																		//freeing a

	printf("a content : %s\n", a);																								//checking the content was copied

	unsigned limits[2] = {40, 300};																								//size limits for block to find
	printf("\nfreed block : %llu\n", (ULL) fnd_blk_by_sz(&FREE_BLKS.tail, limits));	//a has been added in the free block list

	char *b = cmemalloc(100);																											//declare b
	printf("\naddress of block b : %llu\n", (ULL) b - sizeof(struct block));							//b took the freed block a
	printf("content of b : %s\n", b);																							//b has been cleared

	void *y = memalloc(1000);

	printf("\nprogram break : %llu\n", (ULL) sbrk(0));
	memfree(y);																																		//checks that last block is removed if freed
	printf("new break : %llu\n", (ULL) sbrk(0));
}
