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
    GESTIONE DELLE LIST-BOXES
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>


/*
    ***************************************************************************
    GESTIONE DEI LISTBOX ITEM
    ***************************************************************************
*/


void ndDB_InitListOfLBoxItem (struct HEADLIST_LBoxItem_Type *MyListHandle)
/*
   Questa procedura viene usata per inizializzare una struttura che dovrà contenere un elenco
   di LBoxItem.
   
   La routine chiamante deve allocare una struttura di tipo HEADLIST_LBoxItem_Type. E' compito della
   routine chiamante allocare correttamente tale struttura: di solito essa è statica.
   
   Alla routine ndDB_InitList viene passato l'indirizzo della struttura.
*/
{
  MyListHandle->FirstElementAddr=0;
  MyListHandle->LastElementAddr=0;
  MyListHandle->NrElements=0;
};

char ndDB_AppendLBoxItemToList (struct HEADLIST_LBoxItem_Type *MyListHandle, 
                                char *NameLBoxItem, void *LBoxCallback, ndint64 LBoxCode, 
                                char IsInhibited)
/*
   Questa routine alloca uno spazio in memoria per inserire un nuovo elemento nella lista 
   dei LBoxItem, dopodichè aggiunge l'indirizzo nella lista di indirizzi definita 
   dalla struttura MyListHandle.

   L'elemento aggiunto ha le informazioni che vengono passati come parametri.

   Se il nome passato contiene più di 64 caratteri, esso verrà troncato al 64-esimo 
   carattere.

   Le liste di LBoxItem di Nanodesktop non possono superare i 20.000 elementi. Il motivo è molto semplice:
   gli handles di memoria su alcune piattaforme sono limitati, ed una lista di LBoxItem troppo lunga
   consumerebbe tutti gli handles disponibili. Se si tenta di allocare LBoxItem oltre il valore citato,
   si otterrà un errore ERR_LBOXLIST_OVERFLOW. 
   
   Sulle piattaforme in cui la gestione della memoria è effettuata dal sistema operativo (che
   fornisce le funzioni alloc e free senza che queste debbano essere emulate dall'HAL di
   Nanodesktop), è possibile bypassare tale controllo per mezzo della costante manifesta
   NO_FILELIST_OVERFLOW_CHECK in ndHAL_Variables.c (la costante manifesta è lo stessa usata 
   per le liste di file). 

   Sono previsti i codici di errore:
      ERR_MEMORY_OVERFLOW          La memoria non è sufficiente
      ERR_MEMORY_ERROR             Errore nella memoria
      ERR_LBOXLIST_OVERFLOW
*/

{
   struct ELEMENT_LBoxItem_Type *PointerToNewLBoxItem;
   char Counter, YouCanExit;
   char *PointerToChar;
   char MemErrRep;
   
   #ifndef NO_LBoxItemLIST_OVERFLOW_CHECK
   if ((MyListHandle->NrElements)==MAXLISTELEMENTALLOWED)
     return ERR_LBOXLIST_OVERFLOW;
   #endif
   
   PointerToNewLBoxItem = malloc  ( sizeof (*PointerToNewLBoxItem) );
   // Alloca in memoria lo spazio per memorizzare le informazioni relative al nuovo LBoxItem.
   
   if (PointerToNewLBoxItem!=0)     // L'allocazione ha avuto successo
   {
      YouCanExit=0;
      Counter=0;
      PointerToChar=NameLBoxItem;     // Punta al primo carattere del nome LBoxItem

      ndStrCpy (PointerToNewLBoxItem->Name, NameLBoxItem, 63, 0);
      
      PointerToNewLBoxItem->LBoxCallback = LBoxCallback;
      PointerToNewLBoxItem->LBoxCode     = LBoxCode;

      PointerToNewLBoxItem->IsInhibited  = IsInhibited;
      PointerToNewLBoxItem->IsInEvidence = 0;

      MemErrRep=ndDB_AppendElementToList (MyListHandle, PointerToNewLBoxItem);
      // Memorizza l'indirizzo del nuovo elemento nella lista di indirizzi.

      if (MemErrRep!=0)        // Il tentativo di appendere il nuovo elemento può essere fallito.
      {                        // In tal caso si deve disallocare l'area dove abbiamo copiato le informazioni relative al nuovo LBoxItem
          
          free ( PointerToNewLBoxItem );
          return ERR_MEMORY_ERROR;
      }
      else
          return 0;

  }
  else
      return ERR_MEMORY_OVERFLOW;

}

char ndDB_GetLBoxItemFromList ( struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement, struct ELEMENT_LBoxItem_Type *MyLBoxItem)
/*
   Questa routine permette di ottenere informazioni sull'n-esimo elemento della lista
   MyListHandle, dove n è dato da NrElement. 
   
   I dati vengono trasferiti nella struttura puntata da MyLBoxItem: la routine chiamante 
   deve avere allocato un'opportuna struttura di tipo ELEMENT_LBoxItem_Type atta a contenere le 
   informazioni e deve passarne l'indirizzo per mezzo del parametro MyLBoxItem. 
   
   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
   struct ELEMENT_LBoxItem_Type *PointerToLBoxItem;
   char ErrRep;
   int PointerToRam;
   
   ErrRep=ndDB_GetElementFromList (MyListHandle, NrElement, &PointerToRam );
   // Deposita dentro PointerToRam l'indirizzo dell'n-esimo elemento.

   PointerToLBoxItem = PointerToRam;
   // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
   // direttamente &PointerToLBoxItem alla routine precedente, ma si verifica un errore.

   if (!ErrRep)
   {
      // La routine provvede a copiare i dati dalla loro posizione originale in Ram, al buffer
      // puntato da MyLBoxItem. Questo buffer in genere è una struttura che è stata allocata dalla
      // routine chiamante (magari in stack).

      ndStrCpy (MyLBoxItem->Name, PointerToLBoxItem->Name, 63, 0);
      
      MyLBoxItem->LBoxCallback = PointerToLBoxItem->LBoxCallback;
      MyLBoxItem->LBoxCode     = PointerToLBoxItem->LBoxCode;

      MyLBoxItem->IsInhibited  = PointerToLBoxItem->IsInhibited;
      MyLBoxItem->IsInEvidence = PointerToLBoxItem->IsInEvidence;
       
      return 0;
   }
   else
     return ErrRep;

}

char ndDB_GetLBoxItemAddrFromList (  struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement, void *AddressGet)
/*
   Questa routine fornisce l'indirizzo di memoria Ram in cui sono memorizzati i dati relativi
   ad un certo elemento della lista MyListHandle

   Il parametro AddressGet è un puntatore al puntatore. Esso contiene l'indirizzo del puntatore
   alla struttura di tipo struct ELEMENT_LBoxItem_Type richiesta
   
   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_NR_ELEMENT_IS_INVALID    Il nr. di elemento non è valido
*/
{
   
   return ndDB_GetElementFromList ( MyListHandle, NrElement, AddressGet );
   // Poni l'indirizzo ottenuto dalla routine dentro AddressGet

}

char ndDB_SwapLBoxItemInList (struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement)
/*
   Questa routine provvede a swappare tra loro i ListBox Items nr. NrElement e
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




char ndDB_RemoveLastLBoxItemFromList (struct HEADLIST_LBoxItem_Type *MyListHandle)
/*
   Questa routine provvede a rimuovere l'ultimo elemento dalla lista di LBoxItem 
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
       // Disalloca l'area di memoria che conteneva i dati per l'ultimo LBoxItem della lista

       return ndDB_RemoveLastElementFromList (MyListHandle);
       // Ed ora rimuove anche l'ultimo elemento della lista concatenata di indirizzi
    }
    else
      return ErrRep;
  }
  else
      return ERR_LIST_IS_EMPTY;
}

char ndDB_RemoveSpecificLBoxItemFromList (struct HEADLIST_LBoxItem_Type *MyListHandle, int NrElement)
/*
   Questa routine provvede a rimuovere un elemento specifico dalla lista di LBoxItem 
   identificata da MyListHandle.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY             La lista è già vuota
        ERR_NR_ELEMENT_IS_INVALID     Il nr. di elemento non è valido
*/
{
  int ActualPosOfTarget;
  
  if ( (MyListHandle->NrElements)!=0)
  {
     if (NrElement<MyListHandle->NrElements)
     {
            if (NrElement < MyListHandle->NrElements-1)      // L'elemento richiesto non è già l'ultimo della lista
            {
                   // Porta l'elemento che ci serve alla fine....
                   
                   ActualPosOfTarget = NrElement;
                   
                   while (ActualPosOfTarget < MyListHandle->NrElements - 1)
                   {
                       ndDB_SwapLBoxItemInList (MyListHandle, ActualPosOfTarget);
                       ActualPosOfTarget++;  
                   } 
                   
                   // Adesso l'elemento è alla fine
                   
                   return ndDB_RemoveLastLBoxItemFromList (MyListHandle);
            }
            else          // L'elemento richiesto è già l'ultimo
            {
               return ndDB_RemoveLastLBoxItemFromList (MyListHandle); 
            } 
     }
     else return ERR_NR_ELEMENT_IS_INVALID;  
  }
  else
      return ERR_LIST_IS_EMPTY;
}



void ndDB_DestroyListOfLBoxItem (struct HEADLIST_LBoxItem_Type *MyListHandle)
/*
   Questa routine provvede a cancellare tutti i LBoxItem che sono memorizzati in una lista
   di LBoxItem.
   La lista da cancellare viene specificata dalla struttura HEADLIST_LBoxItem_Type. La procedura
   chiamante deve passare l'indirizzo di tale procedura

   Non sono previsti codici di errore per questa routine.
*/

{
  int Counter;
  int OriginalNrElements;

  OriginalNrElements=MyListHandle->NrElements;

  for (Counter=0; Counter<OriginalNrElements; Counter++)
  {
     ndDB_RemoveLastLBoxItemFromList (MyListHandle);
  }
}



/*
    ***************************************************************************
    MANAGERS PER LA GESTIONE DELLE LIST-BOX
    ***************************************************************************
*/


void INTERNAL_ListBoxRenderer (int ListBoxHandle, char WndHandle, char RenderNow);

ndint64 INTERNAL_ListBoxManager (char *StringID, ndint64 InfoField, char WndHandle)
/*
     Questa routine provvede a gestire la pressione di un tasto corrispondente
     ad una voce in una listbox
*/
{
      void (*PntToLBoxCallback)(int NrItem, ndint64 LBoxCode, int ListBoxHandle, char WndHandle);
      
      struct ELEMENT_LBoxItem_Type *MyLBoxItem, *ScanLBoxItem;
      int    ListBoxHandle, NrItem;
      int    Counter;
      
      ListBoxHandle = (InfoField & 0xFFFF0000) >> 16;
      NrItem = WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized + (InfoField & 0xFFFF);
      
      ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), NrItem, &MyLBoxItem);
      
      if (!MyLBoxItem->IsInhibited)
      {
              switch (WindowData [WndHandle].ListBox [ListBoxHandle].EvidStrategy)
              {
                     case ND_LBOX_STABLE_EVIDENCE:
                     {
                          for (Counter=0; Counter<WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements; Counter++)
                          {
                              ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), Counter, &ScanLBoxItem);
                              ScanLBoxItem->IsInEvidence = 0;
                          }
                          
                          MyLBoxItem->IsInEvidence = 1;
                          
                          INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RENDER); 
                          break;
                     }     
              
                     case ND_LBOX_FLASH_EVIDENCE:
                     {
                          for (Counter=0; Counter<WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements; Counter++)
                          {
                              ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), Counter, &ScanLBoxItem);
                              ScanLBoxItem->IsInEvidence = 0;
                          }
                          
                          MyLBoxItem->IsInEvidence = 1;
                          INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RENDER); 
                          
                          MyLBoxItem->IsInEvidence = 0;
                          INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RENDER); 
                          
                          break;
                     }
                     
                     case ND_LBOX_MULTI_EVIDENCE:
                     {
                          MyLBoxItem->IsInEvidence = !(MyLBoxItem->IsInEvidence);
                          INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RENDER); 
                          
                          break; 
                     }
              
                     case ND_LBOX_NULL_EVIDENCE:
                     {
                          break;
                     }
              }
              
              // Ora eseguiamo la callback di lista
              
              PntToLBoxCallback = MyLBoxItem->LBoxCallback; 
              if (PntToLBoxCallback!=0) 
              {
                 (*PntToLBoxCallback)(NrItem, MyLBoxItem->LBoxCode, ListBoxHandle, WndHandle);
              }
      }
      
      return 0;
}

ndint64 INTERNAL_ListBoxManagerForScrollUp (char *StringID, ndint64 InfoField, char WndHandle)
{
     int ThFirstItemVisualized;
     int ListBoxHandle;
      
     ListBoxHandle = InfoField;             // Per prima cosa recuperiamo l'handle della listbox
     ThFirstItemVisualized = WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized - 1;
     
     if (ThFirstItemVisualized>=0)
     {
        WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized = ThFirstItemVisualized;
        INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RENDER); 
     }
     
     return 0;
}

ndint64 INTERNAL_ListBoxManagerForScrollDown (char *StringID, ndint64 InfoField, char WndHandle)
{
     int ThFirstItemVisualized;
     int ListBoxHandle;
      
     ListBoxHandle = InfoField;             // Per prima cosa recuperiamo l'handle della listbox
     ThFirstItemVisualized = WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized + 1;
     
     if (ThFirstItemVisualized + WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable 
             <= WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements)
     {
        WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized = ThFirstItemVisualized;
        INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RENDER); 
     }
     
     return 0;
}







/*
    ***************************************************************************
    RENDERERS PER CREAZIONE/DISTRUZIONE DELLE LISTBOX
    ***************************************************************************
*/

void INTERNAL_ListBox_DrawArrowUp (int PosX, int PosY, TypeColor Color, int ListBoxHandle, char WndHandle)
{
   int CounterX, CounterY;
   
   for (CounterY=0; CounterY<8; CounterY++)
   {
       for (CounterX=0; CounterX<8; CounterX++)
       {
           if (Button_ScrollUp [CounterY][CounterX] == 'x')
              ndWS_PutPixel (PosX+CounterX, PosY+CounterY, Color, WndHandle, NORENDER);
       }
   } 
}


void INTERNAL_ListBox_DrawArrowDown (int PosX, int PosY, TypeColor Color, int ListBoxHandle, char WndHandle)
{
   int CounterX, CounterY;
   
   for (CounterY=0; CounterY<8; CounterY++)
   {
       for (CounterX=0; CounterX<8; CounterX++)
       {
           if (Button_ScrollDown [CounterY][CounterX] == 'x')
              ndWS_PutPixel (PosX+CounterX, PosY+CounterY, Color, WndHandle, NORENDER);
       }
   } 
}


void INTERNAL_CreateKeysOfListBox (int ListBoxHandle, char WndHandle, char CallbacksInNewContext)
{
   ndint64 ListBoxCode;
   
   char StringButton0 [16];
   char StringButton1 [16];
   
   int Counter;
   int AlphaX1, AlphaY1, AlphaX2, AlphaY2;
   int BetaX1,  BetaY1,  BetaX2,  BetaY2;
   
   int BtnX1, BtnY1, BtnX2, BtnY2;
   int BtnForScrollUp, BtnForScrollDown;
   
   ndint64 BtnFeatures;
   
   // Calcoliamo i parametri Alpha e Beta
   
   AlphaX1 = 2;
   AlphaY1 = 2;
   AlphaX2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenX-10;
   AlphaY2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenY-2;
     
   BetaX1 = WindowData [WndHandle].ListBox [ListBoxHandle].LenX-10;
   BetaY1 = 2;
   BetaX2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenX-2;
   BetaY2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenY-2;
   
   for (Counter=0; Counter<WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable; Counter++)
   {
       BtnX1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + AlphaX1 + 2;
       BtnY1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + WindowData [WndHandle].ListBox [ListBoxHandle].MarginY + AlphaY1 + 14*(Counter);
       BtnX2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + AlphaX2 - 2;
       BtnY2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + WindowData [WndHandle].ListBox [ListBoxHandle].MarginY + AlphaY1 + 14*Counter+14;
       
       // Provvedi a registrare il pulsante: genera un identificatore che permetterà di individuare il
       // pulsante
       
       strcpy (StringButton0, "#LB");
       _itoa  (ListBoxHandle, StringButton1, 10);
       strcat (StringButton0, StringButton1);
       strcat (StringButton0, "_");
       _itoa  (Counter, StringButton1, 10);
       strcat (StringButton0, StringButton1);
       
       // Creiamo un campo contenente ListBox e numero di slot. Questo parametro a 64 bit
       // sarà passato al gestore di listbox quando sarà premuto un pulsante slot
       
       ListBoxCode = (ListBoxHandle << 16) + Counter;
       
       // Definiamo i pulsanti della listbox
       
       BtnFeatures =  ND_BUTTON_LOGIC_MODE | ND_TRASPARENT_CALLBACK;
       if (CallbacksInNewContext) BtnFeatures = BtnFeatures | ND_CALLBACK_IN_NEW_CONTEXT; 
       
       ndCTRL_CreateButton    (BtnX1, BtnY1, BtnX2, BtnY2, WndHandle,
                               StringButton0, "", "",
                               0, 0, 0, BtnFeatures, 
                               &(INTERNAL_ListBoxManager), ListBoxCode, 
                               NORENDER);
   }  
   
   // Ok, ora definiamo i pulsanti per scroll-up
   
   BtnX1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1;
   BtnY1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY1; 
   BtnX2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX2; 
   BtnY2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY1 + 10;
   
   strcpy (StringButton0, "#LBXUP");
   _itoa  (ListBoxHandle, StringButton1, 10);
   strcat (StringButton0, StringButton1);
       
   BtnForScrollUp = ndCTRL_CreateButton   (BtnX1, BtnY1, BtnX2, BtnY2, WndHandle,
                                           StringButton0, "", "",
                                           0, 0, 0,
                                           ND_BUTTON_LOGIC_MODE | ND_TRASPARENT_CALLBACK | ND_BUTTON_INHIBITED, 
                                           &(INTERNAL_ListBoxManagerForScrollUp), ListBoxHandle, 
                                           NORENDER); 
           
   // I pulsanti per scroll-down
   
   BtnX1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1;
   BtnY1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY2 - 10; 
   BtnX2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX2; 
   BtnY2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY2;
   
   strcpy (StringButton0, "#LBXDW");
   _itoa  (ListBoxHandle, StringButton1, 10);
   strcat (StringButton0, StringButton1);
       
   BtnForScrollDown = ndCTRL_CreateButton (BtnX1, BtnY1, BtnX2, BtnY2, WndHandle,
                                           StringButton0, "", "",
                                           0, 0, 0,
                                           ND_BUTTON_LOGIC_MODE | ND_TRASPARENT_CALLBACK | ND_BUTTON_INHIBITED, 
                                           &(INTERNAL_ListBoxManagerForScrollDown), ListBoxHandle, 
                                           NORENDER); 
       
   WindowData [WndHandle].ListBox [ListBoxHandle].BtnForScrollUp   = BtnForScrollUp;
   WindowData [WndHandle].ListBox [ListBoxHandle].BtnForScrollDown = BtnForScrollDown;    
}


void INTERNAL_DestroyKeysOfListBox (char OnlyLogical, TypeColor CoverColor, int ListBoxHandle, char WndHandle)
{
   char StringButton0 [16];
   char StringButton1 [16];
   
   int Counter;
   
   // Calcoliamo i parametri Alpha e Beta
   
   for (Counter=0; Counter<WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable; Counter++)
   {
       // Provvedi a ricostruire la label del pulsante: permetterà di identificare il pulsante da
       // cancellare
       
       strcpy (StringButton0, "#LB");
       _itoa  (ListBoxHandle, StringButton1, 10);
       strcat (StringButton0, StringButton1);
       strcat (StringButton0, "_");
       _itoa  (Counter, StringButton1, 10);
       strcat (StringButton0, StringButton1);
       
       // Cancelliamo i pulsanti della listbox
       
       ndCTRL_DestroyButtonUsingStringID (StringButton0, WndHandle, OnlyLogical, CoverColor, NORENDER);
       
   }  
   
   // Ok, ora definiamo i pulsanti per scroll-up
   
   strcpy (StringButton0, "#LBXUP");
   _itoa  (ListBoxHandle, StringButton1, 10);
   strcat (StringButton0, StringButton1);
       
   ndCTRL_DestroyButtonUsingStringID (StringButton0, WndHandle, OnlyLogical, CoverColor, NORENDER);
           
   // I pulsanti per scroll-down
   
   strcpy (StringButton0, "#LBXDW");
   _itoa  (ListBoxHandle, StringButton1, 10);
   strcat (StringButton0, StringButton1);
       
   ndCTRL_DestroyButtonUsingStringID (StringButton0, WndHandle, OnlyLogical, CoverColor, NORENDER);
}


void INTERNAL_ListBoxRenderer (int ListBoxHandle, char WndHandle, char RenderNow)
{ 
   struct ELEMENT_LBoxItem_Type LBoxItem;
   
   char StringBuffer  [64];
   
   int Counter;
   int AlphaX1, AlphaY1, AlphaX2, AlphaY2;
   int BetaX1,  BetaY1,  BetaX2,  BetaY2;
   int CBarX1,  CBarY1,  CBarX2,  CBarY2;
   
   int DBarX1, DBarY1, DBarX2, DBarY2;
   int BarLenY;
   
   float BarRatio;
   
   int ArrowX1, ArrowY1;
   
   int BtnX1, BtnY1, BtnX2, BtnY2;
   
   int VisualizedElement;
   int NrCharsInSlot;
   
   TypeColor _ColorSlot, _ColorTextSlot, _ColorBorderSlot;
   
   // Resetta l'area
   
    ndWS_DrawRectangle (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1,
                        WindowData [WndHandle].ListBox [ListBoxHandle].PosY1,
                        WindowData [WndHandle].ListBox [ListBoxHandle].PosX2,
                        WindowData [WndHandle].ListBox [ListBoxHandle].PosY2,
                        WindowData [WndHandle].ListBox [ListBoxHandle].ColorBackground,
                        WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorder, WndHandle, NORENDER);      
   
   // Calcoliamo i parametri Alpha e Beta
   
   AlphaX1 = 2;
   AlphaY1 = 2;
   AlphaX2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenX-10;
   AlphaY2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenY-2;
     
   BetaX1 = WindowData [WndHandle].ListBox [ListBoxHandle].LenX-10;
   BetaY1 = 2;
   BetaX2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenX-2;
   BetaY2 = WindowData [WndHandle].ListBox [ListBoxHandle].LenY-2;
   
   // Disegna le voci
   
   for (Counter=0; Counter<WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable; Counter++)
   {
       BtnX1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + AlphaX1 + 2;
       BtnY1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + WindowData [WndHandle].ListBox [ListBoxHandle].MarginY + AlphaY1 + 14*(Counter);
       BtnX2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + AlphaX2 - 2;
       BtnY2 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + WindowData [WndHandle].ListBox [ListBoxHandle].MarginY + AlphaY1 + 14*Counter+14;
       
       VisualizedElement = WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized + Counter;
       
       if (VisualizedElement<WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements)
       {
           // Preleva i dati che ci servono sullo slot
           
           ndDB_GetLBoxItemFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), VisualizedElement, &(LBoxItem)  );
          
           // Determina i colori da usare nel sistema
           
           if (!LBoxItem.IsInhibited)
           {
                if (!LBoxItem.IsInEvidence)
                {
                     _ColorSlot        = WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlot;
                     _ColorTextSlot    = WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlot;
                     _ColorBorderSlot  = WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot;                      
                }
                else         // Pulsante in evidenza
                {
                     _ColorSlot        = WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlotActive;
                     _ColorTextSlot    = WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlotActive;
                     _ColorBorderSlot  = WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot;
                }
           }
           else  // Pulsante inibito
           {
                _ColorSlot        = WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlotInh;
                _ColorTextSlot    = WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlotInh;
                _ColorBorderSlot  = WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot;      
           }
                     
           ndWS_DrawRectangle  (BtnX1, BtnY1, BtnX2, BtnY2, _ColorSlot, _ColorBorderSlot, WndHandle, NORENDER); 
       
           NrCharsInSlot = (BtnX2-BtnX1-6)/8;
           ndStrCpy (StringBuffer, LBoxItem.Name, NrCharsInSlot, 0);
           
           ndWS_GrWriteLn (BtnX1+3, BtnY1+3, StringBuffer, _ColorTextSlot, _ColorSlot, WndHandle, 
                             NDKEY_FONT (WindowData [WndHandle].ListBox [ListBoxHandle].FntHandle) | NORENDER);
       }
   }  
   
   if ( WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements > WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable)
   {  
           // Crea set di colori standard
           
           _ColorSlot        = WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlot;
           _ColorTextSlot    = WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlot;
           _ColorBorderSlot  = WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot; 
           
           // Provvedi a creare la scroll-bars
           
           ndWS_DrawRectangle  (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY1, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX2, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY2, 
                                _ColorSlot, _ColorBorderSlot, WndHandle, NORENDER); 
           
           // Disegna la freccia superiore
           
           ndWS_DrawSpRectangle (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1, 
                                 WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY1, 
                                 WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX2, 
                                 WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY1 + 10, 
                                 _ColorSlot, _ColorBorderSlot, ND_VOID, WndHandle, NORENDER); 
           
           ArrowX1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1;
           ArrowY1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY1 + 1;
           
           INTERNAL_ListBox_DrawArrowUp (ArrowX1, ArrowY1, WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot, 
                                         ListBoxHandle, WndHandle);
           
           // Disegna la freccia inferiore
           
           ndWS_DrawSpRectangle (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1, 
                                 WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY2 - 10, 
                                 WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX2, 
                                 WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY2, 
                                 _ColorSlot, _ColorBorderSlot, ND_VOID, WndHandle, NORENDER); 
           
           ArrowX1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + BetaX1;
           ArrowY1 = WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + BetaY2 - 10 + 1;
           
           INTERNAL_ListBox_DrawArrowDown (ArrowX1, ArrowY1, WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot, 
                                           ListBoxHandle, WndHandle);
   
           // Barra centrale
           
           CBarX1 = BetaX1 + 4;
           CBarX2 = BetaX1 + 4;
           CBarY1 = BetaY1 + 10 + 3;
           CBarY2 = BetaY2 - 10 - 3;
           
           ndWS_DrawRectangle  (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + CBarX1, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + CBarY1, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + CBarX2, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + CBarY2, 
                                _ColorBorderSlot, _ColorBorderSlot, WndHandle, NORENDER); 
   
           // SubBarra
           
           BarLenY  = abs (CBarY2 - CBarY1);
           BarRatio = BarLenY/WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements;
           
           DBarX1 = CBarX1 - 2;
           DBarY1 = CBarY1 + BarRatio*WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized;
           DBarX2 = CBarX1 + 2;
           DBarY2 = CBarY1 + BarRatio*(WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized +
                                          WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable);
                                          
           ndWS_DrawRectangle  (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + DBarX1, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + DBarY1, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 + DBarX2, 
                                WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 + DBarY2, 
                                _ColorSlot, _ColorBorderSlot, WndHandle, NORENDER);         
           
           // Provvediamo a deinibire i pulsanti
           
           ndCTRL_DeInhibitButton (WindowData [WndHandle].ListBox [ListBoxHandle].BtnForScrollUp,   WndHandle, NORENDER);
           ndCTRL_DeInhibitButton (WindowData [WndHandle].ListBox [ListBoxHandle].BtnForScrollDown, WndHandle, NORENDER);
   
   }
   else  // Non è necessario disegnare la barra di scrolling, ma dobbiamo inibire i pulsanti
   {
           ndCTRL_InhibitButton (WindowData [WndHandle].ListBox [ListBoxHandle].BtnForScrollUp,   WndHandle, NORENDER);
           ndCTRL_InhibitButton (WindowData [WndHandle].ListBox [ListBoxHandle].BtnForScrollDown, WndHandle, NORENDER);
   }
   
   if (RenderNow) XWindowRender (WndHandle);     
}








/*
    ***************************************************************************
    API PUBBLICA
    ***************************************************************************
*/


int  ndLBOX_CreateListBox (unsigned short int PosX1, unsigned short int PosY1,
                           unsigned short int PosX2, unsigned short int PosY2,
                           ndint64 Options, struct ndListBoxColors_Type *ListBoxColor,
                           char WndHandle, char RenderNow)
                           
/*
    Questa routine si occupa di creare una listbox nella posizione indicata. 
    La listbox che viene creata inizialmente è vuota: per riempirla è
    necessario usare la funzione ndLBOX_AddItemToListBox. 
    
    Il parametro Options è un parametro a 64 bit che consente di impostare
    alcune opzioni per la listbox. I bit sono mappati nel seguente modo:
           
           31-24        Contiene l'handle del font usato dalla TextArea
                        L'utente può settare questo valore utilizzando la key
                        NDKEY_FONT (WndHandle). Se questi bit sono 
                        impostati a zero, il sistema userà il font standard
                        della finestra WndHandle
                        
           0-1          Stabilisce la strategia di evidenziazione automatica
           
                        0    ND_LBOX_STABLE_EVIDENCE
                        
                        Quando viene cliccata una voce della ListBox, questa 
                        viene evidenziata stabilmente. Può essere presente
                        solo una voce evidenziata per volta
                        
                        1    ND_LBOX_FLASH_EVIDENCE
                        
                        Quando viene cliccata una voce della ListBox, questa
                        viene evidenziata per un attimo e poi deevidenziata
                        dopo qualche decimo di secondo. Può essere presente
                        solo una voce evidenziata per volta
                        
                        2    ND_LBOX_MULTI_EVIDENCE
                        
                        Quando viene cliccata una voce della ListBox, questa
                        viene evidenziata. Quando viene cliccata una seconda
                        volta la stessa voce della ListBox, questa viene
                        deevidenziata. In questa modalità possono esserci
                        più voci evidenziate contemporaneamente
                        
                        3   ND_LBOX_NULL_EVIDENCE
                        
                        Non fa nulla, e lascia alla callback della ListBox il
                        compito di gestire la evidenziazione o deevidenziazione
                        dell'oggetto
                        
           3            Specifica se le callbacks che sono associate alla list 
                        box devono essere eseguite in un nuovo context. Usate
                        la costante manifesta
                        
                        ND_CALLBACK_IN_NEW_CONTEXT 
                        
    
    Il parametro ListBoxColor è un puntatore ad una struttura di tipo
    ndListBoxColors_Type che contiene tutte le informazioni sui colori
    che dovranno essere usati nella listbox. 
    
    L'utente può impostare una struttura apposita, passando i colori voluti,
    oppure può passare come parametro &ndLBoxDefaultColor0 o &ndLBoxDefaultColor1
    che contengono dei set di colori già disponibili di default. 
    
    Se l'utente passa un puntatore NULL, Nanodesktop assumerà automaticamente che
    si vogliano usare i colori della struct ndLBoxDefaultColor0.
    
    La routine restituisce l'handle della listbox. 
    
    Se il valore restituito è negativo, siamo in presenza di una listbox.
    Sono previsti i seguenti codici di errore:
         
    ERR_WINDOW_IS_NOT_ALLOCATED     La finestra non è allocata     
    ERR_LISTBOX_IS_TOO_SMALL        La listbox era troppo piccola
    ERR_FONTHDL_IS_NOT_ACTIVE       Il font handle non è attivo
    ERR_NO_LISTBOX_HANDLE_IS_AVAILABLE    Non ci sono listbox handle disponibili
    ERR_RECTANGLE_OUT_OF_WS         Il rettangolo è fuori dal WS      
    ERR_I_NEED_MORE_BUTTONS         Mi servono più tasti
*/

{
   unsigned short int LenX, LenY;
   char YouCanExit, IHaveFound;
   unsigned char FntHandle;
   int  ListBoxHandle;
   int  Counter;
   int  ButtonsNeeded;
   char CallbacksInNewContext;
   
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
       LenX = abs (PosX2 - PosX1);
       LenY = abs (PosY2 - PosY1);
       
       if ((LenX<WindowData [WndHandle].WSWndLength) && (LenY<WindowData [WndHandle].WSWndHeight))
       { 
               if ((LenX>=32) && (LenY>=32))
               {
                    // Cerchiamo un handle libero per la listbox
                    
                    YouCanExit=0;
                    
                    FntHandle = (unsigned char)((Options >> (31-8)) & 0xFF); 
                    if (FntHandle==0) FntHandle = WindowData [WndHandle].ndWS_CurrentFntHandle;
                          
                    if (FontHandlesArray [FntHandle].IsActive)
                    {
                           
                           // Cerchiamo un handle per le listbox che sia libero
                                       
                           IHaveFound = 0;
                           
                           for (Counter=0; Counter<__NRLISTBOXALLOCABLE; Counter++)
                           {
                               if (!WindowData [WndHandle].ListBox [Counter].IsActive)
                               {
                                   ListBoxHandle  = Counter;
                                   IHaveFound = 1;
                                   
                                   break;
                               } 
                           }
                           
                           if (IHaveFound)      // Ho trovato un handle
                           {
                                   WindowData [WndHandle].ListBox [ListBoxHandle].PosX1 = PosX1;
                                   WindowData [WndHandle].ListBox [ListBoxHandle].PosX2 = PosX2;
                                   WindowData [WndHandle].ListBox [ListBoxHandle].PosY1 = PosY1;
                                   WindowData [WndHandle].ListBox [ListBoxHandle].PosY2 = PosY2;
                                                        
                                   WindowData [WndHandle].ListBox [ListBoxHandle].LenX  = LenX; 
                                   WindowData [WndHandle].ListBox [ListBoxHandle].LenY  = LenY; 
                                                
                                   WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable =  (LenY-4) / 14;
                                   WindowData [WndHandle].ListBox [ListBoxHandle].MarginY             = ((LenY-4) % 14)/2;
                                   
                                   ButtonsNeeded = WindowData [WndHandle].ListBox [ListBoxHandle].NrSlotsVisualizable + 2;
                                   
                                   if ( (WindowData [WndHandle].NrButtonsAllocated + ButtonsNeeded) < __NRBUTTONSALLOCABLE )
                                   {
                                           // Se l'utente non ha specificato una struct standard per le listbox, si
                                           // prende di default ndLBoxDefaultColor1
                                           
                                           if (ListBoxColor==0) ListBoxColor=&ndLBoxDefaultColor0;
                                           
                                           // Copia i colori visualizzabili impostati dall'utente
                                           
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorder           = ListBoxColor->ColorBorder;
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorBackground       = ListBoxColor->ColorBackground; 
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorBorderSlot       = ListBoxColor->ColorBorderSlot; 
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlot             = ListBoxColor->ColorSlot;
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlotActive       = ListBoxColor->ColorSlotActive; 
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorSlotInh          = ListBoxColor->ColorSlotInh;
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlot         = ListBoxColor->ColorTextSlot;
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlotActive   = ListBoxColor->ColorTextSlotActive; 
                                           WindowData [WndHandle].ListBox [ListBoxHandle].ColorTextSlotInh      = ListBoxColor->ColorTextSlotInh;               
                                           
                                           // Aggiorna le altre informazioni
                                           
                                           WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized=0;
                                           WindowData [WndHandle].ListBox [ListBoxHandle].FntHandle = FntHandle;
                                           WindowData [WndHandle].ListBox [ListBoxHandle].EvidStrategy = Options & 3;
                                   
                                           // Verifica se è necessario creare callbacks in new context
                                           
                                           if ( (Options & ND_CALLBACK_IN_NEW_CONTEXT) == ND_CALLBACK_IN_NEW_CONTEXT )
                                              CallbacksInNewContext=1;
                                           else
                                              CallbacksInNewContext=0;                                               
                                           
                                           // Provvedi ad azzeramento core
                                           
                                           INTERNAL_CreateKeysOfListBox (ListBoxHandle, WndHandle, CallbacksInNewContext); 
                                           ndDB_InitListOfLBoxItem ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core) );
                                                        
                                           INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
                                           
                                           WindowData [WndHandle].ListBox [ListBoxHandle].IsActive = 1;
                                           return ListBoxHandle;
                                   }
                                   else return ERR_I_NEED_MORE_BUTTONS;
                           }
                           else return ERR_NO_LISTBOX_HANDLE_IS_AVAILABLE;              
                    }
                    else return ERR_FONTHDL_IS_NOT_ACTIVE;         
               }
               else return ERR_LISTBOX_IS_TOO_SMALL;    
       }
       else return ERR_RECTANGLE_OUT_OF_WS;         
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;     
}


char ndLBOX_AddItemToListBox (char *NameLBoxItem, void *LBoxCallback, ndint64 LBoxCode, char IsInhibited, int ListBoxHandle, char WndHandle)
/*
    Questa routine provvede a registrare un nuovo elemento all'interno di una listbox.
    La listbox viene individuata per mezzo della coppia LBoxHandle/WndHandle

    Si ricordi che questa funzione non crea il nuovo elemento sullo schermo: questo
    viene creato solo quando, dopo avere registrato tutti gli elementi, viene
    richiamata la funzione ndLBOX_UpdateListBox
    
    LBoxCallback è il puntatore ad una funzione con prototipo (callback di lista):
    void (*PntToLBoxCallback)(int NrItem, ndint64 LBoxCode, int ListBoxHandle, char WndHandle);             
    
    Sono previsti i seguenti codici di errore:
      ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
      ERR_LISTBOX_IS_NOT_ACTIVE     ListBox non è attiva
          
*/
{
    struct HEADLIST_LBoxItem_Type *CoreListBox;
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
             CoreListBox = &(WindowData [WndHandle].ListBox [ListBoxHandle].Core);
             return ndDB_AppendLBoxItemToList (CoreListBox, NameLBoxItem, LBoxCallback, LBoxCode, IsInhibited);    
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;  
}

char ndLBOX_UpdateListBox (int ListBoxHandle, char WndHandle, char RenderNow)
/*
     Da richiamarsi dopo avere inserito tutti gli elementi nella listbox
*/
{
     INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
     return 0;
}


int ndLBOX_FindListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle)
/*
     Questa routine provvede a cercare un item all'interno di una lista.
     Restituisce in uscita il numero di ordine dell'item. 
     
     Se il risultato restituito è negativo, siamo in presenza di un
     errore. Sono previsti i seguenti codici di errore
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/
{
    struct ELEMENT_LBoxItem_Type *PntToMyListBoxItem;
    int  Counter;
    
    char StringBuffer [64];
    char IHaveFound, ErrRep;
    
    ndStrCpy (StringBuffer, Name, 63, 0);
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
            IHaveFound = 0;
            
            for (Counter=0; Counter<WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements; Counter++)
            {
                ErrRep=ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), Counter, &PntToMyListBoxItem);
              
                if (!ErrRep)
                {
                   if (strcmp (PntToMyListBoxItem->Name, StringBuffer)==0)
                   {
                       IHaveFound=1;
                       break;       
                   } 
                }
            }
            
            if (IHaveFound) 
               return Counter;
            else 
               return ERR_STRINGID_NOT_FOUND;           
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   

}



char ndLBOX_InhibitListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine provvede ad inibire un item della list-box. La listbox viene
    individuata dal parametro ListBoxHandle/WndHandle.
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_NR_ELEMENT_IS_INVALID     Il numero di elemento non è valido
*/       
{
    struct ELEMENT_LBoxItem_Type *PntToMyListBoxItem;
    char ErrRep;
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
            ErrRep=ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), NrElement, &PntToMyListBoxItem);
              
            if (!ErrRep)
            {
               PntToMyListBoxItem->IsInhibited=1;
               
               INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
               return 0;  
            }
            else return ErrRep;  
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}

char ndLBOX_InhibitListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Analoga alla precedente, però l'item viene identificato per mezzo del nome
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/       
{
    int NrElementOrError;
    
    NrElementOrError =  ndLBOX_FindListBoxItemByName (Name, ListBoxHandle, WndHandle);
    
    if (NrElementOrError>=0)
    {
        return  ndLBOX_InhibitListBoxItem (NrElementOrError, ListBoxHandle, WndHandle, RenderNow); 
    }
    else
      return NrElementOrError;
}




char ndLBOX_DeInhibitListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine provvede ad deinibire un item della list-box. La listbox viene
    individuata dal parametro ListBoxHandle/WndHandle.
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_NR_ELEMENT_IS_INVALID     Il numero di elemento non è valido
*/       
{
    struct ELEMENT_LBoxItem_Type *PntToMyListBoxItem;
    char ErrRep;
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
            ErrRep=ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), NrElement, &PntToMyListBoxItem);
              
            if (!ErrRep)
            {
               PntToMyListBoxItem->IsInhibited=0;
               
               INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
               return 0;  
            }
            else return ErrRep;  
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}


char ndLBOX_DeInhibitListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Analoga alla precedente, però l'item viene identificato per mezzo del nome
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/       
{
    int NrElementOrError;
    
    NrElementOrError =  ndLBOX_FindListBoxItemByName (Name, ListBoxHandle, WndHandle);
    
    if (NrElementOrError>=0)
    {
        return  ndLBOX_DeInhibitListBoxItem (NrElementOrError, ListBoxHandle, WndHandle, RenderNow); 
    }
    else
      return NrElementOrError;
}



char ndLBOX_EvidenceListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine provvede ad evidenziare un item della list-box. La listbox viene
    individuata dal parametro ListBoxHandle/WndHandle.
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_NR_ELEMENT_IS_INVALID     Il numero di elemento non è valido
*/       
{
    struct ELEMENT_LBoxItem_Type *PntToMyListBoxItem;
    char ErrRep;
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
            ErrRep=ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), NrElement, &PntToMyListBoxItem);
              
            if (!ErrRep)
            {
               PntToMyListBoxItem->IsInEvidence=1;
               
               INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
               return 0;  
            }
            else return ErrRep;  
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}


char ndLBOX_EvidenceListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Analoga alla precedente, però l'item viene identificato per mezzo del nome
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/       
{
    int NrElementOrError;
    
    NrElementOrError =  ndLBOX_FindListBoxItemByName (Name, ListBoxHandle, WndHandle);
    
    if (NrElementOrError>=0)
    {
        return  ndLBOX_EvidenceListBoxItem (NrElementOrError, ListBoxHandle, WndHandle, RenderNow); 
    }
    else
      return NrElementOrError;
}


char ndLBOX_DeEvidenceListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine provvede ad deevidenziare un item della list-box. La listbox viene
    individuata dal parametro ListBoxHandle/WndHandle.
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_NR_ELEMENT_IS_INVALID     Il numero di elemento non è valido
*/       
{
    struct ELEMENT_LBoxItem_Type *PntToMyListBoxItem;
    char ErrRep;
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
            ErrRep=ndDB_GetLBoxItemAddrFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), NrElement, &PntToMyListBoxItem);
              
            if (!ErrRep)
            {
               PntToMyListBoxItem->IsInEvidence=0;
               
               INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
               return 0;  
            }
            else return ErrRep;  
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}


char ndLBOX_DeEvidenceListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Analoga alla precedente, però l'item viene identificato per mezzo del nome
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/       
{
    int NrElementOrError;
    
    NrElementOrError =  ndLBOX_FindListBoxItemByName (Name, ListBoxHandle, WndHandle);
    
    if (NrElementOrError>=0)
    {
        return  ndLBOX_DeEvidenceListBoxItem (NrElementOrError, ListBoxHandle, WndHandle, RenderNow); 
    }
    else
      return NrElementOrError;
}



char ndLBOX_RemoveListBoxItem (int NrElement, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Questa routine provvede a rimuovere un certo elemento da una list-box.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/
{
    char ErrRep;
    
    if (WindowData [WndHandle].WindowIsActive)
    {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
            ErrRep = ndDB_RemoveSpecificLBoxItemFromList ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core), NrElement );
              
            if (!ErrRep)
            {
               // E' necessario correggere alcuni parametri
               
               if (WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements>0)
               {
                       if (WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized >
                              WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements - 1)
                       {
                           WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized =
                              WindowData [WndHandle].ListBox [ListBoxHandle].Core.NrElements - 1;    
                       }
               }
               else
               {
                       WindowData [WndHandle].ListBox [ListBoxHandle].FirstItemVisualized = 0;
               } 
               
               INTERNAL_ListBoxRenderer (ListBoxHandle, WndHandle, RenderNow);
               return 0;  
            }
            else return ErrRep;  
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;  
    }
    else return ERR_WINDOW_IS_NOT_ALLOCATED;   
}


char ndLBOX_RemoveListBoxItemByName (char *Name, int ListBoxHandle, char WndHandle, char RenderNow)
/*
    Analoga alla precedente, però l'item viene identificato per mezzo del nome
    
    L'item viene individuato in base al numero NrElement.
    
    Sono previsti i seguenti codici di errore:
         ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non è allocata
         ERR_LISTBOX_IS_NOT_ACTIVE     La listbox non è attiva
         ERR_STRINGID_NOT_FOUND        Il nome richiesto non è stato trovato
*/       
{
    int NrElementOrError;
    
    NrElementOrError =  ndLBOX_FindListBoxItemByName (Name, ListBoxHandle, WndHandle);
    
    if (NrElementOrError>=0)
    {
        return  ndLBOX_RemoveListBoxItem (NrElementOrError, ListBoxHandle, WndHandle, RenderNow); 
    }
    else
      return NrElementOrError;
}


char ndLBOX_DestroyListBox (char ListBoxHandle, char WndHandle, char OnlyLogical, TypeColor CoverColor, char RenderNow)
/*
    Questa routine ha lo scopo di distruggere una list box. L'elemento viene identificato 
    dalla coppia ListBoxHandle/WndHandle. 
    
    Se si pone ad 1 il parametro OnlyLogical, la list box verrà cancellata solo dal
    punto di vista logico: il disegno dell'area rimarrà sullo schermo, anche se
    cliccandoci sopra MouseControl non farà nulla. 
   
    Se invece si pone a 0 il parametro OnlyLogical, l'area verrà cancellata
    anche graficamente: questo viene fatto riempiendo lo spazio dove si trovava il
    pulsante con il colore CoverColor (che è in genere il colore di sfondo della finestra.
   
    Sono previsti i seguenti codici di errore
          ERR_WINDOW_IS_NOT_ALLOCATED        La finestra non è allocata
          ERR_LISTBOX_IS_NOT_ACTIVE          L'handle della TextBox non è attivo   
*/
{
   if (!ndInitSystemExecuted) ndInitSystem();
   
   if (WindowData [WndHandle].WindowIsActive)
   {
        if (WindowData [WndHandle].ListBox [ListBoxHandle].IsActive)
        {
              ndDB_DestroyListOfLBoxItem ( &(WindowData [WndHandle].ListBox [ListBoxHandle].Core) );
              // Distrugge la lista degli items
              
              INTERNAL_DestroyKeysOfListBox (OnlyLogical, CoverColor, ListBoxHandle, WndHandle);
              // Distrugge i tasti che sono legati alla list box
              
              if (!OnlyLogical)
              {
                   // Cancella tutta la vecchia area
   
                   ndWS_DrawRectangle (WindowData [WndHandle].ListBox [ListBoxHandle].PosX1, WindowData [WndHandle].ListBox [ListBoxHandle].PosY1,
                                       WindowData [WndHandle].ListBox [ListBoxHandle].PosX2, WindowData [WndHandle].ListBox [ListBoxHandle].PosY2,
                                       CoverColor, CoverColor, WndHandle, NORENDER);
              }
              
              if (RenderNow) XWindowRender (WndHandle);
              
              WindowData [WndHandle].ListBox [ListBoxHandle].IsActive=0;
              WindowData [WndHandle].NrListBoxAllocated--;
              
              return 0;
        }
        else return ERR_LISTBOX_IS_NOT_ACTIVE;
   }
   else return ERR_WINDOW_IS_NOT_ALLOCATED;        
}























