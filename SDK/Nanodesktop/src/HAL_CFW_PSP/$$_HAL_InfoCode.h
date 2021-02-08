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
    CODICI INFO (HAL - PSP CFW mode)
    ----------------------------------------------------------------------------
*/   

#ifndef _HAL_INFOCODE_CONTAINER
        #define _HAL_INFOCODE_CONTAINER
        
        
                    #define NDGET_HAL_PSP                               0x80000000
                    
                    #define NDGET___ERROR                   0x8000000000000000
                    
                    // Codici info specifici per la PSP/PSPE
                    
                    #define NDGET_PAGESIZE                  NDGET_HAL_PSP + 0x0001
                    #define NDGET_ACTIVEFBPAGE              NDGET_HAL_PSP + 0x0002
                    #define NDGET_PSPEBASEREGISTER          NDGET_HAL_PSP + 0x0003
                    #define NDGET_XADDRVIDEOMEMORY          NDGET_HAL_PSP + 0x0004
                    #define NDGET_ADDRVIDEOMEMORY           NDGET_HAL_PSP + 0x0005
                    #define NDGET_LASTFBPAGEWRITTEN         NDGET_HAL_PSP + 0x0006
                    #define NDGET_WRCURRENTPAGEADDR         NDGET_HAL_PSP + 0x0007
                    #define NDGET_LASTFBPAGEREAD            NDGET_HAL_PSP + 0x0008
                    #define NDGET_RDCURRENTPAGEADDR         NDGET_HAL_PSP + 0x0009
                    #define NDGET_LASTLINEREAD              NDGET_HAL_PSP + 0x000A
                    #define NDGET_LASTLINEWRITTEN           NDGET_HAL_PSP + 0x000B
                    #define NDGET_RDCURRENTLINEADDR         NDGET_HAL_PSP + 0x000C
                    #define NDGET_WRCURRENTLINEADDR         NDGET_HAL_PSP + 0x000D
                    #define NDGET_PSPFIRMWAREVERSION        NDGET_HAL_PSP + 0x000E
                    #define NDGET_EMULATEDFIRMWAREVERSION   NDGET_HAL_PSP + 0x000F
                    #define NDGET_ISPSPSLIM                 NDGET_HAL_PSP + 0x0010           
                    
                    
                    #define NDGET_OPERATINGSYSTEM           NDGET_HAL_PSP + 0x1000
                    #define NDGET_CPUFREQUENCY              NDGET_HAL_PSP + 0x1001
   

#endif   // #ifndef _HAL_INFOCODE_CONTAINER
