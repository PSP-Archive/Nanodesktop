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
#endif

#define ARRAY  1

int ndMain()
{
    IplImage* img = cvCreateImage( cvSize( 500, 500 ), 8, 3 );
#if !ARRAY        
    CvMemStorage* storage = cvCreateMemStorage(0);
#endif

    cvNamedWindow( "rect & circle", 1 );
        
    for(;;)
    {
        int key;
        int i, count = rand()%100 + 1;
        CvPoint pt0, pt;
        CvBox2D box;
        CvPoint2D32f box_vtx[4];
        CvPoint2D32f center;
        CvPoint icenter;
        float radius;
#if !ARRAY            
        CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),
                                     sizeof(CvPoint), storage );
        for( i = 0; i < count; i++ )
        {
            pt0.x = rand() % (img->width/2) + img->width/4;
            pt0.y = rand() % (img->height/2) + img->height/4;
            cvSeqPush( ptseq, &pt0 );
        }
#ifndef _EiC /* unfortunately, here EiC crashes */
        box = cvMinAreaRect2( ptseq, 0 );
#endif
        cvMinEnclosingCircle( ptseq, &center, &radius );
#else
        CvPoint* points = (CvPoint*)malloc( count * sizeof(points[0]));
        CvMat pointMat = cvMat( 1, count, CV_32SC2, points );

        for( i = 0; i < count; i++ )
        {
            pt0.x = rand() % (img->width/2) + img->width/4;
            pt0.y = rand() % (img->height/2) + img->height/4;
            points[i] = pt0;
        }
#ifndef _EiC
        box = cvMinAreaRect2( &pointMat, 0 );
#endif
        cvMinEnclosingCircle( &pointMat, &center, &radius );
#endif
        cvBoxPoints( box, box_vtx );
        cvZero( img );
        for( i = 0; i < count; i++ )
        {
#if !ARRAY                
            pt0 = *CV_GET_SEQ_ELEM( CvPoint, ptseq, i );
#else
            pt0 = points[i];
#endif
            cvCircle( img, pt0, 2, CV_RGB( 255, 0, 0 ), CV_FILLED, CV_AA, 0 );
        }

#ifndef _EiC
        pt0.x = cvRound(box_vtx[3].x);
        pt0.y = cvRound(box_vtx[3].y);
        for( i = 0; i < 4; i++ )
        {
            pt.x = cvRound(box_vtx[i].x);
            pt.y = cvRound(box_vtx[i].y);
            cvLine(img, pt0, pt, CV_RGB(0, 255, 0), 1, CV_AA, 0);
            pt0 = pt;
        }
#endif
        icenter.x = cvRound(center.x);
        icenter.y = cvRound(center.y);
        cvCircle( img, icenter, cvRound(radius), CV_RGB(255, 255, 0), 1, CV_AA, 0 );

        cvShowImage( "rect & circle", img );

        key = cvWaitKey(0);
        if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
            break;

#if !ARRAY
        cvClearMemStorage( storage );
#else
        free( points );
#endif
    }
    
    cvDestroyWindow( "rect & circle" );
    return 0;
}



}






