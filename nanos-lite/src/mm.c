#include "memory.h"
#include"proc.h"
//extern PCB*current;
static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  if(current->max_brk==0){
    current->max_brk=brk;
  }
  if(brk+increment>current->max_brk){
    void *va=(void*)((current->max_brk/4096)<<12);
    int left=brk+increment-current->max_brk;
    //int res = (4096-(current->max_brk%4096))%4096;
    //left = left-res;
    while(left>0){
      _map(&current->as,(void*)va,new_page(1),1);
      va+=PGSIZE;
      left-=PGSIZE;
    }
    current->max_brk=brk+increment;
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
