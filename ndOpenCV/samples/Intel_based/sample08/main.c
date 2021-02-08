/* 
   ----------------------------------------------------------------------------
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


#define ND_DISABLE_NETWORK_SERVICES

#include <cv.h>
#include <ndhighgui.h>




static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;



void detect_and_draw( IplImage* image );


const char* cascade_name =
    "ms0:/DEMOPACK/CVDEMO8/haarcascade_frontalface_alt2.xml";





void detect_and_draw( IplImage* img )
{
  
  
    int scale = 1;
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );
    CvPoint pt1, pt2;
    int i;
    
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
    
    storage = cvCreateMemStorage(0);
    
    //cvPyrDown( img, temp, CV_GAUSSIAN_5x5 );
    cvClearMemStorage( storage );

    if( cascade )
    {
        CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                            1.2, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );
                                            
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            pt1.x = r->x*scale;
            pt2.x = (r->x+r->width)*scale;
            pt1.y = r->y*scale;
            pt2.y = (r->y+r->height)*scale;
            cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
        }
    }

    cvShowImage ( "result", img );
    cvShowImage ( "result", img );
    cvReleaseImage( &temp );
    
    
}



int ndMain()
{
    cvInitSystem (0, 0);
    
    
    CvCapture* capture = 0;
    IplImage *frame, *frame_copy = 0;
    const char* input_name;
    
    cvNamedWindow( "result", 1 );

        char filename [254];
        
        strcpy (filename, "ms0:/DEMOPACK/CVDEMO8/lena.bmp");
        IplImage* image = cvLoadImage( filename, 1 );

        if( image )
        {
            detect_and_draw( image );
            cvWaitKey(0);
            cvReleaseImage( &image );
        }
        
    cvDestroyWindow("result");

}





    

}









