/*
   --------------------------------------------------------------------------
   cxx.cpp                                      NanoCPP
   --------------------------------------------------------------------------
   Auxiliary symbols for compiler support
   ----------------------------------------------------------------------------
*/   

#include <malloc.h>

#include <stdio.h>    // for fprintf()
#include <stdlib.h>   // for abort()

#include <cstddef>
#include <new>


/*
 -------------------------------------------------------------------------
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * cxx.cpp - Simple C++ memory allocation operators.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: cxx.cpp 339 2005-06-27 02:24:25Z warren $
 * -----------------------------------------------------------------------
 */

__extension__ typedef int __guard __attribute__((mode (__DI__))); 


void __cxa_pure_virtual()
{
    printf ("Exception: Pure virtual method called \n");
    abort();
}

int __cxa_guard_acquire(__guard *g) 
{  
    return !*(char *)(g);
}; 

void __cxa_guard_release (__guard *g) 
{
     *(char *)g = 1;
}; 

void __cxa_guard_abort (__guard *g) 
{
    // Nothing to do //
}; 


/*
   ----------------------------------------------------------------------------
   NEW, DELETE OPERATORS
   ----------------------------------------------------------------------------
*/

void __throw_bad_alloc ()
{
    /* Nothing to do */ 
}


void* operator new (size_t numBytes) throw(std::bad_alloc)
{
	//C++ stardard 5.3.4.8 requires that a valid pointer be returned for
	//a call to new(0). Thus:
	
    if(numBytes == 0)
    {
		numBytes = 1;
	}
	
	void * p = malloc(numBytes);
	
    if(p == 0)
    {
	   __throw_bad_alloc ();
	}
	
	return p;
}


void* operator new(size_t numBytes, const std::nothrow_t& ) throw()
{
	return malloc(numBytes);
}

void* operator new[](size_t numBytes) throw(std::bad_alloc)
{
	//C++ stardard 5.3.4.8 requires that a valid pointer be returned for
	//a call to new(0). Thus:
        
	if(numBytes == 0)
    {
		numBytes = 1;
	}
	
	void * p = malloc(numBytes);
	
    if(p == 0)
    {
		__throw_bad_alloc ();
	}
	
	return p;
}

void* operator new[](size_t numBytes, const std::nothrow_t& ) throw()
{
	return malloc(numBytes);
}

void operator delete(void* ptr) throw()
{
	free(ptr);
}


void operator delete(void* ptr, const std::nothrow_t& ) throw() 
{
	free(ptr);
}


void operator delete[](void * ptr) throw()
{
	free(ptr);
}


void operator delete[](void* ptr, const std::nothrow_t& ) throw() 
{
	free(ptr);
}




