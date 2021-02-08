/*
   --------------------------------------------------------------------------
   $$_ded_malloc.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di gestione della memoria (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_MALLOC_H_
        #define _DED_MALLOC_H_

        // Richiamo di altri files necessari per il funzionamento della 
        // libreria

        #include <pspkernel.h>
        #include <string.h>
        #include <stddef.h>
        #include <stdarg.h>
        #include <errno.h>

        #ifdef __cplusplus
        extern "C" 
        {
        #endif

        /*
           --------------------------------------------------------------------
           COSTANTI MANIFESTE DI COMPILAZIONE
           --------------------------------------------------------------------
        */
        
        #ifdef DED_MALLOC_I_NEED_VARIABLES
                #define MALLOC_EXTERN
        #else
                #define MALLOC_EXTERN extern
        #endif
        
        
        /*
           --------------------------------------------------------------------
           SYMBOLS
           --------------------------------------------------------------------
        */
        
        MALLOC_EXTERN unsigned int   MemoryAvail;                                  // La memoria libera in quel momento. 

        /*
           --------------------------------------------------------------------
           PROTOTYPES
           --------------------------------------------------------------------
        */
        
        void _pspsdk_alloc_init();
        void _pspsdk_alloc_deinit();

        /* stdlib/malloc */

        void *malloc  (size_t Size);
        void *realloc (void *Pointer, size_t Size);
        void *calloc  (size_t n, size_t size);
        void *memalign(size_t align, size_t size);
        int   free    (void *Pointer);
        int   memfree (void);       // Restituisce la memoria ancora libera

        #ifdef __cplusplus
        }
        #endif
        
#endif

