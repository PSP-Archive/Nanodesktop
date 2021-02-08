/* 
    ----------------------------------------------------------------------------
    NanoDesktop HighGUI (ndHighGUI) Library - Version 0.2
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    This is the porting of HighGUI (the component of Intel (R) OpenCV Libraries
    that allows use of windows environment), for Nanodesktop windows manager.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
        
    By downloading, copying, installing or using the software you agree to this license.
    If you do not agree to this license, do not download, install,
    copy or use the software.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    
      * Redistribution's of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
    
      * Redistribution's in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
    
      * The name of the Visilab research center may not be used to endorse or promote products
        derived from this software without specific prior written permission.
    
    This software is provided by the copyright holders and contributors "as is" and
    any express or implied warranties, including, but not limited to, the implied
    warranties of merchantability and fitness for a particular purpose are disclaimed.
    In no event shall the author of the software, the Visilab Research Center or 
    contributors be liable for any direct, indirect, incidental, special, exemplary, 
    or consequential damages (including, but not limited to, procurement of substitute 
    goods or services; loss of use, data, or profits; or business interruption) 
    however caused and on any theory of liability, whether in contract, strict liability,
    or tort (including negligence or otherwise) arising in any way out of
    the use of this software, even if advised of the possibility of such damage.
    ---------------------------------------------------------------------------
     ndHighGUI ERRORS MANAGER
    ---------------------------------------------------------------------------   
*/

#include <$_NDHGUI_for_NDHGUI.h>


void HGUI_NotifyError ( int code, char code2, const char *func_name, const char *err_msg)
/* ----------------------------------------------------------------------------
   Routine HGUI_NotifyError                                   Pubblica                   
   ----------------------------------------------------------------------------
   Esegue la notifica visuale dell'errore HighGUI.
   ---------------------------------------------------------------------------
*/

{
    unsigned char WndHandle;
    
    unsigned short int PosX1, PosY1, PosX2, PosY2;
    unsigned short int B1_Y1, B1_Y2;
    
    struct ndEvent_Type Event;
    
    if (HGUI_SystemErrorNotify_Enabled)
    {
        PosX1=(Windows_MaxScreenX - __HIGHGUIERRORBOX_LENX)/2;
        PosY1=(Windows_MaxScreenY - __HIGHGUIERRORBOX_LENY)/2;
        PosX2=(PosX1 + __HIGHGUIERRORBOX_LENX);
        PosY2=(PosY1 + __HIGHGUIERRORBOX_LENY);
    
        WndHandle=ndLP_CreateWindow (PosX1, PosY1, PosX2, PosY2, "ndHIGHGUI ERROR", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0);
        ndLP_MaximizeWindow (WndHandle);
    
        ndWS_WriteLn ("Internal error:", COLOR_BLACK, WndHandle, NORENDER);
        ndWS_CarriageReturn (WndHandle);
        ndWS_WriteLn ( (char *) (cvErrorStr(code)) , COLOR_BLACK, WndHandle, NORENDER);
        ndWS_WriteLn ( (char *) (err_msg), COLOR_BLACK, WndHandle, NORENDER);
    
        ndWS_CarriageReturn (WndHandle);
        ndWS_WriteLn ( (char *) (func_name), COLOR_BLACK, WndHandle, NORENDER);
        ndWS_CarriageReturn (WndHandle);
        
        if (code2!=0) ndWS_PrintLn (COLOR_BLACK, WndHandle, NORENDER, "Codice di errore Nanodesktop: %d", code2);
        
        B1_Y1 = WindowData [WndHandle].WSWndHeight - 25;
        B1_Y2 = WindowData [WndHandle].WSWndHeight - 5;
    
        ndCTRL_MakeOneButton ("OK", B1_Y1, B1_Y2, COLOR_BLACK, COLOR_GRAY, COLOR_BLACK, WndHandle, NOAUTOCLOSE);
        // Consenti all'utente di premere il tasto OK
    
        ndLP_DestroyWindow (WndHandle);
    }
    
    return;
}





void HGUI_DisableErrorNotification (void)
/* ----------------------------------------------------------------------------
   Routine HGUI_NotifyError                                   Pubblica                   
   ----------------------------------------------------------------------------
   Gli utenti ndHighGUI hanno la possibilità di disattivare la notifica visuale
   delle funzioni per mezzo di questa routine. In questo caso, il controllo
   degli eventuali errori potrà essere effettuato solo per mezzo della
   variabile di ambiente HGUI_SystemError.
   ----------------------------------------------------------------------------
*/
{
   HGUI_SystemErrorNotify_Enabled=0;
   return;   
}


void HGUI_EnableErrorNotification (void)
{
   HGUI_SystemErrorNotify_Enabled=1;  
   return;
}


