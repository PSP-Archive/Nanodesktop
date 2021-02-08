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
    ROUTINES PER L'EMULAZIONE DEL MENU (PSPE)
    ----------------------------------------------------------------------------
    Queste routine si occupano di gestire l'emulazione del mouse
    ----------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

char ndHAL_GetMouseState (struct ndMouseState_Type *MouseState)
/* -----------------------------------------------------------------------------
   Questa routine si occupa di ricavare lo stato del mouse.
   
   Per capire bene come funziona questo tipo di routine, è necessario prima
   analizzare i campi della struttura struct ndMouseState_Type. 
   
   La struttura possiede i seguenti campi:
   
   MousePointer_PosX       Due campi che contengono la posizione del mouse.
   MousePointer_PosY       In realtà, all'interno della libreria è possibile
                           bypassare la routine accedendo direttamente alle
                           variabili MousePointer_PosX e MousePointer_PosY.
                           
   Buttons                 Un campo a 16 bit che contiene le informazioni sui
                           tasti che sono stati premuti. Attualmente ha 
                           significato solo il bit meno significativo, che
                           corrisponde alla pressione del tasto sinistro
                           
   VectorX, VectorY        GetMouseState eseguirà una verifica delle mouse
                           gestoure prima di ritornare il controllo alla
                           routine chiamante: VectorX e VectorY rappresentano
                           uno spostamento eseguito per mezzo del mouse.
                           
   Poichè la PSP non possiede un mouse, viene usato il pad e si emula uno
   spostamento VectorX=+-5 VectorY=+-5 con una tecnica particolare.
   
   E' previsto il codice di errore:
     ERR_MOUSE_NOT_CONNECTED
   -----------------------------------------------------------------------------
*/

{
           ndHAL_MouseEmu();       
           // Chiamare questa routine consente un cambiamento dei registri
           // HAL_MouseEmu_PosXY nel medesimo contesto della routine
           // chiamante
           
           MouseState->MousePointer_PosX    = HAL_MouseEmu_PosX;
           MouseState->MousePointer_PosY    = HAL_MouseEmu_PosY;
           MouseState->Buttons = HAL_MouseButtons;
           MouseState->VectorX = HAL_MouseVectorX;
           MouseState->VectorY = HAL_MouseVectorY;
           
           return 0;
}

/*
    ---------------------------------------------------------------------------
    Funzioni per il disegno e per la cancellazione del puntatore del mouse:
    queste routine agiscono esclusivamente a livello grafico
    ---------------------------------------------------------------------------
*/


void ndHAL_GrRegisterMouseBackGr (unsigned short int PosX, unsigned short int PosY, unsigned char Page)
/*
   Questa funzione provvede a registrare il vecchio background che si trova dietro 
   al mouse
*/
{
   register int CounterX, CounterY;
   register int LenX, LenY;
   
   if (PosX+16<MaxScreenX)
      LenX=16;
   else
      LenX=MaxScreenX-PosX;
        
   if (PosY+16<MaxScreenY)
      LenY=16;
   else
      LenY=MaxScreenY-PosY;
       
   // Registra il contenuto dello schermo precedente la registrazione del puntatore
             
   for (CounterY=0; CounterY<LenY; CounterY++)
   {
         for (CounterX=0; CounterX<LenX; CounterX++)
         {
             BackgroundMouse [CounterY][CounterX]=ndHAL_GetPixelFromPage (PosX+CounterX, PosY+CounterY, Page);       
         }
   }   
   
   BackgroundMouseFrame = 0;
   // Sotto    
}


void ndHAL_GrDrawMousePointer (unsigned short int PosX, unsigned short int PosY, unsigned char Page)
/*
   Questa funzione provvede a disegnare il puntatore del mouse in una determinata 
   posizione che viene passata alla funzione esplicitamente
*/

{
     register int CounterX, CounterY;
     register int LenX, LenY;
     
     if (PosX+16<MaxScreenX)
        LenX=16;
     else
        LenX=MaxScreenX-PosX;
        
     if (PosY+16<MaxScreenY)
        LenY=16;
     else
        LenY=MaxScreenY-PosY;
            
     for (CounterY=0; CounterY<LenY; CounterY++)
     {
         for (CounterX=0; CounterX<LenX; CounterX++)
         {
             if (MouseTheme [CounterY][CounterX] == 'B') ndHAL_PutPixelToPage (PosX+CounterX, PosY+CounterY, 0, Page);
             if (MouseTheme [CounterY][CounterX] == 'W') ndHAL_PutPixelToPage (PosX+CounterX, PosY+CounterY, 0xFFFF, Page);
         }
     }
     
     HAL_IsTheFirstTimeForMouse=0;
}


void ndHAL_GrHideMousePointer (unsigned short int PosX, unsigned short int PosY, unsigned char Page)
{
     register int CounterX, CounterY;
     register int LenX, LenY;
     
     if (HAL_MouseEmuSystemEnabled)
     {
        if (!HAL_IsTheFirstTimeForMouse)
        {
           if (PosX+16<MaxScreenX)
               LenX=16;
           else
               LenX=MaxScreenX-PosX;
        
           if (PosY+16<MaxScreenY)
               LenY=16;
           else
               LenY=MaxScreenY-PosY;
          
           for (CounterY=0; CounterY<LenY; CounterY++)
               for (CounterX=0; CounterX<LenX; CounterX++)
                   ndHAL_PutPixelToPage (PosX + CounterX, PosY + CounterY, BackgroundMouse [CounterY][CounterX], Page);            
        }
     }        
}

/*
   -----------------------------------------------------------------------------
   Routines per la gestione del mouse: wrapper per la sezione CODE
   -----------------------------------------------------------------------------
*/

inline char ndHAL_MoveMousePointerToPos (unsigned short int PosX, unsigned short int PosY)
/*
   Questa funzione provvede a spostare il puntatore del mouse in una determinata 
   posizione che viene passata alla funzione esplicitamente. 
   
   ****** La presente routine è il wrapper per la sezione CODE
   
   Sono previsti i seguenti codici di errore:
        ERR_WRONG_PARAMETERS       Le coordinate del puntatore sono errate
*/ 
{
            if ( (PosX<Mouse_MaxScreenX) && (PosY<Mouse_MaxScreenY) )
            {
                 ndHAL_GrHideMousePointer (HAL_MouseEmu_OldPosX, HAL_MouseEmu_OldPosY, ActiveFBPage);
                 // Cancella il puntatore del mouse
            
                 HAL_MouseEmu_OldPosX = HAL_MouseEmu_PosX;       
                 HAL_MouseEmu_OldPosY = HAL_MouseEmu_PosY;
                 HAL_MouseEmu_PosX    = PosX;
                 HAL_MouseEmu_PosY    = PosY;
                 
                 ndHAL_GrRegisterMouseBackGr (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
                 // Registra il background del mouse
                 
                 ndHAL_GrDrawMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
                 // Disegna il puntatore del mouse nella nuova posizione
                 
                 return 0;
            }
            else
                return ERR_WRONG_PARAMETERS;
    
}


inline void ndHAL_HideMousePointer ()
/* 
   Cancella il puntatore del mouse.
   
   ****** La presente routine è il wrapper per la sezione CODE
   
   Non sono previsti codici di errore per questa routine.
*/
{
   
            ndHAL_GrHideMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
            HAL_IsTheFirstTimeForMouse = 1;
              
}


inline void ndHAL_ShowMousePointer ()
/* 
   Fa riapparire il puntatore del mouse. 
    
   ****** La presente routine è il wrapper per la sezione CODE
   
   Non sono previsti codici di errore per questa routine.
*/
{
          if (HAL_IsTheFirstTimeForMouse)    // Deve fare qualcosa solo se il puntatore non era sullo schermo
          {
                 ndHAL_GrHideMousePointer (HAL_MouseEmu_OldPosX, HAL_MouseEmu_OldPosY, ActiveFBPage);
                 // Cancella il puntatore del mouse
            
                 HAL_MouseEmu_OldPosX = HAL_MouseEmu_PosX;       
                 HAL_MouseEmu_OldPosY = HAL_MouseEmu_PosY;
                 
                 ndHAL_GrRegisterMouseBackGr (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
                 // Registra il background del mouse
                 
                 ndHAL_GrDrawMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
                 // Disegna il puntatore del mouse nella nuova posizione
          }       
   
}

/*
   -----------------------------------------------------------------------------
   Routines per l'abilitazione/disabilitazione del puntatore del mouse
   -----------------------------------------------------------------------------
*/



void ndHAL_EnableMousePointer ()
/*
    Questa routine dovrebbe provvedere ad attivare il Phoenix Mouse Thread.
    
    Essa non ha effetto sotto PSPE
*/
{
    return;    
}


void ndHAL_DisableMousePointer ()
/*
    Questa routine dovrebbe provvedere ad attivare il Phoenix Mouse Thread.
    
    Essa non ha effetto sotto PSPE
*/
{
    return;
}


void ndHAL_MouseControl (struct ndEvent_Type *EventAddress)
/*
     Questa routine costituisce il wrapper specifico per la piattaforma di
     MouseControl. 
     
     Si veda la documentazione di MouseControl in ndCODE_MouseCtrl.c per il
     significato della struct EventAddress. 
*/
{
     // Viene richiamata direttamente la versione NoPhoenix
     
     INTERNAL_NOPHOENIX_MouseControl (EventAddress);  
}

