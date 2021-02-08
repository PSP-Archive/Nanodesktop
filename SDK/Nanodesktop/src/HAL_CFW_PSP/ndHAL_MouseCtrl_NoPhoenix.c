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
    ---------------------------------------------------------------------------
    GESTORI MOUSECONTROL, EVENT MANAGER, WMI EVENT MANAGER (PSP CFW mode)
    VERSIONE NOPHOENIX
    
    Questo file contiene le routines MouseControl, Event Manager, WMI Event
    Manager utilizzate dal sistema qualora l'utente abbia disabilitato il
    sottosistema Phoenix per mezzo della direttiva ND_DISABLE_PHOENIX_SUBSYSTEM
    nel programma principale
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>

/* 
    ***************************************************************************
    ***************************************************************************
                            WMI EVENT MANAGER   (No Phoenix)
    ***************************************************************************
    ***************************************************************************
*/

void INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (unsigned char WndHandle, char Value)
{
     WindowData [WndHandle].PrevOpenL2WMIMenu = WindowData [WndHandle].OpenL2WMIMenu;
     WindowData [WndHandle].OpenL2WMIMenu     = Value;
}

void INTERNAL_NOPHOENIX_Scroll_WMI1_To_Right (char WndHandle)
/* 
    Esegue lo scrolling a destra dei menù di tipo 1
*/

{
    ndHAL_HideMousePointer ();
    
    if (WindowData [WndHandle].OpenL2WMIMenu) INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
    // L'operazione di scrolling a destra o a sinistra chiude sempre eventuali menù
    // di secondo livello aperti.
       
    WindowData [WndHandle].NrFirstL1WMIButton++;
    WindowData [WndHandle].MenuBarIsChanged=1;

    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
    ndHAL_ShowMousePointer ();
}

void INTERNAL_NOPHOENIX_Scroll_WMI1_To_Left (char WndHandle)
/* 
    Esegue lo scrolling a sinistra dei menù di tipo 1
*/

{
    ndHAL_HideMousePointer ();
    
    if (WindowData [WndHandle].OpenL2WMIMenu) INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
    // L'operazione di scrolling a destra o a sinistra chiude sempre eventuali menù
    // di secondo livello aperti.
    
    WindowData [WndHandle].NrFirstL1WMIButton--;
    WindowData [WndHandle].MenuBarIsChanged=1;
    
    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
    ndHAL_ShowMousePointer ();
}


void INTERNAL_NOPHOENIX_Scroll_WMI2_To_Up (char WndHandle)
{
    ndHAL_HideMousePointer ();
    
    WindowData [WndHandle].L2WMIScrollElement--;
    WindowData [WndHandle].MenuBarIsChanged=1;
    
    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
    ndHAL_ShowMousePointer ();
}


void INTERNAL_NOPHOENIX_Scroll_WMI2_To_Down (char WndHandle)
{
    ndHAL_HideMousePointer ();
    
    WindowData [WndHandle].L2WMIScrollElement++;
    WindowData [WndHandle].MenuBarIsChanged=1;
    
    XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
    ndHAL_ShowMousePointer ();   
}


void INTERNAL_NOPHOENIX_GoToPreviousWMIMenu (char WndHandle)
{
   char TempBuffer [512];
   short int Counter, LenString;
   char SlashFound, SlashPos;
   
   ndHAL_HideMousePointer ();
   
   SlashFound=0;
   LenString = strlen (WindowData [WndHandle].ZString);
   
   for (Counter=LenString; Counter>=0; Counter--)
   {
       if (WindowData [WndHandle].ZString [Counter] == '/')
       {
          SlashFound=1;
          SlashPos=Counter;
       }
   }
   
   if (!SlashFound)        // Non c'era nessuno slash
   {
      strcpy (WindowData [WndHandle].ZString, "");
   }
   else
   {
      ndStrCpy (TempBuffer, &(WindowData [WndHandle].ZString), SlashPos-1, 0);
      // Copia tutto ciò che c'era prima dello slash
      
      strcpy ( &(WindowData [WndHandle].ZString), TempBuffer);
   }
   
   WindowData [WndHandle].NrFirstL1WMIButton=0;
   WindowData [WndHandle].NrL1WMIButtonOpened=0;
   WindowData [WndHandle].L2WMIMenuScrollActive=0;
   WindowData [WndHandle].L2WMIScrollElement=0;
   
   INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
   
   WindowData [WndHandle].MenuBarIsChanged=1;
   
   XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
   ndHAL_ShowMousePointer ();  
}


void INTERNAL_NOPHOENIX_EnterToNew_WMI1  (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ndEvent_Type *EventAddress, char WndHandle)
{ 
   ndHAL_HideMousePointer ();
   WindowData [WndHandle].MenuBarIsChanged=1;
    
   if (strcmp (  &(WindowData[WndHandle].ZString), "" )==0)
   {
      strcpy (  &(WindowData[WndHandle].ZString), MyL1WMI->StringID );
   }
   else
   {
      strcat (  &(WindowData [WndHandle].ZString), "/" );
      strcat (  &(WindowData [WndHandle].ZString), MyL1WMI->StringID );
   }   
   
   
   WindowData [WndHandle].NrFirstL1WMIButton=0;
   WindowData [WndHandle].NrL1WMIButtonOpened=0;
   WindowData [WndHandle].L2WMIMenuScrollActive=0;
   WindowData [WndHandle].L2WMIScrollElement=0;
   
   INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
   
   XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
   ndHAL_ShowMousePointer ();  
}


void INTERNAL_NOPHOENIX_CloseWMI2MenuOpened_Resolutive (char WndHandle)
/* 
   Chiude il menù di livello 2 aperto in modo risolutivo, nel senso che la routine
   chiamante dopo restituirà semplicemente 0 alla routine di partenza e verrà 
   eseguito un nuovo ciclo di gestione del mouse
*/
{
   ndHAL_HideMousePointer ();                        
   WindowData [WndHandle].MenuBarIsChanged=1;
   INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);         // Chiude il vecchio menù
   WindowData [WndHandle].NrL1WMIButtonOpened = 0;
               
   XWindowRender (WndHandle);                         // Disegnerà la nuova configurazione del menù
   ndHAL_ShowMousePointer ();                  
}


void INTERNAL_NOPHOENIX_CloseWMI2MenuOpened_Not_Resolutive (char WndHandle)
/* 
   Chiude il menù di livello 2 aperto in modo non risolutivo, nel senso che la routine
   chiamante dovrà eseguire delle altre operazioni successivamente
*/
{
   WindowData [WndHandle].MenuBarIsChanged=1;       // Per prima cosa disattiviamo 
                                                                // il vecchio menù
   INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
   WindowData [WndHandle].NrL1WMIButtonOpened = 0;  // Cambia configurazione del menù          
}



void INTERNAL_NOPHOENIX_OpenWMI2Menu (char NrWMI1, char WndHandle)
{
   ndHAL_HideMousePointer ();
   WindowData [WndHandle].MenuBarIsChanged=1;
           
   INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 1);
   WindowData [WndHandle].NrL1WMIButtonOpened = NrWMI1;
           
   XWindowRender (WndHandle);                  // Disegnerà la nuova configurazione del menù
   ndHAL_ShowMousePointer ();             
}





void INTERNAL_NOPHOENIX_Execute_WMI1  (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ndEvent_Type *EventAddress, char WndHandle)
{
   ndint64 (*PointerToCallback)(char WndHandle);      // Puntatore alla Callback
     
   // Queste operazioni provocheranno l'uscita da MouseControl.
   // Prima di uscire da MouseControl o di eseguire la Callback il puntatore del mouse 
   // deve essere cancellato senza che poi si provveda al ripristino
   
   ndHAL_HideMousePointer ();   
                               
   EventAddress->__TypeEvent = L1WMI_BUTTON_PRESSED;
   EventAddress->__WindowID  = WndHandle;
   EventAddress->__ButtonID  = 0;
   
   strcpy ( &(EventAddress->__StringID), "" );

   if (strcmp (WindowData [WndHandle].ZString, "")==0)
   {
      strcpy ( &(EventAddress->__ZString), &(MyL1WMI->StringID) );
   }
   else
   {
      strcpy ( &(EventAddress->__ZString), &(WindowData [WndHandle].ZString) );
      strcat ( &(EventAddress->__ZString), "/");
      strcat ( &(EventAddress->__ZString), &(MyL1WMI->StringID) );
   }
   
   if (MyL1WMI->CallbackFunction!=0)
   {
      PointerToCallback = MyL1WMI->CallbackFunction;
      EventAddress->__CallbackExecuted = 1;
      EventAddress->__CallbackAnswer   = (*PointerToCallback)(WndHandle); 
   }
   else
   {
       EventAddress->__CallbackExecuted  = 0;
       EventAddress->__CallbackAnswer    = 0;
   }   
        
}





void INTERNAL_NOPHOENIX_Execute_WMI2  (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, 
                             struct ndEvent_Type *EventAddress, char WndHandle)
{
   ndint64 (*PointerToCallback)(char WndHandle);      // Puntatore alla Callback
     
   EventAddress->__TypeEvent = L2WMI_BUTTON_PRESSED;
   EventAddress->__WindowID  = WndHandle;
   EventAddress->__ButtonID  = 0;
   
   strcpy ( &(EventAddress->__StringID), "" );

   if (strcmp (WindowData [WndHandle].ZString, "")==0)
   {
      strcpy ( &(EventAddress->__ZString), &(MyL1WMI->StringID) );
   }
   else
   {
      strcpy ( &(EventAddress->__ZString), &(WindowData [WndHandle].ZString) );
      strcat ( &(EventAddress->__ZString), "/");
      strcat ( &(EventAddress->__ZString), &(MyL1WMI->StringID) );
   }
   
   strcat ( &(EventAddress->__ZString), "." );
   strcat ( &(EventAddress->__ZString), &(MyL2WMI->StringID) );
   
   if (MyL2WMI->CallbackFunction!=0)
   {
      PointerToCallback = MyL2WMI->CallbackFunction;
      EventAddress->__CallbackExecuted = 1;
      EventAddress->__CallbackAnswer   = (*PointerToCallback)(WndHandle); 
   }
   else
   {
       EventAddress->__CallbackExecuted  = 0;
       EventAddress->__CallbackAnswer    = 0;
   }      
   
   
}







void INTERNAL_NOPHOENIX_WMI1_Pressed (char ButtonFoundID, struct ndEvent_Type *EventAddress, char WndHandle)
/*
     Questa routine viene eseguita quando viene premuto un tasto corrispondente ad un WMI
     di tipo 1.
*/
{
     struct ELEMENT_L1WMI_Type *MyL1WMI;
     char NrWMI1;
     char ErrRep;
     
     NrWMI1 = ButtonFoundID + WindowData [WndHandle].NrFirstL1WMIButton;
     // Determina a quale WMI1 ci si riferisce
     
     ErrRep=ndWMENU_EnumL1WMI (WndHandle, &(WindowData [WndHandle].ZString), NrWMI1, &MyL1WMI);
     // Consente di avere tutte le informazioni necessarie sull'elemento in questione
     
     if (!ErrRep)
     {
        // Per prima cosa, verifichiamo che non ci siano menù aperti
        
        if (WindowData [WndHandle].OpenL2WMIMenu)        // C'era già un menu aperto
        {
           if (WindowData [WndHandle].NrL1WMIButtonOpened == NrWMI1)    // Il menu aperto è quello su cui si è ricliccato
           {
              INTERNAL_NOPHOENIX_CloseWMI2MenuOpened_Resolutive (WndHandle); 
              return; 
           }
           else        // Il menù aperto è diverso da quello in cui si è cliccato
           {
              INTERNAL_NOPHOENIX_CloseWMI2MenuOpened_Not_Resolutive (WndHandle);  
           }
        }
        
        // A questo punto, se c'era un vecchio menù, di sicuro è stato disattivato. Attenzione
        // però perchè è necessario un XWindowRender per la visualizzazione dei cambiamenti
        
        
        if ((MyL1WMI->L2SubMenu)!=0)       // C'è un nuovo submenu di tipo 2 da visualizzare
        {   
           INTERNAL_NOPHOENIX_OpenWMI2Menu (NrWMI1, WndHandle);
        }   
        else
        {
              if ((MyL1WMI->L1SubMenu)!=0)             // C'è un nuovo submenu di tipo 1 da visualizzare
              {
                 INTERNAL_NOPHOENIX_EnterToNew_WMI1 (MyL1WMI, EventAddress, WndHandle); 
              }  
              else    // Non c'è nessun submenu, provvedi ad eseguire un eventuale callback WMI1    
              {
                 INTERNAL_NOPHOENIX_Execute_WMI1 (MyL1WMI, EventAddress, WndHandle); 
              }
                 
        } 
     }
}


void INTERNAL_NOPHOENIX_WMI2_Pressed (char ButtonFoundID, struct ndEvent_Type *EventAddress, char WndHandle)
/*
     Questa routine viene eseguita quando viene premuto un tasto corrispondente ad un WMI
     di tipo 2.
*/

{
     struct ELEMENT_L1WMI_Type *MyL1WMI;
     struct ELEMENT_L2WMI_Type *MyL2WMI;
     
     char NrL1WMI, NrL2WMI;
     char ErrRep;
     
     char TempBuffer [512];
     char SBuffer [512];
     
     unsigned short int SSPosX1, SSPosY1, SSPosX2, SSPosY2;
     unsigned short int rX1, rY1;
   
     unsigned short int LenButton;
     unsigned short int NrCharsInButton;
     unsigned short int Margin;
        
     NrL1WMI = WindowData [WndHandle].NrL1WMIButtonOpened;
     // Determina a quale WMI1 ci si riferisce
     
     ErrRep = ndWMENU_EnumL1WMI (WndHandle, &(WindowData [WndHandle].ZString), NrL1WMI, &MyL1WMI);
     // Consente di avere tutte le informazioni necessarie sull'elemento in questione
     
     if (!ErrRep)
     {
         if (strcmp ( &(WindowData [WndHandle].ZString), "" ) == 0)
         {
            strcpy (TempBuffer, MyL1WMI->StringID);
         }       
         else
         {
            strcpy (TempBuffer, &(WindowData [WndHandle].ZString) );
            strcat (TempBuffer, "/");
            strcat (TempBuffer, MyL1WMI->StringID); 
         }
         
         if (!WindowData[WndHandle].L2WMIMenuScrollActive)
            NrL2WMI = ButtonFoundID;
         else
            NrL2WMI = ButtonFoundID + WindowData[WndHandle].L2WMIScrollElement;
         
         // Ottieni le informazioni necessarie sull'elemento selezionato
     
         ErrRep = ndWMENU_EnumL2WMI (WndHandle, TempBuffer, NrL2WMI, &MyL2WMI);
         
         if (!ErrRep)
         {
             // Quello che segue è un semplice effetto grafico: il tasto che è stato
             // premuto deve venire evidenziato per un attimo prima che il sistema
             // dei menù venga chiuso.
             
             SSPosX1 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosX1;
             SSPosY1 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosY1;
             SSPosX2 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosX2;
             SSPosY2 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosY2;
             
             LenButton       = SSPosX2 - SSPosX1 - 4;
            
             NrCharsInButton = LenButton / 8;
             Margin          = LenButton - (NrCharsInButton * 8) ;
        
             rX1 = WindowData [WndHandle].ButtonWMI2 [ButtonFoundID].SSPosX1 + 2 + Margin/2;
             rY1 = SSPosY1 + 4;
        
             ndStrCpy (SBuffer, MyL2WMI->NameItem, NrCharsInButton, 0);
        
             ndHAL_HideMousePointer ();
             // Queste operazioni provocheranno l'uscita da MouseControl.
             // Prima di uscire da MouseControl il puntatore del mouse deve essere cancellato
                 
             ndSS_DrawRectangle    (SSPosX1, SSPosY1, SSPosX2, SSPosY2, COLOR_BLUE, COLOR_BLUE, WndHandle, NORENDER);
             ndSS_WriteStringInSS  (rX1,     rY1,     SBuffer, COLOR_WHITE, COLOR_BLUE, WndHandle, NORENDER);
             
             XWindowRender (WndHandle);   // Mostra il pulsante evidenziato
             
             INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (WndHandle, 0);
             WindowData [WndHandle].MenuBarIsChanged = 1;
             
             XWindowRender (WndHandle);   // Mostra che il menù L2WMI è scomparso
             
             // Prepariamoci a fornire un messaggio alla routine chiamante
             
             INTERNAL_NOPHOENIX_Execute_WMI2 (MyL1WMI, MyL2WMI, EventAddress, WndHandle);
         }    
     }
}




char INTERNAL_NOPHOENIX_WMIEventManager (unsigned short int MousePointer_RPosX, unsigned short int MousePointer_RPosY, char WindowFoundID, 
                                          struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState)
{
   unsigned short int MBPosX1, MBPosY1;
   unsigned short int SSPosX1, SSPosY1;
   char ButtonFoundID;
   char ButtonFound;
   char CounterA;
   
   // Controlliamo innanzitutto se è stato premuto un tasto WMI1Serv
   
   MBPosX1 = MousePointer_RPosX - WindowData [WindowFoundID].MenuBar_PosX1;
   MBPosY1 = MousePointer_RPosY - WindowData [WindowFoundID].MenuBar_PosY1;
   
   ButtonFound=0;
     
   for (CounterA=0; CounterA<3; CounterA++)
   {          
     if ( (WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].ButtonIsActivated) &&
          (PixelInRect (MBPosX1, MBPosY1, WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosX1,
                        WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosY1,
                        WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosX2,
                        WindowData [WindowFoundID].ButtonWMI1Serv [CounterA].MBPosY2) ) )       
     {
          ButtonFound=1;
          ButtonFoundID=CounterA;
          
          break;
     }
   }
   
   if (ButtonFound)              // E' stato premuto un pulsante WMI1Serv
   {
      switch (ButtonFoundID)
      {
             case WMI_GO_BACK:
                  {
                     INTERNAL_NOPHOENIX_GoToPreviousWMIMenu (WindowFoundID);
                     return 1;    // Informa EventManager che è necessario iniziare un nuovo ciclo mouse
                  }
             case SCROLL_WMI1_TO_RIGHT:
                  {
                     INTERNAL_NOPHOENIX_Scroll_WMI1_To_Right (WindowFoundID);
                     return 1;    // Informa EventManager che è necessario iniziare un nuovo ciclo mouse  
                  }
             case SCROLL_WMI1_TO_LEFT:
                  {
                     INTERNAL_NOPHOENIX_Scroll_WMI1_To_Left (WindowFoundID);
                     return 1;    // Informa EventManager che è necessario iniziare un nuovo ciclo mouse
                  }
      }   
   }
   
   // Ora vediamo se è stato premuto un tasto WMI1
   
   ButtonFound=0;
     
   for (CounterA=0; CounterA<WindowData [WindowFoundID].NrWMI1ButtonsAllocated; CounterA++)
   {          
     if ( (WindowData [WindowFoundID].ButtonWMI1 [CounterA].ButtonIsActivated) &&
          (PixelInRect (MBPosX1, MBPosY1, WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosX1,
                        WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosY1,
                        WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosX2,
                        WindowData [WindowFoundID].ButtonWMI1 [CounterA].MBPosY2) ) )       
     {
          ButtonFound=1;
          ButtonFoundID=CounterA;
          
          break;
     }
   }
   
   if (ButtonFound)  // E' stato trovato un tasto WMI1
   {
       INTERNAL_NOPHOENIX_WMI1_Pressed (ButtonFoundID, EventAddress, WindowFoundID);
       ndHAL_RequestDelay (ND_REQDELAY_WMI1PRESSED);
       
       return 1;             // Informa EventManager che è necessario iniziare un nuovo ciclo mouse  
   }
   
   // A questo punto controlliamo se sono stati premuti tasti WMI2Serv
   
   SSPosX1 = MousePointer_RPosX - WindowData [WindowFoundID].AA3_X1;
   SSPosY1 = MousePointer_RPosY - WindowData [WindowFoundID].AA3_Y1;
   
   ButtonFound=0;
   for (CounterA=0; CounterA<2; CounterA++)
   {          
     if ( (WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].ButtonIsActivated) &&
          (PixelInRect (SSPosX1, SSPosY1, WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosX1,
                        WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosY1,
                        WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosX2,
                        WindowData [WindowFoundID].ButtonWMI2Serv [CounterA].SSPosY2) ) )       
     {
          ButtonFound=1;
          ButtonFoundID=CounterA;
          
          break;
     }
   }
   
   
   if (ButtonFound)              // E' stato premuto un pulsante WMI2Serv
   {
      switch (ButtonFoundID)
      {
             case SCROLL_WMI2_TO_UP:
                  {
                     INTERNAL_NOPHOENIX_Scroll_WMI2_To_Up (WindowFoundID);
                     return 1;     // Informa EventManager che è necessario iniziare un nuovo ciclo mouse             
                  }
             case SCROLL_WMI2_TO_DOWN:
                  {
                     INTERNAL_NOPHOENIX_Scroll_WMI2_To_Down (WindowFoundID);
                     return 1;     // Informa EventManager che è necessario iniziare un nuovo ciclo mouse
                  }
      }   
   }
   
   // Non ci resta che controllare se è stato premuto un tasto WMI2
   
   ButtonFound=0;
   for (CounterA=0; CounterA<WindowData [WindowFoundID].NrWMI2ButtonsAllocated; CounterA++)
   {          
     if ( (WindowData [WindowFoundID].ButtonWMI2 [CounterA].ButtonIsActivated) &&
          (PixelInRect (SSPosX1, SSPosY1, WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosX1,
                        WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosY1,
                        WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosX2,
                        WindowData [WindowFoundID].ButtonWMI2 [CounterA].SSPosY2) ) )       
     {
          ButtonFound=1;
          ButtonFoundID=CounterA;
          
          break;
     }
   }
   
   if (ButtonFound)  // E' stato trovato un tasto WMI2
   {
       INTERNAL_NOPHOENIX_WMI2_Pressed (ButtonFoundID, EventAddress, WindowFoundID);
       ndHAL_RequestDelay (ND_REQDELAY_WMI2PRESSED);
       
       return 1;     // Informa EventManager che è necessario iniziare un nuovo ciclo mouse        
   }
   
     
   return 0; 
   // Informa EventManager che è necessario continuare l'analisi con i pulsanti classe 4                 
}





/* 
    ***************************************************************************
    ***************************************************************************
                            EVENT MANAGER   (No Phoenix)
    ***************************************************************************
    ***************************************************************************
*/


inline void INTERNAL_NOPHOENIX_EventManager_For_Button1 (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState)
/*
    Questa routine è esclusivamente per uso interno. Si occupa di gestire gli eventi
    quando viene premuto il pulsante nr.1 della piattaforma su cui gira 
    Nanodesktop
*/
{
   /* 
      L'algoritmo che si occupa di gestire gli eventi è alquanto complesso. 
      Per prima cosa, è necessario controllare che il mouse, al momento in
      cui viene premuto il pulsante, non fosse su un pulsante di classe 1.
   */
   
   unsigned short int CounterA;
   
   unsigned char ButtonFound, ButtonFoundID;
   unsigned char WindowFoundID;   
   unsigned char WindowInFirstPlaneID;   
   
   unsigned short int WndLenX, WndLenY;
   unsigned short int MousePointer_RPosX, MousePointer_RPosY;
   unsigned short int MousePointer_RRPosX, MousePointer_RRPosY;
   
   int OLDWindowX1, OLDWindowY1, OLDWindowX2, OLDWindowY2;
   int THWindowX1,  THWindowY1,  THWindowX2,  THWindowY2; 
   int NEWWindowX1, NEWWindowY1, NEWWindowX2, NEWWindowY2;
   
   int ChangeX, ChangeY;
   
   unsigned short int OLDROIPosX, OLDROIPosY;
   unsigned short int IsWindowInFirstPlane, PseudoSecondaryWindow, CounterK, CounterM;
   unsigned short int PosWindowInWV, WindowSpecific;   
   unsigned short int NewSizeX, NewSizeY;
      
   int SuggestedX, SuggestedY;
   
   ndint64 (*PointerToCallback)(char *StringID, ndint64 InfoField, char WndHandle);
      
   ButtonFound=0;
   
   for (CounterA=0; CounterA<__NRWINDOWSALLOCABLE; CounterA++)
   {
       if (ButtonClass1_Data [CounterA].ButtonIsActivated)
       {
           if (  PixelInRect (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, 
                             ButtonClass1_Data [CounterA].x1, ButtonClass1_Data [CounterA].y1,
                             ButtonClass1_Data [CounterA].x2, ButtonClass1_Data [CounterA].y2)  )
           {
                 ButtonFound=1;
                 ButtonFoundID=CounterA;
                 
                 break;
           }
       }
   }
   
   if (ButtonFound)                     // E' stato premuto un pulsante di classe 1
   {
      ndHAL_HideMousePointer ();
      ndLP_PutElementAtTheTopOfWV (ButtonClass1_Data [ButtonFoundID].ButtonID);
      ndLP_MaximizeWindow (ButtonClass1_Data [ButtonFoundID].ButtonID);
      
      ndHAL_RequestDelay (ND_REQDELAY_MOUSECTRL);     // Sotto PSP è necessario un piccolo ritardo
                                                      // per evitare la massimizzazione multipla
      
      ndHAL_ShowMousePointer ();
      return;
   } 

   /* 
      Se il processo precedente fallisce, allora è necessario verificare se è stato
      premuto un pulsante di classe 2. 
   */

   ButtonFound=0;
   
   for (CounterA=0; CounterA<4; CounterA++)
   {
       if (ButtonClass2_Data [CounterA].ButtonIsActivated)
       {                   
           if (  PixelInRect (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, 
                             ButtonClass2_Data [CounterA].x1, ButtonClass2_Data [CounterA].y1,
                             ButtonClass2_Data [CounterA].x2, ButtonClass2_Data [CounterA].y2)    )                         
           {      
                   ButtonFound=1;
                   ButtonFoundID=CounterA;
                   
                   break;
           }
       }
   }
   
   if (ButtonFound)
   {
        WindowInFirstPlaneID = WindowsVector [0];
        
        if (!WindowData [WindowInFirstPlaneID].NoResizeable)
        {               
            switch (ButtonClass2_Data [ButtonFoundID].ButtonID)
            {
                   case ENLARGE_X:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX+20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [WindowInFirstPlaneID].WSWndLength, 
                                          WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedX)))
                                NewSizeX=SuggestedX;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            ndHAL_ShowMousePointer ();
                            
                            break;
                   }
                   
                   case SMALLER_X:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX-20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [WindowInFirstPlaneID].WSWndLength,
                                      WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedX)))
                             NewSizeX=SuggestedX;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            ndHAL_ShowMousePointer ();
                            
                            break;      
                   }
                   
                   case ENLARGE_Y:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY+20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [WindowInFirstPlaneID].WSWndHeight, 
                                     WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedY)))
                             NewSizeY=SuggestedY;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            ndHAL_ShowMousePointer ();
                            
                            break;
                   }
                   
                   case SMALLER_Y:
                   {
                            WndLenX = WindowData [WindowInFirstPlaneID].WndLength;
                            WndLenY = WindowData [WindowInFirstPlaneID].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY-20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [WindowInFirstPlaneID].WSWndHeight, 
                                     WindowData [WindowInFirstPlaneID].MenuSupported, &SuggestedY)))
                             NewSizeY=SuggestedY;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, WindowInFirstPlaneID);
                            ndHAL_ShowMousePointer ();
                            
                            break;    
                   }
          
            }
       
            return;
            
        }   // End of "if (!WindowData [WindowInFirstPlaneID].NoResizeable)"
        
        
   }
   
   /*
   A questo punto si passa a verificare se il puntatore del mouse ha toccato qualche
   finestra che non è in primo piano. Infatti, se fosse così, il sistema provvederà
   a portare la finestra che è stata toccata in primo piano.
   */
   
   INTERNAL_FindOwnerOfPixel (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, &WindowFoundID);
   
   if (WindowFoundID!=255)                    // Il pixel appartiene ad una finestra
   {
      
      WindowInFirstPlaneID = WindowsVector [0];
      IsWindowInFirstPlane = 0;
     
      if (WindowInFirstPlaneID == WindowFoundID)
         IsWindowInFirstPlane=1;
      else
         {
           // Se la finestra non è in primo piano è necessario verificare se siamo in presenza di una finestra pseudo-secondaria.
           // Le finestre pseudo-secondarie, sono quelle finestre che non sono in primo piano ma,
           // poichè non sono coperte da alcuna finestra di livello superiore, appaiono al
           // sistema come finestre in primo piano. 
     
           PseudoSecondaryWindow=1;          // Partiamo con un'ipotesi ottimistica. Essa dovrà essere confermata da tutti 
                                             // i cicli di analisi successiva
           
           for (CounterK=0; CounterK<__NRWINDOWSALLOCABLE; CounterK++)
           {
               if (WindowsVector [CounterK] == WindowFoundID) PosWindowInWV=CounterK;
           }
                                
           for (CounterM=0; CounterM<PosWindowInWV; CounterM++)
           {
              WindowSpecific = WindowsVector [CounterM]; 
               
              if (WindowData [WindowSpecific].WindowIsActive)
              {
                 if (! (INTERNAL_RettangoliDisgiunti (WindowData [WindowSpecific].WndLimitX1, WindowData [WindowSpecific].WndLimitY1,
                                              WindowData [WindowSpecific].WndLimitX2, WindowData [WindowSpecific].WndLimitY2,
                                              WindowData [WindowFoundID].WndLimitX1, WindowData [WindowFoundID].WndLimitY1,
                                              WindowData [WindowFoundID].WndLimitX2, WindowData [WindowFoundID].WndLimitY2) ) )
              
                 {
                     PseudoSecondaryWindow = 0;
                 }
              }                  
           }
         }
              
      if ( (!(IsWindowInFirstPlane)) && (!(PseudoSecondaryWindow)) )   // Il pixel appartiene ad una finestra non in primo piano
      {
         ndHAL_HideMousePointer ();
         ndLP_PutWindowInFirstPlane (WindowFoundID);
         ndHAL_ShowMousePointer ();
         
         return;
      }
      else                            
      {
         /* Se il pixel appartiene ad una finestra in primo piano, o a una finestra pseudo-secondaria, 
            è necessario verificare se è stato toccato un pulsante di classe 3 */
            
         MousePointer_RPosX = HAL_MouseEmu_PosX - WindowData [WindowFoundID].WndLimitX1;
         MousePointer_RPosY = HAL_MouseEmu_PosY - WindowData [WindowFoundID].WndLimitY1;
         
         ButtonFound=0;
         
         for (CounterA=0; CounterA<7; CounterA++)
         {
             if ( (WindowData [WindowFoundID].ButtonClass3_Data [CounterA].ButtonIsActivated) &&
                  (PixelInRect (MousePointer_RPosX, MousePointer_RPosY, 
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].x1,
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].y1,
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].x2,
                               WindowData [WindowFoundID].ButtonClass3_Data [CounterA].y2) ) )       
             {
                  ButtonFound=1;
                  ButtonFoundID=CounterA;
                  
                  break;
             }
         }
         
         if (ButtonFound)                // E' stato trovato un pulsante di classe 3
         {
            switch (WindowData [WindowFoundID].ButtonClass3_Data [ButtonFoundID].ButtonID) 
            {
                case MINIMIZEWINDOW_BUTTON:
                {
                    ndHAL_HideMousePointer ();
                    ndLP_MinimizeWindow (WindowFoundID);
                    ndHAL_ShowMousePointer ();
                    
                    return;
                }
                
                case TITLEBAR_BUTTON:
                {    
                     if ((MouseState->VectorX!=0) || (MouseState->VectorY!=0))
                     {
                             OLDWindowX1 = WindowData [WindowFoundID].WndLimitX1;
                             OLDWindowY1 = WindowData [WindowFoundID].WndLimitY1;
                             OLDWindowX2 = WindowData [WindowFoundID].WndLimitX2;
                             OLDWindowY2 = WindowData [WindowFoundID].WndLimitY2;
                             
                             // Imposta di default le coordinate della nuova finestra
                             // alle vecchie: queste impostazioni saranno cambiate in
                             // un secondo momento dal programma, se necessario
                             
                             NEWWindowX1  = OLDWindowX1;
                             NEWWindowY1  = OLDWindowY1;
                             NEWWindowX2  = OLDWindowX2;
                             NEWWindowY2  = OLDWindowY2;
                             
                             // Inizia l'elaborazione
                             
                             if (MouseState->VectorX<0)            // E' stato richiesto uno spostamento a destra
                             {
                                 THWindowX1 = OLDWindowX1 - abs(MouseState->VectorX);
                                 THWindowX2 = OLDWindowX2 - abs(MouseState->VectorX);
                                 
                                 if (THWindowX1>=0)
                                 {
                                    NEWWindowX1 = THWindowX1;
                                    NEWWindowX2 = THWindowX2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowX1 = 0;
                                    NEWWindowX2 = WindowData [WindowFoundID].WndLength;
                                 }
                             }
                             
                             if (MouseState->VectorX>=0)      // E' stato richiesto uno spostamento a sinistra
                             {
                                 THWindowX1 = OLDWindowX1 + abs(MouseState->VectorX);
                                 THWindowX2 = OLDWindowX2 + abs(MouseState->VectorX);
                                 
                                 if (THWindowX2<Windows_MaxScreenX)
                                 {
                                    NEWWindowX1 = THWindowX1;
                                    NEWWindowX2 = THWindowX2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowX1 = Windows_MaxScreenX - 1 - WindowData [WindowFoundID].WndLength;
                                    NEWWindowX2 = Windows_MaxScreenX - 1;
                                 }
                             }
                             
                             if (MouseState->VectorY<0)            // E' stato richiesto uno spostamento in alto
                             {
                                 THWindowY1 = OLDWindowY1 - abs(MouseState->VectorY);
                                 THWindowY2 = OLDWindowY2 - abs(MouseState->VectorY);
                                 
                                 if (THWindowY1>=0)
                                 {
                                    NEWWindowY1 = THWindowY1;
                                    NEWWindowY2 = THWindowY2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowY1 = 0;
                                    NEWWindowY2 = WindowData [WindowFoundID].WndHeight;
                                 }
                             }
                             
                             if (MouseState->VectorY>=0)      // E' stato richiesto uno spostamento in basso
                             {
                                 THWindowY1 = OLDWindowY1 + abs(MouseState->VectorY);
                                 THWindowY2 = OLDWindowY2 + abs(MouseState->VectorY);
                                 
                                 if (THWindowY2<Windows_MaxScreenY)
                                 {
                                    NEWWindowY1 = THWindowY1;
                                    NEWWindowY2 = THWindowY2;
                                 }
                                 else           // La finestra andrebbe fuori schermo
                                 {
                                    NEWWindowY1 = Windows_MaxScreenY - 1 - WindowData [WindowFoundID].WndHeight;
                                    NEWWindowY2 = Windows_MaxScreenY - 1;
                                 }
                             }
                             
                             ChangeX = NEWWindowX1 - OLDWindowX1;
                             ChangeY = NEWWindowY1 - OLDWindowY1;
                             
                             ndHAL_HideMousePointer ();
                             ndLP_MoveWindow (NEWWindowX1, NEWWindowY1, WindowFoundID);
                             HAL_MouseEmu_PosX = HAL_MouseEmu_PosX + ChangeX;
                             HAL_MouseEmu_PosY = HAL_MouseEmu_PosY + ChangeY;
                             
                             ndHAL_ShowMousePointer ();    
                     }
                                                  
                     return;
                }  

                case SCROLL_LEFT_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndHAL_HideMousePointer ();
                     ndLP_ChangeWindowROI (OLDROIPosX - 20, OLDROIPosY, ROI_SMART, WindowFoundID);
                     ndHAL_ShowMousePointer ();
                     
                     return;
                }
             
                case SCROLL_RIGHT_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndHAL_HideMousePointer ();
                     ndLP_ChangeWindowROI (OLDROIPosX + 20, OLDROIPosY, ROI_SMART, WindowFoundID);
                     ndHAL_ShowMousePointer ();
                     
                     return;
                }
    
                case SCROLL_UP_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndHAL_HideMousePointer ();
                     ndLP_ChangeWindowROI (OLDROIPosX, OLDROIPosY - 20, ROI_SMART, WindowFoundID);
                     ndHAL_ShowMousePointer ();
                     
                     return;
                }
             
                case SCROLL_DOWN_BUTTON:
                {
                     OLDROIPosX = WindowData [WindowFoundID].ROI_PosX;
                     OLDROIPosY = WindowData [WindowFoundID].ROI_PosY;
                     
                     ndHAL_HideMousePointer ();
                     ndLP_ChangeWindowROI (OLDROIPosX, OLDROIPosY + 20, ROI_SMART, WindowFoundID);
                     ndHAL_ShowMousePointer ();
                     
                     return;
                }

			    case CLOSEWINDOW_BUTTON:
                {
                     // Puntatore alla CloseWindow Callback
                     ndint64 (*PntToCloseWndCallback)(char WndHandle, ndint64 WndInfoField);
                     ndint64  WndInfoField;
                           
                     // Setup della callback
                     PntToCloseWndCallback = WindowData [WindowFoundID].PntToCloseWndCallback;
                     WndInfoField          = WindowData [WindowFoundID].WndInfoField;
                           
                     
                     if (PntToCloseWndCallback==0)
                     {
                           // Non è stata specificata una CloseWndCallback: segnaliamo a 
                           // MouseControl che una finestra ha chiesto di essere chiusa  
                     
                           ndHAL_HideMousePointer ();
                           EventAddress->__TypeEvent=A_WINDOW_WANTS_TO_BE_CLOSED;
                           EventAddress->__WindowID=WindowFoundID;
                           EventAddress->__ButtonID=0;
                           return;
                     }
                     else  // E' stata specificata una CloseWndCallback
                     {
                           if (!WindowData [WindowFoundID].NoNotifyToMouseControl)
                           {
                                  ndHAL_HideMousePointer ();
                                  
                                  EventAddress->__TypeEvent = CLOSEWNDCALLBACK_HAS_BEEN_EXEC;
                                  EventAddress->__WindowID  = WindowFoundID;
                                  EventAddress->__ButtonID  = -1;
                                  
                                  strcpy (EventAddress->__StringID, "");
                                  
                                  EventAddress->__CallbackExecuted = 1;
                                  EventAddress->__CallbackAnswer = (*PntToCloseWndCallback)(WindowFoundID, WndInfoField);
                                  
                                  strcpy (EventAddress->__ZString, "");  
                                  return;
                           }
                           else
                           {
                                  // Esegui la callback di chiusura finestra senza uscire da
                                  // MouseControl 
                                  
                                  ndHAL_HideMousePointer ();
                                     (*PntToCloseWndCallback)(WindowFoundID, WndInfoField);
                                  ndHAL_ShowMousePointer ();
                                  
                                  return;
                           }      
                     }  
                }
                
            }              // Chiude lo switch
          }                // Chiude if ButtonFound
      
          // Se il pixel non appartiene ad alcun pulsante di classe 3 si può verificare se
          // esso appartiene ad un pulsante virtuale WMI, ossia ad un pulsante dei menù.
          // A tale scopo, si richiama la routine specializzata WMIMouseControl.
          
          
          
          if (WindowData [WindowFoundID].MenuSupported)
          { 
             if ( (INTERNAL_NOPHOENIX_WMIEventManager (MousePointer_RPosX, MousePointer_RPosY, WindowFoundID, EventAddress, MouseState))==1) return;
             // Se la routine restituisce 1, allora ci sono due possibilità:
             //    a) l'utente ha premuto un pulsante di tipo WMIServ1, WMIServ2, WMI1, WMI2 che
             //       ha effettuato l'esecuzione delle callback. In questo caso si deve uscire
             //       da EventManager, ma la routine MouseControl rileverà che la struttura di
             //       tipo ndEvent_Type indirizzata da EventAddress ha ancora il campo TypeEvent
             //       posto a zero, e quindi ricomincerà un altro ciclo del loop di scelta del
             //       pixel (il mouse continuerà a muoversi).
             //    b) l'utente ha premuto un pulsante di tipo WMI1 o WMI2 e questo ha provocato
             //       l'esecuzione di una callback, o comunque l'esecuzione di un messaggio. 
             //       Anche in questo caso si deve uscire da EventManager, solo che anche
             //       MouseControl uscirà perchè ora il campo TypeEvent della struttura di tipo
             //       ndEvent_Type indirizzata da EventAddress ha valore non nullo.
             //
             //    Se la routine restituisce 0, significa semplicemente che non è stato trovato
             //    nessun pulsante WMI1, WMI2, WMI1Serv o WMI2Serv e quindi bisogna proseguire
             //    la fase di ricerca con i pulsanti di classe 4 (ossia i pulsanti custom
             //    definiti dall'utente. 
          }
          
          
          
          MousePointer_RRPosX = MousePointer_RPosX - WindowData [WindowFoundID].AA3_X1 + WindowData [WindowFoundID].ROI_PosX;
          MousePointer_RRPosY = MousePointer_RPosY - WindowData [WindowFoundID].AA3_Y1 + WindowData [WindowFoundID].ROI_PosY;
          // Adesso in MousePointer_RRPosX e MousePointer_RRPosY c'è la posizione nel windows space della finestra che
          // corrisponde al punto dove si trova il puntatore del mouse.
          
          ButtonFound=0;
          
          for (CounterA=0; CounterA<__NRBUTTONSALLOCABLE; CounterA++)
          {
             if ( WindowData [WindowFoundID].ButtonClass4_Data [CounterA].ButtonIsActivated )
             {
                  if (!WindowData [WindowFoundID].ButtonClass4_Data [CounterA].ButtonIsInhibited)
                  {
                      if (  PixelInRect (MousePointer_RRPosX, MousePointer_RRPosY, 
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosX1,
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosY1,
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosX2,
                                         WindowData [WindowFoundID].ButtonClass4_Data [CounterA].RRPosY2)  )
                      {
                           ButtonFound=1;
                           ButtonFoundID=CounterA;
                           
                           break;
                      }
                  }
             }
          }
          
          if (ButtonFound)
          {
             ndHAL_HideMousePointer ();
             
             // Verifichiamo se è necessario eseguire una callback
             
             if (WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].Callback!=0)
             {
                PointerToCallback = WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].Callback;
                
                EventAddress->__CallbackAnswer=(*PointerToCallback)(&(WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].StringID), 
                                                                      WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].InfoField, 
                                                                      WindowFoundID);
                                                                   
                EventAddress->__CallbackExecuted=1;
             }
             else   // Non è stato necessario eseguire una callback
             {
                EventAddress->__CallbackAnswer=0; 
                EventAddress->__CallbackExecuted=0;
             }
             
             // Se non è stata attivata la modalità trasparent callback dispatch di un messaggio
             if (!((WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].ButtonStyle & ND_TRASPARENT_CALLBACK) == ND_TRASPARENT_CALLBACK))
             {
                EventAddress->__TypeEvent = CUSTOM_BUTTON_PRESSED;
                EventAddress->__WindowID = WindowFoundID;
                EventAddress->__ButtonID = ButtonFoundID;
                strcpy ( &(EventAddress->__StringID), &(WindowData [WindowFoundID].ButtonClass4_Data [ButtonFoundID].StringID) );
                return;
             
                // Segnaliamo a mouse control che è stato cliccato un pulsante definito dall'utente
             }
             else   // Trasparent Callback attivata
             {
                 ndHAL_ShowMousePointer();
                 return;
             } 
            
             
          }
          
          // Se siamo arrivati a questo punto del controllo, vuol dire che il pixel appartiene ad
          // una finestra pseudo-secondaria o in primo piano, ma non appartiene a nessun pulsante.
          // Per le finestre pseudo-secondarie, si esegue un ulteriore operazione: la finestra viene
          // portata al top del window vector (sullo schermo non si vedrà nessun cambiamento, ma
          // sarà possibile per l'utente selezionare le opzioni di ingrandimento finestra).
          
          if (PseudoSecondaryWindow)
          {
             ndHAL_HideMousePointer ();
             ndLP_PutWindowInFirstPlane (WindowFoundID);
             ndHAL_ShowMousePointer ();
      
             return;                         
          }
                    
                
      }                    // Chiude l'else per la finestra non in primo piano
   }                       // Chiude if foundID !=255
      
}


inline void INTERNAL_NOPHOENIX_EventManager_For_Button4 (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState)
/*
    Questa routine è esclusivamente per uso interno. Si occupa di gestire gli eventi
    quando viene premuto il pulsante nr.4 della piattaforma su cui gira 
    Nanodesktop
*/

{
   char WindowFoundID; 
   char Command; 
   
   unsigned short int NewSizeX, NewSizeY;
   unsigned short int WndLenX, WndLenY;
   
   int SuggestedX, SuggestedY;
   
    
   // Il pulsante nr. 4 della piattaforma viene usato normalmente per eseguire operazioni
   // di resize della finestra. Tutto deve avvenire in modo trasparente alla routine
   // chiamante, quindi la struct EventAddress deve essere posta a NO_EVENT.
   
   EventAddress->__TypeEvent=NO_EVENT;
   EventAddress->__WindowID=0;
   EventAddress->__ButtonID=0;
   EventAddress->__CallbackExecuted=0;
   EventAddress->__CallbackAnswer=0;
   
   strcpy ( &(EventAddress->__StringID), "");
   strcpy ( &(EventAddress->__ZString),  "");
   
   if (!ResizerFocusActive)               // Il resizer non ha agganciato alcuna finestra
   {
       INTERNAL_FindOwnerOfPixel (MousePointer_PosX, MousePointer_PosY, &WindowFoundID);
   
       if (WindowFoundID!=255)                    // Il pixel appartiene ad una finestra
       {
           if (!WindowData [WindowFoundID].NoResizeable)
           {
              ResizerFocusActive=1;
              ResizerFocusWindow=WindowFoundID;
           }
       }
   }
   
   if (ResizerFocusActive)                        // Qui il sistema ha effettivamente agganciato
   {                                              // una finestra
       
       // Ora cerca di determinare il comando di resize
       
       Command=0;
       
       if (MouseState->VectorX==-1) Command=SMALLER_X;
       else
       {
           if (MouseState->VectorX==+1) Command=ENLARGE_X;
           else
           {
               if (MouseState->VectorY==-1) Command=SMALLER_Y;
               else
               {
                   if (MouseState->VectorY==+1) Command=ENLARGE_Y;
               }
           }
       }
       
       
       if (Command!=0)     // E' stato trovato un codice di comando corretto
       {
          
            switch (Command)
            {
                   case ENLARGE_X:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX+20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [ResizerFocusWindow].WSWndLength, 
                                          WindowData [ResizerFocusWindow].MenuSupported, &SuggestedX)))
                                NewSizeX=SuggestedX;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            ndHAL_ShowMousePointer ();
              
                            break;
                   }
                   
                   case SMALLER_X:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX-20;
                            NewSizeY=WndLenY;
                            
                            if (!(ndLP_ValidateNewSizeX (NewSizeX, WindowData [ResizerFocusWindow].WSWndLength,
                                      WindowData [ResizerFocusWindow].MenuSupported, &SuggestedX)))
                             NewSizeX=SuggestedX;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            ndHAL_ShowMousePointer ();
              
                            break;      
                   }
                   
                   case ENLARGE_Y:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY+20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [ResizerFocusWindow].WSWndHeight, 
                                     WindowData [ResizerFocusWindow].MenuSupported, &SuggestedY)))
                             NewSizeY=SuggestedY;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            ndHAL_ShowMousePointer ();
              
                            break;
                   }
                   
                   case SMALLER_Y:
                   {
                            WndLenX = WindowData [ResizerFocusWindow].WndLength;
                            WndLenY = WindowData [ResizerFocusWindow].WndHeight;
                            
                            NewSizeX=WndLenX;
                            NewSizeY=WndLenY-20;
                            
                            if (!(ndLP_ValidateNewSizeY (NewSizeY, WindowData [ResizerFocusWindow].WSWndHeight, 
                                     WindowData [ResizerFocusWindow].MenuSupported, &SuggestedY)))
                             NewSizeY=SuggestedY;
                            
                            ndHAL_HideMousePointer ();
                            ndLP_ResizeWindow (NewSizeX, NewSizeY, ResizerFocusWindow);
                            ndHAL_ShowMousePointer ();
              
                            break;    
                   }
          
            }  // End of switch
                        
       }  // End of command!=0
   
       return;
       
       
   }  // End of "if ResizerFocusActive"
   
}


inline void INTERNAL_NOPHOENIX_EventManager_For_Button5 (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState)
/*
    Questa routine è esclusivamente per uso interno. Si occupa di gestire gli eventi
    quando viene premuto il pulsante nr.5 della piattaforma su cui gira 
    Nanodesktop
*/
{
   EventAddress->__TypeEvent=SPECIAL_KEY_PRESSED;
   EventAddress->__WindowID=0;
   EventAddress->__ButtonID=ND_BUTTON5;
   EventAddress->__CallbackExecuted=0;
   EventAddress->__CallbackAnswer=0;
   
   return;
}


inline void INTERNAL_NOPHOENIX_NoEventManager (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState)
/*
    Esegue delle operazioni di reset qualora non vi siano eventi
*/
{
    ResizerFocusActive=0;
    // Pone a 0 la variabile ResizerFocusActive. Questo farà sì che l'eventuale finestra
    // agganciata dal resizer venga rilasciata.
}




/* 
    ***************************************************************************
    ***************************************************************************
                            MOUSE CONTROL   (No Phoenix)
    ***************************************************************************
    ***************************************************************************
*/

void INTERNAL_NOPHOENIX_MouseControl (struct ndEvent_Type *EventAddress)
/*
    Questa routine gestisce MouseControl quando è stato disabilitato il
    Phoenix SubSystem. 
    
    Il thread chiamante viene sospeso fin quando non viene recapitato un
    messaggio o non avviene un evento che non può essere gestito 
    internamente da MouseControl. 
*/

{
   static struct ndMouseState_Type MyMouseState;
   char Button1Pressed, Button4Pressed, Button5Pressed;
   
   if ( !ndInitSystemExecuted ) ndInitSystem();
   
   ndHAL_ShowMousePointer ();
   
   EventAddress->__TypeEvent = 0;
   
   while ( !(EventAddress->__TypeEvent) )
   {
       ndHAL_GetMouseState (&MyMouseState);
       
       Button1Pressed = ((MyMouseState.Buttons & ND_BUTTON1)==ND_BUTTON1);
       Button4Pressed = ((MyMouseState.Buttons & ND_BUTTON4)==ND_BUTTON4);
       Button5Pressed = ((MyMouseState.Buttons & ND_BUTTON5)==ND_BUTTON5);
   
       if (!HAL_IsTheFirstTimeForMouse)      // Non è la prima volta per il mouse
       {       
               if ( (HAL_MouseEmu_PosX != HAL_MouseEmu_OldPosX) || (HAL_MouseEmu_PosY != HAL_MouseEmu_OldPosY) )
               {
                     ndHAL_GrHideMousePointer (HAL_MouseEmu_OldPosX, HAL_MouseEmu_OldPosY, ActiveFBPage);    
                     // Nascondi il vecchio puntatore del mouse    
               
                     ndHAL_GrRegisterMouseBackGr (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
                     // Registra il background in quella che sarà la nuova posizione del mouse
             
                     HAL_MouseEmu_OldPosX = HAL_MouseEmu_PosX;
                     HAL_MouseEmu_OldPosY = HAL_MouseEmu_PosY;
                     // La nuova posizione diventa la vecchia, così al prossimo spostamento sarà 
                     // possibile cancellare il puntatore dalla vecchia posizione
    
                     ndHAL_GrDrawMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);   
                     // Disegna il puntatore nella posizione attuale
               }
       }
       
       
       // Se viene premuto un tasto, il sistema gestisce la consegna del messaggio: la tecnica è 
       // quella della chiamata a catena
       
       
       if (Button1Pressed)
       {
           INTERNAL_NOPHOENIX_EventManager_For_Button1 (EventAddress, &MyMouseState);
       }
       else
       {
           if (Button4Pressed)
           {
              INTERNAL_NOPHOENIX_EventManager_For_Button4 (EventAddress, &MyMouseState); 
           }
           else
           {
               if (Button5Pressed)
               {
                  INTERNAL_NOPHOENIX_EventManager_For_Button5 (EventAddress, &MyMouseState);               
               }
               else     // Nessuno dei tasti è stato premuto
               {
                  INTERNAL_NOPHOENIX_NoEventManager (EventAddress, &MyMouseState);      
               }
                  
           }
       }
   
   }   // End of while loop
   
    
}


