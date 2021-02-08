/*
   --------------------------------------------------------------------------
   stdiostream.c                                      NanoC Files (PSP)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di base per la gestione degli stream 
   ----------------------------------------------------------------------------
*/   



#define DED_STDIOSTREAM_I_NEED_VARIABLES
          #include <psp/$$_ded_stdiostream.h>
#undef  DED_STDIOSTREAM_I_NEED_VARIABLES

/*
    --------------------------------------------------------------------------
    RIFERIMENTI A ROUTINES O STREAMS NANODESKTOP USATI NEL SEGUITO
    --------------------------------------------------------------------------
*/


    #define NORENDER 0
    #define RENDER   1
    
    extern void ndInitStdInSystem (void); 
    extern void ndSET_SystemKeyboardAutoClose (char Value); 
    extern char ndSET_SystemKeyboardType (char Type); 
    extern int  ndGetChar (void); 
    extern void ndCloseSystemKeyboard (void); 
    
    extern void ndInitStdOutSystem (void); 
    extern int  ndActivateStdOutDialogBox (void); 
    extern int  ndWriteToStdOut (char Carattere, char TypeChannel, char RenderNow);      
    extern void ndDisableStdOutDialogBox (void); 
    extern char ndEnableStdOutCustomDims (int NewDimX, int NewDimY);
    extern void ndDisableStdOutCustomDims (void);
    extern void ndStdOutRender ();
                                
    extern char StdOutWindowActivated;
    extern char StdOutWindowHandle;
    
    extern char XWindowRender (unsigned char WndHandle); 
    extern char Magn (char Carattere); 
    
    extern int printf (const char *format, ...); 
    extern int fprintf (FILE *pOut, const char *zFormat, ...); 
                  
    extern char *_itoa (int n, char *buf, int radix); 


/* -----------------------------------------------------------------------
 * Origin: PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * stdio.c - Simple standard C library implementation.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 * $Id: stdio.c 1057 2005-09-20 22:21:07Z tyranid $
 * -----------------------------------------------------------------------
 
   Note:
        Following routines have been modified by Filippo Battaglia. My
        target was to allow the integration in Nanodesktop.
    -----------------------------------------------------------------------
*/

// ----------------------- Bridge STD-OUT per Nanodesktop -----------------

char fwrite_to_stdout (char Carattere, char TypeChannel)
{
   char Result1, Result2;  
     
   if (!StdOutWindowActivated)
   {
      Result1=ndActivateStdOutDialogBox ();
   
      if (Result1==1)                 // E' stato possibile aprire la std-out
                                      // dialog-box
      {
         Result2=ndWriteToStdOut (Carattere, TypeChannel, NORENDER);
                  
         return Result2;       // Se ndWriteToStdOut fallisce, restituirà un valore negativo.
                               // Questo segnalerà alla routine chiamante che c'è stato un errore.
                               // Altrimenti verrà restituito zero.
      }
      else
      {
          errno=5;                   // Simula errore di I/O                           
          return -1;                 // Segnala un errore alla routine chiamante 
      }
   }
   else        // Non è necessario aprire una dialog-box
   {
      Result2=ndWriteToStdOut (Carattere, TypeChannel, NORENDER);
      return Result2;
   }
}




// -------------------------------- Codice di gestione file --------------------

FILE *fopen (const char *fname, const char *mode)
/*
**  [func] - fopen.
**  [desc] - attempts to open the fname file using the mode file mode. if able
**           open the fname then returns the pointer to the FILE stream. else
**           returns NULL.
**  [entr] - const char *fname; the filename string pointer.
**           const char *mode; the file mode string pointer.
**  [exit] - FILE *; the pointer the fname FILE stream. else NULL.
**  [prec] - fname and mode are valid string pointers.
**  [post] - the fname file is opened.

    Nota:
         
    Sotto PSPE la libreria NanoC mostra delle differenze rispetto a quanto è
    normalmente previsto dallo standard ANSI C.
    
    Una stringa mode pari ad r apre il file in lettura, una stringa mode 
    pari a w apre il file in scrittura, ed una stringa mode pari ad a 
    apre il file in scrittura con append. Nel caso di mode pari ad a 
    oppure w, se il file non esiste viene creato.
    
    I simboli b t e + sono SEMPRE ignorati dalla routine.
*/
{
  FILE *ret = NULL;
  int  fd, flag = 0, i, iomode = 0;
  
  int  Counter;
  char Carattere;
  char _ReadMode, _WriteMode, _AppendMode;


  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  // Verifica che mode e fname non siano stringhe vuote
  if ((fname != NULL) && (*fname != '\0')) 
  {
    if ((mode != NULL) && (*mode != '\0')) 
    {
       _ReadMode=0;
       _WriteMode=0;
       _AppendMode=0;       
                
       for (Counter=0; Counter<(strlen (mode)); Counter++)
       {
           Carattere = Magn (mode [Counter]);
       
           switch (Carattere)
           {
                  case 'R':
                       {
                           _ReadMode=1;
                           break;
                       }
                  
                  case 'W':
                       {
                           _WriteMode=1;
                           break;
                       }
                  
                  case 'A':
                       {
                           _AppendMode=1;
                           break;
                       }
           
                  // I simboli +,b,t vengono ignorati
           }
       }       
       
       if (_AppendMode)         // Se viene indicato l'appending, viene ignorato qualsiasi 
       {                        // altro simbolo
            
            if (fexist ( (char *)(fname) ))    // Controlla se il file esiste già
            {            
               flag = _IOWRT;
               iomode = (O_WRONLY | O_APPEND);
            }
            else
            {
               flag = _IOWRT;
               iomode = (O_WRONLY | O_CREAT | O_TRUNC);
            } 
       
       }
       else   // Non è avviata la modalità di appending
       {
            if ((_ReadMode) && (_WriteMode))    // Attiva modalità di lettura/scrittura
            {
                flag = _IOREAD & _IOWRT;
                iomode = (O_RDWR | O_CREAT | O_TRUNC);
            }
            else       // Non è vero che è attiva la modalità di lettura/scrittura
            {
                if (_ReadMode)
                {
                   flag = _IOREAD;
                   iomode = O_RDONLY;  
                }
       
                if (_WriteMode)
                {
                   flag = _IOWRT;
                   iomode = (O_WRONLY | O_CREAT | O_TRUNC); 
                }
            }
       }
       
       // Assegniamo un opportuno stream 
       
       for (i = 3; i < _NFILE; ++i)            // Si inizia il conteggio da zero per saltare stdin, stdout, stderr
       {
           if (StreamsArray [i].fd < 0) break;
       }
      
       if (i < _NFILE)                        // Abbiamo trovato un handle disponibile
       {
         fd = open (fname, iomode, 0777);     // attempt to open the fname file. 
              
         if (fd >= 0) 
         {
           StreamsArray [i].type = STD_IOBUF_TYPE_MS;
           StreamsArray [i].fd   = fd;
           StreamsArray [i].cnt  = 0;
           StreamsArray [i].flag = flag;
           StreamsArray [i].has_putback = 0;
           ret = &(StreamsArray[i]);
         }
       }
      
    }
  }
  
  return ret;
}



FILE   *freopen (char *fname, char *mode, FILE *stream)
/*
**  [func] - freopen.
**  [desc] - redirect a stream. if able open the fname then returns the 
             pointer to the FILE stream. else returns NULL.
**  [entr] - const char *fname; the filename string pointer.
**           const char *mode; the file mode string pointer.
             FILE *stream; the stream to redirect
**  [exit] - FILE *; the pointer the fname FILE stream. else NULL.
**  [prec] - fname and mode are valid string pointers.
**  [post] - the fname file is opened.

    Nota:
         
    Sotto PSPE la libreria NanoC mostra delle differenze rispetto a quanto è
    normalmente previsto dallo standard ANSI C.
    
    Una stringa mode pari ad r apre il file in lettura, una stringa mode 
    pari a w apre il file in scrittura, ed una stringa mode pari ad a 
    apre il file in scrittura con append. Nel caso di mode pari ad a 
    oppure w, se il file non esiste viene creato.
    
    I simboli b t e + sono SEMPRE ignorati dalla routine.
*/
{
   FILE *ret = NULL;
   int  fd, flag = 0, iomode = 0;
    
   char _ReadMode, _WriteMode, _AppendMode;
   int  Counter;
   char Carattere;
   
   int  ErrorInDiskFClose, ErrorInStreamDealloc;
   
 
   if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
   // Richiama automaticamente la funzione di reinizializzazione del file-system
   // qualora essa non sia già stata utilizzata
  
   // Verifica che mode e fname non siano stringhe vuote
   if ((fname != NULL) && (*fname != '\0')) 
   {
        if ((mode != NULL) && (*mode != '\0')) 
        {
              // Attempt to close the previous file
              switch (stream->type) 
              {
                    case STD_IOBUF_TYPE_NONE:
                    case STD_IOBUF_TYPE_GE:
                    case STD_IOBUF_TYPE_STDOUTHOST:
                          // In questo contesto la chiusura di stdout è da considerarsi
                          // consentita: in questo modo la routine proseguirà il processo
                          // di redirect dello stream.
                          ErrorInStreamDealloc = 0;
                          break;
                    default:
                            {
                                if (stream->fd >=0)
                                {
                                   ErrorInDiskFClose = close (stream->fd);
                                
                                   if (ErrorInDiskFClose >= 0)  
                                   {
                                      stream->type = STD_IOBUF_TYPE_NONE;
                                      stream->fd = -1;
                                      stream->cnt = 0;
                                      stream->flag = 0;
                                      
                                      ErrorInStreamDealloc = 0;
                                   }
                                   else ErrorInStreamDealloc = EOF;
                                }
                                else ErrorInStreamDealloc = EOF;
                            }       
              }     // End of switch
              
              // Arrivati a questo punto, se ErrorInStreamDealloc è posto pari a 0, significa che 
              // il file associato allo stream iniziale è stato chiuso. In questo
              // caso può proseguire la fase di redirect
              
              if (!ErrorInStreamDealloc)
              {
                       _ReadMode=0;
                       _WriteMode=0;
                       _AppendMode=0;       
                                
                       for (Counter=0; Counter<(strlen (mode)); Counter++)
                       {
                           Carattere = Magn (mode [Counter]);
                       
                           switch (Carattere)
                           {
                                  case 'R':
                                       {
                                           _ReadMode=1;
                                           break;
                                       }
                                  
                                  case 'W':
                                       {
                                           _WriteMode=1;
                                           break;
                                       }
                                  
                                  case 'A':
                                       {
                                           _AppendMode=1;
                                           break;
                                       }
                           
                                  // I simboli +,b,t vengono ignorati
                           }
                       }       
                       
                       if (_AppendMode)         // Se viene indicato l'appending, viene ignorato qualsiasi 
                       {                        // altro simbolo
                            
                            if (fexist (fname))    // Controlla se il file esiste già
                            {            
                               flag = _IOWRT;
                               iomode = (O_WRONLY | O_APPEND);
                            }
                            else
                            {
                               flag = _IOWRT;
                               iomode = (O_WRONLY | O_CREAT | O_TRUNC);
                            } 
                       
                       }
                       else   // Non è avviata la modalità di appending
                       {
                            if ((_ReadMode) && (_WriteMode))    // Attiva modalità di lettura/scrittura
                            {
                                flag = _IOREAD & _IOWRT;
                                iomode = (O_RDWR | O_CREAT | O_TRUNC);
                            }
                            else       // Non è vero che è attiva la modalità di lettura/scrittura
                            {
                                if (_ReadMode)
                                {
                                   flag = _IOREAD;
                                   iomode = O_RDONLY;  
                                }
                       
                                if (_WriteMode)
                                {
                                   flag = _IOWRT;
                                   iomode = (O_WRONLY | O_CREAT | O_TRUNC); 
                                }
                            }
                       }
                       
                       // Riprogrammiamo lo stream
                       
                       fd = open (fname, iomode, 0777);     // attempt to open the fname file. 
              
                       if (fd >= 0) 
                       {
                          stream->type = STD_IOBUF_TYPE_MS;
                          stream->fd   = fd;
                          stream->cnt  = 0;
                          stream->flag = flag;
                          stream->has_putback = 0;
                          
                          ret = stream;                    // Success: report in exit addr of redirected stream
                       }
              
                   
              }     // End or (if !ErrorInStreamDealloc)
                 
        }
  }
  
  return ret;
} 


FILE *fdopen (int fd, char *mode)
/*
**
**  [func] - fdopen.
**  [desc] - produces a file descriptor of type `FILE *', from a 
**           descriptor for an already-open file (returned, for 
**           example, by the system subroutine `open' rather than by `fopen').
**           The MODE argument has the same meanings as in `fopen'.
**  [entr] - int fd; file descriptor returned by 'open'.
**           const char *mode; the file mode string pointer.
**  [exit] - file pointer or `NULL', as for `fopen'.
**
**
**  Per quanto riguarda mode, sotto PSPE si applicano le medesime regole già
**  viste per fopen. 
*/
{
  FILE *ret = NULL;
  int flag = 0, i, iomode = 0;

  int  Counter;
  char Carattere;
  char _ReadMode, _WriteMode, _AppendMode;
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
    if ((mode != NULL) && (*mode != '\0')) 
    {
       _ReadMode=0;
       _WriteMode=0;
       _AppendMode=0;       
              
       for (Counter=0; Counter<(strlen (mode)); Counter++)
       {
           Carattere = Magn (mode [Counter]);
       
           switch (Carattere)
           {
                  case 'R':
                       {
                           _ReadMode=1;
                           break;
                       }
                  
                  case 'W':
                       {
                           _WriteMode=1;
                           break;
                       }
                  
                  case 'A':
                       {
                           _AppendMode=1;
                           break;
                       }
           
                  // I simboli +,b,t vengono ignorati
           }
       }       
       
       if (_AppendMode)         // Se viene indicato l'appending, viene ignorato qualsiasi 
       {                        // altro simbolo
            
               // Se c'è già un virtual file handle aperto, vuol dire che il file già
               // esiste                 
            
               flag = _IOWRT;
               iomode = (O_WRONLY | O_APPEND);
               
       }
       else   // Non è avviata la modalità di appending
       {
            if ((_ReadMode) && (_WriteMode))    // Attiva modalità di lettura/scrittura
            {
                flag = _IOREAD & _IOWRT;
                iomode = (O_RDWR | O_CREAT | O_TRUNC);
            }
            else       // Non è vero che è attiva la modalità di lettura/scrittura
            {
                if (_ReadMode)
                {
                   flag = _IOREAD;
                   iomode = O_RDONLY;  
                }
       
                if (_WriteMode)
                {
                   flag = _IOWRT;
                   iomode = (O_WRONLY | O_CREAT | O_TRUNC); 
                }
            }
       }
       
       
       // Assegniamo un opportuno stream 
       
       for (i = 3; i < _NFILE; ++i)            // Si inizia il conteggio da zero per saltare stdin, stdout, stderr
       {
           if (StreamsArray [i].fd < 0) break;
       }
      
       if (i < _NFILE)                        // Abbiamo trovato un handle disponibile
       {
         if (fd >= 0) 
         {
           StreamsArray [i].type = STD_IOBUF_TYPE_MS;
           StreamsArray [i].fd   = fd;
           StreamsArray [i].cnt  = 0;
           StreamsArray [i].flag = flag;
           StreamsArray [i].has_putback = 0;
           ret = &(StreamsArray[i]);
         }
       }
      
    }
  
    return ret;
}



int fclose (FILE *stream)
{
  int ret;
  int ErrRep;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  /* test the file stream type. */
  switch (stream->type) 
  {
        case STD_IOBUF_TYPE_NONE:
        case STD_IOBUF_TYPE_GE:
        case STD_IOBUF_TYPE_STDOUTHOST:
          /* cannot close stdin, stdout, or stderr. */
          // duh.. this is wrong. One SHOULD be able to close
          // std*. That's a common unix doing. However, I doubt
          // allowing this madness could be a good idea.
          ret = EOF;
          break;
        default:
                {
                    if (stream->fd >=0)
                    {
                       ErrRep = close (stream->fd);
                    
                       if (ErrRep >= 0)  
                       {
                          stream->type = STD_IOBUF_TYPE_NONE;
                          stream->fd = -1;
                          stream->cnt = 0;
                          stream->flag = 0;
                          ret = 0;
                       }
                       else ret = EOF;
                    }
                    else ret = EOF;
                }
          
  }
  
  return (ret);
}





/*
**
**  [func] - fcloseall.
**  [desc] - attempts to close all the open files. if able to close all the open
**           files then returns the number of files closed. else returns -1.
**  [entr] - none.
**  [exit] - int; the number of files closed if successful. else -1.
**  [prec] - none.
**  [post] - all open non-system files are closed.
**
*/
int fcloseall(void)
{
  int  i, ret = 0;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  /* process all open files except for stdout, stdin and stderr. */
  for (i = 3; i < _NFILE; ++i) 
  {
      if (StreamsArray [i].fd >= 0) 
      {
         /* attempt to close the current file. */
         if (  (fclose (  (FILE *)(&StreamsArray [i])  )==0) && (ret >= 0)  ) 
            ++ret;
         else 
            ret = EOF;
      }
  }
  
  return (ret);
}


size_t fread (void *buf, int r, int n, FILE *stream)
/*
    [func] - fread
    [desc] - tenta di legge un numero n di record di size r dal file stream
             e ritorna il numero di records che sono stati letti con successo.
             
    [entr] - Quando il file stream è diverso da stdin, il significato dei 
             parametri è il seguente:
                       
                void *buf; the pointer to the destination data buffer.
                size_t r; the size of the records to read.
                size_t n; the number of records to read.
                FILE *stream; the pointer to the FILE stream. 

             Le cose cambiano radicalmente quando il file stream è pari a stdin.
             
             In tal caso, i valori di n e di r contengono due informazioni: una 
             è legata al segno, e l'altra al valore assoluto. 
             
             Se il segno di n è positivo, allora il sistema richiama la Virtual
             Keyboard con la funzione di AutoClose attivata.
             
             Se il segno di n è negativo, allora il sistema richiama la Virtual
             Keyboard con la funzione di AutoClose disattivata e sarà necessario
             che la routine chiamante usi la funzione ndCloseSystemKeyboard
             per chiudere la finestra della tastiera. 
             
             Il valore assoluto di n indica sempre il numero di records da
             leggere. 
             
             Anche il valore di r contiene due informazioni.
             
             Se il segno di r è positivo, allora la funzione read eseguirà un
             echo su stdin di ogni carattere immesso dall'utente.
             
             Se il segno di r è negativo, allora non vi sarà alcun echo.
             
    [exit] - size_t; il numero di records letti con successo dal file stream
    [prec] - buf è un puntatore di memoria valido di almeno (r*n) bytes di dimensione
             e stream è un puntatore di tipo FILE valido
    [post] - il file stream viene modificato 
*/
{
    int IoReport, ret;
    int abs_r, abs_n;
    int Counter;
    int IO_Char;
    
    char EchoActive;
    char *PointerToChar;
    
    if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
    // Richiama automaticamente la funzione di reinizializzazione del file-system
    // qualora essa non sia già stata utilizzata
  
    if (stream!=stdin)
    {
       if ( (n>=0) && (r>0) )     // Controlla che n ed r siano positivi. Un n o un r negativo non ha significato
       {                          // quando lo stream è diverso da stdin
       
          switch (stream->type) 
          {
                 case STD_IOBUF_TYPE_NONE:
                 case STD_IOBUF_TYPE_GE:
                 case STD_IOBUF_TYPE_STDOUTHOST:
                      // cannot read from stdout or stderr. 
                      ret = 0;
                      break;
                 default:
                      // attempt to read from the stream file. 
                      {
                         IoReport = read (stream->fd, buf, r*n);    
      
                         if (IoReport>=0)
                         {
                            if ( IoReport == (r*n) )
                            {     
                                  ret = (IoReport / r);
                                  stream->flag = (stream->flag & (~_IOERR));
                                  errno = 0;
                            }                  
                            else               // Non è stato possibile leggere tutto
                            { 
                                  ret = (IoReport / r);        // segnala ugualmente il numero di records letti
                                  stream->flag = (stream->flag | _IOERR);
                                  errno = 5;                   // segnala errore di I/O
                            }
                         }
                         else     // IoReport è negativo. Errore disco.
                         {
                            ret=0;  
         
                            stream->flag = (stream->flag | _IOERR);
                            errno = 5;                   // segnala errore di I/O
                         }
           
                         if (VFH [stream->fd].PosSeek32 == VFH [stream->fd].FileLength)  // Verifica se è l'ultimo carattere
                            stream->flag = (stream->flag | _IOEOF);  
                         else  
                            stream->flag = (stream->flag & (~_IOEOF));
                      }
           }                    // End of switch
         
           return (ret);
         }
         else               // n ha un valore negativo. Condizione di errore.
         {
           stream->flag = (stream->flag | _IOERR);
           errno = 5;                   
         
           return 0;       // Simula un errore di I/O per la routine chiamante 
         }     
    }                       // End of if stream!=stdin
    else       
    {
       if (n>=0)
       {
          ndSET_SystemKeyboardAutoClose (1);
          // Un valore positivo di n indica la necessità di usare l'autoclose per
          // la tastiera di sistema
          
          abs_n = n;
       }
       else
       {
          ndSET_SystemKeyboardAutoClose (0);
          // Un valore negativo di n indica la necessità di disattivare l'autoclose
          
          abs_n = (-1)*n;
       }
       
       if (r>=0)
       {
          EchoActive=1;
          ndActivateStdOutDialogBox ();
          // Attiva la StdOut DialogBox prima dell'apparizione della tastiera virtuale.
          // Questo migliora il comportamento grafico.
          
          abs_r = r;
       }
       else
       {
          EchoActive=0;
          abs_r = (-1)*r;
       }
    
       
       Counter=0;     
       
       while (Counter<abs_r*abs_n)
       {         
          IO_Char = ndGetChar (); // Preleva un carattere con la VirtualKeyboard. Se al momento della
                                  // scelta del carattere avviene la chiusura della VirtualKeyboard,
                                  // viene deciso dalla variabile SystemKeyboardAutoClose.
    
                                  // Ricordiamo che ndGetChar restituisce un valore pari a -1 in caso
                                  // di errore nell'allocazione della finestra, 8 per backspace, /n per
                                  // enter, e 27 per ESC. 
    
          if (IO_Char>=0)         // Se non ci sono errori
          {
             if (EchoActive) putchar (IO_Char);
             // Esegui l'echo su console se necessario
             
             PointerToChar  = buf + Counter;
             *PointerToChar = (char)IO_Char;
             
             (stdin)->flag = (stdin)->flag & (~_IOEOF);       // Pone a 0 il flag IOEOF di stdin
             Counter++;   
          }
          else
          {
             (stdin)->flag = (stdin)->flag | (_IOEOF);        // Setta IOEOF di stdin
             break;                                           // Esci dal ciclo while
          }
       
       }                                       // End of while   
    
       return Counter;  
    }                   // End of else stream==stdin
}





size_t fwrite (const void *buf, size_t r, size_t n, FILE *stream)
/*
**
**  [func] - fwrite.
**  [desc] - attempts to write n number of records of r size to the stream file
**           and returns the number of records successfully written to the file.
**  [entr] - const void *buf; the pointer to the source data buffer.
**           size_t r; the size of the records to write.
**           size_t n the number of records to write.
**           FILE *stream; the pointer to the FILE stream.
**  [exit] - size_t; the number of records successfully written to the stream file.
**  [prec] - buf is a valid memory pointer of (r * n) size in bytes and stream
**           is a valid FILE pointer.
**  [post] - the stream file is modified.
**
*/
{
  size_t ret, IoReport;
  int CounterA, CounterB;
  
  char *PointerToRam;
  char TypeChannel;
  char Result;
  
  int VFH_ID;
  
  size_t CharWritten;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if ((n>=0) && (r>0))     // Controlla la validità dei parametri
  {
     if ((stream==stdout) || (stream==stderr))
     {
        if (stream==stdout) TypeChannel=0;
        if (stream==stderr) TypeChannel=1;                  
                       
        CharWritten=0;
                           
        for (CounterA=0; CounterA<n; CounterA++)
        {   
            for (CounterB=0; CounterB<r; CounterB++)
            {
                PointerToRam=(char *)(buf)+r*CounterA+CounterB;
      
                Result=fwrite_to_stdout (*PointerToRam, TypeChannel);
                // Scrive il carattere in stdout.
         
                if (Result==0) CharWritten++;    // Se non ci sono errori incrementa CharWritten
            }
        }
     
        ndStdOutRender ();
        return CharWritten;
                      
     }
     else
     {
         /* attempt to write the real stream file. */
         IoReport = write ( stream->fd, (void *)buf, r*n );
      
         VFH_ID=stream->fd;
      
         if (IoReport>=0)
         {
            if ( IoReport == (r*n) )
            {     
               ret = (IoReport / r);
               stream->flag = (stream->flag & (~_IOERR));
               errno = 0;
            }                  
            else               // Non è stato possibile scrivere tutto
            {
               ret=(IoReport / r);   // Segnala comunque il numero di records scritti
               stream->flag = (stream->flag | _IOERR);
               errno = 5;                   // segnala errore di I/O
            }
         }
         else     // IoReport è negativo. Errore disco.
         {
            ret=0;
            stream->flag = (stream->flag | _IOERR);
            errno = 5;                   // segnala errore di I/O  
         }
      
         if (VFH [VFH_ID].PosSeek32 == VFH [VFH_ID].FileLength - 1)  // Verifica se è l'ultimo carattere
            stream->flag = (stream->flag | _IOEOF);  
         else
            stream->flag = (stream->flag & (~_IOEOF));
  
         return (ret);
     }
  }
  else   // I parametri r o n sono errati
  {
     ret=0;                       // Segnala comunque il numero di records scritti
     stream->flag = (stream->flag | _IOERR);
     errno = 5;                   // segnala errore di I/O
  
     return -1;
  }
}

/*
    --------------------------------------------------------------------------
    Funzioni Get
    --------------------------------------------------------------------------
*/


int fgetc (FILE *stream)
/*
**
**  [func] - fgetc.
**  [desc] - attempts to read one character from the stream file. if able to
**           read one character from the file then returns the chaaracter
**           read. else EOF.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - int; the character read from the stream file. else -1.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream file is modified.
**
** Nota: questa funzione tende ad usare l'AutoClose per la VirtualKeyboard, quindi
** è adatta per i casi in cui deve essere immesso un singolo carattere.
**
** Quando si vuole immettere un insieme di n caratteri distinti, conviene usare
** fgets.
*/

{
  unsigned char c;
  int  ret;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if (stream->has_putback) 
  {
    stream->has_putback = 0;
    return stream->putback;
  }

  switch (stream->type) 
  {
    case STD_IOBUF_TYPE_GE:
    case STD_IOBUF_TYPE_STDOUTHOST:
      /* cannot read from stdout or stderr. */
      ret = EOF;
      break;
    default:
      {            
         ret = ((fread (&c, 1, 1, stream) == 1) ? (int)c : EOF);
      
         // Si noti che, qualora lo stream fosse stdin, il fatto che il terzo
         // parametro sia positivo implica che la funzione di AutoClose sarà
         // attivata. Questa routine è utile quando si vogliono raccogliere
         // singoli caratteri.
         // Inoltre, il fatto che il secondo parametro sia positivo, implica
         // che la funzione di echo è attiva.
      }
  }
  
  return (ret);
}


int xgetc (FILE *stream)
/*
**
**  [func] - xgetc.
**  [desc] - Questa funzione è una delle funzioni non standard della libreria
**           NanoC. La funzione si occupa di ricavare un carattere dallo stream
**           FILE, esattamente come fgetc, ma con una importante differenza:
**           se lo stream è stdin, allora la funzione richiama la Virtual
**           Keyboard senza attivare la funzione autoclose.
**             
**           La funzione riporta in uscita il carattere letto. Se non è possibile
**           leggere un carattere, riporta in uscita EOF.
**
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - int; the character read from the stream file. else -1.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream file is modified.
**
*/

{
  char c;
  int  ret;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if (stream->has_putback) 
  {
    stream->has_putback = 0;
    return stream->putback;
  }

  switch (stream->type) 
  {
    case STD_IOBUF_TYPE_GE:
    case STD_IOBUF_TYPE_STDOUTHOST:
      /* cannot read from stdout or stderr. */
      ret = EOF;
      break;
    default:
      {            
         if (stream!=stdin)
            ret = ((fread (&c, 1, 1, stream) == 1) ? (int)c : EOF);
         else
            ret = ((fread (&c, 1, -1, stream) == 1) ? (int)c : EOF);
      
         // Si noti che, qualora lo stream fosse stdin, il fatto che il terzo
         // parametro sia negativo implica che la funzione di AutoClose sarà
         // disattivata. Questa routine è utile quando si vogliono raccogliere
         // singoli caratteri.
         // Inoltre, il fatto che il secondo parametro sia positivo, implica
         // che la funzione di echo è attiva.
      }
  }
  return (ret);
}




char *fgets (char *buf, int n, FILE *stream)
/*
**
**  [func] - fgets.
**  [desc] - attempts to read a string from the stream file. if able to read
**           a string from the stream file stdin then stores the string up to
**           n characters to the memory pointed by buf and returns buf. else
**           returns NULL.
**  [entr] - char *buf; the pointer to the destination string buffer.
**           int n; the maximum number of characters to write to buf.
**           FILE *stream; the pointer to the FILE stream.
**  [exit] - char *; buf if the string is read successfully. else NULL.
**  [prec] - buf is a valid memory pointer of n size in bytes and stream is a
**           valid FILE pointer.
**  [post] - the memory pointed to by buf is modified and the stream file
**           pointer is modified.
**
*/
{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if ((stream==stderr) || (stream==stdout) || !(stream->flag & _IOREAD) || (n<=0) )
  {
      return NULL;                 
  }
  
  if (stream==stdin)     // Acquisizione da stdin
  {
      int  c;
      char *cp;
   
      n--;
      for (c = 0, cp = buf; (c!='\n') && (n>0); n--, cp++) 
      {
	     c = ((fread (&c, 1, -1, stream) == 1) ? (int)c : EOF);
	     // Qui verrà preso un carattere con la funzione di AutoClose della Virtual Keyboard 
         // disattivata
         
         if (c == EOF) 
         {
            ndCloseSystemKeyboard ();    
            return NULL;
         }
	     
         *cp = (char) c;
      }

      *cp = '\0';
     
      ndCloseSystemKeyboard ();
      // Chiude la SystemKeyboard alla fine del ciclo di acquisizione. 
      
      return buf;             
  }
  
  // Acquisizione da uno stream normale
  
  {
      int c = 0;
      char* cs;
    
      cs = buf;
      
      while (--n>0 && (c = getc(stream)) != EOF) 
      {
        *cs++ = c;
        if (c == '\n') break;
      }
      
      if (c == EOF && cs == buf) return NULL;

      *cs++ = '\0';              
      return buf;              
  } 
}


char *fgets0D (char *buf, int n, FILE *stream)
/*
**
**  [func] - fgets0D.
**  [desc] - attempts to read a string from the stream file. if able to read
**           a string from the stream file stdin then stores the string up to
**           n characters to the memory pointed by buf and returns buf. else
**           returns NULL.
**           This routine is a variance of fgets. Some applications, like 
**           ISpell, need a version of fgets that skippes automatically
**           the char 0x0D.
**  [entr] - char *buf; the pointer to the destination string buffer.
**           int n; the maximum number of characters to write to buf.
**           FILE *stream; the pointer to the FILE stream.
**  [exit] - char *; buf if the string is read successfully. else NULL.
**  [prec] - buf is a valid memory pointer of n size in bytes and stream is a
**           valid FILE pointer.
**  [post] - the memory pointed to by buf is modified and the stream file
**           pointer is modified.
**
*/
{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if ((stream==stderr) || (stream==stdout) || !(stream->flag & _IOREAD) || (n<=0) )
  {
      return NULL;                 
  }
  
  if (stream==stdin)     // Acquisizione da stdin
  {
      int  c;
      char *cp;
   
      n--;
      for (c = 0, cp = buf; (c!='\n') && (n>0); n--, cp++) 
      {
	     c = ((fread (&c, 1, -1, stream) == 1) ? (int)c : EOF);
	     // Qui verrà preso un carattere con la funzione di AutoClose della Virtual Keyboard 
         // disattivata
         
         if (c == EOF) 
         {
            ndCloseSystemKeyboard ();    
            return NULL;
         }
	     
         *cp = (char) c;
      }

      *cp = '\0';
     
      ndCloseSystemKeyboard ();
      // Chiude la SystemKeyboard alla fine del ciclo di acquisizione. 
      
      return buf;             
  }
  
  // Acquisizione da uno stream normale
  
  {
      int c = 0;
      char* cs;
    
      cs = buf;
      
      while (--n>0 && (c = getc(stream)) != EOF) 
      {
            if (c!=0x0D)
            {
                *cs++ = c;
                if (c == '\n') break;
            }
      }
      
      if (c == EOF && cs == buf) return NULL;

      *cs++ = '\0';              
      return buf;              
  }
}



char *gets (char *buf)
/*
**
**  [func] - gets.
**  [desc] - attempts to read a string from stdin file. The number of
             chars is not limited. If there are no errors i returns buf,
             else returns NULL. The read process will be arrested only
             when the stdin char will be '/n'
**  [entr] - char *buf; the pointer to the destination string buffer.
**  [exit] - char *; buf if the string is read successfully. else NULL.
**  [prec] - buf is a valid memory pointer of n size in bytes. It is user's
             responsability to ensure that buffer has sufficient space to
             accept new chars
**  [post] - the memory pointed to by buf is modified and the stream file
**           pointer is modified.
**
*/
{
    char *ret = buf;
    int  c;
    char *cp;

    if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
    // Richiama automaticamente la funzione di reinizializzazione del file-system
    // qualora essa non sia già stata utilizzata
         
    for (c = 0, cp = buf; c != '\n'; cp++) 
    {
        if ((c = xgetc (stdin)) == EOF) return NULL;
        // Grazie a questa istruzione ci assicuriamo che se lo stream è stdin, verrà preso
        // un carattere con la funzione di AutoClose della Virtual Keyboard disattivata 
        // Se invece lo stream non è stdin il comportamento sarà normale.
         
        *cp = (char) c;
    }
    
    *cp = '\0';
    
    ndCloseSystemKeyboard ();
    // Se lo stream è sempre stdin.  
    // Quindi chiude sempre la SystemKeyboard alla fine del ciclo di acquisizione.

    return (ret);
}


int getc (FILE *stream)
/*
**
**  [func] - getc.
**  [desc] - attempts to read one character from the stream file. if able to
**           read one character from the file then returns the chaaracter
**           read. else EOF.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - int; the character read from the stream file. else -1.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream file is modified.
**
*/
{
  unsigned char c;
  int  ret;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  switch (stream->type) 
  {
    case STD_IOBUF_TYPE_GE:
    case STD_IOBUF_TYPE_STDOUTHOST:
      /* cannot read from stdout or stderr. */
      ret = EOF;
      break;
    default:
      {            
         ret = ((fread (&c, 1, 1, stream) == 1) ? (int)c : EOF);
      
         // Si noti che, qualora lo stream fosse stdin, il fatto che il terzo
         // parametro sia positivo implica che la funzione di AutoClose sarà
         // attivata. Questa routine è utile quando si vogliono raccogliere
         // singoli caratteri.
         // Inoltre, il fatto che il secondo parametro sia positivo, implica
         // che la funzione di echo è attiva.
      }
  }
   
  return (ret);

}

int getchar(void)
{
    return getc (stdin);
}


/* 
   ----------------------------------------------------------------------------
   Funzioni Put
   ----------------------------------------------------------------------------
*/





int fputc (int c, FILE *stream)
/*
**
**  [func] - fputc.
**  [desc] - attempts to write the c character to the stream file. if able to
**           write the character to the stream file then returns the character
**           written. else returns -1.
**  [entr] - int c; the character to write to the file.
**           FILE *stream; the pointer to the FILE stream.
**  [exit] - int; the character written to the file if successful. else -1.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream file is modified.
**
*/
{
  char ch;

  ch = (char)c;
  return ((fwrite(&ch, 1, 1, stream) == 1) ? 0 : EOF);
}


int fputs (const char *s, FILE *stream)
/*
**
**  [func] - fputs.
**  [desc] - attempts to write the s string to the stream file. if able to
**           successfully write the string to the stream file then returns
**           the number of characters written to the file. else returns -1.
**  [entr] - const char *s; the source string pointer.
**  [exit] - int; the number of chars. written to file if successful. else -1.
**  [prec] - stream is a valid FILE pointer and s is a valid string pointer.
**  [post] - the stream file is modified.
**
*/
{
  size_t len, result;

  len = strlen (s);
    
  result=fwrite (s, len, 1, stream);

  if (result!=EOF)
  {
    fputc('\n', stream);
  }
  
  return result+1; 
}
   




int putc(int c, FILE *stream)
/*
**
**  [func] - putc.
**  [desc] - attempts to write the c character to the stream file. if able to
**           write the character to the stream file then returns the character
**           written. else returns -1.
**  [entr] - int c; the character to write to the file.
**           FILE *stream; the pointer to the FILE stream.
**  [exit] - int; the character written to the file if successful. else -1.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream file is modified.
**
*/
{
  char ch;
  int  ret = 0;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  switch (stream->type) 
  {
      break;
    default:
      /* write one character to the stream file. */
      ch = (char)c;
      ret = ((fwrite(&ch, 1, 1, stream) == 1) ? c : EOF);
  }
  
  return (ret);
}






int puts (char *s)
/*
**
**  [func] - puts.
**  [desc] - attempts to write the s string to stdout. if able to write the s
**           string to stdout then returns the number of characters written.
**           else returns -1.
**  [entr] - const char *s; the source string pointer.
**  [exit] - int; the number of characters written to stdout. else -1.
**  [prec] - s is a valid string pointer.
**  [post] - the stdout file stream is modified.
**
*/
{
  return fputs (s, stdout);
}


int putchar(int c)
/*
**
**  [func] - putchar.
**  [desc] - attempts to write the c character to stdout. if able to write
**           the character to stdout then returns the character written.
**           else returns -1.
**  [entr] - int c; the character to write to stdout.
**  [exit] - int; the character written to stdout. else -1.
**  [prec] - none.
**  [post] - the stdout file stream is modified.
**
*/
{
  return (putc(c, stdout));
}


/* 
   ---------------------------------------------------------------------------
   Funzioni seek/tell
   ---------------------------------------------------------------------------
*/


int fseek(FILE *stream, long offset, int origin)
/*
**
**  [func] - fseek.
**  [desc] - attempts to seek the stream file pointer to offset from origin.
**           if able to seek the stream file pointer to offset from origin
**           returns 0. else returns -1.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**           long offset; the seek offset.
**           int origin; the seek origin.
**  [exit] - int; 0 if able to seek to offset from origin successfully. else -1.
**  [prec] - stream is a valid FILE pointer and origin is a valid seek origin
**           type.
**  [post] - the stream file pointer position is modified.
**
*/
{
  int ret, seekret;
  int VFH_ID;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  switch (stream->type) 
  {
    case STD_IOBUF_TYPE_NONE:
    case STD_IOBUF_TYPE_GE:
    case STD_IOBUF_TYPE_STDOUTHOST:
      /* cannot seek stdout or stderr. */
      ret = -1;
      break;
    default:
      {
        seekret = lseek(stream->fd, (int)offset, origin);
        
        if (seekret>=0)                // E' stato possibile eseguire il seek
        {
           ret=0;                      // Setta variabile di ritorno a 0.
           
           VFH_ID=stream->fd; 
           stream->flag = (stream->flag & (~_IOERR) );
        
           if (VFH [VFH_ID].PosSeek32 == VFH [VFH_ID].FileLength)  // Verifica se è l'ultimo carattere
              stream->flag = (stream->flag | _IOEOF);  
           else
              stream->flag = (stream->flag & (~_IOEOF));     
        }
        else
        {
           ret=-1;
           
           stream->flag = (stream->flag | _IOERR);
           errno = 5;                   // segnala errore di I/O
        }
        
      }
  }
  
  return (ret);
}


/*
**  [func] - ftell.
**  [desc] - attempts to retrieve the stream file stream pointer position.
**           if able to retrieve the stream file stream pointer position
**           then returns the position. else sets error code and returns -1.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - long; the stream file pointer position if successful. else -1.
**  [prec] - stream is a valid FILE pointer.
**  [post] - none.
*/
long ftell(FILE *stream)
{
  long n, ret;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  switch (stream->type) 
  {
    case STD_IOBUF_TYPE_NONE:
    case STD_IOBUF_TYPE_GE:
    case STD_IOBUF_TYPE_STDOUTHOST:
      /* stdout or stderr is an invalid seek stream argument. */
      errno = EINVAL;
      ret = -1L;
      break;
    default:
      if (stream->fd < 0) 
      {
        /* file is not open. */
        errno = EBADF;
        ret = -1L;
      }
      else
      { 
        n = tell (stream->fd);
        
        if (n>=0)
           ret = (long) n;
        else
           ret = -1L; 
        
      }
  }
  return (ret);
}


int fgetpos (FILE *stream, fpos_t *pos)
/*
**
**  [func] - fgetpos.
**  [desc] - attempts to retrieve the stream file stream pointer position.
**           if able to retrieve the stream file stream pointer position
**           then stores the position to pos and returns 0. else returns -1.
**  [entr] - FILE *stream; the pointer to the file stream.
**           fpos_t *pos; the pointer to the destination file position buffer.
**  [exit] - int; 0 if able to retrieve the stream file pointer position. else -1.
**  [prec] - stream is a valid FILE pointer and pos is a valid fpos_t pointer.
**  [post] - the memory pointed to by pos is modified.
**
*/
{
  long n;

  n = tell (stream->fd);

  if (n >= 0) *pos = (fpos_t)n;
  return ((n >= 0) ? 0 : -1);
}

int fsetpos (FILE *stream, fpos_t *pos)
/*
**  [func] - fsetpos.
**  [desc] - attempts to set the stream file pointer position to the pos offset.
**           if able to set the stream file pointer position to pos then returns
**           0. else returns -1.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**           const fpos_t *pos; the pointer to the source file position buffer.
**  [exit] - 0 if able to set the stream file pointer position. else -1.
**  [prec] - stream is a valid FILE pointer and pos is a valid fpos_t pointer.
**  [post] - the stream file pointer position is modified.
*/
{
  return (fseek (stream, *pos, SEEK_SET));
}



void rewind(FILE *stream)
/*
**
**  [func] - rewind.
**  [desc] - resets the stream file pointer to 0.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - none.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream file pointer is modified.
**
*/
{
  fseek(stream, 0, SEEK_SET);
}




int ungetc (int c, FILE *stream)
/*
**
**  [func] - ungetc.
**  [desc] -
**  [entr] - int c;
**           FILE *stream; the pointer to the FILE stream.
**  [exit] - int;
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream FILE stream is modified.
**
*/
{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if (c == EOF || stream->has_putback) 
  {
    /* invalid input, or putback queue full */
    return EOF;
  }

  stream->putback = (u8)c;
  stream->has_putback = 1;
  return c;
}



char *tmpnam(char *name)
/*
**
**  [func] - tmpnam.
**  [desc] - creates a temporary filename string, 
**  [entr] - char *name; the pointer to the destination string pointer.
**  [exit] - char *;
**  [prec] -
**  [post] -
**
*/
{
  char *ret = NULL;

  return (ret);
}


FILE *tmpfile (void)
/*
**
**  [func] - tmpfile.
**  [desc] - attempts to create a temporary file. if able to create a temporary
**           file then returns the pointer to the FILE stream. else returns NULL.
**  [entr] - none.
**  [exit] - FILE *; the ptr. to the opened temp. file if successful. else NULL.
**  [prec] - none.
**  [post] - a temporary is opened.
**
*/
{
  FILE *retcode;
  
  retcode = fopen( (char *)(&__stdio_tmpnam), "w+");
  return retcode;
}


int fileno (FILE * f) 
{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  return f->fd;
}




/*
**
**  [func] - feof.
**  [desc] - if the stream file stream has reached the end of the file then
**           returns non-zero. else returns 0.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - int; non-zero if the stream file has reached EOF. else 0.
**  [prec] - stream is a valid FILE pointer.
**  [post] - none.
**
*/
int feof(FILE *stream)
{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
    
  return ((stream->flag & _IOEOF) != 0);
}



/*
**
**  [func] - ferror.
**  [desc] - if an error has occured for the stream file stream then returns
**           non-zero. else returns 0.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - int; non-zero if error has occured for the stream file. else 0.
**  [prec] - stream is a valid FILE pointer.
**  [post] - none.
**
*/
int ferror(FILE *stream)
{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
    
  return ( (stream->flag & _IOERR) != 0);
}


static const char * file_errors[] = {
"",										//	0
"Not super-user",								//	1
"No such file or directory",						//	2
"No such process",							//	3
"Interrupted system call",						//	4
"I/O error",								//	5
"No such device or address",						//	6
"Arg list too long",							//	7
"Exec format error",							//	8
"Bad file number",							//	9
"No children",								//	10
"No more processes",							//	11
"Not enough core",							//	12
"Permission denied",							//	13
"Bad address",								//	14
"Block device required",						//	15
"Mount device busy",							//	16
"File exists",								//	17
"Cross-device link",							//	18
"No such device",								//	19
"Not a directory",							//	20
"Is a directory",								//	21
"Invalid argument",							//	22
"Too many open files in system",					//	23
"Too many open files",							//	24
"Not a typewriter",							//	25
"Text file busy",								//	26
"File too large",								//	27
"No space left on device",						//	28
"Illegal seek",								//	29
"Read only file system",						//	30
"Too many links",								//	31
"Broken pipe",								//	32
"Math arg out of domain of func",					//	33
"Math result not representable",					//	34
"No message of desired type",						//	35
"Identifier removed",							//	36
"Channel number out of range",					//	37
"Level 2 not synchronized",						//	38
"Level 3 halted",								//	39
"Level 3 reset",								//	40
"Link number out of range",						//	41
"Protocol driver not attached",					//	42
"No CSI structure available",						//	43
"Level 2 halted",								//	44
"Deadlock condition",							//	45
"No record locks available",						//	46
"",										//	47
"",										//	48
"",										//	49
"Invalid exchange",							//	50
"Invalid request descriptor",						//	51
"Exchange full",								//	52
"No anode",									//	53
"Invalid request code",							//	54
"Invalid slot",								//	55
"File locking deadlock error",					//	56
"Bad font file fmt",							//	57
"Bad symbolic file name",						//	58
"",										        //	59
"Device not a stream",							//	60
"No data (for no delay io)",						//	61
"Timer expired",								//	62
"Out of streams resources",						//	63
"Machine is not on the network",					//	64
"Package not installed",						//	65
"The object is remote",							//	66
"The link has been severed",						//	67
"Advertise error",							//	68
"Srmount error",								//	69
"Communication error on send",					//	70
"Protocol error",								//	71
"",										//	72
"",										//	73
"Multihop attempted",							//	74
"Inode is remote (not really error)",				//	75


"Cross mount point (not really error)",				//	76
"Trying to read unreadable message",				//	77
"",										//	78
"Inappropriate file type or format",				//	79
"Given log. name not unique",						//	80
"f.d. invalid for this operation",					//	81
"Remote address changed",						//	82
"Can't access a needed shared lib",					//	83
"Accessing a corrupted shared lib",					//	84
".lib section in a.out corrupted",					//	85
"Attempting to link in too many libs",				//	86
"Attempting to exec a shared library",				//	87
"Function not implemented",						//	88
"No more files",								//	89
"Directory not empty",							//	90
"File or path name too long",						//	91
"Too many symbolic links",						//	92
"Out of memory",										//	93
"",										//	94
"Operation not supported on transport endpoint",		//	95
"Protocol family not supported",					//	96
"",										//	97
"",										//	98
"",										//	99
"",										//	100
"",										//	101
"",										//	102
"",										//	103
"Connection reset by peer",						//	104
"No buffer space available",						//	105
"Address family not supported by protocol family",		//	106
"Protocol wrong type for socket",					//	107
"Socket operation on non-socket",					//	108
"Protocol not available",						//	109
"Can't send after socket shutdown",					//	110
"Connection refused",							//	111
"Address already in use",						//	112
"Connection aborted",							//	113
"Network is unreachable",						//	114
"Network interface is not configured",				//	115
"Connection timed out",							//	116
"Host is down",								//	117
"Host is unreachable",							//	118
"Connection already in progress",					//	119
"Socket already connected",						//	120
"Destination address required",					//	121
"Message too long",							//	122
"Unknown protocol",							//	123
"Socket type not supported",						//	124
"Address not available",						//	125
"",										//	126
"Socket is already connected",					//	127
"Socket is not connected",						//	128
"",										//	129
"EPROCLIM",									//	130
"EUSERS",									//	131
"EDQUOT",									//	132
"ESTALE",									//	133
"Not supported",								//	134
"No medium (in tape drive)",						//	135
"No such host or network path",					//	136
"Filename exists with different case",				//	137
"EILSEQ",									//	138
"Value too large for defined data type",				//	139
""};


char * strerror (int err) 
{
    return (char *)(&file_errors [err]);
}


/*
**
**  [func] - perror.
**  [desc] - if there is a current error then prints the corresponding error
**           and then prints s to stderr. else prints s to stderr.
**  [entr] - const char *s; the error string pointer.
**  [exit] - none.
**  [prec] - s is a valid string pointer.
**  [post] - none.
**
*/
void perror (char *s)
{
  char *err;

  /* print to stderr output. */
  if ((err = strerror(errno)) != NULL) fprintf(stderr, "%s : ", err);
  
  fputs(s, stderr);
}


void clearerr(FILE *stream)
{
  stream->flag &= (~_IOERR);
}



int fflush(FILE *stream)
/*
**
**  [func] - fflush.
**  [desc] - if the stream file is opened as read-only then returns 0. else
**           if stream is a valid FILE stream and able to flush the stream
**           file write buffer then returns 0. else returns EOF.
**  [entr] - FILE *stream; the pointer to the FILE stream.
**  [exit] - int; 0 if able to flush the write buffer or file is read-only. else EOF.
**  [prec] - stream is a valid FILE pointer.
**  [post] - the stream FILE stream write buffer is flushed.
**
*/
{
  int VFHNumber, ErrRep;
      
  int ret           = EOF; // Same as default case below.

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  switch (stream->type) 
  {
    case STD_IOBUF_TYPE_GE:
    case STD_IOBUF_TYPE_STDOUTHOST:
      // stdout & stderr are never buffered. 
    case STD_IOBUF_TYPE_UMD:
      // cd-rom files are read-only so no write buffer to flush. 
      ret = 0;
  
      break;
    case STD_IOBUF_TYPE_MS:         // E' il caso dei file che vengono normalmente letti o scritti
      {
         if ((stream!=stdin) && (stream!=stdout) && (stream!=stderr))
         {                                                  
            if ( (stream->fd) >=0)   // Lo stream è già in uso
            {
                if (!((stream->flag && _IOWRT) == _IOWRT))        // Lo stream è aperto solo in lettura
                {
                   ret=0;
                }
                else     // Lo stream è aperto anche in scrittura. Eseguiamo il flush
                {
                   VFHNumber = stream->fd;                        // Estrae il valore del VFH 
                   
                   if ( (VFH [VFHNumber].PosSeek32 - VFH [VFHNumber].BeginOfBank)>0)
                   {
                      ErrRep=INTERNAL_SaveBank (VFHNumber, VFH [VFHNumber].NrBankLoaded);
                      // Scarica il buffer di scrittura
                      
                      if (ErrRep==0) ret=0;     // E' stato capace di eseguire il flush
                         else ret=EOF;  
                   }
                   else   // Non era necessario eseguire un flush: restituisci 0
                     ret=0;      
                }
            }
            else                 // Lo stream non era in uso, quindi deve ritornare EOF
              ret=EOF;
         }
         else         // Si sta tentando il flush su stdin, stdout, o stderr
           ret = EOF;
      
         break;
      }
    default:
      /* unknown/invalid I/O buffer type. */
      ret = EOF;
  }
  
  return (ret);
}


int fflushall (void)
/*
**
**  [func] - _fcloseall.
**  [desc] - attempts to flush all the open files with write-access. if able
**           to flush all the open files with write-access then returns the
**           number of files flushed. else returns -1.
**  [entr] - none.
**  [exit] - int; the number of files flushed if successful. else -1.
**  [prec] - none.
**  [post] - all open non-system files with write-access are flushed.
**
*/
{
  int  i, ret = 0;
  FILE *StreamsArrayElement;

  /* process all open files except for stdout, stdin and stderr. */
  for (i = 3; i < _NFILE; ++i) 
  {
    StreamsArrayElement = &(StreamsArray [i]);  
      
    if (StreamsArrayElement->fd >= 0) 
    {
      /* attempt to flush the current file. */
      
      if ((fflush (StreamsArrayElement) == 0) && (ret >= 0)) 
        ++ret;
      else 
        ret = EOF;
    }
  }
  
  return (ret);
}

int setvbuf(FILE *Stream, char *Buf, int Mode, size_t BufSize)
/*
   Le routine gestite da NanoC non supportano distinzioni tra 
   stream bufferizzati e non bufferizzati
   
   Di conseguenza, la routine si limita semplicemente a restituire
   un valore pari a 0 (tutto ok) in tutti i casi in cui viene 
   richiesto un mode bufferizzato, mentre viene restituito
   il valore -1 (errore) quando viene richiesto un meccanismo
   diverso di bufferizzazione.
*/
{
   if ((Stream->fd)>=0)         // Lo stream deve essere già attivo
   {
      if (Mode==_IOFBF)
         return 0;
   }
   
   return -1;   // Condizione di errore.
}



int setbuf (FILE *Stream, char *Buf)
{
    return setvbuf (Stream, Buf, Buf ? _IOFBF : _IONBF, 0);
}     



int devctl (const char *dev, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen)
/*
  Send a devctl command to a device.

  Example: Sending a simple command to a device (not a real devctl)
  sceIoDevctl("ms0:", 0x200000, indata, 4, NULL, NULL);

  Parameters:
  dev  - String for the device to send the devctl to (e.g. "ms0:")
  cmd  - The command to send to the device
  indata  - A data block to send to the device, if NULL sends no data
  inlen  - Length of indata, if 0 sends no data
  outdata  - A data block to receive the result of a command, if NULL receives no data
  outlen  - Length of outdata, if 0 receives no data

  Returns:
  0 on success, < 0 on error
*/

{
  return sceIoDevctl (dev, cmd, indata, inlen, outdata, outlen);
}



int	fstat  ( int VFH_Handle, struct stat *Stat )
/*
     Questa routine si occupa di eseguire l'operazione fstat. Essa preleva
     il nome del file dallo stream, ed esegue uno stat su tale nome. 

     Restituisce 0 in caso di successo e -1 in caso di errore. fd è il
     VFH handle del file. 
*/
{
    if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
    // Richiama automaticamente la funzione di reinizializzazione del file-system
    // qualora essa non sia già stata utilizzata
    
    return stat ( (const char *)(&(VFH [VFH_Handle].Name)), Stat);       
}


int	lstat ( const char *NameFile, struct stat *Stat )
{
    // Del tutto equivalente a stat sotto PSP
    
    return stat (NameFile, Stat);
}

/* 
   ---------------------------------------------------------------------------
   Funzioni per la gestione dei files temporanei
   ---------------------------------------------------------------------------
*/


int mkstemp (char* templat) 
{
  char LongName [256]; 
  char Path [256]; 
  char Name [64]; 
  char Ext  [32];
  
  char PreviousDir [512];
  
  char strbuffer0 [10];
  int  n,slen,nlen,nrfile,fd;
  
  int  LNResult;
  int  ChDirResult;
  
  slen = strlen (templat);
  
  if (slen>6)   
  {
      // Provvede ad espandere il nome che è stato fornito in modo da ottenere
      // un nome lungo
      
      LNResult = fssGenerateLongName ( (char *)(&SystemPath), templat, (char *)(&LongName) );
  
      if (LNResult==0)
      {
              // Adesso è necessario eseguire uno split del nome file in modo da isolare 
              // il nome della directory 
               
              fssSplitNameFile ( (char *)(&LongName), Path, Name, Ext);
              
              // Salva il nome della vecchia directory
              
              getcwd ( (char *)(&PreviousDir), 511);
              
              // Tenta un chdir per vedere se è possibile entrare nella directory richiesta
              
              ChDirResult = chdir ( (char *)(&Path) );
              
              if (ChDirResult==0)        // Il path esiste
              { 
                      // Ripristina la vecchia directory prima di continuare
                      
                      chdir ( (char *)(&PreviousDir) );
                       
                      // Esegui un controllo per verificare se gli ultimi caratteri del template
                      // sono costituiti solo dalla x
                      
                      for (n=slen-6; n<=slen-1; n++)
                      {
                          // Per il corretto funzionamento del sistema, è necessario controllare
                          // che gli ultimi sei caratteri siano XXXXXX. Tuttavia, in alcuni casi
                          // questo può creare problemi (se si usa mktemp "FILEXXXXXX", all'inizio
                          // funzionerà, ma poi non sarà più così perchè il sistema modifica il
                          // contenuto della ram nel punto dove è stata memorizzata la stringa dal
                          // compilatore. 
                      
                          // Per aggirare il problema, questa versione di mkstemp accetta anche 
                          // le cifre 0..9 se compaiono al posto delle X nel template
                          
                          // Questo passaggio della routine provvede anche a riempire di zeri 
                          // la regione dove è presente il pattern XXXXXX
                      
                          if ( (templat[n]=='X') || ( (templat[n]>='0') && (templat[n]<='9') ) ) 
                          {
                             templat[n]='0';  
                          }
                          else
                          {
                             errno=EINVAL; 
                             return -1; 
                          }
                      } 
                      
                      // Adesso provvede a generare un nuovo numero 
                      
                      for (nrfile=0; nrfile<999999; nrfile++)
                      {
                          _itoa (nrfile, (char *)(&strbuffer0), 10);
                          nlen = strlen (strbuffer0);
                          
                          // Adesso copia dentro template il numero che ci interessa
                          strcpy ( &(templat [slen-nlen]), strbuffer0 );
                          
                          // Bene, verifica se questo file già esiste
                          
                          if (!fexist (templat))    // No: in template forse c'è il nome file che ci serve
                          {
                              // Verifica che il file necessario sia realmente creabile
                              
                              fd = open (templat, O_CREAT|O_RDWR, 777);
                              
                              if (fd>=0) return fd;
                              // Se il file è stato creato con successo in scrittura, 
                              // ritorna l'handle del file temporaneo che è stato scelto     
                          } 
                      } 
                      
                      // Se il sistema arriva a questo punto, vuol dire che sono stati
                      // provati tutti i 999.999 files temporanei
                      
                      return -1;
              }  
              else      // Il path richiesto non esiste
              {
                      chdir ( (char *)(&PreviousDir) );
                      return -1;  
              }
      }
      else   // La sintassi del nome lungo è errata
        return -1;     
  }
  else  // Se non ci sono almeno 6 caratteri, sicuramente il template è errato
    return -1; 
}


char* mktemp(char* templat) 
{
  char LongName [256]; 
  char Path [256]; 
  char Name [64]; 
  char Ext  [32];
  
  char PreviousDir [512];
  
  char strbuffer0 [10];
  int  n,slen,nlen,nrfile;
  int  LNResult;
  int  ChDirResult;
  
  slen = strlen (templat);
  
  if (slen>6)   
  {
      // Provvede ad espandere il nome che è stato fornito in modo da ottenere
      // un nome lungo
      
      LNResult = fssGenerateLongName ( (char *)(&SystemPath), templat, (char *)(&LongName) );
  
      if (LNResult==0)
      {
              // Adesso è necessario eseguire uno split del nome file in modo da isolare 
              // il nome della directory 
               
              fssSplitNameFile ( (char *)(&LongName), Path, Name, Ext);
              
              // Salva il nome della vecchia directory
              
              getcwd ( (char *)(&PreviousDir), 511);
              
              // Tenta un chdir per vedere se è possibile entrare nella directory richiesta
              
              ChDirResult = chdir ( (char *)(&Path) );
              
              if (ChDirResult==0)        // Il path esiste
              { 
                      // Ripristina la vecchia directory prima di continuare
                      
                      chdir ( (char *)(&PreviousDir) );
                       
                      // Esegui un controllo per verificare se gli ultimi caratteri del template
                      // sono costituiti solo dalla x
                      
                      for (n=slen-6; n<=slen-1; n++)
                      {
                          // Per il corretto funzionamento del sistema, è necessario controllare
                          // che gli ultimi sei caratteri siano XXXXXX. Tuttavia, in alcuni casi
                          // questo può creare problemi (se si usa mktemp "FILEXXXXXX", all'inizio
                          // funzionerà, ma poi non sarà più così perchè il sistema modifica il
                          // contenuto della ram nel punto dove è stata memorizzata la stringa dal
                          // compilatore. 
                      
                          // Per aggirare il problema, questa versione di mkstemp accetta anche 
                          // le cifre 0..9 se compaiono al posto delle X nel template
                          
                          // Questo passaggio della routine provvede anche a riempire di zeri 
                          // la regione dove è presente il pattern XXXXXX
                      
                          if ( (templat[n]=='X') || ( (templat[n]>='0') && (templat[n]<='9') ) ) 
                          {
                             templat[n]='0';  
                          }
                          else
                          {
                             errno=EINVAL; 
                             return 0; 
                          }
                      } 
                      
                      // Adesso provvede a generare un nuovo numero 
                      
                      for (nrfile=0; nrfile<999999; nrfile++)
                      {
                          _itoa (nrfile, (char *)(&strbuffer0), 10);
                          nlen = strlen (strbuffer0);
                          
                          // Adesso copia dentro template il numero che ci interessa
                          strcpy ( &(templat [slen-nlen]), strbuffer0 );
                          
                          // Bene, verifica se questo file già esiste
                          
                          if (!fexist (templat))    // No: in template forse c'è il nome file che ci serve
                          {
                              // Ritorna il nome del file che è stato scelto
                          
                              return templat;        
                          } 
                      } 
                      
                      // Se il sistema arriva a questo punto, vuol dire che sono stati
                      // provati tutti i 999.999 files temporanei
                      
                      return 0;
              }  
              else      // Il path richiesto non esiste
              {
                      chdir ( (char *)(&PreviousDir) );
                      return 0;  
              }
      }
      else   // La sintassi del nome lungo è errata
        return 0;     
  }
  else  // Se non ci sono almeno 6 caratteri, sicuramente il template è errato
    return 0; 
}


