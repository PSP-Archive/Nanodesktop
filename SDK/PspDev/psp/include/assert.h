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
             
                        #ifdef __cplusplus
                        extern "C" {
                        #endif
                        
                        #include "_ansi.h"
                        
                        #undef assert
                        
                        #ifdef NDEBUG           /* required by ANSI standard */
                        #define assert(p)  	((void)0)
                        #else
                        
                        #ifdef __STDC__
                        #define assert(e)       ((e) ? (void)0 : __assert(__FILE__, __LINE__, #e))
                        #else   /* PCC */
                        #define assert(e)       ((e) ? (void)0 : __assert(__FILE__, __LINE__, "e"))
                        #endif
                        
                        #endif /* NDEBUG */
                        
                        void _EXFUN(__assert,(const char *, int, const char *));
                        
                        #ifdef __cplusplus
                        }
                        #endif

        #endif        // #if (defined PSP_PLATFORM || defined PSPE_PLATFORM)
        
#endif







