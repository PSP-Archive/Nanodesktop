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
    ROUTINES PER LA GESTIONE DELL'API WINMENU
    
    Queste routine sono quelle usate dall'utente per la definizione delle voci
    di finestra
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

char ndWMENU_DefineMenuItem (unsigned char WndHandle, char *ZString, char *NameItem, char Features, void *CallbackFunction, unsigned char RenderNow)
/*
   Questa routine definisce una voce di men� con determinate caratteristiche per la finestra 
   specificata dal parametro WndHandle. 
   
   La voce che viene creata pu� essere di tipo 1 o di tipo 2: ci� viene determinato dalla stringa
   ZString. 
   
   Il campo Features � un campo a 8 bit che contiene informazioni sulle caratteristiche della
   voce: i suoi bit sono mappati nel seguente modo
   
   1         Se posto a 1 la voce di men� � inibita: usate la costante manifesta ND_WMI_INHIBITED
   4         Se posto a 1, quando verr� eseguita una nuova callback sar� usato un nuovo contesto:
             usate la costante manifesta ND_CALLBACK_IN_NEW_CONTEXT
   
   Se RenderNow � posto a 1, al termine dell'operazione la routine richiama ndWinMenuRender per 
   visualizzare subito le modifiche alle voci di men�.
   
   Sono previsti i seguenti codici di errore:
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        
*/

{
   char ErrRep;    
   char Item_Inhibited, ExecInNewContext; 
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
             if ((Features & ND_WMI_INHIBITED) == ND_WMI_INHIBITED) Item_Inhibited=1;
                else Item_Inhibited=0;
                
             if ((Features & ND_CALLBACK_IN_NEW_CONTEXT) == ND_CALLBACK_IN_NEW_CONTEXT) ExecInNewContext=1;
                else ExecInNewContext=0;  
             
             ErrRep = INTERNAL_ndDB_MenuParser_Write (&(WindowData [WndHandle].MainWinMenu), ZString, NameItem, Item_Inhibited, ExecInNewContext, 0, CallbackFunction);
             // ItemExpanded all'inizio viene posto sempre a 0
             
             if (ErrRep!=0) return ErrRep;
             
             WindowData [WndHandle].MenuBarIsChanged=1;      
             if (RenderNow) XWindowRender (WndHandle);
             return 0;
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;
}


char ndWMENU_FeaturesMenuItem (unsigned char WndHandle, char *ZString, int *AddressGet)
/*
   Questa routine consente di ottenere le caratteristiche di una voce di men� nella finestra 
   specificata dal parametro WndHandle. 
   
   La voce che viene analizzata viene specificata dalla stringa ZString. 
   
   La routine chiamante deve allocare una variabile di tipo struct ELEMENT_L1WMI_Type* o
   struct ELEMENT_L2WMI_Type* atta a contenere un indirizzo a 32 bit.
   
   Poi, passa a questa routine l'indirizzo di questo puntatore per mezzo del parametro
   AddressGet. 
   
   Alla fine dell'operazione, se questa ha avuto successo, nel puntatore allocato dalla
   funzione chiamante ci sar� l'indirizzo dell'elemento cercato.
   
   Sono previsti i seguenti codici di errore:
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
        
*/

{        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
           return INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, AddressGet); 
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;
}


int ndWMENU_NumberOfL1WMIs (char WndHandle, char *ZString)
/* 
    Questa routine si porta su una voce di men� nella finestra WndHandle e restituisce
    il numero di elementi nel sottomen� di tipo 1 che risultano essere 
    presenti.
    
    La voce di men� che viene analizzata � quella determinata da ZString. 
   
    Sono previsti i seguenti codici di errore:
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
              
*/
{
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
           return  INTERNAL_ndDB_NumberOfL1WMIs (&(WindowData [WndHandle].MainWinMenu), ZString); 
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;
}


int ndWMENU_NumberOfL2WMIs (char WndHandle, char *ZString)
/* 
    Questa routine si porta su una voce di men� nella finestra WndHandle e restituisce
    il numero di elementi nel sottomen� di tipo 2 che risultano essere 
    presenti.
    
    La voce di men� che viene analizzata � quella determinata da ZString. 
   
    Sono previsti i seguenti codici di errore:
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
              
*/
{        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
           return  INTERNAL_ndDB_NumberOfL2WMIs (&(WindowData [WndHandle].MainWinMenu), ZString); 
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;
}


char ndWMENU_EnumL1WMI (char WndHandle, char *ZString, int NrElement, int *AddressGet)
/*
   Questa routine consente di enumerare le voci di sottomen� di livello 1 che sono
   agganciate ad una voce di men� nella finestra WndHandle. 
   
   La voce che viene analizzata viene specificata dalla stringa ZString. 
   
   La routine chiamante deve allocare una variabile di tipo struct ELEMENT_L1WMI_Type* o
   struct ELEMENT_L2WMI_Type* atta a contenere un indirizzo a 32 bit.
   
   Poi, passa a questa routine l'indirizzo di questo puntatore per mezzo del parametro
   AddressGet. 
   
   Alla fine dell'operazione, se questa ha avuto successo, nel puntatore allocato dalla
   funzione chiamante ci sar� l'indirizzo dell'elemento cercato.
   
   Sono previsti i seguenti codici di errore:
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
         
*/
{
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
           return  INTERNAL_ndDB_EnumL1WMI (&(WindowData [WndHandle].MainWinMenu), ZString, NrElement, AddressGet); 
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}


char ndWMENU_EnumL2WMI (char WndHandle, char *ZString, int NrElement, int *AddressGet)
/*
   Questa routine consente di enumerare le voci di sottomen� di livello 2 che sono
   agganciate ad una voce di men� nella finestra WndHandle. 
   
   La voce che viene analizzata viene specificata dalla stringa ZString. 
   
   La routine chiamante deve allocare una variabile di tipo struct ELEMENT_L1WMI_Type* o
   struct ELEMENT_L2WMI_Type* atta a contenere un indirizzo a 32 bit.
   
   Poi, passa a questa routine l'indirizzo di questo puntatore per mezzo del parametro
   AddressGet. 
   
   Alla fine dell'operazione, se questa ha avuto successo, nel puntatore allocato dalla
   funzione chiamante ci sar� l'indirizzo dell'elemento cercato.
   
   Sono previsti i seguenti codici di errore:
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
         
*/
{              
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
           return INTERNAL_ndDB_EnumL2WMI (&(WindowData [WndHandle].MainWinMenu), ZString, NrElement, AddressGet);
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}


char ndWMENU_InhibitL1WMI (char WndHandle, char *ZString, char RenderNow)
/* 
   Questa routine inibisce una voce di men� L1WMI. L'inibizione ha effetto 
   immediato, ma l'aggiornamento del men� grafico avviene immediatamente solo
   se si pone RenderNow=1. Se si pone RenderNow=0 l'aggiornamento avverr� solo 
   quando verr� eseguita una XWindowRender. 
   
   Sono previsti i seguenti codici di errore:
       ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
       ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
       ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
       ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
       ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
       ERR_INTERNAL_ERROR            Errore interno
       ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto 
*/

{
   struct ELEMENT_L1WMI_Type *MyL1WMI; 
   
   char ErrRep;     
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
               ErrRep=INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, &MyL1WMI);
               
               if (ErrRep==0)
               {
                   MyL1WMI->Item_Inhibited=1;
                   WindowData [WndHandle].MenuBarIsChanged=1;
                   
                   if (RenderNow) XWindowRender (WndHandle);
               } 
               
               return ErrRep;
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}




char ndWMENU_DeInhibitL1WMI (char WndHandle, char *ZString, char RenderNow)
/* 
   Questa routine deinibisce una voce di men� L1WMI. La deinibizione ha effetto 
   immediato, ma l'aggiornamento del men� grafico avviene immediatamente solo
   se si pone RenderNow=1. Se si pone RenderNow=0 l'aggiornamento avverr� solo 
   quando verr� eseguita una XWindowRender. 
   
   Sono previsti i seguenti codici di errore:
       ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
       ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
       ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
       ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
       ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
       ERR_INTERNAL_ERROR            Errore interno
       ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto 
*/

{
   struct ELEMENT_L1WMI_Type *MyL1WMI; 
   
   char ErrRep;     
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
           if (WindowData [WndHandle].MenuSupported)
           {
               ErrRep=INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, &MyL1WMI);
               
               if (ErrRep==0)
               {
                   MyL1WMI->Item_Inhibited=0;
                   WindowData [WndHandle].MenuBarIsChanged=1;
                   
                   if (RenderNow) XWindowRender (WndHandle);
               } 
               
               return ErrRep;
           }
           else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}


char ndWMENU_InhibitL2WMI (char WndHandle, char *ZString, char RenderNow)
/* 
   Questa routine inibisce una voce di men� L2WMI. L'inibizione ha effetto 
   immediato, ma l'aggiornamento del men� grafico avviene immediatamente solo
   se si pone RenderNow=1. Se si pone RenderNow=0 l'aggiornamento avverr� solo 
   quando verr� eseguita una XWindowRender. 
   
   Sono previsti i seguenti codici di errore:
       ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
       ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
       ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
       ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
       ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
       ERR_INTERNAL_ERROR            Errore interno
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto 
*/

{
   struct ELEMENT_L2WMI_Type *MyL2WMI; 
   char ErrRep;     
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
               ErrRep=INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, &MyL2WMI);
               
               if (ErrRep==0)
               {
                   MyL2WMI->Item_Inhibited=1;
                   WindowData [WndHandle].MenuBarIsChanged=1;
                   
                   if (RenderNow) XWindowRender (WndHandle);
               } 
               
               return ErrRep;
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}




char ndWMENU_DeInhibitL2WMI (char WndHandle, char *ZString, char RenderNow)
/* 
   Questa routine deinibisce una voce di men� L2WMI. La deinibizione ha effetto 
   immediato, ma l'aggiornamento del men� grafico avviene immediatamente solo
   se si pone RenderNow=1. Se si pone RenderNow=0 l'aggiornamento avverr� solo 
   quando verr� eseguita una XWindowRender. 
   
   Sono previsti i seguenti codici di errore:
       ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
       ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
       ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
       ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
       ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
       ERR_INTERNAL_ERROR            Errore interno
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto 
*/

{
   struct ELEMENT_L2WMI_Type *MyL2WMI; 
   
   char ErrRep;     
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
               ErrRep=INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, &MyL2WMI);
               
               if (ErrRep==0)
               {
                   MyL2WMI->Item_Inhibited=0;
                   WindowData [WndHandle].MenuBarIsChanged=1;
                   
                   if (RenderNow) XWindowRender (WndHandle);
               } 
               
               return ErrRep;
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}


char ndWMENU_ChangeCallback_For_L1WMI (char WndHandle, char *ZString, int NewCallback)
/* 
   Questa routine cambia l'indirizzo della callback per un certo L1WMI. Pu�
   anche disattivare la callback: � sufficiente passare il parametro 
   NewCallback=0.
   
   Sono previsti i seguenti codici di errore:
       ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
       ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
       ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
       ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
       ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
       ERR_INTERNAL_ERROR            Errore interno
       ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto 
*/

{
   struct ELEMENT_L1WMI_Type *MyL1WMI; 
   
   char ErrRep;     
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
               ErrRep=INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, &MyL1WMI);
               
               if (ErrRep==0)  MyL1WMI->CallbackFunction=NewCallback;
               return ErrRep;
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}


char ndWMENU_ChangeCallback_For_L2WMI (char WndHandle, char *ZString, int NewCallback)
/* 
   Questa routine cambia l'indirizzo della callback per un certo L2WMI. Pu�
   anche disattivare la callback: � sufficiente passare il parametro 
   NewCallback=0.
   
   Sono previsti i seguenti codici di errore:
       ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
       ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
       ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
       ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
       ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
       ERR_INTERNAL_ERROR            Errore interno
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
       ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto 
*/

{
   struct ELEMENT_L2WMI_Type *MyL2WMI; 
   
   char ErrRep;     
        
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
               ErrRep=INTERNAL_ndDB_MenuParser_Read (&(WindowData [WndHandle].MainWinMenu), ZString, &MyL2WMI);
               
               if (ErrRep==0)  MyL2WMI->CallbackFunction=NewCallback;
               return ErrRep;
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;      
}




char ndWMENU_DestroyL1SubMenu (char WndHandle, char *ZString)
/*
   Questa routine consente di cancellare le voci di sottomen� di livello 1 che sono
   agganciate ad una voce di men� nella finestra WndHandle. 
   
   La voce che viene analizzata viene specificata dalla stringa ZString. 
   
   Sono previsti i seguenti codici di errore:
         
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
         
*/
{
   struct ELEMENT_L1WMI_Type  *L1WMI;
   char ErrRep;      
         
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
               if (strcmp (ZString, "")==0)
               {
                  ndDB_DestroyListOfL1WMI (& (WindowData [WndHandle].MainWinMenu) );
               }
               else
               {
                  ErrRep = ndWMENU_FeaturesMenuItem (WndHandle, ZString, &(L1WMI) );
                  
                  if (ErrRep!=0) return ErrRep;   // Non ho trovato l'item richiesto
                  
                  if (L1WMI->L1SubMenu!=0)
                  {
                        ndDB_DestroyListOfL1WMI (L1WMI->L1SubMenu);       
                  } 
               } 
           
           WindowData [WndHandle].MenuBarIsChanged=1;
           return 0; 
            
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;         
         
}



char ndWMENU_DestroyL2SubMenu (char WndHandle, char *ZString)
/*
   Questa routine consente di cancellare le voci di sottomen� di livello 2 che sono
   agganciate ad una voce di men� nella finestra WndHandle. 
   
   La voce che viene analizzata viene specificata dalla stringa ZString. 
   
   Sono previsti i seguenti codici di errore:
         
        ERR_INVALID_WINHANDLE         L'handle di finestra non � valido
        ERR_WINDOW_IS_NOT_ALLOCATED   La finestra non � allocata
        ERR_WINMENU_NOT_SUPPORTED     La finestra non supporta i winmenu
        ERR_ZSTRING_TOO_LONG          La ZString � lunga pi� di 512 caratteri
        ERR_SYNTAX_ERROR              Errore di sintassi nella ZString
        ERR_INTERNAL_ERROR            Errore interno
        ERR_L1WMI_NOT_FOUND           Non � stato trovato l'L1WMI richiesto
        ERR_L2WMI_NOT_FOUND           Non � stato trovato l'L2WMI richiesto
         
*/
{
   struct ELEMENT_L1WMI_Type  *L1WMI;
   char ErrRep;      
         
   if (WndHandle<__NRWINDOWSALLOCABLE)
   {
      if (WindowData [WndHandle].WindowIsActive)
      {
         if (WindowData [WndHandle].MenuSupported)
         {
           if (strcmp (ZString, "")==0)
           {
              ndDB_DestroyListOfL1WMI (& (WindowData [WndHandle].MainWinMenu) );
           }
           else
           {
              ErrRep = ndWMENU_FeaturesMenuItem (WndHandle, ZString, &(L1WMI) );
              
              if (ErrRep!=0) return ErrRep;   // Non ha trovato l'item richiesto  
              
              if (L1WMI->L2SubMenu!=0)
              {
                  ndDB_DestroyListOfL2WMI (L1WMI->L2SubMenu);                     
              }
              
           } 
           
           WindowData [WndHandle].MenuBarIsChanged=1;
           return 0;  
           
         }
         else return ERR_WINMENU_NOT_SUPPORTED;
      }
      else return ERR_WINDOW_IS_NOT_ALLOCATED;
   }
   else return ERR_INVALID_WINHANDLE;         
         
}





