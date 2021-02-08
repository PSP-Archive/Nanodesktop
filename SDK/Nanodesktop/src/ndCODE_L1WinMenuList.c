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
    TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI L1WMI
    
    Queste routine sono usate solo dall'API ndWMENU per la gestione interna
    della memoria. Non sono accessibili all'utente.
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>
                                          
void ndDB_InitList_of_L1WMI (struct HEADLIST_L1WMI_Type *MyListHandle, char Dynamic)
/*
   Questa procedura viene usata per inizializzare una struttura che dovrà contenere un elenco
   di voci per un winmenu di livello 1. Tali voci sono dette L1WMI (LevelOne WinMenu Item)
   
   La routine chiamante deve allocare una struttura di tipo HEADLIST_L1WMI_Type. E' compito della
   routine chiamante allocare correttamente tale struttura: di solito essa è statica.
   Alla routine ndDB_InitList viene passato l'indirizzo della struttura.
   
   Il parametro Dynamic segnala al sistema se la struttura di tipo HEADLIST_L1WMI_Type è
   di tipo dinamico.
*/
{
  MyListHandle->FirstElementAddr=0;
  MyListHandle->LastElementAddr=0;
  MyListHandle->NrElements=0;
  MyListHandle->Dynamic=Dynamic;
};



char ndDB_IsPresentSameStringID_in_L1WMI_List (struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID)
/* 
   Ogni L1WMI è caratterizzata da un identificatore chiamato StringID. Questa 
   routine verifica se un L1WMI è già presente nella lista con il medesimo  
   StringID. 
   
   Il WinMenu analizzato è quello puntato dalla struct MyListHandle. 
   
   Se è già presente la voce, la routine restituisce 1, altrimenti restituisce 0.
*/ 

{
   struct ELEMENT_L1WMI_Type            *PointerTo_L1WMI;
   struct ELEMENT_Generic_Type          *PointerToAddr_L1WMI;
   
   int PointerToRam;
   char YouCanExit;
   
   if (MyListHandle->NrElements>0)
   {
       PointerToRam           = MyListHandle->FirstElementAddr;
       PointerToAddr_L1WMI    = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente MyListHandle->FirstElementAddr a PointerToAddr_L1WMI 
       // ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         // In PointerToAddr_L1WMI c'è il puntatore all'elemento della lista
         // di indirizzi.     
             
         PointerTo_L1WMI    = PointerToAddr_L1WMI->Addr;
         // In PointerTo_L1WMI ci sono i dati del blocco collegato: questa
         // variabile punta alle caratteristiche della voce di menù.
         
         if ( strcmp (PointerTo_L1WMI->StringID, StringID) == 0 )
           return 1;
         
         if (PointerToAddr_L1WMI->NextElementAddr!=0)
           PointerToAddr_L1WMI = PointerToAddr_L1WMI -> NextElementAddr;
         else
           YouCanExit=1;
       }
       
       return 0;     
   }
   else
    return 0;
    
}                   



char ndDB_Append_L1WMI_ToList (struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID,
                                       char *NameItem, char Item_Inhibited, char ExecInNewContext, 
                                       char Item_Expanded, void *CallbackFunction) 
/* 
   Questa routine alloca uno spazio in memoria e vi memorizza una nuovo elemento 
   della lista di L1WMI.
   
   Ogni voce della lista L1WMI è caratterizzata da una StringID, che la 
   identifica in modo univoco. Se nella lista c'è già una voce con la medesima
   StringID, la routine restituirà un errore.
   
   Sono ammessi StringID e NameItem non superiori a 128 caratteri. 
   
   Sono previsti i codici di errore:
      ERR_MEMORY_OVERFLOW               La memoria non è sufficiente
      ERR_MEMORY_ERROR                  Errore nella memoria
      ERR_STRINGID_TOO_LONG             La StringID è troppo lunga
      ERR_NAMEITEM_TOO_LONG             Il NameItem è troppo lungo
      ERR_SAME_L1WMI_IS_PRESENT         E' già presente la medesima NameClass.
      
*/

{      
   struct ELEMENT_L1WMI_Type *PointerToNew_L1WMI;
   char MemErrRep;
   unsigned char LenStringIDItem, LenNameItem;
   
   LenStringIDItem = strlen (StringID);
   LenNameItem     = strlen (NameItem);
   
   if (LenStringIDItem<128)     // Controlla che la lunghezza di StringID sia corretta          
   {
      if (LenNameItem < 128)    // Controlla che la lunghezza di LenNameItem sia corretta
      {
              if (ndDB_IsPresentSameStringID_in_L1WMI_List (MyListHandle, StringID)==0)    // Se la StringID non è già presente
              {
                 PointerToNew_L1WMI = malloc  ( sizeof (*PointerToNew_L1WMI) );
                 // Alloca in memoria lo spazio per memorizzare le informazioni relative alla nuova voce
                 // di menù.
          
                 if (PointerToNew_L1WMI!=0)     // L'allocazione ha avuto successo
                 {                 
                    ndStrCpy (PointerToNew_L1WMI->StringID, StringID, LenStringIDItem, 1);
                    // Copia la string ID nello spazio allocato
                                   
                    ndStrCpy (PointerToNew_L1WMI->NameItem, NameItem, LenNameItem, 0);
                    // Copia il nome della voce
                    
                    PointerToNew_L1WMI->Item_Inhibited = Item_Inhibited;
                    // Copia il valore del campo Item_Inhibited
                    
                    PointerToNew_L1WMI->Item_Expanded = Item_Expanded;
                    // Copia il valore del campo Item_Expanded
                    
                    PointerToNew_L1WMI->ExecInNewContext = ExecInNewContext;
                    // Copia il valore del campo ExecInNewContext
                    
                    PointerToNew_L1WMI->L1SubMenu = 0;
                    // Memorizza l'informazione che indica che non è stato creato alcun submenu di livello 1
                    
                    PointerToNew_L1WMI->L2SubMenu = 0;
                    // Memorizza l'informazione che indica che non è stato creato alcun submenu di livello 1
                    
                    PointerToNew_L1WMI->CallbackFunction = CallbackFunction;
                    // Memorizza l'indirizzo dell'eventuale call-back
                                                                                      
                    MemErrRep=ndDB_AppendElementToList (MyListHandle, PointerToNew_L1WMI);
                    // Memorizza l'indirizzo del nuovo elemento nella lista di indirizzi.
        
                    if (MemErrRep!=0)        // Il tentativo di appendere il nuovo elemento può essere fallito.
                    {                        // In tal caso si deve disallocare l'area dove abbiamo copiato le informazioni relative al nuovo Class
                       free ( PointerToNew_L1WMI );
                       return ERR_MEMORY_ERROR;
                    }
                    else
                      return 0;
                                        
                 }
                 else
                   return ERR_MEMORY_OVERFLOW;
                                                      
              }
              else
                return ERR_SAME_L1WMI_IS_PRESENT;
   
        }
        else ERR_NAMEITEM_IS_TOO_LONG;
   }       
   else
     return ERR_STRINGID_TOO_LONG;     
}



char ndDB_GetL1WMIFromList ( struct HEADLIST_L1WMI_Type *MyListHandle, int NrElement, struct ELEMENT_L1WMI_Type *MyL1WMI)
/*
   Supponiamo che la routine chiamante abbia allocato come variabile locale una struttura di tipo
   ELEMENT_L1WMI_Type.

   La routine ndDB_GetL1WMIFromList copia i dati relativi all'item di L1WinMenu di numero
   d'ordine NrElement ed appartenente al menù individuato dall'handle MyListHandle nella 
   struttura *MyL1WMI. 
   
   La routine chiamante deve avere allocato un'opportuna struttura di tipo ELEMENT_L1WMI_Type 
   atta a contenere le informazioni e deve passarne l'indirizzo per mezzo del parametro MyL1WMI.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
   struct ELEMENT_L1WMI_Type *PointerToL1WMI;
   char ErrRep;
   int PointerToRam;
   
   ErrRep=ndDB_GetElementFromList (MyListHandle, NrElement, &PointerToRam );
   // Deposita dentro PointerToRam l'indirizzo dell'n-esimo elemento.

   PointerToL1WMI = PointerToRam;
   // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
   // direttamente &PointerToL1WMI alla routine precedente, ma si verifica un errore.

   if (!ErrRep)
   {
      // La routine provvede a copiare i dati dalla loro posizione originale in Ram, al buffer
      // puntato da MyL1WMI. Questo buffer in genere è una struttura che è stata allocata dalla
      // routine chiamante (magari in stack).

      ndStrCpy (MyL1WMI->StringID, PointerToL1WMI->StringID, 127, 0);
      ndStrCpy (MyL1WMI->NameItem, PointerToL1WMI->NameItem, 127, 0);
      
      MyL1WMI->Item_Inhibited   = PointerToL1WMI->Item_Inhibited;
      MyL1WMI->Item_Expanded    = PointerToL1WMI->Item_Expanded;
      MyL1WMI->L1SubMenu        = PointerToL1WMI->L1SubMenu;
      MyL1WMI->L2SubMenu        = PointerToL1WMI->L2SubMenu;
      MyL1WMI->CallbackFunction = PointerToL1WMI->CallbackFunction;
      
      return 0;
   }
   else
     return ErrRep;

}

char ndDB_GetL1WMIAddrFromList ( struct HEADLIST_L1WMI_Type *MyListHandle, int NrElement, int *AddressGet)
/*
   Questa routine fornisce l'indirizzo di memoria Ram in cui sono memorizzati i dati relativi
   alla voce della lista NrElement.

   Tale indirizzo di memoria viene memorizzato nel puntatore AddressGet. Tale puntatore deve
   essere già stato allocato dalla routine chiamante.

   La routine agisce sul puntatore, **non** sul contenuto del puntatore. Pertanto, un
   corretto modello di chiamata può essere:

   ELEMENT_L1WMI_Type *Adr;
   ndDB_GetL1WMIAddrFromList (&MyL1WMIHandle, 0, Adr);

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
   return ndDB_GetElementFromList (MyListHandle, NrElement, AddressGet);
   // Pone in AddressGet l'indirizzo dell'elemento richiesto.
}



char ndDB_GetL1WMIFromList_UsingStringID ( struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID, 
                                           struct ELEMENT_L1WMI_Type *MyL1WMI)
/*
   La routine ndDB_GetL1WMIFromList_UsingStringID restituisce i dati relativi all'L1WMI che
   appartiene al menu puntato da MyListHandle e caratterizzato da una data StringID. 
   
   La routine chiamante deve avere allocato un'opportuna struttura di tipo ELEMENT_L1WMI_Type 
   atta a contenere le informazioni e deve passarne l'indirizzo per mezzo del parametro MyL1WMI.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_L1WMI_NOT_FOUND          La L1WMIe con quel NameL1WMI non è stata trovata
*/
{        
   struct ELEMENT_L1WMI_Type   *PointerToL1WMI;
   struct ELEMENT_Generic_Type *PointerToAddrL1WMI;
   
   int PointerToRam;
   char YouCanExit;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, StringID, strlen (StringID), 1);
       // Trasforma il NameL1WMI in caratteri maiuscoli                           
                                  
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrL1WMI = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToL1WMI alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToL1WMI     = PointerToAddrL1WMI->Addr;
         // In PointerToL1WMI ci sono i dati relativi al L1WMI, in PointerToAddrL1WMI ci
         // sono i dati relativi all'indirizzo dei dati del L1WMI ed all'indirizzo del
         // L1WMI successivo 
    
         if ( strcmp (PointerToL1WMI->StringID, InternalBuffer) == 0 )  // abbiamo trovato L1WMI giusto
         {
           ndStrCpy (MyL1WMI->StringID, PointerToL1WMI->StringID, 127, 0);
           ndStrCpy (MyL1WMI->NameItem, PointerToL1WMI->NameItem, 127, 0);
      
           MyL1WMI->Item_Inhibited   = PointerToL1WMI->Item_Inhibited;
           MyL1WMI->Item_Expanded    = PointerToL1WMI->Item_Expanded;
           MyL1WMI->L1SubMenu        = PointerToL1WMI->L1SubMenu;
           MyL1WMI->L2SubMenu        = PointerToL1WMI->L2SubMenu;
           MyL1WMI->CallbackFunction = PointerToL1WMI->CallbackFunction;
      
           return 0;
         }
         
         if (PointerToAddrL1WMI->NextElementAddr!=0)
           PointerToAddrL1WMI = PointerToAddrL1WMI->NextElementAddr;
         else
           return ERR_L1WMI_NOT_FOUND;
           
       }  // End of while
        
   }
   else
    return ERR_LIST_IS_EMPTY;

}


char ndDB_GetL1WMIAddrFromList_UsingStringID ( struct HEADLIST_L1WMI_Type *MyListHandle, char *StringID, 
                                               int *AddressGet)
/*
   La routine ndDB_GetL1WMIAddrFromList_UsingStringID restituisce l'indirizzo in ram dei 
   dati relativi all'L1WMI che appartiene al menu puntato da MyListHandle e 
   caratterizzato da una data StringID. 
   
   La routine chiamante deve allocare una variabile atta a contenere un indirizzo a 32 bit,
   e deve passare l'indirizzo di tale variabile per mezzo del parametro AddressGet.
   
   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_L1WMI_NOT_FOUND          La L1WMIe con quel NameL1WMI non è stata trovata
*/
{        
   struct ELEMENT_L1WMI_Type   *PointerToL1WMI;
   struct ELEMENT_Generic_Type *PointerToAddrL1WMI;
   
   int PointerToRam;
   char YouCanExit;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, StringID, strlen (StringID), 1);
       // Trasforma il NameL1WMI in caratteri maiuscoli                           
                                  
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrL1WMI = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToL1WMI alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToL1WMI     = PointerToAddrL1WMI->Addr;
         // In PointerToL1WMI ci sono i dati relativi al L1WMI, in PointerToAddrL1WMI ci
         // sono i dati relativi all'indirizzo dei dati del L1WMI ed all'indirizzo del
         // L1WMI successivo 
    
         if ( strcmp (PointerToL1WMI->StringID, InternalBuffer) == 0 )  // abbiamo trovato L1WMI giusto
         {
           *AddressGet = PointerToL1WMI;
           // Pone nella locazione puntata da AddressGet l'indirizzo dei dati richiesti.
           
           return 0;
         }
         
         if (PointerToAddrL1WMI->NextElementAddr!=0)
           PointerToAddrL1WMI = PointerToAddrL1WMI->NextElementAddr;
         else
           return ERR_L1WMI_NOT_FOUND;
           
       }  // End of while
        
   }
   else
    return ERR_LIST_IS_EMPTY;

}



char ndDB_RemoveLastL1WMIFromList (struct HEADLIST_L1WMI_Type *MyListHandle)
/*
   Questa routine provvede a rimuovere l'ultimo elemento dalla lista di L1WMI 
   identificata da MyListHandle.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è già vuota
        ERR_INTERNAL_ERROR         Errore interno
*/
{
  struct ELEMENT_L1WMI_Type *PointerToL1WMI;
       
  int PointerToRam;
  char ErrRep;

  if ( (MyListHandle->NrElements)!=0)
  {
    ErrRep = ndDB_GetLastElementFromList (MyListHandle, &PointerToRam);
    // Ricava l'indirizzo dell'ultimo elemento memorizzato in memoria.

    if (ErrRep==0)                    // Se non ci sono errori
    {
       PointerToL1WMI = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToL1WMI alla routine precedente, ma si verifica un errore.
       
       if ((PointerToL1WMI->L1SubMenu)!=0) 
          ndDB_DestroyListOfL1WMI (PointerToL1WMI->L1SubMenu);
       // Richiama se stessa per disallocare il sottomenù di livello 1 che è stato
       // agganciato
       
       if ((PointerToL1WMI->L2SubMenu)!=0) 
          ndDB_DestroyListOfL2WMI (PointerToL1WMI->L2SubMenu);
       // Richiama per disallocare il sottomenù di livello 2 che è stato
       // agganciato
       
       free (PointerToRam);
       // Disalloca l'area di memoria che conteneva i dati per l'ultimo L1WMI della lista

       return ndDB_RemoveLastElementFromList (MyListHandle);
       // Ed ora rimuove anche l'ultimo elemento della lista concatenata di indirizzi: il
       // risultato sarà restituito come codice di uscita
    }
    else
        return ERR_INTERNAL_ERROR;
  }
  else
      return ERR_LIST_IS_EMPTY;
}


void ndDB_DestroyListOfL1WMI (struct HEADLIST_L1WMI_Type *MyListHandle)
/*
   Questa routine provvede a cancellare tutte le voci che sono memorizzate in un L1WinMenu.
   La lista da cancellare viene specificata dalla struttura HEADLIST_L1WMI_Type. La procedura
   chiamante deve passare l'indirizzo di tale procedura

   Non sono previsti codici di errore per questa routine.
*/

{
  int Counter;
  int OriginalNrElements;

  OriginalNrElements=MyListHandle->NrElements;

  for (Counter=0; Counter<OriginalNrElements; Counter++)
  {
     ndDB_RemoveLastL1WMIFromList (MyListHandle);
  }
  
  if (MyListHandle->Dynamic)      // Se l'handle della lista è stato allocato dinamicamente, esso viene disallocato
  {
    free (MyListHandle);
  }
  
}


