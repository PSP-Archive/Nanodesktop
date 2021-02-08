/*
   --------------------------------------------------------------------------
   $$_ded_stdiostream.h               NanoC File Headers (PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines printf e derivate (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_STDIOPRINTF_H_
       #define _DED_STDIOPRINTF_H_
       
       /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
          #include <pspkernel.h>
          #include <psptypes.h>
          
          #include <pspiofilemgr.h>          
          #include <pspiofilemgr_stat.h>

          #include <stdint.h>
          #include <stddef.h>
          #include <stdarg.h>

          #include <limits.h>
          #include <errno.h>

          #include <malloc.h>
          #include <string.h>
          
          #include <float.h>
          #include <math.h>
          #include <nanoc_fsstringfunc.h>

          // Inclusione di $$_ded_stdiocore.h che fornirà i prototipi della
          // funzioni a livello più basso (ed i riferimenti extern)
          #include <pspe/$$_ded_stdiocore.h>
          
          // Inclusione di $$_ded_stdiostream.h che fornirà i prototipi della
          // funzioni a livello più basso (ed i riferimenti extern)
          #include <pspe/$$_ded_stdiostream.h>
           
          
       #ifdef __cplusplus
       extern "C" 
       {
       #endif
       
       /*
          ---------------------------------------------------------------------
          STRUTTURE
          ---------------------------------------------------------------------
       */
       
        struct s_strargument            
        {
          char *next;                   /* Prossimo carattere libero nella stringa */
          char *last;                   /* Ultimo carattere della stringa */
        };

       /*
          ---------------------------------------------------------------------
          PROTOTIPI
          ---------------------------------------------------------------------
       */
            
           extern void _INTERNAL_WriteFncToFile_ (char *zNewText, int nNewChar, void *arg); 
           extern void _INTERNAL_WriteFncToMemory_ (char *zNewText, int nNewChar, void *arg); 
           extern void _INTERNAL_WriteFncToString_ (char *txt, int amt, void *arg); 
     
           extern int isdigit (int);
           
           extern int vxprintf (void (*func) (char *, int, void *), void *arg, const char *format, va_list ap); 
           extern int xprintf (void (*func) (char *, int, void *), void *arg, const char *format, ...); 
           
           extern int snprintf (char *str, size_t sz, const char *format, ...); 
           extern int vsnprintf (char *buf, size_t n, const char *fmt, va_list ap); 
           
           extern int sprintf (char *str, const char *format, ...); 
           extern int vsprintf (char *buf, const char *fmt, va_list ap); 
           
           extern char *mprintf (const char *zFormat, ...); 
           extern char *vmprintf (const char *zFormat, va_list ap); 
           
           extern int asprintf (char **strp, const char *zFormat, ...); 
           extern int vasprintf (char **strp, const char *format, va_list ap); 
           
           extern int fprintf (FILE *pOut, const char *zFormat, ...); 
           extern int vfprintf (FILE *pOut, const char *zFormat, va_list ap); 
           
           extern int printf (const char *format, ...); 
           extern int vprintf (const char *format, va_list args); 
     
           int errprintf(const char *format, ...);
           int errvprintf(const char *format, va_list args);    

       #ifdef __cplusplus
       }
       #endif
       

#endif
