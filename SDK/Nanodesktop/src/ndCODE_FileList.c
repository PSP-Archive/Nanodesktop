/*  
    ----------------------------------------------------------------------------
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
    ----------------------------------------------------------------------------
    TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI FILE 
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void ndDB_InitListOfFile (struct HEADLIST_File_Type *MyListHandle)
/*
   Questa procedura viene usata per inizializzare una struttura che dovrà contenere un elenco
   di file.
   La routine chiamante deve allocare una struttura di tipo HEADLIST_File_Type. E' compito della
   routine chiamante allocare correttamente tale struttura: di solito essa è statica.
   Alla routine ndDB_InitList viene passato l'indirizzo della struttura.
*/
{
  MyListHandle->FirstElementAddr=0;
  MyListHandle->LastElementAddr=0;
  MyListHandle->NrElements=0;
};

char ndDB_AppendFileToList (struct HEADLIST_File_Type *MyListHandle, char *Namefile, char Ext[3], int Size, char IsDir,
                            struct ndFS_DateTime_Type *MyDateTime)
/*
   Questa routine alloca uno spazio in memoria per inserire un nuovo elemento nella lista dei file, dopodichè
   aggiunge l'indirizzo nella lista di indirizzi definita dalla struttura MyListHandle.

   L'elemento aggiunto ha il nome, l'estensione, le dimensioni ed il datetime che vengono passati come parametri.

   Se il nome passato contiene più di 64 caratteri, esso verrà troncato al 64-esimo carattere.

   Le liste di file di Nanodesktop non possono superare i 20.000 elementi. Il motivo è molto semplice:
   gli handles di memoria su alcune piattaforme sono limitati, ed una lista di file troppo lunga
   consumerebbe tutti gli handles disponibili. Se si tenta di allocare file oltre il valore citato,
   si otterrà un errore ERR_FILELIST_OVERFLOW. 
   
   Sulle piattaforme in cui la gestione della memoria è effettuata dal sistema operativo (che
   fornisce le funzioni alloc e free senza che queste debbano essere emulate dall'HAL di
   Nanodesktop), è possibile bypassare tale controllo per mezzo della costante manifesta
   NO_FILELIST_OVERFLOW_CHECK in ndHAL_Variables.c 

   Sono previsti i codici di errore:
      ERR_MEMORY_OVERFLOW          La memoria non è sufficiente
      ERR_MEMORY_ERROR             Errore nella memoria
      ERR_FILELIST_OVERFLOW
*/

{
   struct ELEMENT_File_Type *PointerToNewFile;
   char Counter, YouCanExit;
   char *PointerToChar;
   char MemErrRep;
   
   #ifndef NO_FILELIST_OVERFLOW_CHECK
   if ((MyListHandle->NrElements)==MAXLISTELEMENTALLOWED)
     return ERR_FILELIST_OVERFLOW;
   #endif
   
   PointerToNewFile = malloc  ( sizeof (*PointerToNewFile) );
   // Alloca in memoria lo spazio per memorizzare le informazioni relative al nuovo file.
   
   if (PointerToNewFile!=0)     // L'allocazione ha avuto successo
   {
      YouCanExit=0;
      Counter=0;
      PointerToChar=Namefile;     // Punta al primo carattere del nome file

      ndStrCpy (PointerToNewFile->Name, Namefile, 63, 0);
      ndStrCpy (PointerToNewFile->Ext, Ext, 3, 0);
      
      PointerToNewFile->Size=Size;
      PointerToNewFile->IsDir=IsDir;

      PointerToNewFile->DateTime.day       = MyDateTime->day;
      PointerToNewFile->DateTime.month     = MyDateTime->month;
      PointerToNewFile->DateTime.year      = MyDateTime->year;
      PointerToNewFile->DateTime.hour      = MyDateTime->hour;
      PointerToNewFile->DateTime.min       = MyDateTime->min;
      PointerToNewFile->DateTime.sec       = MyDateTime->sec;
      PointerToNewFile->DateTime.msec      = MyDateTime->msec;
      PointerToNewFile->DateTime.available = MyDateTime->available;
      
      MemErrRep=ndDB_AppendElementToList (MyListHandle, PointerToNewFile);
      // Memorizza l'indirizzo del nuovo elemento nella lista di indirizzi.

      if (MemErrRep!=0)        // Il tentativo di appendere il nuovo elemento può essere fallito.
      {                        // In tal caso si deve disallocare l'area dove abbiamo copiato le informazioni relative al nuovo file
          free ( PointerToNewFile );
          return ERR_MEMORY_ERROR;
      }
      else
          return 0;

  }
  else
      return ERR_MEMORY_OVERFLOW;

}

char ndDB_GetFileFromList ( struct HEADLIST_File_Type *MyListHandle, int NrElement, struct ELEMENT_File_Type *MyFile)
/*
   Supponiamo che la routine chiamante abbia allocato come variabile locale una struttura di tipo
   ELEMENT_File_Type.

   La routine ndDB_GetFileFromList copia i dati relativi al file nr. NrElement della lista individuata
   dall'handle MyListHandle nella struttura *MyFile. La routine chiamante deve avere allocato
   un'opportuna struttura di tipo ELEMENT_File_Type atta a contenere le informazioni e deve
   passarne l'indirizzo per mezzo del parametro MyFile.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
   struct ELEMENT_File_Type *PointerToFile;
   char ErrRep;
   int PointerToRam;
   
   ErrRep=ndDB_GetElementFromList (MyListHandle, NrElement, &PointerToRam );
   // Deposita dentro PointerToRam l'indirizzo dell'n-esimo elemento.

   PointerToFile = PointerToRam;
   // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
   // direttamente &PointerToFile alla routine precedente, ma si verifica un errore.

   if (!ErrRep)
   {
      // La routine provvede a copiare i dati dalla loro posizione originale in Ram, al buffer
      // puntato da MyFile. Questo buffer in genere è una struttura che è stata allocata dalla
      // routine chiamante (magari in stack).

      ndStrCpy (MyFile->Name, PointerToFile->Name, 63, 0);
      ndStrCpy (MyFile->Ext,  PointerToFile->Ext,   3, 0);
      
      MyFile->Size  = PointerToFile->Size;
      MyFile->IsDir = PointerToFile->IsDir;

      MyFile->DateTime.day       = PointerToFile->DateTime.day;
      MyFile->DateTime.month     = PointerToFile->DateTime.month;
      MyFile->DateTime.year      = PointerToFile->DateTime.year;
      MyFile->DateTime.hour      = PointerToFile->DateTime.hour;
      MyFile->DateTime.min       = PointerToFile->DateTime.min;
      MyFile->DateTime.sec       = PointerToFile->DateTime.sec;
      MyFile->DateTime.msec      = PointerToFile->DateTime.msec;
      MyFile->DateTime.available = PointerToFile->DateTime.available;

      return 0;
   }
   else
     return ErrRep;

}

char ndDB_GetFileAddrFromList ( struct HEADLIST_File_Type *MyListHandle, int NrElement, struct ELEMENT_File_Type *AddressGet)
/*
   Questa routine fornisce l'indirizzo di memoria Ram in cui sono memorizzati i dati relativi
   al file nr. NrElement.

   Tale indirizzo di memoria viene memorizzato nel puntatore AddressGet. Tale puntatore deve
   essere già stato allocato dalla routine chiamante.

   La routine agisce sul puntatore, **non** sul contenuto del puntatore. Pertanto, un
   corretto modello di chiamata può essere:

   ELEMENT_File_Type *Adr;
   ndDB_GetFileAddrFromList (&MyListHandle, 0, Adr);

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
   return ndDB_GetElementFromList (MyListHandle, NrElement, &AddressGet);
   // Pone in AdrObtained l'indirizzo dell'elemento richiesto.
}

char ndDB_SwapFileInList (struct HEADLIST_File_Type *MyListHandle, int NrElement)
/*
   Questa routine provvede a swappare tra loro i file nr. NrElement e
   NrElement+1.

   Non è consentito di porre NrElement al numero di elementi attuale della lista.

   La routine restituisce 0 qualora l'operazione ha successo, altrimenti
   restituisce -1

   Sono previsti i seguenti codici di errore:
        ERR_SWAP_IS_NOT_ALLOWED    Ci sono meno di due elementi
        ERR_NR_ELEMENT_IS_INVALID  Il numero di elemento è invalido
*/

{
  return ndDB_SwapElementInList (MyListHandle, NrElement); 
}


char ndDB_RemoveLastFileFromList (struct HEADLIST_File_Type *MyListHandle)
/*
   Questa routine provvede a rimuovere l'ultimo elemento dalla lista di file 
   identificata da MyListHandle.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è già vuota
*/
{
  int PointerToRam;
  char ErrRep;

  if ( (MyListHandle->NrElements)!=0)
  {
    ErrRep = ndDB_GetLastElementFromList (MyListHandle, &PointerToRam);
    // Ricava l'indirizzo dell'ultimo elemento memorizzato in memoria.

    if (ErrRep==0)                    // Se non ci sono errori
    {
       free (PointerToRam);
       // Disalloca l'area di memoria che conteneva i dati per l'ultimo file della lista

       ErrRep=ndDB_RemoveLastElementFromList (MyListHandle);
       // Ed ora rimuove anche l'ultimo elemento della lista concatenata di indirizzi

       return ErrRep;
    }
  }
  else
      return ERR_LIST_IS_EMPTY;
}


void ndDB_DestroyListOfFile (struct HEADLIST_File_Type *MyListHandle)
/*
   Questa routine provvede a cancellare tutti i file che sono memorizzati in una lista
   di file.
   La lista da cancellare viene specificata dalla struttura HEADLIST_File_Type. La procedura
   chiamante deve passare l'indirizzo di tale procedura

   Non sono previsti codici di errore per questa routine.
*/

{
  int Counter;
  int OriginalNrElements;

  OriginalNrElements=MyListHandle->NrElements;

  for (Counter=0; Counter<OriginalNrElements; Counter++)
  {
     ndDB_RemoveLastFileFromList (MyListHandle);
  }
}


char ndDB_CompareFile (char *Name1, char *Ext1, char IsDir1, char *Name2, char *Ext2, char IsDir2)
/*
   Esegue la comparazione tra due file. Se il primo file deve essere anteposto nella fase
   di sorting, restituisce 0. Se invece è necessario anteporre il secondo file, restituisce 1

   Non sono previsti codici di errore per questa funzione.
*/
{
   char LenStringa1, LenStringa2, MaxLen;
   char Char1, Char2;
   char Counter;
   char YouCanExit;

   if (IsDir1!=IsDir2)
   {
      if (IsDir1<IsDir2) return 1;
         else return 0;
   }
   else              // Sono entrambi file o entrambe directory
   {  
      Counter=0;
      YouCanExit=0;

      while (!YouCanExit)
      {
         Char1 = *(Ext1+Counter);
         Char2 = *(Ext2+Counter);
         
         if ( (Char2==0) && (Char2==0) ) break;
         
         if (Char1>Char2) return 1;
         if (Char1<Char2) return 0;
         
         
         // Il loop prosegue solo se i caratteri dell'estensione sono identici

         Counter++;
         if (Counter==3) YouCanExit=1;
      }
      
      LenStringa1 = strlen (Name1);
      LenStringa2 = strlen (Name2);

      if (LenStringa1>LenStringa2)
         MaxLen=LenStringa1;
      else
         MaxLen=LenStringa2;

      YouCanExit=0;
      Counter=0;

      while (!YouCanExit)
      {
         if (Counter<LenStringa1)
            Char1 = *(Name1 + Counter);
         else
            Char1 = 0;

         if (Counter<LenStringa2)
            Char2 = *(Name2 + Counter);
         else
            Char2 = 0;

         if (Char1>Char2) return 1;
         if (Char1<Char2) return 0;
         
         // Il loop prosegue solo se i caratteri sono identici

         Counter++;
         if (Counter==MaxLen) YouCanExit=1;
      }

      return 0;
   }
}






char ndDB_FillFileList (char *DirPath, struct HEADLIST_File_Type *MyListHandle)
/*
   Questa routine va nella directory individuata da DirPath ed inizia ad
   analizzare i file ivi presenti. I dati rilevati vengono memorizzati nella
   lista di file T2 individuata da MyListHandle.

   Se la directory possiede più di 2 elementi, viene eseguito un algoritmo
   di sorting delle voci, in modo da mettere all'inizio tutte le directory
   e disporre i file in ordine alfabetico.

   La routine prevede i seguenti codici di errore:
      ERR_I_CANNOT_OPEN_DIR      Non posso aprire la directory

*/

{
   int     DirHandle;
   struct  ndFS_DirEntry_Type DirData;    // Conterrà i dati delle varie entries della directory

   int     ReadStatus;
   char    ErrRep, YouCanExit, YouMustSort, SortExecuted;

   char    XName     [255];
   char    XExt      [255];

   char    QPath     [255];
   char    QName     [255];
   char    QExt      [255];

   int     LenFile;
   char    IsDir;
   char    CounterA;
   
   struct  ELEMENT_File_Type Ele1, Ele2;

   ndDB_DestroyListOfFile (MyListHandle);
   // Cancella la lista di file eventualmente già allocata in memoria ed identificata da
   // MyListHandle.
   
   ndDB_InitListOfFile (MyListHandle);
   // Inizializza la lista.

   DirHandle = ndHAL_Dopen (DirPath);
   
   if (DirHandle>=0)               // Se non ci sono stati errori nell'apertura della directory
   {
      YouCanExit=0;

      while (!YouCanExit)
      {   
         ReadStatus=ndHAL_Dread (DirHandle, &DirData);
         // Legge una directory entry. In ReadStatus c'è un valore pari  a 1 se è stato possibile leggere 
         // una entry, un valore pari a 0 se non è stato possibile leggere alcuna entry ed un valore negativo
         // in caso di errore disco.
         
         if (ReadStatus>0)   // E' stato possibile leggere una entry
         {                            
            fssSplitNameFile (DirData.d_name, QPath, QName, QExt);
            // Divide percorso, file ed estensione: comunque, visto come vengono restituiti i nomi
            // dalla routine del kernel, qui QPath sarà sempre una stringa vuota   
            
            if (strcmp (QName, ".") != 0)               // Se QName è diverso da . o ..
            {                                           // La voce . non viene mai processata, mentre
               if (strcmp (QName, "..") != 0)           // per .. si usa un diverso process
               {
                  if ( (DirData.Status.attr & 16) != 0)   // Verifica se è una directory
                      IsDir=1;
                   else
                      IsDir=0;

                   LenFile = strlen (QName);
                   ndStrCpy (XName, QName, LenFile, 1);          // Copia QName in XName convertendo i caratteri in maiuscoli
                   ndStrCpyBounded (XExt,  QExt,  0, 2, 1);      // Copia i primi 3 caratteri di QExt in XExt convertendo i caratteri in maiuscoli
                                     
                   LenFile = DirData.Status.size;
                             
                   ErrRep=ndDB_AppendFileToList (MyListHandle, XName, XExt, LenFile, IsDir, &(DirData.Status.ctime));
                   // Memorizza i dati ricavati nella lista di file di tipo T2

                   if (ErrRep!=0) YouCanExit=1;
                }
                else  // Se l'elemento è ..
                {
                   if (strcmp (ndFileManCurrPath, SysRootPath))        // Se ndFileManCurrPath è diverso da ms0:/
                   {
                       XName [0]='.';
                       XName [1]='.';
                       XName [2]=0;

                       XExt [0]=0;
                       XExt [1]=0;
                       XExt [2]=0;

                       LenFile=0;
                       IsDir=2;
                       
                       ErrRep=ndDB_AppendFileToList (MyListHandle, XName, XExt, LenFile,
                       							  IsDir, &(DirData.Status.ctime));
                       
                       if (ErrRep!=0) YouCanExit=1;
                   }
                }
             }
         }

         if (ReadStatus<=0) YouCanExit=1;
      }

      // Chiude l'handle precedentemente aperto per la directory
      ndHAL_Dclose (DirHandle);

      // Esegue un sorting della directory con l'algoritmo bubble-sort
      if (MyListHandle->NrElements>=2)
      {
         YouCanExit=0;

         while (!YouCanExit)
         {
               SortExecuted=0;

               for (CounterA=0; CounterA<=((MyListHandle->NrElements)-2); CounterA++)
               {
                   ndDB_GetFileFromList (MyListHandle, CounterA,   &Ele1);
                   ndDB_GetFileFromList (MyListHandle, CounterA+1, &Ele2);

                   YouMustSort=ndDB_CompareFile (Ele1.Name, Ele1.Ext, Ele1.IsDir, Ele2.Name, Ele2.Ext, Ele2.IsDir);
                   
                   if (YouMustSort)
                   {
                      ndDB_SwapFileInList (MyListHandle, CounterA);
                      SortExecuted=1;
                   }

               }

               if (!(SortExecuted)) YouCanExit=1;
         }
      }
            
      return 0;
   }
   else
       return ERR_I_CANNOT_OPEN_DIR;
}


