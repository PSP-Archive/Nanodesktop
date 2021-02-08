/*
   --------------------------------------------------------------------------
   nanoc_wctype.h               NanoC File Headers (PSP/PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   -----------------------------------------------------------------------
*/ 

#ifndef _NANOC_WCTYPE_H
        #define _NANOC_WCTYPE_H
        
        
        #ifdef PSPE_PLATFORM
                #include <nanoc_3rdparty/pspe/utf8/include/utf8/nd_wctype.h>        
        #endif
         
        #ifdef PSP_PLATFORM
                #include <nanoc_3rdparty/psp/utf8/include/utf8/nd_wctype.h>
        #endif
        
                
#endif   // _NANOC_WCTYPE_H
