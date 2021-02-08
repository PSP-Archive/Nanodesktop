
/* pngmem.c - stub functions for memory allocation
 *
 * Last changed in libpng 1.2.9 April 14, 2006
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 1998-2006 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This file provides a location for all memory allocation.  Users who
 * need special memory handling are expected to supply replacement
 * functions for png_malloc() and png_free(), and to use
 * png_create_read_struct_2() and png_create_write_struct_2() to
 * identify the replacement functions.
 */

#define PNG_INTERNAL
#include "png.h"

#if defined(PNG_READ_SUPPORTED) || defined(PNG_WRITE_SUPPORTED)

/* Allocate memory for a png_struct or a png_info.  The malloc and
   memset can be replaced by a single call to calloc() if this is thought
   to improve performance noticably. */
png_voidp /* PRIVATE */
png_create_struct(int type)
{
   png_size_t size;
   png_voidp struct_ptr;

   if (type == PNG_STRUCT_INFO)
      size = png_sizeof(png_info);
   else if (type == PNG_STRUCT_PNG)
      size = png_sizeof(png_struct);
   else
      return (NULL);

   struct_ptr = (png_voidp)malloc(size);

   if (struct_ptr != NULL)
      memset(struct_ptr, 0, size);

   return (struct_ptr);
}


/* Free memory allocated by a png_create_struct() call */
void /* PRIVATE */
png_destroy_struct(png_voidp struct_ptr)
{
   if (struct_ptr != NULL)
   {
      free(struct_ptr);
   }
}

/* Allocate memory.  For reasonable files, size should never exceed
   64K.  However, zlib may allocate more then 64K if you don't tell
   it not to.  See zconf.h and png.h for more information.  zlib does
   need to allocate exactly 64K, so whatever you call here must
   have the ability to do that. */

png_voidp PNGAPI
png_malloc(png_structp png_ptr, png_uint_32 size)
{
   png_voidp ret;

   if (png_ptr == NULL || size == 0)
      return (NULL);

 /* Check for overflow */
 if (size != (size_t)size)
   ret = NULL;
 else
   ret = malloc((size_t)size);

   return (ret);
}

/* Free a pointer allocated by png_malloc().  If ptr is NULL, return
   without taking any action. */
void PNGAPI
png_free(png_structp png_ptr, png_voidp ptr)
{
   if (png_ptr == NULL || ptr == NULL)
      return;

   free(ptr);
}

#if defined(PNG_1_0_X)
#  define png_malloc_warn png_malloc
#else
/* This function was added at libpng version 1.2.3.  The png_malloc_warn()
 * function will set up png_malloc() to issue a png_warning and return NULL
 * instead of issuing a png_error, if it fails to allocate the requested
 * memory.
 */
png_voidp PNGAPI
png_malloc_warn(png_structp png_ptr, png_uint_32 size)
{
   png_voidp ptr;
   png_uint_32 save_flags=png_ptr->flags;

   png_ptr->flags|=PNG_FLAG_MALLOC_NULL_MEM_OK;
   ptr = (png_voidp)png_malloc((png_structp)png_ptr, size);
   png_ptr->flags=save_flags;
   return(ptr);
}
#endif

png_voidp PNGAPI
png_memcpy_check (png_structp png_ptr, png_voidp s1, png_voidp s2,
   png_uint_32 length)
{
   /*
   int size;

   size = (int) length;
   if ((png_uint_32)size != length)
      png_error(png_ptr,"Overflow in png_memcpy_check.");
   */

   return memcpy (s1, s2, length);
}

png_voidp PNGAPI
png_memset_check (png_structp png_ptr, png_voidp s1, int value,
   png_uint_32 length)
{
   png_size_t size;

   size = (png_size_t)length;
   
   if ((png_uint_32)size != length)
      png_error(png_ptr,"Overflow in png_memset_check.");
   
   (memset (s1, value, size));
   
   return 0;
}



#endif /* PNG_READ_SUPPORTED || PNG_WRITE_SUPPORTED */
