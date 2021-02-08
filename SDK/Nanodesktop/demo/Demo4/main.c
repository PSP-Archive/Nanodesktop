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
    NANODESKTOP VOICE SYNTHESIS DEMO 
    ----------------------------------------------------------------------------
*/



#define ND_DISABLE_MEDIA_SERVICES
#define ND_DISABLE_USB_SERVICES
        #include <nanodesktop.h>


#include "flite.h"

cst_voice *register_cmu_us_kal();
cst_voice *v;

int WndHandle, BtnHandle, TxtHandle; 
int YouCanExit = 0;
static char Message [255];



void cbProcessValueAfter (void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle)
{
     ndCTRL_DeInhibitButton (BtnHandle, WndHandle, RENDER);     
}


ndint64 TheCloseWndCallback (char WndHandle, ndint64 WndInfoField)
{
     YouCanExit=1;
}

ndint64 cbSpeak (char *StringID, ndint64 InfoField, char WndHandle)
{
    ndWS_GrWriteLn (30, 80, "Waiting....", COLOR_BLACK, COLOR_GRAY12, WndHandle, RENDER);
    flite_text_to_speech (Message,v,"play");
    ndWS_DrawRectangle (10, 78, 240, 92, COLOR_GRAY12, COLOR_GRAY12, WndHandle, RENDER);    
}


void DrawInterface ()
{
    struct ndEvent_Type Evento; 
       
    ndInitSystem ();
    
    WndHandle = ndLP_AllocateWindow (30, 30, 360, 220, "Voice synthesis", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY12, COLOR_BLACK);
    ndLP_MaximizeWindow (WndHandle);
    
    ndLP_SetupCloseWndCallback (&TheCloseWndCallback, 0, 0, WndHandle);   
    
    ndWS_GrWriteLn (10, 15, "Type a message in the text box", COLOR_BLACK, COLOR_GRAY12, WndHandle, NORENDER); 
    
    TxtHandle = ndTBOX_CreateTextArea (10, 30, 300, 50, "Text area 0", NDKEY_FONT (3) | NDKEY_SETTEXTAREA (255, 1), &Message, 
                                   COLOR_BLACK, COLOR_WHITE, 
                                   COLOR_BLACK, COLOR_GRAY07, 
                                   COLOR_BLACK, COLOR_GRAY03,
                                   0, 0, &cbProcessValueAfter, 
                                   0, WndHandle, NORENDER);
                                   
    BtnHandle = ndCTRL_CreateButton (250, 70, 300, 100, WndHandle, "Speak", "Speak", "", COLOR_BLACK, COLOR_GRAY14, COLOR_BLACK, ND_BUTTON_ROUNDED | ND_BUTTON_INHIBITED, &cbSpeak, 0, RENDER); 
        
}


int ndMain ()
{
    int Counter;
    
    ndInitSystem ();
    
    flite_init();
    v = register_cmu_us_kal ();
    memset (&Message, 0, 255);
    
    DrawInterface ();
    
    ndProvideMeTheMouse_Until (&YouCanExit, ND_IS_EQUAL, 1, 0, 0); 
    ndHAL_SystemHalt (0);  
}
 
