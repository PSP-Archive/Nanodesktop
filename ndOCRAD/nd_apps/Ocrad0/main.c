/* ----------------------------------------------------------------------------
   NANODESKTOP ndOCRAD DEMO 1
   ----------------------------------------------------------------------------
   Copyright 2008-2013 Filippo Battaglia
   Source code released under BSD license
   ----------------------------------------------------------------------------
*/


#include <nanodesktop.h>
#include <ndhighgui.h>
#include <iostream>

int OCRAD_main( const int argc, const char * argv[] ) throw();
int ErrorLevel;

IplImage *cvImage, *cvImageThr, *cvImageRes;
int UsbHandleCamera;

struct ndDataControllerType ndPadRecord;


int ndMain()
{
   cvInitSystem (0, 0);
   using namespace std;
   
   UsbHandleCamera = ndHAL_CAM_ActivateCamera (ND_USE_PSP_GOCAM);
    
   ndHAL_EnableMousePointer ();
   ndLP_EnableTrasparency ();
    
   if (UsbHandleCamera>=0)
   {
        // Change in B/N mode
        
        ndHAL_CAM_ChangeTransferMode (UsbHandleCamera, ND_GRAY_TRASMISSION_MODE);
        ndHAL_CAM_ChangeResolution   (UsbHandleCamera, 1024, 768);
        
        // Allocate the OpenCV image
                      
        cvImage    = cvCreateImage( cvSize (1024, 768), 8, 1);
        cvImageThr = cvCreateImage( cvSize (1024, 768), 8, 1);
        cvImageRes = cvCreateImage( cvSize (640, 480),  8, 1);
        
        while (1)
        {
            ndHAL_CAM_GrabNewImage (UsbHandleCamera, ND_CAM_TO_CVIMAGE, (int)(cvImage), 0, 0);
            // Put video informations in the OpenCV image
            
            cvShowImage ("Webcam image", cvImage);
            // Show the OpenCV image
            
            ndHAL_GetPad (&ndPadRecord);

            if (ndPadRecord.Buttons & PSP_CTRL_TRIANGLE)
            {
                   cvAdaptiveThreshold (cvImage, cvImageThr, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 12);
                   // Execute a thresholding
                
                   cvResize (cvImageThr, cvImageRes, CV_INTER_AREA);
                   // Resizing
                
                   cvShowImage ("Thresholding image", cvImageRes);
                   // Show the OpenCV image
                
                   cvSaveImage ("ms0:/ocrad.pgm", cvImageRes);
                   // Save the image
                   
                   pseudoExec ( (void *)(&OCRAD_main), "Ocrad", "ms0:/ocrad.pgm", &ErrorLevel);
            }
        }
    }
    else
    {
        printf ("GoCam is not connected \n");
        ndDelay (2);
    }
}

