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
    ROUTINES DI GESTIONE USB SPECIFICHE PER PERIFERICA GOCAM  
    (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>



/*
   ---------------------------------------------------------------------------
   OPEN/CLOSE
   ---------------------------------------------------------------------------
*/



int USBDEV1_GOCAM_OpenDevice (void)
/*

    Questa routine provvede ad aprire la periferica GoCam di Sony.
    
    Se il sistema riesce a collegarsi, impegna un handle USB e lo 
    restituisce in uscita.
    
    Altrimenti viene restituito un codice di errore negativo.
    
    Sono previsti i seguenti codici di errore:
       ERR_GOCAM_SUPPORT_IS_DISABLED            Il supporto GoCam è stato disattivato  
       ERR_FUNCTION_HAS_BEEN_DISABLED           Le funzionalità USB sono state disabilitate dall'utente
       ERR_DISABLED_FOR_TROUBLES_IN_MODULE_LOAD Problemi nel caricamento dei moduli del kernel
       ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC     Disabilitato per problemi nel supporto JPEG
       ERR_NO_USB_HANDLE_IS_AVAILABLE           Nessun handle USB disponibile
       ERR_IMPOSSIBLE_TO_CHANGE_STATE           Il sistema non riesce a cambiare lo stato del bus USB
       ERR_DEVICE_IS_NOT_OPERATIVE              La GoCam non è collegata
       ERR_IO_ERROR                             Fallita la configurazione iniziale della videocamera
       ERR_SYSTEM_IS_BUSY                       Il bus USB è impegnato da un'altra periferica
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


void USBDEV1_GOCAM_CloseDevice (void)
{
   /* Nothing to do */ 
}


