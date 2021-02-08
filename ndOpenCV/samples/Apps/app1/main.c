/* ----------------------------------------------------------------------------
   NANODESKTOP OPENCV INDIPENDENT DEMO NR. 1
   ----------------------------------------------------------------------------
   di Battaglia Filippo
   ----------------------------------------------------------------------------
   Questo demo è stato sviluppato autonomamente da Battaglia Filippo, autore
   anche della libreria Nanodesktop, nonchè del porting delle librerie 
   OpenCV su Nanodesktop.
   
   Il demo viene fornito in bundle ai packages che contengono le versioni
   delle OpenCV e di HighGUI sviluppate per funzionare in ambiente
   Nanodesktop.
   ----------------------------------------------------------------------------
*/

#include <nanodesktop.h>
#include <ndhighgui.h>




extern "C"
{



char MainWndHandler;
char WndAboutDialogBox;

struct ndEvent_Type Evento;

char YouCanExit, ErrRep;
char TrasformImageIsOk;

IplImage* MainImage;
IplImage* ImageTrasformed;





ndint64 Trasform_RED (char WndHandle)
{
   char ErrRep;  
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   ImageTrasformed=cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 3);
   
   if (ImageTrasformed!=0)
   {
      cvSetImageCOI (MainImage, 0);
      cvCopy (MainImage, ImageTrasformed, 0);
      cvSetImageCOI (ImageTrasformed, 1);
      
      cvShowImage ("Trasform", ImageTrasformed);
   
      if (!HGUI_SystemError)
      {
         TrasformImageIsOk=1;
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return 0;
      }
      else
      {
         TrasformImageIsOk=0;
         free (ImageTrasformed);
         ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return -1;  
      }   
   } 
   
}

ndint64 Trasform_GREEN (char WndHandle)
{
   char ErrRep;  
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   ImageTrasformed=cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 3);
   
   if (ImageTrasformed!=0)
   {
      cvSetImageCOI (MainImage, 0);
      cvCopy (MainImage, ImageTrasformed, 0);
      cvSetImageCOI (ImageTrasformed, 2);
      
      cvShowImage ("Trasform", ImageTrasformed);
   
      if (!HGUI_SystemError)
      {
         TrasformImageIsOk=1;
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return 0;
      }
      else
      {
         TrasformImageIsOk=0;
         free (ImageTrasformed);
         ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return -1;  
      }   
   } 
   
}

ndint64 Trasform_BLUE (char WndHandle)
{
   char ErrRep;  
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   ImageTrasformed=cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 3);
   
   if (ImageTrasformed!=0)
   {
      cvSetImageCOI (MainImage, 0);
      cvCopy (MainImage, ImageTrasformed, 0);
      cvSetImageCOI (ImageTrasformed, 3);
      
      cvShowImage ("Trasform", ImageTrasformed);
   
      if (!HGUI_SystemError)
      {
         TrasformImageIsOk=1;
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return 0;
      }
      else
      {
         TrasformImageIsOk=0;
         free (ImageTrasformed);
         ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return -1;  
      }   
   } 
   
}


ndint64 Trasform_BW (char WndHandle)
{
   char ErrRep;  
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   ImageTrasformed=cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   
   if (ImageTrasformed!=0)
   {
      cvConvertImage (MainImage, ImageTrasformed, 0);
      
      cvShowImage ("Trasform", ImageTrasformed);
   
      if (!HGUI_SystemError)
      {
         TrasformImageIsOk=1;
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return 0;
      }
      else
      {
         TrasformImageIsOk=0;
         free (ImageTrasformed);
         ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
         return -1;  
      }   
   } 
   
}


ndint64 Trasform_Laplace3 (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_16S, 1);
      
      if (ImageTrasformed)
      {
         cvLaplace (TempImage, ImageTrasformed, 3);
      
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}


ndint64 Trasform_Laplace5 (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_16S, 1);
      
      if (ImageTrasformed)
      {
         cvLaplace (TempImage, ImageTrasformed, 5);
         TrasformImageIsOk=1;
      
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}

ndint64 Trasform_Canny (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_8U, 1);
      
      if (ImageTrasformed)
      {
         cvCanny (TempImage, ImageTrasformed, 30, 10, 3);
      
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}


ndint64 Trasform_Threshold_100 (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_8U, 1);
      
      if (ImageTrasformed)
      {
         cvThreshold (TempImage, ImageTrasformed, 100, 255, CV_THRESH_BINARY);
      
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}


ndint64 Trasform_Threshold_200 (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_8U, 1);
      
      if (ImageTrasformed)
      {
         cvThreshold (TempImage, ImageTrasformed, 200, 255, CV_THRESH_BINARY);
      
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}




ndint64 Trasform_Smooth (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_8U, 1);
      
      if (ImageTrasformed)
      {
         cvSmooth (TempImage, ImageTrasformed, CV_GAUSSIAN, 5, 0, 0);
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}




ndint64 Trasform_Blur (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_8U, 1);
      
      if (ImageTrasformed)
      {
         cvSmooth (TempImage, ImageTrasformed, CV_BLUR, 5, 0, 0);
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}




ndint64 Trasform_Not (char WndHandle)
{
   char ErrRep;  
   IplImage *TempImage;
    
   if (ImageTrasformed!=0) free (ImageTrasformed); 
     
   TempImage = cvCreateImage (cvGetSize (MainImage), IPL_DEPTH_8U, 1);
   // Crea un immagine temporanea
   
   if (TempImage)
   {
      cvConvertImage (MainImage, TempImage, 0);
      // Converti l'immagine in bianco e nero.
    
      ImageTrasformed = cvCreateImage (cvGetSize(MainImage), IPL_DEPTH_8U, 1);
      
      if (ImageTrasformed)
      {
         cvNot (TempImage, ImageTrasformed);
         free (TempImage);     // Cancella l'immagine temporanea
          
         cvShowImage ("Trasform", ImageTrasformed);
   
         if (!HGUI_SystemError)
         {
             TrasformImageIsOk=1;
             ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return 0;
         }
         else
         {
             TrasformImageIsOk=0;
             free (ImageTrasformed);
             ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", RENDER);
             return -1;  
         }   
       } 
   }

}









void DefineAppMenu ()
{
    ndWMENU_DefineMenuItem (MainWndHandler, "File",      "File",     0, 0, NORENDER);
    
    ndWMENU_DefineMenuItem (MainWndHandler, "File.Open",  "Open",  0, 0, NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "File.Save",  "Save",  1, 0, NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "File.SaveT", "SaveT", 1, 0, NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "File.Exit",  "Exit",  0, 0, NORENDER);
    
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform",  "Trasform", 0, 0, NORENDER);
    
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Red",           "Red", 1, (void *)(&Trasform_RED), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Green",         "Green", 1, (void *)(&Trasform_GREEN), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Blue",          "Blue", 1, (void *)(&Trasform_BLUE), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Black and white",  "Black and white", 1, (void *)(&Trasform_BW), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Laplace3",     "Laplace 3", 1, (void *)(&Trasform_Laplace3), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Laplace5",     "Laplace 5", 1, (void *)(&Trasform_Laplace5), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Canny",        "Canny", 1, (void *)(&Trasform_Canny), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Threshold100", "Threshold 100", 1, (void *)(&Trasform_Threshold_100), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Threshold200", "Threshold 200", 1, (void *)(&Trasform_Threshold_200), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Smooth",       "Smooth", 1, (void *)(&Trasform_Smooth), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Blur",         "Blur", 1, (void *)(&Trasform_Blur), NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "Trasform.Not",          "Not", 1, (void *)(&Trasform_Not), NORENDER);
    
    ndWMENU_DefineMenuItem (MainWndHandler, "About", "About", 0, 0, NORENDER);
    
    XWindowRender (MainWndHandler);
}




void AppLoadImage (char *NameFile)
{
  int ErrRep;
  
  if (MainImage!=0) free (MainImage);
  
  MainImage=cvLoadImage (NameFile, 1);
  
  if ( (int)(MainImage) >0)
  {             
     cvShowImage ("OpenCV Application 1", MainImage);
    
     if (!HGUI_SystemError)
     {                               
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.Save",   NORENDER);
         
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Red",            NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Green",          NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Blue",           NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Black and white",NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Laplace3",       NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Laplace5",       NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Canny",          NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Threshold100",   NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Threshold200",   NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Smooth",         NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Blur",           NORENDER);
         ndWMENU_DeInhibitL2WMI (MainWndHandler, "Trasform.Not",            NORENDER);
    
         XWindowRender (MainWndHandler);
     }
     
  } 
}


void AppSaveImage (char *NameFile)
{
      int ErrRep;
      char SaveWindow;
      
      SaveWindow = ndLP_AllocateWindow (30, 30, 450, 200, "Save original image", COLOR_WHITE, COLOR_LBLUE, COLOR_BLACK, COLOR_WHITE);
      ndLP_MaximizeWindow (SaveWindow);
             
      ndWS_WriteLn ("Waiting....", COLOR_WHITE, SaveWindow, RENDER);
      ndWS_CarriageReturn (SaveWindow);
             
      ndWS_WriteLn ("Saving original image", COLOR_WHITE, SaveWindow, RENDER);
      cvSaveImage (NameFile, MainImage);
             
      if (!HGUI_SystemError)
         ndWS_WriteLn ("Saving successful", COLOR_WHITE, SaveWindow, RENDER);
      else
         ndWS_WriteLn ("Failure on writing. Errors reported.", COLOR_RED, SaveWindow, RENDER);
      
      ndLP_DestroyWindow (SaveWindow);
}



void AppSaveTrasfImage (char *NameFile)
{
      int ErrRep;
      char SaveWindow;
      
      SaveWindow = ndLP_AllocateWindow (30, 30, 450, 200, "Save trasformed image", COLOR_WHITE, COLOR_LBLUE, COLOR_BLACK, COLOR_WHITE);
      ndLP_MaximizeWindow (SaveWindow);
             
      ndWS_WriteLn ("Waiting....", COLOR_WHITE, SaveWindow, RENDER);
      ndWS_CarriageReturn (SaveWindow);
             
      ndWS_WriteLn ("Saving trasformation", COLOR_WHITE, SaveWindow, RENDER);
      cvSaveImage (NameFile, ImageTrasformed);
             
      if (!HGUI_SystemError)
         ndWS_WriteLn ("Saving successful", COLOR_WHITE, SaveWindow, RENDER);
      else
         ndWS_WriteLn ("Failure on writing. Errors reported.", COLOR_RED, SaveWindow, RENDER);
      
      ndLP_DestroyWindow (SaveWindow);
}


void AboutDialogBox (void)
{
  WndAboutDialogBox = ndLP_AllocateWindow (10, 30, Windows_MaxScreenX-10, Windows_MaxScreenY-30,
                         "About", COLOR_WHITE, COLOR_RED, COLOR_WHITE, COLOR_BLACK);
                         
  ndWS_CarriageReturn (WndAboutDialogBox);
  
  ndWS_WriteLn ("ndOpenCV Application 1 - Version 3.00", COLOR_BLUE, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("Copyright (C) 2006-2011 Filippo Battaglia", COLOR_BLUE, WndAboutDialogBox, NORENDER);
  ndWS_CarriageReturn (WndAboutDialogBox);
  ndWS_WriteLn ("This application is based on Nanodesktop library", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_CarriageReturn (WndAboutDialogBox);
  ndWS_WriteLn ("Routines for loading/saving images are provided by", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("ndDevIL released under LGPL license", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("DevIL: Copyright (C) 2000-2002 by Denton Woods", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  
  ndWS_CarriageReturn (WndAboutDialogBox);
  ndWS_WriteLn ("This application uses also IJG libjpg, pnglib", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("tifflib and libz deflate. ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_CarriageReturn (WndAboutDialogBox);
  
  ndWS_WriteLn ("Image algorithms come from ndOpenCV, modified", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("version of Intel OpenCV ", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("OpenCV: Copyright (C) 2000, Intel Corporation", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  
  ndWS_CarriageReturn (WndAboutDialogBox);
  ndWS_WriteLn ("See file License-demo-Visilab-v2.rtf in zip", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  ndWS_WriteLn ("package for further informations", COLOR_BLACK, WndAboutDialogBox, NORENDER);
  
  ndLP_MaximizeWindow (WndAboutDialogBox);  
  XWindowRender (WndAboutDialogBox);
}



void ControlRing1 ()
{
   YouCanExit=0;
   char FileToLoad[64];
   char FileToSave[64];
   
   while (!YouCanExit)
   {
       MouseControl (&Evento);
       
       if ((Evento.__TypeEvent==L2WMI_BUTTON_PRESSED) && (strcmp (Evento.__ZString, "FILE.OPEN")==0))  
       {
          ErrRep = FileManager ("Choose a file to load", 0, 0, &(FileToLoad[0]));
          
          if (ErrRep==0)
          {
             AppLoadImage (&(FileToLoad[0]));
          }
       }
       
       if ((Evento.__TypeEvent==L2WMI_BUTTON_PRESSED) && (strcmp (Evento.__ZString, "FILE.SAVE")==0))
       {
          ErrRep = FileManager ("Save original image", 0, 1, &(FileToSave[0]));
       
          if (ErrRep==0)
          {
             AppSaveImage (&(FileToSave[0]));           
          }
       }
       
       if ((Evento.__TypeEvent==L2WMI_BUTTON_PRESSED) && (strcmp (Evento.__ZString, "FILE.SAVET")==0))
       {
          ErrRep = FileManager ("Save trasformation", 0, 1, &(FileToSave[0]));
       
          if (ErrRep==0)
          {
             AppSaveTrasfImage (&(FileToSave[0]));           
          }
       }
       
       if ((Evento.__TypeEvent==L2WMI_BUTTON_PRESSED) && (strcmp (Evento.__ZString, "FILE.EXIT")==0))
       {
          ndSystemHalt (0);
       }
       
       if ((Evento.__TypeEvent==L1WMI_BUTTON_PRESSED) && (strcmp (Evento.__ZString, "ABOUT")==0))
       {
          AboutDialogBox ();
       }
       
       if ((Evento.__TypeEvent==A_WINDOW_WANTS_TO_BE_CLOSED) && (Evento.__WindowID==MainWndHandler))
       {
          ndLP_DestroyWindow (MainWndHandler);
          cvDestroyWindow ("Trasform");
          
          YouCanExit=1;
       }
       
       if ((Evento.__TypeEvent==A_WINDOW_WANTS_TO_BE_CLOSED) && (Evento.__WindowID==cvGetNdWindowHandler ("Trasform")))
       {
          cvDestroyWindow ("Trasform");
          ndWMENU_InhibitL2WMI (MainWndHandler, "File.SaveT", NORENDER);
         
          TrasformImageIsOk=0;
          
       }
       
       if ((Evento.__TypeEvent==A_WINDOW_WANTS_TO_BE_CLOSED) && (Evento.__WindowID==WndAboutDialogBox))
       {
          ndLP_DestroyWindow (WndAboutDialogBox);
       }
       
       if ((Evento.__TypeEvent==A_WINDOW_WANTS_TO_BE_CLOSED) && (Evento.__WindowID==cvGetNdWindowHandler ("OpenCV Application 1")))
       {
          ndSystemHalt (0);
       }
       
   }
}



                      


int ndMain ()
{
    cvInitSystem (0, 0); 
    ndHAL_MouseControlledByAnalog (0);
    
    ndLP_LoadWallPaperFromFile ("ms0:/DEMOPACK/CVAPP1/sfondo.jpg");
    ndLP_EnableTrasparency();
    
    TrasformImageIsOk=0;
    
    cvNamedWindow ("OpenCV Application 1", CVKEY_SETPOS (5, 5) | CVKEY_SETSIZE (455, 245) | CV_MENUSUPPORTED);
    MainWndHandler = cvGetNdWindowHandler ("OpenCV Application 1");
          
    ndLP_MaximizeWindow (MainWndHandler);
    
    DefineAppMenu ();
    ControlRing1 ();
    
    ndSystemHalt (0);
}

}
