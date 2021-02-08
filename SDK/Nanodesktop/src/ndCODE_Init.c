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
    ROUTINE NDINITSYSTEM
    ----------------------------------------------------------------------------
*/

#define I_NEED_TO_DECLARE_VARIABLES
        #include <$_NDforND.h>
#undef I_NEED_TO_DECLARE_VARIABLES



void ndInitSystem (void)
/* 
   Questa routine è il cuore di tutto il sistema di inizializzazione di
   Nanodesktop. Si occupa di reinizializzare tutti i vari sottosistemi
   presenti.
*/

{
    if (!ndInitSystemExecuted)
    {                                                 
       ndHAL_InitPlatform_StepA ();  
       // Reinizializza l'hardware di sistema, il sistema della memoria, il sistema grafico, 
       // il file-system, il sistema ir
         
       ndCODE_InitGraph ();    // Reinizializza la parte hardware independent di Nanodesktop     
       ndInitFont ();          // Reinizializza il gestore dei fonts
        
       InitializeStandardIconList ();  
       InitializeStandardClassList();
       
       #ifdef I_HAVE_DEV_IL
             ilInit ();
       #endif
       
       
       ndHAL_InitKeyboards ();
       
       ndInitStdInSystem ();
       ndInitStdOutSystem ();
       
       ndHAL_InitPlatform_StepB ();
       // Reinizializza il Phoenix Subsystem se abilitato  
       
       ndHAL_UpdateWindowsRenderRegisters_Core ();
       // Esegui l'inizializzazione dei registri per WindowsRender: in questo modo, se la prima richiesta
       // fatta dal programmatore è per una routine di tipo BaseScreen, non si verificherà un crash
      
       ndHAL_InitPlatform_StepC ();
       // Sotto PSP KSU, reinizializza i servizi KSU  
       
       ndInitSystemExecuted=1;
       
       ndHAL_SignalTroublesInKernelExtender ();
       // Sotto CFW, segnala problemi nel kernel extender. Non ha effetti sulle altre piattaforme
    
       ndHAL_ExecuteCPPGlobalConstructors ();
       // Inizializza i costruttori globali del C++
    }
}
