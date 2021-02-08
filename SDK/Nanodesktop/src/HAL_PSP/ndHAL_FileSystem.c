/*  ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.    
    ---------------------------------------------------------------------------
    WRAPPER PER LE FUNZIONI DI GESTIONE DISCO (PSP)
    ---------------------------------------------------------------------------
    Le routine che fanno normalmente parte della libc standard (eccetto printf
    e derivate) come open, close, flen ecc. ecc. ecc. sono wrappate da 
    Nanodesktop alla libc della piattaforma.
    
    In questo modo, quanto il codice Nanodesktop richiama ndHAL_Open, il 
    sistema richiama la funzione standard open. 
    Questa tecnica consente di adattare velocemente Nanodesktop a piattaforme
    che possiedono libc non standard o prive di particolari funzionalità.
    
    Tutte le funzioni del wrapper sono dichiarate in-line in modo da ridurre
    l'overhead di chiamata a funzione.
    ----------------------------------------------------------------------------
*/

#include <HAL_PSP\$_HAL_NDforND.h>

/* 
   ---------------------------------------------------------------------------
   ROUTINE DI INIZIALIZZAZIONE DEL FILE-SYSTEM
   ---------------------------------------------------------------------------
*/

void ndHAL_InitFileSystem ()
/*
    Inizializza eventuali routine connesse con il file system.
    
*/

{
  _pspsdk_stdio_init ();
  // Nell'architettura PSPE, inizializza le strutture dati per
  // la gestione del file-system, per mezzo dell'apposita
  // routine posta nella libreria NanoC per PSPE.

  ndFSInitExecuted=1;
}

/* 
   --------------------------------------------------------------------------
   WRAPPER PER LA LIBC NanoC
   --------------------------------------------------------------------------
*/

inline int ndHAL_Flen (char *NameFile)
/* 
   Questa funzione restituisce la lunghezza di un file in byte. Qualora vi
   fossero problemi ad aprire il file, viene restituito il valore -1
*/

{
  return flen (NameFile);
}        


inline int ndHAL_Fexist (char *NameFile)
/* 
   Questa funzione verifica se un certo file esiste già sul disco. Se
   è così restituisce 1 altrimenti restituisce 0.
*/
{
  return fexist (NameFile);
}        


inline int ndHAL_Open (char *file, int flags, int mode)
/*
    Open or create a file for reading or writing.

    file  - Pointer to a string holding the name of the file to open
    flags  - Libc styled flags that are or'ed together
    mode  - File access mode.

    Return:
    A non-negative integer is a valid fd, anything else an error
*/
{
   return open (file, flags, mode);   
}  


inline int ndHAL_Close (int VFHNumber)
/*
   Chiude il file. 
   
   Per i file che sono aperti in lettura, scarica dalla memoria la cache e disalloca il
   relativo VHF.
*/

{
  return close (VFHNumber);                    
} 




inline int ndHAL_Read (int VFHNumber, void *Data, int Size)
/*
  Read input

  fd    - Opened file descriptor to read from
  data  - Pointer to the buffer where the read data will be placed
  size  - Size of the read in bytes

  Returns:
  The number of bytes read
*/

{
  return read (VFHNumber, Data, Size);    
}


inline int ndHAL_Write (int VFHNumber, void *Data, int Size)
/*
  Write output.

  fd  - Opened file descriptor to write to
  data  - Pointer to the data to write
  size  - Size of data to write

  Returns:
  The number of bytes written
*/

{
  return write (VFHNumber, Data, Size);    
}

  
inline int ndHAL_Lseek (int VFHNumber, int Offset, int Whence)
/*
  Reposition read/write file descriptor offset.

  Parameters:

  fd  - Opened file descriptor with which to seek
  offset  - Relative offset from the start position given by whence
  whence  - Set to SEEK_SET to seek from the start of the file, SEEK_CUR seek from the current position and SEEK_END to seek from the end.

  Returns:
  The position in the file after the seek.
*/  
  
{
  return lseek (VFHNumber, Offset, Whence);  
}  
  
  
inline int ndHAL_Tell (int VFHNumber)
/*
   Il firmware della PSP non gestisce la funzione Tell delle libc dei PC. Poichè talune
   librerie necessitano di tale funzionalità (Dev IL), essa viene emulata dall'HAL
*/
{
  return tell (VFHNumber);          
}
 
 
inline int ndHAL_EraseFile (const char *file)
/*
  Remove directory entry.

  Parameters:

  file  - Path to the file to remove

  Returns:
  < 0 on error
*/

{
  return remove (file);
}
 
 
  
inline int ndHAL_MakeDir (const char *dir, int mode)
/*
  Make a directory file.

  Parameters:

  dir   - Nome directory
  mode  - Access mode.

  Returns:
  Returns the value 0 if its succesful otherwise -1
*/
{
  return mkdir (dir, mode);        
}


inline int ndHAL_RemoveDir (const char *dir)
/*
  Remove a directory file.

  Parameters:
  path  - Removes a directory file pointed by the string path

  Returns:
  Returns the value 0 if its succesful otherwise -1
*/

{
  return rmdir (dir);
}


inline int ndHAL_RenameFile (const char *oldname, const char *newname)
/*
  Change the name of a file.

  Parameters:
    oldname  - The old filename
    newname  - The new filename

  Returns:
  < 0 on error.
*/

{
  return rename (oldname, newname);
}



inline FILE *ndHAL_Fopen (const char *fname, const char *mode)
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
    
    Innanzitutto, non è ammessa l'apertura di un file contemporaneamente in
    lettura e scrittura: qualsiasi tentativo di aprire un file in lettura e
    scrittura, farà sì che la routine restituisca NULL.
    
    Una stringa mode pari ad r apre il file in lettura, una stringa mode 
    pari a w apre il file in scrittura, ed una stringa mode pari ad a 
    apre il file in scrittura con append. Nel caso di mode pari ad a 
    oppure w, se il file non esiste viene creato.
    
    I simboli b t e + sono SEMPRE ignorati dalla routine.
*/
{
  return fopen (fname, mode);
}


inline FILE *ndHAL_Fdopen (int fd, const char *mode)
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
  return fdopen (fd, mode);
}



inline int ndHAL_Fclose (FILE *stream)
{
  return fclose (stream);
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
inline int ndHAL_Fcloseall(void)
{
  return fcloseall ();
}


inline size_t ndHAL_Fread (void *buf, int r, int n, FILE *stream)
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
    return fread (buf, r, n, stream);
}


inline size_t ndHAL_Fwrite(const void *buf, size_t r, size_t n, FILE *stream)
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
  return fwrite (buf, r, n, stream);
}

/*
    --------------------------------------------------------------------------
    Funzioni Get
    --------------------------------------------------------------------------
*/


inline int ndHAL_Fgetc (FILE *stream)
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
  return fgetc (stream);       
}


inline int ndHAL_Xgetc (FILE *stream)
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
  return xgetc (stream);  
}




inline char *ndHAL_Fgets (char *buf, int n, FILE *stream)
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
  return fgets (buf, n, stream);
}

inline int ndHAL_Getc (FILE *stream)
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
  return getc (stream);
}

inline int ndHAL_Getchar(void)
{
    return getchar ();
}


/* 
   ----------------------------------------------------------------------------
   Funzioni Put
   ----------------------------------------------------------------------------
*/


inline int ndHAL_Fputc (int c, FILE *stream)
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
  return fputc (c, stream);
}


inline int ndHAL_Fputs (const char *s, FILE *stream)
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
  return fputs (s, stream);
}
   




inline int ndHAL_Putc(int c, FILE *stream)
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
  return putc (c, stream);  
}






inline int ndHAL_Puts (const char *s)
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
  return puts (s);
}


inline int ndHAL_Putchar (int c)
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
  return putchar (c);  
}


/* ---------------------------------------------------------------------------
   Funzioni seek/tell
   ---------------------------------------------------------------------------
*/


inline int ndHAL_Fseek(FILE *stream, long offset, int origin)
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
  return fseek (stream, offset, origin);
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
inline long ndHAL_Ftell(FILE *stream)
{
  return ftell (stream);
}


inline int ndHAL_Fgetpos (FILE *stream, fpos_t *pos)
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
  return fgetpos (stream, pos);  
}

inline int ndHAL_Fsetpos (FILE *stream, fpos_t *pos)
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
  return fsetpos (stream, pos);
}



inline void ndHAL_rewind(FILE *stream)
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
  return rewind (stream);
}



inline int ndHAL_Rename(const char *name, const char *newname)
/*
**
**  [func] - rename.
**  [desc] -
**  [entr] - const char *name; the filename string pointer.
**           const char *newname; the new filename string pointer.
**  [exit] - int;
**  [prec] - name and newname are valid string pointers.
**  [post] - the name filen name is modified.
**
*/
{
  return rename (name, newname);
}



inline int ndHAL_Remove(const char *s)
/*
**
**  [func] - remove.
**  [desc] - if the s named file exists then deletes the s named file and
**           returns 0. else returns -1.
**  [entr] - const char *s; the filename string pointer.
**  [exit] - int; 0 if able to delete the s file. else -1.
**  [prec] - s is a valid string pointer.
**  [post] - the s file is deleted.
**
*/
{
  return remove (s);  
}


inline int ndHAL_Ungetc (int c, FILE *stream)
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
  return ungetc (c, stream);  
}



inline char ndHAL_Tmpnam (char *name)
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
  return tmpnam (name);  
}


inline FILE *ndHAL_Tmpfile (void)
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
  return tmpfile();  
}


inline int ndHAL_Fileno (FILE * f) 
{
  return fileno (f);  
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
inline int ndHAL_Feof(FILE *stream)
{
  return feof (stream);
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
inline int ndHAL_Ferror(FILE *stream)
{
  return ferror (stream);
}

inline char *ndHAL_Strerror(int err) 
{
    return strerror (err);
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
inline void ndHAL_Perror(const char *s)
{
  return perror (s);   
}


inline void ndHAL_Clearerr(FILE *stream)
{
  return clearerr (stream);
}



inline int ndHAL_Fflush(FILE *stream)
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
  return fflush (stream);  
}


inline int ndHAL_Fflushall (void)
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
  return fflushall ();  
}



inline int ndHAL_Setvbuf (FILE *Stream, char *Buf, int Mode, size_t BufSize)
/*
   Le routine gestite da NanoC non supportano stream bufferizzati.
   
   Di conseguenza, la routine si limita semplicemente a restituire
   un valore pari a 0 (tutto ok) in tutti i casi in cui viene 
   richiesto un mode non bufferizzato, mentre viene restituito
   il valore -1 (errore) quando viene richiesto un meccanismo
   diverso di bufferizzazione.
*/
{
   return setvbuf (Stream, Buf, Mode, BufSize);
}



inline int ndHAL_Setbuf (FILE *Stream, char *Buf)
{
    return setbuf (Stream, Buf);
}     



inline int ndHAL_Devctl (const char *dev, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen)
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
  return devctl (dev, cmd, indata, inlen, outdata, outlen);  
}


/* 
   ----------------------------------------------------------------------------
   ROUTINE NON WRAPPATE SU FUNZIONI STANDARD DELLA LIBRERIA NANO-C
   ----------------------------------------------------------------------------
*/


int ndHAL_Dopen (const char *fn)
/*
  Open a directory.

  Parameters:
    dirname  - The directory to open for reading.

  Returns:
  if >= 0 then a valid file descriptor, otherwise a Sony error code.
*/

{
  return sceIoDopen (fn);
}



int ndHAL_Dread (SceUID fd, struct ndFS_DirEntry_Type *NdDir)
/*
  Reads an entry from an opened file descriptor.

  Parameters:
    fd  - Already opened file descriptor (using sceIoDopen)
    dir  - Pointer to an io_dirent_t structure to hold the file information

  Returns: Read status
  0 - No more directory entries left
  > 0 - More directory entired to go
  < 0 - Error
*/
{
  static struct SceIoDirent PSPDir;    // Buffer interno compatibile con l'ABI del kernel PSP
  int ErrorCode, Counter;

  ErrorCode = sceIoDread (fd, &PSPDir);

  NdDir->Status.mode = PSPDir.d_stat.st_mode;
  NdDir->Status.attr = PSPDir.d_stat.st_attr;
  NdDir->Status.size = PSPDir.d_stat.st_size;

  NdDir->Status.ctime.year  = PSPDir.d_stat.st_ctime.year;
  NdDir->Status.ctime.month = PSPDir.d_stat.st_ctime.month;
  NdDir->Status.ctime.day   = PSPDir.d_stat.st_ctime.day;
  NdDir->Status.ctime.hour  = PSPDir.d_stat.st_ctime.hour;
  NdDir->Status.ctime.min   = PSPDir.d_stat.st_ctime.minute;
  NdDir->Status.ctime.sec   = PSPDir.d_stat.st_ctime.second;
  NdDir->Status.ctime.msec  = PSPDir.d_stat.st_ctime.microsecond;
  NdDir->Status.ctime.available = 1;

  NdDir->Status.atime.year  = PSPDir.d_stat.st_atime.year;
  NdDir->Status.atime.month = PSPDir.d_stat.st_atime.month;
  NdDir->Status.atime.day   = PSPDir.d_stat.st_atime.day;
  NdDir->Status.atime.hour  = PSPDir.d_stat.st_atime.hour;
  NdDir->Status.atime.min   = PSPDir.d_stat.st_atime.minute;
  NdDir->Status.atime.sec   = PSPDir.d_stat.st_atime.second;
  NdDir->Status.atime.msec  = PSPDir.d_stat.st_atime.microsecond;
  NdDir->Status.atime.available = 1;

  NdDir->Status.mtime.year  = PSPDir.d_stat.st_mtime.year;
  NdDir->Status.mtime.month = PSPDir.d_stat.st_mtime.month;
  NdDir->Status.mtime.day   = PSPDir.d_stat.st_mtime.day;
  NdDir->Status.mtime.hour  = PSPDir.d_stat.st_mtime.hour;
  NdDir->Status.mtime.min   = PSPDir.d_stat.st_mtime.minute;
  NdDir->Status.mtime.sec   = PSPDir.d_stat.st_mtime.second;
  NdDir->Status.mtime.msec  = PSPDir.d_stat.st_mtime.microsecond;
  NdDir->Status.mtime.available = 1;

  for (Counter=0; Counter<6; Counter++)
  {
      NdDir->Status.st_private [Counter] = PSPDir.d_stat.st_private [Counter];
  }

  strcpy (NdDir->d_name, PSPDir.d_name);

  NdDir->d_private = PSPDir.d_private;

  return ErrorCode;
}


int ndHAL_Dclose (int fd)
/*
  Close an opened directory file descriptor.

  Parameters:
    fd  - Already opened file descriptor (using sceIoDopen)

  Returns:
  < 0 on error
*/
{
  return sceIoDclose (fd);
}





