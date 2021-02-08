/* ----------------------------------------------------------------------------
   OPENCV per Nanodesktop Demo
   ----------------------------------------------------------------------------
   
   Questi sono demo per le librerie OpenCv basate sulla libreria Nanodesktop
   per sistemi embedded.
   
   Questi demo sono basati su una versione modificata del codice presente 
   nella cartella /samples delle librerie OpenCV originali di Intel Corporation. 
   
   LA SUDDETTA SOCIETA' NON E' IN ALCUN MODO LEGATA AI SOFTWARE Nanodesktop,
   o ndHighGUI, che sono stati sviluppati indipendentemente. 
   
   INOLTRE, LA SUDDETTA SOCIETA' NON SUPPORTA IN ALCUN MODO LA LIBRERIA 
   ndOPENCV, CHE E' STATA MODIFICATA DA SVILUPPATORI INDIPENDENTI E NON
   LEGATI IN ALCUN MODO ALLA SOCIETA'.  
   
   LA SUDDETTA SOCIETA' NON  PUO' ESSERE CONSIDERATA RESPONSABILE PER IL 
   MALFUNZIONAMENTO DELLA SUDDETTA LIBRERIA.
   
   Il nome della suddetta società viene qui citato esclusivamente allo scopo 
   di informare l'utilizzatore della provenienza del codice originale, 
   conformemente a quanto previsto dall'art. 1 della licenza BSD che accompagna 
   le librerie OpenCV originali.
   
   In nessun caso, questa nota dovrà essere considerata un tentativo da 
   parte degli sviluppatori di utilizzare il nome della suddetta società
   a scopo pubblicitario, in violazione dell'art. 3 della licenza che
   accompagna il codice originale.
   ---------------------------------------------------------------------------
*/

    
extern "C"
{


//#ifndef _EiC
#include "cv.h"
#include "ndhighgui.h"
#include <stdio.h>
//#endif

char wndname[] = "Distance transform";
char tbarname[] = "Threshold";
int edge_thresh = 100;

// The output images
IplImage* dist = 0;
IplImage* dist8u1 = 0;
IplImage* dist8u2 = 0;
IplImage* dist8u = 0;
IplImage* dist32s = 0;

IplImage* gray = 0;
IplImage* edge = 0;

// define a trackbar callback
void on_trackbar( int dummy )
{
    dummy;
    
    cvThreshold( gray, edge, (float)edge_thresh, (float)edge_thresh, CV_THRESH_BINARY );
    //Distance transform                  
    cvDistTransform( edge, dist, CV_DIST_L2, CV_DIST_MASK_5, NULL, NULL );

    cvConvertScale( dist, dist, 5000.0, 0 );
    cvPow( dist, dist, 0.5 );
    
    cvConvertScale( dist, dist32s, 1.0, 0.5 );
    cvAndS( dist32s, cvScalarAll(255), dist32s, 0 );
    cvConvertScale( dist32s, dist8u1, 1, 0 );
    cvConvertScale( dist32s, dist32s, -1, 0 );
    cvAddS( dist32s, cvScalarAll(255), dist32s, 0 );
    cvConvertScale( dist32s, dist8u2, 1, 0 );
    cvMerge( dist8u1, dist8u2, dist8u2, 0, dist8u );
    cvShowImage( wndname, dist8u );
}

int ndMain()
{
    char filename [255];
    
    cvInitSystem (0, 0);
    
    strcpy (filename, "ms0:/DEMOPACK/CVDEMO4/stuff.jpg");
    
    if( (gray = cvLoadImage( filename, 0 )) == 0 )
        return -1;
    
    // Create the output image
    dist = cvCreateImage( cvSize(gray->width,gray->height), IPL_DEPTH_32F, 1 );
    dist8u1 = cvCloneImage( gray );
    dist8u2 = cvCloneImage( gray );
    dist8u = cvCreateImage( cvSize(gray->width,gray->height), IPL_DEPTH_8U, 3 );
    dist32s = cvCreateImage( cvSize(gray->width,gray->height), IPL_DEPTH_32S, 1 );

    // Convert to grayscale
    edge = cvCloneImage( gray );

    // Create a window
    cvNamedWindow( wndname, 1 );

    // create a toolbar 
    cvCreateTrackbar( tbarname, wndname, &edge_thresh, 255, on_trackbar );

    // Show the image
    on_trackbar(0);

    // Wait for a key stroke; the same function arranges events processing
    cvWaitKey(0);

    cvReleaseImage( &gray );
    cvReleaseImage( &edge );
    cvReleaseImage( &dist );
    cvReleaseImage( &dist8u );
    cvReleaseImage( &dist8u1 );
    cvReleaseImage( &dist8u2 );
    cvReleaseImage( &dist32s );
    
    cvDestroyWindow( wndname );
    
    return 0;
    
    
}



}






