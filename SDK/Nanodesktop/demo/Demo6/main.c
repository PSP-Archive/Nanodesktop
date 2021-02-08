/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    The library has been developed as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    NANODESKTOP DEMO RECORDER
    ----------------------------------------------------------------------------
    Copyright 2008-2013 bigboss (psp-ita team)
    All right reserved
    ----------------------------------------------------------------------------
*/



#include <nanodesktop.h>

extern unsigned int WallPaperWidth;
extern unsigned int WallPaperHeight;
extern unsigned short WallPaper[];

int  WndHandle, WndHandleAbout;
int  TBoxHandle;
char NameFile[255];

int YouCanExit;   

int UsbHandleCamera;
int isRec=0;
int isPlay=0;

struct ndMediaObj_Type ndMediaObj;

/*
   -------------- FINESTRA ABOUT --------------------
*/

ndint64 cbCloseWndAbout (char WndHandle, ndint64 WndInfoField)
{
     ndLP_DestroyWindow (WndHandleAbout);
} 



void AboutWindow (void)
{
    WndHandleAbout = ndLP_CreateWindow (90, 100, Windows_MaxScreenX-10, Windows_MaxScreenY-10, 
                                 "About", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, NO_RESIZEABLE); 
                                 
    ndLP_MaximizeWindow (WndHandleAbout);
    
    ndWS_GrWriteLn (10, 20, "ND Audio Recorder ", COLOR_LBLUE, COLOR_WHITE, WndHandleAbout, NORENDER);
    
    ndWS_GrWriteLn (10, 40, "Copyright 2008-2013 bigboss (psp-ita team)", COLOR_BLACK, COLOR_WHITE, WndHandleAbout, NORENDER);
    ndWS_GrWriteLn (10, 50, "All right reserved ", COLOR_BLACK, COLOR_WHITE, WndHandleAbout, NORENDER);
    
    ndWS_GrWriteLn (10, 70, "This application is redistribuited ", COLOR_BLACK, COLOR_WHITE, WndHandleAbout, NORENDER);
    ndWS_GrWriteLn (10, 80, "with Nanodesktop distribution 0.3.4 ", COLOR_BLACK, COLOR_WHITE, WndHandleAbout, NORENDER);
    ndWS_GrWriteLn (10, 90, "as Nanodesktop Demo 6 ", COLOR_BLACK, COLOR_WHITE, WndHandleAbout, NORENDER);
    
    XWindowRender (WndHandleAbout);
    
    ndLP_SetupCloseWndCallback (&cbCloseWndAbout, 0, 0, WndHandleAbout); 
}

/*
   -------------- STATUS BAR ------------------------
*/

void StatusBar (char *Message)
{
    ndWS_DrawRectangle (10, 55, 310, 70, COLOR_GRAY20, COLOR_GRAY24, WndHandle, NORENDER); 
    ndWS_GrWriteLn (12, 58, Message, COLOR_BLACK, COLOR_GRAY20, WndHandle, NORENDER);
    XWindowRender (WndHandle);
}


/*
   -------------- AUDIO CALLBACKS -------------------
*/

static int initWebCam(void)
{
	UsbHandleCamera = ndHAL_CAM_ActivateCamera (ND_USE_PSP_GOCAM);
	
    if (UsbHandleCamera>=0) return 1;
	 else return 0;
}

static ndint64 cbRecAudio (char *StringID, ndint64 InfoField, char WndHandle)
{
	int ErrRep;
    	
    ErrRep=ndHAL_CAM_StartAudioRecording (UsbHandleCamera, (void *)(&NameFile), 44100, 40);
		
	if (!ErrRep)
    {	
	    isRec=1;
	    StatusBar ("Recording....");
        
        ndLP_PutWindowInFirstPlane(WndHandle);
	    return 0;
    }
    else
    {
        StatusBar ("Error in recording");
        return -1;
    }
}

static ndint64 cbStopAudio (char *StringID, ndint64 InfoField, char WndHandle)
{
	if(isRec)
	{
		ndHAL_CAM_StopAudioRecording (UsbHandleCamera); 
        ndDelay (2.0);
                                
		isRec=0;
	}
    
    if(isPlay)
    {
        ndHAL_MEDIA_Stop(&ndMediaObj); 
        isPlay=0;    
        
        isPlay=0;
        StatusBar ("");
        ndHAL_MEDIA_UnLoad (&ndMediaObj);
    }
    
    StatusBar ("");
}

static ndint64 cbPlayAudio (char *StringID, ndint64 InfoField, char WndHandle)
{
    if ( (!isRec) && (!isPlay) )
    {
        isPlay=1;
        int LoadMp3 = ndHAL_MEDIA_LoadFile ((void *)(&NameFile), -1, &ndMediaObj, 0, 0);
        
        StatusBar ("Playing....");
        ndHAL_MEDIA_Play (&ndMediaObj, ND_LOCK_UNTIL_SONG_FINISH);
        
        isPlay=0;
        StatusBar ("");
        ndHAL_MEDIA_UnLoad (&ndMediaObj);
    }
}


/*
   -------------- FINESTRA PRINCIPALE -------------------
*/


ndint64 CloseWndCallback (char WndHandle, ndint64 WndInfoField)
{
     YouCanExit=1;
     
     if (isRec)  ndHAL_CAM_StopAudioRecording (UsbHandleCamera);   
     if (isPlay) ndHAL_MEDIA_Stop(&ndMediaObj); 
     
     ndHAL_Delay(0.15);
} 

static ndint64 cbAbout (char WndHandle)
{
    AboutWindow ();   
}

static ndint64 cbExit (char WndHandle)
{
    YouCanExit=1;   
}



// Main source


int ndMain()
{
	struct ndImage_Type BackGround;
	char IMGERR;
	
	ndInitSystem();
	
    if ( initWebCam() )
    {
        ndIMG_InitImage (&BackGround);
    	
        ndIMG_LoadImageFromNDFArray (&BackGround, WallPaperWidth, WallPaperWidth, &WallPaper, NDMGKNB);
        ndIMG_ShowImage_InBS (&BackGround, 0, 0, RENDER);
        
        ndLP_EnableTrasparency ();
        	
    	WndHandle=ndLP_CreateWindow (10, 10, 350, 200, "Audio Recorder", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY22, COLOR_GRAY, MENUSUPPORTED);
    	ndLP_MaximizeWindow (WndHandle);
    	
    	ndWMENU_DefineMenuItem (WndHandle, "File", "File", 0, 0,  NORENDER);
       	ndWMENU_DefineMenuItem (WndHandle, "File.About", "About", 0, &cbAbout,  NORENDER);
    	ndWMENU_DefineMenuItem (WndHandle, "File.Exit",  "Exit",  0, &cbExit,   NORENDER);
    	
    	// Status bar iniziale
    	
    	StatusBar ("");
    	
        //Pulsanti
        
    	ndCTRL_CreateButton (310-40-40-38, 90, 310-40-40, 115, WndHandle, "REC", 
                             "REC", "", COLOR_RED, COLOR_BLUE, 
                             COLOR_WHITE, ND_CALLBACK_IN_NEW_CONTEXT | ND_BUTTON_ROUNDED | NDKEY_SETFONT (4),
                             &cbRecAudio, 0, NORENDER); //>>Pulsante REGISTRA
    	
    	ndCTRL_CreateButton (310-40-38, 90, 310-40, 115, WndHandle, "PLAY", 
                             "PLAY", "", COLOR_WHITE, COLOR_BLUE, 
                             COLOR_WHITE, ND_CALLBACK_IN_NEW_CONTEXT | ND_BUTTON_ROUNDED | NDKEY_SETFONT (4),
                             &cbPlayAudio, 0, NORENDER); //>>Pulsante RIPRODUCI
    	
    	ndCTRL_CreateButton (310-38, 90, 310, 115, WndHandle, "STOP", 
                             "STOP", "", COLOR_WHITE, COLOR_BLUE, 
                             COLOR_WHITE, ND_CALLBACK_IN_NEW_CONTEXT | ND_BUTTON_ROUNDED | NDKEY_SETFONT (4),
                             &cbStopAudio, 0, NORENDER); //>>Pulsante STOP
    	
    	memset (&NameFile, 0, 40);
    	strcpy (NameFile, "recorder.wav");
    	
    	TBoxHandle =   ndTBOX_CreateTextArea(10,10,310,40,"Nome File",
    										 NDKEY_SETTEXTAREA (40, 1) | NDKEY_FONT (3), &NameFile,
    										 COLOR_BLACK, COLOR_WHITE,
    										 COLOR_BLACK, COLOR_GRAY10,
    										 COLOR_BLACK, COLOR_GRAY03,
    										 NULL, NULL, NULL,
    										 0,
    										 WndHandle, NORENDER); //>>Text Box
    										 
        ndLP_SetupCloseWndCallback (&CloseWndCallback, 0, 0, WndHandle);
        
    	XWindowRender (WndHandle);
    	
    	YouCanExit=0;
    	ndProvideMeTheMouse_Until (&YouCanExit, ND_IS_EQUAL, 1, 0, 0); 
    	
    	ndLP_DestroyWindow(WndHandle);
    }
    else
    {
        printf (" Camera is not connected. Exiting... \n");
    }
    
    ndHAL_SystemHalt (0);
}
