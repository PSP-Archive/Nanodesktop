/*
   --------------------------------------------------------------------------
   $$_ded_stdlib.h               NanoC File Headers (PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Funzioni standard di libreria (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_STDLIB_H_
       #define _DED_STDLIB_H_
       
       /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
           #include <pspkernel.h>
           #include <stddef.h>
           #include <errno.h>

           #include <stdio.h>
           #include <string.h>
           #include <limits.h>

           #include <float.h>
           #include <math.h>
       
       /*
          ---------------------------------------------------------------------
          SIMBOLI STDLIB
          ---------------------------------------------------------------------
       */
       
          #ifdef __cplusplus
          extern "C" 
          {
          #endif

          
                /* ensure NULL is defined. */
                #ifndef NULL
                #define NULL                           (void *)0
                #endif
    
                /* exit status constants. */
                #define EXIT_SUCCESS                   0
                #define EXIT_FAILURE                   1
                
                /* multibyte maximum character constant. */
                #define MB_CUR_MAX                     1
                
                /* ensure div_t is defined. */
                #ifndef __DIV_T_DEFINED
                #define __DIV_T_DEFINED
                typedef struct 
                {
                  int quot;
                  int rem;
                } div_t;
                #endif // __DIV_T_DEFINED
                            
                /* ensure ldiv_t is defined. */
                #ifndef __LDIV_T_DEFINED
                #define __LDIV_T_DEFINED
                typedef struct 
                {
                  long quot;
                  long rem;
                } ldiv_t;
                #endif // __LDIV_T_DEFINED
                
                #ifndef __LLDIV_T_DEFINED
                #define __LLDIV_T_DEFINED
                typedef struct 
                {
                  long long quot;
                  long long rem;
                } lldiv_t;
                #endif // __LLDIV_T_DEFINED
                
                #ifndef __ENVIRONVARIABLE_T_DEFINED
                #define __ENVIRONVARIABLE_T_DEFINED
                typedef struct 
                {
                   char name[256];
                   char value[256];
                } environvariable_t;
                #endif // __ENVIRONVARIABLE_T_DEFINED
    
                /* we don't check for any previously defined value. This HAS to be that. */
                #define        RAND_MAX        2147483647
                
                typedef int		cmp_t(const void *, const void *);
       
                #define	RAND48_SEED_0	(0x330e)
                #define	RAND48_SEED_1	(0xabcd)
                #define	RAND48_SEED_2	(0x1234)
                #define	RAND48_MULT_0	(0xe66d)
                #define	RAND48_MULT_1	(0xdeec)
                #define	RAND48_MULT_2	(0x0005)
                #define	RAND48_ADD	(0x000b)
              
       
       /*
          ---------------------------------------------------------------------
          PROTOTIPI 
          ---------------------------------------------------------------------
       */
       
             extern void _pspsdk_stdlib_init (void); 
             extern void _pspsdk_stdlib_deinit (void); 
 
             extern int atexit (void (*func) (void)); 
             extern void exit (int status); 
             extern void done (int status); 
             extern void abort (void); 
           
             extern int abs (int c); 
             extern long int labs (long int n); 
             extern long long int llabs (long long int n); 
             extern void *bsearch (const void *key, const void *base, size_t count, size_t size, int (*compare) (const void *, const void *)); 
             extern div_t div (int n, int d); 
             extern ldiv_t ldiv (long int n, long int d); 
             extern lldiv_t lldiv (long long int n, long long int d); 
             extern char *_itoa (int n, char *buf, int radix); 
             extern char *_lltoa (long long int n, char *buf, int radix); 
             extern char *_ltoa (long int n, char *buf, int radix); 
             extern int rand (void); 
             extern void srand (unsigned int seed); 
             extern char *getenv (const char *name); 
             extern int setenv (const char *name, const char *value, int rewrite); 
             extern int putenv (const char *string);
             extern int unsetenv (const char *name);
             extern int __assert_fail (const char *assertion, const char *file, unsigned int line); 
             extern long int strtol (const char *nptr, char **endptr, int base); 
             extern int atoi (const char *s); 
             extern long int atol (const char *s); 
             extern long unsigned int strtoul (const char *nptr, char **endptr, int base); 
             extern double strtod (const char *str, char **endptr); 
             extern float strtof (const char *str, char **endptr);
             extern double atof (const char *str); 
             
             extern void *lfind (const void *key, const void *base, size_t *nelp, size_t width, int (*compar) (const void *, const void *)); 
             extern void *lsearch (const void *key, void *base, size_t *nelp, size_t width, int (*compar) (const void *, const void *)); 
             extern int system (const char *string);     // Predisposto per il C++
            
             extern void qsort (void *a, size_t n, size_t es, cmp_t (*cmp)); 
             
             extern int   mkstemp (char* templat);
             extern char* mktemp (char* templat); 
             
             extern void  __dorand48(unsigned short[3]);
             extern long  nrand48(unsigned short xseed[3]);
             
             extern int sgn (int x);

             // Include un header UTF8 per fornire i prototipi delle routines
             // mblen, mbstowcs, mbtowc, wcstombs, wctomb

             #include <nanoc_3rdparty/pspe/utf8/include/utf8/nd_stdlib.h>

             
       
       #ifdef __cplusplus
       }
                         // C++ is evil, let's defeat it... 
                         #define _CPP_CSTDLIB 1
       #endif
       
             
       
#endif
