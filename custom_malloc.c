#include "stdio.h"
#include "custom_malloc.h"


void CustomMallocInit(HEAP_INFO_t *pHeapInfo)
{
    pHeapInfo->alloc_dll = &(pHeapInfo->dummy1);
    pHeapInfo->alloc_dll->next = NULL;
    pHeapInfo->alloc_dll->prev = NULL;
    pHeapInfo->avail_dll = &(pHeapInfo->dummy2);
    pHeapInfo->avail_dll->prev = NULL;
    pHeapInfo->avail_dll->next = pHeapInfo->pHeap;
    pHeapInfo->avail_dll->next->blkSize = pHeapInfo->heapSz - sizeof(BD_t);
    pHeapInfo->avail_dll->next->next = NULL;
    pHeapInfo->avail_dll->next->prev = pHeapInfo->avail_dll;
}

void *CustomMalloc(HEAP_INFO_t *pHeapInfo, size_t sz)
{
    BD_t *iter = pHeapInfo->avail_dll->next;

    while(iter != NULL)
    {
        int residueMemory = iter->blkSize-sz;
        if(residueMemory > 0)
        {
            if((unsigned int)residueMemory > sizeof(BD_t))
            {
                /*Update the BD moved to alloc_dll*/
                iter->blkSize = sz;
                
                BD_t *newFreeBlk = (void*)iter + sizeof(BD_t) + sz;
                newFreeBlk->blkSize = residueMemory-sizeof(BD_t);
                /*Remove iter and add the new BD to the avail_dll*/
                iter->prev->next = newFreeBlk;
                newFreeBlk->next = iter->next;
                newFreeBlk->prev = iter->prev;
                if(iter->next!=NULL)
                {
                    iter->next->prev = newFreeBlk;
                }
            }
            else
            {
                /*Remove the current BD from avail_dll*/
                iter->prev->next = iter->next;
                if(iter->next != NULL)
                    iter->next->prev = iter->prev;
            }
            /*Add iter to the alloc_dll*/

            /*Establish forward link*/
            iter->next = pHeapInfo->alloc_dll->next;
            pHeapInfo->alloc_dll->next = iter;

            /*Establish backward link*/
            if(iter->next != NULL)
                iter->next->prev = iter;
            iter->prev = pHeapInfo->alloc_dll;
            return (void*)iter + sizeof(BD_t);
        }
        iter = iter->next;
    }
    return NULL;
}

void CustomFree(HEAP_INFO_t *pHeapInfo, void *blkPtr)
{
    BD_t *bd = blkPtr-(sizeof(BD_t));
    /*Remove from alloc_dll*/
    bd->prev->next = bd->next;
    if(bd->next!=NULL)
        bd->next->prev = bd->prev;

    /*Add bd to avail_dll*/
    BD_t *iter = pHeapInfo->avail_dll;
    while(iter->next != NULL)
    {
        /*Check whether the preceding BD is found*/
        if(iter->next > bd)
            break;
        iter = iter->next;
    }

    /* Re-map the forward links*/
    bd->next = iter->next;
    iter->next = bd;
    /* Re-map the reverse links*/
    bd->prev = iter;
    if(bd->next != NULL)
        bd->next->prev = bd;

    /*Check whether the blocks can be merged*/

    /*Try merging current block and next block*/
    if( ((void*)bd + sizeof(BD_t) + bd->blkSize) == (bd->next))
    {
        bd->blkSize = bd->blkSize + sizeof(BD_t) + bd->next->blkSize;

        bd->next = bd->next->next;
        if(bd->next != NULL)
            bd->next->prev = bd;
    }   
    /* Try merging previous block and current block*/
    if( ((void*)iter + sizeof(BD_t) + iter->blkSize) == bd)
    {
        iter->blkSize = iter->blkSize + sizeof(BD_t) + bd->blkSize;
        iter->next = bd->next;
        if(iter->next != NULL)
            iter->next->prev = iter;
    }
}
