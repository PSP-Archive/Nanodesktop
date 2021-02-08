/*  
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ----------------------------------------------------------------------------
    Version for PSP/PSPE
    ----------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ----------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    -----------------------------------------------------------------------------
    DRIVER PER LA GESTIONE DELLA WEBCAM VIA EYESERVER (PSP KSU mode)
    -----------------------------------------------------------------------------
    Questo modulo contiene le routines dell'Eye-Client che consentono l'utilizzo
    di una webcam sotto PSP
    -----------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>


/*
   -----------------------------------------------------------------------------
   Definizione di strutture OpenCV
   -----------------------------------------------------------------------------
*/


// Riportiamo in questo punto la definizione di IplImage. Questo evita di
// dovere includere cxtypes.h dentro il codice di Nanodesktop.h (si
// creerebbe una mutua dipendenza intollerabile)

typedef struct _IplImage
{
    int  nSize;         /* sizeof(IplImage) */
    int  ID;            /* version (=0)*/
    int  nChannels;     /* Most of OpenCV functions support 1,2,3 or 4 channels */
    int  alphaChannel;  /* ignored by OpenCV */
    int  depth;         /* pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S,
                           IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported */
    char colorModel[4]; /* ignored by OpenCV */
    char channelSeq[4]; /* ditto */
    int  dataOrder;     /* 0 - interleaved color channels, 1 - separate color channels.
                           cvCreateImage can only create interleaved images */
    int  origin;        /* 0 - top-left origin,
                           1 - bottom-left origin (Windows bitmaps style) */
    int  align;         /* Alignment of image rows (4 or 8).
                           OpenCV ignores it and uses widthStep instead */
    int  width;         /* image width in pixels */
    int  height;        /* image height in pixels */
    struct _IplROI *roi;/* image ROI. if NULL, the whole image is selected */
    struct _IplImage *maskROI; /* must be NULL */
    void  *imageId;     /* ditto */
    struct _IplTileInfo *tileInfo; /* ditto */
    int  imageSize;     /* image data size in bytes
                           (==image->height*image->widthStep
                           in case of interleaved data)*/
    char *imageData;  /* pointer to aligned image data */
    int  widthStep;   /* size of aligned image row in bytes */
    int  BorderMode[4]; /* ignored by OpenCV */
    int  BorderConst[4]; /* ditto */
    char *imageDataOrigin; /* pointer to very origin of image data
                              (not necessarily aligned) -
                              needed for correct deallocation */
}
IplImage;


typedef struct _IplROI
{
    int  coi; /* 0 - no COI (all channels are selected), 1 - 0th channel is selected ...*/
    int  xOffset;
    int  yOffset;
    int  width;
    int  height;
}
IplROI;


/*
   -----------------------------------------------------------------------------
   Pseudoregistri e variabili interne
   -----------------------------------------------------------------------------
*/


// Cam Registers

static int CAM_DimX;
static int CAM_DimY;
static int CAM_NrChannels;
static int CAM_FrameCounter;
static int CAM_NrTilesX;
static int CAM_NrTilesY;
static int CAM_WndHandle;
static int CAM_TransferMode;
static int CAM_UsbHandle;
static int CAM_TargetPosX;
static int CAM_TargetPosY;
static int CAM_Force;
static int CAM_TilesCounter;
static int CAM_TileInTheBlock;
static int CAM_CurrentTileX;
static int CAM_CurrentTileY;
static int CAM_NextTileX;
static int CAM_NextTileY;
static int CAM_GlobalError;

static int CAM_IplWidthStep;
static int CAM_IplLenX;
static int CAM_IplLenY;
static char *CAM_ImageData;

static struct ndImage_Type *CAM_ndImgTarget;
IplImage *CAM_IplImageTarget;


int CAM_IsTheFirstTimeForCamImage = 1;


   
// Puntatori a funzione che vengono settati dalla routine di grabbing

char (*INTERNAL_PntToValidateParameters)   (int _UsbHandleCam, int _AuxValue, ndint64 _Options);
void (*INTERNAL_PntToProvideToRenderTile)  (void);
void (*INTERNAL_PntToExecuteRender)        (void);


/*
   ----------------------------------------------------------------------------
   Riferimenti a routines definite in seguito
   ----------------------------------------------------------------------------
*/


static void INTERNAL_EYESVR_RegisterNewInfoAboutCameraImages (int _DimImgX, int _DimImgY, int _NrChannels, int _NrFrame, int _NrTilesX, int _NrTilesY, char _TrasmissionMode); 
static char INTERNAL_EYESVR_RequestABlockOfTiles (void); 

static char INTERNAL_EYESVR_CAMtoNULL_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options); 
static void INTERNAL_EYESVR_CAMtoNULL_ProvideToRenderTile (void); 
static void INTERNAL_EYESVR_CAMtoNULL_ExecuteRender (void); 
static char INTERNAL_EYESVR_CAMtoWND_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options); 
static void INTERNAL_EYESVR_CAMtoWND_ProvideToRenderTile (void); 
static void INTERNAL_EYESVR_CAMtoWND_ExecuteRender (void); 
static char INTERNAL_EYESVR_CAMtoBS_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options); 
static void INTERNAL_EYESVR_CAMtoBS_ProvideToRenderTile (void); 
static void INTERNAL_EYESVR_CAMtoBS_ExecuteRender (void); 
static char INTERNAL_EYESVR_CAMtoNDIMG_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options); 
static void INTERNAL_EYESVR_CAMtoNDIMG_ProvideToRenderTile (void); 
static void INTERNAL_EYESVR_CAMtoNDIMG_ExecuteRender (void); 
static char INTERNAL_EYESVR_CAMtoCVIMG_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options); 
static void INTERNAL_EYESVR_CAMtoCVIMG_ProvideToRenderTile (void); 
static void INTERNAL_EYESVR_CAMtoCVIMG_ExecuteRender (void); 

static void INTERNAL_EYESVR_SetupPointers (int Target); 
static void INTERNAL_EYESVR_DeterminateNextTile (void); 
static char INTERNAL_EYESVR_DecideIfYouMustExit (void); 




















/*
   ----------------------------------------------------------------------------
   1. ROUTINES DI SUPPORTO PER ndHAL_GrabNewImage 
   ----------------------------------------------------------------------------
*/


static void INTERNAL_EYESVR_RegisterNewInfoAboutCameraImages (int _DimImgX, int _DimImgY, int _NrChannels, int _NrFrame, int _NrTilesX, int _NrTilesY, char _TrasmissionMode)
/*
    Questa routine provvede a memorizzare nei cam registers le informazioni
    relative all'immagine
*/
{
    if (CAM_IsTheFirstTimeForCamImage)
    {
       CAM_DimX            = _DimImgX;
       CAM_DimY            = _DimImgY;
       CAM_NrChannels      = _NrChannels;
       CAM_FrameCounter    = _NrFrame;
       CAM_NrTilesX        = _NrTilesX;
       CAM_NrTilesY        = _NrTilesY;
       CAM_TransferMode = _TrasmissionMode;
       
       CAM_IsTheFirstTimeForCamImage = 0;                               
    }
} 




static inline char INTERNAL_EYESVR_RequestABlockOfTiles (void)
/*
       Questa routine si occupa di richiedere al sistema un nuovo tile-block.
*/
{                     
       int IOReport;
       
       memset (&UsbWCPacketRq, 0, sizeof (UsbWCPacketRq));
                                            
       UsbWCPacketRq.cmd.magic     = HOSTFS_MAGIC;
       UsbWCPacketRq.cmd.command   = CMD_SEND_ME_A_TILESBLOCK;
       UsbWCPacketRq.cmd.extralen  = 0;
                                  
       UsbWCPacketRq.NrImage       = CAM_FrameCounter;
       UsbWCPacketRq.Value [0]     = CAM_CurrentTileX;              // Nr. ordine X della tile
       UsbWCPacketRq.Value [1]     = CAM_CurrentTileY;              // Nr. ordine Y della tile 
                     
       
       switch (CAM_TransferMode)
       {
              case 24:
              {                         
                   IOReport=(USB_PRX_CommXChg (&UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp24, sizeof (UsbWCPacketResp24), 0, 0, NULL, 0, 2000000));            
                   break;    
              }
              
              case 16:
              {                         
                   IOReport=(USB_PRX_CommXChg (&UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp16, sizeof (UsbWCPacketResp16), 0, 0, NULL, 0, 2000000));            
                   break;    
              }
              
              case 8:
              {                         
                   IOReport=(USB_PRX_CommXChg (&UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp8, sizeof (UsbWCPacketResp8), 0, 0, NULL, 0, 2000000));            
                   break;    
              }
       }  
       
       if (IOReport>=0) return 0;
          else return -1;   
}



/*
   ----------------------------------------------------------------------------
   2. Routines di tipo CAM TO NULL
   ----------------------------------------------------------------------------
   Queste routines si occupano di gestire la webcam quando il target è NULL
   ----------------------------------------------------------------------------
*/

static char INTERNAL_EYESVR_CAMtoNULL_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è NULL. 
*/
{
    // Registra l'handle USB
                 
    CAM_UsbHandle = _UsbHandleCam; 
    return 0; 
}

static inline void INTERNAL_EYESVR_CAMtoNULL_ProvideToRenderTile (void)
/*
   Questa routine non ha funzioni effettive (render delle tiles in NULL).
*/
{  
}

static void INTERNAL_EYESVR_CAMtoNULL_ExecuteRender (void)
/*
   Questa routine non ha funzioni effettive (esecuzione del render in NULL).
*/
{    
}


/*
   ----------------------------------------------------------------------------
   3. Routines di tipo CAM TO WINDOW
   ----------------------------------------------------------------------------
   Queste routines si occupano di gestire la webcam quando il target è una
   finestra di Nanodesktop
   ----------------------------------------------------------------------------
*/

static char INTERNAL_EYESVR_CAMtoWND_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è una finestra. 
    
    Ricordiamo che in questo caso AuxValue rappresenta l'handle della
    finestra in cui è necessario inviare l'immagine che è stata grabbata
    
    La routine provvede anche a registrare i parametri che saranno necessari
    per il funzionamento dei vari tiles renderer
*/
{
    // Registra l'handle USB
    
    CAM_UsbHandle = _UsbHandleCam;
    
    // Adesso registra i parametri TargetPosX e TargetPosY
    
    CAM_TargetPosX = (_Options >> 11) & 0x7FF;
    CAM_TargetPosY = (_Options)       & 0x7FF;
    
    // Adesso verifica se l'opzione ND_FORCECAM è stata richiesta dall'utente
    
    if ((_Options & ND_FORCECAM) == ND_FORCECAM)   CAM_Force = 1;
    else CAM_Force = 0; 
    
    // Registra il wndhandle e verifica che la finestra bersaglio sia già stata
    // allocata
    
    if (WindowData [_AuxValue].WindowIsActive)
    {
    CAM_WndHandle = _AuxValue;
    
    return 0;
    }
    else
    return ERR_WINDOW_IS_NOT_ALLOCATED;  

}





static inline void INTERNAL_EYESVR_CAMtoWND_ProvideToRenderTile (void)
/*
   Questa routine ha il compito di renderizzate una tile in una determinata finestra.
*/
{
   register int CounterX, CounterY;
   register int PixelX, PixelY;
   
   register int _Red, _Green, _Blue, _Luma;
   register TypeColor MagicNumber;
   
   register int LimitTileX1, LimitTileY1; 
   register int LimitTileX2, LimitTileY2;
   
   register TypeColor *PntToMemX, *PntToMemY;  
   register int        StepMemX,   StepMemY;
                               
   static struct   ndPixelElement_Type *PntToPixel;
                               
   
   /*
       La gestione della richiesta è diversa a seconda del TrasmissionMode
       che in quel momento è utilizzato dall'Eyeserver.
   */
   
   switch (CAM_TransferMode)
   {
          
          case 24:     // Modalità di trasmissione RGB 24 bit
          {
                       // Calcoliamo i limiti della tile: se la tile è dentro la finestra, posso
                       // usare PutPixel_NORENDER_FAST per velocizzare la routine
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                       
                       if (  (LimitTileX2            <  WindowData [CAM_WndHandle].WSWndLength) &&
                             (LimitTileY2            <  WindowData [CAM_WndHandle].WSWndHeight) &&
                             (CAM_CurrentTileX       <  CAM_NrTilesX-1)    && 
                             (CAM_CurrentTileY       <  CAM_NrTilesY-1)         )
                       { 
                               // Queste routines di Nanodesktop sono state ottimizzate al massimo in modo
                               // da ridurre al minimo il tempo necessario per l'elaborazione: il
                               // sistema tiene in memoria due puntatori di tipo *TypeColor che hanno
                               // la funzione di indirizzare in memoria i dati necessari. 
                               // Questi puntatori sono incrementati di una certa quantità ad ogni
                               // avanzamento dei due cicli for
                                                               
                               
                               PntToMemY = (WindowData [CAM_WndHandle]._ws + WindowData [CAM_WndHandle].WSWndLength * LimitTileY1);      
                               StepMemY  = (WindowData [CAM_WndHandle].WSWndLength); 
                       
                               for (  CounterY=0; CounterY<WEBCAM_TILE_DIMY; CounterY++, PntToMemY+=StepMemY  )
                               {    
                                       PntToMemX = PntToMemY + LimitTileX1;
                                       StepMemX  = 1;                          // Stiamo usando l'aritmetica dei puntatori
                                       
                                       for (CounterX=0; CounterX<WEBCAM_TILE_DIMX; CounterX++, PntToMemX+=StepMemX)
                                       {
                                           PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                           
                                           _Red   = PntToPixel->Red;
                                           _Green = PntToPixel->Green;
                                           _Blue  = PntToPixel->Blue; 
                                           
                                           // Eseguiamo direttamente il calcolo del MagicNumber, senza passare per
                                           // la routine di conversione: in questo modo eliminiamo l'overhead di
                                           // conversione
                                           
                                           *(PntToMemX) = ((_Blue & 0xF8)<<7) + ((_Green & 0xF8)<<2) + ((_Red & 0xF8)>>3) + 0x8000;
                                       }
                               }
                       }
                       else
                       {
                               register int TPixelX, TPixelY;
                               
                               for (CounterY=0, PixelY=LimitTileY1, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY; CounterY++, PixelY++, TPixelY++)
                               {
                                       for (CounterX=0, PixelX=LimitTileX1, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX; CounterX++, PixelX++, TPixelX++)
                                       {
                                           PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                           
                                           _Red   = PntToPixel->Red;
                                           _Green = PntToPixel->Green;
                                           _Blue  = PntToPixel->Blue; 
                                           
                                           MagicNumber = ((_Blue & 0xF8)<<7) + ((_Green & 0xF8)<<2) + ((_Red & 0xF8)>>3) + 0x8000;
                                           ndWS_PutPixel ( PixelX, PixelY, MagicNumber, CAM_WndHandle, NORENDER );    
                                       }
                               }
                       }
                         
                       break;            
                                               
           }     // End of RGB 24 bit per pixel  
           
           case 16:             // Modalità di trasmissione RGB 16 bit (PSP native)
           {
                       
                       
                       // Calcoliamo i limiti della tile: se la tile è dentro la finestra, posso
                       // usare PutPixel_NORENDER_FAST per velocizzare la routine
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                       
                       if (   (LimitTileX2      < WindowData [CAM_WndHandle].WSWndLength) &&
                              (LimitTileY2      < WindowData [CAM_WndHandle].WSWndHeight) &&
                              (CAM_CurrentTileX < CAM_NrTilesX-1)    && 
                              (CAM_CurrentTileY < CAM_NrTilesY-1)                                    ) 
                       { 
                               // Queste routines di Nanodesktop sono state ottimizzate al massimo in modo
                               // da ridurre al minimo il tempo necessario per l'elaborazione: il
                               // sistema tiene in memoria due puntatori di tipo *TypeColor che hanno
                               // la funzione di indirizzare in memoria i dati necessari. 
                               // Questi puntatori sono incrementati di una certa quantità ad ogni
                               // avanzamento dei due cicli for
                                                                
                               PntToMemY = (WindowData [CAM_WndHandle]._ws + WindowData [CAM_WndHandle].WSWndLength * LimitTileY1);      
                               StepMemY  = (WindowData [CAM_WndHandle].WSWndLength); 
                       
                               for (  CounterY=0; CounterY<WEBCAM_TILE_DIMY; CounterY++, PntToMemY+=StepMemY  )
                               {    
                                       PntToMemX = PntToMemY + LimitTileX1;
                                       StepMemX  = 1;                          // Stiamo usando l'aritmetica dei puntatori
                                       
                                       for (CounterX=0; CounterX<WEBCAM_TILE_DIMX; CounterX++, PntToMemX+=StepMemX)
                                       {
                                           *(PntToMemX) = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                       }
                               }
                               
                       }
                       else
                       {
                               register int TPixelX, TPixelY;
                               
                               for (CounterY=0, PixelY=LimitTileY1, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY; CounterY++, PixelY++, TPixelY++)
                               {
                                       for (CounterX=0, PixelX=LimitTileX1, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX; CounterX++, PixelX++, TPixelX++)
                                       {
                                           ndWS_PutPixel ( PixelX, PixelY, UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX], CAM_WndHandle, NORENDER );    
                                       }
                               }
                       }
                       
                       
                       
                       break;
                                                     
           }     // End of RGB 16 bit per pixel     
           
           
           case 8:     // Modalità di trasmissione B/N 8 bit
           {
                       // Calcoliamo i limiti della tile: se la tile è dentro la finestra, posso
                       // usare PutPixel_NORENDER_FAST per velocizzare la routine
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                       
                       if ( (LimitTileX2      < WindowData [CAM_WndHandle].WSWndLength) &&
                            (LimitTileY2      < WindowData [CAM_WndHandle].WSWndHeight) &&
                            (CAM_CurrentTileX < CAM_NrTilesX-1)    && 
                            (CAM_CurrentTileY < CAM_NrTilesY-1)                                    ) 
                       { 
                               // Queste routines di Nanodesktop sono state ottimizzate al massimo in modo
                               // da ridurre al minimo il tempo necessario per l'elaborazione: il
                               // sistema tiene in memoria due puntatori di tipo *TypeColor che hanno
                               // la funzione di indirizzare in memoria i dati necessari. 
                               // Questi puntatori sono incrementati di una certa quantità ad ogni
                               // avanzamento dei due cicli for
                                                                
                               PntToMemY = (WindowData [CAM_WndHandle]._ws + WindowData [CAM_WndHandle].WSWndLength * LimitTileY1);      
                               StepMemY  = (WindowData [CAM_WndHandle].WSWndLength); 
                       
                               for (  CounterY=0; CounterY<WEBCAM_TILE_DIMY; CounterY++, PntToMemY+=StepMemY  )
                               {    
                                       PntToMemX = PntToMemY + LimitTileX1;
                                       StepMemX  = 1;                          // Stiamo usando l'aritmetica dei puntatori
                                       
                                       for (CounterX=0; CounterX<WEBCAM_TILE_DIMX; CounterX++, PntToMemX+=StepMemX)
                                       {
                                           _Luma        = UsbWCPacketResp8.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                           *(PntToMemX) = ((_Luma & 0xF8)<<7) + ((_Luma & 0xF8)<<2) + ((_Luma & 0xF8)>>3) + 0x8000;
                                       }
                               }
                               
                       }
                       else
                       {
                               register int TPixelX, TPixelY;
                               
                               for (CounterY=0, PixelY=LimitTileY1, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY; CounterY++, PixelY++, TPixelY++)
                               {
                                       for (CounterX=0, PixelX=LimitTileX1, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX; CounterX++, PixelX++, TPixelX++)
                                       {
                                           _Luma        = UsbWCPacketResp8.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                           MagicNumber  = ((_Luma & 0xF8)<<7) + ((_Luma & 0xF8)<<2) + ((_Luma & 0xF8)>>3) + 0x8000;
                                            
                                           ndWS_PutPixel ( PixelX, PixelY, MagicNumber, CAM_WndHandle, NORENDER );    
                                       }
                               }
                       }
                                        
                       break;
           
           }     // End of 8 bit mode  
           
   }       // End of switch 
     
}

static void INTERNAL_EYESVR_CAMtoWND_ExecuteRender (void)
{
    XWindowRender (CAM_WndHandle);    
}

/*
   ----------------------------------------------------------------------------
   4. Routines di tipo CAM TO BASE SCREEN
   ----------------------------------------------------------------------------
   Queste routines si occupano di gestire la webcam quando il target è il
   BaseScreen di Nanodesktop
   ----------------------------------------------------------------------------
*/

static char INTERNAL_EYESVR_CAMtoBS_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è il base screen. 
    
    Ricordiamo che in questo caso AuxValue viene ignorato
    
    La routine provvede anche a registrare i parametri che saranno necessari
    per il funzionamento dei vari tiles renderer
*/
{
    // Registra l'handle USB
    
    CAM_UsbHandle = _UsbHandleCam;
    
    // Adesso registra i parametri TargetPosX e TargetPosY
    
    CAM_TargetPosX = (_Options >> 11) & 0x7FF;
    CAM_TargetPosY = (_Options)       & 0x7FF;
    
    // Adesso verifica se l'opzione ND_FORCECAM è stata richiesta dall'utente
    
    if ((_Options & ND_FORCECAM) == ND_FORCECAM)   CAM_Force = 1;
    else CAM_Force = 0; 
    
    // Tutto ok. Restituisci 0 alla routine chiamante
    
    return 0;  
}





static inline void INTERNAL_EYESVR_CAMtoBS_ProvideToRenderTile (void)
/*
   Questa routine ha il compito di renderizzate una tile all'interno del base-screen.
*/
{
   register int CounterX, CounterY;
   register int PixelX, PixelY;
   
   register int _Red, _Green, _Blue, _Luma;
   register TypeColor MagicNumber;
   
   register int LimitTileX1, LimitTileY1; 
   register int LimitTileX2, LimitTileY2;
                                
   static struct   ndPixelElement_Type *PntToPixel;
                               
   
   /*
       La gestione della richiesta è diversa a seconda del TrasmissionMode
       che in quel momento è utilizzato dall'Eyeserver.
   */
   
   switch (CAM_TransferMode)
   {
          
          case 24:     // Modalità di trasmissione RGB 24 bit
          {
                       // Calcoliamo i limiti della tile: se la tile è dentro il BaseScreen, posso
                       // usare PutPixel_NORENDER_FAST per velocizzare la routine
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1 + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1 + WEBCAM_TILE_DIMY;
                       
                       if (  (LimitTileX2        < Windows_MaxScreenX) &&
                             (LimitTileY2        < Windows_MaxScreenY) &&
                             (CAM_CurrentTileX   < CAM_NrTilesX-1)    && 
                             (CAM_CurrentTileY   < CAM_NrTilesY-1)                 )   
                       { 
                               
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                           
                                           _Red   = PntToPixel->Red;
                                           _Green = PntToPixel->Green;
                                           _Blue  = PntToPixel->Blue; 
                                           
                                           // Eseguiamo direttamente il calcolo del MagicNumber, senza passare per
                                           // la routine di conversione: in questo modo eliminiamo l'overhead di
                                           // conversione
                                           
                                           BaseScreen [PixelY][PixelX] = ((_Blue & 0xF8)<<7) + ((_Green & 0xF8)<<2) + ((_Red & 0xF8)>>3) + 0x8000;
                                       }
                               }
                       }
                       else
                       {
                               register int TPixelX, TPixelY;
                               
                               for (CounterY=0, PixelY=LimitTileY1, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY; CounterY++, PixelY++, TPixelY++)
                               {
                                       for (CounterX=0, PixelX=LimitTileX1, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX; CounterX++, PixelX++, TPixelX++)
                                       {
                                           PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                           
                                           _Red   = PntToPixel->Red;
                                           _Green = PntToPixel->Green;
                                           _Blue  = PntToPixel->Blue; 
                                           
                                           MagicNumber = ((_Blue & 0xF8)<<7) + ((_Green & 0xF8)<<2) + ((_Red & 0xF8)>>3) + 0x8000;
                                           ndBS_PutPixel ( PixelX, PixelY, MagicNumber, NORENDER );    
                                       }
                               }
                       }
                         
                       break;            
                                               
           }     // End of RGB 24 bit per pixel  
           
           case 16:             // Modalità di trasmissione RGB 16 bit (PSP native)
           {
                       // Calcoliamo i limiti della tile: se la tile è dentro il BaseScreen, posso
                       // usare PutPixel_NORENDER_FAST per velocizzare la routine
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                       
                       if ( (LimitTileX2      < Windows_MaxScreenX) &&
                            (LimitTileY2      < Windows_MaxScreenY) &&
                            (CAM_CurrentTileX < CAM_NrTilesX-1) && 
                            (CAM_CurrentTileY < CAM_NrTilesY-1)                 ) 
                       { 
                               
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           BaseScreen [PixelY][PixelX] = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                       }
                               }
                       }
                       else
                       {
                               register int TPixelX, TPixelY;
                               
                               for (CounterY=0, PixelY=LimitTileY1, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY; CounterY++, PixelY++, TPixelY++)
                               {
                                       for (CounterX=0, PixelX=LimitTileX1, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX; CounterX++, PixelX++, TPixelX++)
                                       {
                                           ndBS_PutPixel ( PixelX, PixelY, UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX], NORENDER );    
                                       }
                               }
                       }
                         
                       break;
                                                     
           }     // End of RGB 16 bit per pixel     
           
           
           case 8:             // Modalità di trasmissione B/N 8 bit
           {
                       // Calcoliamo i limiti della tile: se la tile è dentro il BaseScreen, posso
                       // usare PutPixel_NORENDER_FAST per velocizzare la routine
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1 + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1 + WEBCAM_TILE_DIMY;
                       
                       
                       if ( (LimitTileX2         < Windows_MaxScreenX) &&
                            (LimitTileY2         < Windows_MaxScreenY) && 
                            (CAM_CurrentTileX    < CAM_NrTilesX-1)    && 
                            (CAM_CurrentTileY    < CAM_NrTilesY-1)                 ) 
                       { 
                               
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           _Luma       = UsbWCPacketResp8.Pixel [CAM_TileInTheBlock][CounterY][CounterX];
                                           BaseScreen [PixelY][PixelX] = ((_Luma & 0xF8)<<7) + ((_Luma & 0xF8)<<2) + ((_Luma & 0xF8)>>3) + 0x8000;
                                       }
                               }
                       }
                       else
                       {
                               register int TPixelX, TPixelY;
                               
                               for (CounterY=0, PixelY=LimitTileY1, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY;    CounterY++, PixelY++, TPixelY++)
                               {
                                       for (CounterX=0, PixelX=LimitTileX1, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX;    CounterX++, PixelX++, TPixelX++)
                                       {
                                           _Luma       = UsbWCPacketResp8.Pixel [CAM_TileInTheBlock][CounterY][CounterX];
                                           
                                           MagicNumber = ((_Luma & 0xF8)<<7) + ((_Luma & 0xF8)<<2) + ((_Luma & 0xF8)>>3) + 0x8000; 
                                           ndBS_PutPixel ( PixelX, PixelY, MagicNumber, NORENDER );    
                                       }
                               }
                       }
                                                      
                       break;
           
           }     // End of 8 bit mode  
           
   }       // End of switch 
     
}

static void INTERNAL_EYESVR_CAMtoBS_ExecuteRender (void)
{
    BaseScreenRender ();    
}

/*
   ----------------------------------------------------------------------------
   5. Routines di tipo ND_CAM_TO_NDIMAGE_STRUCT
   ----------------------------------------------------------------------------
   Queste routines si occupano di gestire la webcam quando il target è una
   immagine descritta da una struct ndImage
   ----------------------------------------------------------------------------
*/

static char INTERNAL_EYESVR_CAMtoNDIMG_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è una immagine Nanodesktop. 
    
    Ricordiamo che in questo caso AuxValue rappresenta l'indirizzo di
    una struct di tipo ndImage_Type che deve contenere l'immagine
    
    La routine provvede anche a registrare i parametri che saranno necessari
    per il funzionamento dei vari tiles renderer
*/
{ 
    // Registra l'handle USB
    
    CAM_UsbHandle = _UsbHandleCam;
    
    // Adesso registra i parametri TargetPosX e TargetPosY
    
    CAM_TargetPosX = (_Options >> 11) & 0x7FF;
    CAM_TargetPosY = (_Options)       & 0x7FF;
    
    // Quando il target è una immagine l'opzione Force è *SEMPRE* attivata
    CAM_Force=1;
    
    // Registra l'indirizzo della struct ndImage_Type che dovrà contenere
    // l'immagine proveniente dalla videocamera
    
    CAM_ndImgTarget = _AuxValue;
    
    if (CAM_ndImgTarget!=0)
    {
         if (CAM_ndImgTarget->Data!=0)
         {
             // Tutto ok. Restituisci 0 alla routine chiamante
             return 0;                            
         }
         else
             return ERR_IMAGE_IS_NOT_ALLOCATED;   
    } 
    else
    return ERR_IMAGE_IS_NOT_ALLOCATED;     

}





static inline void INTERNAL_EYESVR_CAMtoNDIMG_ProvideToRenderTile (void)
/*
   Questa routine ha il compito di creare le varie tiles all'interno delle immagini.
*/
{
   register int CounterX, CounterY;
   register int PixelX, PixelY;
   register int TPixelX, TPixelY;
   
   register int _Red, _Green, _Blue, _Luma;
   register int LenX, LenY;
   register TypeColor MagicNumber;
   
   register int LimitTileX1, LimitTileY1; 
   register int LimitTileX2, LimitTileY2;
   
   register unsigned char *PointerToMem;
                               
   static struct   ndPixelElement_Type *PntToPixel;
                               
   
   /*
       La gestione della richiesta è diversa a seconda del TrasmissionMode
       che in quel momento è utilizzato dall'Eyeserver.
   */
   
   switch (CAM_TransferMode)
   {
          
          case 24:     // Modalità di trasmissione RGB 24 bit
          {
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                       
                       LenX        = CAM_ndImgTarget->LenX;
                       LenY        = CAM_ndImgTarget->LenY;
                       
                       PointerToMem = (unsigned char *)(CAM_ndImgTarget->Data);
                                      
                       for (CounterY=0, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY;    CounterY++, TPixelY++)
                       {
                               for (CounterX=0, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX;    CounterX++, TPixelX++)
                               {
                                   PixelX = LimitTileX1 + CounterX;
                                   PixelY = LimitTileY1 + CounterY;        
                                   
                                   if ((PixelX<CAM_ndImgTarget->LenX) && (PixelY<CAM_ndImgTarget->LenY))
                                   {
                                            PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                   
                                            _Red   = PntToPixel->Red;
                                            _Green = PntToPixel->Green;
                                            _Blue  = PntToPixel->Blue; 
                                                                  
                                            switch (CAM_ndImgTarget->ColorFormat)
                                            {
                                                  case NDMGKNB:
                                                  {
                                                       *(CAM_ndImgTarget->Data + PixelY*LenX + PixelX) = ((_Blue & 0xF8)<<7) + ((_Green & 0xF8)<<2) + ((_Red & 0xF8)>>3) + 0x8000;
                                                       break;
                                                  }
                        
                                                  case NDRGB:
                                                  {
                                                       
                                                       *(PointerToMem + 3*(PixelY*LenX + PixelX) + 0) = _Red;
                                                       *(PointerToMem + 3*(PixelY*LenX + PixelX) + 1) = _Green;
                                                       *(PointerToMem + 3*(PixelY*LenX + PixelX) + 2) = _Blue;
                                                       break;
                                                  }          
                                            } 
                                   }      
                                   
                               }
                       }
               
                         
                       break;            
                                               
           }     // End of RGB 24 bit per pixel  
           
           case 16:             // Modalità di trasmissione RGB 16 bit (PSP native)
           {
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                       
                       LenX        = CAM_ndImgTarget->LenX;
                       LenY        = CAM_ndImgTarget->LenY;
                       
                       PointerToMem = (unsigned char *)(CAM_ndImgTarget->Data);
                             
                       for (CounterY=0, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY;    CounterY++, TPixelY++)
                       {
                               for (CounterX=0, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX ;    CounterX++, TPixelX++)
                               {
                                        PixelX = LimitTileX1 + CounterX;
                                        PixelY = LimitTileY1 + CounterY;        
                                   
                                        if ((PixelX<CAM_ndImgTarget->LenX) && (PixelY<CAM_ndImgTarget->LenY))
                                        {
                                                switch (CAM_ndImgTarget->ColorFormat)
                                                {
                                                      case NDMGKNB:
                                                      {
                                                           *(CAM_ndImgTarget->Data + PixelY*LenX + PixelX) = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                                           
                                                           break;
                                                      }
                            
                                                      case NDRGB:
                                                      {
                                                           
                                                           MagicNumber = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];       
             
                                                           *(PointerToMem + 3*(PixelY*LenX + PixelX) + 0) = (char)(( (MagicNumber & 0x001F) ) << 3);
                                                           *(PointerToMem + 3*(PixelY*LenX + PixelX) + 1) = (char)(( (MagicNumber & 0x03E0) >> 5) << 3);
                                                           *(PointerToMem + 3*(PixelY*LenX + PixelX) + 2) = (char)(( (MagicNumber & 0x7C00) >> 10) << 3);
                     
                                                           break;
                                                      }          
                                               }
                                               
                                       }
                               }
                       }
               
                         
                       break; 
                                                     
           }     // End of RGB 16 bit per pixel     
           
           
           case 8:             // Modalità di trasmissione B/N 8 bit
           {
                       
                   LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                   LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                   LimitTileX2 = LimitTileX1    + WEBCAM_TILE_DIMX;
                   LimitTileY2 = LimitTileY1    + WEBCAM_TILE_DIMY;
                   
                   LenX        = CAM_ndImgTarget->LenX;
                   LenY        = CAM_ndImgTarget->LenY;
                   
                   PointerToMem = (unsigned char *)(CAM_ndImgTarget->Data);
                          
                   for (CounterY=0, TPixelY=CAM_CurrentTileY*WEBCAM_TILE_DIMY;    CounterY<WEBCAM_TILE_DIMY, TPixelY<CAM_DimY;    CounterY++, TPixelY++)
                   {
                           for (CounterX=0, TPixelX=CAM_CurrentTileX*WEBCAM_TILE_DIMX;    CounterX<WEBCAM_TILE_DIMX, TPixelX<CAM_DimX ;    CounterX++, TPixelX++)
                           {
                                    PixelX = LimitTileX1 + CounterX;
                                    PixelY = LimitTileY1 + CounterY;        
                               
                                    if ((PixelX<CAM_ndImgTarget->LenX) && (PixelY<CAM_ndImgTarget->LenY))
                                    {
                                            _Luma       = UsbWCPacketResp8.Pixel [CAM_TileInTheBlock][CounterY][CounterX];
                                   
                                            switch (CAM_ndImgTarget->ColorFormat)
                                            {
                                                  case NDMGKNB:
                                                  {
                                                       *(CAM_ndImgTarget->Data + PixelY*LenX + PixelX) = ((_Luma & 0xF8)<<7) + ((_Luma & 0xF8)<<2) + ((_Luma & 0xF8)>>3) + 0x8000;
                              
                                                       break;
                                                  }
                        
                                                  case NDRGB:
                                                  {
                                                       
                                                       MagicNumber = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];       
         
                                                       *(PointerToMem + 3*(PixelY*LenX + PixelX) + 0) = _Luma;
                                                       *(PointerToMem + 3*(PixelY*LenX + PixelX) + 1) = _Luma;
                                                       *(PointerToMem + 3*(PixelY*LenX + PixelX) + 2) = _Luma;
                 
                                                       break;
                                                  }          
                                           }
                                           
                                   }
                           }
                   }
                        
                       
           }     // End of 8 bit mode  
           
   }       // End of switch 
     
}

static void INTERNAL_EYESVR_CAMtoNDIMG_ExecuteRender (void)
{
    // Nessuna operazione prevista in questo caso    
}



/*
   ----------------------------------------------------------------------------
   6. Routines di tipo ND_CAM_TO_CVIMAGE
   ----------------------------------------------------------------------------
   Queste routines si occupano di gestire la webcam quando il target è una
   immagine OpenCV
   ----------------------------------------------------------------------------
*/





static char INTERNAL_EYESVR_CAMtoCVIMG_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è una immagine IplImage. 
    
    Ricordiamo che in questo caso AuxValue è l'indirizzo dell'immagine.
    
    La routine provvede anche a registrare i parametri che saranno necessari
    per il funzionamento dei vari tiles renderer
*/
{
    // Registra l'handle USB
    
    CAM_UsbHandle = _UsbHandleCam;
    
    // Adesso registra i parametri TargetPosX e TargetPosY
    
    CAM_TargetPosX = 0;
    CAM_TargetPosY = 0;
    
    // Adesso verifica se l'opzione ND_FORCECAM è stata richiesta dall'utente
    
    if ((_Options & ND_FORCECAM) == ND_FORCECAM)   CAM_Force = 1;
    else CAM_Force = 0; 
    
    // Memorizza l'indirizzo dell'immagine
    
    CAM_IplImageTarget = _AuxValue;
    
    CAM_IplLenX        = CAM_IplImageTarget->width;
    CAM_IplLenY        = CAM_IplImageTarget->height;
    
    CAM_IplWidthStep   = CAM_IplImageTarget->widthStep;
    
    CAM_ImageData      = CAM_IplImageTarget->imageData;
    
    // Tutto ok. Restituisci 0 alla routine chiamante
    
    return 0;  

}





static inline void INTERNAL_EYESVR_CAMtoCVIMG_ProvideToRenderTile (void)
/*
   Questa routine ha il compito di renderizzate una tile all'interno del base-screen.
*/
{
   register int CounterX, CounterY;
   register int PixelX, PixelY;
   
   register TypeColor MagicNumber;
   
   register int LimitTileX1, LimitTileY1; 
   register int LimitTileX2, LimitTileY2;
                               
   static struct   ndPixelElement_Type *PntToPixel;
                               
   register char* CAM_ImageDataY;
   register char* CAM_ImageDataYX;
   register char Luma;                    
   
   /*
       La gestione della richiesta è diversa a seconda del TrasmissionMode
       che in quel momento è utilizzato dall'Eyeserver.
   */
   
   switch (CAM_TransferMode)
   {
          
          case 24:     // Modalità di trasmissione RGB 24 bit
          {
                      
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1 + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1 + WEBCAM_TILE_DIMY;
                       
                       if (  (LimitTileX2        < Windows_MaxScreenX) &&
                             (LimitTileY2        < Windows_MaxScreenY) &&
                             (CAM_CurrentTileX   < CAM_NrTilesX-1)    && 
                             (CAM_CurrentTileY   < CAM_NrTilesY-1)                 )   
                       { 
                               
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       CAM_ImageDataY = CAM_ImageData + PixelY*CAM_IplWidthStep; 
                                       
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                           
                                           CAM_ImageDataYX = CAM_ImageDataY + 3*PixelX;
                                           
                                           *(CAM_ImageDataYX + 0) = PntToPixel->Blue;
                                           *(CAM_ImageDataYX + 1) = PntToPixel->Green;
                                           *(CAM_ImageDataYX + 2) = PntToPixel->Red;
                                       }
                               }
                       }
                       else
                       {
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       CAM_ImageDataY = CAM_ImageData + PixelY*CAM_IplWidthStep; 
                                       
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           PntToPixel = &(UsbWCPacketResp24.Pixel[CAM_TileInTheBlock][CounterY][CounterX]);
                                           
                                           if ((PixelX<CAM_IplLenX) && (PixelY<CAM_IplLenY))
                                           {
                                              
                                              CAM_ImageDataYX = CAM_ImageDataY + 3*PixelX;
                                           
                                              *(CAM_ImageDataYX + 0) = PntToPixel->Blue;
                                              *(CAM_ImageDataYX + 1) = PntToPixel->Green;
                                              *(CAM_ImageDataYX + 2) = PntToPixel->Red;
                                          
                                           }
                                       }
                               }
                       }
                         
                       break;            
                                               
           }     // End of RGB 24 bit per pixel  
           
           case 16:             // Modalità di trasmissione RGB 16 bit (PSP native)
           {
                       
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1 + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1 + WEBCAM_TILE_DIMY;
                       
                       if (  (LimitTileX2        < Windows_MaxScreenX) &&
                             (LimitTileY2        < Windows_MaxScreenY) &&
                             (CAM_CurrentTileX   < CAM_NrTilesX-1)    && 
                             (CAM_CurrentTileY   < CAM_NrTilesY-1)                 )   
                       { 
                               
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       CAM_ImageDataY = CAM_ImageData + PixelY*CAM_IplWidthStep; 
                                       
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           MagicNumber = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                           
                                           CAM_ImageDataYX = CAM_ImageDataY + 3*PixelX;
                                           
                                           *(CAM_ImageDataYX + 2) = (char)(( (MagicNumber & 0x001F) ) << 3);
                                           *(CAM_ImageDataYX + 1) = (char)(( (MagicNumber & 0x03E0) >> 5) << 3);
                                           *(CAM_ImageDataYX + 0) = (char)(( (MagicNumber & 0x7C00) >> 10) << 3);
                                       }
                               }
                       }
                       else
                       {
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       CAM_ImageDataY = CAM_ImageData + PixelY*CAM_IplWidthStep; 
                                       
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           MagicNumber = UsbWCPacketResp16.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                           
                                           if ((PixelX<CAM_IplLenX) && (PixelY<CAM_IplLenY))
                                           {
                                              CAM_ImageDataYX = CAM_ImageDataY + 3*PixelX;
                                           
                                              *(CAM_ImageDataYX + 2) = (char)(( (MagicNumber & 0x001F) ) << 3);
                                              *(CAM_ImageDataYX + 1) = (char)(( (MagicNumber & 0x03E0) >> 5) << 3);
                                              *(CAM_ImageDataYX + 0) = (char)(( (MagicNumber & 0x7C00) >> 10) << 3);
                                           }
                                       }
                               }
                       }
                       
                         
                       break;
                                                     
           }     // End of RGB 16 bit per pixel     
           
           
           case 8:             // Modalità di trasmissione B/N 8 bit
           {
                       LimitTileX1 = CAM_TargetPosX + CAM_CurrentTileX*WEBCAM_TILE_DIMX;
                       LimitTileY1 = CAM_TargetPosY + CAM_CurrentTileY*WEBCAM_TILE_DIMY;
                       LimitTileX2 = LimitTileX1 + WEBCAM_TILE_DIMX;
                       LimitTileY2 = LimitTileY1 + WEBCAM_TILE_DIMY;
                       
                       if (  (LimitTileX2        < Windows_MaxScreenX) &&
                             (LimitTileY2        < Windows_MaxScreenY) &&
                             (CAM_CurrentTileX   < CAM_NrTilesX-1)    && 
                             (CAM_CurrentTileY   < CAM_NrTilesY-1)                 )   
                       { 
                               
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       CAM_ImageDataY = CAM_ImageData + PixelY*CAM_IplWidthStep; 
                                       
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           Luma = UsbWCPacketResp8.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                           
                                           CAM_ImageDataYX = CAM_ImageDataY + PixelX;
                                           *(CAM_ImageDataYX) = (char)(Luma);
                                       }
                               }
                       }
                       else
                       {
                               for (  CounterY=0, PixelY=LimitTileY1; CounterY<WEBCAM_TILE_DIMY; CounterY++, PixelY++)
                               {    
                                       CAM_ImageDataY = CAM_ImageData + PixelY*CAM_IplWidthStep; 
                                       
                                       for (CounterX=0, PixelX=LimitTileX1; CounterX<WEBCAM_TILE_DIMX; CounterX++, PixelX++)
                                       {
                                           Luma = UsbWCPacketResp8.Pixel[CAM_TileInTheBlock][CounterY][CounterX];
                                           
                                           if ((PixelX<CAM_IplLenX) && (PixelY<CAM_IplLenY))
                                           {
                                              CAM_ImageDataYX = CAM_ImageDataY + PixelX;
                                              *(CAM_ImageDataYX) = (char)(Luma);
                                           }
                                       }
                               }
                       }
                       
                                                    
                       break;
           
           }     // End of 8 bit mode  
           
   }       // End of switch 
     
}

static void INTERNAL_EYESVR_CAMtoCVIMG_ExecuteRender (void)
{  
}



/*
   ----------------------------------------------------------------------------
   7. ROUTINES DI SUPPORTO PER ndHAL_GrabNewImage (seconda parte)
   ----------------------------------------------------------------------------
*/

static void INTERNAL_EYESVR_SetupPointers (int Target)
{
   switch (Target)
   {
          case ND_CAM_TO_NULL:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_EYESVR_CAMtoNULL_ValidateParameters);
              INTERNAL_PntToProvideToRenderTile = &(INTERNAL_EYESVR_CAMtoNULL_ProvideToRenderTile);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_EYESVR_CAMtoNULL_ExecuteRender);
              break;
          } 
               
          case ND_CAM_TO_WINDOW:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_EYESVR_CAMtoWND_ValidateParameters);
              INTERNAL_PntToProvideToRenderTile = &(INTERNAL_EYESVR_CAMtoWND_ProvideToRenderTile);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_EYESVR_CAMtoWND_ExecuteRender);
              break;
          } 
          
          case ND_CAM_TO_BASESCREEN:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_EYESVR_CAMtoBS_ValidateParameters);
              INTERNAL_PntToProvideToRenderTile = &(INTERNAL_EYESVR_CAMtoBS_ProvideToRenderTile);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_EYESVR_CAMtoBS_ExecuteRender);
              break; 
          }     
          
          case ND_CAM_TO_NDIMAGE_STRUCT:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_EYESVR_CAMtoNDIMG_ValidateParameters);
              INTERNAL_PntToProvideToRenderTile = &(INTERNAL_EYESVR_CAMtoNDIMG_ProvideToRenderTile);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_EYESVR_CAMtoNDIMG_ExecuteRender);
              break; 
          } 
          
          case ND_CAM_TO_CVIMAGE:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_EYESVR_CAMtoCVIMG_ValidateParameters);
              INTERNAL_PntToProvideToRenderTile = &(INTERNAL_EYESVR_CAMtoCVIMG_ProvideToRenderTile);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_EYESVR_CAMtoCVIMG_ExecuteRender);
              break;  
          }
   } 
}


static inline void INTERNAL_EYESVR_DeterminateNextTile ()
{   
    
   /*
      La gestione della richiesta è differente a seconda del trasmission mode 
      usato dall'Eyeserver
   */
   
   switch (CAM_TransferMode)
   {
          case 24:
          {
                  CAM_NextTileX = UsbWCPacketResp24.NextTileX [CAM_TileInTheBlock];
                  CAM_NextTileY = UsbWCPacketResp24.NextTileY [CAM_TileInTheBlock];  
                  
                  break;
          } 
          
          case 16:
          {
                  CAM_NextTileX = UsbWCPacketResp16.NextTileX [CAM_TileInTheBlock];
                  CAM_NextTileY = UsbWCPacketResp16.NextTileY [CAM_TileInTheBlock];  
                  
                  break;
                   
          } 
          
          case 8:
          {
                  CAM_NextTileX = UsbWCPacketResp8.NextTileX [CAM_TileInTheBlock];
                  CAM_NextTileY = UsbWCPacketResp8.NextTileY [CAM_TileInTheBlock];  
                  
                  break; 
          } 
               
   }
}


static inline char INTERNAL_EYESVR_DecideIfYouMustExit (void)
/*
    Questa routine viene richiamata dalla routine principale GrabNewImage
    per decidere quale deve essere la prossima azione del loop di sistema
    
    La routine restituisce dei valori diversi a seconda della reazione
    che deve avere il sistema in quel punto del loop.
    
    I valori restituiti sono 
      0      se è necessario richiedere una nuova tile nell'ambito dello
             stesso TilesBlock
      1      se è necessario richiedere un nuovo TilesBlock
      2      se è necessario uscire dal doppio loop perchè l'immagine è finita
     -1      in caso di errore logico del server
    
    Inoltre, la routine imposta l'eventuale errore riscontrato nella 
    variabile CAM_GlobalError. 
*/
{
   // Valutiamo il contenuto degli pseudoregistri CAM_NextTile
   
   if ( (CAM_NextTileX>=0) && (CAM_NextTileY>=0) && (CAM_NextTileX<CAM_NrTilesX) && (CAM_NextTileY<CAM_NrTilesY))   
   {
             // Il sistema sta indirizzando una tile successiva: impostiamo i registri CAM_CurrentTile in
             // modo che puntino alla tile successiva
             
             CAM_CurrentTileX = CAM_NextTileX;
             CAM_CurrentTileY = CAM_NextTileY;
             
             // Aumenta di 1 il registro che tiene conto del numero di tiles lette nel blocco
             
             CAM_TileInTheBlock++;
             
             if (CAM_TileInTheBlock<WEBCAM_PACKET_SIZE)
                return 0;                                 // Devi richiedere una nuova tile
             else
                return 1;                                 // Devi richiedere un nuovo tiles block     
             
   }  
   else      // L'immagine è terminata o c'è un errore nell'Eyeserver. Controlliamo.
   {
            if ((CAM_NextTileX==-1) && (CAM_NextTileY==-1))
            {
               CAM_GlobalError               = 0;          // L'immagine è finita
               return 2;
            }
            else                                           // Errore logico nell'Eyeserver
            { 
               CAM_GlobalError       = ERR_LOGICAL_ERROR_IN_EYESERVER;       
               return -1;
            }
  }
                                                                       
}




/*
   -----------------------------------------------------------------------------
   WEBCAM API per l'Eyeserver
   -----------------------------------------------------------------------------
*/


char INTERNAL_EYESVR_ActivateCamera (void)
/*
     Routine specifica per l'Eyeserver. Provvede ad attivarlo usando la sezione
     specifica di inizializzazione USB
*/
{
     static struct usb_device devCamera;
     char UsbHandle;
     
     devCamera.descriptor.idVendor  = EYESERVER_VENDOR_ID;
     devCamera.descriptor.idProduct = EYESERVER_PRODUCT_ID;
        
     UsbHandle = ndHAL_USB_OpenDevice (&devCamera);   // Tenta la connessione con l'Eyeserver
     if (UsbHandle>=0) return UsbHandle;              // Se è riuscito ritorna l'USB handle       
}





char INTERNAL_EYESVR_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow)
/*
    Questa routine provvede a chiedere all'EyeServer l'invio di un'immagine.
    Questa routine è alla base di tutto il sistema di gestione webcam di
    Nanodesktop. 
    
    Il parametro UsbHandleCamera indica l'handle USB della videocamera
    da utilizzare (l'handle USB è quello assegnato da ndHAL_CAM_ActivateCamera
    che deve essere richiamata prima di chiamare questa routine).

    Il significato dei parametri di questa routine, che è esclusivamente per
    uso interno, sono indicati nella documentazione della routine pubblica
    ndHAL_CAM_GrabNewImage nel file ndHAL_WebCam.c
     
    La routine restituisce i seguenti codici di errore:
     
       ERR_WRONG_PARAMETERS             Parametri errati (ad esempio, il parametro Target non
                                        è riconosciuto)           
       ERR_IO_ERROR                     L'handle della webcam è errato, o si riferisce ad un
                                        dispositivo USB che non è una webcam
       ERR_LOGICAL_ERROR_IN_EYESERVER   Non è stato possibile capire il significato
                                        delle informazioni ottenute dell'Eyeserver
       ERR_DATA_INTERRUPTED             Il flusso dei dati è stato interrotto
   
*/
{
    int    ErrRep;
    
    char   ReqSingleTile_DoStop, ReqTilesBlocks_DoStop;
    char   DecisionCode;
    
    // inizio del codice
    
    if ((Target>=0) && (Target<=4))
    {     
            INTERNAL_EYESVR_SetupPointers (Target);
            
            ErrRep = (*INTERNAL_PntToValidateParameters)(UsbHandleCamera, AuxValue, Options);
            
            if (ErrRep==0)            // Non ci sono errori in fase di validazione: il sistema si è anche 
            {                         // accertato che questo dispositivo USB sia associato ad una webcam
                
                memset (&UsbWCPacketRq, 0, sizeof (UsbWCPacketRq));
                memset (&UsbWCPacketResp, 0, sizeof (UsbWCPacketResp));
                
                UsbWCPacketRq.cmd.magic     = HOSTFS_MAGIC;
                UsbWCPacketRq.cmd.command   = CMD_START_NEW_IMAGE;
                UsbWCPacketRq.cmd.extralen  = 0;
                
                // Nel registro Value0 viene memorizzato un codice che informa l'EyeServer
                // delle opzioni richieste, (ad esempio dell'opzione ND_FORCECAM attivata)
                
                if (CAM_Force) UsbWCPacketRq.Value [0] = UsbWCPacketRq.Value [0] | 1;
                
                // Provvede a trasferire ad EyeServer il pacchetto che richiede la richiesta
                // di iniziare la trasmissione dell'immagine
                
                if ( USB_PRX_CommXChg ( &UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp, sizeof(UsbWCPacketResp), 0, 0, NULL, 0, 0) )
                {
                     // Interpretiamo le informazioni che provengono dal server
                     
                     if ( (UsbWCPacketResp.cmd.magic    == HOSTFS_MAGIC) &&    // E' stato possibile eseguire
                          (UsbWCPacketResp.cmd.command  == 0xFFFF) &&          // il grab di un frame
                          (UsbWCPacketResp.cmd.extralen == 0) &&
                          (UsbWCPacketResp.Message      == MSG_OK)  )
                     {
                          // Le informazioni che provengono dal server hanno questo schema:     
                          // Value[0] e Value[1] servono a fornire le dimensioni X ed Y. 
                          // Value[2] fornisce il numero di canali, 
                          // Value[3] contiene il numero di frame generato dal frame counter
                          // Value[4] contiene il numero di tiles in orizzontale
                          // Value[5] contiene il numero di tiles in verticale
                          // Value[6] contiene il trasmission mode usato dal server
        
                          // La prossima routine provvede a registrare i dati relativi all'immagine
        
                          INTERNAL_EYESVR_RegisterNewInfoAboutCameraImages (UsbWCPacketResp.Value [0], UsbWCPacketResp.Value [1], UsbWCPacketResp.Value [2], 
                                                                     UsbWCPacketResp.Value [3], UsbWCPacketResp.Value [4], UsbWCPacketResp.Value [5],
                                                                     UsbWCPacketResp.Value [6]);
                                            
                          // Inizia a richiedere le tiles all'Eyeserver. Il funzionamento è il 
                          // seguente: la prima tile richiesta è sempre quella di coordinate
                          // (0, 0). Ogni pacchetto restituito dall'Eyeserver conterrà poi,
                          // un insieme di altre tiles. Ogni tiles contiene un puntatore alla tile
                          // successiva. Quando l'Eyeserver invia l'ultima tile di un frame,
                          // pone a -1 i campi NextTileX e NextTileY relativi all'ultima tile
                          
                          // Ciò si rende necessario perchè il server esegue in ogni frame
                          // un algoritmo che esclude le zone dell'immagine che non sono
                          // state modificate. In tal modo, il transito dei dati lungo il
                          // cavo USB si riduce notevolmente, così come la potenza di calcolo
                          // necessaria per la PSP. 
                          
                          CAM_CurrentTileX = 0;
                          CAM_CurrentTileY = 0;
                          
                          CAM_GlobalError = 0;
                          
                          CAM_TilesCounter    = 0;
                          CAM_FrameCounter++;
                          
                          ReqTilesBlocks_DoStop=0;
                          
                          while (!ReqTilesBlocks_DoStop)
                          {   
                             // Provvedi a richiedere un nuovo TilesBlock al sistema
                          
                             if ( !INTERNAL_EYESVR_RequestABlockOfTiles () )                 // Non ci sono stati errori di I/O
                             {
                                      
                                      ReqSingleTile_DoStop      =0;
                                      CAM_TileInTheBlock        =0;
                                      
                                      while ( !ReqSingleTile_DoStop )
                                      {
                                              // Iniziamo a renderizzare le tiles        
                                              (*INTERNAL_PntToProvideToRenderTile)();
                                              
                                              // Determina la tile successiva
                                              INTERNAL_EYESVR_DeterminateNextTile ();
                                              
                                              // Decide se deve uscire
                                              DecisionCode = INTERNAL_EYESVR_DecideIfYouMustExit (); 
                                              
                                              // Processiamo il Decision Code
                                              switch (DecisionCode)
                                              {
                                                     case 0:       // Il sistema deve semplicemente proseguire
                                                          break;
                                                     case 1:       // Il sistema deve richiedere un nuovo blocco
                                                          {
                                                                   ReqSingleTile_DoStop=1;
                                                                   break;
                                                          }
                                                     case 2:       // L'immagine è finita. Esci da entrambi i loop
                                                          {
                                                                   ReqSingleTile_DoStop=1;
                                                                   ReqTilesBlocks_DoStop=1;
                                                                   break;
                                                          }
                                                     case -1:      // Codice di errore logico del server
                                                          {
                                                                   ReqSingleTile_DoStop=1;
                                                                   ReqTilesBlocks_DoStop=1;
                                                                   break;
                                                          }
                                              }                    // End of switch (Decision Code)
                                            
                                      }     // End of while di secondo livello
                                      
                                      
                                      //    Quando il sistema arriva in questo punto, tutte le tile
                                      //    di un TilesBlock sono state richieste e quindi si deve
                                      //    caricare un nuovo blocco.
                             }
                             else  // Ci sono stati errori di I/O nel richiedere il tile block
                             {
                                   CAM_GlobalError = ERR_DATA_INTERRUPTED;
                                   ReqTilesBlocks_DoStop = 1;
                             }
                          
                          }
                          
                          // Arrivati a questo punto, il ciclo while di primo livello è concluso e l'immagine
                          // dovrebbe essere tutta letta: esegui eventualmente il rendering
                          
                          if (RenderNow) (*INTERNAL_PntToExecuteRender)(); 
                          
                          // Ritorna l'errore che ha riscontrato
                          
                          return CAM_GlobalError;           
                     }
                     else // C'è stato un problema ad eseguire il grab del frame.
                        return ERR_IO_ERROR;
                     
                }
                else     // Errore nell'invio dei dati al server via USB
                   return ERR_IO_ERROR;
                        
            }
            else                 // Errore durante la fase di validazione dei parametri
               return ErrRep;    // Riporta l'errore di validazione riscontrato           
    }
    else
        return ERR_WRONG_PARAMETERS;
} 
    


char INTERNAL_EYESVR_ChangeResolution (int UsbHandleCam, int NewSizeX, int NewSizeY)
/*
   Questa routine informa l'Eyeserver che è necessario cambiare la risoluzione
   con cui le immagini vengono inviate al client. 
   
   NewSizeX e NewSizeY rappresentano le nuove dimensioni. In uscita viene 
   normalmente restituito 0, se viene restituito un valore negativo siamo
   in presenza di un errore. Sono previsti i seguenti codici di errore:
   
           
      ERR_WRONG_PARAMETERS             Parametri NewSizeX o NewSizeY non corretti  
      
      ERR_IO_ERROR                     Errore di I/O          
*/
{
   if ((NewSizeX>0) && (NewSizeY>0))
   {
            memset (&UsbWCPacketRq,   0, sizeof (UsbWCPacketRq));
            memset (&UsbWCPacketResp, 0, sizeof (UsbWCPacketResp));
            
            UsbWCPacketRq.cmd.magic     = HOSTFS_MAGIC;
            UsbWCPacketRq.cmd.command   = CMD_CHANGE_RESOLUTION;
            UsbWCPacketRq.cmd.extralen  = 0;
            
            // NewSizeX e NewSizeY vengono memorizzati nel pacchetto: il sistema provvederà
            // a passare il comando all'Eyeserver
            
            UsbWCPacketRq.Value [0] = NewSizeX;
            UsbWCPacketRq.Value [1] = NewSizeY;
            
            if ( USB_PRX_CommXChg (&UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp, sizeof(UsbWCPacketResp), 0, 0, NULL, 0, 2000000) )
            {
                 // Comando ricevuto dal server: verifichiamo che la risposta sia corretta
                  
                 if ( (UsbWCPacketResp.cmd.magic    == HOSTFS_MAGIC) &&    
                      (UsbWCPacketResp.cmd.command  == CMD_RESPONSE_TO_CHG_RES) &&          
                      (UsbWCPacketResp.cmd.extralen == 0) &&
                      (UsbWCPacketResp.Message      == MSG_OK)  )
                 {     
                       // Dobbiamo invalidare le vecchie impostazioni di configurazione, perchè
                       // queste dovranno essere aggiornate appena il client richiederà la
                       // nuova immagine
                       
                       CAM_IsTheFirstTimeForCamImage =1;
                       
                       // Ritardo di due secondi per dare il tempo al server di eseguire la
                       // commutazione
                       
                       ndHAL_SoftDelay (2);
                       return 0;
                 }
                 else
                     return ERR_IO_ERROR;
            }
            else
                 return ERR_IO_ERROR;        
   }
   else
     return ERR_WRONG_PARAMETERS;         
  
}
 
 
char INTERNAL_EYESVR_ChangeTransferMode (int UsbHandleCam, int NewMode)
/*
    Questa routine permette di cambiare il transfer mode utilizzato
    dall'EyeServer. 
    
    Il parametro Mode può assumere esclusivamente 3 valori: 8, 16, 24.
    
    Il valore 8 permette di ottenere dall'Eyeserver immagini in toni di
    grigio a 256 livelli, il valore 16 consente di ottenere immagini a
    colori in formato RGB555 (il formato nativo della PSP), mentre il
    valore 24 consente di ottenere immagini RGB standard
    
    L'utente può usare le costanti manifeste ND_RGB_TRASMISSION_MODE,
    ND_RGB555_TRASMISSION_MODE e ND_GRAY_TRASMISSION_MODE in luogo
    delle semplici costanti numeriche. 

    Sono previsti i seguenti codici di errore:
         
      ERR_WRONG_PARAMETERS             Parametri NewMode non corretto 
      
      ERR_IO_ERROR                     Errore di I/O      
*/
{
   if ( (NewMode==ND_RGB_TRASMISSION_MODE) || (NewMode==ND_RGB555_TRASMISSION_MODE) || (NewMode==ND_GRAY_TRASMISSION_MODE) )
   {
            memset (&UsbWCPacketRq,   0, sizeof (UsbWCPacketRq));
            memset (&UsbWCPacketResp, 0, sizeof (UsbWCPacketResp));
            
            UsbWCPacketRq.cmd.magic     = HOSTFS_MAGIC;
            UsbWCPacketRq.cmd.command   = CMD_CHANGE_TRANSFER_MODE;
            UsbWCPacketRq.cmd.extralen  = 0;
            
            // Il nuovo transfer mode viene memorizzato in Value [0]
            
            UsbWCPacketRq.Value [0] = NewMode;
            
            if ( USB_PRX_CommXChg (&UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp, sizeof(UsbWCPacketResp), 0, 0, NULL, 0, 2000000)>0 )
            {
                 // Comando ricevuto dal server: verifichiamo che la risposta sia corretta
                 
                 if ( (UsbWCPacketResp.cmd.magic    == HOSTFS_MAGIC) &&    
                      (UsbWCPacketResp.cmd.command  == CMD_RESPONSE_TO_CHG_TMODE) &&          
                      (UsbWCPacketResp.cmd.extralen == 0) &&
                      (UsbWCPacketResp.Message      == MSG_OK)  )
                 {     
                       // Dobbiamo invalidare le vecchie impostazioni di configurazione, perchè
                       // queste dovranno essere aggiornate appena il client richiederà la
                       // nuova immagine
                       
                       CAM_IsTheFirstTimeForCamImage =1;
                       
                       // Ritardo di due secondi per dare il tempo al server di eseguire la
                       // commutazione
                       
                       ndHAL_Delay (2);
                       
                       return 0;
                 }
                 else
                     return ERR_IO_ERROR;
            }
            else
                 return ERR_IO_ERROR;        
   }
   else
     return ERR_WRONG_PARAMETERS;          
}


char INTERNAL_EYESVR_GetInfo (int UsbHandleCam, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode)
/*
    Questa routine si occupa di prelevare le informazioni sulle immagini attualmente
    visualizzate dall'EyeServer. Attenzione perchè le infomazioni di dimensione
    hanno un significato solo dopo che l'utente ha impostato la dimensione
    dell'immagine o dopo che l'Eyeserver lo ha fatto automaticamente attraverso
    una chiamata a ndHAL_CAM_GrabNewImage.
    
    Se si richiama ndHAL_CAM_GetInfo prima di avere impostato una dimensione
    immagine, il sistema restituirà le dimensioni entrambe a -1
    
    Sono previsti i seguenti codici di errore:
         
      ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
      ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
      ERR_IO_ERROR                     Errore di I/O      
*/
{
    memset (&UsbWCPacketRq,   0, sizeof (UsbWCPacketRq));
    memset (&UsbWCPacketResp, 0, sizeof (UsbWCPacketResp));
    
    UsbWCPacketRq.cmd.magic     = HOSTFS_MAGIC;
    UsbWCPacketRq.cmd.command   = CMD_GET_INFO;
    UsbWCPacketRq.cmd.extralen  = 0;
    
    if ( USB_PRX_CommXChg ( &UsbWCPacketRq, sizeof(UsbWCPacketRq), &UsbWCPacketResp, sizeof(UsbWCPacketResp), 0, 0, NULL, 0, 2000000) )
    {
         // Comando ricevuto dal server: verifichiamo che la risposta sia corretta
         
         if ( (UsbWCPacketResp.cmd.magic    == HOSTFS_MAGIC) &&    
              (UsbWCPacketResp.cmd.command  == CMD_RESPONSE_TO_GET_INFO) &&          
              (UsbWCPacketResp.cmd.extralen == 0) &&
              (UsbWCPacketResp.Message      == MSG_OK)  )
         {     
               // I dati vengono restituiti dall'Eyeserver nel seguente ordine:
               // In Value [0] la DimX
               // In Value [1] la DimY
               // In Value [2] il FrameCounter
               // In Value [3] il TransferMode
               
               *DimImgX      = UsbWCPacketResp.Value[0];
               *DimImgY      = UsbWCPacketResp.Value[1];
               *FrameCounter = UsbWCPacketResp.Value[2];
               *TransferMode = UsbWCPacketResp.Value[3];
               
               return 0;
         }
         else
             return ERR_IO_ERROR;
    }
    else
         return ERR_IO_ERROR;        
   
}

char INTERNAL_EYESVR_ChangeSensorMode (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la modalità di video del
     sensore della videocamera.
     
     Eyeserver non supporta questa funzionalità
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}

char INTERNAL_EYESVR_ChangeHwResolution (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la risoluzione del
     sensore della videocamera. 
     
     La routine non deve essere confusa con INTERNAL_EYESVR_ChangeResolution.
     
     Infatti, la routine INTERNAL_EYESVR_ChangeResolution accetta in
     input qualsiasi risoluzione: Nanodesktop provvederà ad uno scaling
     automatico per ottenere la risoluzione richiesta a partire dalla
     risoluzione del sensore. 
     
     Al contrario, INTERNAL_EYESVR_ChangeHwResolution accetta in ingresso
     solo un numero finito di codici di risoluzione, e setta direttamente
     la risoluzione del sensore
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}


char INTERNAL_EYESVR_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness)
/*
    Consente di settare la luminosità della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED;
}

char INTERNAL_EYESVR_ChangeHwContrast (int UsbHandleCamera, int NewContrast)
/*
    Consente di settare il contrasto della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}

char INTERNAL_EYESVR_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness)
/*
    Consente di settare la nitidezza della videocamera (0-255). 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}

char INTERNAL_EYESVR_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel)
/*
    Consente di settare il livello di esposizione (da -128 a +128) 
*/
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


int INTERNAL_EYESVR_GetSaturationLevel (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


int INTERNAL_EYESVR_GetBrightness (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


int INTERNAL_EYESVR_GetContrast (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}



int INTERNAL_EYESVR_GetSharpness (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}



int INTERNAL_EYESVR_GetHwSensorMode (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}


int INTERNAL_EYESVR_GetEvLevel (int UsbHandleCamera)
{
    return ERR_FUNCTION_NOT_SUPPORTED; 
}

char INTERNAL_EYESVR_ActivateCamMicrophone (int WorkAreaAudioSize, int Frequency, int Gain)
/*
    Questa routine è esclusivamente per uso interno. Essa provvede ad attivare il 
    microfono associato alla webcam. 
    
    Il primo parametro è la dimensione dell'area di lavoro associata al driver
    audio. Tale area di lavoro verrà allocata sul momento a richiesta
    
    Il secondo ed il terzo parametro sono la frequenza di lavoro ed il guadagno del
    microfono. 
    
    Viene restituito il valore 0 se non vi sono errori ed un valore negativo in 
    caso di errore. 
    
    Vengono restituiti i seguenti codici di errore:
        ERR_FUNCTION_NOT_SUPPORTED       Il microfono non è supportato dalla webcam
        ERR_DEVICE_IS_NOT_OPERATIVE      Problema nell'inizializzazione del microfone
        ERR_IO_ERROR                     Errore interno   
*/
{
    /* Nulla da fare, perchè Eyeserver non supporta il microfono */
    return ERR_FUNCTION_NOT_SUPPORTED; 
}
    
    
int INTERNAL_EYESVR_ReadAudioBlock (void *Buffer, int NrBytesToRead)
/*
    Questa routine legge un blocco di dati audio dal microfono
    e lo copia nel buffer *Buffer. 
    
    L'utente deve avere allocato in precedenza una area di memoria 
    opportuna, e ne deve passarne l'indirizzo alla routine 

    La routine restituisce in uscita il numero di bytes letti 
*/
{
    /* Nulla da fare, perchè Eyeserver non supporta il microfono */
    return ERR_FUNCTION_NOT_SUPPORTED;
}    
    
void INTERNAL_EYESVR_DeActivateCamMicrophone (void)
/*
    Provvede a disattivare il microfono associato alla webcam    
*/  
{
    /* Nulla da fare, perchè Eyeserver non supporta il microfono */
}



void INTERNAL_EYESVR_DisableCamera (int UsbHandleCamera)
/*
    Usa la funzione del sottosistema Usb associata per liberare l'handle
    e per scaricare i drivers associati
*/
{
    ndHAL_USB_CloseDevice (UsbHandleCamera);
}

char INTERNAL_EYESVR_StartAudioRecording (char *NameFile, int Frequency, int Gain)
/*
     Questa routine è esclusivamente per uso interno.
     
     Questa routine inizia a registrare il flusso audio proveniente dalla webcam su un
     file di nome NameFile in formato WAV. 
*/
{
    /* Nulla da fare, perchè Eyeserver non supporta il microfono */
    return ERR_FUNCTION_NOT_SUPPORTED;
}


void INTERNAL_EYESVR_StopAudioRecording (void)
/*
    Arresta il processo di registrazione
*/
{
}









