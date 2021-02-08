/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3)
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
    NANODESKTOP EYECLIENT USB DEMO 01
    ----------------------------------------------------------------------------
*/


#include <nanodesktop.h>

int  NrUsbChannel;
int  CameraHandle;    
char CameraExit = 0;

char cbExit (char WndHandle)
{
     CameraExit = 1;
}

char cbRgb (char WndHandle)
{
   ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_RGB_TRASMISSION_MODE);
}

char cbRgb555 (char WndHandle)
{
   ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_RGB555_TRASMISSION_MODE); 
}

char cbGray (char WndHandle)
{
   ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_GRAY_TRASMISSION_MODE);  
}



int ndMain()
{    
    ndInitSystem ();
    
	//ndHAL_Delay (10);
	ndHAL_EnableMousePointer ();
	ndLP_EnableTrasparency ();
	
    CameraHandle = ndHAL_CAM_ActivateCamera (0);
	
    if (CameraHandle>=0)
    {
       int CamViewHandle;           
       CamViewHandle = ndLP_AllocateWindow_MENU (130, 0, 360, 230, "Webcam result", COLOR_WHITE, COLOR_LBLUE, COLOR_BLACK, COLOR_WHITE);
       
       ndWMENU_DefineMenuItem (CamViewHandle, "Options",        "Options", 0, 0,         RENDER);
       ndWMENU_DefineMenuItem (CamViewHandle, "Options.RGB",    "RGB",     ND_CALLBACK_IN_NEW_CONTEXT, &cbRgb,    RENDER);
       ndWMENU_DefineMenuItem (CamViewHandle, "Options.RGB555", "RGB555",  ND_CALLBACK_IN_NEW_CONTEXT, &cbRgb555, RENDER);
       ndWMENU_DefineMenuItem (CamViewHandle, "Options.Gray",   "Gray",    0, &cbGray,   RENDER); 
       ndWMENU_DefineMenuItem (CamViewHandle, "Options.Exit",   "Exit",    0, &cbExit,   RENDER); 
     
       ndLP_MaximizeWindow (CamViewHandle);
        
       ndHAL_CAM_ChangeResolution (CameraHandle, 200, 200);
       ndHAL_CAM_ChangeTransferMode (CameraHandle, ND_RGB555_TRASMISSION_MODE);
       
       ndHAL_Delay (3);
       
       while (!CameraExit)
       {
           ndHAL_CAM_GrabNewImage (CameraHandle, ND_CAM_TO_WINDOW, CamViewHandle, NDKEY_POSX (10) | NDKEY_POSY (10), 1);    
       }
    }
     	
	ndDelay (10);
	ndHAL_SystemHalt (0);
    
	return 0;

}
