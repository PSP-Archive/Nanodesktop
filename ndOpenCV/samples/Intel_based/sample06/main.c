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
#include <stdio.h>
#include <math.h>
#endif

int ndMain(int argc, char** argv)
{
    const float A[] = { 1, 1, 0, 1 };
    
    IplImage* img = cvCreateImage( cvSize(500,500), 8, 3 );
    CvKalman* kalman = cvCreateKalman( 2, 1, 0 );
    CvMat* state = cvCreateMat( 2, 1, CV_32FC1 ); /* (phi, delta_phi) */
    CvMat* process_noise = cvCreateMat( 2, 1, CV_32FC1 );
    CvMat* measurement = cvCreateMat( 1, 1, CV_32FC1 );
    CvRNG rng = cvRNG(-1);
    int code = -1;

    cvZero( measurement );
    cvNamedWindow( "Kalman", 1 );

    for(;;)
    {
        cvRandArr( &rng, state, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(0.1) );
        
        memcpy( kalman->transition_matrix->data.fl, A, sizeof(A));
        cvSetIdentity( kalman->measurement_matrix, cvRealScalar(1) );
        cvSetIdentity( kalman->process_noise_cov, cvRealScalar(1e-5) );
        cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(1e-1) );
        cvSetIdentity( kalman->error_cov_post, cvRealScalar(1));
        cvRandArr( &rng, kalman->state_post, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(0.1) );
        
        for(;;)
        {
            #define calc_point(angle)                                      \
                cvPoint( cvRound(img->width/2 + img->width/3*cos(angle)),  \
                         cvRound(img->height/2 - img->width/3*sin(angle))) 

            float state_angle = state->data.fl[0];
            CvPoint state_pt = calc_point(state_angle);
            
            const CvMat* prediction = cvKalmanPredict( kalman, 0 );
            float predict_angle = prediction->data.fl[0];
            CvPoint predict_pt = calc_point(predict_angle);
            float measurement_angle;
            CvPoint measurement_pt;

            cvRandArr( &rng, measurement, CV_RAND_NORMAL, cvRealScalar(0),
                       cvRealScalar(sqrt(kalman->measurement_noise_cov->data.fl[0])) );

            /* generate measurement */
            cvMatMulAdd( kalman->measurement_matrix, state, measurement, measurement );

            measurement_angle = measurement->data.fl[0];
            measurement_pt = calc_point(measurement_angle);
            
            /* plot points */
            #define draw_cross( center, color, d )                                 \
                cvLine( img, cvPoint( center.x - d, center.y - d ),                \
                             cvPoint( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
                cvLine( img, cvPoint( center.x + d, center.y - d ),                \
                             cvPoint( center.x - d, center.y + d ), color, 1, CV_AA, 0 )

            cvZero( img );
            draw_cross( state_pt, CV_RGB(255,255,255), 3 );
            draw_cross( measurement_pt, CV_RGB(255,0,0), 3 );
            draw_cross( predict_pt, CV_RGB(0,255,0), 3 );
            cvLine( img, state_pt, measurement_pt, CV_RGB(255,0,0), 3, CV_AA, 0 );
            cvLine( img, state_pt, predict_pt, CV_RGB(255,255,0), 3, CV_AA, 0 );
            
            cvKalmanCorrect( kalman, measurement );

            cvRandArr( &rng, process_noise, CV_RAND_NORMAL, cvRealScalar(0),
                       cvRealScalar(sqrt(kalman->process_noise_cov->data.fl[0])));
            cvMatMulAdd( kalman->transition_matrix, state, process_noise, state );

            cvShowImage( "Kalman", img );
            code = cvWaitKey( 100 );
            
            if( code > 0 )
                break;
        }
        if( code == 27 || code == 'q' || code == 'Q' )
            break;
    }
    
    cvDestroyWindow("Kalman");

    return 0;
}



}






