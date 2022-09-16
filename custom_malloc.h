#ifndef _CUSTOM_MALLOC_H_
#define _CUSTOM_MALLOC_H_

#include "stdio.h"



struct BD_t;

typedef struct BD_t
{
    int blkSize;
    struct BD_t *prev;
    struct BD_t *next;
} BD_t;

typedef struct HEAP_INFO_t {
    void *pHeap;
    size_t heapSz;
    BD_t *alloc_dll;
    BD_t *avail_dll;
    /*Dummy variables added for easy traversal of DLL*/
    BD_t dummy1;
    BD_t dummy2;
} HEAP_INFO_t;

/*
 * Initializes the heap. It internally created a BD and places at start of the
 * heap. The BD is added to avail_dll.
 * @param pHeapInfo: Pointer to the current heap info.
 **/
void CustomMallocInit(HEAP_INFO_t *pHeapInfo);

/*
 * Mimics the malloc function(i.e provides the requested memory). Returns NULL in
 * case is it not possible to allocate the requested memory.
 * @param pHeapInfo: Pointer to the current heap info.
 * @param sz: The size of the requested memory.
 **/
void *CustomMalloc(HEAP_INFO_t *pHeapInfo, size_t sz);

/*
 * Mimics the free function(i.e frees an block that was already allocated)
 * @param pHeapInfo: Pointer to the current heap info.
 * @param blkPtr: Pointer to the block to be freed.
 **/
void CustomFree(HEAP_INFO_t *pHeapInfo, void *blkPtr);


#endif	