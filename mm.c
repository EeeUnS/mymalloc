/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.ggit
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "memlib.h"

#ifdef DEFAULT_TRACEFILES
#include "mm.h"
#include <unistd.h>
 /*********************************************************
  * NOTE TO STUDENTS: Before you do anything else, please
  * provide your team information in the following struct.
  ********************************************************/
team_t team = {
	/* Team name */
	"EUnS",
	/* First member's full name */
	"yunselee",
	/* First member's email address */
	"euns312510@gmail.com",
	/* Second member's full name (leave blank if none) */
	"",
	/* Second member's email address (leave blank if none) */
	""
};
#include <assert.h>
#define ASSERT(expr, ...) assert(expr);
#else
#include <intrin.h>
#define ASSERT(expr, ...) if(!(expr)) __debugbreak();
#endif 

 //#define WSIZE     4       /* Word size in bytes */
#define DSIZE     8       /* Double word size in bytes */
#define CHUNKSIZE (1<<12) /* Page size in bytes */
#define MINSIZE   (DSIZE)      /* Minimum block size */

#define DWORD unsigned long long

 /* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(DWORD *)(p))
#define PUT(p, val)  (*(DWORD *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~(MINSIZE - 1))
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - DSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - sizeof(DWORD))))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - sizeof(DWORD) * 2)))

/* single word (4) or double word (8) alignment */
#define ALIGNMENT (sizeof(DWORD) * 2)

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define SIZE_PTR(p)  ((size_t*)(((char*)(p)) - SIZE_T_SIZE))

#define MAX_SIZE 16

#define POOL_SIZE  1024
/*
address 64bit os
using memory pool
*/

// 2^{index} 2^12 = 4096
static void* free_list[MAX_SIZE];
static void* fool[MAX_SIZE];


static_assert(sizeof(void*) == DSIZE, "pointer size isn't 8");
static_assert(sizeof(DWORD) == DSIZE, "pointer size isn't 8");

static void set_header(void* base_ptr, size_t size, int is_alloc)
{
	ASSERT(size >= MINSIZE);
	ASSERT((size & (MINSIZE - 1)) == 0);

	PUT(base_ptr, PACK(size, is_alloc));
}


static DWORD* get_header_pointer(void* base_ptr)
{
	ASSERT((DWORD)((DWORD*)base_ptr - 1) == ((DWORD)base_ptr - DSIZE));

	return (DWORD*)HDRP(base_ptr);
}

DWORD get_alloced_size(void* base_ptr)
{
	return GET_SIZE(get_header_pointer(base_ptr));
}

DWORD is_allocated(void* base_ptr)
{
	return GET_ALLOC(get_header_pointer(base_ptr));
}

static void set_next_pointer(void* base_ptr, void* next_ptr)
{
	PUT((DWORD *)base_ptr, (DWORD)next_ptr);
}

static DWORD* get_next_pointer(void* base_ptr)
{
	return (DWORD*)GET(base_ptr);
}


/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */

 //사이즈를 넣으면 사이즈를 할당해주고 헤더넣고 반화하는 함수
static void* raw_alloc(size_t size)
{
	char* bp;
	if ((DWORD)(bp = (char *)mem_sbrk(size)) == -1)
	{
		ASSERT(1 != 1);
		return NULL;
	}

	set_header(bp, size, 0);
	bp += DSIZE;
	return bp;
}

static size_t get_index(size_t size)
{
	size_t index = 0;
	size_t block_size = 1;
	while (block_size <= size)
	{
		block_size = block_size << 1;
		index++;
	}
	--index;
	ASSERT(index < MAX_SIZE, "size over");
	return index;
}

void* mm_malloc(size_t size)
{
	size_t index = get_index(size);

	void* ret;
	if (free_list[index] == NULL)
	{
		// block size 2^index
		size_t block_size = 2 << index;

		size_t block_num;
		if (POOL_SIZE > block_size)
		{
			//ASSERT(POOL_SIZE > block_size);
			block_num = POOL_SIZE / block_size;
		}
		else
		{
			block_num = 1;
		}

		size_t chunk_size = DSIZE + block_size;
		size_t total_size = DSIZE + chunk_size * block_num;

		void* ptr = raw_alloc(total_size);

		DWORD *pptr = (DWORD*)HDRP(ptr);
		size_t t = GET(pptr);
		size_t ssize = GET_SIZE(pptr);

		printf("%lld is allocate and my alloc is %lld\n", get_alloced_size(ptr), total_size);
		printf("%p is start\n", ptr);

		//first alloc fool[index] is NULL
		set_next_pointer(ptr, fool[index]);
		fool[index] = ptr;

		// + pointer size + header size 
		ptr = (unsigned char*)ptr + DSIZE * 2;
		free_list[index] = ptr;

		for (int i = 0; i < block_num; ++i)
		{
			printf("%p is block\n", ptr);
			set_header(ptr, block_size, 0);
			void* next_ptr = (unsigned char*)ptr + chunk_size;
			set_next_pointer(ptr, next_ptr);
			printf("%p saved next block %p\n", ptr, get_next_pointer(ptr));
			ptr = next_ptr;
		}
		set_next_pointer(((unsigned char*)ptr - chunk_size), NULL);
		printf("%p saved next block %p\n", ptr, get_next_pointer(ptr));
	}

	ASSERT(free_list[index] != NULL);

	ret = free_list[index];
	DWORD* next_block = get_next_pointer(free_list[index]);
	free_list[index] = next_block;

	return ret;
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void* ptr)
{
	/*
	ptr header 까서 size
	size get_index
	index
	free_list index

	free_list[index] = ~?
	ptr free_list[index]
	free_list[index] = ptr;
	*/

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void* mm_realloc(void* ptr, size_t size)
{
	void* oldptr = ptr;
	void* newptr;
	size_t copySize;

	newptr = mm_malloc(size);
	if (newptr == NULL)
		return NULL;
	copySize = *(size_t*)((char*)oldptr - SIZE_T_SIZE);
	if (size < copySize)
		copySize = size;
	memcpy(newptr, oldptr, copySize);
	mm_free(oldptr);
	return newptr;
}
