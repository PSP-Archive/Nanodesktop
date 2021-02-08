/*
   --------------------------------------------------------------------------
   nanoc_stdio.h               NanoC File Headers (PSP/PSPE)
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


#ifndef _NANOC_STDIO_H
         #define _NANOC_STDIO_H

         #ifdef PSPE_PLATFORM
                #include <pspe/$$_ded_stdiocore.h>
                #include <pspe/$$_ded_stdiostream.h>
                #include <pspe/$$_ded_stdioprintf.h>
                #include <pspe/$$_ded_stdioscanf.h>
                #include <pspe/$$_ded_stdioexec.h>
                #include <pspe/$$_ded_stdiowchar.h>
                #include <pspe/$$_ded_stdiobridgecpp.h>
                #include <pspe/$$_ded_misc0.h>
                #include <pspe/$$_ded_misc1.h>
         #endif
         
         #ifdef PSP_PLATFORM
                #include <psp/$$_ded_stdiocore.h>
                #include <psp/$$_ded_stdiostream.h>
                #include <psp/$$_ded_stdioprintf.h>
                #include <psp/$$_ded_stdioscanf.h>
                #include <psp/$$_ded_stdioexec.h>
                #include <psp/$$_ded_stdiowchar.h>
                #include <psp/$$_ded_stdiobridgecpp.h>
                #include <psp/$$_ded_misc0.h>
                #include <psp/$$_ded_misc1.h>
         #endif

#endif	// _NANOC_STDIO_H
