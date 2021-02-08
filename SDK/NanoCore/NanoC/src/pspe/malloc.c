/*
   --------------------------------------------------------------------------
   malloc.c                                      NanoC Files (PSPE)
   --------------------------------------------------------------------------
   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   ----------------------------------------------------------------------------
   Routines di gestione della memoria 
   ----------------------------------------------------------------------------
*/   

#define DED_MALLOC_I_NEED_VARIABLES
     #include <pspe/$$_ded_malloc.h>
#undef  DED_MALLOC_I_NEED_VARIABLES

/* -----------------------------------------------------------------------------
   Origin: Filippo Battaglia                                     MALLOC EMULATOR
   -----------------------------------------------------------------------------
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
   -----------------------------------------------------------------------------
   Note: The malloc emulator has been introduced because PSPSDK routines for malloc/free
   don't work under PSPE emulator.
   -----------------------------------------------------------------------------
*/

//   Routine di inizializzazione e deinizializzazione dell'emulatore malloc  

void _pspsdk_alloc_init()
{
     int Counter;
     
     if (!_PSPSDK_MemoryInitExecuted)
     {
        MaxMemoryAllocable =MAXMEMORY-1;
        MemoryAvail        =MAXMEMORY-1;
     
        for (Counter=0; Counter<MAXHANDLESALLOCABLE; Counter++)
        {
            MemoryHandle [Counter].Start=0;
            MemoryHandle [Counter].End = 0;
        }
     
        NrHandlesAllocated=0;
     
        for (Counter=0; Counter<MAXMEMORY; Counter++)
        {
            MemoryArea [Counter]=0;
        }
        
        _PSPSDK_MemoryInitExecuted=1;
     }
}      

void _pspsdk_alloc_deinit()
{
   _PSPSDK_MemoryInitExecuted=0;
}

 
// Routine per la gestione della memoria (emulatore malloc)    

static inline char INTERNAL_GenerateAlignedAdr (int Size, int Alligned, int Limit1, int Limit2, int *ZoneBegin, int *ZoneEnd)
/*
   Questa routine genera un indirizzo che sia allineato secondo il criterio
   alligned e che sia compreso tra le celle di memoria array Limit1 e Limit2.
*/
{
   register int AdrLimit1, AdrLimit2;
   register int AllignedCell, TmpAllignedCell; 
   register int AllignedAdr,  SecAllignedAdr;
   register int Offset, MemAv;
   
   AdrLimit1 = (int)(&(MemoryArea [Limit1]));          // Trasforma l'indirizzo nella MemoryArea
   AdrLimit2 = (int)(&(MemoryArea [Limit2]));          // nel relativo indirizzo ram
   
   if (Limit2-Limit1>0)
   {         
           MemAv = AdrLimit2-AdrLimit1;
           // Se AdrLimit1 è 10 e AdrLimit2 è 11, lo spazio disponibile è 1 byte
           
           if (MemAv>=Size)               // L'area di memoria disponibile è sufficiente
           {
               // Siccome il processore MIPS deve sempre trattare dati allineati a 4,
               // è necessario un doppio processo di allineamento. Prima si allinea
               // al valore richiesto dall'utente e poi si allinea a 4.
               
               Offset = AdrLimit1 % Alligned;     // Allineamento ai valori richiesti dall'utente
               
               if (Offset==0)                     // I dati sono già allineati
                  TmpAllignedCell = Limit1;
               else
                  TmpAllignedCell = Limit1 + (Alligned - Offset); 
               
               Offset = TmpAllignedCell % 4;      // Allineamento a 4     
               
               if (Offset==0)                     // I dati sono già allineati
                  AllignedCell = TmpAllignedCell;
               else
                  AllignedCell = TmpAllignedCell + (4 - Offset); 
               
               AllignedAdr    = (int) (&(MemoryArea [AllignedCell]));
               SecAllignedAdr = AllignedAdr+Size;
                   
               if (SecAllignedAdr<AdrLimit2)     // L'algoritmo di allocazione scarta in automatico l'ultimo byte 
               {
                   *ZoneBegin = AllignedCell;
                   *ZoneEnd   = AllignedCell+Size;
                   // Se è stato richiesto un byte, l'inizio è 100, ma la fine è 101
                   // perchè l'algoritmo di allocazione scarta l'ultimo byte
                   
                   return 1;       // Segnala successo               
               }
               else
               {
                   return 0;
                   // Lo spazio in teoria sarebbe stato sufficiente, però una volta
                   // eseguito l'allineamento, si è scoperto che in realtà non era così...
               } 
           }
           else   // L'intervallo di memoria non è sufficiente per allocare la memoria richiesta
              return 0;
   }
   else
      return 0;
}



void *memalign(size_t align, size_t size)
{
     register unsigned int CounterA, CounterB;
     register unsigned int ByteRest;
     
     int  ZoneBegin, ZoneEnd;
     register char Result;
     
     
     if (!_PSPSDK_MemoryInitExecuted) _pspsdk_alloc_init ();
     // Provvede ad eseguire automaticamente l'inizializzazione del sistema di gestione
     // della memoria, liberando il programma chiamante dall'onere di richiamare 
     // espressamente la funzione
     
     if (align==0) align=1;   
     // Un memalign con align zero è equivalente ad una malloc (ossia ad una malign con allign 1)
     
     if (  (size>MemoryAvail) || (NrHandlesAllocated == MAXHANDLESALLOCABLE)  )  return 0;
         // La routine si deve comportare come la malloc prevista nella
         // normale libc. Se si richiede di allocare più memoria di quella
         // che effettivamente è presente, allora vuol dire che la routine
         // fallirà. Questo si simula facendo in modo che in uscita sia
         // restituito il valore 0.
         // Stesso identico discorso se il numero di handles allocati è
         // già pari al numero massimo di handles allocabili.
     
     if (NrHandlesAllocated == 0)            // Se non è stato allocato alcun altro handle, allora tutto quello
     {                                       // che si deve fare è allocare il nuovo handle. La memoria basterà,
                                             // visto che già prima si è visto che la richiesta era minore di
                                             // MemoryAvail.
            
            Result = INTERNAL_GenerateAlignedAdr (size, align, 0, MAXMEMORY, &ZoneBegin, &ZoneEnd);
            
            if (Result)                      // E' riuscito a generare l'indirizzo allineato
            {
                 MemoryHandle [0].Start   = (void *)(ZoneBegin);
                 MemoryHandle [0].End     = (void *)(ZoneEnd);
                 MemoryHandle [0].Aligned = align;
            
                 NrHandlesAllocated++;
                 MemoryAvail=MemoryAvail-size;
            
                 ByteRest=(int)(MemoryHandle [0].Start);
                 return &(MemoryArea [ByteRest]);
            }
            else           
              return 0;    // Segnala errore
     }
     else
     {
         // Se è stato allocato almeno un handle, la prima analisi che deve essere fatta, riguarda lo spazio
         // tra l'inizio della memoria e l'inizio dell'handle che è stato allocato
         // L'area libera all'inizio è data dalla differenza tra il primo byte dell'area di memoria determinato
         // dal primo handle e zero, ossia dall'inizio dell'area di memoria puntata dal primo handle.
         
         Result = INTERNAL_GenerateAlignedAdr (size, align, 0, (int)(MemoryHandle [0].Start), &ZoneBegin, &ZoneEnd);
         
         if (Result)   // Abbiamo trovato uno spazio libero all'inizio
         {        
                  for ( CounterB=(MAXHANDLESALLOCABLE-2); CounterB>=0; CounterB-- )
                      // Il ciclo di spostamento deve essere eseguito dal penultimo elemento del vettore degli handles
                      // (che sarà copiato nell'ultimo) al primo handle allocato. Ognuno di essi sarà copiato in Counter+1, 
                      // in modo che il primo handle (quello di codice 0) rimanga libero per noi.
                      
                      // Attenzione perchè il vettore degli handles ha dimensione MAXHANDLESALLOCABLE.
                      // Questo significa che l'ultimo elemento del vettore ha nr. di ordine 
                      // MAXHANDLESALLOCABLE-1 ed il penultimo ha nr. di ordine MAXHANDLESALLOCABLE-2.
                      
                      // Si noti che è impossibile che ci sia una perdita di handles, perchè l'ultimo handle
                      // di sicuro è vuoto. Ciò è stato già verificato, quando abbiamo controllato che il nr. di handles
                      // già allocati non fosse pari a NrHandlesAllocable.
                      
                  {
                      MemoryHandle [CounterB+1].Start   = MemoryHandle [CounterB].Start;
                      MemoryHandle [CounterB+1].End     = MemoryHandle [CounterB].End;
                      MemoryHandle [CounterB+1].Aligned = MemoryHandle [CounterB].Aligned;
                  }
                  
                  MemoryHandle [0].Start   = (void *)(ZoneBegin);
                  MemoryHandle [0].End     = (void *)(ZoneEnd);
                  MemoryHandle [0].Aligned = align;
                  
                  NrHandlesAllocated++;
                  MemoryAvail=MemoryAvail - size;
                  
                  ByteRest=(int)(MemoryHandle [0].Start);
                  return &(MemoryArea [ByteRest]);
         }
         
         // Se siamo arrivati a questo punto del ciclo, vuol dire che non c'era spazio all'inizio
         // della MemoryArea. Proviamo nelle aree di memoria tra i vari handles.
         
    
         CounterA=0;
            
         while ( (CounterA < MAXHANDLESALLOCABLE) && (CounterA < (NrHandlesAllocated-1) ) )
         // Questo ciclo sarà eseguito fin quando saranno verificate due condizioni:
         //        a) il contatore ha un valore inferiore al numero di handles allocabili
         //        b) non stiamo analizzando l'ultimo handle allocato (per il quale serve un diverso trattamento)


         // Si osservi che se abbiamo allocato 1 handle, questo ha numero d'ordine 0. Quindi, l'ultimo handle
         // allocato lo abbiamo quando CounterA ha valore pari a NrHandlesAllocated-1.         
         
         {
             
             Result = INTERNAL_GenerateAlignedAdr (size, align, (int)(MemoryHandle [CounterA].End), (int)(MemoryHandle [CounterA+1].Start), &ZoneBegin, &ZoneEnd);
             
             if (Result)     // Abbiamo trovato uno spazio libero. Inseriamo un handle per bloccarlo                     
             { 
                   
                  for ( CounterB=(MAXHANDLESALLOCABLE-2); CounterB>=(CounterA+1); CounterB-- )
                      // Il ciclo di spostamento deve essere eseguito dal penultimo elemento del vettore degli handles
                      // (che sarà copiato nell'ultimo) all'handle immediatamente successivo a Counter (ossia l'handle
                      // di ordine Counter+1, che sarà copiato in Counter+2, in modo che Counter+1 rimanga libero per
                      // noi). 
                      
                      // Attenzione perchè il vettore degli handles ha dimensione MAXHANDLESALLOCABLE.
                      // Questo significa che l'ultimo elemento del vettore ha nr. di ordine 
                      // MAXHANDLESALLOCABLE-1 ed il penultimo ha nr. di ordine MAXHANDLESALLOCABLE-2.
                      
                      // Si noti che è impossibile che ci sia una perdita di handles, perchè l'ultimo handle
                      // di sicuro è vuoto. Ciò è stato già verificato, quando abbiamo controllato che il nr. di handles
                      // già allocati non fosse pari a NrHandlesAllocable.
                      
                  {
                      MemoryHandle [CounterB+1].Start   = MemoryHandle [CounterB].Start;
                      MemoryHandle [CounterB+1].End     = MemoryHandle [CounterB].End;
                      MemoryHandle [CounterB+1].Aligned = MemoryHandle [CounterB].Aligned;
                  }
                  
                  // La memoria che serve viene bloccata allocandola nell'handle individuato da Counter+1. 
                  
                  MemoryHandle [CounterA+1].Start   = (void *)(ZoneBegin);
                  MemoryHandle [CounterA+1].End     = (void *)(ZoneEnd);
                  MemoryHandle [CounterA+1].Aligned = align;
                  
                  NrHandlesAllocated++;
                  MemoryAvail=MemoryAvail-size;
                  
                  ByteRest=(int)(MemoryHandle [CounterA+1].Start);                     
                  return &(MemoryArea [ByteRest]);
                  
             }
         
             CounterA++;
         }
         
         // Se si è arrivati a questo livello del ciclo, vuol dire che CounterA è pari al numero di handles allocati. 
         // Significa che stiamo analizzando l'ultimo handle, per il quale è necessario un diverso trattamento.
         
         Result = INTERNAL_GenerateAlignedAdr (size, align, (int)(MemoryHandle [CounterA].End), MAXMEMORY, &ZoneBegin, &ZoneEnd);
         
         if (Result) 
         {
             MemoryHandle [CounterA+1].Start   = (void *)(ZoneBegin);
             MemoryHandle [CounterA+1].End     = (void *)(ZoneEnd);
             MemoryHandle [CounterA+1].Aligned = align;
                            
             NrHandlesAllocated++;
             MemoryAvail=MemoryAvail-size;
             
             ByteRest = (int)(MemoryHandle [CounterA+1].Start);
             return &(MemoryArea [ByteRest]);
         }
         
         // Se si è arrivati a questo punto del ciclo, allora significa che non c'era davvero nessuna area di memoria libera
         // per eseguire l'allocazione. Il processo di memalign fallisce ed il sistema restituisce il valore 0.
         
         return 0;
     } 
}




void *malloc(size_t Size)
{
     return memalign (1, Size);                     
}


int free (void *Pointer)
{     
  unsigned int PointerToMemoryArea, AddressRequest;
  unsigned int HandleDelete, SizeDelete;
  unsigned short int CounterA, CounterB;
     
  if (!_PSPSDK_MemoryInitExecuted) _pspsdk_alloc_init ();
  // Provvede ad eseguire automaticamente l'inizializzazione del sistema di gestione
  // della memoria, liberando il programma chiamante dall'onere di richiamare 
  // espressamente la funzione
         
  if ( (_PSPSDK_MemoryInitExecuted) && (NrHandlesAllocated>=1) )
  {
     PointerToMemoryArea = (unsigned int) (& (MemoryArea [0]) );
     AddressRequest = (unsigned int) (Pointer);
     
     HandleDelete=AddressRequest-PointerToMemoryArea;
                  
     for (CounterA=0; CounterA<=(NrHandlesAllocated-1); CounterA++)
     {
         if ( (int)(MemoryHandle [CounterA].Start) == HandleDelete )
            // Abbiamo trovato l'handle da cancellare
         {
               SizeDelete = (int)(MemoryHandle [CounterA].End) - (int)(MemoryHandle [CounterA].Start);
               
               for (CounterB=CounterA; CounterB<=(NrHandlesAllocated-1); CounterB++)
               {
                   MemoryHandle [CounterB].Start = MemoryHandle [CounterB+1].Start;
                   MemoryHandle [CounterB].End   = MemoryHandle [CounterB+1].End;
               }
               
               MemoryAvail=MemoryAvail+SizeDelete;
               NrHandlesAllocated--;
               
               return 0;
         }     
     }
     
     return -1;
  
  }   
  else
     return (-1);       // Segnala l'errore al processo chiamante
}
               

void *realloc (void *Pointer, size_t NewDimension)
{
    unsigned int PointerToMemoryArea, AddressRequest;
    unsigned int HandleToChange;
    void *NewPointer;
    
    unsigned int OldDimension, OldAligned;
    unsigned char *PointerToByteA, *PointerToByteB;
    
    int CounterHdr, Counter;
    
    if (!_PSPSDK_MemoryInitExecuted) _pspsdk_alloc_init ();
     // Provvede ad eseguire automaticamente l'inizializzazione del sistema di gestione
     // della memoria, liberando il programma chiamante dall'onere di richiamare 
     // espressamente la funzione
     
    if (Pointer==0)          // Per convenzione, una realloc con puntatore pari a 0 equivale
    {                        // ad una malloc
       return malloc (NewDimension);
    }
    
    if (NewDimension!=0)             // La nuova Size richiesta non è nulla
    {
       PointerToMemoryArea = (unsigned int) (& (MemoryArea [0]) );
       AddressRequest      = (unsigned int) (Pointer);
       HandleToChange      = AddressRequest - PointerToMemoryArea;
    
       for (CounterHdr=0; CounterHdr<=(NrHandlesAllocated-1); CounterHdr++)
       {
             if ( (int)(MemoryHandle [CounterHdr].Start) == HandleToChange )
             // Abbiamo trovato l'handle da cambiare
             {
                OldDimension = MemoryHandle [CounterHdr].End - MemoryHandle [CounterHdr].Start;
                // Questa è la dimensione del vecchio handle
                
                OldAligned  = MemoryHandle [CounterHdr].Aligned;
                // Questa è la vecchia direttiva di allineamento
                
                NewPointer = memalign (OldAligned, NewDimension);
                // Alloca una nuova area in memoria per i nuovi dati
                
                if (NewPointer!=0)   // L'allocazione è andata a buon fine
                {
                    if (NewDimension>OldDimension)     // Si è richiesto l'allargamento dell'area di memoria
                    {
                         memset (NewPointer, 0, NewDimension);
                         // Riempi di zero la nuova area di memoria
                
                         for (Counter=0; Counter<OldDimension; Counter++)
                         {
                             PointerToByteA = Pointer + Counter;
                             PointerToByteB = NewPointer + Counter;
                       
                             *PointerToByteB = *PointerToByteA;
                         }
                    }
                    else     // La nuova area di memoria è più piccola: copia solo quanto necessario a riempire la
                    {        // nuova dimensione: il resto dei dati viene perso
                    
                         for (Counter=0; Counter<NewDimension; Counter++)
                         {
                             PointerToByteA = Pointer + Counter;
                             PointerToByteB = NewPointer + Counter;
                       
                             *PointerToByteB = *PointerToByteA;
                         }    
                    } 
                
                    free (Pointer);          // Disalloca il vecchio blocco di memoria
                    return NewPointer;
                }
                else         // Il processo di allocazione del nuovo blocco è fallito
                  return 0;  // Segnala errore alla routine chiamante       
             }     
       }
       
       // Se siamo arrivati a questo punto del ciclo, significa che l'handle 
       // non è stato trovato. La realloc è equivalente ad una malloc a questo
       // punto
       
       return malloc (NewDimension);
     
    }
    else      // Una realloc con size=0 equivale ad una free.
    {
       free (Pointer);
       return 0;
    } 
} 
    
void *calloc(size_t n, size_t size)
{
	register void *ptr = NULL;
	register size_t sz = n * size;

    ptr = malloc (sz);
    if (ptr==NULL) return 0;
   
	memset(ptr, 0, sz);
	return ptr;
}





int memfree ()
/*
    Restituisce la memoria disponibile nel sistema
*/

{
    return MemoryAvail;
}
