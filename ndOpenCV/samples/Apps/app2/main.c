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
IplImage *hist_image = 0;

int _brightness = 100;
int _contrast = 100;

int hist_size = 32;
float range_0[]={0,256};
float* ranges[] = { range_0 };

CvHistogram *hist;
uchar lut[256];
CvMat* lut_mat;

char TbCreated=0;




/* brightness/contrast callback function */
void update_brightcont( int arg )
{
    int brightness = _brightness - 100;
    int contrast = _contrast - 100;
    int i, bin_w;
    float max_value = 0;

    /*
     * The algorithm is by Werner D. Streidt
     * (http://visca.com/ffactory/archives/5-99/msg00021.html)
     */
    if( contrast > 0 )
    {
        double delta = 127.*contrast/100;
        double a = 255./(255. - delta*2);
        double b = a*(brightness - delta);
        for( i = 0; i < 256; i++ )
        {
            int v = cvRound(a*i + b);
            if( v < 0 )
                v = 0;
            if( v > 255 )
                v = 255;
            lut[i] = (uchar)v;
        }
    }
    else
    {
        double delta = -128.*contrast/100;
        double a = (256.-delta*2)/255.;
        double b = a*brightness + delta;
        for( i = 0; i < 256; i++ )
        {
            int v = cvRound(a*i + b);
            if( v < 0 )
                v = 0;
            if( v > 255 )
                v = 255;
            lut[i] = (uchar)v;
        }
    }

    cvLUT( MainImage, ImageTrasformed, lut_mat );
    cvShowImage( "OpenCV Application 2", ImageTrasformed );
    
    cvCalcHist( &ImageTrasformed, hist, 0, NULL );
    //cvZero( ImageTrasformed );
    cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
    cvScale( hist->bins, hist->bins, ((double)hist_image->height)/max_value, 0 );
    /*cvNormalizeHist( hist, 1000 );*/

    cvSet( hist_image, cvScalarAll(255), 0 );
    bin_w = cvRound((double)hist_image->width/hist_size);

    for( i = 0; i < hist_size; i++ )
        cvRectangle( hist_image, cvPoint(i*bin_w, hist_image->height),
                     cvPoint((i+1)*bin_w, hist_image->height - cvRound(cvGetReal1D(hist->bins,i))),
                     cvScalarAll(0), -1, 8, 0 );
   
    cvShowImage( "histogram", hist_image );
    
}









void DefineAppMenu ()
{
    ndWMENU_DefineMenuItem (MainWndHandler, "File",      "File",     0, 0, NORENDER);
    
    ndWMENU_DefineMenuItem (MainWndHandler, "File.Open",  "Open",  0, 0, NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "File.Save",  "Save",  1, 0, NORENDER);
    ndWMENU_DefineMenuItem (MainWndHandler, "File.Exit",  "Exit",  0, 0, NORENDER);
    
    ndWMENU_DefineMenuItem (MainWndHandler, "About", "About", 0, 0, NORENDER);
    
    XWindowRender (MainWndHandler);
}




void AppLoadImage (char *NameFile)
{
  int ErrRep;
  
  if (MainImage!=0) free (MainImage);
  
  MainImage=cvLoadImage (NameFile, 0);
  
  if ( (int)(MainImage) >0)
  {             
     ndWMENU_DeInhibitL2WMI (MainWndHandler, "File.Save",   NORENDER);
         
     if (!TbCreated)
     {
        cvCreateTrackbar ("brightness", "OpenCV Application 2", &_brightness, 200, update_brightcont);
        cvCreateTrackbar ("contrast", "OpenCV Application 2", &_contrast, 200, update_brightcont);
        TbCreated=1;
     }

     cvShowImage ("OpenCV Application 2", MainImage);
     cvNamedWindow("histogram", CVKEY_SETPOS (280, 30) | CVKEY_SETSIZE (150, 150) | CV_FORCE);

     ImageTrasformed = cvCloneImage (MainImage);
     hist_image = cvCreateImage (cvSize(320,200), 8, 1);
     hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
     lut_mat = cvCreateMatHeader( 1, 256, CV_8UC1 );
     cvSetData( lut_mat, lut, 0 );
     
     update_brightcont(0);
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
      cvSaveImage (NameFile, ImageTrasformed);
             
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
  
  ndWS_WriteLn ("ndOpenCV Application 2 - Version 3.00", COLOR_BLUE, WndAboutDialogBox, NORENDER);
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
    
    ndLP_LoadWallPaperFromFile ("ms0:/DEMOPACK/CVAPP1/sfondo.jpg");
    ndLP_EnableTrasparency();
    
    TrasformImageIsOk=0;
    
    cvNamedWindow ("OpenCV Application 2", CVKEY_SETPOS (5, 5) | CVKEY_SETSIZE (455, 245) | CV_MENUSUPPORTED);
    MainWndHandler = cvGetNdWindowHandler ("OpenCV Application 2");
          
    ndLP_MaximizeWindow (MainWndHandler);
    
    DefineAppMenu ();
    ControlRing1 ();
    
    ndSystemHalt (0);
    
}

}
