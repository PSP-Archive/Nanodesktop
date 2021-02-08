/*
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2006-2011 Filippo Battaglia
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
    FUNZIONI DI INIZIALIZZAZIONE DELLA PIATTAFORMA (PSPE)
    ---------------------------------------------------------------------------
*/

#define HAL_I_NEED_TO_DECLARE_VARIABLES
        #include <HAL_PSPE\$_HAL_NDforND.h>
#undef  HAL_I_NEED_TO_DECLARE_VARIABLES





void ndHAL_InitPlatform_StepA (void)
{
     ndHAL_InitComponents ();    // Reinizializza la libc ed il gestore di memoria
     ndHAL_InitGraph  ();        // Esegue le routine HAL per l'inizializzazione del sottosistema grafico
     ndHAL_InitFileSystem();     // Inizializza il file system
     ndHAL_InitIrSystem();       // Reinizializza il sistema ir  
     ndHAL_StartPhoenixGraphThread();  // Serve solo ad inizializzare le variabili per l'emulazione mouse  
}

void ndHAL_InitPlatform_StepB (void)
{
     // Niente da eseguire sotto PSPE
}

void ndHAL_InitPlatform_StepC (void)
{
     ndHAL_InitPosixEngine ();   // Inizializza i moduli per compatibilità POSIX
     ndPspeSeed = 1;
}
