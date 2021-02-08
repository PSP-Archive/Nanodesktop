/*
   --------------------------------------------------------------------------
   errno.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Errno
   ----------------------------------------------------------------------------
*/   



#define _ONLY_ERRNO_ERRORS  
        #include <errno.h>    // Permette di caricare le definizioni di errno senza
                              // la riga che definisce la variabile come extern
#undef  _ONLY_ERRNO_ERRORS

int errno = 0;

int *__errno = 0;


