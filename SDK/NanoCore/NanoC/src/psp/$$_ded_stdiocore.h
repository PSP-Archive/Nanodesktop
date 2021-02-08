/*
   --------------------------------------------------------------------------
   $$_ded_stdiocore.h               NanoC File Headers (PSP)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di base per la gestione del sottosistema dischi (header)
   ----------------------------------------------------------------------------
*/   

#ifndef _DED_STDIOCORE_H_
       #define _DED_STDIOCORE_H_

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
          
          #include <sys\stat.h>
          
          #include <nanoc_fsstringfunc.h>

          
          
       #ifdef __cplusplus
       extern "C" 
       {
       #endif
       
       /*
          ---------------------------------------------------------------------
          CONFIGURAZIONE
          ---------------------------------------------------------------------
       */
       
          #ifdef DED_STDIOCORE_I_NEED_VARIABLES
                 #define STDIOCORE_EXTERN 
          #else  
                 #define STDIOCORE_EXTERN extern
          #endif
          
       /*
          ---------------------------------------------------------------------
          SIMBOLI DI COMPILAZIONE
          ---------------------------------------------------------------------
       */
       
       
               /*
                * -----------------------------------------------------------------------
                * Origin: PSP Software Development Kit - http://www.pspdev.org
                * -----------------------------------------------------------------------
                * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
                *
                * stdio.h
                *
                * Copyright (c) 2002-2004 PS2DEV
                * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
                * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
                * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
                *
                * $Id: stdio.h 1095 2005-09-27 21:02:16Z jim $
                -----------------------------------------------------------------------
               */
        
                    #define _open open
                    #define _close close
                    #define _read read
                    #define _write write
                    #define _lseek lseek
                    
                    #define _O_APPEND O_APPEND
                    #define _O_BINARY O_BINARY
                    #define _O_CREAT  O_CREAT
                    #define _O_RDONLY O_RDONLY
                    #define _O_RDWR   O_RDWR
                    #define _O_TEXT   O_TEXT
                    #define _O_TRUNC  O_TRUNC
                    #define _O_WRONLY O_WRONLY
                    
                    #define STD_IOBUF_TYPE_NONE            0
                    #define STD_IOBUF_TYPE_GE              1
                    #define STD_IOBUF_TYPE_MS			   2
                    #define STD_IOBUF_TYPE_UMD	           4
                    #define STD_IOBUF_TYPE_HOST            8
                    #define STD_IOBUF_TYPE_STDOUTHOST     16
                    
                    #define __NRVIRTUALFILEHANDLER 20
                    #define _NFILE __NRVIRTUALFILEHANDLER
                    
                    #define _IOFBF                         0x0000
                    #define _IOLBF                         0x0100
                    #define _IONBF                         0x0004
                    #define _IOEOF                         0x0020
                    #define _IOERR                         0x0040
                    
                    #define _IOREAD                        0x0001
                    #define _IOWRT                         0x0002
                    #define _IORW                          0x0200
                    #define _IOMYBUF                       0x0010
                    
                    #define O_RDONLY                       0x0001
                    #define O_WRONLY                       0x0002
                    #define O_RDWR                         0x0003
                    #define O_NBLOCK                       0x0010
                    #define O_APPEND                       0x0100
                    #define O_CREAT                        0x0200
                    #define O_TRUNC                        0x0400
                    #define O_EXCL	                       0x0800
                    #define O_NOWAIT                       0x8000
                    
                    /* ensure EOF is defined. */
                    #ifndef EOF
                    #define EOF                            (-1)
                    #endif
                    
                    #define FOPEN_MAX                      _NFILE
                    #define FILENAME_MAX                   1024
                    
                    #define SEEK_SET                       0
                    #define SEEK_CUR                       1
                    #define SEEK_END                       2
                    
                    #define NO_FILE_ERR 0
                    #define FILE_ERR 1
                    
                    #ifndef O_BINARY
                    #define O_BINARY 0
                    #endif
                    
                    #ifndef O_TEXT
                    #define O_TEXT 0
                    #endif
                    
                    #define BUFSIZ                         1024
                    
                    /* ensure fpos_t is defined. */
                    #ifndef __FPOS_T_DEFINED
                    #define __FPOS_T_DEFINED
                    typedef long fpos_t;
                    #endif // __FPOS_T_DEFINED
                    
                    /* ensure FILE is defined. */
                    #ifndef __FILE_DEFINED
                    #define __FILE_DEFINED
                    typedef struct 
                    {
                      int  type;
                      int  fd;
                      int  cnt;
                      int  flag;
                      int  len;
                      int  has_putback;
                      u8   putback;
                    } FILE;
                    #endif // __FILE_DEFINED
        
               /*
                * --------------------------------------------------------------------------------------
                * Origin: Filippo Battaglia
                * --------------------------------------------------------------------------------------    
                   Copyright 2006-2011 Filippo Battaglia
                   
                   IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
                
                   By downloading, copying, installing or using the software you agree to this license.
                   If you do not agree to this license, do not download, install,
                   copy or use the software.
                
                   Redistribution and use in source and binary forms, with or without modification,
                   are permitted provided that the following conditions are met:
                
                     * Redistribution's of source code must retain the above copyright notice,
                       this list of conditions and the following disclaimer.
                 
                     * Redistribution's in binary form must reproduce the above copyright notice,
                       this list of conditions and the following disclaimer in the documentation
                       and/or other materials provided with the distribution.
                  
                     * The name of the author may not be used to endorse or promote products
                       derived from this software without specific prior written permission.
                  
                   This software is provided by the copyright holders and contributors "as is" and
                   any express or implied warranties, including, but not limited to, the implied
                   warranties of merchantability and fitness for a particular purpose are disclaimed.
                   In no event shall the author or contributors be liable for any direct,
                   indirect, incidental, special, exemplary, or consequential damages
                   (including, but not limited to, procurement of substitute goods or services;
                   loss of use, data, or profits; or business interruption) however caused
                   and on any theory of liability, whether in contract, strict liability,
                   or tort (including negligence or otherwise) arising in any way out of
                   the use of this software, even if advised of the possibility of such damage.
               ------------------------------------------------------------------------------------
               */
               
                    struct VirtualHandle_Type
                    {
                       int  OsHandle;
                       char IsEnabled;
                       char ReadOn;
                       char WriteOn;
                       int  SysWord;
                       int  HW_Word;
                       
                       int  PosSeek32;
                       int  FileLength;
                       void* AddressData;
                       
                       char BankLoaded;
                       int  NrBankLoaded;
                       int  BeginOfBank;
                       int  EndOfBank;
                       int  BankIsUpdated;
                       
                       char Name [128]; 
                    };
                    
                    #define stdin                          (&StreamsArray [0])
                    #define stdout                         (&StreamsArray [1])
                    #define stderr                         (&StreamsArray [2])
                    
                    #define  DISK_JUMP          60000 
                    #define  DISK_BUFFER_SIZE  200000
                    
                    

                    
       /*
          ---------------------------------------------------------------------
          STRUTTURE E VARIABILI
          ---------------------------------------------------------------------
       */
    
                    extern  int errno;    // E' un simbolo esterno. Lo definisce errno.h
                    
                    STDIOCORE_EXTERN  struct VirtualHandle_Type VFH [__NRVIRTUALFILEHANDLER];
                    STDIOCORE_EXTERN  FILE    StreamsArray [_NFILE];        // Usato al posto di _iob
                    
                    #ifdef DED_STDIOCORE_I_NEED_VARIABLES
                           char   SystemPath  [255] = "ms0:/";
                           char   SysRootPath [255] = "ms0:/";
                           char   __stdio_tmpnam[256] = "ms0:/temp.tmp";   // Il file temporaneo previsto dalla libc per Unix
                           
                           char   _PSPSDK_InitFSExecuted = 0;
                    #else
                           STDIOCORE_EXTERN  char SystemPath  [255];
                           STDIOCORE_EXTERN  char SysRootPath [255];
                           STDIOCORE_EXTERN  char  __stdio_tmpnam[256];
                           
                           STDIOCORE_EXTERN  char   _PSPSDK_InitFSExecuted;
                    #endif
                    
                    
          
                    STDIOCORE_EXTERN int ndTempFilesBusy; 
          
        /*
          ---------------------------------------------------------------------
          PROTOTIPI
          ---------------------------------------------------------------------
       */
              
             extern void _pspsdk_stdio_init (void); 
             extern void _pspsdk_stdio_deinit (void); 
             
             extern int INTERNAL_Seek32 (SceUID fd, SceOff Offset, int Whence); 
             extern int INTERNAL_SaveBank (int VFH_Handle, int NrBank); 
             extern int INTERNAL_LoadBank (int VFH_Handle, int NrBank); 
             
             extern int open (const char *NameFile, int flags, int mode); 

             extern int INTERNAL_open____mode_RD (char *NameFile, char *LongName, int flags, int mode, int VFHChoosen); 
             extern int INTERNAL_open____mode_WR_RW (char *NameFile, char *LongName, int flags, int mode, int VFHChoosen); 

             extern int read (int VFHNumber, void *Data, int Size); 
             extern int write (int VFHNumber, const void *Data, int Size); 
             extern int close (int VFHNumber); 
             extern int lseek (int VFHNumber, int Offset, int Whence); 
             extern int tell (int VFHNumber); 
             extern int remove (const char *NameFile); 
             extern int mkdir (const char *dir, int mode); 
             extern int rmdir (const char *dir); 
             extern int chdir (char *dir); 
             extern int rename (const char *name, const char *newname); 

             extern char *get_current_working_dir_name (void); 
             extern char *getcwd (char *buf, size_t size); 

             extern int flen (char *NameFile); 
             extern int fexist (char *NameFile); 
             extern int access (char *path, int mode);
             extern int truncate (char *name, int length);
             
             extern int	stat ( const char *NameFile, struct stat *Stat );
             
             extern int link(const char *oldpath, const char *newpath);
             extern int unlink(const char *pathname); 
       
             extern int creat(const char *path, mode_t mode);
             extern int chmod (const char *filename, mode_t mode);
             
             extern int fcntl (int fd, int cmd, ...);
             
             // Funzioni speciali di copia
             extern char doscopy (char *filedest, char *filesrc);

                        
       #ifdef __cplusplus
       }
       #endif
       
#endif
