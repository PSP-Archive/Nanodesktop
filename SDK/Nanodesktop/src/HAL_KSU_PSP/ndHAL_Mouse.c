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
    ROUTINES PER L'INTERAZIONE CON I MENU TRAMITE MOUSE (PSP KSU mode)
    ----------------------------------------------------------------------------
    Queste routine si occupano di gestire l'emulazione del mouse
    ----------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>


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
    queste routine agiscono esclusivamente a livello grafico: sono usate
    per esempio dal sistema PhoenixGraph per visualizzare/spostare/cancellare
    il puntatore del mouse
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
   
   BackgroundMouseFrame = ndNrVideoFrame;   
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
    //   Il comportamento è diverso a seconda che sia stato o meno disattivato il
    //   Phoenix SubSystem. 
     
    if (!ND_SWMODE_DisablePhoenixSubsystem)
    {
             // Se il Phoenix Subsystem non è stato disabilitato, il sistema
             // deve limitarsi a cambiare i registri HAL_MouseEmu_PosXY ed
             // ad attendere che il client WindowsRender ridisegni il
             // frame con il puntatore del mouse nella nuova posizione
             
             if ( (PosX<Mouse_MaxScreenX) && (PosY<Mouse_MaxScreenY) )
             {
                  if (HAL_MouseEmuSystemEnabled)
                  {
                      sceKernelSuspendThread (phnGraph_ThreadID);     // Sospendi il thread di gestione grafica per sicurezza
                         
                      HAL_MouseEmu_OldPosX = HAL_MouseEmu_PosX;       
                      HAL_MouseEmu_OldPosY = HAL_MouseEmu_PosY;
                      HAL_MouseEmu_PosX    = PosX;
                      HAL_MouseEmu_PosY    = PosY;
                      
                      sceKernelResumeThread (phnGraph_ThreadID);      // Riattiva il thread grafico
                      
                      ndHAL_WindowsRender_CLIENT ();                  // L'esecuzione di un WindowsRender ricreerà il frame grafico            
                                                                      // con il puntatore del mouse nella nuova posizione
                  }
                  
                  return 0;
             }
             else
                 return ERR_WRONG_PARAMETERS;
    }
    else         // Il Phoenix SubSystem è stato disattivato
    {
            // Se il Phoenix SubSystem è stato disabilitato, il sistema deve spostare il
            // puntatore con un'operazione nello stesso contesto della routine chiamante
            
            // Quando il Phoenix SubSystem è stato disabilitato, l'operazione di salvataggio
            // del background e di ridisegno del puntatore deve avvenire necessariamente
            // sulla pagina del framebuffer corrente (e non su quella in costruzione in
            // background come avviene quando il Phoenix è attivato)
            
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
}


inline void ndHAL_HideMousePointer ()
/* 
   Cancella il puntatore del mouse. Questa routine fa scomparire il puntatore del
   mouse, ma non arresta l'eventuale Phoenix Mouse Thread che continua a girare
   in background nel frattempo. 
   
   Se si vuole cancellare il puntatore e nel contempo arrestare il Phoenix Mouse
   Thread, usare la funzione ndHAL_DisableMousePointer()
   
   ****** La presente routine è il wrapper per la sezione CODE
   
   Non sono previsti codici di errore per questa routine.
*/
{
   if (!ND_SWMODE_DisablePhoenixSubsystem)
   {
          if (HAL_MouseEmuSystemEnabled)
          {
              sceKernelSuspendThread (phnGraph_ThreadID);     // Sospendi il thread di gestione grafica per sicurezza             
              HAL_MouseEmuSystemEnabled=0;                    // Indurrà il thread a cancellare il puntatore del mouse   
              sceKernelResumeThread (phnGraph_ThreadID);      // Riattiva il thread grafico
              
              ndHAL_WindowsRender_CLIENT ();                  // L'esecuzione di un WindowsRender ricreerà il frame grafico            
                                                              // senza il puntatore del mouse
          }
   }
   else   // Il sistema Phoenix è disabilitato
   {
          ndHAL_GrHideMousePointer (HAL_MouseEmu_PosX, HAL_MouseEmu_PosY, ActiveFBPage);
          HAL_IsTheFirstTimeForMouse = 1;
   }           
}


inline void ndHAL_ShowMousePointer ()
/* 
   Fa riapparire il puntatore del mouse. La routine ha un comportamento diverso
   a seconda che il Phoenix Subsystem sia stato o meno disabilitato con la
   direttiva #define ND_DISABLE_PHOENIX_SUBSYSTEM nel codice principale del
   programma.
   
   Se il Phoenix Subsystem non è stato disabilitato con tale direttiva, la
   routine farà comparire il puntatore del mouse nella sua attuale posizione,
   a condizione che il Phoenix Mouse Thread in quel momento sia già attivo.
   Se il Phoenix Mouse Thread non è attivo in quel momento, questa routine
   non ha effetto. Ricordiamo al lettore che è possibile riattivare il Phoenix
   Mouse Thread, nelle piattaforme che lo supportano, per mezzo della routine
   ndHAL_EnableMousePointer (). 
    
   Se invece il Phoenix Subsystem era stato disabilitato con la direttiva
   #define ND_DISABLE_PHOENIX_SUBSYSTEM, oppure se la piattaforma non supporta
   il sistema Phoenix (es. PSPE), il comportamento è diverso: la routine si
   limita a fare comparire sullo schermo un puntatore del mouse nella sua
   attuale posizione.   
    
   ****** La presente routine è il wrapper per la sezione CODE
   
   Non sono previsti codici di errore per questa routine.
*/
{
   if (!ND_SWMODE_DisablePhoenixSubsystem)
   {   
          if (HAL_MouseEmuSystemEnabled)
          {
              sceKernelSuspendThread (phnGraph_ThreadID);     // Sospendi il thread di gestione grafica per sicurezza             
              HAL_MouseEmuSystemEnabled=1;                    // Indurrà il thread a fare riapparire il puntatore del mouse   
              sceKernelResumeThread (phnGraph_ThreadID);      // Riattiva il thread grafico
              
              ndHAL_WindowsRender_CLIENT ();                  // L'esecuzione di un WindowsRender ricreerà il frame grafico            
                                                              // con il puntatore del mouse
          }  
   }
   else
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
}


/*
   -----------------------------------------------------------------------------
   Routines per l'abilitazione/disabilitazione del puntatore del mouse
   -----------------------------------------------------------------------------
*/



void ndHAL_EnableMousePointer ()
/*
   Questa routine provvede a mostrare sullo schermo il puntatore del mouse. 
   
   Se il thread del mouse non stato ancora avviato, la routine avvia il thread
   del mouse
   
   Se il thread del mouse è stato già avviato, la routine si limita a fare uscire
   il thread dallo stato di sospensione in cui si trova.
   
   La routine ha effetto solo se il Phoenix Mouse Thread non è stato disabilitato
   per mezzo della direttiva ND_DISABLE_PHOENIX_SUBSYSTEM, altrimenti non ha
   alcun effetto. 
*/
{
   if (!ND_SWMODE_DisablePhoenixSubsystem)
   {  
              if (HAL_MouseThreadStatus==0)     // Il thread che gestisce il mouse non era stato ancora avviato
              {
                   ndHAL_StartPhoenixMouseThread ();
                   // Avvia il thread di gestione del mouse: Phoenix Graph provvederà dopo qualche millisecondo
                   // a visualizzare la freccia sullo schermo
                   
                   ndDelay (0.1);               // Ritardo necessario per dare il tempo al thread di stabilizzarsi
                   
                   return;
              }
              
              if (HAL_MouseThreadStatus==1)     // Il thread che gestisce il mouse è stato già avviato ed è in stato di 
              {                                 // sospensione
                   
                   ndHAL_ResumePhoenixMouseThread (); 
                   // Riavvia il thread di gestione del mouse che era stato sospeso
                   
                   ndDelay (0.1);               // Ritardo necessario per dare il tempo al thread di stabilizzarsi
                   
                   return;                                                            
              }
   }          
}


void ndHAL_DisableMousePointer ()
/*
   Questa routine provvede a cancellare dallo schermo il puntatore del mouse.
   
   Se il thread del mouse è stato già avviato, esso viene posto in stato di
   sospensione. 
   
   La routine ha effetto solo se il Phoenix Mouse Thread non è stato disabilitato
   per mezzo della direttiva ND_DISABLE_PHOENIX_SUBSYSTEM, altrimenti non ha
   alcun effetto. 
*/
{
   if (!ND_SWMODE_DisablePhoenixSubsystem)
   {  
        if (HAL_MouseThreadStatus==2)     // Il thread era stato avviato ed era già attivo
        {
            ndHAL_SuspendPhoenixMouseThread ();
        }
   }   
}


void ndHAL_MouseControl (struct ndEvent_Type *EventAddress)
/*
     Questa routine costituisce il wrapper specifico per la piattaforma di
     MouseControl. 
     
     Si veda la documentazione di MouseControl in ndCODE_MouseCtrl.c per il
     significato della struct EventAddress. 
*/
{
     if ( !ndInitSystemExecuted ) ndInitSystem();
     
     if (!ND_SWMODE_DisablePhoenixSubsystem)
     { 
         INTERNAL_PHOENIX_MouseControl_CLIENT (EventAddress);    
     }                      
     else   // Il Phoenix Mouse Thread è stato disabilitato. 
     {
         INTERNAL_NOPHOENIX_MouseControl (EventAddress);   
     }
}

