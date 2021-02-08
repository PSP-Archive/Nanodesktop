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

#include <ndhighgui.h>
    
extern "C"
{


#define NUMBER 10
#define DELAY 5
char wndname[] = "Drawing Demo";

CvScalar random_color(CvRNG* rng)
{
    int icolor = cvRandInt(rng);
    return CV_RGB(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

int ndMain(void)
{ 
    cvInitSystem (0, 0); 
     
    int line_type = CV_AA; // change it to 8 to see non-antialiased graphics
    int i;
    CvPoint pt1,pt2;
    double angle;
    CvSize sz;
    CvPoint  ptt[6];
    CvPoint* pt[2];
    int  arr[2];
    CvFont font;
    CvRNG rng;
    int width = 400, height = 220;
    int width3 = width*3, height3 = height*3;
    CvSize text_size;
    int ymin = 0;
    // Load the source image
    IplImage* image = cvCreateImage( cvSize(width,height), 8, 3 );
    IplImage* image2;

    // Create a window
    cvSetNextColorWnd (COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE);
    cvNamedWindow(wndname, 1);
    cvZero( image );
    cvSetNextColorWnd (COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_WHITE);
    cvShowImage(wndname,image);

    rng = cvRNG((unsigned)-1);
    pt[0] = &(ptt[0]);
    pt[1] = &(ptt[3]);

    arr[0] = 3;
    arr[1] = 3;

    for (i = 0; i< NUMBER; i++)
    {
        pt1.x=cvRandInt(&rng) % width3 - width;
        pt1.y=cvRandInt(&rng) % height3 - height;
        pt2.x=cvRandInt(&rng) % width3 - width;
        pt2.y=cvRandInt(&rng) % height3 - height;

        cvLine( image, pt1, pt2, random_color(&rng), cvRandInt(&rng)%10, line_type, 0 );
        cvShowImage(wndname,image);
    }
    
    for (i = 0; i< NUMBER; i++)
    {
        pt1.x=cvRandInt(&rng) % width3 - width;
        pt1.y=cvRandInt(&rng) % height3 - height;
        pt2.x=cvRandInt(&rng) % width3 - width;
        pt2.y=cvRandInt(&rng) % height3 - height;

        cvRectangle( image,pt1, pt2, random_color(&rng), cvRandInt(&rng)%10-1, line_type, 0 );        
        cvShowImage(wndname,image);
    }

    

    for (i = 0; i< NUMBER; i++)
    {
        pt1.x=cvRandInt(&rng) % width3 - width;
        pt1.y=cvRandInt(&rng) % height3 - height;
        sz.width =cvRandInt(&rng)%200;
        sz.height=cvRandInt(&rng)%200;
        angle = (cvRandInt(&rng)%1000)*0.180;

        cvEllipse( image, pt1, sz, angle, angle - 100, angle + 200,
                   random_color(&rng), cvRandInt(&rng)%10-1, line_type, 0 );
    
        cvShowImage(wndname,image);           
    }
    
    

    for (i = 0; i< NUMBER; i++)
    {
        pt[0][0].x=cvRandInt(&rng) % width3 - width;
        pt[0][0].y=cvRandInt(&rng) % height3 - height;
        pt[0][1].x=cvRandInt(&rng) % width3 - width;
        pt[0][1].y=cvRandInt(&rng) % height3 - height;
        pt[0][2].x=cvRandInt(&rng) % width3 - width;
        pt[0][2].y=cvRandInt(&rng) % height3 - height;
        pt[1][0].x=cvRandInt(&rng) % width3 - width;
        pt[1][0].y=cvRandInt(&rng) % height3 - height;
        pt[1][1].x=cvRandInt(&rng) % width3 - width;
        pt[1][1].y=cvRandInt(&rng) % height3 - height;
        pt[1][2].x=cvRandInt(&rng) % width3 - width;
        pt[1][2].y=cvRandInt(&rng) % height3 - height;

        cvPolyLine( image, pt, arr, 2, 1, random_color(&rng), cvRandInt(&rng)%10, line_type, 0 );
        cvShowImage(wndname,image);
    }

    


    for (i = 0; i< NUMBER; i++)
    {
        pt[0][0].x=cvRandInt(&rng) % width3 - width;
        pt[0][0].y=cvRandInt(&rng) % height3 - height;
        pt[0][1].x=cvRandInt(&rng) % width3 - width;
        pt[0][1].y=cvRandInt(&rng) % height3 - height;
        pt[0][2].x=cvRandInt(&rng) % width3 - width;
        pt[0][2].y=cvRandInt(&rng) % height3 - height;
        pt[1][0].x=cvRandInt(&rng) % width3 - width;
        pt[1][0].y=cvRandInt(&rng) % height3 - height;
        pt[1][1].x=cvRandInt(&rng) % width3 - width;
        pt[1][1].y=cvRandInt(&rng) % height3 - height;
        pt[1][2].x=cvRandInt(&rng) % width3 - width;
        pt[1][2].y=cvRandInt(&rng) % height3 - height;

        cvFillPoly( image, pt, arr, 2, random_color(&rng), line_type, 0 );
        cvShowImage(wndname,image);
    }
    
    
    
    for (i = 0; i< NUMBER; i++)
    {
        pt1.x=cvRandInt(&rng) % width3 - width;
        pt1.y=cvRandInt(&rng) % height3 - height;

        cvCircle( image, pt1, cvRandInt(&rng)%300, random_color(&rng),
                  cvRandInt(&rng)%10-1, line_type, 0 );
        
        cvShowImage(wndname,image);
    }
    
    
    
    for (i = 1; i< NUMBER; i++)
    {
        pt1.x=cvRandInt(&rng) % width3 - width;
        pt1.y=cvRandInt(&rng) % height3 - height;

        cvInitFont( &font, cvRandInt(&rng) % 8,
                    (cvRandInt(&rng)%100)*0.05+0.1, (cvRandInt(&rng)%100)*0.05+0.1, 
                    (cvRandInt(&rng)%5)*0.1, cvRound(cvRandInt(&rng)%10), line_type );

        cvPutText( image, "Testing text rendering!", pt1, &font, random_color(&rng));
        cvShowImage(wndname,image);
    }
    
    

    cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 3, 3, 0.0, 5, line_type );

    cvGetTextSize( "OpenCV forever!", &font, &text_size, &ymin );

    pt1.x = (width - text_size.width)/2;
    pt1.y = (height + text_size.height)/2;
    image2 = cvCloneImage(image);

    for( i = 0; i < 10; i++ )
    {
        cvSubS( image2, cvScalarAll(i), image, 0 );
        cvPutText( image, "OpenCV forever!", pt1, &font, CV_RGB(255,i,i));
        
        cvShowImage(wndname,image);
    }
    
    cvWaitKey (0);

    // Wait for a key stroke; the same function arranges events processing
    cvReleaseImage(&image);
    cvReleaseImage(&image2);
    cvDestroyWindow(wndname);

    return 0;



}





}
