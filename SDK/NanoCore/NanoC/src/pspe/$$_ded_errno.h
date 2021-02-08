/*
   --------------------------------------------------------------------------
   $$_ded_errno.h               NanoC File Headers (PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Errno (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_ERRNO_H_
#define _DED_ERRNO_H_

        #ifndef __error_t_defined
                typedef int error_t;
                #define __error_t_defined 1
        #endif
                    
        #include <nanoc_sys/nanoc_errno.h>

#endif
