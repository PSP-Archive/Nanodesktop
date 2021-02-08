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




        #include <nanodesktop.h>
        #include <ndhighgui.h>

#include <stdio.h>
#include <cv.h>

int ndMain()
{
    CvCapture* camera = NULL; 
    IplImage *imgA = NULL; 
    IplImage *grayA = NULL; 
    IplImage *grayA_old = NULL; 
    IplImage *velx = NULL; 
    IplImage *vely = NULL; 
    
    int key; 
    int flag=0; 
    camera=cvCaptureFromCAM(-1);
    
    if(camera==NULL)
    {
        printf("Cannot Open Camera Device!\n");
        return(-1); // ..........
    }
    
    imgA = cvQueryFrame(camera);
    
    grayA =cvCreateImage(cvGetSize(imgA),IPL_DEPTH_8U,1);
    grayA_old=cvCreateImage(cvGetSize(imgA),IPL_DEPTH_8U,1);
    velx =cvCreateImage(cvGetSize(imgA),32,1);
    vely =cvCreateImage(cvGetSize(imgA),32,1);
    
    cvNamedWindow("OpticalFlow",CV_WINDOW_AUTOSIZE);
    
    for(;;)
    {
        imgA = cvQueryFrame(camera); 
        grayA->origin = imgA->origin; 
        
        //grayA_old->origin = imgA->origin; 
        
        cvCvtColor(imgA,grayA,CV_BGR2GRAY); 
        
        if(flag==0)
        {
            flag=1;
            cvCopy(grayA,grayA_old,NULL);
        }
        else
        {
            cvCalcOpticalFlowLK(grayA_old,grayA,cvSize(5,5),velx,vely);
            
            int i,j;
            
            for (i=0;i<imgA->height;i+=20)
            {
                for (j=0;j<imgA->width;j+=20)
                {
                    int dx = (int)cvGetReal2D(velx,i,j);
                    int dy = (int)cvGetReal2D(vely,i,j);
                    cvLine(imgA,cvPoint(j,i),cvPoint(j+dx,i+dy),
                    CV_RGB(255,255,255),1,8,0);
                }
            }
        }
    
        cvShowImage("OpticalFlow",imgA); // ........................
        cvCopy(grayA,grayA_old,NULL); // ..................(old)......
        
        key = cvWaitKey(10); // ........
    
        if(key==0x1b)
        { 
           printf("ESC key Quit\n");
           break;
        } 
    }
    
    cvReleaseCapture(&camera);
    cvDestroyWindow("Show");
    return(0); 
}
