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
    ROUTINE E STRUTTURE PER LA GESTIONE DI LISTE DI INDIRIZZI
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>



void ndDB_InitList (struct HEADLIST_Generic_Type *MyListHandle)
/*
   Questa procedura viene usata per inizializzare la struttura Head of List. E' compito della
   routine chiamante allocare correttamente tale struttura: di solito essa è statica.
   Alla routine ndDB_InitList viene passato l'indirizzo della struttura.
*/
{
  MyListHandle->FirstElementAddr=0;
  MyListHandle->LastElementAddr=0;
  MyListHandle->NrElements=0;
};


char ndDB_AppendElementToList (struct HEADLIST_Generic_Type *MyListHandle, int GenericalAddr)
/*
   Questa procedura aggiunge un nuovo indirizzo di memoria ad una lista.
   La lista su cui agisce viene identificata dalla struttura *MyListHandle.
   Tale struct è di tipo HEADLIST_Generic_Type e la routine chiamante deve
   allocarlo e passare il corretto indirizzo ad ndDB_AppendElementToList.

   La routine chiamante inoltre deve allocare l'elemento dati nella memoria, e deve
   passare l'indirizzo ad ndDB_AppendElementToList, in formato int.

   Le routine che gestiscono liste ad un livello superiore a quello generic, provvedono
   ad allocare una struct in memoria che contiene i dati necessari e poi salvano il
   valore nella lista di indirizzi per mezzo di una ndDB_AppendElementToList.

   E' previsto il codice di errore
      ERR_MEMORY_OVERFLOW   Non c'è memoria sufficiente per il nuovo elemento

*/
{
  struct ELEMENT_Generic_Type  *PointerToNewElement, *PointerToListElement;
  
  PointerToNewElement = malloc  (sizeof (*PointerToNewElement));
  // Alloca lo spazio in memoria per il nuovo elemento della lista.
  
  if ( ((int)(PointerToNewElement)) != 0)
  {
     PointerToNewElement->Addr=GenericalAddr;
     PointerToNewElement->NextElementAddr=0;
     // Memorizza il dato (l'indirizzo GenericalAddr) nel nuovo elemento che è stato allocato
     // in memoria.

     if (MyListHandle->FirstElementAddr==0)
     {
        MyListHandle->FirstElementAddr=(int) (PointerToNewElement);
        MyListHandle->LastElementAddr =(int) (PointerToNewElement);
        MyListHandle->NrElements=1;

        return 0;
     }
     else
     {
         PointerToListElement=MyListHandle->LastElementAddr;
         // Fa in modo che PointerToListElement punti all'ultimo elemento della lista indirizzi

         PointerToListElement->NextElementAddr=(int) (PointerToNewElement);
         // Esegue l'append dell'elemento di tipo PointerToNewElement alla lista.

         MyListHandle->LastElementAddr = (int) (PointerToNewElement);
         MyListHandle->NrElements = MyListHandle->NrElements + 1;

         return 0;
     }
  
  
  }
  else
      return ERR_MEMORY_OVERFLOW;
};




char ndDB_GetElementFromList (struct HEADLIST_Generic_Type *MyListHandle, int NrElement, int AddressGet)
/*
   Questa routine provvede ad estrarre l'elemento nr. NrElement dalla lista identificata
   da MyListHandle. Si ricordi che la lista identificata da HeadOfListType è una lista di
   indirizzi, pertanto il risultato di questa routine è sempre un indirizzo. Tale indirizzo
   viene depositato nella locazione di memoria puntata da AddressGet.

   Il primo elemento della lista ha sempre identificatore 0.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
  struct ELEMENT_Generic_Type  *PointerToListElement;
  int *PointerToAddrGet;
  int Counter;

  if ( (MyListHandle->NrElements)!=0)
  {
     if (NrElement < (MyListHandle->NrElements) )
     {
           PointerToListElement=(MyListHandle->FirstElementAddr);
           Counter=0;

           while ( Counter < NrElement )
           {
               PointerToListElement=(PointerToListElement->NextElementAddr);
               Counter++;
           }

           PointerToAddrGet = AddressGet;
           // Trasferire l'indirizzo in una variabile locale prima di trasferire i dati
           // impedisce un malfunzionamento del compilatore.

           *PointerToAddrGet = (int) (PointerToListElement->Addr);

           return 0;
     }
     else
         return ERR_NR_ELEMENT_IS_INVALID;
  }
  else
      return ERR_LIST_IS_EMPTY;
}



char ndDB_GetLastElementFromList (struct HEADLIST_Generic_Type *MyListHandle, int AddressGet)
/*
   Questa routine provvede ad estrarre l'ultimo elemento in basso della lista identificata
   da MyListHandle. Si ricordi che la lista identificata da HeadOfListType è una lista di
   indirizzi, pertanto il risultato di questa routine è sempre un indirizzo. Tale indirizzo
   viene depositato nella locazione di memoria puntata da AddressGet.

   E' previsto il codice di errore:
     ERR_LIST_IS_EMPTY             La lista è vuota
*/
{
  struct ELEMENT_Generic_Type  *PointerToListElement;
  int *PointerToAddrGet;

  if ( (MyListHandle->NrElements)!=0)
  {
     PointerToListElement=(MyListHandle->LastElementAddr);

     PointerToAddrGet = AddressGet;
     // Trasferire l'indirizzo in una variabile locale prima di trasferire i dati
     // impedisce un malfunzionamento del compilatore.

     *PointerToAddrGet = (int) (PointerToListElement->Addr);

     return 0;
  }
  else
   return ERR_LIST_IS_EMPTY;
}




char ndDB_SwapElementInList (struct HEADLIST_Generic_Type *MyListHandle, int NrElement)
/*
   Questa routine provvede a scambiare di posto l'elemento nr. NrElement
   e quello NrElement+1.

   Non è consentito di porre NrElement al numero di elementi attuale della lista.

   Sono previsti i seguenti codici di errore:
        ERR_SWAP_IS_NOT_ALLOWED    Ci sono meno di due elementi
        ERR_NR_ELEMENT_IS_INVALID  Il numero di elemento è invalido
*/
{
  struct ELEMENT_Generic_Type  *PointerToListElement, *PointerToSecondListElement;
  int Counter, Swap;

  if ( (MyListHandle->NrElements)>=2 )
  // Se ci sono meno di 2 elementi non si può fare lo swap
  {
     if (NrElement <= ((MyListHandle->NrElements)-2) )
     // Se fossero ammessi tutti i numeri, poichè quando NrElements=5 il massimo numero
     // che si può passare alla funzione è 4 (da 0 a 4), allora dovrei scrivere
     // minore uguale di MyListHandle->NrElements-1. Ma poichè l'ultimo elemento
     // non è ammesso, allora devo porre la condizione minore o uguale di
     // (MyListHandle->NrElements)-2.
     {
           PointerToListElement=(MyListHandle->FirstElementAddr);
           Counter=0;

           while ( Counter < NrElement )
           {
               PointerToListElement=(PointerToListElement->NextElementAddr);
               Counter++;
           }

           PointerToSecondListElement=(PointerToListElement->NextElementAddr);
           // Non può essere zero, grazie alle condizioni che abbiamo posto

           Swap = PointerToSecondListElement->Addr;
           PointerToSecondListElement->Addr = PointerToListElement->Addr;
           PointerToListElement->Addr = Swap;

           // Lo scambio di indirizzi cambia l'ordine con cui compaiono i
           // contenuti associati alla lista

           return 0;
     }
     else
         return ERR_NR_ELEMENT_IS_INVALID;
  }
  else
      return ERR_SWAP_IS_NOT_ALLOWED;
}


char ndDB_RemoveLastElementFromList (struct HEADLIST_Generic_Type *MyListHandle)
/*
   Questa routine provvede a rimuovere l'ultimo elemento dalla lista identificata
   da MyListHandle.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è vuota
*/
{
  struct ELEMENT_Generic_Type  *PointerToListElement, *PointerToPreviousListElement;
  
  if ( (MyListHandle->NrElements)>0 )
  {
            PointerToListElement         = MyListHandle->FirstElementAddr;
            // Fa in modo che PointerToListElement punti al primo elemento della lista indirizzi

            if ( MyListHandle->NrElements == 1 )
            {
                 // In questo caso significa che la lista ha solo un elemento

                 free (PointerToListElement);
                 // Disalloca i dati relativi al primo ed ultimo indirizzo

                 MyListHandle->NrElements=0;
                 MyListHandle->FirstElementAddr=0;
                 MyListHandle->LastElementAddr=0;
            }
            else
            {
                 PointerToListElement = MyListHandle->FirstElementAddr;

                 while ( (PointerToListElement->NextElementAddr) != 0)
                 {
                       PointerToPreviousListElement = PointerToListElement;
                       PointerToListElement         = PointerToListElement->NextElementAddr;
                 }

                 free (PointerToListElement);
                 // Disalloca dalla memoria i dati relativi all'ultimo indirizzo.

                 PointerToPreviousListElement->NextElementAddr=0;
                 // Pone a zero il riferimento all'elemento successivo nel penultimo elemento

                 MyListHandle->LastElementAddr = (int) (PointerToPreviousListElement);
                 MyListHandle->NrElements = MyListHandle->NrElements - 1;
            }

            return 0;

  }
  else
      return ERR_LIST_IS_EMPTY;
}


void ndDB_DestroyList (struct HEADLIST_Generic_Type *MyListHandle)
/*
   Questa routine provvede a disallocare tutti gli indirizzi che sono memorizzati in una lista.
   La lista da cancellare viene specificata dalla struttura HEADLIST_Generic_Type. La procedura
   chiamante deve passare l'indirizzo di tale procedura

   Non sono previsti codici di errore per questa routine.
*/

{
  int Counter;
  int OriginalNrElements;

  OriginalNrElements=MyListHandle->NrElements;

  for (Counter=0; Counter<OriginalNrElements; Counter++)
  {
     ndDB_RemoveLastElementFromList (MyListHandle);
  }
}

