/* -----------------------------------------------------------------------
   NanoCore Libraries Collection (PSP/PSPE)
   -----------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   Headers file
   ------------------------------------------------------------------------
*/  

#ifndef _NANOC_H
        #define _NANOC_H

/*
   -----------------------------------------------------------------------------
   INDICAZIONE DELLA PIATTAFORMA
   -----------------------------------------------------------------------------
   Questa sezione è progettata per fare in modo che i programmi generino un 
   errore di compilazione, se non viene dichiarata la piattaforma di 
   compilazione
   -----------------------------------------------------------------------------
*/

#if ( (!defined PSPE_PLATFORM) && (!defined PSP_PLATFORM) )
        
      #error  No platform variabile defined. Use -D"PSP_PLATFORM" or -D"PSPE_PLATFORM" 
      // Questo genererà un errore di compilazione se non viene dichiarata la 
      // piattaforma di compilazione
              
#endif
        


/*
   --------------------------------------------------------------------------
   INIZIO DEGLI HEADERS
   --------------------------------------------------------------------------
*/


#ifdef __cplusplus
extern "C" {
#endif



#include <pspkerneltypes.h>          // Includerà pspkerneltype e psptypes

#include <errno.h>                   // Include headers per errno

#include <init.h>                    // Include headers delle routine di inizializzazione della libc

#include <malloc.h>                  // Include headers per emulatore alloc

#include <string.h>                  // Include headers per routines di manipolazione stringa

#include <stdio.h>                   // Include headers per routines di gestione disco e I/O

#include <stdlib.h>                  // Include headers per routines di servizio

#include <time.h>                    // Include headers per gestione orologio

#include <assert.h>                  // Include headers per assert

#include <nanoc_fsstringfunc.h>      // Include headers per fsstringfunc

#ifdef __cplusplus
}
#endif



#endif
