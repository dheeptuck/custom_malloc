/** @file tests.c
 *  @brief Contains tests for functions CustomMallocInit, CustomMalloc
 *         & CustomFree. Each test is a function and the test description is 
 *         described in the function header.
 *
 *  @author Sudeep C
 */
#include "stdio.h"
#include "custom_malloc.h"
#include "stdbool.h"
#include "stdint.h"

typedef bool (*TEST_t)(void);

/** @brief Tests whether CustomMalloc places the first BD at the right address 
 *         in heap memory.
 *
 *  @return true if the test succeeds
 */
bool Test1()
{
    HEAP_INFO_t hp;
    uint8_t heap_arr[1000];

    hp.pHeap = (void*)heap_arr;
    hp.heapSz = sizeof(heap_arr);
    CustomMallocInit(&hp);

    int *ptr = (int*)CustomMalloc(&hp, sizeof(int));
    if(ptr == (void*)heap_arr+sizeof(BD_t))
        return true;
    return false;
}

/** @brief Tests whether CustomMalloc places the second BD at the right address 
 *         in heap memory.
 *
 *  @return true if the test succeeds
 */
bool Test2()
{
    HEAP_INFO_t hp;
    uint8_t heap_arr[1000];

    hp.pHeap = (void*)heap_arr;
    hp.heapSz = sizeof(heap_arr);
    CustomMallocInit(&hp);

    (void*)CustomMalloc(&hp, sizeof(int));
    int *ptr2 = (int*)CustomMalloc(&hp, sizeof(int));
    if(ptr2 == (void*)heap_arr+sizeof(BD_t)+sizeof(int)+sizeof(BD_t))
        return true;
    return false;
}

/** @brief Tests whether a freed block is reallocated.
 *
 *  @return true if the test succeeds
 */
bool Test3()
{
    HEAP_INFO_t hp;
    uint8_t heap_arr[1000];

    hp.pHeap = (void*)heap_arr;
    hp.heapSz = sizeof(heap_arr);
    CustomMallocInit(&hp);

    int *ptr1 = (int*)CustomMalloc(&hp, sizeof(int));
    CustomFree(&hp, ptr1);
    int *ptr2 = (int*)CustomMalloc(&hp, sizeof(int));
    if(ptr2 == (void*)heap_arr+sizeof(BD_t))
        return true;
    return false;
}


/** @brief Tests whether the right memory is returned after three consecutive
 *         CustomMalloc calls.
 *
 *  @return true if the test succeeds
 */
bool Test4()
{
    HEAP_INFO_t hp;
    uint8_t heap_arr[1000];

    hp.pHeap = (void*)heap_arr;
    hp.heapSz = sizeof(heap_arr);
    CustomMallocInit(&hp);

    (void)CustomMalloc(&hp, sizeof(int));
    (void)CustomMalloc(&hp, sizeof(int));
    int *ptr3 = (int*)CustomMalloc(&hp, sizeof(int));
    if(ptr3 != ((void*)heap_arr+3*sizeof(BD_t)+2*sizeof(int))  )
        return false;
    return true;
}

/** @brief Tests whether a freed block is re-allocated.
 *
 *  @return true if the test succeeds
 */
bool Test5()
{
    HEAP_INFO_t hp;
    uint8_t heap_arr[1000];

    hp.pHeap = (void*)heap_arr;
    hp.heapSz = sizeof(heap_arr);
    CustomMallocInit(&hp);

    int *ptr1 = (int*)CustomMalloc(&hp, sizeof(int));
    int *ptr2 = (int*)CustomMalloc(&hp, sizeof(int));
    (void)CustomMalloc(&hp, sizeof(int));
    CustomFree(&hp, ptr1);
    CustomFree(&hp, ptr2);
    int *ptr4 = (int*)CustomMalloc(&hp, 2*sizeof(int));
    if(ptr4 == (void*)heap_arr+sizeof(BD_t))
        return true;
    return false;
}

/** @brief Tests whether a coalescing of blocks works.
 *
 *  @return true if the test succeeds
 */
bool Test6()
{
    HEAP_INFO_t hp;
    uint8_t heap_arr[1000];

    hp.pHeap = (void*)heap_arr;
    hp.heapSz = sizeof(heap_arr);
    CustomMallocInit(&hp);

    (void)CustomMalloc(&hp, sizeof(int));
    int *ptr2 = (int*)CustomMalloc(&hp, sizeof(int));
    int *ptr3 = (int*)CustomMalloc(&hp, sizeof(int));
    int *ptr4 = (int*)CustomMalloc(&hp, sizeof(int));

    CustomFree(&hp, ptr2);
    CustomFree(&hp, ptr4);
    CustomFree(&hp, ptr3);

    int *ptr5 = (int*)CustomMalloc(&hp, 3*sizeof(int));
    
    if(ptr5 == ptr2)
        return true;
    return false;
}

int main()
{
    TEST_t aTests[] = {Test1, Test2, Test3, Test4, Test5, Test6};
    for(unsigned int i=0;i< (sizeof(aTests)/sizeof(TEST_t));i++)
    {
        if(aTests[i]())
        {
            printf("TEST %d PASS\n", i+1);
        }
        else
        {
            printf("TEST %d FAIL\n", i+1);  
        }
    }
}