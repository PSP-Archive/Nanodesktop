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
     FILE HEADERS $$_HGUI_VARIABLES
    --------------------------------------------------------------------------------------   
    Questo file può essere richiamato da diversi altri files .h in diverse occasioni:
           a) in fase di compilazione delle applicazioni, viene richiamato da ndHighGUI.h 
              in modo che possa fornire i prototipi delle strutture e le variabili
              pubbliche di ndHighGUI. Tutte queste variabili vengono definite come
              extern.
           b) in fase di compilazione dei moduli ndHGUI, viene richiamato da 
              $_NDHGUIforNDHGUI.h in modo che i prototipi delle strutture e le 
              variabili siano resi disponibili. 
              In questo caso, le variabili vengono definite come extern, salvo che
              in una circostanza: ndHGUI_cvInitSystem.c richiama  $_NDHGUIforNDHGUI.h 
              settando il simbolo NDHGUI_DEFINE_ALL_VARIABLES. In tal modo, le
              variabili saranno definite realmente e non solo per mezzo del
              riferimento extern
    ---------------------------------------------------------------------------------------  
*/

#ifndef _NDHGUI_VARIABLES_CONTAINER_
        #define _NDHGUI_VARIABLES_CONTAINER_
        
 
                    /* 
                       ----------------------------------------------------------------------------
                       NDHIGHGUI EXTERN
                       ----------------------------------------------------------------------------
                    */
                    
                       #ifndef NDHGUI_DEFINE_ALL_VARIABLES
                              #define NDHGUI_EXTERN extern
                       #else
                              #define NDHGUI_EXTERN
                       #endif  
                    
                    /* 
                       ----------------------------------------------------------------------------
                       EXPLICIT CONSTANTS
                       ----------------------------------------------------------------------------
                    */
                    
                    #define CV_CVTIMG_FLIP                         1
                    #define CV_CVTIMG_SWAP_RB                      2 
                    
                    #define __HIGHGUIERRORBOX_LENX               410
                    #define __HIGHGUIERRORBOX_LENY               200
                    
                    #define BIT63                 0x8000000000000000 
                    #define BIT40                      0x10000000000
                    #define BIT17                            0x20000
                    #define BIT16                            0x10000
                    #define BIT15                             0x8000
                    
                    #define CV_MENUSUPPORTED                   BIT17
                    #define CV_USE_NDWSWND                     BIT16
                    #define CV_FORCE                           BIT15
                    
                    // Simboli definiti per compatibilità OpenCV
                    
                    #define CV_WINDOW_AUTOSIZE            0
                    #define true                          1
                    #define false                         0
                    
                    /*
                       ----------------------------------------------------------------------------
                       NDHIGHGUI ERROR CODES
                       ----------------------------------------------------------------------------
                    */
                    
                    // Codici messaggio
                    
                    #define NDHGUI_MSG_WINDOW_NOT_FOUND           -1
                    #define NDHGUI_MSG_IMAGE_NOT_ALLOCATED        -2
                    #define NDHGUI_MSG_WRONG_COLOR_DEPTH          -3
                    #define NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS    -4
                    
                    // Codici di errore 
                    
                    NDHGUI_EXTERN int  HGUI_SystemError;
                    NDHGUI_EXTERN char HGUI_SystemErrorNotify_Enabled;
                    
                    #define NDHGUI_WRONG_WINDOW_DEFAULT_LENXY      -1
                    #define NDHGUI_WINDOW_NOT_FOUND                -2
                    #define NDHGUI_WRONG_POS_COORDINATES           -3
                    #define NDHGUI_ERROR_IN_POSITION_GENERATOR     -4
                    #define NDHGUI_UNKNOWN_ERROR                   -5
                    #define NDHGUI_MEMORY_ALLOCATION_ERROR         -6
                    #define NDHGUI_NO_WINDOW_IS_AVAILABLE          -7
                    #define NDHGUI_I_CANNOT_RESIZE_WINDOW          -8
                    #define NDHGUI_I_CANNOT_MOVE_WINDOW            -9
                    #define NDHGUI_I_CANNOT_MAXIMIZE_WINDOW       -10 
                    #define NDHGUI_I_CANNOT_MINIMIZE_WINDOW       -11 
                    #define NDHGUI_TRACKBAR_IN_NDWSWND            -12
                    #define NDHGUI_NO_TRACKBAR_HANDLE_AVAILABLE   -13
                    #define NDHGUI_SAME_TRACKBAR_NAME_IS_PRESENT  -14
                    #define NDHGUI_TRACKBAR_NOT_FOUND             -15
                    #define NDHGUI_OUT_OF_BOUND                   -16
                    #define NDHGUI_ERR_ND_TBAR_SETVALUE           -17
                    #define NDHGUI_THE_IMAGE_MUST_HAVE_SAME_DIMS  -18
                    #define NDHGUI_UNKNOWN_ERROR_IMG_SRC          -19
                    #define NDHGUI_WRONG_NRCHANNELS_IMG_SRC       -20
                    #define NDHGUI_WRONG_COLOR_DEPTH_IMG_SRC      -21
                    #define NDHGUI_IMAGE_SRC_NOT_INITIALIZED      -22
                    #define NDHGUI_UNKNOWN_ERROR_IMG_DST          -23
                    #define NDHGUI_WRONG_NRCHANNELS_IMG_DST       -24
                    #define NDHGUI_WRONG_COLOR_DEPTH_IMG_DST      -25
                    #define NDHGUI_IMAGE_DST_NOT_INITIALIZED      -26
                    #define NDHGUI_FILE_NOT_FOUND                 -27
                    #define NDHGUI_FORMAT_NOT_SUPPORTED           -28 
                    #define NDHGUI_IO_ERROR                       -29
                    #define NDHGUI_IMAGE_NOT_INITIALIZED          -30
                    #define NDHGUI_OUT_OF_MEMORY                  -31
                    #define NDHGUI_CANNOT_CREATE_FILE             -32
                    #define NDHGUI_UNKNOWN_ERROR_IN_FILE_WRITING  -33
                    #define NDHGUI_WRONG_COLOR_DEPTH              -34
                    #define NDHGUI_WRONG_NR_CHANNELS              -35
                    #define NDHGUI_PIXEL_OUT_OF_IMAGE             -36
                    #define NDHGUI_I_CANNOT_DO_WND_AUTOALLOCATION -37
                    #define NDHGUI_I_CANNOT_DO_WINDOW_ALLOCATION  -38
                    #define NDHGUI_ERR_IN_NDTRACKBAR_ALLOCATION   -39
                    #define NDHGUI_I_CANNOT_DRAW_ALL_TRACKBARS    -40
                    #define NDHGUI_NDIMAGE_NOT_INITIALIZED        -41
                    #define NDHGUI_CAMERA_IS_NOT_AVAILABLE        -42
                    #define NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO -43
                    #define NDHGUI_I_CANNOT_EXEC_NULL_FLASH       -44  
                    #define NDHGUI_I_CANNOT_CHANGE_RESOLUTION     -45
                    #define NDHGUI_I_CANNOT_GRAB_THE_IMAGE        -46
                    #define NDHGUI_I_CANNOT_CHANGE_CCD_MODE       -47
                    #define NDHGUI_I_CANNOT_CHANGE_CCD_RESOLUTION -48
                    
                    
                    /* 
                       ----------------------------------------------------------------------------
                       TYPES DECLARATIONS
                       ----------------------------------------------------------------------------
                    */
                    
                    #ifdef NDHGUI_DEFINE_ALL_VARIABLES
                           char HGUI_ColorSeq [4] = {'R', 'G', 'B', 'A'};
                    #else
                           NDHGUI_EXTERN char HGUI_ColorSeq [4];
                    #endif

                    // Pseudoregistri HighGUI

                    NDHGUI_EXTERN int      HGUI_ColorValue          [4];
                    NDHGUI_EXTERN float    HGUI_FlColorValue        [4];
                    NDHGUI_EXTERN int      HGUI_ColorValue_Dst      [4];
                    NDHGUI_EXTERN float    HGUI_FlColorValue_Dst    [4];

                    NDHGUI_EXTERN int      HGUI_AdrDataImage, HGUI_AdrDataImage_Dst; 
                    NDHGUI_EXTERN int      HGUI_SizeValue, HGUI_SizeValue_Dst;
                    NDHGUI_EXTERN int      HGUI_NrChannels, HGUI_NrChannels_Dst; 
                    NDHGUI_EXTERN int      HGUI_ColorDepth, HGUI_ColorDepth_Dst;
                    NDHGUI_EXTERN int      HGUI_WidthStep, HGUI_WidthStep_Dst;
                    NDHGUI_EXTERN int      HGUI_DestValue [4], HGUI_DestValue_Dst [4];
                     

                    // struct HGUIPixelType: questa struttura viene normalmente usata per contenere le informazioni su un
                    // singolo pixel dello schermo. 
                    
                    struct HGUIPixelType
                    {
                       unsigned short PosX, PosY;   // Coordinate del punto
                       
                       unsigned char NrChannels;    // Posto a 1 per immagini a tono di grigio,
                                                    // posto a 3 per immagini a colori RGB   
                       
                       int ColorDepth;              // Posto a 8,16,32 a seconda della profondità di colore    
                       
                       float C1;                    // Per immagini RGB, si ha sempre la convenzione
                       float C2;                    // C1=R, C2=G, C3=B. 
                       float C3;             
                                                    // Per immagini a toni di grigio, il campo C1 contiene
                                                    // il valore del colore, mentre C2 e C3 sono posti a 0.                                                                
                    };
                    
                    struct cvTrackBarData_Type 
                    {
                       char  IsActive;
                       char  TBName [32];
                       char  NDHandle;
                       int*  PointerToValue;
                       int   Value;
                       int   MaxValue;
                       int   UserCallBack;
                    };
                    
                    struct cvWindowData_Type
                    {
                       unsigned short PictX1, PictY1, PictX2, PictY2;
                       unsigned short PictLenX, PictLenY;
                       unsigned short OLD_cvLenX, OLD_cvLenY;
                       
                       int   RenderScaledLenX, RenderScaledLenY;
                       float RenderRatioX,     RenderRatioY;
                       int   RenderShowX1,     RenderShowY1;
                       
                       char CanUseNanodesktopWSWnd, UseForce, WndStateIsChanged;
                       
                       IplImage *LastIplImage;
                       
                       char NrcvTrackBarsAllocated;
                       struct cvTrackBarData_Type cvTrackBarData [__NRTRACKBARSALLOCABLE];
                    };
                    
                    struct cvUserArea_Type
                    {
                       char   Label [7];
                       struct cvWindowData_Type *cvWindowData;
                    };
                    
                    typedef void (CV_CDECL *CvTrackbarCallback)(int pos);
                    
                    
                    /*
                      ----------------------------------------------------------------------------
                       VARIABLES FOR DEFAULT PARAMETERS IN WINDOWS CREATION
                      ----------------------------------------------------------------------------
                    */
                    
                    
                    NDHGUI_EXTERN     unsigned short int HGUI_DefaultLenX, HGUI_DefaultLenY;
                    
                    NDHGUI_EXTERN     TypeColor          HGUI_DefWnd_ColorTitle,  HGUI_DefWnd_ColorBGTitle,
                                                         HGUI_DefWnd_ColorBorder, HGUI_DefWnd_ColorBGWS;
                                       
                    NDHGUI_EXTERN     TypeColor          HGUI_NextWnd_ColorTitle,  HGUI_NextWnd_ColorBGTitle,
                                                         HGUI_NextWnd_ColorBorder, HGUI_NextWnd_ColorBGWS;
                    
                    NDHGUI_EXTERN     char               NoDefaultColor;
                    
                    /*
                      ----------------------------------------------------------------------------
                       VARIABLES FOR WEBCAM SUPPORT
                      ----------------------------------------------------------------------------
                    */
                    
                    struct StructCvCapture 
                    {
                           int           CamHandle;
                           int           CamSizeX;
                           int           CamSizeY;
                           
                           char          cvGrabFrameStatus;
                           IplImage      *GrabbedFrame;
                           
                    };
                    
                    typedef struct StructCvCapture CvCapture;
                    
                    #define CV_CAP_PROP_TRASMISSION_MODE     1
                    #define CV_CAP_PROP_POS_MSEC             2
                    #define CV_CAP_PROP_POS_FRAMES           3
                    #define CV_CAP_PROP_POS_AVI_RATIO        4
                    #define CV_CAP_PROP_FRAME_WIDTH          5
                    #define CV_CAP_PROP_FRAME_HEIGHT         6
                    #define CV_CAP_PROP_FPS                  7
                    #define CV_CAP_PROP_FOURCC               8
                    #define CV_CAP_SENSOR_MODE               9
                    #define CV_CAP_HW_RESOLUTION            10
                    #define CV_CAP_HW_EVLEVEL               11
                    
                    /* 
                       ----------------------------------------------------------------------------
                       VARIABLES FOR ndHighGUI INITIALIZATION
                       ----------------------------------------------------------------------------
                    */
                    
                    #ifdef NDHGUI_DEFINE_ALL_VARIABLES
                           int  cvInitSystemCalled = 0;   // Posto ad 1 quando viene richiamata la prima volta cvInitSystem.    
                    #else
                           extern int cvInitSystemCalled;
                    #endif
                    
                    
                    
                    
        
#endif   // #ifndef _NDHGUI_VARIABLES_CONTAINER_
