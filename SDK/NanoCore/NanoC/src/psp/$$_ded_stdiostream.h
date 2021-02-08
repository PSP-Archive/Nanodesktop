/*
   --------------------------------------------------------------------------
   $$_ded_stdiostream.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di base per la gestione degli stream (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_STDIOSTREAM_H_
       #define _DED_STDIOSTREAM_H_
       
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
          
          
          // Inclusione di $$_ded_stdiocore.h che fornirà i prototipi della
          // funzioni a livello più basso (ed i riferimenti extern)
          #include <psp/$$_ded_stdiocore.h>
          #include <nanoc_fsstringfunc.h>
          
          #include <sys\stat.h>
          
                
          
       #ifdef __cplusplus
       extern "C" 
       {
       #endif
       
       /*
          ---------------------------------------------------------------------
          SIMBOLI
          ---------------------------------------------------------------------
       */
       
       #define STDIN_FILENO  (int)(&StreamsArray [0])
       #define STDOUT_FILENO (int)(&StreamsArray [1])
       #define STDERR_FILENO (int)(&StreamsArray [2])
       
       
       /*
          ---------------------------------------------------------------------
          PROTOTIPI
          ---------------------------------------------------------------------
       */
            
             extern FILE *fopen (const char *fname, const char *mode); 
             extern FILE *freopen (char *fname, char *mode, FILE *stream); 
             extern FILE *fdopen (int fd, char *mode); 
             extern int fclose (FILE *stream); 
             extern int fcloseall (void); 
             extern size_t fread (void *buf, int r, int n, FILE *stream); 
             extern size_t fwrite (const void *buf, size_t r, size_t n, FILE *stream); 
             extern int fgetc (FILE *stream); 
             extern int xgetc (FILE *stream); 
             extern char *fgets (char *buf, int n, FILE *stream);
             extern char *fgets0D (char *buf, int n, FILE *stream); 
             extern char *gets (char *buf); 
             extern int getc (FILE *stream); 
             extern int getchar (void); 
             extern int fputc (int c, FILE *stream); 
             extern int fputs (const char *s, FILE *stream); 
             extern int putc (int c, FILE *stream); 
             extern int puts (char *s); 
             extern int putchar (int c); 
             extern int fseek (FILE *stream, long int offset, int origin); 
             extern long int ftell (FILE *stream); 
             extern int fgetpos (FILE *stream, fpos_t *pos); 
             extern int fsetpos (FILE *stream, fpos_t *pos); 
             extern void rewind (FILE *stream); 
             extern int ungetc (int c, FILE *stream); 
             extern char *tmpnam (char *name); 
             extern FILE *tmpfile (void); 
             extern int fileno (FILE *f); 
             extern int feof (FILE *stream); 
             extern int ferror (FILE *stream); 
             extern char *strerror (int err); 
             extern void perror (char *s); 
             extern void clearerr (FILE *stream); 
             extern int fflush (FILE *stream); 
             extern int fflushall (void); 
             extern int setvbuf (FILE *Stream, char *Buf, int Mode, size_t BufSize); 
             extern int setbuf (FILE *Stream, char *Buf); 
             extern int devctl (const char *dev, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen); 
            
             extern int	fstat ( int VFH_Handle, struct stat *Stat );
             extern int	lstat ( const char *NameFile, struct stat *Stat );       
            
             
            
       #ifdef __cplusplus
       }
       #endif
       

#endif
