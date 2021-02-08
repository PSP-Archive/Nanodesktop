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
    FUNZIONI DI INTERFACCIAMENTO CON IL SISTEMA OPERATIVO
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void ndSystemHalt (int ExitCode)
/*
   Questa routine provoca la terminazione immediata dell'applicazione Nanodesktop

   Se il sistema operativo prevede un ErrorLevel, viene restituito il parametro
   ExitCode.
*/

{
  ndHAL_SystemHalt (ExitCode);
}


