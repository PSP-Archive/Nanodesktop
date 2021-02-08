/*
   --------------------------------------------------------------------------
   stdiocore.c                                      NanoC Files (PSP)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di base per la gestione del sottosistema dischi 
   ----------------------------------------------------------------------------
*/   

#define DED_STDIOCORE_I_NEED_VARIABLES
          #include <psp/$$_ded_stdiocore.h>
#undef  DED_STDIOCORE_I_NEED_VARIABLES



/*
    --------------------------------------------------------------------------
    RIFERIMENTI A ROUTINES NANODESKTOP USATE NEL SEGUITO
    --------------------------------------------------------------------------
*/

extern void ndStrCpy (char *StringaDest, char *StringaSrc, unsigned char Limit, char MakeMagn); 
extern void ndStrCpyBounded (char *StringaDest, char *StringaSrc, unsigned char LimitX1, unsigned char LimitX2, char MakeMagn); 
   
extern char *_itoa (int n, char *buf, int radix); 
                                    
/*
    --------------------------------------------------------------------------
    INIZIALIZZAZIONE
    --------------------------------------------------------------------------
*/

void _pspsdk_stdio_init()
/*
    Inizializza eventuali routine connesse con il file system.
    
    Nell'architettura PSPE, provvede a reinizializzare i dati relativi ai 
    Virtual File Header ed agli StreamsArray
*/

{
  int Counter;
  
  for (Counter=0; Counter<__NRVIRTUALFILEHANDLER; Counter++) 
    VFH [Counter].IsEnabled=0;
  
  StreamsArray [0].type = -1;                                 // stdin
  StreamsArray [0].fd   = 0;
  StreamsArray [0].cnt  = 0;
  StreamsArray [0].flag = _IOREAD;
  
  StreamsArray [1].type = STD_IOBUF_TYPE_STDOUTHOST;          // stdout
  StreamsArray [1].fd   = 1;
  StreamsArray [1].cnt  = 0;
  StreamsArray [1].flag = _IOWRT;
  
  StreamsArray [2].type = STD_IOBUF_TYPE_STDOUTHOST;          // stderr
  StreamsArray [2].fd   = 2;
  StreamsArray [2].cnt  = 0;
  StreamsArray [2].flag = _IOWRT;
  
  for (Counter=3; Counter<_NFILE; Counter++) 
    StreamsArray [Counter].fd = -1;               // Equivale ad handle non impegnato
  
  // Il sistema ora deve settare al valore di default il path, e deve
  // reinizializzare l'OS in modo da settare la directory corrente al
  // valore di default. Siccome la PSP non possiede una funzione getcwd, 
  // la NanoC deve compensare nel seguente modo: ogni volta che viene
  // richiesto un cambiamento di directory, Nanodesktop aggiorna il
  // valore della stringa SystemPath
  
  strcpy (SystemPath, SysRootPath);
  
  // Files temporanei impegnati inizialmente a zero
  
  ndTempFilesBusy = 0;
  
  // Impedirà ulteriori inizializzazioni
  
  _PSPSDK_InitFSExecuted=1;    
}

void _pspsdk_stdio_deinit()
{
  _PSPSDK_InitFSExecuted=0;    
}



/*
    ------------------------------------------------------------------------------------
    ACCESSO A BASSO LIVELLO
    ----------------------------------------------------------------------------------  
*/ 
 
int INTERNAL_Seek32 (SceUID fd, SceOff Offset, int Whence)
{
    // Sotto PSP è possibile utilizzare la routine del firmware sceIoLseek32.
    
    return sceIoLseek32 (fd, Offset, Whence);       
}   


int INTERNAL_SaveBank (int VFH_Handle, int NrBank)
{
   int TotalBanks;
   int OffsetBank;
   
   int BeginOfBank;
   int BytesToWrite;
   int BytesReallyWrite;
   
   TotalBanks = VFH [VFH_Handle].FileLength / DISK_BUFFER_SIZE;
   OffsetBank = VFH [VFH_Handle].FileLength % DISK_BUFFER_SIZE;
   
   BeginOfBank = NrBank * DISK_BUFFER_SIZE;
   
   if ((VFH [VFH_Handle].FileLength - BeginOfBank) > DISK_BUFFER_SIZE)
     BytesToWrite = DISK_BUFFER_SIZE;        
   else
     BytesToWrite = VFH [VFH_Handle].FileLength - BeginOfBank;
     
   INTERNAL_Seek32 (VFH [VFH_Handle].OsHandle, BeginOfBank, SEEK_SET);
   BytesReallyWrite = sceIoWrite (VFH [VFH_Handle].OsHandle, VFH [VFH_Handle].AddressData, BytesToWrite);
   
   if (BytesReallyWrite>=0) // Non ci sono stati errori nel salvare il banco
   {   
      VFH [VFH_Handle].BankIsUpdated = 0;
      return 0;
   }   
   else                     // C'è stato un errore nella routine di caricamento dei banchi
   {   
      errno=28;             // errno: No space left on device  
      return -1;            // Restituisci condizione di errore        
   }
}



int INTERNAL_LoadBank (int VFH_Handle, int NrBank)
{
   int TotalBanks;
   int OffsetBank;
   
   int BeginOfBank;
   int BytesToRead;
   int BytesReallyRead;
   
   int SaveIOResult;
   
   if ( (VFH [VFH_Handle].WriteOn) && (VFH [VFH_Handle].BankIsUpdated) )
   {
        SaveIOResult=INTERNAL_SaveBank (VFH_Handle, VFH [VFH_Handle].NrBankLoaded);
        // Provvede a salvare il vecchio banco
        
        if (SaveIOResult!=0)     // Se il processo di salvataggio del banco fallisce, SaveBank restituisce     
        {                        // un valore !=0. In questo caso, anche LoadBank fallisce   
           errno=5;              // errno: errore di I/O
           return -1;
        }
   }
   
   TotalBanks = VFH [VFH_Handle].FileLength / DISK_BUFFER_SIZE;
   OffsetBank = VFH [VFH_Handle].FileLength % DISK_BUFFER_SIZE;
   
   BeginOfBank = NrBank * DISK_BUFFER_SIZE;
   
   if ((VFH [VFH_Handle].FileLength - BeginOfBank) > DISK_BUFFER_SIZE)
     BytesToRead = DISK_BUFFER_SIZE;        
   else
     BytesToRead = VFH [VFH_Handle].FileLength - BeginOfBank;
     
   INTERNAL_Seek32 (VFH [VFH_Handle].OsHandle, BeginOfBank, SEEK_SET);
   BytesReallyRead = sceIoRead (VFH [VFH_Handle].OsHandle, VFH [VFH_Handle].AddressData, BytesToRead);
   
   if (BytesReallyRead>=0) // Non ci sono stati errori nel load del banco
   {
      VFH [VFH_Handle].BankLoaded    = 1;
      VFH [VFH_Handle].NrBankLoaded  = NrBank;
      VFH [VFH_Handle].BeginOfBank   = BeginOfBank;
      VFH [VFH_Handle].EndOfBank     = BeginOfBank + BytesReallyRead;
      VFH [VFH_Handle].BankIsUpdated = 0;
        
      return 0;
   } 
   else      // C'è stato un errore nella routine di caricamento dei banchi
   {
      VFH [VFH_Handle].BankLoaded     = 0;
      
      errno = 5;                                     // errno: Errore di I/O
      return -1;                                     // Restituisci condizione di errore        
   }
}

int INTERNAL_FillTheFile (int VFHNumber, char Byte, int Size)
/*
    Riempie un file con un certo carattere
*/
{
    int Counter, BytesWritten;
    
    for (Counter=0; Counter<Size; Counter++)
    {
        BytesWritten = write (VFHNumber, &Byte, 1);
        
        if (BytesWritten!=1)             // Ci sono dei problemi in scrittura
        {
             errno=5;              // Errore di I/O             
             return -1;            // Ritorna l'errore alla routine chiamante                            
        }
    }
    
    // Se il sistema è arrivato qui, vuol dire che è tutto a posto. 
    return 0;
}


/*
   ------------------------------------------------------------------------------------
   ROUTINE OPEN ANSI COMPLIANT
   ----------------------------------------------------------------------------------  
*/ 


int open (const char *NameFile, int flags, int mode)
/*
    Open or create a file for reading or writing.

    NameFile  - Pointer to a string holding the name of the file to open
    flags  - Libc styled flags that are or'ed together
    mode  - File access mode.

    Return:
    A non-negative integer is a valid fd, anything else an error
*/
{
  
  char LongName [256];
  char LNResult;
  
  unsigned short int VFHChoosen, Counter;
  char IHaveFound;
  
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  
  LNResult = fssGenerateLongName ((char *)(&SystemPath), (char *)(NameFile), (char *)(&LongName));
     
  if (LNResult==0)
  {        
          // Provvede ad analizzare la richiesta eseguita. Per prima cosa deve trovare un
          // VFH per memorizzare i dati
          
          IHaveFound=0;
          
          for (Counter=1; (Counter<__NRVIRTUALFILEHANDLER) && (!IHaveFound); Counter++)
          // Il conteggio parte da 1 perchè l'handle in uscita non può essere nullo
          {
              if (!(VFH [Counter].IsEnabled))    // Abbiamo trovato l'handler virtuale
              {
                 IHaveFound=1;
                 VFHChoosen=Counter;                  
              }
          } 
                 
          if (IHaveFound)                        // E' disponibile il VFH
          {
                      VFH [VFHChoosen].IsEnabled = 1; 
                      
                      // Analizza le richieste che sono state fatte
                      
                      VFH [VFHChoosen].ReadOn  = 0;
                      VFH [VFHChoosen].WriteOn = 0;
                      
                      if ((flags & O_RDONLY) == O_RDONLY)
                      {
                         VFH [VFHChoosen].ReadOn  = 1;
                      }
                      
                      if ((flags & O_WRONLY) == O_WRONLY)
                      {
                         VFH [VFHChoosen].WriteOn = 1;
                      }
                      
                      if ((flags & O_RDWR) == O_RDWR)
                      {
                         VFH [VFHChoosen].ReadOn  = 1;
                         VFH [VFHChoosen].WriteOn = 1;
                      }
                      
                      if (!(VFH [VFHChoosen].WriteOn))
                      {
                           // E' stata richiesta l'apertura di un file in modalità solo lettura
                           
                           return INTERNAL_open____mode_RD ((char *)(NameFile), (char *)(&LongName), flags, mode, VFHChoosen);    
                      }
                      else
                      {
                           // E' stata richiesta l'apertura in sola scrittura oppure l'apertura in
                           // modalità read-write.
                           
                           return INTERNAL_open____mode_WR_RW ((char *)(NameFile), (char *)(&LongName), flags, mode, VFHChoosen); 
                      }
          }
          else     // Non è stato possibile trovare un VFH handler disponibile
          {
               errno = 24; // Too many open files
               return -1;  // Simula una condizione di errore per la routine chiamante          
          }        
  }
  else
  {
      errno = 58;
      return -1;
  }  
}  



int INTERNAL_open____mode_RD (char *NameFile, char *LongName, int flags, int mode, int VFHChoosen)
/*
    Questa routine è esclusivamente per uso interno. Provvede ad aprire un file
    in modalità solo lettura.
*/
{
   int ErrFLen, DiskBankError;
   void* MemoryHandle; 
   int fwFileHandle;
 
   // E' stata richiesta l'apertura in sola lettura. Ok, per prima cosa
   // verifichiamo se il file già esiste e l'eventuale lunghezza
   
   ErrFLen = flen (LongName); 
   
   if (ErrFLen>=0)         // Il file esiste ed è possibile aprirlo
   {
       // Tentiamo di allocare il buffer che farà da cache in lettura
       
       MemoryHandle = malloc (DISK_BUFFER_SIZE+1);
       
       if (MemoryHandle>=0)         // E' stato possibile allocare un handler
       {
           VFH [VFHChoosen].PosSeek32   = 0;
           VFH [VFHChoosen].FileLength  = flen (LongName);
           VFH [VFHChoosen].AddressData = MemoryHandle;
           
           VFH [VFHChoosen].SysWord     = O_RDONLY;
           VFH [VFHChoosen].HW_Word     = O_RDONLY;
           
           // Ora prepariamoci ad aprire il file con il codice presente in HW_Word
           
           fwFileHandle = sceIoOpen (LongName, VFH [VFHChoosen].HW_Word, mode);
           
           if (fwFileHandle>=0)
           {
              // Salva una copia del nome dentro il VFH. Necessario per
              // il funzionamento di routines come fstat
              
              ndStrCpy ( (char *)(&(VFH [VFHChoosen].Name)), LongName, 128, 0 ); 
              
              // Memorizza le altre informazioni
              
              VFH [VFHChoosen].OsHandle      = fwFileHandle;
              VFH [VFHChoosen].BankLoaded    = 0;
              VFH [VFHChoosen].BankIsUpdated = 0;
              
              DiskBankError=INTERNAL_LoadBank (VFHChoosen, 0);
           
              if (DiskBankError>=0)    // Non ci sono stati errori
              {
                 return VFHChoosen;    // Ritorna il VFH alla routine chiamante
              }
              else    // Non è stato possibile caricare neanche il primo banco
              {
                 sceIoClose (fwFileHandle);
                 free (VFH [VFHChoosen].AddressData);
                 VFH [VFHChoosen].IsEnabled = 0;
                 
                 errno=5;              // errno: Errore di I/O
                 return -1;            // Ritorna l'errore alla routine chiamante
              }
           }
           else     // Il processo di Open del file è fallito
           {
              free (VFH [VFHChoosen].AddressData);
              VFH [VFHChoosen].IsEnabled = 0;
           
              errno=5;                 // errno: Errore di I/O   
              return -1;
           }
           
       }   
       else       // Non è stato possibile eseguire malloc
       {
          VFH [VFHChoosen].IsEnabled = 0;
          
          errno=105;                 // No buffer space available
          return -1;        
       }
       
   }       // Si sta tentando di aprire in lettura un file di cui non è possibile
   else    // nemmeno determinare la lunghezza
   {
     errno = 2;       // No such file or directory
     
     VFH [VFHChoosen].IsEnabled = 0;
     return -1;      
   }
}


int INTERNAL_open____mode_WR_RW (char *NameFile, char *LongName, int flags, int mode, int VFHChoosen)
/*
    Questa routine è esclusivamente per uso interno. Provvede ad aprire un file
    in modalità scrittura o lettura/scrittura.
*/
{
   int ErrFLen, DiskBankError;
   void *MemoryHandle; 
   int fwFileHandle;
  
   // Tentiamo di allocare il buffer che farà da cache in scrittura o da
   // cache in lettura/scrittura

   MemoryHandle = malloc (DISK_BUFFER_SIZE+1);
   
   if (MemoryHandle>=0)   // Ok, è stato possibile allocare un handler
   {
      VFH [VFHChoosen].AddressData = MemoryHandle;
      VFH [VFHChoosen].SysWord = flags;
      
      // Sotto PSPE, il tentativo di aprire files in modalità O_WRONLY dà
      // luogo a dei file di dimensione nulla, probabilmente a causa di un
      // bug nel kernel emulato di PSPE. 
      
      // Sotto PSP, invece, si ottiene un errore sconosciuto in scrittura.
      // Per tali ragioni, si provvede a sostituire il flag O_WRONLY con O_RDWR.
      
      VFH [VFHChoosen].HW_Word = ((flags & (~O_WRONLY))) | O_RDWR; 
      
      
      if ((flags & O_APPEND) != O_APPEND)        // Non si vuole una apertura in modo append
      {                                          // Creiamo un file totalmente nuovo o cancelliamo quello 
                                                 // esistente
              
              fwFileHandle = sceIoOpen (LongName, VFH [VFHChoosen].HW_Word, mode);
              
              if (fwFileHandle>=0)     // Non si è verificato alcun errore nell'apertura in scrittura
              {
                          // Salva una copia del nome dentro il VFH. Necessario per il funzionamento di 
                          // routines come fstat
                          
                          ndStrCpy ( (char *)(&(VFH [VFHChoosen].Name)), LongName, 128, 0 ); 
                      
                          // Salva le altre informazioni
                              
                          VFH [VFHChoosen].OsHandle   = fwFileHandle;
                          VFH [VFHChoosen].FileLength = 0;
                          VFH [VFHChoosen].PosSeek32  = 0;
                          
                          // A questo punto in FileLength e PosSeek32 ci sono la lunghezza attuale del
                          // file e la posizione del puntatore seek (qui sono entrambi a zero perchè
                          // il file è nuovo)
              
                          VFH [VFHChoosen].BankLoaded    = 0;
                          VFH [VFHChoosen].BankIsUpdated = 0;
                      
                          // Carica il primo banco
                          
                          DiskBankError=INTERNAL_LoadBank (VFHChoosen, VFH [VFHChoosen].BankLoaded);
              
                          if (DiskBankError>=0)    // Non ci sono stati errori
                          {
                                  return VFHChoosen;    // OK: Apertura riuscita
                                                        // Ritorna il VFH alla routine chiamante
                                                         
                          }
                          else    // Non è stato possibile caricare neanche il primo banco
                          {
                                  sceIoClose (fwFileHandle);
                 
                                  free (VFH [VFHChoosen].AddressData);
                                  VFH [VFHChoosen].IsEnabled = 0;
                 
                                  errno=5;              // Errore di I/O             
                                  return -1;            // Ritorna l'errore alla routine chiamante
                          }  
              }
              else                     // Il tentativo di aprire il file in scrittura ha portato ad un errore
              {
                  free (VFH [VFHChoosen].AddressData);
                  VFH [VFHChoosen].IsEnabled = 0;
                  return -1;  
              }     
      }
      else    // E' stato richiesto che il file sia aperto con un append in scrittura 
      {
              // Determiniamo la lunghezza del file
      
              ErrFLen = flen (LongName);
      
              if (ErrFLen>=0)       // Il file effettivamente già esiste
              {                               // aprilo in modalità scrittura append
                     
                          fwFileHandle = sceIoOpen (LongName, VFH [VFHChoosen].HW_Word, mode);
                
                          if (fwFileHandle>=0)     // Non si è verificato alcun errore nell'apertura in scrittura
                          {
                                 // Salva una copia del nome dentro il VFH. Necessario per il funzionamento di 
                                 // routines come fstat
                                          
                                 ndStrCpy ( (char *)(&(VFH [VFHChoosen].Name)), LongName, 128, 0 ); 
                                 
                                 // Aggiungi le altre informazioni     
                                 
                                 VFH [VFHChoosen].OsHandle   = fwFileHandle;
                                 VFH [VFHChoosen].FileLength = ErrFLen;
                                 VFH [VFHChoosen].PosSeek32  = ErrFLen;
                                 
                                 // A questo punto in FileLength e PosSeek32 ci sono la lunghezza attuale del
                                 // file e la posizione del puntatore seek.
          
                                 VFH [VFHChoosen].BankLoaded    = VFH [VFHChoosen].PosSeek32 / DISK_BUFFER_SIZE;
                                 VFH [VFHChoosen].BankIsUpdated = 0;
                                 
                                 // Carica il primo banco
                                 
                                 DiskBankError=INTERNAL_LoadBank (VFHChoosen, VFH [VFHChoosen].BankLoaded);
      
                                 if (DiskBankError>=0)     // Non ci sono stati errori
                                 {
                                      return VFHChoosen;    // OK: Apertura riuscita
                                                            // Ritorna il VFH alla routine chiamante                    
                                 }
                                 else    // Non è stato possibile caricare neanche il primo banco
                                 {
                                     sceIoClose (fwFileHandle);
                                     
                                     free (VFH [VFHChoosen].AddressData);
                                     VFH [VFHChoosen].IsEnabled = 0;
                                     
                                     errno=5;              // Errore di I/O             
                                     return -1;            // Ritorna l'errore alla routine chiamante
                                 } 
                                      
                          }
                          else   // Il tentativo di aprire il file in scrittura append ha portato ad un errore
                          {
                             free (VFH [VFHChoosen].AddressData);
                             VFH [VFHChoosen].IsEnabled = 0;
                             return -1;  
                          }
              }
              else    // Il file che si è chiesto di aprire in modalità append in realtà non esiste. 
              {       // Crea un nuovo file
              
                      VFH [VFHChoosen].HW_Word = ((flags & (~O_APPEND))) | O_RDWR; 
                      // Converti dalla modalità append a read-write
                     
                      fwFileHandle = sceIoOpen (LongName, VFH [VFHChoosen].HW_Word, mode);
              
                      if (fwFileHandle>=0)     // Non si è verificato alcun errore nell'apertura in scrittura
                      {
                                  // Salva una copia del nome dentro il VFH. Necessario per il funzionamento di 
                                  // routines come fstat
                                  
                                  ndStrCpy ( (char *)(&(VFH [VFHChoosen].Name)), LongName, 128, 0 ); 
                              
                                  // Salva le altre informazioni
                                      
                                  VFH [VFHChoosen].OsHandle   = fwFileHandle;
                                  VFH [VFHChoosen].FileLength = 0;
                                  VFH [VFHChoosen].PosSeek32  = 0;
                                  
                                  // A questo punto in FileLength e PosSeek32 ci sono la lunghezza attuale del
                                  // file e la posizione del puntatore seek (qui sono entrambi a zero perchè
                                  // il file è nuovo)
                      
                                  VFH [VFHChoosen].BankLoaded    = 0;
                                  VFH [VFHChoosen].BankIsUpdated = 0;
                              
                                  // Carica il primo banco
                                  
                                  DiskBankError=INTERNAL_LoadBank (VFHChoosen, VFH [VFHChoosen].BankLoaded);
                      
                                  if (DiskBankError>=0)    // Non ci sono stati errori
                                  {
                                          return VFHChoosen;    // OK: Apertura riuscita
                                                                // Ritorna il VFH alla routine chiamante
                                                                 
                                  }
                                  else    // Non è stato possibile caricare neanche il primo banco
                                  {
                                          sceIoClose (fwFileHandle);
                         
                                          free (VFH [VFHChoosen].AddressData);
                                          VFH [VFHChoosen].IsEnabled = 0;
                         
                                          errno=5;              // Errore di I/O             
                                          return -1;            // Ritorna l'errore alla routine chiamante
                                  }  
                      }
                      else                     // Il tentativo di aprire il file in scrittura ha portato ad un errore
                      {
                          free (VFH [VFHChoosen].AddressData);
                          VFH [VFHChoosen].IsEnabled = 0;
                          return -1;  
                      }  
                      
              }    
      }
   }
   else                    // Non è stato possibile allocare un memory handle
   {
      errno=105;           // No buffer space available
      
      VFH [VFHChoosen].IsEnabled = 0;
      return -1;           // Segnala l'errore alla routine chiamante
   }   
   
}


/*
   ------------------------------------------------------------------------------------
   ALTRE ROUTINES ANSI A BASSO LIVELLO
   ----------------------------------------------------------------------------------  
*/ 


int read (int VFHNumber, void *Data, int Size)
/*
  Read input

  fd    - Opened file descriptor to read from
  data  - Pointer to the buffer where the read data will be placed
  size  - Size of the read in bytes

  Returns:
  The number of bytes read
*/

{
   int  Counter, CounterBytesRead;    
   char YouCanExit;
   char *PointerToByteRead, *PointerToByteWritten;
   
   int  NrBankNeeded, OffBankNeeded; 
   int  IOResult; 
   int  OffsetByteRead;  
   
   // Richiama automaticamente la funzione di reinizializzazione del file-system
   // qualora essa non sia già stata utilizzata
   if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
      
   if (VFH [VFHNumber].IsEnabled)
   {
      if (VFH [VFHNumber].ReadOn)
      {
         YouCanExit=0;
         CounterBytesRead=0;
        
         for (Counter=0; ( (Counter<Size) && (!YouCanExit) && (VFH [VFHNumber].PosSeek32 < VFH [VFHNumber].FileLength) ); Counter++)
         {
             NrBankNeeded  = VFH [VFHNumber].PosSeek32 / DISK_BUFFER_SIZE;
             OffBankNeeded = VFH [VFHNumber].PosSeek32 % DISK_BUFFER_SIZE;
                   
             // Carica il nuovo banco ove necessario
                   
             if (NrBankNeeded!=VFH [VFHNumber].NrBankLoaded) 
                IOResult=INTERNAL_LoadBank (VFHNumber, NrBankNeeded);
             else
                IOResult=0;    
                       
             if (IOResult==0)             // Non ci sono stati errori nel caricamento del banco
             {    
                 if ( (VFH [VFHNumber].PosSeek32 >= VFH [VFHNumber].BeginOfBank) &&
                      (VFH [VFHNumber].PosSeek32 <= VFH [VFHNumber].EndOfBank) )
                 {
                          OffsetByteRead = VFH [VFHNumber].PosSeek32 - VFH [VFHNumber].BeginOfBank;
                          
                          PointerToByteRead    = VFH [VFHNumber].AddressData + OffsetByteRead;
                          PointerToByteWritten = Data + Counter;
                          
                          *PointerToByteWritten = *PointerToByteRead;
                          // Punta al byte di destinazione in memoria
                          
                          CounterBytesRead++;
                          VFH [VFHNumber].PosSeek32++;
                 }
             }
             else   // C'è stato un errore disco nel caricamento del banco
             {
                YouCanExit=1;
             }       
         }
         
         return CounterBytesRead;
           
      }
      else  // Il VFH è aperto in modalità solo scrittura
      {
            errno = 13;   // errno: Permission denied
            return -2;
      }  
   }
   else  // Il VFH non è abilitato.
   {
       errno = 9;      // errno: Bad file number  
       return -1;
   }
}




int write (int VFHNumber, const void *Data, int Size)
/*
  Write output

  fd    - Opened file descriptor to write
  data  - Pointer to the buffer where the written data will be placed
  size  - Size of the write in bytes

  Returns:
  The number of written chars
*/

{
   int  Counter, CounterBytesWritten;    
   char YouCanExit;
   char *PointerToByteRead, *PointerToByteWritten;
   
   int  NrBankNeeded, OffBankNeeded; 
   int  IOResult; 
   int  OffsetByteWritten;  
   
   // Richiama automaticamente la funzione di reinizializzazione del file-system
   // qualora essa non sia già stata utilizzata
   if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
      
   if (VFH [VFHNumber].IsEnabled)
   {
      if (VFH [VFHNumber].WriteOn)
      {
         YouCanExit=0;
         CounterBytesWritten=0;
        
         for (Counter=0; ( (Counter<Size) && (!YouCanExit) ); Counter++)
         {
             NrBankNeeded  = VFH [VFHNumber].PosSeek32 / DISK_BUFFER_SIZE;
             OffBankNeeded = VFH [VFHNumber].PosSeek32 % DISK_BUFFER_SIZE;
                   
             // Carica il nuovo banco ove necessario. Questa operazione salverà anche il
             // banco attualmente in memoria
                   
             if (NrBankNeeded!=VFH [VFHNumber].NrBankLoaded) 
                IOResult=INTERNAL_LoadBank (VFHNumber, NrBankNeeded);
             else
                IOResult=0;
                
             if (IOResult==0)             // Non ci sono stati errori nel caricamento del banco
             {                              
                 if ( VFH [VFHNumber].PosSeek32 <= VFH [VFHNumber].FileLength )  
                 {
                          OffsetByteWritten = VFH [VFHNumber].PosSeek32 - VFH [VFHNumber].BeginOfBank;
                          
                          PointerToByteRead        = (char *)(Data + Counter);
                          PointerToByteWritten     = VFH [VFHNumber].AddressData + OffsetByteWritten;
                 
                          *PointerToByteWritten    = *PointerToByteRead;
                          // Punta al byte del buffer di scrittura
                          
                          VFH [VFHNumber].BankIsUpdated=1;
                          // Segnala al sistema che il banco è stato modificato
                          
                          CounterBytesWritten++;
                          
                          if ((VFH [VFHNumber].PosSeek32)<(VFH [VFHNumber].FileLength))
                          {
                             VFH [VFHNumber].PosSeek32++;
                          }
                          
                          if ((VFH [VFHNumber].PosSeek32)==(VFH [VFHNumber].FileLength))
                          {
                             VFH [VFHNumber].PosSeek32++;
                             VFH [VFHNumber].FileLength++;
                          }
                          
                 }
             }
             else   // C'è stato un errore disco nel caricamento del banco
             {
                YouCanExit=1;
             }       
         }
         
         return CounterBytesWritten;
           
      }
      else  // Il VFH è aperto in modalità solo scrittura
      {
         errno=13;    // errno: permission denied     
         return -2;
      }  
   }
   else  // Il VFH non è abilitato.
   {
       errno=9;        // errno: bad file number  
       return -1;
   }
}


int close (int VFHNumber)
/*
   Chiude il file. 
   
   Per i file che sono aperti in scrittura, scarica la cache sul disco.
*/

{
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if (VFH [VFHNumber].IsEnabled)
  {
     if (VFH [VFHNumber].WriteOn)
     {
             if ( (VFH [VFHNumber].PosSeek32 - VFH [VFHNumber].BeginOfBank)>0)
             {
                 INTERNAL_SaveBank (VFHNumber, VFH [VFHNumber].NrBankLoaded);
                 // Scarica il buffer di scrittura prima di chiudere il file
             }
             
             sceIoClose (VFH [VFHNumber].OsHandle);
             
             free (VFH [VFHNumber].AddressData);    // Disalloca il buffer di scrittura
             
             VFH [VFHNumber].IsEnabled=0;        
     }
     else
     {
             sceIoClose (VFH [VFHNumber].OsHandle);
               
             free (VFH [VFHNumber].AddressData);   // Non è necessario chiudere fisicamente il file perchè di esso è presente solo una
             VFH [VFHNumber].IsEnabled=0;          // copia in memoria. 
     }
     
     return 0;     
  }
  else   // Il VFH non è allocato
  {
    errno=9;         // Bad file number  
    return -1;       // Segnala l'errore alla routine di partenza
  }          
} 


int lseek (int VFHNumber, int Offset, int Whence)
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
  int NewSeekPos; 
  int ErrRep; 
    
  if (VFH [VFHNumber].IsEnabled)
  {
        switch (Whence)
        {
               case SEEK_SET:
               {
                    NewSeekPos = Offset;
                    
                    if (NewSeekPos>=0) 
                    {
                         if (VFH [VFHNumber].FileLength<NewSeekPos)
                         {
                             // E' stato scelto di portare il puntatore del file oltre gli
                             // attuali limiti fisici del file. Per farlo, bisogna riempire
                             // di zero le posizioni mancanti del file     
                                 
                             ErrRep=INTERNAL_FillTheFile (VFHNumber, 0, NewSeekPos-VFH [VFHNumber].FileLength);
                             
                             if (ErrRep)     // C'è stato un errore nell'operazione di fill
                             {
                                 // Grazie alla routine precedente il puntatore è già posizionato  
                                 // nel punto corretto
                                 
                                 errno = 29;     // errno: Illegal seek
                                 return -1;           
                             }    
                         }
                         
                         VFH [VFHNumber].PosSeek32 = NewSeekPos;               
                         return NewSeekPos;
                    }
                    else 
                    {
                         errno = 29;     // errno: Illegal seek
                         return -1;
                    }
          
                    break;
               }
     
               case SEEK_CUR:
               {
                    NewSeekPos = VFH [VFHNumber].PosSeek32 + Offset;
     
                    if (NewSeekPos>=0) 
                    {
                         if (VFH [VFHNumber].FileLength<NewSeekPos)
                         {
                             // E' stato scelto di portare il puntatore del file oltre gli
                             // attuali limiti fisici del file. Per farlo, bisogna riempire
                             // di zero le posizioni mancanti del file     
                                 
                             ErrRep=INTERNAL_FillTheFile (VFHNumber, 0, NewSeekPos-VFH [VFHNumber].FileLength);
                             
                             if (ErrRep)     // C'è stato un errore nell'operazione di fill
                             {
                                 // Grazie alla routine precedente il puntatore è già posizionato  
                                 // nel punto corretto
                                 
                                 errno = 29;     // errno: Illegal seek
                                 return -1;           
                             }    
                         }
                         
                         VFH [VFHNumber].PosSeek32 = NewSeekPos;
                         return NewSeekPos;
                    }
                    else 
                    {
                         errno = 29;     // errno: Illegal seek
                         return -1;
                    }
                    
                    break;    
               }
     
               case SEEK_END:
               {
                    NewSeekPos = VFH [VFHNumber].FileLength - Offset;
     
                    if (NewSeekPos>=0) 
                    {
                         if (VFH [VFHNumber].FileLength<NewSeekPos)
                         {
                             // E' stato scelto di portare il puntatore del file oltre gli
                             // attuali limiti fisici del file. Per farlo, bisogna riempire
                             // di zero le posizioni mancanti del file     
                                 
                             ErrRep=INTERNAL_FillTheFile (VFHNumber, 0, NewSeekPos-VFH [VFHNumber].FileLength);
                             
                             if (ErrRep)     // C'è stato un errore nell'operazione di fill
                             {
                                 // Grazie alla routine precedente il puntatore è già posizionato  
                                 // nel punto corretto
                                 
                                 errno = 29;     // errno: Illegal seek
                                 return -1;           
                             }    
                         }
                         
                         VFH [VFHNumber].PosSeek32 = NewSeekPos;
                         return NewSeekPos;
                    } 
                    else 
                    {
                         errno = 29;     // errno: Illegal seek
                         return -1;
                    }
          
                    break;    
               }
     
               default:         // Se il valore di Whence è sbagliato, restituisci -1
               {     
                  errno = 22;    // errno: Invalid argument   
                  return -1;
               }
        }  
  }
  else   // Il VFH non è allocato
  {
     errno = 9;        // errno: Bad file number
     return -1;        // Segnala l'errore alla routine di partenza
  }
}  
  
  
int tell (int VFHNumber)
/*
   Il firmware della PSP non gestisce la funzione Tell delle libc dei PC. Poichè talune
   librerie necessitano di tale funzionalità (Dev IL), essa viene emulata dall'HAL
*/
{
  if (VFH [VFHNumber].IsEnabled)
  {
    return VFH [VFHNumber].PosSeek32; 
  }    
  else               // Il VFH non è allocato
  {
     errno = 9;       // errno: Bad file number                  
     return -1;       // Segnala l'errore alla routine di partenza
  }
}
 
 
int remove (const char *NameFile)
/*
  Remove directory entry.

  Parameters:

  file  - Path to the file to remove

  Returns:
  < 0 on error
*/

{
  char LongName [256];
  char LNResult;
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  LNResult = fssGenerateLongName ( (char *)(&SystemPath), (char *)(NameFile), (char *)(&LongName) );
    
  if (LNResult==0)
  {    
     return sceIoRemove ( (char *)(&LongName) );
  }
  else
  {
     errno=58;
     return -1;  
  }
}
 
 
  
int mkdir (const char *dir, int mode)
/*
  Make a directory file.

  Parameters:

  dir   - Nome directory
  mode  - Access mode.

  Returns:
  Returns the value 0 if its succesful otherwise -1
*/
{
  char LongName [256];
  char LNResult;

  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata

  LNResult = fssGenerateLongName ( (char *)(&SystemPath), (char *)(dir), (char *)(&LongName) );
  
  if (LNResult==0)
  {    
     return sceIoMkdir ( (char *)(&LongName), mode);
  }
  else
  {
     errno=58;
     return -1;   
  }
}





int rmdir (const char *dir)
/*
  Remove a directory file.

  Parameters:
  path  - Removes a directory file pointed by the string path

  Returns:
  Returns the value 0 if its succesful otherwise -1
*/

{       
  char LongName [256];
  char LNResult;
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata

  LNResult = fssGenerateLongName ( (char *)(&SystemPath), (char *)(dir), (char *)(&LongName) );
  
  if (LNResult==0)
  {    
         if (strcmp ( (char *)(&SystemPath), (char *)(&LongName) )!=0)      // SystemPath e long name sono diverse
         {
             return sceIoRmdir ( (char *)(&LongName) );       
         }
         else       // Stai tentando di cancellare la directory corrente. Errore
         {
             errno=13;      // Permission denied
             return -1;       
         }
  }
  else
  {
     errno=58;
     return -1;   
  }       
}

int chdir (char *dir)
{
  char LongName [256];
  char LNResult;
  int  fdd;
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata

  LNResult = fssGenerateLongName ( (char *)(&SystemPath), (char *)(dir), (char *)(&LongName) );
  
  if (LNResult==0)
  {    
     // Attenzione perchè sceIoChdir non sembra funzionare correttamente 
     // sotto PSPE. In fondo, a noi interessa semplicemente verificare
     // che la directory esista, senza entrarvi veramente, perchè 
     // i nomi vengono generati a runtime in ogni routine attraverso
     // una routine che trasforma ogni percorso relativo in un nome
     // lungo. Quindi, usiamo sceIoDopen per accertarci che la 
     // directory esista realmente
     
     fdd=sceIoDopen ( (char *)(&LongName) );
     
     if (fdd>=0)     // La directory esiste. Ok.  
     {
         sceIoDclose (fdd);
         // Chiudi il descrittore di directory
         
         strcpy ( (char *)(&SystemPath), (char *)(&LongName) );
         // Adesso viene aggiornata SystemPath al valore corrente
         
         return 0;             
     }
     else
     {
        errno = 2;             // Not such file or directory
        return -1;
     }
  }
  else
  {
     errno=58;
     return -1;   
  } 
}


int rename (const char *name, const char *newname)
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
  char LongName0 [256], LongName1 [256];
  char LNResult0, LNResult1;
  
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
   
  LNResult0 = fssGenerateLongName ( (char *)(&SystemPath), (char *)(name),  (char *)(&LongName0) );
  LNResult1 = fssGenerateLongName ( (char *)(&SystemPath), (char *)(newname), (char *)(&LongName1) ); 
  
  if ( (LNResult0==0) && (LNResult1==0) )
  {
      return sceIoRename ( (char *)(&LongName0), (char *)(&LongName1) ); 
  } 
  else
  {
     errno=58;
     return -1;  
  }
}
 

/*
   ------------------------------------------------------------------------------------
   ESTRAZIONE DELLA DIRECTORY CORRENTE
   ----------------------------------------------------------------------------------  
*/ 

char *get_current_working_dir_name(void)
/*
   get_current_dir_name will malloc(3) an array  big  enough
   to  hold  the  current directory name. After this  that
   value will be returned.
*/
{
  char *newbuffer;
  int  cwdlen;
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata    
  
  cwdlen    = strlen (SystemPath);
  newbuffer = malloc (cwdlen);
      
  if (newbuffer!=0)  // Ok, è riuscito ad allocare il buffer richiesto
  {
         strcpy (newbuffer, SystemPath);
         return newbuffer;
  }
  else    // Allocazione non riuscita
  {
         errno = 93;     // Segnala errore di allocazione    
         return 0; 
  }  
}

char *getcwd(char *buf, size_t size)
/*
   The getcwd() function copies the absolute pathname of  the
   current  working directory to the array pointed to by buf,
   which is of length size.

   If the current absolute path name would require  a  buffer
   longer  than size elements, NULL is returned, and errno is
   set to ERANGE; an application should check for this error,
   and allocate a larger buffer if necessary.

   As  an  extension  to the POSIX.1 standard, getcwd() allocates 
   the buffer dynamically using malloc() if buf is NULL
   on  call.   In  this  case,  the  allocated buffer has the
   length size unless size is less than  zero,  when  buf  is
   allocated  as  big  as  necessary.   It  is possible (and,
   indeed, advisable) to free() the buffers if they have been
   obtained this way.
*/
{
  int  cwdlen;
  
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  if (buf!=0)     // Il buffer richiesto non è NULL
  {
     cwdlen = strlen (SystemPath);
     
     if (cwdlen<size)
     {
        strcpy (buf, SystemPath);
        return buf;          
     } 
     else
     {
        errno=ERANGE;        // Failure
        return 0; 
     }            
  }    
  else            // Ha richiesto 0 alloca quanto richiesto
  {
      return ( get_current_working_dir_name() );
  }     
}


/*
   ------------------------------------------------------------------------------------
   ROUTINES NON ANSI COMPLIANT
   ----------------------------------------------------------------------------------  
*/ 


int flen (char *NameFile)
/* 
   Questa funzione restituisce la lunghezza di un file in byte. Qualora vi
   fossero problemi ad aprire il file, viene restituito il valore -1
*/

{           
    char LongName [256];
    char LNResult;
    
    int FileHeaderOrError;
    int Old_PosSeekPointer;
    int FileLength;
    
    if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
    // Richiama automaticamente la funzione di reinizializzazione del file-system
    // qualora essa non sia già stata utilizzata
    
    LNResult = fssGenerateLongName ( (char *)(&SystemPath), (char *)(NameFile), (char *)(&LongName) );
    
    if (LNResult==0)
    {         
         FileHeaderOrError = sceIoOpen ( (char *)(&LongName), O_RDONLY, 0777);
    
         if (FileHeaderOrError>=0)
         {
            Old_PosSeekPointer = INTERNAL_Seek32 (FileHeaderOrError, 0, SEEK_CUR);
            // Registra la posizione del vecchio puntatore
             
            FileLength = INTERNAL_Seek32 (FileHeaderOrError, 0, SEEK_END); 
            // Porta il puntatore alla fine in modo da trovare la lunghezza del file
            
            INTERNAL_Seek32 (FileHeaderOrError, Old_PosSeekPointer, SEEK_SET);
            // Ripristina la vecchia posizione 
            
            sceIoClose (FileHeaderOrError);
            return FileLength;
         }
         else
         {
            errno=2;                                 // errno: No such file or directory    
            return FileHeaderOrError;                // Riporta l'errore alla routine chiamante
         }         
    }
    else    // Error in LongName Generation
    {
       errno=58;
       return -1;
    }      
}        


int fexist (char *NameFile)
/* 
   Questa funzione verifica se un certo file esiste già sul disco. Se
   è così restituisce 1 altrimenti restituisce 0.
*/
{
  int FileHeaderOrError;
  
  char LongName [256];
  char LNResult;
    
  if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
  // Richiama automaticamente la funzione di reinizializzazione del file-system
  // qualora essa non sia già stata utilizzata
  
  LNResult = fssGenerateLongName ( (char *)(&SystemPath), NameFile, (char *)(&LongName) );
    
  if (LNResult==0)
  {        
      FileHeaderOrError = sceIoOpen ( (char *)(&LongName), O_RDONLY, 0777);
      
      if (FileHeaderOrError>=0)     // Il file esiste già
      {
         sceIoClose (FileHeaderOrError);
         return 1;
      }
      else
      {
         return 0;
      } 
        
  }
  else    // Error in LongName Generation
  {
       errno=58;
       return -1;
  }
}        

int access (char *path, int mode)
{
    if (fexist (path)) return 0;
       else return -1;
}


int truncate (char *name, int length)
{
    char tmpfilename [512];
    char strbuffer0 [16];
    
    int fd, intfd, oldlen;
    int charswr, charsrd, counter, error;
    
    static int Buffer = 0;
    
    if (fexist (name))
    {
       oldlen = flen (name);
       
       if (oldlen<length)   // Bisogna aggiungere degli zeri
       {
           intfd = open (name, O_WRONLY, 0x777);
           
           if (intfd>=0)
           {
               lseek (intfd, 0, SEEK_END);                  // Puntatore alla fine
               error=0;
               
               for (counter=0; counter<length-oldlen; counter++)
               {
                   charswr=write (intfd, &Buffer, 1);     
                   
                   if (charswr!=1) 
                   {
                       error=-1;
                       break;
                   }
               }
               
               close (intfd);
               return error;
           }
           else       // Problema ad aprire il file in scrittura
           {
               return -1;       
           }
       }    
       else       // Si deve troncare il vecchio file
       {
           ndTempFilesBusy++;
           
           strcpy (tmpfilename, "$TEMP$");
           _itoa  (ndTempFilesBusy, (char *)(&strbuffer0), 10);
           strcat (tmpfilename, strbuffer0);
           strcat (tmpfilename, "$");
           strcat (tmpfilename, name);
           
           error=0;
           
           fd    = open (name,        O_RDONLY, 0x777);
           intfd = open (tmpfilename, O_WRONLY | O_CREAT, 0x777);
                  
           for (counter=0; counter<length; counter++)
           {
               charsrd=read   (fd, &Buffer, 1);
               if (charsrd!=1) 
               {
                   error=-1;
                   break;
               }
               
               charswr=write  (intfd, &Buffer, 1); 
               if (charswr!=1) 
               {
                   error=-1;
                   break;
               } 
           }       
           
           close (intfd);
           close (fd);
           
           ndTempFilesBusy--;
           
           if (!error)   // Non ci sono errori
           {
                remove (name);                // Cancella il vecchio file 
                rename (tmpfilename, name);       // Trasforma il file temporaneo nel vecchio file
           
                return 0;
           }
           else // il sistema ha trovato degli errori
           {
                remove (tmpfilename);             // Rimuovi il file temporaneo  
                return -1;
           }
           
       }                           
           
    }
    else return -1;   // Il file non esiste           
}




/* -----------------------------------------------------------------------------
   Origin: PSPSDK                                               STAT
   -----------------------------------------------------------------------------
   
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
   ----------------------------------------------------------------------------------
*/



static time_t psp_to_epoch_time(ScePspDateTime psp_time)
{
	struct tm conv_time;
	conv_time.tm_year = psp_time.year;
	conv_time.tm_mon = psp_time.month;
	conv_time.tm_mday = psp_time.day;
	conv_time.tm_hour = psp_time.hour;
	conv_time.tm_min = psp_time.minute;
	conv_time.tm_sec = psp_time.second;
	conv_time.tm_isdst = -1;
	return mktime(&conv_time);
}



int stat(const char *filename, struct stat *buf)
{
	SceIoStat psp_stat;
	char dest[512];
	int LNResult, ret;

	if (!_PSPSDK_InitFSExecuted) _pspsdk_stdio_init();
    // Richiama automaticamente la funzione di reinizializzazione del file-system
    // qualora essa non sia già stata utilizzata
    
    LNResult = fssGenerateLongName ( (char *)(&SystemPath), (char *)(filename), (char *)(&dest) );
    
	memset(buf, '\0', sizeof(struct stat));
	ret = sceIoGetstat(dest, &psp_stat);
	if (ret < 0) 
    {
	    errno=ret;
    	return ret;
	}
	
	buf->st_ctime = psp_to_epoch_time(psp_stat.st_ctime);
	buf->st_atime = psp_to_epoch_time(psp_stat.st_atime);
	buf->st_mtime = psp_to_epoch_time(psp_stat.st_mtime);

	buf->st_mode = (psp_stat.st_mode & 0xfff) |
		       ((FIO_S_ISLNK(psp_stat.st_mode))?(S_IFLNK):(0)) |
		       ((FIO_S_ISREG(psp_stat.st_mode))?(S_IFREG):(0)) |
		       ((FIO_S_ISDIR(psp_stat.st_mode))?(S_IFDIR):(0));
	buf->st_size = psp_stat.st_size;
	return 0;
}

int link(const char *oldpath, const char *newpath)
{
    // PSPE e PSP non supportano link simbolici, quindi restituisce sempre -1
    
    errno=EPERM;
    return -1;
}

int unlink(const char *pathname) 
{
    // PSPE e PSP non supportano link simbolici, quindi restituisce sempre -1
    
    errno=EPERM;
    return -1;
}

 
int creat(const char *path, mode_t mode)
{
    return open (path, O_CREAT | O_TRUNC | O_WRONLY, mode);
}


int chmod (const char *filename, mode_t mode)
{
    // Funzione ancora non implementata. Viene comunque riportato il successo
    // per non interferire con dei programmi che usano tale funzione e che
    // sono usati internamente a Visilab Research
    
    return 0;
} 


/* -----------------------------------------------------------------------------
   Origin: Filippo Battaglia                               FUNZIONI SPECIALI DOS                
   -----------------------------------------------------------------------------
   Copyright 2006-2011 Filippo Battaglia
   
   IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

   By downloading, copying, installing or using the software you agree to this 
   license.
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
   ----------------------------------------------------------------------------------
*/


char doscopy (char *filedest, char *filesrc)
{
     int  vfhdest, vfhsrc;
     char Char, YouCanExit;
     int  RdResult, WrResult;
     
     vfhdest = open (filedest, O_WRONLY | O_CREAT | O_TRUNC, 0x777);
     
     if (vfhdest>=0)
     {
          vfhsrc = open (filesrc, O_RDONLY, 0x777);
          
          if (vfhsrc>=0)
          {
               YouCanExit=0;
               
               while (!YouCanExit)
               {
                     RdResult = read (vfhsrc, &Char, 1);
                     
                     if (RdResult==1)
                     {
                         WrResult = write (vfhdest, &Char, 1);       
                     
                         if (WrResult!=1)         // Problema in scrittura
                         {
                             YouCanExit=2;
                             break;
                         }
                     }
                     else
                     {
                         YouCanExit=1;
                         break;
                     }
               }
               
               close (vfhsrc);
               close (vfhdest);
               
               if (YouCanExit==2) return -1;        // C'è un problema di scrittura. Segnala -1
                  else  return 0;                   // I dati sono stati copiati con successo
          }
          else
          {
              close (vfhdest);
              return -2;
          }        
     }
     else
       return -1;
}


/*
   ----------------------------------------------------------------------------
   FNCNTL
   ----------------------------------------------------------------------------
   Questa è solo una implementazione parziale, necessaria per il funzionamento
   di NanoCPP.
   ----------------------------------------------------------------------------
*/

int _fcntl (int fd, int cmd, va_list args)
{	
	int TempBank;
    
    if ( !VFH [fd].IsEnabled )
		return -EBADF;
	
    switch (cmd) 
    {
        case 0x03:            // F_GETFL
            {
                 TempBank=0;
                 
                 if   (VFH [fd].WriteOn) TempBank |= O_WRONLY;
                 if   (VFH [fd].ReadOn)  TempBank |= O_RDONLY;
                 
                 if   ( (VFH [fd].ReadOn) && (VFH [fd].WriteOn) )
                     TempBank |= O_RDWR; 
                 
                 return TempBank;
            } 
           
        default:
            {
                 return -EINVAL;
            }
    }
}    

int fcntl (int fd, int cmd, ...)
{
    va_list args;
	int ret;

	va_start(args, cmd);
	ret = _fcntl (fd, cmd, args);
	va_end(args);

	return ret;  
}



