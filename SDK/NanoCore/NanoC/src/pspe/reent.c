/*
   --------------------------------------------------------------------------
   cxx.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di supporto al C++
   ----------------------------------------------------------------------------
*/   


#include <reent.h>


/*
    ----------------------------------------------------------------------
    DEFINIZIONE DI SIMBOLI PER COMPATIBILITA' LINKER CON IL C++
    ----------------------------------------------------------------------
*/

    struct _reent *_impure_ptr __ATTRIBUTE_IMPURE_PTR__;



