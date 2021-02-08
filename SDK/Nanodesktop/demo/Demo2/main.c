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
    NANODESKTOP GRAPHDEMO 2
    ----------------------------------------------------------------------------
*/


#include <nanodesktop.h>

static int  GrabberThreadID;
int  NrUsbChannel;
int  CameraHandle, CamWndHandle;    
char CameraExit = 0;

char AboutWndIsOpen   = 0;
char OptionsWndIsOpen = 0;
char WallPaperNameIsOpen = 0;

char WndAboutDialogBox;
char WndOptions;

int  OverTextIsOn = 0;
int  TrasparencyIsOn = 0;
int  WallpaperIsOn = 0;
int  Cbox_ForEnableOverText, Cbox_ForEnableTrasparency, Cbox_ForEnableWallpaper;
int  Tbox_MsgOnScreen, Lbox_TrasmissionMode;

int  Btn_Wallpaper;
char AWallpaperNameHasBeenChoosen=0;

char StringBuffer0 [30];
char StringBuffer1 [30];
char NrCharsVisualizable;
int  PixPosX0, PixPosY0;
int  PixPosX1, PixPosY1; 

char ChangeModeRequest = 0;         
char ActualMode        = 1;         // 0 - RGB; 1 - RGB555; 2 - GRAY         
   
char MsgData [51][51];
char WallPaperName [255] = "ms0:\sfondo.bmp";

void ResetTextDataBuffer()
{
     int CounterX, CounterY;
     
     for (CounterY=0; CounterY<3; CounterY++)
     {
         for (CounterX=0; CounterX<51; CounterX++)
         {
             MsgData [CounterY][CounterX]=0;
         }
     }
}


void cbEnableDisableOverText(int OverTextIsEnabled, char *StringID, int CBoxHandle, char WndHandle)
{
     if (OverTextIsEnabled)
     {
        ndTBOX_DeInhibitTextArea (Tbox_MsgOnScreen, WndHandle, RENDER);
        return;
     }
     else
     {
        ndTBOX_InhibitTextArea (Tbox_MsgOnScreen, WndHandle, RENDER);
        return;
     } 
}

void cbChangeMsgOnScreen_BEFORE (void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle)
{
   sceKernelSuspendThread (GrabberThreadID); 
}


void cbChangeMsgOnScreen_AFTER (void *ProcessValueData, void *TextData, int TextBoxHandle, char TextBoxWndHandle)
{
   NrCharsVisualizable = 210 / 8;
   ndStrCpy (&(StringBuffer0), &(MsgData[0][0]), NrCharsVisualizable, 0);
   ndStrCpy (&(StringBuffer1), &(MsgData[1][0]), NrCharsVisualizable, 0);
   
   PixPosX0 = (WindowData [CamWndHandle].WSWndLength - strlen (StringBuffer0)*8)/2;
   PixPosY0 = 160;
   
   PixPosX1 = (WindowData [CamWndHandle].WSWndLength - strlen (StringBuffer1)*8)/2;
   PixPosY1 = 168;
   
   sceKernelResumeThread (GrabberThreadID);
}



static void cbRgb (int NrItem, ndint64 LBoxCode, int ListBoxHandle, char WndHandle)
{
   ChangeModeRequest=1;
   ActualMode       =0;
}

static void cbRgb555 (int NrItem, ndint64 LBoxCode, int ListBoxHandle, char WndHandle)
{
   ChangeModeRequest=1;
   ActualMode       =1;
}

static void cbGray (int NrItem, ndint64 LBoxCode, int ListBoxHandle, char WndHandle)
{
   ChangeModeRequest=1;
   ActualMode       =2;
}


void cbEnableDisableTrasparency (int TrasparencyIsEnabled, char *StringID, int CBoxHandle, char WndHandle)
{
     if (TrasparencyIsEnabled)
     {
        sceKernelSuspendThread (GrabberThreadID);
        ndLP_EnableTrasparency ();
        sceKernelResumeThread (GrabberThreadID);
        
        return;
     }
     else
     {
        sceKernelSuspendThread (GrabberThreadID);
        ndLP_DisableTrasparency ();
        sceKernelResumeThread (GrabberThreadID);
        
        return;
     } 
}


void cbEnableDisableWallpaper (int WallpaperIsEnabled, char *StringID, int CBoxHandle, char WndHandle)
{
     if (WallpaperIsEnabled)
     {
        ndCTRL_DeInhibitButton (Btn_Wallpaper, WndOptions, RENDER);
        
        ndLP_EnableBaseScreen ();
        ndLP_LoadWallPaperFromFile (WallPaperName);
        
        return;
     }
     else
     {
        ndCTRL_InhibitButton (Btn_Wallpaper, WndOptions, RENDER);
        
        ndLP_DisableBaseScreen ();
        return;
     } 
}


long int cbChooseWallpaperName (char *StringID, ndint64 InfoField, char WndHandle)
{
   char ErrRep;
   static char InternalBuffer [256];
   
   if (!WallPaperNameIsOpen)
   {
      WallPaperNameIsOpen=1;
      ErrRep = FileManager ("Choose a new wallpaper", 0, 0, &(InternalBuffer) );
      WallPaperNameIsOpen=0;
      
      if (!ErrRep)          // L'utente ha scelto un nuovo sfondo
      {
         AWallpaperNameHasBeenChoosen=1;
         strcpy (WallPaperName, InternalBuffer); 
         
         ndLP_LoadWallPaperFromFile (WallPaperName);                    
      }   
   }   
   
}




long int cbOptionsWnd_CLOSE (char WndHandle, ndint64 WndInfoField)
{
     ndLP_DestroyWindow (WndOptions);
     OptionsWndIsOpen=0;
}



char cbOptionsWnd_CREATE (char WndHandle)
{
     ndint64 BtnWallpaper_Features;
     
     if (!OptionsWndIsOpen)
     {
         WndOptions = ndLP_CreateWindow (40, 10, 380, 240, "About", COLOR_WHITE, COLOR_BLUE, COLOR_GRAY20, COLOR_GRAY20, 0); 
         ndLP_MaximizeWindow (WndOptions);
         
         if (WndOptions>=0)
         {
             ndLP_SetupCloseWndCallback (cbOptionsWnd_CLOSE, 0, 1, WndOptions);
             
             // Crea la casella di spunta per il controllo dell'overtext
             
             Cbox_ForEnableOverText=ndCBOX_CreateCBox (10, 10, "OverText", OverTextIsOn, 0, &OverTextIsOn, COLOR_GRAY24, &cbEnableDisableOverText, WndOptions, NORENDER); 
             ndWS_GrWriteLn (40, 13, "Enable message on screen", COLOR_BLACK, COLOR_GRAY20, WndOptions, NORENDER);
             
             
             Tbox_MsgOnScreen = ndTBOX_CreateTextArea    (10, 30, 300, 55, "Message on screen", ND_TEXTAREA_INHIBITED | NDKEY_SETTEXTAREA (50, 2), &MsgData, 
                                                                 COLOR_BLACK, COLOR_WHITE, COLOR_BLACK, COLOR_GRAY20, COLOR_BLACK, COLOR_GRAY10,
                                                                 &cbChangeMsgOnScreen_BEFORE, 0, &cbChangeMsgOnScreen_AFTER, 0, WndOptions, NORENDER);
             
             ndWS_GrWriteLn (10, 65, "Choose trasmission mode", COLOR_BLACK, COLOR_GRAY20, WndOptions, NORENDER);
             
             
             Lbox_TrasmissionMode = ndLBOX_CreateListBox (10, 80, 300, 130, ND_LBOX_STABLE_EVIDENCE | ND_CALLBACK_IN_NEW_CONTEXT, &ndLBoxDefaultColor1, WndOptions, RENDER);
   
             ndLBOX_AddItemToListBox ("RGB mode",  &cbRgb, 0, 0, Lbox_TrasmissionMode, WndOptions);
             ndLBOX_AddItemToListBox ("RGB555 mode", &cbRgb555, 0, 0, Lbox_TrasmissionMode, WndOptions);
             ndLBOX_AddItemToListBox ("GRAY tones", &cbGray, 0, 0, Lbox_TrasmissionMode, WndOptions);
             
             ndLBOX_UpdateListBox (Lbox_TrasmissionMode, WndOptions, NORENDER);
             ndLBOX_EvidenceListBoxItem (ActualMode, Lbox_TrasmissionMode, WndOptions, NORENDER);
             
             Cbox_ForEnableTrasparency=ndCBOX_CreateCBox (10, 140, "EnableTrasparency", TrasparencyIsOn, 0, &TrasparencyIsOn, COLOR_GRAY24, &cbEnableDisableTrasparency, WndOptions, NORENDER); 
             ndWS_GrWriteLn (40, 143, "Enable trasparency", COLOR_BLACK, COLOR_GRAY20, WndOptions, NORENDER);
             
             Cbox_ForEnableWallpaper=ndCBOX_CreateCBox (10, 170, "EnableWallpaper", WallpaperIsOn, 0, &WallpaperIsOn, COLOR_GRAY24, &cbEnableDisableWallpaper, WndOptions, NORENDER);
             ndWS_GrWriteLn (40, 173, "Enable wallpaper", COLOR_BLACK, COLOR_GRAY20, WndOptions, NORENDER); 
             
             BtnWallpaper_Features = ND_BUTTON_ROUNDED | ND_CALLBACK_IN_NEW_CONTEXT;
             if (!WallpaperIsOn) BtnWallpaper_Features = BtnWallpaper_Features | ND_BUTTON_INHIBITED;
             // Inibisce il pulsante se i wallpaper non sono attivati
             
             Btn_Wallpaper = ndCTRL_CreateButton (200, 160, 300, 200, 
                               WndOptions, "Wallpaper", "Choose", "wallpaper", 
                               COLOR_BLACK, COLOR_GRAY20, COLOR_BLACK,
                               BtnWallpaper_Features, 
                               &(cbChooseWallpaperName), 0, NORENDER);
             
             
             XWindowRender (WndOptions);       
             OptionsWndIsOpen=1; 
         }        
     }     
}








ndint64 cbAboutWnd_CLOSE (char WndHandle, ndint64 WndInfoField)
{
     ndLP_DestroyWindow (WndAboutDialogBox);
     AboutWndIsOpen=0;
}


char cbAboutWnd_CREATE (char WndHandle)
{
     if (!AboutWndIsOpen)
     {
         WndAboutDialogBox = ndLP_CreateWindow (20, 20, 410, 200, "About", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK, 0); 
         ndLP_MaximizeWindow (WndAboutDialogBox);
         
         if (WndAboutDialogBox>=0)
         {
             ndLP_SetupCloseWndCallback (cbAboutWnd_CLOSE, 0, 1, WndAboutDialogBox);
             
             ndWS_CarriageReturn (WndAboutDialogBox);
             ndWS_WriteLn (" Nanodesktop Demo 2 - Version 1.00", COLOR_BLUE, WndAboutDialogBox, NORENDER);
             ndWS_WriteLn (" Copyright (C) 2007-2011 Filippo Battaglia", COLOR_BLUE, WndAboutDialogBox, NORENDER);
             ndWS_CarriageReturn (WndAboutDialogBox);
             ndWS_WriteLn (" This application is based on Nanodesktop lib", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_WriteLn (" vers.0.3 ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
             ndWS_CarriageReturn (WndAboutDialogBox);
             XWindowRender (WndAboutDialogBox);         
                      
             AboutWndIsOpen=1; 
         }        
     }     
}

char cbExit (char WndHandle)
{
     CameraExit = 1;
}























void MakeOverText ()
{
   ndWS_GrWriteLn (PixPosX0, PixPosY0, StringBuffer0, COLOR_YELLOW, 0, CamWndHandle, TRASP | NORENDER);
   
   ndWS_GrWriteLn (PixPosX1, PixPosY1, StringBuffer1, COLOR_YELLOW, 0, CamWndHandle, TRASP | NORENDER);   
}     



static int GrabbedThread (SceSize args, void *argp)
{   
   CameraExit=0;
   
   while (!CameraExit)
   {
       if (ChangeModeRequest)
       {
           switch (ActualMode)
           {
               case 0:
               {
                    ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_RGB_TRASMISSION_MODE); 
                    ChangeModeRequest=0;
                    break;
               }
               
               case 1:
               {
                    ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_RGB555_TRASMISSION_MODE); 
                    ChangeModeRequest=0;
                    break;
               }
               
               case 2:
               {
                    ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_GRAY_TRASMISSION_MODE); 
                    ChangeModeRequest=0;
                    break;
               }   
           }                 
       }
       else
       {
           ndHAL_CAM_GrabNewImage (CameraHandle, ND_CAM_TO_WINDOW, CamWndHandle, NDKEY_POSX (10) | NDKEY_POSY (10), NORENDER);    
           if (OverTextIsOn) MakeOverText (); 
       
           XWindowRender (CamWndHandle);
       }
   } 
   
   sceKernelExitDeleteThread (0);
}



int ndMain ()
{    
    char ExStr [512];
    char InternalBuffer [512];
    
    ndInitSystem ();
	ResetTextDataBuffer ();
	
	ndHAL_EnableMousePointer ();
	
    CameraHandle = ndHAL_CAM_ActivateCamera (0);
	
    if (CameraHandle>=0)
    { 
       CamWndHandle = ndLP_CreateWindow (130, 0, 360, 230, "Webcam result", COLOR_WHITE, COLOR_LBLUE, COLOR_BLACK, COLOR_WHITE, MENUSUPPORTED);
       
       ndWMENU_DefineMenuItem (CamWndHandle, "File",            "File",     0, 0,         RENDER);
       ndWMENU_DefineMenuItem (CamWndHandle, "File.Options",    "Options",  0, &cbOptionsWnd_CREATE,  RENDER);
       ndWMENU_DefineMenuItem (CamWndHandle, "File.About",      "About",    0, &cbAboutWnd_CREATE,    RENDER);
       ndWMENU_DefineMenuItem (CamWndHandle, "File.Exit",       "Exit",     0, &cbExit,   RENDER); 
       
       ndLP_MaximizeWindow (CamWndHandle);
        
       ndHAL_CAM_ChangeResolution (CameraHandle, 200, 200);
       ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_RGB555_TRASMISSION_MODE);
       
       GrabberThreadID = sceKernelCreateThread ("GrabberThread", &GrabbedThread, 0x15, 32768, THREAD_ATTR_USER, 0);
       sceKernelStartThread (GrabberThreadID, 0, 0);
       
       sceKernelWaitThreadEnd (GrabberThreadID, 0);
       // Attendi la fine del thread che esegue il ciclo grab
    
    }
    
	ndHAL_SystemHalt (0);
    
	return 0;

}










