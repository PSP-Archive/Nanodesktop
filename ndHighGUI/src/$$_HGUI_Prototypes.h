/* 
    ----------------------------------------------------------------------------
    NanoDesktop HighGUI (ndHighGUI) Library - Version 0.2
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    This is the porting of HighGUI (the component of Intel (R) OpenCV Libraries
    that allows use of windows environment), for Nanodesktop windows manager.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
        
    By downloading, copying, installing or using the software you agree to this license.
    If you do not agree to this license, do not download, install,
    copy or use the software.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    
      * Redistribution's of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
    
      * Redistribution's in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
    
      * The name of the Visilab research center may not be used to endorse or promote products
        derived from this software without specific prior written permission.
    
    This software is provided by the copyright holders and contributors "as is" and
    any express or implied warranties, including, but not limited to, the implied
    warranties of merchantability and fitness for a particular purpose are disclaimed.
    In no event shall the author of the software, the Visilab Research Center or 
    contributors be liable for any direct, indirect, incidental, special, exemplary, 
    or consequential damages (including, but not limited to, procurement of substitute 
    goods or services; loss of use, data, or profits; or business interruption) 
    however caused and on any theory of liability, whether in contract, strict liability,
    or tort (including negligence or otherwise) arising in any way out of
    the use of this software, even if advised of the possibility of such damage.
    --------------------------------------------------------------------------------------
     FILE HEADERS $$_HGUI_PROTOTYPES
    --------------------------------------------------------------------------------------   
    Questo file può essere richiamato da diversi altri files .h in diverse occasioni:
           a) in fase di compilazione delle applicazioni, viene richiamato da ndHighGUI.h 
              in modo che possa fornire i prototipi delle funzioni: non
              vengono forniti i prototipi delle routines INTERNAL
           b) in fase di compilazione dei moduli ndHGUI, viene richiamato da 
              $_NDHGUIforNDHGUI.h in modo che i prototipi delle funzioni siano 
              resi disponibili. Qui viene usato il simbolo NDHGUI_I_NEED_ALL_PROTOTYPES,
              in modo che i moduli ndHGUI possano accedere ai prototipi di tutte le routines
              della libreria.
    ---------------------------------------------------------------------------------------  
*/

#ifndef _NDHGUI_PROTOTYPES_CONTAINER_
        #define _NDHGUI_PROTOTYPES_CONTAINER_
        
                  
                  /*
                    ------------------------------------------------------------------------------
                    RIFERIMENTI A PROTOTIPI NANODESKTOP
                    ------------------------------------------------------------------------------
                  */
                    
                    #ifdef NDHGUI_I_NEED_ALL_PROTOTYPES
                           extern void ndWS_PutPixel_NORENDER_FAST (short unsigned int, short unsigned int, TypeColor, unsigned char);
                    
                           // E' un prototipo interno di Nanodesktop ma non è accessibile da Nanodesktop.h
                           // quindi viene riportato qui
                    #endif
                    
                  
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES CHE OPERANO SUGLI PSEUDO-REGISTRI HGUI
                    ------------------------------------------------------------------------------
                  */
              
                   #ifdef NDHGUI_I_NEED_ALL_PROTOTYPES
                      inline char INTERNAL_SetupHGUIRegisters (IplImage *MyIPLImage);
                      inline char INTERNAL_SetupHGUIDstRegisters (IplImage *MyIPLImage);
                      
                      inline void INTERNAL_GetPixelFromCurrentImage (int PosX, int PosY);
                      inline void INTERNAL_PutPixelToCurrentImage (int PosX, int PosY);
                      inline TypeColor INTERNAL_CurrentRGBToMagicNumber ();
                      inline void INTERNAL_ConvertCurrentPixel (void);
                   #endif
              
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES PER CONTROLLO IMPOSTAZIONI HIGHGUI
                    ------------------------------------------------------------------------------
                  */
                
                    #ifdef NDHGUI_I_NEED_ALL_PROTOTYPES
                       extern void INTERNAL_ProvideColor (TypeColor *ColorTitle, TypeColor *ColorBGTitle, TypeColor *ColorBGWS, TypeColor *ColorBorder); 
                    #endif
              
                    extern void cvChangeDefaultWindowDim (unsigned short int NewLenX, unsigned short int NewLenY); 
                    
                    extern void cvSetDefaultColorWnd (TypeColor ColorTitle, TypeColor ColorBGTitle, TypeColor ColorBGWS, TypeColor ColorBorder); 
                    extern void cvSetDefaultColorTitle (TypeColor ColorTitle); 
                    extern void cvSetDefaultColorBGTitle (TypeColor ColorBGTitle); 
                    extern void cvSetDefaultColorBGWS (TypeColor ColorBGWS); 
                    extern void cvSetDefaultColorBorder (TypeColor ColorBorder); 
                    extern void cvSetNextColorWnd (TypeColor ColorTitle, TypeColor ColorBGTitle, TypeColor ColorBGWS, TypeColor ColorBorder); 
                    
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES KEYS A 64 BIT
                    ------------------------------------------------------------------------------
                  */
                  
                    extern ndint64 CVKEY_SETPOS (short unsigned int PosX, short unsigned int PosY); 
                    extern ndint64 CVKEY_SETSIZE (short unsigned int LenX, short unsigned int LenY); 
                    
                  /*
                    ------------------------------------------------------------------------------
                    RICERCA FINESTRE
                    ------------------------------------------------------------------------------
                  */
                    
                  #ifdef NDHGUI_I_NEED_ALL_PROTOTYPES  
                    extern char INTERNAL_FindWndByName (char *NameWindowToSearch);
                  #endif  
                     
                    extern char cvFindWindowByName (char* NameWindowToSearch);

                    extern char cvGetNdWindowHandler (char *name); 
                    extern char *cvGetNdWindowTitle (char WndHandle); 
                    
                  /*
                    ------------------------------------------------------------------------------
                    OPENCV FUNCTIONS
                    ------------------------------------------------------------------------------
                  */
                     
                    extern int cvNamedWindow (char *NameWindow, ndint64 flags); 
                    
                    extern void cvResizeWindow (char *NameWindow, int width, int height); 
                    extern void cvMoveWindow (char *NameWindow, int NewPosX, int NewPosY); 
                    
                    extern void cvDestroyWindow (char *NameWindow); 
                    extern void cvDestroyAllWindows (void); 
                    
                    extern void cvMaximizeWindow (char *Name); 
                    extern void cvMinimizeWindow (char *Name); 
                    
                  /*
                    ------------------------------------------------------------------------------
                    OPENCV TRACKBAR FUNCTIONS
                    ------------------------------------------------------------------------------
                  */
                     
                    #ifdef NDHGUI_I_NEED_ALL_PROTOTYPES
                           extern void INTERNAL_ndHGUI_TrackBar_Manager (float fValue, int TB_InfoField, unsigned char WndHandler); 
                    #endif
                    
                    extern int cvCreateTrackbar (const char *trackbar_name, const char *window_name, int *value, int count, CvTrackbarCallback on_change); 
                    extern int cvGetTrackbarPos (const char *trackbar_name, const char *window_name); 
                    extern void cvSetTrackbarPos (const char *trackbar_name, const char *window_name, int Pos); 
                    extern char cvFindTrackbarByName (const char *trackbar_name, const char *window_name); 
                    extern void cvDestroyTrackbar (const char *trackbar_name, const char *window_name); 
                    extern void cvForceTrackbarRender (const char *window_name); 
                    
                  
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES PIXELS
                    ------------------------------------------------------------------------------
                  */
                   
                    extern TypeColor cvRGBToMagicNumber (struct HGUIPixelType *MyHGUIPixel); 
                    
                    extern void cvConvertPixel (struct HGUIPixelType *Record1, struct HGUIPixelType *Record2); 
                    extern void cvPutPixelToImage (IplImage *MyIPLImage, struct HGUIPixelType *MyHGUIPixel); 
                    extern void cvGetPixelFromImage (IplImage *MyIPLImage, struct HGUIPixelType *MyHGUIPixel); 
                  
                    
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES OPENCV PER GESTIONE IMMAGINI
                    ------------------------------------------------------------------------------
                  */
                    
                    extern void cvShowImage  (char *WindowTitle, IplImage *image); 
                    
                    extern IplImage *cvConvertNDItoIPLImage (struct ndImage_Type *MyImage, int ColorDepth, int NrChannels, int Magn); 
                    extern char ndIMG_ConvertIPLtoNDIImage (struct ndImage_Type *MyImage, IplImage *MyIPLImage, float ScaleX, float ScaleY, char IsColor, char ColorFormat); 
                    
                    extern IplImage *cvLoadImage (char *filename, int iscolor); 
                    extern void cvSaveImage (char *filename, IplImage *image); 
                    extern void cvConvertImage (IplImage *ImageSrc, IplImage *ImageDest, int flags); 
                   
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES OPENCV PER GESTIONE WEBCAM
                    ------------------------------------------------------------------------------
                  */
                  
                    extern CvCapture *cvCaptureFromCAM (int WebCamID);
                    extern int cvGrabFrame (CvCapture *PntToCvCapture);
                    
                    extern int cvSetCaptureProperty (CvCapture *PntToCvCapture, int property_id, double Value);
                    extern int cvGetCaptureProperty (CvCapture *PntToCvCapture, int property_id);
                    
                    extern IplImage *cvRetrieveFrame (CvCapture *PntToCvCapture);
                    extern IplImage *cvQueryFrame (CvCapture *PntToCvCapture);
                    extern void cvReleaseCapture (CvCapture **PntToPntToCvCapture);
                    
                    extern CvCapture *cvCaptureFromFile (char *NameFile);
                  
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES OPENCV PER INTERAZIONE UTENTE
                    ------------------------------------------------------------------------------
                  */
                    
                    extern int cvWaitKey (int Param); 

                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES DI SEGNALAZIONE DEGLI ERRORI
                    ------------------------------------------------------------------------------
                  */
              
                    extern void HGUI_NotifyError (int code, char code2, const char *func_name, const char *err_msg); 
                    extern void HGUI_DisableErrorNotification (void);
                    extern void HGUI_EnableErrorNotification (void);
                  
                  /*
                    ------------------------------------------------------------------------------
                    ROUTINES DI INIZIALIZZAZIONE
                    ------------------------------------------------------------------------------
                  */

                    extern void cvInitSystem (int argc, char **argv); 


        
                
#endif   // #ifndef _NDHGUI_PROTOTYPES_CONTAINER_

