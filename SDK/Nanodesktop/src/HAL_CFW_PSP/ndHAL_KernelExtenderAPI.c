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
    API del Kernel Extender (PSP CFW mode)
    ----------------------------------------------------------------------------
*/          

#include <HAL_CFW_PSP\$_HAL_NDforND.h>

/*
   ----------------------------------------------------------------------------
   ROUTINES DI CARICAMENTO DEL KERNEL EXTENDER
   ----------------------------------------------------------------------------
*/

void ndHAL_LoadKernelExtender ()
/*
     Questa funzione ha lo scopo di caricare il kernel extender
*/
{
    u32 loadResult; 
    u32 startResult; 
    int status; 

    loadResult = kuKernelLoadModule (ND_KERNEL_EXTENDER_NAME, NULL, NULL); 
    
    if (loadResult & 0x80000000)
    { 
       ndKernelExtenderIsInError=1; 
       return;
    }
    else
    { 
       startResult = sceKernelStartModule(loadResult, 0, NULL, &status, NULL); 
    } 

    if (loadResult != startResult)
    { 
       ndKernelExtenderIsInError=2; 
       return; 
    } 
    
    ndKernelExtenderIsInError=0; 
    return; 
}

/*
   ---------------------------------------------------------------------------
   SEGNALAZIONE DI PROBLEMI NEL CARICAMENTO DEL KERNEL EXTENDER  
   ----------------------------------------------------------------------------
*/
 


static int AdvTroublesInKrnExtenderLoad_YouCanExit = 0;

static ndint64 INTERNAL_cbOK_AdvTroublesInKrnExtenderLoad (char *StringID, ndint64 InfoField, char WndHandle)
{
    AdvTroublesInKrnExtenderLoad_YouCanExit = 1;  
    return 0; 
}

void ndHAL_SignalTroublesInKernelExtender (void)
{
    int  WndHandle, BtnHandle;
    
    if (ndKernelExtenderIsInError)
    {       
            WndHandle = ndLP_CreateWindow (5, 10, Windows_MaxScreenX-5, Windows_MaxScreenY-10, 
                                           "Advice", COLOR_WHITE, COLOR_RED, COLOR_GRAY11, COLOR_GRAY11, 0);                               
            if (WndHandle>=0)
            {
                ndLP_MaximizeWindow (WndHandle);            
                
                ndWS_CarriageReturn (WndHandle);
                ndWS_CarriageReturn (WndHandle);
                
                ndWS_WriteLn ("This application has been developed with a version of", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_WriteLn ("Nanodesktop that requires the kernel extender .PRX to", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_WriteLn ("work correctly. The system is unable to find the    ", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_WriteLn ("following version of the kernel extender:           ", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_CarriageReturn (WndHandle);
                
                ndWS_WriteLn (ND_KERNEL_EXTENDER_NAME, COLOR_LBLUE, WndHandle, NORENDER);
                ndWS_CarriageReturn (WndHandle);
                
                ndWS_WriteLn ("This file is usually distributed by the producer of", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_WriteLn ("the software together with the PBP file", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_CarriageReturn (WndHandle);
                
                ndWS_WriteLn ("It can be also downloaded by Nanodesktop website    ", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_CarriageReturn (WndHandle);
                
                ndWS_WriteLn ("Now, Nanodesktop will disable the following functions:", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_CarriageReturn (WndHandle);
                ndWS_WriteLn ("- Audio, Media support                              ", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_CarriageReturn (WndHandle);
                
                ndWS_WriteLn ("If you find the correct version of the driver, copy it", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_WriteLn ("in the root folder of your memory stick and restart ", COLOR_BLACK, WndHandle, NORENDER);
                ndWS_WriteLn ("the program ", COLOR_BLACK, WndHandle, NORENDER);
                
                BtnHandle = ndCTRL_CreateButton   (190, 180, 190+60, 180+20, WndHandle, "OKButton", "OK", "",
                                                   COLOR_BLACK, COLOR_GRAY11, COLOR_BLACK,
                                                   ND_BUTTON_ROUNDED, 
                                                   &INTERNAL_cbOK_AdvTroublesInKrnExtenderLoad, 0, NORENDER);
                
                XWindowRender (WndHandle);
                                                   
                ndProvideMeTheMouse_Until (&AdvTroublesInKrnExtenderLoad_YouCanExit, ND_IS_EQUAL, 1, 0, 0); 
                
                ndLP_DestroyWindow (WndHandle);
            }        
    }
}

 
