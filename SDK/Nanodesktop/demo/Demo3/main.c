/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2007-2012 Filippo Battaglia 
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
    license.txt in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ----------------------------------------------------------------------------
    NANODESKTOP INTERNET CONNECTION DEMO 
    ----------------------------------------------------------------------------
*/



#include <nanodesktop.h>
#include <gcurl/gcurl.h>

static struct gCurl_FileTransfer_Options  Options;
struct ndNetworkObject_Type NetObj;
    
int WndHandle, BtnHandle0, BtnHandle1, TxtHandle0, TxtHandle1; 
int YouCanExit = 0;
static char Site     [512];
static char Location [512];




ndint64 TheCloseWndCallback (char WndHandle, ndint64 WndInfoField)
{
     YouCanExit=1;
}

ndint64 cbConnect (char *StringID, ndint64 InfoField, char WndHandle)
{
    int ErrRep;
    
    Options.BeVerbose                   = 0;
    Options.NoAutoCloseDBoxWhenFinish   = 0;
    
    ErrRep=ndHAL_WLAN_OpenPSPAccessPoint (1, &NetObj, 0, 10);
    
    if (!ErrRep)                         // Connessione riuscita
    {
        ndCTRL_InhibitButton   (BtnHandle0, WndHandle, NORENDER);   
        ndCTRL_DeInhibitButton (BtnHandle1, WndHandle, NORENDER);
        XWindowRender (WndHandle);    
    }
    else                                 // Connessione fallita
    {
        ndCTRL_InhibitButton (BtnHandle1, WndHandle, NORENDER); 
        XWindowRender (WndHandle); 
    }

}

ndint64 cbDownload (char *StringID, ndint64 InfoField, char WndHandle)
{
    ndGCURL_GlobalInit ();
    ndGCURL_FileTransfer (&NetObj, Site, Location, &Options);
    ndGCURL_GlobalDeInit ();
    
    ndHAL_WLAN_ClosePSPAccessPoint (1);      // Chiudi la connessione realizzata
    
    // Ripristina la configurazione dei tasti originaria
    
    ndCTRL_DeInhibitButton (BtnHandle0, WndHandle, NORENDER);   
    ndCTRL_InhibitButton   (BtnHandle1, WndHandle, NORENDER);
    XWindowRender (WndHandle);    
}

void DrawInterface ()
{
    struct ndEvent_Type Evento; 
       
    ndInitSystem ();
    
    WndHandle = ndLP_AllocateWindow (30, 20, 360, 190, "Download a file", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY12, COLOR_BLACK);
    ndLP_MaximizeWindow (WndHandle);
    
    ndLP_SetupCloseWndCallback (&TheCloseWndCallback, 0, 0, WndHandle);   
    
    ndWS_GrWriteLn (10, 15, "Type internet address", COLOR_BLACK, COLOR_GRAY12, WndHandle, NORENDER); 
    
    TxtHandle0 = ndTBOX_CreateTextArea (10, 30, 300, 50, "Text area 0", NDKEY_SETTEXTAREA (255, 1), &Site, 
                                   COLOR_BLACK, COLOR_WHITE, 
                                   COLOR_BLACK, COLOR_GRAY07, 
                                   COLOR_BLACK, COLOR_GRAY03,
                                   0, 0, 0, 
                                   0, WndHandle, NORENDER);
    
    ndWS_GrWriteLn (10, 65, "Type destination path", COLOR_BLACK, COLOR_GRAY12, WndHandle, NORENDER); 
    
    TxtHandle1 = ndTBOX_CreateTextArea (10, 80, 300, 100, "Text area 1", NDKEY_SETTEXTAREA (255, 1), &Location, 
                                   COLOR_BLACK, COLOR_WHITE, 
                                   COLOR_BLACK, COLOR_GRAY07, 
                                   COLOR_BLACK, COLOR_GRAY03,
                                   0, 0, 0, 
                                   0, WndHandle, NORENDER);
    
                                   
    BtnHandle0 = ndCTRL_CreateButton (100, 110, 195, 130, WndHandle, "Connect",  "Connect", "",  COLOR_BLACK, COLOR_GRAY14, COLOR_BLACK, ND_BUTTON_ROUNDED | ND_CALLBACK_IN_NEW_CONTEXT, &cbConnect, 0, RENDER); 
    BtnHandle1 = ndCTRL_CreateButton (200, 110, 300, 130, WndHandle, "Download", "Download", "", COLOR_BLACK, COLOR_GRAY14, COLOR_BLACK, ND_BUTTON_ROUNDED | ND_BUTTON_INHIBITED | ND_CALLBACK_IN_NEW_CONTEXT,
     &cbDownload, 0, RENDER); 
    
    XWindowRender (WndHandle);    
}


int ndMain ()
{
    int Counter;
    
    ndInitSystem ();
    
    //ndHAL_NET_EnableNetworkMonitor();
    
    strcpy (&Site, "http://visilab.unime.it/~filippo/Nanodesktop/PSP_PSPE/Downloads/Distribution_0_3/Nanodesktop_TestDistribution.zip");
    strcpy (&Location, "ms0:/nanodesktop.zip");
    DrawInterface ();
    
    ndProvideMeTheMouse_Until (&YouCanExit, ND_IS_EQUAL, 1, 0, 0); 
    ndHAL_SystemHalt (0);  
}
 

