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
    TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI ICONE 
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void ndDB_InitListOfIcons (struct HEADLIST_Icon_Type *MyListHandle)
/*
   Questa procedura viene usata per inizializzare una struttura che dovrà contenere un elenco
   di Icone.
   La routine chiamante deve allocare una struttura di tipo HEADLIST_Icon_Type. E' compito della
   routine chiamante allocare correttamente tale struttura: di solito essa è statica.
   Alla routine ndDB_InitList viene passato l'indirizzo della struttura.
*/
{
  MyListHandle->FirstElementAddr=0;
  MyListHandle->LastElementAddr=0;
  MyListHandle->NrElements=0;
};

char ndDB_IsPresentIconStringID (struct HEADLIST_Icon_Type *MyListHandle, char *StringID)
/* 
   Questa routine verifica se una determinata StringID è già presente o meno
   nella lista di icone puntata da MyListHandle. Se è così restituisce 1,
   altrimenti restituisce 0.
*/ 

{
   struct ELEMENT_Icon_Type    *PointerToIcon;
   struct ELEMENT_Generic_Type *PointerToAddrIcon;
   int PointerToRam;
   char YouCanExit;
   char IHaveFound;
   
   
   if (MyListHandle->NrElements>0)
   {
       PointerToRam      = MyListHandle->FirstElementAddr;
       PointerToAddrIcon = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToIcon alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToIcon     = PointerToAddrIcon->Addr;
         // In PointerToIcon ci sono i dati relativi all'icona, in PointerToAddrIcon ci
         // sono i dati relativi all'indirizzo dei dati dell'icona ed al prossimo elemento
        
         if ( strcmp (PointerToIcon->StringID, StringID) == 0 )
         {
           IHaveFound=1;
           YouCanExit=1;
         }
         
         if (PointerToAddrIcon->NextElementAddr!=0)
         {
           PointerToAddrIcon = PointerToAddrIcon->NextElementAddr;
         }
         else
         {
           IHaveFound=0;
           YouCanExit=1;
         }
         
       }
       
       return IHaveFound;
   }
   else
    return 0;
    
}                   
   


char INTERNAL_ndDB_AppendIconToList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID, char Format, 
                                struct ndImage_Type *MyImage, char SystemAuth)
/* 
   Questa routine è esclusivamente per scopo interno.

   Questa routine alloca uno spazio in memoria per inserire un nuovo elemento nella lista delle icone, dopodichè
   aggiunge l'indirizzo nella lista di indirizzi definita dalla struttura MyListHandle.

   L'icona che viene aggiunta presenterà un identificatore dato dalla stringa IDIcon. 
   Format definisce il formato dell'icona: è pari a 8 se l'icona è costituita da 8x8 pixel,
   è pari a 16 se l'icona è costituita da 16x16 pixel, e pari a 22 se l'icona è costituita
   da 22x22 pixel. Non sono ammessi altri valori.
   
   La stringa IDIcon deve avere un massimo di 11 caratteri: non sono ammesse StringID più
   lunghe. 
   
   Quando SystemAuth è pari a 0, non è consentito all'utente immettere delle StringID che inizino per
   SYS.  
   
   Inoltre, qualora nella lista fosse già presente un icona con medesimo IDIcon, la routine
   non eseguirà la memorizzazione e restituirà un errore.
   
   La struct MyImage, di cui viene passato l'indirizzo da parte della routine chiamante, 
   deve già essere stata allocata e deve contenere un numero di pixel adeguato al formato
   di icona richiesto. 
   
   Se l'immagine passata ha dimensioni diverse da quelle dell'icona del formato richiesto,
   allora ndDB_AppendIconToList eseguirà uno scaling per adattare le dimensioni dell'immagine
   alle dimensioni dell'icona. 
   
   Sono previsti i codici di errore:
      ERR_MEMORY_OVERFLOW          La memoria non è sufficiente
      ERR_MEMORY_ERROR             Errore nella memoria
      ERR_WRONG_ICON_FORMAT        Formato dell'icona non valido (può essere solo 8,16 o 22);
      ERR_STRINGID_TOO_LONG        La stringa ID è troppo lunga
      ERR_STRINGID_SYS_DENIED      Non è consentito all'utente immettere StringID che inizino per SYS
      ERR_SAME_STRINGID_IS_PRESENT E' già presente il medesimo StringID.
      ERR_IMAGE_IS_NOT_ALLOCATED   L'immagine passata non è stata ancora allocata
*/

{      
   struct ELEMENT_Icon_Type *PointerToNewIcon;
   char MemErrRep;
   char InternalBuffer [12];
   unsigned char LenStringID;
   float ScaleX, ScaleY;
   unsigned short int CounterX, CounterY;
   unsigned short int PosX, PosY;
   TypeColor PixelIcon_MagicNumber;
   
   LenStringID = strlen (StringID);
   
   if (LenStringID<11)                            // Controlla che la StringID sia di meno di 11 caratteri          
   {
      ndStrCpy (InternalBuffer, StringID, 3, 1);  // Copia i primi 3 caratteri di StringID in InternalBuffer
      
      if ( (SystemAuth) || (strcmp (InternalBuffer, "SYS")!=0) )      // Se StringID non inizia per SYS
      {                                                               // oppure SystemAuth è attivata
      
          if (MyImage->Data !=0)                  // Controlla che l'immagine allocata non sia vuota
          {
              if (ndDB_IsPresentIconStringID (MyListHandle, StringID)==0)    // Se lo StringID non è già presente
              {
                    if ( (Format==8) || (Format==16) || (Format==22) )
                    {
                         
                         PointerToNewIcon = malloc  ( sizeof (*PointerToNewIcon) );
                         // Alloca in memoria lo spazio per memorizzare le informazioni relative alla nuova icona
  
                         if (PointerToNewIcon!=0)     // L'allocazione ha avuto successo
                         {
                            
                            ndStrCpy (PointerToNewIcon->StringID, StringID, LenStringID, 1);
                            // Copia la string ID nello spazio allocato
                           
                            PointerToNewIcon->Format=Format;
                            // Copia il formato;
                            
                            ScaleX = ( (float) MyImage->LenX )/( (float) Format);
                            ScaleY = ( (float) MyImage->LenY )/( (float) Format);
                            
                            for (CounterY=0; CounterY<Format; CounterY++)
                            {
                                for (CounterX=0; CounterX<Format; CounterX++)
                                {
                                    PosX = (unsigned short int) ( (float) CounterX * ScaleX);
                                    PosY = (unsigned short int) ( (float) CounterY * ScaleY);
                                    
                                    ndIMG_GetPixelFromImage (MyImage, PosX, PosY, &PixelIcon_MagicNumber); 
                                    PointerToNewIcon->MNPixel [CounterX][CounterY] = PixelIcon_MagicNumber;                                  
                                }
                            }    
                                                  
                            MemErrRep=ndDB_AppendElementToList (MyListHandle, PointerToNewIcon);
                            // Memorizza l'indirizzo del nuovo elemento nella lista di indirizzi.

                            if (MemErrRep!=0)        // Il tentativo di appendere il nuovo elemento può essere fallito.
                            {                        // In tal caso si deve disallocare l'area dove abbiamo copiato le informazioni relative al nuovo Icon
                                free ( PointerToNewIcon );
                                return ERR_MEMORY_ERROR;
                            }
                            else
                                return 0;
                                
                         }
                         else
                            return ERR_MEMORY_OVERFLOW;
                                              
                     }
                     else
                       return  ERR_WRONG_ICON_FORMAT;
              }
              else
                return ERR_SAME_STRINGID_IS_PRESENT;
           }
           else
             return ERR_IMAGE_IS_NOT_ALLOCATED;
       }
       else
          return ERR_STRINGID_SYS_DENIED;
   }
   else
     return ERR_STRINGID_TOO_LONG;     
}



char ndDB_AppendIconToList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID, char Format, struct ndImage_Type *MyImage)
/* 
   Questa routine alloca uno spazio in memoria per inserire un nuovo elemento nella lista delle icone, dopodichè
   aggiunge l'indirizzo nella lista di indirizzi definita dalla struttura MyListHandle.

   L'icona che viene aggiunta presenterà un identificatore dato dalla stringa IDIcon. 
   Format definisce il formato dell'icona: è pari a 8 se l'icona è costituita da 8x8 pixel,
   è pari a 16 se l'icona è costituita da 16x16 pixel, e pari a 22 se l'icona è costituita
   da 22x22 pixel. Non sono ammessi altri valori.
   
   La stringa IDIcon deve avere un massimo di 11 caratteri: non sono ammesse StringID più
   lunghe. 
   
   Non è consentito all'utente immettere delle StringID che inizino per SYS.  
   
   Inoltre, qualora nella lista fosse già presente un icona con medesimo IDIcon, la routine
   non eseguirà la memorizzazione e restituirà un errore.
   
   La struct MyImage, di cui viene passato l'indirizzo da parte della routine chiamante, 
   deve già essere stata allocata e deve contenere un numero di pixel adeguato al formato
   di icona richiesto. 
   
   Se l'immagine passata ha dimensioni diverse da quelle dell'icona del formato richiesto,
   allora ndDB_AppendIconToList eseguirà uno scaling per adattare le dimensioni dell'immagine
   alle dimensioni dell'icona. 
   
   Sono previsti i codici di errore:
      ERR_MEMORY_OVERFLOW          La memoria non è sufficiente
      ERR_MEMORY_ERROR             Errore nella memoria
      ERR_WRONG_ICON_FORMAT        Formato dell'icona non valido (può essere solo 8,16 o 22);
      ERR_STRINGID_TOO_LONG        La stringa ID è troppo lunga
      ERR_STRINGID_SYS_DENIED      Non è consentito all'utente immettere StringID che inizino per SYS
      ERR_SAME_STRINGID_IS_PRESENT E' già presente il medesimo StringID.
      ERR_IMAGE_IS_NOT_ALLOCATED   L'immagine passata non è stata ancora allocata
*/

{
   return INTERNAL_ndDB_AppendIconToList (MyListHandle, StringID, Format, MyImage, 0);
}  


char ndDB_GetIconFromList ( struct HEADLIST_Icon_Type *MyListHandle, char *StringID, struct ELEMENT_Icon_Type *MyIcon)
/*
   La routine ndDB_GetIconFromList copia i dati relativi all'icona caratterizzata da un dato
   StringID nella struttura *MyIcon. La routine chiamante deve avere allocato
   un'opportuna struttura di tipo ELEMENT_Icon_Type atta a contenere le informazioni e deve
   passarne l'indirizzo per mezzo del parametro MyIcon.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_ICON_NOT_FOUND           L'icona con quella StringID non è stata trovata
*/
{
   struct ELEMENT_Icon_Type    *PointerToIcon;
   struct ELEMENT_Generic_Type *PointerToAddrIcon;
   int PointerToRam;
   char YouCanExit, IHaveFound;
   unsigned short int CounterX, CounterY;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, StringID, strlen (StringID), 1);
       // Trasforma la stringID in caratteri maiuscoli                           
                                  
       PointerToRam      = MyListHandle->FirstElementAddr;
       PointerToAddrIcon = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToIcon alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToIcon     = PointerToAddrIcon->Addr;
         // In PointerToIcon ci sono i dati relativi all'icona, in PointerToAddrIcon ci
         // sono i dati relativi all'indirizzo dei dati dell'icona ed al prossimo elemento
    
         if ( strcmp (PointerToIcon->StringID, InternalBuffer) == 0 )  // abbiamo trovato l'icona
         {
           strcpy (MyIcon->StringID, PointerToIcon->StringID);
           MyIcon->Format = PointerToIcon->Format;
           
           for (CounterY=0; CounterY<PointerToIcon->Format; CounterY++)
           {
               for (CounterX=0; CounterX<PointerToIcon->Format; CounterX++)
               {
                   MyIcon->MNPixel [CounterX][CounterY] = PointerToIcon->MNPixel [CounterX][CounterY];
               }
           }
           
           YouCanExit=1;
           IHaveFound=1;
           return 0;
         }
         
         if (PointerToAddrIcon->NextElementAddr!=0)
           PointerToAddrIcon = PointerToAddrIcon->NextElementAddr;
         else
         {
           IHaveFound=0; 
           YouCanExit=1;
         }
         
       }  // End of while
       
       if (IHaveFound) return 0;
         else return ERR_ICON_NOT_FOUND;
   }
   else
    return ERR_LIST_IS_EMPTY;

}

char ndDB_GetIconAddrFromList ( struct HEADLIST_Icon_Type *MyListHandle, char *StringID, int *AddrMyIconPointer)
/*
   La routine ndDB_GetIconAddrFromList determina l'indirizzo dei dati relativi all'icona con un dato
   StringID.
   La routine chiamante deve avere allocato un puntatore di tipo ELEMENT_Icon_Type atto a contenere 
   l'indirizzo.
   
   Il terzo parametro deve essere l'indirizzo del puntatore che conterrà l'indirizzo dei dati icona.
   
   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_ICON_NOT_FOUND           L'icona con quella StringID non è stata trovata
*/
{
   struct ELEMENT_Icon_Type    *PointerToIcon;
   struct ELEMENT_Generic_Type *PointerToAddrIcon;
   
   char YouCanExit;
   int PointerToRam;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, StringID, strlen (StringID), 1);
       // Trasforma la stringID in caratteri maiuscoli                           
                                  
       PointerToRam      = MyListHandle->FirstElementAddr;
       PointerToAddrIcon = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToIcon alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToIcon     = PointerToAddrIcon->Addr;
         // In PointerToIcon ci sono i dati relativi all'icona, in PointerToAddrIcon ci
         // sono i dati relativi all'indirizzo dei dati dell'icona ed al prossimo elemento
    
         if ( strcmp (PointerToIcon->StringID, InternalBuffer) == 0 )  // abbiamo trovato l'icona
         {
           *AddrMyIconPointer = PointerToIcon;     // Deposita il puntatore alla Ram che è stato 
                                                   // trovato, nella locazione di memoria puntata
                                                   // da AddrMyIconPointer. 
           return 0;
         }
         
         if (PointerToAddrIcon->NextElementAddr==0)
           YouCanExit=1;
         else
           PointerToAddrIcon = PointerToAddrIcon->NextElementAddr;
         
       }  // End of while
       
       return ERR_ICON_NOT_FOUND;  // Se si è arrivati a questo punto vuol dire che l'icona non è stata
                                   // trovata
   }
   else
    return ERR_LIST_IS_EMPTY;

}



char ndDB_RemoveIconFromList (struct HEADLIST_Icon_Type *MyListHandle, char *StringID)
/*
   Questa routine provvede a rimuovere dalla lista identificata da MyListHandle
   l'icona caratterizzata da un dato StringID.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è già vuota
        ERR_ICON_NOT_FOUND         L'icona non è stata trovata
        ERR_STRINGID_SYS_DENIED    Non è consentito all'utente immettere StringID che inizino per SYS      
*/
{

  struct ELEMENT_Icon_Type    *PointerToIcon;
  struct ELEMENT_Generic_Type *PointerToAddrIcon, *PointerToAddrNextIcon, *PointerToAddrPrevIcon;
  
  int PointerToRam;
  char YouCanExit;
   
  char InternalBuffer [255];


  if ( (MyListHandle->NrElements)!=0)
  {
       ndStrCpy (InternalBuffer, StringID, 3, 1);
       // Copia i primi 3 caratteri della stringID e li trasforma in caratteri maiuscoli                           
   
       if (strcmp (InternalBuffer, "SYS")!=0)  
       {  
          ndStrCpy (InternalBuffer, StringID, strlen (StringID), 1);
          // Copia tutti i caratteri della stringID e li trasforma in caratteri maiuscoli                           
                                             
          PointerToRam      = MyListHandle->FirstElementAddr;
          PointerToAddrIcon = PointerToRam;
          // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
          // direttamente &PointerToIcon alla routine precedente, ma si verifica un errore.
     
          if ((MyListHandle->NrElements)==1)            // La lista ha un solo elemento
          {
             PointerToIcon     = PointerToAddrIcon->Addr;
             // PointerToIcon punta ai dati dell'icona
          
             if (strcmp (PointerToIcon->StringID, InternalBuffer)==0)
             {
                free (PointerToIcon);    // Disalloca l'area di memoria che contiene le informazioni
                                               // sull'icona
                                               
                free (PointerToAddrIcon);  // Cancella l'area di memoria che contiene l'indirizzo
                                                 // dell'elemento
                                                 
                MyListHandle->NrElements=0;
                MyListHandle->FirstElementAddr=0;
                MyListHandle->LastElementAddr=0;
                
                return 0;
             }
             else      // C'era un solo elemento ma la StringID non corrisponde
             {
                return ERR_ICON_NOT_FOUND;
             }
          }       
          
          if ( (MyListHandle->NrElements)>1 )         // Nella lista ci sono 2 o più elementi
          {
             PointerToAddrNextIcon = PointerToAddrIcon->NextElementAddr;
             // PointerToAddrNextIcon punta alla seconda icona  
               
             PointerToIcon     = PointerToAddrIcon->Addr;     
             // PointerToIcon punta ai dati della prima icona
          
             if (strcmp (PointerToIcon->StringID, InternalBuffer)==0)
             {
                free (PointerToIcon);    // Disalloca l'area di memoria che contiene le informazioni
                                               // sull'icona
                                               
                free (PointerToAddrIcon);  // Cancella l'area di memoria che contiene l'indirizzo
                                                 // dell'elemento
             
                MyListHandle->NrElements       = MyListHandle->NrElements - 1;
                MyListHandle->FirstElementAddr = PointerToAddrNextIcon;
                
                return 0;
             }
             else
             {         // Passa ad operare sulle icone successive
                
                YouCanExit=0;
                
                while (!YouCanExit)
                {
                   PointerToAddrPrevIcon = PointerToAddrIcon;
                   PointerToAddrIcon     = PointerToAddrIcon->NextElementAddr;
                   PointerToAddrNextIcon = PointerToAddrIcon->NextElementAddr;
                   
                   PointerToIcon     = PointerToAddrIcon->Addr;     
                   // PointerToIcon punta ai dati della prima icona
          
                   if (strcmp (PointerToIcon->StringID, InternalBuffer)==0)
                   {
                      free (PointerToIcon);    // Disalloca l'area di memoria che contiene le informazioni
                                                     // sull'icona
                                               
                      free (PointerToAddrIcon);  // Cancella l'area di memoria che contiene l'indirizzo
                                                       // dell'elemento
                    
                    
                      PointerToAddrPrevIcon->NextElementAddr = PointerToAddrNextIcon;
                      
                      MyListHandle->NrElements       = MyListHandle->NrElements - 1;
                
                      if (PointerToAddrNextIcon==0)           // Stiamo lavorando sull'ultimo elemento della lista
                         MyListHandle->LastElementAddr = PointerToAddrPrevIcon;
                      
                      return 0;                                     
                    }
                    
                    if (PointerToAddrNextIcon==0) return ERR_ICON_NOT_FOUND;
                    // Non ci sono altre icone da esaminare, quindi esci.
                                         
                }  // End of while
             }     // Else per operare icone successive
          }    // Else 2 o + elementi
     }
     else return ERR_STRINGID_SYS_DENIED;         
  }
  else
      return ERR_LIST_IS_EMPTY;
  
}


char INTERNAL_RemoveLastIconFromList (struct HEADLIST_Icon_Type *MyListHandle)
/*
   Questa routine è esclusivamente per uso interno.

   Questa routine provvede a rimuovere dalla lista identificata da MyListHandle
   l'ultima icona.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è già vuota
*/
{

  struct ELEMENT_Icon_Type    *PointerToIcon;
  struct ELEMENT_Generic_Type *PointerToAddrIcon, *PointerToAddrPrevIcon;
  
  int PointerToRam;
   
  if ( (MyListHandle->NrElements)!=0)
  {
    PointerToRam      = MyListHandle->FirstElementAddr;
    PointerToAddrIcon = PointerToRam;
    // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
    // direttamente &PointerToIcon alla routine precedente, ma si verifica un errore.
     
    if ((MyListHandle->NrElements)==1)            // La lista ha un solo elemento
    {
      PointerToIcon     = PointerToAddrIcon->Addr;
      // PointerToIcon punta ai dati dell'icona
          
      free (PointerToIcon);    // Disalloca l'area di memoria che contiene le informazioni
                                     // sull'icona
                                               
      free (PointerToAddrIcon);  // Cancella l'area di memoria che contiene l'indirizzo
                                                 // dell'elemento
                                                 
      MyListHandle->NrElements=0;
      MyListHandle->FirstElementAddr=0;
      MyListHandle->LastElementAddr=0;
                
      return 0;
    }
          
    if ( (MyListHandle->NrElements)>1 )         // Nella lista ci sono 2 o più elementi
    {
      while ((PointerToAddrIcon->NextElementAddr)!=0)
      {
        PointerToAddrPrevIcon = PointerToAddrIcon;
        PointerToAddrIcon     = PointerToAddrIcon->NextElementAddr;
      }  
      // Alla fine del processo, in PointerToPrevAddrIcon c'è l'indirizzo
      // del penultimo elemento, e in PointerToAddrIcon c'è l'indirizzo dell'
      // ultimo elemento
      
      PointerToIcon = PointerToAddrIcon->Addr;
      // Punta sui dati dell'ultima icona
      
      free (PointerToIcon);    // Disalloca l'area di memoria che contiene le informazioni
                                     // sull'icona
                                               
      free (PointerToAddrIcon);  // Disalloca l'area di memoria che contiene l'indirizzo
                                       // dell'elemento
      
      PointerToAddrPrevIcon->NextElementAddr=0;
      // Adesso il penultimo elemento non ha nessun successivo.
      
      MyListHandle->NrElements      = MyListHandle->NrElements-1;  
      MyListHandle->LastElementAddr = PointerToAddrPrevIcon;
      
      return 0;
    }
  }  
  else
      return ERR_LIST_IS_EMPTY;
  
}


void ndDB_DestroyListOfIcon (struct HEADLIST_Icon_Type *MyListHandle)
/*
   Questa routine provvede a cancellare tutti le icone che sono memorizzati nella relativa
   lista.
   La lista da cancellare viene specificata dalla struttura HEADLIST_Icon_Type. La procedura
   chiamante deve passare l'indirizzo di tale struttura.

   Non sono previsti codici di errore per questa routine.
*/

{
  while (MyListHandle->NrElements!=0)
  {
     INTERNAL_RemoveLastIconFromList (MyListHandle);
  }   
}




void ndIMG_ShowIcon (struct ELEMENT_Icon_Type *MyIcon, unsigned short int RRPosX, unsigned short int RRPosY, char XWndHandle)
/*
   Questa routine provvede a rappresentare l'icona MyIcon nella finestra XWndHandle alle
   coordinate KPosX, KPosY del Window Space.
   
   Non sono previsti codici di errore per questa routine.
*/
{
   unsigned short int CounterX, CounterY; 
   TypeColor          MagicNumber, MagicNumberBlack;
   
   MagicNumberBlack = ndHAL_RGBToMagicNumber (0, 0, 0);
   
   for (CounterY=0; CounterY<MyIcon->Format; CounterY++)
   {     
      for (CounterX=0; CounterX<MyIcon->Format; CounterX++)
      {
         MagicNumber = MyIcon->MNPixel [CounterX][CounterY];
           
         if (MagicNumber!=MagicNumberBlack)   // Se il colore non è nero
         {
            ndWS_PutPixel (RRPosX+CounterX, RRPosY+CounterY, MagicNumber, XWndHandle, NORENDER);
         }
      }  
   }   
}




void InitializeStandardIconList()
/*
    Nanodesktop usa una struct HEADLIST_Icon_Type che si chiama SystemIcons.
    
    In questa struct vengono allocate da ndInitSystem tutte le icone standard del
    sistema. Le applicazioni possono accedere in ogni momento ai dati di queste
    icone. L'identificativo di queste icone inizia per SYS seguito da un
    ID numerico a 4 cifre.

    Questa routine si occupa dell'allocazione iniziale.
*/
{
  struct ndImage_Type InternalImage;
  
  ndDB_InitListOfIcons (&SystemIcons);
  ndIMG_LoadImageFromNDFArray     (&InternalImage, QuestionPointWidth, QuestionPointHeight, QuestionPoint, NDMGKNB);
  
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDQuestionPoint, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage);
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, FolderIconWidth, FolderIconHeight, FolderIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDFolderIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage);
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, FolderExitIconWidth, FolderExitIconHeight, FolderExitIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDFolderExitIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage);
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, ImageIconWidth, ImageIconHeight, ImageIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDImageIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage); 
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, OfficeIconWidth, OfficeIconHeight, OfficeIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDOfficeIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage); 
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, VideoIconWidth, VideoIconHeight, VideoIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDVideoIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage); 
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, AudioIconWidth, AudioIconHeight, AudioIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDAudioIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage); 
  
  ndIMG_LoadImageFromNDFArray     (&InternalImage, PDFIconWidth, PDFIconHeight, PDFIcon, NDMGKNB);
  INTERNAL_ndDB_AppendIconToList (&SystemIcons, IDPDFIcon, 22, &InternalImage, 1);
  ndIMG_ReleaseImage (&InternalImage); 

}



