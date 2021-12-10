/**
 * Implementation of a memory allocator based on the Buddy System.
 * See Knuth Art of Computer Programming, vol. 1, page 442. 
 * Each available block starts with a header that consists
 * of a tag (free/reserved), kval (size of block 2^kval), next
 * and previous pointers. Each reserved block has the tag and kval 
 * field only. All allocations are done in powers of two. All requests
 * are rounded up to the next power of two.
 * 
 * @author Jake Beslanowitch
 * @version Fall 2021
 */
 
#include "buddy.h"

int initialized = FALSE;

/* the header for an available block */
struct block_header {
	short tag;
	short kval;
	struct block_header *next;
	struct block_header *prev;
};
const int RESERVED = 0;
const int FREE = 1;
const int UNUSED = -1; /* useful for header nodes */
void merge(struct block_header *block);

/* supports memory upto 2^(MAX_KVAL-1) (or 64 GB) in size */
#define  MAX_KVAL  37

/* default memory allocation is 512MB */
const size_t DEFAULT_MAX_MEM_SIZE = 512*1024*1024;


/* A static structure stores the table of pointers to the lists in the buddy system.  */
struct pool {
	void *start; // pointer to the start of the memory pool
	int lgsize;  // log2 of size
	size_t size; // size of the pool, same as 2 ^ lgsize
	/* the table of pointers to the buddy system lists */
	struct block_header* avail[MAX_KVAL+1];
} pool;

/**
 * Initialize the buddy system to the given size 
 * (rounded up to the next power of two)
 *
 * @return  TRUE if successful, ENOMEM otherwise.
 */
int buddy_init(size_t size) {

	initialized = TRUE;

	if((Log2n(size)) > MAX_KVAL){
		return ENOMEM;
	}
	
	if(size==0) {
		pool.size = DEFAULT_MAX_MEM_SIZE;
	} else {
		pool.size = (1<<Log2n(size));
	}

	pool.lgsize=Log2n(pool.size);
    pool.start=sbrk(pool.size);
	if(pool.start==(void *)-1)
	{
		return ENOMEM;
	}

	((struct block_header *)pool.start)->tag = FREE;
	((struct block_header *)pool.start)->kval=MAX_KVAL;
	((struct block_header *)pool.start)->next=pool.start;
	((struct block_header *)pool.start)->prev=pool.start;
	pool.avail[pool.lgsize]=pool.start;

	return 1;
}

/**
 * Allocate dynamic memory. Rounds up the requested size to next power of two.
 * Returns a pointer that should be type casted as needed.
 * @param size  The amount of memory requested
 * @return Pointer to new block of memory of the specified size.
 */
void *buddy_malloc(size_t size) {

	if(size==0) {
		return NULL;
	}

	if(!pool.start) {
		 if(buddy_init(0) < 1) {
			return NULL;
		 }	
	 }

	size+=sizeof(struct block_header);
	if (size > pool.size) {
		return (void*)ENOMEM;
	}

	int kv;
	for(kv=1;size>(1<<kv);kv++) {
		;
	}
	
	int j = kv;
	struct block_header* block = kv;

	while(j<=pool.lgsize && !(block)) {
		block = pool.avail[j];
		j++;
	}
	
	if(!block) {
		return NULL;
	}

	while(j>kv && j<pool.lgsize) {
		if(block->next==block) {
			pool.avail[j]=NULL;
		} else {
			(block->prev)->next=block->next;
			(block->next)->prev=block->prev;
			pool.avail[j]=block->prev;
		}

		j--;

		struct block_header* buddy = (struct block_header*) block + (1<<j);
		buddy->tag = block->tag = FREE;
		buddy->kval=block->kval = j;
		buddy->next=buddy->prev = block;
		block->next = block->prev = buddy;
	
		if(pool.avail[j]) {
			(pool.avail[j]->prev)->next=block;
			block->prev=pool.avail[j];
			pool.avail[j]=block;

		} else {
			pool.avail[j]=block;
		}
	}
	if(block->next==block) {
		pool.avail[j]=NULL;
	} else {
		(block->prev)->next=block->next;
		(block->next)->prev=block->prev;
		pool.avail[j] = block->prev;
	}

	block->tag=RESERVED;
	block->prev=block->next=block;

	return(void*)((char*)block+sizeof(struct block_header));
}

/**
 * Allocate and clear memory to all zeroes. Wrapper function that just calles buddy_malloc.
 *
 * @param nmemb  The number of members needed
 * @param size   Size of each member
 * @return Pointer to start of the array of members
 */
void *buddy_calloc(size_t nmemb, size_t size) 
{
	return malloc(nmemb * size);
}

/**
 * Changes the size of the memory block pointed to by ptr to size bytes. 
 * The contents will be unchanged to the minimum of the old and new sizes; newly 
 * allocated memory will be uninitialized. If ptr is NULL, the call is equivalent 
 * to buddy_malloc(size); if size is equal to zero, the call is equivalent to buddy_free(ptr). 
 * Unless ptr is NULL, it must have been returned by an earlier call to buddy_malloc(), 
 * buddy_calloc() or buddy_realloc().
 *
 * @param  ptr Pointer to existing memory block
 * @param  size The new size of the memory block
 * @return The pointer to the resized block
 */
void *buddy_realloc(void *ptr, size_t size) 
{
	return NULL;
}

/**
 * Frees the memory space pointed to by ptr, which must have been returned 
 * by a previous call to buddy_malloc(), buddy_calloc() or buddy_realloc(). Otherwise, 
 * or if buddy_free(ptr) has already been called before, undefined behaviour occurs. If 
 * ptr is NULL, no operation is performed. 
 * @param ptr Pointer to memory block to be freed
 */
void buddy_free(void *ptr) 
{
	
	if(ptr==NULL)
	{
		return;
	}

	struct block_header *block = (struct block_header *)((char*) ptr-sizeof(struct block_header));
	
	merge(block);
	
	block->tag=FREE;
	if(pool.avail[block->kval])
	{
		(pool.avail[block->kval]->prev)->next=block;
		block->prev=pool.avail[block->kval]->prev;
		block->next=pool.avail[block->kval];
		pool.avail[block->kval]=block;
		pool.avail[block->kval]=block;
	} else {
		pool.avail[block->kval]=block;
	}
}

void merge(struct block_header *block) {
	if(block->kval<MAX_KVAL) {
		struct block_header* buddy =((struct block_header*) pool.start + (((struct block_header*) block - (struct block_header*) pool.start) ^ (1 << block->kval)));
		if((buddy->tag=FREE) && block->kval==buddy->kval) {
			if(buddy->next==buddy) {
				pool.avail[buddy->kval] = NULL;
			} else {
				(buddy->prev)->next=buddy->next;
				(buddy->next)->prev=buddy->prev;
				if(pool.avail[buddy->kval]==buddy) {
					pool.avail[buddy->kval]=buddy->prev;
				}
			}
			
			if(block>buddy) {
				block=buddy;
				block->prev=block->next=block;
			}

			block->kval++;
			merge(block);
		}
	} else {

	}
}

void printBuddyLists()
{
	struct block_header* b=pool.avail[pool.size];
	printf("%d",b->kval);
	while(b->next!=NULL)
	{
		printf("%d",b->kval);
		b=b->next;
	}

}
short Log2n(size_t n)
{
    return (n > 1) ? 1 + Log2n(n / 2) : 0;
}
