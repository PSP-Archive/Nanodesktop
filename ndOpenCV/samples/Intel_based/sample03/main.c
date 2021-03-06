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
   
   Il nome della suddetta societ� viene qui citato esclusivamente allo scopo 
   di informare l'utilizzatore della provenienza del codice originale, 
   conformemente a quanto previsto dall'art. 1 della licenza BSD che accompagna 
   le librerie OpenCV originali.
   
   In nessun caso, questa nota dovr� essere considerata un tentativo da 
   parte degli sviluppatori di utilizzare il nome della suddetta societ�
   a scopo pubblicitario, in violazione dell'art. 3 della licenza che
   accompagna il codice originale.
   ---------------------------------------------------------------------------
*/

    
extern "C"
{

#include "cv.h"
#include "ndhighgui.h"
#include <stdlib.h>


#define ARRAY  1

int ndMain( int argc, char** argv )
{
    IplImage* img = cvCreateImage( cvSize( 500, 500 ), 8, 3 );
#if !ARRAY        
    CvMemStorage* storage = cvCreateMemStorage(0);
#endif

    cvNamedWindow( "hull", 1 );
        
    for(;;)
    {
        int key;
        int i, count = rand()%100 + 1, hullcount;
        CvPoint pt0;
#if !ARRAY            
        CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),
                                     sizeof(CvPoint), storage );
        CvSeq* hull;
    
        for( i = 0; i < count; i++ )
        {
            pt0.x = rand() % (img->width/2) + img->width/4;
            pt0.y = rand() % (img->height/2) + img->height/4;
            cvSeqPush( ptseq, &pt0 );
        }
        hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
        hullcount = hull->total;
#else
        CvPoint* points = (CvPoint*)malloc( count * sizeof(points[0]));
        int* hull = (int*)malloc( count * sizeof(hull[0]));
        CvMat pointMat = cvMat( 1, count, CV_32SC2, points );
        CvMat hullMat = cvMat( 1, count, CV_32SC1, hull );

        for( i = 0; i < count; i++ )
        {
            pt0.x = rand() % (img->width/2) + img->width/4;
            pt0.y = rand() % (img->height/2) + img->height/4;
            points[i] = pt0;
        }
        cvConvexHull2( &pointMat, &hullMat, CV_CLOCKWISE, 0 );
        hullcount = hullMat.cols;
#endif
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

#if !ARRAY            
        pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
#else
        pt0 = points[hull[hullcount-1]];
#endif

        for( i = 0; i < hullcount; i++ )
        {
#if !ARRAY                
            CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
#else
            CvPoint pt = points[hull[i]];
#endif
            cvLine( img, pt0, pt, CV_RGB( 0, 255, 0 ), 1, CV_AA, 0 );
            pt0 = pt;
        }

        cvShowImage( "hull", img );

        cvWaitKey(0);
        if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
            break;

#if !ARRAY
        cvClearMemStorage( storage );
#else
        free( points );
        free( hull );
#endif
    }
    
    cvDestroyWindow( "hull" );
    return 0;
}



}






