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
    ROUTINES PER LA GESTIONE DI L1/L2 WINMENU ITEMS
    
    Queste routine si pongono ad un livello superiore rispetto a quelle contenute
    nei files L1WinMenuList.c o L2WinMenuList.c.
    Essi gestiscono il sistema di definizione accesso ai menù per mezzo del
    parser della stringa ZString.
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

char INTERNAL_ndDB_MenuParser_Read (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, int *AddressGet)
/*
   Questa routine consente di leggere una lista mista di L1WMI e di L2WMI. 
   
   La routine chiamante deve allocare lo spazio necessario per allocare un puntatore 
   di tipo struct HEADLIST_L1WMI_Type* oppure struct HEADLIST_L2WMI_Type* e deve
   passare l'indirizzo della variabile dove memorizzare il puntatore per mezzo
   del parametro AddressGet.
*/
{
       
  struct ELEMENT_L1WMI_Type   *Req_ELEMENT_L1WMI;
    
  char InternalBuffer1 [512];
  char InternalBuffer2 [512];
  
  int Counter, PointPos;
  
  int IHaveFound;
  int ZStringLen;
  
  static char ErrRep;   // E' fondamentale che sia dichiarato static, altrimenti si ha un errore
                        // dell'emulatore PSPE
  
  ZStringLen = strlen (ZString);
  
  if (ZStringLen>=512) return ERR_ZSTRING_TOO_LONG;
    
  IHaveFound=0;
  
  for (Counter=0; ((Counter<ZStringLen) && (!IHaveFound)); Counter++)
  {
      if ((ZString [Counter]=='/') || (ZString [Counter]=='.')) 
      {
         PointPos=Counter;
         IHaveFound=1;
      }          
  } 
  
  if (!IHaveFound)     // Non è stato trovato un punto o un backslash
  {
     return ndDB_GetL1WMIAddrFromList_UsingStringID (MyL1Menu, ZString, AddressGet);
     
     // Copia l'indirizzo dell'elemento richiesto (se presente), nella locazione di
     // memoria il cui indirizzo è contenuto in AddressGet.     
  }
  else                 // E' stato trovato un punto o un backslash
  {
     if (ZString [PointPos]=='.')           // E' stato trovato un punto
     {         
        if ((PointPos==0) || (PointPos==ZStringLen))    // Il punto è all'inizio o alla fine
           return ERR_SYNTAX_ERROR;                     // Riporta errore di sintassi alla routine chiamante
     
        ndStrCpyBounded (InternalBuffer1, ZString, 0, PointPos-1, 0);
        ndStrCpyBounded (InternalBuffer2, ZString, PointPos+1, ZStringLen, 0);
        
        // Verifichiamo che non vi sia un secondo punto o uno slash
        
        for (Counter=0; Counter<strlen (InternalBuffer2); Counter++) 
        {
            if ((InternalBuffer2 [Counter]=='.') || (InternalBuffer2 [Counter]=='/'))
               return ERR_SYNTAX_ERROR;
        }
        
        // Cerchiamo prima di determinare l'indirizzo del L1WMI richiesto
        
        ErrRep = ndDB_GetL1WMIAddrFromList_UsingStringID (MyL1Menu, InternalBuffer1, &(Req_ELEMENT_L1WMI) );
     
        if (ErrRep==0)
        {  
           if (Req_ELEMENT_L1WMI->L2SubMenu!=0)
           {
              return ndDB_GetL2WMIAddrFromList_UsingStringID (Req_ELEMENT_L1WMI->L2SubMenu, InternalBuffer2, AddressGet);
                     // N.B. Si noti che L2SubMenu è già un indirizzo, quindi si deve passare L2SubMenu
                     // e non &(L2SubMenu)
           }
           else   
           {
              return ERR_L2WMI_NOT_FOUND;
              // Se non era stato allocato alcun L2WMIMenu, vuol dire che L2WMI non è stato 
              // trovato       
           }   
        }
        else
          return ERR_L1WMI_NOT_FOUND;      
        
     }
     
     if (ZString [PointPos]=='/')           // E' stato trovato un backslash
     {           
        if ((PointPos==0) || (PointPos==ZStringLen))    // Il backslash è all'inizio o alla fine
           return ERR_SYNTAX_ERROR;                     // Riporta errore di sintassi alla routine chiamante
        
        ndStrCpyBounded (InternalBuffer1, ZString, 0, PointPos-1, 0);
        ndStrCpyBounded (InternalBuffer2, ZString, PointPos+1, ZStringLen, 0);
        
        // Verifichiamo che l'elemento L1WMI del quale si vuole esplorare il sottomenù
        // L1WMI sia presente
                
        ErrRep = ndDB_GetL1WMIAddrFromList_UsingStringID (MyL1Menu, InternalBuffer1, &(Req_ELEMENT_L1WMI) );
        
        if (ErrRep==0)                 // L'elemento L1WMI è già presente
        {
           
           if ( (Req_ELEMENT_L1WMI->L1SubMenu)!=0)   
           {
              // Richiama se stessa in modo da potere esplorare il sottomenù                                
              
              return INTERNAL_ndDB_MenuParser_Read ( Req_ELEMENT_L1WMI->L1SubMenu, InternalBuffer2, AddressGet);
           }
           else  // Se non è stato agganciato nessun sottomenù di tipo 1, allora di certo non è stato trovato
           {     // ciò che stiamo cercando
            
              return ERR_L1WMI_NOT_FOUND;
           }
             
        }
        else
          return ERR_L1WMI_NOT_FOUND; 
     }
  }            
}





char INTERNAL_ndDB_MenuParser_Write (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, char *NameItem, char Item_Inhibited, char ExecInNewContext, char Item_Expanded, void *CallbackFunction)
/*
   Questa routine consente di memorizzare una nuova voce di menù.
*/
{
  struct ELEMENT_L1WMI_Type    *PointerToL1WMI;
  
  struct HEADLIST_L1WMI_Type   *New_HEADLIST_L1WMI;
  struct HEADLIST_L2WMI_Type   *New_HEADLIST_L2WMI; 
    
  char InternalBuffer1 [512];
  char InternalBuffer2 [512];
  
  int Counter, PointPos;
  
  int IHaveFound;
  int ZStringLen;
  
  static char ErrRep;   // E' fondamentale che sia dichiarato static, altrimenti si ha un errore
                        // dell'emulatore PSPE
  
  ZStringLen = strlen (ZString);
  
  if (ZStringLen>=512) return ERR_ZSTRING_TOO_LONG;
    
  IHaveFound=0;
  
  for (Counter=0; ((Counter<ZStringLen) && (!IHaveFound)); Counter++)
  {
      if ((ZString [Counter]=='/') || (ZString [Counter]=='.')) 
      {
         PointPos=Counter;
         IHaveFound=1;
      }          
  } 
  
  if (!IHaveFound)     // Non è stato trovato un punto o un backslash
  {
     return ndDB_Append_L1WMI_ToList (MyL1Menu, ZString, NameItem, Item_Inhibited, ExecInNewContext, Item_Expanded, CallbackFunction);
  }
  else                 // E' stato trovato un punto o un backslash
  {
     if (ZString [PointPos]=='.')           // E' stato trovato un punto
     {         
        if ((PointPos==0) || (PointPos==ZStringLen))    // Il punto è all'inizio o alla fine
           return ERR_SYNTAX_ERROR;                     // Riporta errore di sintassi alla routine chiamante
     
        ndStrCpyBounded (InternalBuffer1, ZString, 0, PointPos-1, 0);
        ndStrCpyBounded (InternalBuffer2, ZString, PointPos+1, ZStringLen, 0);
        
        // Verifichiamo che non vi sia un secondo punto o uno slash
        
        for (Counter=0; Counter<strlen (InternalBuffer2); Counter++) 
        {
            if ((InternalBuffer2 [Counter]=='.') || (InternalBuffer2 [Counter]=='/'))
               return ERR_SYNTAX_ERROR;
        }
        
        // Verifichiamo che l'elemento L1WMI di cui si vuole creare un L2WMI figlio sia 
        // già presente
        
        ErrRep = ndDB_GetL1WMIAddrFromList_UsingStringID (MyL1Menu, InternalBuffer1, &PointerToL1WMI);
        if (ErrRep!=0) return ERR_L1WMI_NOT_FOUND;
                 
        // A questo punto PointerToL1WMI di sicuro contiene quanto necessario
                   
        if (PointerToL1WMI->L2SubMenu==0)   // Il submenu di tipo 2 per questo elemento non era 
                                              // ancora stato agganciato
        {
              New_HEADLIST_L2WMI = malloc  ( sizeof (*New_HEADLIST_L2WMI) );
              // Alloca lo spazio per il descrittore della lista di livello 2
              
              ndDB_InitList_of_L2WMI (New_HEADLIST_L2WMI, 1);
              // Inizializza il nuovo descrittore di lista e segnala che è stato allocato
              // dinamicamente
              
              PointerToL1WMI->L2SubMenu = New_HEADLIST_L2WMI;
              // Esegue l'aggancio del menù di tipo 2 sulla voce del menù di tipo 1
                     
        }
           
        // Adesso il submenu è agganciato, e posso eseguire l'append della nuova voce
           
        return ndDB_Append_L2WMI_ToList (PointerToL1WMI->L2SubMenu, InternalBuffer2, NameItem, Item_Inhibited, ExecInNewContext, CallbackFunction);
        
        // N.B. Il parametro Item_Expanded passato a questa routine viene sempre ignorato, perchè gli L2WMI
        // non hanno un campo apposito.
     }
     
     if (ZString [PointPos]=='/')           // E' stato trovato un backslash
     {
        if ((PointPos==0) || (PointPos==ZStringLen))    // Il backslash è all'inizio o alla fine
           return ERR_SYNTAX_ERROR;                     // Riporta errore di sintassi alla routine chiamante
        
        ndStrCpyBounded (InternalBuffer1, ZString, 0, PointPos-1, 0);
        ndStrCpyBounded (InternalBuffer2, ZString, PointPos+1, ZStringLen, 0);
        
        // Verifichiamo che l'elemento L1WMI a cui si vuole agganciare un menù L1WMI sia
        // già presente 
                
        ErrRep = ndDB_GetL1WMIAddrFromList_UsingStringID (MyL1Menu, InternalBuffer1, &PointerToL1WMI);
        
        if (ErrRep!=0)      // L'elemento richiesto non è stato trovato.
        {                   // Vuol dire che lo aggiungiamo
        
          ErrRep = ndDB_Append_L1WMI_ToList (MyL1Menu, InternalBuffer1, "", 0, 0, 0, 0);
          
          if (ErrRep==0)    // Il nuovo elemento è stato aggiunto con successo
          {
              ErrRep = ndDB_GetL1WMIAddrFromList_UsingStringID (MyL1Menu, InternalBuffer1, &PointerToL1WMI);
              // Preleva l'indirizzo del nuovo elemento appena creato
          
              if (ErrRep!=0) return ERR_INTERNAL_ERROR;
          }
        } 
        
        // Arrivati a questo punto, l'elemento L1WMI è di sicuro già presente
        {
           if (PointerToL1WMI->L1SubMenu==0)   // Il submenu di tipo 1 per questo elemento non era 
                                              // ancora stato agganciato
           {
              New_HEADLIST_L1WMI = malloc  ( sizeof (*New_HEADLIST_L1WMI) );
              // Alloca lo spazio per il descrittore della lista di livello 1
              
              ndDB_InitList_of_L1WMI (New_HEADLIST_L1WMI, 1);
              // Inizializza il nuovo descrittore di lista e segnala che è stato allocato
              // dinamicamente
              
              PointerToL1WMI->L1SubMenu = New_HEADLIST_L1WMI;
              // Esegue l'aggancio del menù di tipo 1 sulla voce del menù di tipo 1
           }
           
           // Adesso il submenu è agganciato, e posso eseguire l'append della nuova voce
           // L'append viene eseguito richiamando questa funzione ricorsivamente, usando
           // come ZString quanto contenuto in InternalBuffer2. 
           
           return INTERNAL_ndDB_MenuParser_Write (PointerToL1WMI->L1SubMenu, InternalBuffer2, NameItem, Item_Inhibited, ExecInNewContext,
                                              Item_Expanded, CallbackFunction);         
        }
         
     }
  }            
}


int INTERNAL_ndDB_NumberOfL1WMIs (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString)
/* 
    
    Questa routine si porta sull'elemento caratterizzato dalla stringa ZString
    e restituisce il numero di elementi nel sottomenù di tipo 1 che risultano
    essere presenti
    
*/
{
    struct ELEMENT_L1WMI_Type  *PointerToL1WMI;
    struct HEADLIST_L1WMI_Type *PointerToHL_L1WMI;
    
    char ErrRep;
    
    if (strcmp (ZString, "")==0)
    {
       return MyL1Menu->NrElements;
    }
    
    ErrRep = INTERNAL_ndDB_MenuParser_Read (MyL1Menu, ZString, &PointerToL1WMI);
    
    if (!ErrRep)
    {
       if (PointerToL1WMI->L1SubMenu!=0)      // C'è un submenu di tipo 1 agganciato
       {
           PointerToHL_L1WMI = PointerToL1WMI->L1SubMenu;
           return PointerToHL_L1WMI->NrElements;
       }
       else  // Non c'è alcun menù di tipo 1 agganciato.
       {
           return 0;
       }
    }
    else 
      return ErrRep; 
}




int INTERNAL_ndDB_NumberOfL2WMIs (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString)
/* 
    
    Questa routine si porta sull'elemento caratterizzato dalla stringa ZString
    e restituisce il numero di elementi nel sottomenù di tipo 2 che risultano
    essere presenti
    
*/
{    
    struct ELEMENT_L1WMI_Type  *PointerToL1WMI;
    struct HEADLIST_L2WMI_Type *PointerToHL_L2WMI;
    
    char ErrRep;
    
    if (strcmp (ZString, "")==0)
    {
       return MyL1Menu->NrElements;
    }
    
    ErrRep = INTERNAL_ndDB_MenuParser_Read (MyL1Menu, ZString, &PointerToL1WMI);
    
    if (!ErrRep)
    {
       if (PointerToL1WMI->L2SubMenu!=0)      // C'è un submenu di tipo 2 agganciato
       {
           PointerToHL_L2WMI = PointerToL1WMI->L2SubMenu;
           return PointerToHL_L2WMI->NrElements;
       }
       else  // Non c'è alcun menù di tipo 2 agganciato.
       {
           return 0;
       }
    }
    else 
      return ErrRep; 
}



char INTERNAL_ndDB_EnumL1WMI (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, int NrElement, int *AddressGet)
/*
   Questa routine si occupa di fornire l'indirizzo dei dati relativi all'elemento di nr. NrElement
   che è facente parte del sottomenù di livello 1 dell'elemento caratterizzato da ZString.
   
   Se si passa una ZString vuota, verranno enumerati gli items di livello 1 che stanno al primo
   livello di gerarchia.
*/

{
    struct ELEMENT_L1WMI_Type  *PointerToL1WMI;
    struct HEADLIST_L1WMI_Type *PointerToHL_L1WMI; 
    char ErrRep;   
           
    if (strcmp (ZString, "")==0)
    {
       return ndDB_GetL1WMIAddrFromList (MyL1Menu, NrElement, AddressGet);
    }
    else
    {
        ErrRep = INTERNAL_ndDB_MenuParser_Read (MyL1Menu, ZString, &PointerToL1WMI);
        
        if (!ErrRep)
        {
           if (PointerToL1WMI->L1SubMenu!=0)      // C'è un submenu di tipo 1 agganciato
           {
               PointerToHL_L1WMI = PointerToL1WMI->L1SubMenu;
               return ndDB_GetL1WMIAddrFromList (PointerToHL_L1WMI, NrElement, AddressGet);
           }
           else  
               return ERR_LIST_IS_EMPTY;  
        }
        else 
          return ErrRep;
    }    
}



char INTERNAL_ndDB_EnumL2WMI (struct HEADLIST_L1WMI_Type *MyL1Menu, char *ZString, int NrElement, int *AddressGet)
/*
   Questa routine si occupa di fornire l'indirizzo dei dati relativi all'elemento di nr. NrElement
   che è facente parte del sottomenù di livello 2 dell'elemento caratterizzato da ZString.
   
   Se si passa una ZString vuota, si otterrà un errore ERR_SYNTAX_ERROR (al primo livello
   di gerarchia non ci sono items di livello 2, ma solo di livello 1)
*/

{
    struct ELEMENT_L1WMI_Type  *PointerToL1WMI;
    struct HEADLIST_L2WMI_Type *PointerToHL_L2WMI; 
    char ErrRep;   
           
    if (strcmp (ZString, "")==0)
    {
       return ERR_SYNTAX_ERROR;
    }
    else
    {
        ErrRep = INTERNAL_ndDB_MenuParser_Read (MyL1Menu, ZString, &PointerToL1WMI);
        
        if (!ErrRep)
        {
           if (PointerToL1WMI->L2SubMenu!=0)      // C'è un submenu di tipo 2 agganciato
           {
               PointerToHL_L2WMI = PointerToL1WMI->L2SubMenu;
               
               return ndDB_GetL2WMIAddrFromList (PointerToHL_L2WMI, NrElement, AddressGet);
           }
           else  
               return ERR_LIST_IS_EMPTY;  
        }
        else 
          return ErrRep;
    }    
}
