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
    TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI CLASSI 
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void ndDB_InitListOfClasses (struct HEADLIST_Class_Type *MyListHandle)
/*
   Questa procedura viene usata per inizializzare una struttura che dovrà contenere un elenco
   di CLASSI.
   La routine chiamante deve allocare una struttura di tipo HEADLIST_Class_Type. E' compito della
   routine chiamante allocare correttamente tale struttura: di solito essa è statica.
   Alla routine ndDB_InitList viene passato l'indirizzo della struttura.
*/
{
  MyListHandle->FirstElementAddr=0;
  MyListHandle->LastElementAddr=0;
  MyListHandle->NrElements=0;
};

char ndDB_IsPresentSameNameClass (struct HEADLIST_Class_Type *MyListHandle, char *NameClass)
/* 
   Questa routine verifica se una determinata Nameclass è già presente o meno
   nella lista di classi puntata da MyListHandle. Se è così restituisce 1,
   altrimenti restituisce 0.
*/ 

{

   struct ELEMENT_Class_Type    *PointerToClass;
   struct ELEMENT_Generic_Type  *PointerToAddrClass;
   
   int PointerToRam;
   char YouCanExit;
   
   
   if (MyListHandle->NrElements>0)
   {
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrClass = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToClass     = PointerToAddrClass->Addr;
         // In PointerToClass ci sono i dati relativi alla classe, in PointerToAddrClass ci
         // sono i dati relativi all'indirizzo dei dati della classe ed all'indirizzo del
         // prossimo elemento
    
         if ( strcmp (PointerToClass->NameClass, NameClass) == 0 )
           return 1;
         
         if (PointerToAddrClass->NextElementAddr!=0)
           PointerToAddrClass = PointerToAddrClass->NextElementAddr;
         else
           return 0;
         
       }
       
   }
   else
    return 0;
    
}                   
   


char ndDB_AppendClassToList (struct HEADLIST_Class_Type *MyListHandle, char *NameClass, 
                        char *Extensions, char *StringIDIcon, int PointerToThumbFunc) 
/* 
   Questa routine alloca uno spazio in memoria per definire una nuova classe, dopodichè
   aggiunge l'indirizzo nella lista di indirizzi definita dalla struttura MyListHandle.

   La classe che viene aggiunta presenterà un identificatore dato dalla stringa NameClass. 
   
   Qualora nella lista fosse già presente un Classe con medesimo NameClass, la routine
   non eseguirà la memorizzazione e restituirà un errore.
   
   Sono ammesse solo NameClass di dimensione inferiore a 63 caratteri.
   
   Sono previsti i codici di errore:
      ERR_MEMORY_OVERFLOW              La memoria non è sufficiente
      ERR_MEMORY_ERROR                 Errore nella memoria
      ERR_NAMECLASS_TOO_LONG           La NameClass è troppo lunga
      ERR_SAME_NAMECLASS_IS_PRESENT    E' già presente la medesima NameClass.
      
*/

{      
   struct ELEMENT_Class_Type *PointerToNewClass;
   char MemErrRep;
   unsigned char LenNameClass;
   
   LenNameClass = strlen (NameClass);
   
   if (LenNameClass<62)                            // Controlla che NameClass sia di meno di 11 caratteri          
   {
      if (ndDB_IsPresentSameNameClass (MyListHandle, NameClass)==0)    // Se la NameClass non è già presente
      {
         PointerToNewClass = malloc  ( sizeof (*PointerToNewClass) );
         // Alloca in memoria lo spazio per memorizzare le informazioni relative alla nuova classe
  
         if (PointerToNewClass!=0)     // L'allocazione ha avuto successo
         {                 
            ndStrCpy (PointerToNewClass->NameClass, NameClass, LenNameClass, 1);
            // Copia la string ID nello spazio allocato
                           
            ndStrCpy (PointerToNewClass->Extensions, Extensions, strlen (Extensions), 1);
            // Copia la stringa estensioni
            
            ndStrCpy (PointerToNewClass->StringIDIcon, StringIDIcon, 11, 1);
            // Copia StringID della relativa icona
            
            PointerToNewClass->ThumbFunction = PointerToThumbFunc; 
            // Copia il puntatore della funzione thumbnail               
                                                                              
            MemErrRep=ndDB_AppendElementToList (MyListHandle, PointerToNewClass);
            // Memorizza l'indirizzo del nuovo elemento nella lista di indirizzi.

            if (MemErrRep!=0)        // Il tentativo di appendere il nuovo elemento può essere fallito.
            {                        // In tal caso si deve disallocare l'area dove abbiamo copiato le informazioni relative al nuovo Class
               free ( PointerToNewClass );
               return ERR_MEMORY_ERROR;
            }
            else
              return 0;
                                
         }
         else
           return ERR_MEMORY_OVERFLOW;
                                              
      }
      else
        return ERR_SAME_NAMECLASS_IS_PRESENT;
   }       
   else
     return ERR_NAMECLASS_TOO_LONG;     
}



char ndDB_GetClassFromList ( struct HEADLIST_Class_Type *MyListHandle, char *NameClass, struct ELEMENT_Class_Type *MyClass)
/*
   La routine ndDB_GetClassFromList copia i dati relativi alla classe caratterizzata da un dato
   NameClass nella struttura *MyClass. La routine chiamante deve avere allocato
   un'opportuna struttura di tipo ELEMENT_Class_Type atta a contenere le informazioni e deve
   passarne l'indirizzo per mezzo del parametro MyClass.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_CLASS_NOT_FOUND          La classe con quel NameClass non è stata trovata
*/
{        
   struct ELEMENT_Class_Type   *PointerToClass;
   struct ELEMENT_Generic_Type *PointerToAddrClass;
   
   int PointerToRam;
   char YouCanExit;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, NameClass, strlen (NameClass), 1);
       // Trasforma il NameClass in caratteri maiuscoli                           
                                  
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrClass = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToClass     = PointerToAddrClass->Addr;
         // In PointerToClass ci sono i dati relativi alla classe, in PointerToAddrClass ci
         // sono i dati relativi all'indirizzo dei dati della classe ed al prossimo elemento
    
         if ( strcmp (PointerToClass->NameClass, InternalBuffer) == 0 )  // abbiamo trovato la classe
         {
           strcpy (MyClass->NameClass,    PointerToClass->NameClass);
           strcpy (MyClass->Extensions,   PointerToClass->Extensions);
           strcpy (MyClass->StringIDIcon, PointerToClass->StringIDIcon);
           MyClass->ThumbFunction = PointerToClass->ThumbFunction;
           
           return 0;
         }
         
         if (PointerToAddrClass->NextElementAddr!=0)
           PointerToAddrClass = PointerToAddrClass->NextElementAddr;
         else
           return ERR_CLASS_NOT_FOUND;
           
       }  // End of while
        
   }
   else
    return ERR_LIST_IS_EMPTY;

}


char ndDB_GetClassAddrFromList ( struct HEADLIST_Class_Type *MyListHandle, char *NameClass, int *PointerToClassAddr)
/*
   La routine ndDB_GetClassAddrFromList determina l'indirizzo dei dati relativi alla classe con un dato
   NameClass.
   
   Il terzo parametro deve essere l'indirizzo del puntatore che conterrà l'indirizzo dei dati della
   classe.
   
   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY            La lista è vuota
        ERR_CLASS_NOT_FOUND          La classe con quel NameClass non è stata trovata
*/
{        
   struct ELEMENT_Class_Type    *PointerToClass;
   struct ELEMENT_Generic_Type *PointerToAddrClass;
   
   int PointerToRam;
   char YouCanExit;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, NameClass, strlen (NameClass), 1);
       // Trasforma la NameClass in caratteri maiuscoli                           
                                  
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrClass = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToClass     = PointerToAddrClass->Addr;
         // In PointerToClass ci sono i dati relativi alla classe, in PointerToAddrClass ci
         // sono i dati relativi all'indirizzo dei dati della classe ed al prossimo elemento
    
         if ( strcmp (PointerToClass->NameClass, InternalBuffer) == 0 )  // abbiamo trovato la classe
         {
           *PointerToClassAddr = PointerToClass;
           // Pone nella locazione puntata da PointerToClassAddr l'indirizzo dei dati della classe
           
           return 0;
         }
         
         if (PointerToAddrClass->NextElementAddr!=0)
            PointerToAddrClass = PointerToAddrClass->NextElementAddr;
         else
            return ERR_CLASS_NOT_FOUND;           // Se si è arrivati a questo punto, la classe non è stata trovata    
           
       }  // End of while
       
       
   }
   else
    return ERR_LIST_IS_EMPTY;

}



char ndDB_RemoveClassFromList (struct HEADLIST_Class_Type *MyListHandle, char *NameClass)
/*
   Questa routine provvede a rimuovere dalla lista identificata da MyListHandle
   la classe caratterizzata da un dato NameClass.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è già vuota
        ERR_CLASS_NOT_FOUND        La classe non è stata trovata  
*/
{
  struct ELEMENT_Class_Type   *PointerToClass;
  struct ELEMENT_Generic_Type *PointerToAddrClass, *PointerToAddrNextClass, *PointerToAddrPrevClass;
  
  int PointerToRam;
  char YouCanExit;
   
  char InternalBuffer [255];

  if ( (MyListHandle->NrElements)!=0)
  {
          ndStrCpy (InternalBuffer, NameClass, strlen (NameClass), 1);
          // Copia tutti i caratteri della NameClass e li trasforma in caratteri maiuscoli                           
                                             
          PointerToRam       = MyListHandle->FirstElementAddr;
          PointerToAddrClass = PointerToRam;
          // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
          // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
          if ((MyListHandle->NrElements)==1)            // La lista ha un solo elemento
          {
             PointerToClass     = PointerToAddrClass->Addr;
             // PointerToClass punta ai dati della classe
          
             if (strcmp (PointerToClass->NameClass, InternalBuffer)==0)
             {
                free (PointerToClass);    // Disalloca l'area di memoria che contiene le informazioni
                                               // sulla classe
                                               
                free (PointerToAddrClass);  // Cancella l'area di memoria che contiene l'indirizzo
                                                 // dell'elemento
                                                 
                MyListHandle->NrElements=0;
                MyListHandle->FirstElementAddr=0;
                MyListHandle->LastElementAddr=0;
                
                return 0;
             }
             else      // C'era un solo elemento ma la NameClass non corrisponde
             {
                return ERR_CLASS_NOT_FOUND;
             }
          }       
          
          if ( (MyListHandle->NrElements)>1 )         // Nella lista ci sono 2 o più elementi
          {
             PointerToAddrNextClass = PointerToAddrClass->NextElementAddr;
             // PointerToAddrNextClass punta alla seconda Classa  
               
             PointerToClass     = PointerToAddrClass->Addr;     
             // PointerToClass punta ai dati della prima Classa
          
             if (strcmp (PointerToClass->NameClass, InternalBuffer)==0)
             {
                free (PointerToClass);    // Disalloca l'area di memoria che contiene le informazioni
                                               // sulla classe
                                               
                free (PointerToAddrClass);  // Cancella l'area di memoria che contiene l'indirizzo
                                                 // dell'elemento
             
                MyListHandle->NrElements       = MyListHandle->NrElements - 1;
                MyListHandle->FirstElementAddr = PointerToAddrNextClass;
                
                return 0;
             }
             else
             {         // Passa ad operare sulle CLASSI successive
                
                YouCanExit=0;
                
                while (!YouCanExit)
                {
                   PointerToAddrPrevClass = PointerToAddrClass;
                   PointerToAddrClass     = PointerToAddrClass->NextElementAddr;
                   PointerToAddrNextClass = PointerToAddrClass->NextElementAddr;
                   
                   PointerToClass     = PointerToAddrClass->Addr;     
                   // PointerToClass punta ai dati della prima classe
          
                   if (strcmp (PointerToClass->NameClass, InternalBuffer)==0)
                   {
                      free (PointerToClass);    // Disalloca l'area di memoria che contiene le informazioni
                                                     // sulla classe
                                               
                      free (PointerToAddrClass);  // Cancella l'area di memoria che contiene l'indirizzo
                                                       // dell'elemento
                    
                    
                      PointerToAddrPrevClass->NextElementAddr = PointerToAddrNextClass;
                      
                      MyListHandle->NrElements       = MyListHandle->NrElements - 1;
                
                      if (PointerToAddrNextClass==0)           // Stiamo lavorando sull'ultimo elemento della lista
                         MyListHandle->LastElementAddr = PointerToAddrPrevClass;
                      
                      return 0;                                     
                    }
                    
                    if (PointerToAddrNextClass==0) return ERR_CLASS_NOT_FOUND;
                    // Non ci sono altre CLASSI da esaminare, quindi esci.
                                         
                }  // End of while
             }     // Else per operare CLASSI successive
          }    // Else 2 o + elementi
     
  }
  else
      return ERR_LIST_IS_EMPTY;
  
}


char INTERNAL_RemoveLastClassFromList (struct HEADLIST_Class_Type *MyListHandle)
/*
   Questa routine è esclusivamente per uso interno.

   Questa routine provvede a rimuovere dalla lista identificata da MyListHandle
   l'ultima Classe.

   Sono previsti i seguenti codici di errore:
        ERR_LIST_IS_EMPTY          La lista è già vuota
*/
{

  struct ELEMENT_Class_Type   *PointerToClass;
  struct ELEMENT_Generic_Type *PointerToAddrClass, *PointerToAddrPrevClass;
  
  int PointerToRam;
   
  if ( (MyListHandle->NrElements)!=0)
  {
    PointerToRam       = MyListHandle->FirstElementAddr;
    PointerToAddrClass = PointerToRam;
    // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
    // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
    if ((MyListHandle->NrElements)==1)            // La lista ha un solo elemento
    {
      PointerToClass     = PointerToAddrClass->Addr;
      // PointerToClass punta ai dati della classe
          
      free (PointerToClass);    // Disalloca l'area di memoria che contiene le informazioni
                                     // sulla classe
                                               
      free (PointerToAddrClass);  // Cancella l'area di memoria che contiene l'indirizzo
                                                 // dell'elemento
                                                 
      MyListHandle->NrElements=0;
      MyListHandle->FirstElementAddr=0;
      MyListHandle->LastElementAddr=0;
                
      return 0;
    }
          
    if ( (MyListHandle->NrElements)>1 )         // Nella lista ci sono 2 o più elementi
    {
          while ((PointerToAddrClass->NextElementAddr)!=0)
          {
            PointerToAddrPrevClass = PointerToAddrClass;
            PointerToAddrClass     = PointerToAddrClass->NextElementAddr;
          }  
          // Alla fine del processo, in PointerToPrevAddrClass c'è l'indirizzo
          // del penultimo elemento, e in PointerToAddrClass c'è l'indirizzo dell'
          // ultimo elemento
          
          PointerToClass = PointerToAddrClass->Addr;
          // Punta sui dati dell'ultima Classa
          
          free (PointerToClass);    // Disalloca l'area di memoria che contiene le informazioni
                                          // sulla classe
                                                   
          free (PointerToAddrClass);  // Disalloca l'area di memoria che contiene l'indirizzo
                                           // dell'elemento
          
          PointerToAddrPrevClass->NextElementAddr=0;
          // Adesso il penultimo elemento non ha nessun successivo.
          
          MyListHandle->NrElements      = MyListHandle->NrElements-1;  
          MyListHandle->LastElementAddr = PointerToAddrPrevClass;
          
          return 0;
    }
  }  
  else
      return ERR_LIST_IS_EMPTY;

}



void ndDB_DestroyListOfClass (struct HEADLIST_Class_Type *MyListHandle)
/*
   Questa routine provvede a cancellare tutti le classi che sono memorizzati nella relativa
   lista.
   La lista da cancellare viene specificata dalla struttura HEADLIST_Class_Type. La procedura
   chiamante deve passare l'indirizzo di tale struttura.

   Non sono previsti codici di errore per questa routine.
*/

{
       
  while (MyListHandle->NrElements!=0)
     INTERNAL_RemoveLastClassFromList (MyListHandle);
     
}


char ndDB_FindClassFromExtension (struct HEADLIST_Class_Type *MyListHandle, char *Ext, 
                                    struct ELEMENT_Class_Type *MyClass)
/*
  Questa routine si occupa di cercare tra le varie classi quella che gestisce
  l'estensione Ext.
  
  A quel punto, i dati relativi all'estensione vengono copiati nell'elemento
  MyClass. La routine chiamante deve avere allocato tale elemento in stack,
  e ne passa l'indirizzo come terzo parametro.
  
  La routine restituisce i seguenti codici di errore:
     
  ERR_LIST_IS_EMPTY            La lista è vuota
  ERR_EXT_NOT_FOUND            L'estensione non è stata trovata
*/
{
   struct ELEMENT_Class_Type   *PointerToClass;
   struct ELEMENT_Generic_Type *PointerToAddrClass;
   
   int PointerToRam;
   char YouCanExit;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, Ext, strlen (Ext), 1);
       // Trasforma la ext in caratteri maiuscoli                           
       
       while ( strlen (InternalBuffer)<3 ) strcat (InternalBuffer, " ");      
       // Aggiunge spazi all'estensione in modo che sia sempre di 3 caratteri
                                  
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrClass = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToClass     = PointerToAddrClass->Addr;
         // In PointerToClass ci sono i dati relativi alla classe, in PointerToAddrClass ci
         // sono i dati relativi all'indirizzo dei dati della classe ed al prossimo elemento
      
         if (strstr (PointerToClass->Extensions, InternalBuffer) != 0 )  // abbiamo trovato l'estensione
         {     
           strcpy (MyClass->NameClass,    PointerToClass->NameClass);
           strcpy (MyClass->Extensions,   PointerToClass->Extensions);
           strcpy (MyClass->StringIDIcon, PointerToClass->StringIDIcon);
           MyClass->ThumbFunction = PointerToClass->ThumbFunction;
           
           return 0;
         }
         
         if (PointerToAddrClass->NextElementAddr!=0)
           PointerToAddrClass = PointerToAddrClass->NextElementAddr;
         else
           return ERR_EXT_NOT_FOUND;
           
       }  // End of while
        
   }
   else
    return ERR_LIST_IS_EMPTY;
    
}  


char ndDB_FindClassAddrFromExtension (struct HEADLIST_Class_Type *MyListHandle, char *Ext, 
                                 int *MyClassAddr)
/*
  Questa routine si occupa di cercare tra le varie classi quella che gestisce
  l'estensione Ext.
  
  A quel punto, l'indirizzo di tale classe viene copiato nell'int MyClassAddr.
  
  La routine chiamante deve avere allocato l'int dove depositare tale infomazione,
  e ne passa l'indirizzo come terzo parametro.
  
  La routine restituisce i seguenti codici di errore:
     
  ERR_LIST_IS_EMPTY            La lista è vuota
  ERR_EXT_NOT_FOUND            L'estensione non è stata trovata
*/
{
   struct ELEMENT_Class_Type   *PointerToClass;
   struct ELEMENT_Generic_Type *PointerToAddrClass;
   
   int PointerToRam;
   char YouCanExit;
   
   char InternalBuffer [255];
   
   if (MyListHandle->NrElements>0)
   {
       ndStrCpy (InternalBuffer, Ext, strlen (Ext), 1);
       // Trasforma la ext in caratteri maiuscoli                           
       
       while ( strlen (InternalBuffer)<3 ) strcat (InternalBuffer, " ");      
       // Aggiunge spazi all'estensione in modo che sia sempre di 3 caratteri
                                  
       PointerToRam       = MyListHandle->FirstElementAddr;
       PointerToAddrClass = PointerToRam;
       // Previene un malfunzionamento del compilatore: la scelta più logica sarebbe stata passare
       // direttamente &PointerToClass alla routine precedente, ma si verifica un errore.
     
       YouCanExit=0;
       
       while (!YouCanExit)
       {
         PointerToClass     = PointerToAddrClass->Addr;
         // In PointerToClass ci sono i dati relativi alla classe, in PointerToAddrClass ci
         // sono i dati relativi all'indirizzo dei dati della classe ed al prossimo elemento
      
         if (strstr (PointerToClass->Extensions, InternalBuffer) != 0 )  // abbiamo trovato l'estensione
         {     
           *MyClassAddr = PointerToClass;
           // Copia l'indirizzo
           
           return 0;
         }
         
         if (PointerToAddrClass->NextElementAddr!=0)
           PointerToAddrClass = PointerToAddrClass->NextElementAddr;
         else
           return ERR_EXT_NOT_FOUND;
           
       }  // End of while
        
   }
   else
    return ERR_LIST_IS_EMPTY;
    
}  



char ndDB_ModifyClass (struct HEADLIST_Class_Type *MyListHandle, char *OldNameClass, char *NewNameClass, 
                  char *Extensions, char *StringIDIcon, int PointerToThumbFunc)
/*
    Questa routine si occupa di modificare i dati relativi alla classe OldNameClass, contenuta
    nella lista delle classi MyListHandle. 
    
    Qualora nella lista fosse già presente un classe con un NameClass identico a NewNameClass, 
    la routine non eseguirà la memorizzazione e restituirà un errore.
   
    Sono ammesse solo NameClass di dimensione inferiore a 63 caratteri.
   
   Sono previsti i codici di errore:
      ERR_CLASS_NOT_FOUND              La classe non è stata trovata
      ERR_MEMORY_OVERFLOW              La memoria non è sufficiente
      ERR_MEMORY_ERROR                 Errore nella memoria
      ERR_NAMECLASS_TOO_LONG           La NameClass è troppo lunga
      ERR_SAME_NAMECLASS_IS_PRESENT    E' già presente la medesima NameClass.
      
*/

{      
   struct ELEMENT_Class_Type *PointerToExClass;
   char ErrRep;
   
   unsigned char LenNameClass;
   
   LenNameClass = strlen (NewNameClass);
   
   if (LenNameClass<62)                            // Controlla che NameClass sia di meno di 11 caratteri          
   {
      if (ndDB_IsPresentSameNameClass (MyListHandle, NewNameClass)==0)    // Se NewNameClass non è già presente
      {
        ErrRep = ndDB_GetClassAddrFromList ( MyListHandle, OldNameClass, &PointerToExClass);
        // Fa in modo che PointerToExClass punti alle informazioni della classe con il
        // vecchio nome
        
        if (ErrRep==0)
        {
            LenNameClass = strlen (NewNameClass);          
                      
            ndStrCpy (PointerToExClass->NameClass, NewNameClass, LenNameClass, 1);
            // Copia la string ID nello spazio allocato
                           
            ndStrCpy (PointerToExClass->Extensions, Extensions, 254, 1);
            // Copia la stringa estensioni
            
            ndStrCpy (PointerToExClass->StringIDIcon, StringIDIcon, 11, 1);
            // Copia StringID della relativa icona
            
            PointerToExClass->ThumbFunction = PointerToThumbFunc; 
            // Copia il puntatore della funzione thumbnail                         
        }
        else
         return ErrRep;
                                             
      }
      else
        return ERR_SAME_NAMECLASS_IS_PRESENT;
   }       
   else
     return ERR_NAMECLASS_TOO_LONG;     
}

    
void InitializeStandardClassList()
{  
   ndDB_AppendClassToList (&SystemClasses, "Image", "BMP,TGA,JPG,PNG,GIF,ICO,MNG,PCD,PCX,PIX,PNM,PGM,PBM,PSD,SGI,TIF,XPM", IDImageIcon, &ndIMG_CreateThumbnailFromImage);     
   
   ndDB_AppendClassToList (&SystemClasses, "Office document", "TXT,DOC,XLS", IDOfficeIcon, 0); 
   
   ndDB_AppendClassToList (&SystemClasses, "Video", "AVI,MOV,MPG", IDVideoIcon, 0);     

   ndDB_AppendClassToList (&SystemClasses, "Audio", "MP2,MP3,WAV,MID,MOV", IDAudioIcon, 0);     

   ndDB_AppendClassToList (&SystemClasses, "Documento PDF", "PDF", IDPDFIcon, 0);     
}



