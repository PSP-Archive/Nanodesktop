/*
   --------------------------------------------------------------------------
   assert.h               SDK File Headers
   --------------------------------------------------------------------------
   This file is part of PSPSDK. If the user is compiling using Nanodesktop,
   this file will be replaced by compiler with nanoc_assert.h
   --------------------------------------------------------------------------
*/



#ifndef _ASSERT_H_

        #define	_ASSERT_H_

        #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)     
             #include <nanoc_assert.h>
        #else  
        
                        /*
                         * PSP Software Development Kit - http://www.pspdev.org
                         * -----------------------------------------------------------------------
                         * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
                         *
                         * assert.h
                         *
                         * Copyright (c) 2002-2004 PS2DEV
                         * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
                         * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
                         * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
                         *
                         * $Id: assert.h 1095 2005-09-27 21:02:16Z jim $
                         */
                        
                        #include <pspkernel.h>
                        #include <stdio.h>
                        
                        #ifdef NDEBUG
                                #define assert(cond)
                        #else
                                #ifdef __cplusplus
                                extern "C" {
                                #endif
                         
                                int __assert_fail (const char *assertion, const char *file, unsigned int line) __attribute__((noreturn));
                         
                                #ifdef __cplusplus
                                }
                                #endif
                         
                                #define assert(cond) (void)((cond)?0:__assert_fail(#cond, __FILE__, __LINE__))
                        #endif
                        
                        
        #endif   // #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)  

#endif  // _ASSERT_H








