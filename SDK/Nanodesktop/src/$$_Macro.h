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
    Nanodesktop code macros
    ----------------------------------------------------------------------------
    Questo file speciale contiene le macro usato da Nanodesktop. 
    
    Esso può essere richiamato durante la compilazione dei moduli, oppure durante
    la compilazione delle applicazioni attraverso Nanodesktop.h
    ----------------------------------------------------------------------------
*/

#define WINDOWDATA_P(xA,PosX,PosY)  (*( WindowData[(xA)]._p + ( (WindowData [(xA)].HwWndLength) * (PosY) + (PosX) ) ))

#define WINDOWDATA_WS(xA,PosX,PosY) (*( WindowData[(xA)]._ws + (WindowData [(xA)].WSWndLength * (PosY) + (PosX) ) ))

#define WINDOWDATA_CHARMAP(xA,PosX,PosY) (*( WindowData[(xA)]._CharMap + (WindowData [(xA)].WS_MaxCursX * (PosY) + (PosX) ) ))

#define WINDOWDATA_COLORCHARMAP(xA,PosX,PosY) (*( WindowData[(xA)]._ColorCharMap + (WindowData [(xA)].WS_MaxCursX * (PosY) + (PosX) ) ))

#define WINDOWDATA_SS(xA,PosX,PosY) (*( WindowData[(xA)]._ss + (WindowData [(xA)].SS_LenX * (PosY) + (PosX) ) ))
