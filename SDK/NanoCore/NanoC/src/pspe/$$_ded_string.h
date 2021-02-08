/*
   --------------------------------------------------------------------------
   $$_ded_string.h               NanoC File Headers (PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di gestione stringhe (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_STRING_H_
       #define _DED_STRING_H_
       
       /*
          ---------------------------------------------------------------------
          INCLUSIONE DI ALTRI HEADERS
          ---------------------------------------------------------------------
       */
       
           #include <psptypes.h>
           #include <limits.h>
           #include <string.h>
           #include <malloc.h>
           
       
       /*
          ---------------------------------------------------------------------
          PROTOTIPI 
          ---------------------------------------------------------------------
       */
       

       #ifdef __cplusplus
       extern "C" 
       {
       #endif

             int isalnum(int);
             int isalpha(int);
             int iscntrl(int);
             int isdigit(int);
             int isgraph(int);
             int islower(int);
             int isprint(int);
             int ispunct(int);
             int isspace(int);
             int isupper(int);
             int isxdigit(int);
             int tolower(int);
             int toupper(int);
             int isascii(int x); 

             extern unsigned int strlen (const char *s); 
             extern char *strcat (char *s, const char *append); 
             extern char *strncat (char *s, const char *append, size_t count);
             extern int strcmp (const char *s1, const char *s2); 
             extern int strcoll (const char *s1, const char *s2); 
             extern int stricmp (unsigned char *scan1, unsigned char *scan2); 
             extern int strncmp (const char *s1, const char *s2, size_t count); 
             extern int strnicmp (unsigned char *str1, unsigned char *str2, int len); 
             extern int strxfrm (char *s1, char *s2, int n);
             extern char *strcpy (char *dst, const char *src); 
             extern char *strncpy (char *dst, const char *src, size_t count); 
             extern int memcmp (const void *b1, const void *b2, size_t len); 
             extern void *memcpy (void *dst, const void *src, size_t len); 
             extern void *memmove (void *dst, const void *src, size_t len); 
             extern void *memset (void *b, int c, size_t len); 
             extern void *memchr (const void *b, int c, size_t len); 
             extern char *strchr (const char *s, int c); 
             extern char *strdup (const char *s); 
             extern int strcasecmp (const char *string1, const char *string2); 
             extern int strncasecmp (const char *string1, const char *string2, size_t n); 
             extern char *strtok (char *strToken, const char *strDelimit); 
             extern char *strrchr (const char *string, int c); 
             extern char *strstr (const char *string, const char *substring); 
             extern char *strupr (char *str); 
             extern char *strlwr (char *str); 
             
             extern unsigned char isSpecialSJIS (short int sjis); 
             extern short int isSpecialASCII (unsigned char ascii); 
             extern int strcpy_ascii (char *ascii_buff, const short int *sjis_buff); 
             extern int strcpy_sjis (short int *sjis_buff, const char *ascii_buff); 
             extern char *strpbrk (const char *s, const char *accept); 
             extern size_t strspn (const char *s, const char *accept); 
             extern size_t strcspn (const char *s, const char *reject); 
             
       /* Backward compatibility... */
       #include <ctype.h>
       
       #ifdef __cplusplus
       }
       #endif
       
#endif

