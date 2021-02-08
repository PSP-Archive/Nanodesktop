/*
   --------------------------------------------------------------------------
   $$_ded_stdioscanf.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines scanf e derivate  (header)
   ----------------------------------------------------------------------------
*/   


#ifndef _DED_STDIOSCANF_H_
       #define _DED_STDIOSCANF_H_

       /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
           #include <ctype.h>
           #include <limits.h>
           #include <stdarg.h>
           #include <stdio.h>
           #include <stdlib.h>
           #include <string.h>
           
           // Inclusione di altri header stdio (solo headers)
           #include <psp/$$_ded_stdiocore.h>         
           #include <psp/$$_ded_stdiostream.h>
           #include <psp/$$_ded_stdioprintf.h>

        /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */

       #ifdef __cplusplus
       extern "C" 
       {
       #endif
       

       #ifdef DED_STDIOSCANF_I_NEED_ALL_PROTOTYPES    
           extern int   INTERNAL_GetCharFromFile (void **stream); 
           extern void  INTERNAL_UnGetCharFromFile (int c, void **stream); 
           extern int   INTERNAL_GetCharFromString (void **p_str); 
           extern void  INTERNAL_UnGetCharFromString (int c, void **p_str); 
           extern int   INTERNAL_EngineScanF (int (*xgetc) (void **), void (*xungetc) (int, void **), void *stream, const char *fmt, va_list ap); 
       #endif  
         
           extern int vxscanf (int (*xgetc) (void **), void (*xungetc) (int, void **), void *stream, const char *fmt, va_list ap); 
           extern int vfscanf (FILE *stream, const char *fmt, va_list args); 
           extern int vsscanf (const char *str, const char *fmt, va_list args); 
           extern int vscanf  (const char *fmt, va_list args); 
           extern int fscanf  (FILE *stream, const char *fmt, ...); 
           extern int sscanf  (const char *str, const char *fmt, ...); 
           extern int scanf   (const char *fmt, ...); 
           extern int xscanf  (int (*xgetc) (void **), void (*xungetc) (int, void **), void *stream, const char *fmt, ...); 
        

       #ifdef __cplusplus
       }
       #endif  
       
#endif
