/*
   --------------------------------------------------------------------------
   $$_ded_malloc.h               NanoC File Headers (PSPE)
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
        
        #define MAXMEMORY 18000000
        #define MAXHANDLESALLOCABLE 65500

        MALLOC_EXTERN unsigned int     NrHandlesAllocated;                      // Il numero di aree di memoria allocate. Sono consentite al massimo 32
                                                                                // aree.
                                                                                         
        MALLOC_EXTERN unsigned char    MemoryArea [MAXMEMORY];                  // Questo array è l'area di lavoro dove si svolgono le operazioni. Tutte
                                                                                // le operazioni di allocazione e disallocazione si svolgono qui.

        struct StructMemoryHandle                                               // Ogni area di memoria allocata viene descritta da un descrittore (handle)
        {                                                                       // Ogni handle è un record contenente un campo start ed un campo end. 
           void           *Start;                                               // In pratica segnala al sistema quali aree di memoria sono impegnate.
           void           *End;                   
           int            Aligned;
        };                                                                      // Questo è solo il modello di un singolo descrittore

        MALLOC_EXTERN struct StructMemoryHandle  MemoryHandle[MAXHANDLESALLOCABLE];    // Questa definisce un array di nr. MaxHandlesAllocable descrittori
                                                                                       // ciascuno di tipo _MemoryHandle. I descrittori verranno inizializzati
                                                                                       // dalla routine MemoryInit all'avvio del programma.
   
        MALLOC_EXTERN unsigned int   MaxMemoryAllocable;                        // La massima memoria allocabile. E' pari a MaxMemory - 1.                                                  
        MALLOC_EXTERN unsigned int   MemoryAvail;                               // La memoria libera in quel momento. 

        #ifdef NCHL_MALLOC_I_NEED_VARIABLES
               char _PSPSDK_MemoryInitExecuted = 0;
        #else
               MALLOC_EXTERN char _PSPSDK_MemoryInitExecuted;
        #endif

        /*
           --------------------------------------------------------------------
           PROTOTYPES
           --------------------------------------------------------------------
        */
        
        void _pspsdk_alloc_init();
        void _pspsdk_alloc_deinit();

        /* stdlib/malloc */

        void *malloc  (size_t Size);
        void *realloc (void *Pointer, size_t NewDimension);
        void *calloc  (size_t n, size_t size);
        void *memalign(size_t align, size_t size);
        int   free    (void *Pointer);
        int   memfree (void);       // Restituisce la memoria ancora libera

        #ifdef __cplusplus
        }
        #endif
        
#endif
