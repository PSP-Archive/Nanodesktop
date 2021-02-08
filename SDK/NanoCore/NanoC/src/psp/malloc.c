/*
   --------------------------------------------------------------------------
   malloc.c                                      NanoC Files (PSP)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di gestione della memoria 
   ----------------------------------------------------------------------------
*/   

#define DED_MALLOC_I_NEED_VARIABLES
     #include <psp/$$_ded_malloc.h>
#undef  DED_MALLOC_I_NEED_VARIABLES

/* 
   ----------------------------------------------------------------------------
   Origin: PSPSDK                                       PSP Allocation Routines
   ----------------------------------------------------------------------------
 
   Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Standard C library heap allocation routines.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: alloc.c 540 2005-07-08 19:35:10Z warren $
 *
 * This code is based on code contributed by Philip Joaqiun (jenova0).  
   ----------------------------------------------------------------------------
*/

#ifdef DEBUG_ALLOC
#include <stdio.h>
#endif

unsigned int MemoryAvail;

/* Use this to set the default malloc() alignment. */
#define DEFAULT_ALIGNMENT	16

#ifndef ALIGN
#define ALIGN(x, align) (((x)+((align)-1))&~((align)-1))
#endif

#ifdef DEBUG_ALLOC
#define ALLOC_MAGIC 0xa110ca73
#endif



/* PSP-compatible sbrk(). */
/* TODO: Currently our default heap is set to the maximum available block size
   when sbrk() is first called.  Sony seems to always use a default of 64KB,
   with the expectation that user programs will override the default size with
   their own desired size.  The only reason I can think of them doing this is
   to allow each PRX to have their own seperate heap.  I think that their
   method is overkill for most user programs.

   What I'd like to do instead is to use the default of 64KB for PRXes as Sony
   does, but use the maximum available block size for executables.  This avoids
   the requirement of specifying the heap size manually in user programs.
   However, we currently don't have a clean way to distinguish PRXes and normal
   executables, so this code needs to be revisited once we do come up with a
   way. */
#define DEFAULT_PRX_HEAP_SIZE_KB 64

/* If defined it specifies the desired size of the heap, in KB. */
extern unsigned int sce_newlib_heap_kb_size __attribute__((weak));
extern int __pspsdk_is_prx __attribute__((weak));

/* UID of the memory block that represents the heap. */
static SceUID __psp_heap_blockid;

#define NDGET_HAL_PSP                               0x80000000
#define NDGET_OPERATINGSYSTEM           NDGET_HAL_PSP + 0x1000




static inline int FixedKernelMaxFreeMemSize (void)
{
    char string [256];
    
    ndINFO_GetValue (NDGET_OPERATINGSYSTEM, &string, 0, 0);
    // Determina il tipo di sistema operativo presente
    
    if (!strcmp (string, "Sony Playstation Portable CFW"))      // Siamo su custom firmware
    {
         return sceKernelMaxFreeMemSize() - 1*1024*1024;
         // Un 1 Mb di Heap viene riservato allo stack dei vari thread        
    }
    else           // Normale applicazione per firmware 1.5
    {
         return sceKernelMaxFreeMemSize();          
    }
}


static inline void * _sbrk(ptrdiff_t incr)
{
	static void * heap_bottom = NULL;
	static void * heap_top = NULL;
	static void * heap_ptr = NULL;

	/* Has our heap been initialized? */
	if (heap_bottom == NULL) 
    {
		/* No, initialize the heap. */
		SceSize heap_size;

        if (&sce_newlib_heap_kb_size != NULL) 
        {
			heap_size = sce_newlib_heap_kb_size * 1024;
		} 
        else 
        {
			if (&__pspsdk_is_prx != NULL) 
            {
				heap_size = DEFAULT_PRX_HEAP_SIZE_KB * 1024;
			} 
            else 
            {
				
                
                heap_size = FixedKernelMaxFreeMemSize();
			}
		}
		              
		__psp_heap_blockid = sceKernelAllocPartitionMemory(2, "block", PSP_SMEM_Low, heap_size, NULL);
		
        if (__psp_heap_blockid > 0) {
			heap_bottom = sceKernelGetBlockHeadAddr(__psp_heap_blockid);
			heap_ptr = heap_bottom;
			heap_top = (unsigned char *) heap_bottom + heap_size;
		}
		
		MemoryAvail = heap_size;
		// La memoria disponibile nel sistema
	}

	void * heap_addr = (void *) -1;
	void * next_heap_ptr = (void *) ((ptrdiff_t) heap_ptr + incr);
	if ((heap_bottom != NULL) && (next_heap_ptr >= heap_bottom) && (next_heap_ptr < heap_top)) {
		heap_addr = heap_ptr;
		heap_ptr = next_heap_ptr;
	}

	return heap_addr;
}

/* Free the heap. */
static inline int __psp_free_heap(void)
{
	if (__psp_heap_blockid > 0) {
		return sceKernelFreePartitionMemory(__psp_heap_blockid);
	}

	return __psp_heap_blockid;
}



/*static vs32 alloc_sema = -1;*/
void _pspsdk_alloc_init()
{
/*    ee_sema_t alloc_sema_struct;*/
/*    alloc_sema_struct.init_count = 1;*/
/*    alloc_sema_struct.max_count = 1;*/
/*    alloc_sema = CreateSema(&alloc_sema_struct);*/
}

void _pspsdk_alloc_deinit()
{
/*    if (alloc_sema >= 0) {*/
/*    DeleteSema(alloc_sema);*/
/*    }*/
}

static inline void _pspsdk_alloc_lock()
{
/*    if (alloc_sema >= 0) {*/
/*    WaitSema(alloc_sema);*/
/*    }*/
}

static inline void _pspsdk_alloc_unlock()
{
/*    if (alloc_sema >= 0) {*/
/*    SignalSema(alloc_sema);*/
/*    }*/
}


/* _heap_mem_block_handle structure. */
typedef struct _heap_mem_handle {
#ifdef DEBUG_ALLOC
	u32     magic;
#endif
	void *	ptr;
	size_t	size;
	struct _heap_mem_handle * prev;
	struct _heap_mem_handle * next;
} heap_mem_handle_t;


void * __alloc_heap_base = NULL;
heap_mem_handle_t *__alloc_heap_head = NULL;
heap_mem_handle_t *__alloc_heap_tail = NULL;

/* Find a the lowest block that we can allocate AFTER, returning NULL if there
   are none.  */
static inline heap_mem_handle_t * _heap_mem_fit(heap_mem_handle_t *head, size_t size)
{
	heap_mem_handle_t *prev_mem = head;
	register u32 prev_top, next_bot;

	while (prev_mem != NULL) {
		if (prev_mem->next != NULL) {
			prev_top = (u32)prev_mem->ptr + prev_mem->size;
			next_bot = (u32)prev_mem->next - prev_top;
			if (next_bot >= size)
				return prev_mem;
		}

		prev_mem = prev_mem->next;
	}

	return prev_mem;
}


void *malloc(size_t size)
{
	register void *ptr = NULL, *mem_ptr;
	heap_mem_handle_t *new_mem, *prev_mem;
	register size_t mem_sz, heap_align_bytes;

    mem_sz = size + sizeof(heap_mem_handle_t) + 4;
    // L'offset di 4 viene aggiunto per impedire memory leak quando viene utilizzato
    // malloc in coppia con memset

	if ((mem_sz & (DEFAULT_ALIGNMENT - 1)) != 0)
		mem_sz = ALIGN(mem_sz, DEFAULT_ALIGNMENT);
	
	_pspsdk_alloc_lock();

	/* If we don't have any allocated blocks, reserve the first block from
	   the OS and initialize __alloc_heap_tail.  */
	if (__alloc_heap_head == NULL) 
    {
		/* Align the bottom of the heap to our default alignment.  */
		if (__alloc_heap_base == NULL) 
        {
			heap_align_bytes = (u32) _sbrk(0) & (DEFAULT_ALIGNMENT - 1);
			_sbrk(heap_align_bytes);
			__alloc_heap_base = _sbrk(0);
		}

		/* Allocate the physical heap and setup the head block.  */
		if ((mem_ptr = _sbrk(mem_sz)) == (void *)-1)
			return ptr;	/* NULL */
		
		ptr = (void *)((u32)mem_ptr + sizeof(heap_mem_handle_t));

		__alloc_heap_head       = (heap_mem_handle_t *)mem_ptr;
#ifdef DEBUG_ALLOC
		__alloc_heap_head->magic = ALLOC_MAGIC;
#endif
		__alloc_heap_head->ptr  = ptr;
		__alloc_heap_head->size = mem_sz - sizeof(heap_mem_handle_t);
		__alloc_heap_head->prev = NULL;
		__alloc_heap_head->next = NULL;

		__alloc_heap_tail = __alloc_heap_head;
		
		_pspsdk_alloc_unlock();
		
		MemoryAvail = MemoryAvail - __alloc_heap_head->size;
		return ptr;
	}

	/* Check to see if there's free space at the bottom of the heap.  */
	if ((__alloc_heap_base + mem_sz) < (void *)__alloc_heap_head) {
		new_mem = (heap_mem_handle_t *)__alloc_heap_base;
		ptr     = (void *)((u32)new_mem + sizeof(heap_mem_handle_t));

#ifdef DEBUG_ALLOC
		new_mem->magic = ALLOC_MAGIC;
#endif
		new_mem->ptr  = ptr;
		new_mem->size = mem_sz - sizeof(heap_mem_handle_t);
		new_mem->prev = NULL;
		new_mem->next = __alloc_heap_head;
		new_mem->next->prev = new_mem;
		__alloc_heap_head = new_mem;
		
		_pspsdk_alloc_unlock();
		
        MemoryAvail = MemoryAvail - new_mem->size;
		return ptr;
	}

	/* See if we can allocate the block without extending the heap. */
	prev_mem = _heap_mem_fit(__alloc_heap_head, mem_sz);
	if (prev_mem != NULL) {
		new_mem = (heap_mem_handle_t *)((u32)prev_mem->ptr + prev_mem->size);
		ptr     = (void *)((u32)new_mem + sizeof(heap_mem_handle_t));

#ifdef DEBUG_ALLOC
		new_mem->magic = ALLOC_MAGIC;
#endif
		new_mem->ptr  = ptr;
		new_mem->size = mem_sz - sizeof(heap_mem_handle_t);
		new_mem->prev = prev_mem;
		new_mem->next = prev_mem->next;
		new_mem->next->prev = new_mem;
		prev_mem->next = new_mem;

		_pspsdk_alloc_unlock();
		
		MemoryAvail = MemoryAvail - new_mem->size;
		return ptr;
	}

	/* Extend the heap, but make certain the block is inserted in
	   order. */
	if ((mem_ptr = _sbrk(mem_sz)) == (void *)-1) {
		_pspsdk_alloc_unlock();
		return ptr;	/* NULL */
	}

	ptr = (void *)((u32)mem_ptr + sizeof(heap_mem_handle_t));

	new_mem       = (heap_mem_handle_t *)mem_ptr;
#ifdef DEBUG_ALLOC
	new_mem->magic = ALLOC_MAGIC;
#endif
	new_mem->ptr  = ptr;
	new_mem->size = mem_sz - sizeof(heap_mem_handle_t);
	new_mem->prev = __alloc_heap_tail;
	new_mem->next = NULL;

	__alloc_heap_tail->next = new_mem;
	__alloc_heap_tail       = new_mem;
	
	_pspsdk_alloc_unlock();
	
	MemoryAvail = MemoryAvail - new_mem->size;
	return ptr;
}



void *realloc(void *ptr, size_t size)
{
	heap_mem_handle_t *prev_mem;
	register void *new_ptr = NULL;

	if (!size && ptr != NULL) {
		free(ptr);
		return new_ptr;
	}

	if (ptr == NULL)
		return malloc(size);

	if ((size & (DEFAULT_ALIGNMENT - 1)) != 0)
		size = ALIGN(size, DEFAULT_ALIGNMENT);

	_pspsdk_alloc_lock();
	prev_mem = (heap_mem_handle_t *)((u32)ptr - sizeof(heap_mem_handle_t));

#ifdef DEBUG_ALLOC
	if (prev_mem->magic != ALLOC_MAGIC) {
		fprintf(stderr, "realloc: Pointer at %p was not malloc()ed before, or got overwritten.\n", ptr);

		_pspsdk_alloc_unlock();
		return NULL;
	}
#endif

	/* Don't do anything if asked for same sized block. */
	/* If the new size is shorter, let's just shorten the block. */
	if (prev_mem->size >= size) {
		/* However, if this is the last block, we have to shrink the heap. */
		if (!prev_mem->next)
			_sbrk(ptr + size - _sbrk(0));
		prev_mem->size = size;
		
		_pspsdk_alloc_unlock();
		return ptr;
	}
	
	
	/* We are asked for a larger block of memory. */
	
	/* Are we the last memory block ? */
	if (!prev_mem->next) {
		/* Yes, let's just extend the heap then. */
		if (_sbrk(size - prev_mem->size) == (void*) -1)
			return NULL;
		prev_mem->size = size;
		
		_pspsdk_alloc_unlock();
		return ptr;
	}
	
	/* Is the next block far enough so we can extend the current block ? */
	if ((prev_mem->next->ptr - ptr) > size) {
		prev_mem->size = size;
		
		_pspsdk_alloc_unlock();
		return ptr;
	}
	
	_pspsdk_alloc_unlock();
	
	/* We got out of luck, let's allocate a new block of memory. */
	if ((new_ptr = malloc(size)) == NULL)
		return new_ptr;

        /* New block is larger, we only copy the old data. */
	memcpy(new_ptr, ptr, prev_mem->size);

	free(ptr);
	return new_ptr;
}



__attribute__((weak)) void *memalign(size_t align, size_t size)
{
	heap_mem_handle_t new_mem;
	heap_mem_handle_t *cur_mem;
	heap_mem_handle_t *old_mem;
	void *ptr = NULL;
	
	if (align <= DEFAULT_ALIGNMENT)
		return malloc(size);

	/* Allocate with extra alignment bytes just in case it isn't aligned
	   properly by malloc.  */
	if ((ptr = malloc(size + align)) == NULL)
		return ptr;	/* NULL */

	/* If malloc returned it aligned for us we're fine.  */
	if (((u32)ptr & (align - 1)) == 0)
		return ptr;

	_pspsdk_alloc_lock();
	cur_mem = (heap_mem_handle_t *)((u32)ptr - sizeof(heap_mem_handle_t));
	
	
	// PATCH: Attenzione a quello che accade qui. L'esecuzione di malloc(size+align)
	// ha generato un handle di memoria cur_mem->size dove la size sarà data da
	// size+align. Inoltre, malloc ha diminuito la memoria a disposizione di una
    // quantità pari a size+align. 
    
    // Se noi disallocassimo questo handle adesso, prima della istruzione 
    // successiva, il contatore della memoria a disposizione MemAvail verrebbe 
    // aumentato della quantità size+align e non ci sarebbero problemi. 
    
    // Il problema è che il codice ora esegue questa istruzione:
    
    cur_mem->size -= align;   
    
    // che diminuisce di una quantità pari ad align il campo size dell'handle.
    // Se adesso eseguiamo un free su questo blocco di memoria, MemoryAvail
    // non verrà incrementato della size originaria, ma della size originaria
    // - align. Questo fa apparire un memory leak fittizio esattamente pari ad
    // align. Questo problema viene patchato, aumentando di una quantità pari
    // ad align la variabile MemAvail per la ram a disposizione  

    MemoryAvail += align;       // ECCO LA PATCH

	/* Otherwise, align the pointer and fixup our hearder accordingly.  */
	ptr = (void *)ALIGN((u32)ptr, align);
	
	old_mem = cur_mem;

	/* Copy the heap_mem_handle_t locally, before repositioning (to make
	   sure we don't overwrite ourselves.  */
	memcpy(&new_mem, cur_mem, sizeof (heap_mem_handle_t));
	cur_mem = (heap_mem_handle_t *)((u32)ptr - sizeof(heap_mem_handle_t));
	memcpy(cur_mem, &new_mem, sizeof (heap_mem_handle_t));

	if (cur_mem->prev)
		cur_mem->prev->next = cur_mem;
	if (cur_mem->next)
		cur_mem->next->prev = cur_mem;
	
	if (__alloc_heap_head == old_mem)
		__alloc_heap_head = cur_mem;
	
	if (__alloc_heap_tail == old_mem)
		__alloc_heap_tail = cur_mem;

	cur_mem->ptr = ptr;
	
	_pspsdk_alloc_unlock();
	return ptr;
}


void *calloc(size_t n, size_t size)
{
	register void *ptr = NULL;
	register size_t sz = n * size;

    ptr = malloc (sz);
    if (ptr==NULL) return 0;
   
	memset(ptr, 0, sz);
	return ptr;
}




int free (void *ptr)
{
	heap_mem_handle_t *cur;
	register void *heap_top;
	register size_t size;

	if (!ptr) return -1;

	_pspsdk_alloc_lock();
	
	if (!__alloc_heap_head) 
    {
		_pspsdk_alloc_unlock();
		return 0;
	}

#ifdef DEBUG_ALLOC
	cur = (heap_mem_handle_t *)((u32)ptr - sizeof(heap_mem_handle_t));
	
    if (cur->magic != ALLOC_MAGIC) 
    {
		fprintf(stderr, "free: Pointer at %p was not malloc()ed before, or got overwritten.\n", ptr);
		
		_pspsdk_alloc_unlock();
		return 0;
	}
#endif

	/* Freeing the head pointer is a special case.  */
	if (ptr == __alloc_heap_head->ptr) 
    {
		size = __alloc_heap_head->size +
			(size_t)(__alloc_heap_head->ptr - (void *)__alloc_heap_head);

		__alloc_heap_head = __alloc_heap_head->next;

        MemoryAvail = MemoryAvail + __alloc_heap_head->size;

		if (__alloc_heap_head != NULL) 
        {
			__alloc_heap_head->prev = NULL;
		} 
        else 
        {
			__alloc_heap_tail = NULL;
			_sbrk(-size);
		}
		
		_pspsdk_alloc_unlock();
		return 0;
	}

	cur = __alloc_heap_head;
	while (ptr != cur->ptr)  
    {
		/* ptr isn't in our list */
		if (cur->next == NULL) 
        {
			_pspsdk_alloc_unlock();
			return 0;
	    }

		cur = cur->next;
	}
	
	// Se l'elaborazione è giunta fin qui, vuol dire che è stato trovato un
	// puntatore all'area da cancellare. In cur->size c'è il nr di byte che
	// saranno recuperati.
    MemoryAvail = MemoryAvail + cur->size;
	
	/* Deallocate the block.  */
	if (cur->next != NULL) 
    {
		cur->next->prev = cur->prev;
	} 
    else 
    {
		/* If this block was the last one in the list, shrink the heap.  */
		__alloc_heap_tail = cur->prev;

		/* We need to free (heap top) - (prev->ptr + prev->size), or else
		   we'll end up with an unallocatable block of heap.  */
		heap_top = _sbrk(0);
		size = (u32)heap_top - (u32)(cur->prev->ptr + cur->prev->size);
		
		_sbrk(-size);
	}

	cur->prev->next = cur->next;
	
	_pspsdk_alloc_unlock();
	return 0;
}


int memfree ()
/*
    Restituisce la memoria disponibile nel sistema
*/

{
    return MemoryAvail;
}

