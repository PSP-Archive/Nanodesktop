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
    GESTORE STD-OUT
    ----------------------------------------------------------------------------
*/

#include <$_NDforND.h>

void ndInitStdOutSystem ()
{
     StdOutWindowActivated = 0;                                   // La finestra StdOut è disattivata all'inizio    
     
     StdOutColorTitle = COLOR_WHITE; 
     StdOutColorBGTitle = COLOR_BLUE;
     StdOutColorWindow = COLOR_BLACK;
     StdOutColorBorderWindow = COLOR_WHITE;

     StdOutColorText = COLOR_WHITE;
     StdOutColorTextErr = COLOR_RED;

     StdOutPosX1 = 10;
     StdOutPosY1 = 10;
     StdOutPosX2 = Windows_MaxScreenX - 10;
     StdOutPosY2 = Windows_MaxScreenY - 10;
     
     StdOutWindowHandle = 0;
     StdOutCustomDimsEnabled = 0;     // Dimensioni custom disattivate all'inizio
}

static ndint64 INTERNAL_StdOutWindowExitCallback (char WndHandle, ndint64 WndInfoField)
{
   ndDisableStdOutDialogBox (); 
   return 0; 
}



int ndActivateStdOutDialogBox ()
/* 
   ----------------------------------------------------------------------------
   Attiva la dialog-box che si occupa di visualizzare i messaggi dello stream
   stdout/stderr.
   
   Se la dialog-box è stata attivata, restituisce 1, mentre restituisce 0 in
   caso di errore.
   ----------------------------------------------------------------------------
*/

{
  char WndHandle;
  
  if (!StdOutWindowActivated)
  {
     if (!StdOutCustomDimsEnabled)
     {
         WndHandle = ndLP_CreateWindow (StdOutPosX1, StdOutPosY1, StdOutPosX2, StdOutPosY2, 
                                          "StdOut/StdErr Dialog Box", StdOutColorTitle, StdOutColorBGTitle,
                                          StdOutColorWindow, StdOutColorBorderWindow, 0);
     }
     else
     {
         WndHandle = ndLP_CreateWindow (StdOutPosX1, StdOutPosY1, StdOutPosX2, StdOutPosY2, 
                                          "StdOut/StdErr Dialog Box", StdOutColorTitle, StdOutColorBGTitle,
                                          StdOutColorWindow, StdOutColorBorderWindow, 
                                          NDKEY_SETWSLENGTH (StdOutCustomDimX) | NDKEY_SETWSHEIGHT (StdOutCustomDimY));
     }
                                       
     if (WndHandle>=0)
     {
        ndLP_MaximizeWindow (WndHandle);
     
        ndLP_SetupCloseWndCallback (&INTERNAL_StdOutWindowExitCallback, 0, 0, WndHandle);
     
        StdOutWindowHandle = WndHandle;
        StdOutWindowActivated = 1;
     
        return 1;
     }  
     else
        return 0;
  }
  
}


int ndWriteToStdOut (char Carattere, char TypeChannel, char RenderNow)
/*
   Scrive un carattere nella finestra StdOut/StdErr.
   
   Se TypeChannel=0, allora verrà disegnato il carattere usando il color
   StdOutColorText, mentre se TypeChannel=1 verrà usato il colore 
   StdOutColorTextErr.

   Restituisce in uscita il codice di errore rilevato da ndWS_WriteLetter.
   Se la finestra stderr/stdout non è attiva, verrà restituito il 
   codice di errore
   
     ERR_STDOUT_WINDOW_NOT_ACTIVE     

   Se il carattere da scrivere è pari a '\n', la routine provvederà ad
   andare a capo.
*/

{
   TypeColor Color; 
            
   if (StdOutWindowActivated)
   {
      if (TypeChannel==0) Color=StdOutColorText;
      if (TypeChannel==1) Color=StdOutColorTextErr;
      
      if (Carattere!='\n')
         return ndWS_WriteLetter (Carattere, Color, StdOutWindowHandle, RenderNow);
      else
         return ndWS_CarriageReturn (StdOutWindowHandle);
         
   }
   else return ERR_STDOUT_WINDOW_NOT_ACTIVE;
}


void ndDisableStdOutDialogBox ()
/*
    Disattiva la dialog box stdout/stderr
*/
{
    if (StdOutWindowActivated)
    {
      ndLP_DestroyWindow (StdOutWindowHandle);
      StdOutWindowActivated=0;
    }
}

void  ndStdOutRender (void)
/*
     Esegue il rendering del box std-out. 
     
     Si occupa inoltre di modificare la posizione della ROI, in modo che la 
     posizione del cursore sia sempre in primo piano
*/
{
   int    PosPixCursX, PosPixCursY;
   int    NewOriginX, NewOriginY;
     
   if (StdOutWindowActivated)
   {
         if (StdOutCustomDimsEnabled)     // Attivate dimensioni custom
         {
             PosPixCursX=WindowData [StdOutWindowHandle].WS_PosCursX*8 + WindowData [StdOutWindowHandle].MargineCharX;
             PosPixCursY=WindowData [StdOutWindowHandle].WS_PosCursY*8 + WindowData [StdOutWindowHandle].MargineCharY;
             
             if (!  ((PosPixCursX >= WindowData [StdOutWindowHandle].ROI_PosX1) && 
                     (PosPixCursX <= WindowData [StdOutWindowHandle].ROI_PosX2) &&
                     (PosPixCursY >= WindowData [StdOutWindowHandle].ROI_PosY1) && 
                     (PosPixCursY <= WindowData [StdOutWindowHandle].ROI_PosY2-8))   )
             {
                     NewOriginX = PosPixCursX-WindowData [StdOutWindowHandle].ROI_LenX;
                     if (NewOriginX<0) NewOriginX=0;
                     
                     NewOriginY = PosPixCursY-(WindowData [StdOutWindowHandle].ROI_LenY-8);
                     if (NewOriginY<0) NewOriginY=0;
                     
                     ndLP_ChangeWindowROI (NewOriginX, NewOriginY, 0, StdOutWindowHandle);
                     // Eseguirà anche un WindowsRender in automatico         
             }
             else     // Il cursore è nell'area attiva: è necessario solo un WindowRender
             {
                     XWindowRender (StdOutWindowHandle); 
             }                                 
         }
         else     // Dimensioni custom disattivate: serve solo un window render
         {
             XWindowRender (StdOutWindowHandle);   
         }  
   }
}

      



char  ndEnableStdOutCustomDims (int NewDimX, int NewDimY)
/*
    Abilita le dimensioni custom della finestra stdout
    Ritorna 0 in caso di successo, un valore negativo in caso di errore. Sono
    previsti i seguenti codici di errore:
         ERR_WRONG_PARAMETERS    Le dimensioni sono superiori a 2048
*/
{
    if ((NewDimX<2048) && (NewDimY<2048))
    {
         StdOutCustomDimX=NewDimX; 
         StdOutCustomDimY=NewDimY;
         StdOutCustomDimsEnabled=1;
         
         if (StdOutWindowActivated)
         {
             ndDisableStdOutDialogBox ();
             ndActivateStdOutDialogBox ();                      
         }             
         
         return 0;
    }
    else
      return ERR_WRONG_PARAMETERS;     
}

void ndDisableStdOutCustomDims (void)
{
    StdOutCustomDimsEnabled=0;
         
    if (StdOutWindowActivated)
    {
       ndDisableStdOutDialogBox ();
       ndActivateStdOutDialogBox ();                      
    }    
}

char ndChangeStdOutPosition (unsigned short int PosX1, unsigned short int PosY1, unsigned short int PosX2, unsigned short int PosY2)
{
     if ((PosX1<Windows_MaxScreenX-1) && (PosX2<Windows_MaxScreenX-1) && (PosY1<Windows_MaxScreenY-1) && (PosY2<Windows_MaxScreenY-1))
     {
          StdOutPosX1 = PosX1;
          StdOutPosY1 = PosY1;
          StdOutPosX2 = PosX2;
          StdOutPosY2 = PosY2;
          
          return 0;                       
     }
     else    // Coordinate errate
       return ERR_WRONG_PARAMETERS;
} 

void ndRestoreStdOutStandardPosition ()
{
     StdOutPosX1 = 10;
     StdOutPosY1 = 10;
     StdOutPosX2 = Windows_MaxScreenX - 10;
     StdOutPosY2 = Windows_MaxScreenY - 10;      
}
