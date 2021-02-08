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
    INFO API (HAL - PSP)
    ----------------------------------------------------------------------------
*/          

#include <HAL_PSP\$_HAL_NDforND.h>




inline ndint64 ndINFO_HAL_GetValue (int Code, int Param1, int Param2, int Param3)
/*
    Questa routine consente all'utente di ottenere le informazioni su Nanodesktop
    che sono specifiche per la piattaforma utilizzata (in questo caso PSP/PSPE).
    
    I parametri Param1, Param2, Param3 non hanno sempre significato: il loro
    eventuale significato dipende dal codice.

    La routine segnala un errore nei parametri restituendo un valore in uscita
    NDGET___ERROR.
*/
{
    switch (Code)
    {
           // Routine di identificazione processore, sistema operativo
           
           case NDGET_OPERATINGSYSTEM:
           {
                /* Questa routine fornisce informazioni sul sistema operativo su cui sta girando
                   Nanodesktop. 
                   
                   La routine chiamante deve allocare una stringa di 255 byte, e ne deve passare
                   l'indirizzo tramite il parametro Param1. 
                   
                   La stringa verrà copiata nell'indirizzo fornito.
                */
                
                strcpy (Param1, "Sony Playstation Portable");
                return 0;
           }
           
           case NDGET_CPUFREQUENCY:
           {
                return 333;        // Restituisce la frequenza in Mhz del processore.
           }
           
           // Routine di tipo LowLevel
           
           case NDGET_PAGESIZE:
           {
                return PageSize;
           }
           
           case NDGET_ACTIVEFBPAGE:
           {
                return ActiveFBPage;
           }
           
           case NDGET_PSPEBASEREGISTER:
           {
                return PSPEBaseRegister;
           }
           
           case NDGET_XADDRVIDEOMEMORY:
           {
                return XAddrVideoMemory;
           }
           
           case NDGET_ADDRVIDEOMEMORY:
           {
                return AddrVideoMemory;
           }
           
           case NDGET_LASTFBPAGEWRITTEN:
           {
                return LastFBPageWritten;
           }
           
           case NDGET_WRCURRENTPAGEADDR: 
           {
                return WRCurrentPageAddr;
           }
           
           case NDGET_LASTFBPAGEREAD:
           {
                return LastFBPageRead;
           }
           
           case NDGET_RDCURRENTPAGEADDR:
           {
                return RDCurrentPageAddr;
           }
           
           case NDGET_LASTLINEREAD:
           {
                return LastLineRead;
           }
           
           case NDGET_LASTLINEWRITTEN:
           {
                return LastLineWritten;
           }
           
           case NDGET_RDCURRENTLINEADDR:
           {
                return RDCurrentLineAddr;
           }
           
           case NDGET_WRCURRENTLINEADDR:
           {
                return WRCurrentLineAddr;
           }  
           
           case NDGET_PSPFIRMWAREVERSION:      // Sotto PSP non ci sono funzioni che consentano di accedere
           {                                   // alla rom e ricavare il codice firmware reale
                return NDPSPFW_UNKNOWN;
           }  
           
           case NDGET_EMULATEDFIRMWAREVERSION: 
           {
                return sceKernelDevkitVersion();
           } 
           
           case NDGET_ISPSPSLIM:
           {
                   // Sotto PSP, il simbolo ndIsPspSlim non è disponibile, quindi
                   // il controllo viene eseguito a run-time
                
                   if (kuKernelGetModel() == PSP_MODEL_SLIM_AND_LITE) return 1;
                     else return 0;
                   
           }
    }
    
    return NDGET___ERROR;                // Ritorna codice di errore
}


int ndINFO_HAL_GetPlatformCode (void)
/*
    Restituisce il codice a 32 bit che caratterizza i codici info per
    l'HAL di questa piattaforma
*/
{
    return NDGET_HAL_PSP;
}         
         
