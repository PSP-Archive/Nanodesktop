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
    FUNZIONI DI INTERFACCIAMENTO CON L'OS (PSPE)
    ---------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

volatile void __attribute__((used)) CTORMGR_PSPE_do_global_dtors_aux (void);


void ndHAL_SystemHalt (int ExitCode)
/*
   Questa routine provoca la terminazione immediata dell'homebrew.
   
   Il parametro ExitCode sotto PSPE viene ignorato.
*/

{
  ndHAL_ExecuteCPPGlobalDestructors ();
  ndHAL_ClockDelay (3.0);
  
  sceKernelExitGame();
  return;
}

int ndHAL_SystemExec (char *Name)
/*
    Questa routine provvede ad arrestare il sistema ed a caricare un nuovo
    homebrew. Se la routine ritorna un errore in uscita, vuol dire che il
    processo di caricamento è fallito. Sono previsti i seguenti codici
    di errore:
        ERR_FILE_NOT_FOUND             Il file non è stato trovato
        ERR_FUNCTION_NOT_SUPPORTED     La funzione non è supportata   
*/

{
    // Sotto questa piattaforma la funzione non è supportata
    return ERR_FUNCTION_NOT_SUPPORTED;
}


void ndHAL_SignalTroublesInKernelExtender (void)
{
     /* Nothing to do on this platform */
}
