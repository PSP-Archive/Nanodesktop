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
    Patch for network (PSPE mode)
    ----------------------------------------------------------------------------
    Questo file header aggiunge la definizione di alcuni simboli necessari per
    il funzionamento delle routines di rete, che non sono normalmente inclusi
    nell'SDK della PSP
    ----------------------------------------------------------------------------
*/

#ifndef _NETWORK_PATCH_H_
      #define _NETWORK_PATCH_H_ 
      
      // Impedirà che questo file possa essere incluso due volte nella compilazione
      
          // Necessario per il corretto funzionamento delle routines di rete
      
          #define SOCKET int
          #define INVALID_SOCKET (0xFFFFFFFF)
          
          typedef void (*STATUS_PROC)(int percent, void* statusData);
          
          #define NDSOCKADDR_IN_PTR void* 

#endif    // #ifndef _NETWORK_PATCH_H
