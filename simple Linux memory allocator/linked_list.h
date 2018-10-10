#define LINKED_LIST
#ifndef LINKED_LIST


#endif

#include <stdio.h>

#define NO_FITTED_BLOCK (void*) -1

struct block {
  struct block *prev;
  struct block *next;
  unsigned size;
};

void *add_node(struct block *blk_addr, struct block *head, unsigned size);
void *fnd_blk_by_sz(struct block *tail, unsigned size_lim[]);
void rem_node(struct block *addr);

void *add_node(struct block *blk_addr, struct block *head, unsigned size) {
  blk_addr->prev = head;
  head->next = blk_addr;
  head = blk_addr;
  head->size = size;
  head->next = 0;

  return head;
}

void rem_node(struct block *addr) {
  if(addr->next == 0)
    addr->prev->next = 0;

  else {
    addr->prev->next = addr->next;
    addr->next->prev = addr->prev;
  }
}

void *fnd_blk_by_sz(struct block *tail, unsigned size_lim[]) {
  struct block *read = tail;

  for(;;) {
    if(read->size >= size_lim[0] && read->size <= size_lim[1])
      return read;

    if(!read->next)
      break;

    read = read->next;
  }

  return NO_FITTED_BLOCK;
}
