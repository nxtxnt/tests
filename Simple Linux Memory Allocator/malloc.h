#include <unistd.h>

#include "linked_list.h"

#define ULL unsigned long long
#define SBRK_SYSCALL_REQUEST_DENIED (void*) -1

void *memalloc(int size);																												//memory allocator, returns address
void *cmemalloc(int size);																											//memory allocator that clears the given block, returns address
void *remalloc(void *addr, unsigned size);																			//re-allocating given new size, returns address
void *memfree(struct block *addr);																								//frees the given block
void _memcp64(void *src, void *dst, unsigned size);															//64 bit block copy


struct BLOCKS {
	struct block tail;
	struct block *head;
};

struct BLOCKS TAKEN_BLKS = {0, 0, 0};
struct BLOCKS FREE_BLKS = {0, 0, 0};

void *memalloc(int size) {
	unsigned _size = size + sizeof(struct block);

	if(TAKEN_BLKS.head == (void*) 0)
			TAKEN_BLKS.head = &TAKEN_BLKS.tail;

	unsigned limits[2] = {_size, _size*2};
	struct block *blk = fnd_blk_by_sz(&FREE_BLKS.tail, limits);

	if(blk == (void*) -1) {
		if((blk = sbrk(_size)) == (void*) -1)
			return SBRK_SYSCALL_REQUEST_DENIED;
	}

	TAKEN_BLKS.head = add_node(blk, TAKEN_BLKS.head, _size);
	return (void*) ((ULL) TAKEN_BLKS.head + sizeof(struct block));
}

void *cmemalloc(int size) {
	ULL addr = (ULL) memalloc(size);
	char *a = (char*) addr;
	ULL end = size + (ULL) a;

	for(a; (ULL) a < end; a++)
		*a = (char) 0;

	return (void*) addr;
}


void *memfree(struct block *addr) {
	addr = (void*)(ULL) addr - sizeof(struct block);

	if(FREE_BLKS.head == (void*) 0)
		FREE_BLKS.head = &FREE_BLKS.tail;

	rem_node(addr);

	if(addr == TAKEN_BLKS.head) {
		if(sbrk(-(signed)(addr->size)) == (void*) -1)
			return SBRK_SYSCALL_REQUEST_DENIED;
	}

	else
		add_node(addr, FREE_BLKS.head, addr->size);
}

void *remalloc(void *addr, unsigned size) {
	memfree(addr);
	void *ret = memalloc(size);
	_memcp64(addr, ret, size + sizeof(struct block));

	return ret;
}

void _memcp64(void *src, void *dst, unsigned size) {
  if(size > 16) {
    ULL lim = (ULL) (size%8);
    ULL *_size = (void*) (ULL) size;

    ULL *src_QWORD = (ULL*) src;
    ULL *dst_QWORD = (ULL*) dst;

    int a = 0;
    while(_size-- > (ULL*) lim)
      *dst_QWORD++ = *src_QWORD++;

    char *src_BYTE = (char*) src_QWORD;
    char *dst_BYTE = (char*) dst_QWORD;

    while(lim--)
      *dst_BYTE++ = *src_BYTE++;
  }

  else {
    unsigned _size = size;
    char *src_BYTE = src;
    char *dst_BYTE = dst;

    while(_size-- != 0)
      *dst_BYTE++ = *src_BYTE++;
  }
}
