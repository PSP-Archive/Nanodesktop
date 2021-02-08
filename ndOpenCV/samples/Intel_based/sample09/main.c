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


#ifndef _EiC
#include "cv.h"
#include "ndhighgui.h"
#include <stdlib.h>
#include <stdio.h>
#endif

IplImage* src = 0;
IplImage* image = 0;
IplImage* dest = 0;

IplConvKernel* element = 0;
const int element_shape = CV_SHAPE_RECT;

//the address of variable which receives trackbar position update 
int global_pos = 0;

//callback function for slider , implements opening 
void Opening(int pos)   
{
    element = cvCreateStructuringElementEx( pos*2+1, pos*2+1, pos, pos, element_shape, 0 );
    cvErode(src,image,element,1);
    cvDilate(image,dest,element,1);
    cvReleaseStructuringElement(&element);
    cvShowImage("Opening&Closing window",dest);
}   

//callback function for slider , implements closing 
void Closing(int pos)   
{
    element = cvCreateStructuringElementEx( pos*2+1, pos*2+1, pos, pos, element_shape, 0 );
    cvDilate(src,image,element,1);
    cvErode(image,dest,element,1);
    cvReleaseStructuringElement(&element);
    cvShowImage("Opening&Closing window",dest);
}

//callback function for slider , implements erosion 
void Erosion(int pos)   
{
    element = cvCreateStructuringElementEx( pos*2+1, pos*2+1, pos, pos, element_shape, 0 );
    cvErode(src,dest,element,1);
    cvReleaseStructuringElement(&element);
    cvShowImage("Erosion&Dilation window",dest);
}

//callback function for slider , implements dilation
void Dilation(int pos)   
{
    element = cvCreateStructuringElementEx( pos*2+1, pos*2+1, pos, pos, element_shape, 0 );
    cvDilate(src,dest,element,1);
    cvReleaseStructuringElement(&element);
    cvShowImage("Erosion&Dilation window",dest);
}


int ndMain()
{
    char filename [254];
    strcpy (filename, "ms0:/DEMOPACK/CVDEMO9/baboon.jpg");
    
    cvInitSystem (0, 0);
    
    if( (src = cvLoadImage(filename,1)) == 0 )
        return -1;

    image = cvCloneImage(src);
    dest = cvCloneImage(src);

    //create windows for output images
    cvNamedWindow("Opening&Closing window",1);
    cvNamedWindow("Erosion&Dilation window",1);
    
    cvCreateTrackbar("Open","Opening&Closing window",&global_pos,10,Opening);
    cvCreateTrackbar("Close","Opening&Closing window",&global_pos,10,Closing);
    cvCreateTrackbar("Dilate","Erosion&Dilation window",&global_pos,10,Dilation);
    cvCreateTrackbar("Erode","Erosion&Dilation window",&global_pos,10,Erosion);
    

    cvShowImage("Opening&Closing window",src);
    cvShowImage("Erosion&Dilation window",src);

    
    

    cvWaitKey(0);
    //releases handle an dimage data  
    cvReleaseImage(&src);
    cvReleaseImage(&image);
    cvReleaseImage(&dest);
    //destroys windows 
    cvDestroyWindow("Opening&Closing window"); 
    cvDestroyWindow("Erosion&Dilation window"); 

    return 0;
}



}






