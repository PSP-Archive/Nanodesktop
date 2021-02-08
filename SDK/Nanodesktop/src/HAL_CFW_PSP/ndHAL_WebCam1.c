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
    DRIVER PER LA GESTIONE DELLA GOCAM/CHOTTO CAM (PSP CFW mode)
    -----------------------------------------------------------------------------
    
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>


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
   STRUTTURA PER LA REGISTRAZIONE
   -----------------------------------------------------------------------------
*/

struct MicRec_WAVHeader_Type
{
	u32		ChunkID;
	u32		ChunkSize;
	u32		Format;
	u32		SubChunk1ID;
	u32		SubChunk1Size;
	u16		AudioFormat;
	u16		NumChannels;
	u32		SampleRate;
	u32		ByteRate;
	u16		BlockAlign;
	u16		BitsPerSample;
	u32		SubChunk2ID;
	u32		SubChunk2Size;
};


/*
   -----------------------------------------------------------------------------
   VARIABILI INTERNE
   -----------------------------------------------------------------------------
*/

// Cam Registers

static int CAM_DimX;
static int CAM_DimY;
static int CAM_HwDimX;
static int CAM_HwDimY;

static int CAM_WndHandle;
static int CAM_UsbHandle;
static int CAM_TargetPosX;
static int CAM_TargetPosY;
static int CAM_TransferMode;

static int CAM_IplWidthStep;
static int CAM_IplLenX;
static int CAM_IplLenY;

static struct ndImage_Type *CAM_ndImgTarget;
IplImage *CAM_IplImageTarget;

static char *CAM_ImageData;
static int  FrameCounter = 0;

static char CAM_MicrophoneIsOn = 0;
static void *CAM_MicAreaDriver;

static char CAM_MicRecordIsOn = 0;
static int  CAM_MicRec_ThreadID;
static int  CAM_MicRec_YouCanExit;
static int  CAM_MicRec_FHandle;
static void *CAM_MicRec_AudioBuffer;


// Puntatori a funzione che vengono settati dalla routine di grabbing

static char (*INTERNAL_PntToValidateParameters)   (int _UsbHandleCam, int _AuxValue, ndint64 _Options);
static void (*INTERNAL_PntToProvideToRenderImage)  (void);
static void (*INTERNAL_PntToExecuteRender)        (void);










/*
   ----------------------------------------------------------------------------
   1. ROUTINES DI SUPPORTO PER ndHAL_GrabNewImage 
   ----------------------------------------------------------------------------
*/


void INTERNAL_GOCAM_RegisterNewInfoAboutCameraImages (int _DimImgX, int _DimImgY, int _HwDimImgX, int _HwDimImgY, char _TrasmissionMode)
/*
    Questa routine provvede a memorizzare nei cam registers le informazioni
    relative all'immagine
*/
{
       CAM_DimX            = _DimImgX;
       CAM_DimY            = _DimImgY;
       CAM_HwDimX          = _HwDimImgX;
       CAM_HwDimY          = _HwDimImgY;
       CAM_TransferMode    = _TrasmissionMode;
} 







/*
   ----------------------------------------------------------------------------
   2. Routines di tipo CAM TO NULL
   ----------------------------------------------------------------------------
   Queste routines si occupano di gestire la webcam quando il target è NULL
   ----------------------------------------------------------------------------
*/

static char INTERNAL_GOCAM_CAMtoNULL_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è NULL. 
*/
{
    // Registra l'handle USB
                 
    CAM_UsbHandle = _UsbHandleCam; 
    return 0; 
}

static inline void INTERNAL_GOCAM_CAMtoNULL_ProvideToRenderImage (void)
/*
   Questa routine non ha funzioni effettive (render delle tiles in NULL).
*/
{  
}

static void INTERNAL_GOCAM_CAMtoNULL_ExecuteRender (void)
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

static char INTERNAL_GOCAM_CAMtoWND_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è una finestra. 
    
    Ricordiamo che in questo caso AuxValue rappresenta l'handle della
    finestra in cui è necessario inviare l'immagine che è stata grabbata
    
    La routine provvede anche a registrare i parametri che saranno necessari
    per il funzionamento del renderer di camera
*/
{
    // Registra l'handle USB
    
    CAM_UsbHandle = _UsbHandleCam;
    
    // Adesso registra i parametri TargetPosX e TargetPosY
    
    CAM_TargetPosX = (_Options >> 11) & 0x7FF;
    CAM_TargetPosY = (_Options)       & 0x7FF;
    
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

static inline void INTERNAL_GOCAM_CAMtoWND_ProvideToRenderImage (void)
/*
   Questa routine ha il compito di renderizzate una immagine in una determinata finestra.
*/
{
    register int LimitX1,     LimitY1;
    register int LimitX2,     LimitY2;
    register int RealDimX,    RealDimY;
    register int CounterX,    CounterY;
    register int DstPixelX,   DstPixelY;
    register int SrcPixelX,   SrcPixelY;
    
    register int Code32Bit;
    
    register float RatioX,    RatioY;
    register float InvRatioX, InvRatioY;
    
    register unsigned char R, G, B;
    register int  AV;
    
    register TypeColor *PntToRow, *PntToPixel;
    
    LimitX1 = CAM_TargetPosX;
    LimitY1 = CAM_TargetPosY;
    LimitX2 = CAM_TargetPosX + CAM_DimX;
    LimitY2 = CAM_TargetPosY + CAM_DimY;
    
    if (LimitX2 > WindowData [CAM_WndHandle].WSWndLength)   LimitX2 = WindowData [CAM_WndHandle].WSWndLength;
    if (LimitY2 > WindowData [CAM_WndHandle].WSWndHeight)   LimitY2 = WindowData [CAM_WndHandle].WSWndHeight;

    RatioX = (float)(CAM_DimX) / (float)(CAM_HwDimX);
    RatioY = (float)(CAM_DimY) / (float)(CAM_HwDimY);
    
    InvRatioX = 1.0/RatioX;
    InvRatioY = 1.0/RatioY;
    
    switch (CAM_TransferMode)
    {
           case 24:
           case 16:
                {
                    for (CounterY=LimitY1, DstPixelY=0, PntToRow = WindowData[CAM_WndHandle]._ws + WindowData [CAM_WndHandle].WSWndLength * LimitY1; 
                         CounterY<LimitY2; 
                         CounterY++, DstPixelY++, PntToRow += WindowData [CAM_WndHandle].WSWndLength)
                    {
                        for (CounterX=LimitX1, DstPixelX=0, PntToPixel = PntToRow + LimitX1; 
                             CounterX<LimitX2; 
                             CounterX++, DstPixelX++, PntToPixel++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            *PntToPixel = ((B & 0xF8)<<7) + ((G & 0xF8)<<2) + ((R & 0xF8)>>3) + 0x8000;
                        }
                    }
                    
                    break;
                };
                
           case 8:
                {
                    for (CounterY=LimitY1, DstPixelY=0, PntToRow = WindowData[CAM_WndHandle]._ws + WindowData [CAM_WndHandle].WSWndLength * LimitY1; 
                         CounterY<LimitY2; 
                         CounterY++, DstPixelY++, PntToRow += WindowData [CAM_WndHandle].WSWndLength)
                    {
                        for (CounterX=LimitX1, DstPixelX=0, PntToPixel = PntToRow + LimitX1; 
                             CounterX<LimitX2; 
                             CounterX++, DstPixelX++, PntToPixel++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            AV = (B+G+R)/3;
                            
                            *PntToPixel = ((AV & 0xF8)<<7) + ((AV & 0xF8)<<2) + ((AV & 0xF8)>>3) + 0x8000;
                        }
                    }
                    
                    break;
                };
    }  
}

static void INTERNAL_GOCAM_CAMtoWND_ExecuteRender (void)
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

static char INTERNAL_GOCAM_CAMtoBS_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
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
    
    // Tutto ok. Restituisci 0 alla routine chiamante
    
    return 0;
}





static inline void INTERNAL_GOCAM_CAMtoBS_ProvideToRenderImage (void)
/*
   Questa routine ha il compito di renderizzate l'immagine catturata dalla
   GoCam all'interno del base-screen.
*/
{ 
    register int LimitX1,     LimitY1;
    register int LimitX2,     LimitY2;
    register int RealDimX,    RealDimY;
    register int CounterX,    CounterY;
    register int DstPixelX,   DstPixelY;
    register int SrcPixelX,   SrcPixelY;
    
    register int Code32Bit;
    
    register float RatioX,    RatioY;
    register float InvRatioX, InvRatioY;
    
    register unsigned char R, G, B;
    register int  AV;
    
    LimitX1 = CAM_TargetPosX;
    LimitY1 = CAM_TargetPosY;
    LimitX2 = CAM_TargetPosX + CAM_DimX;
    LimitY2 = CAM_TargetPosY + CAM_DimY;
    
    if (LimitX2 > Windows_MaxScreenX)   LimitX2 = Windows_MaxScreenX;
    if (LimitY2 > Windows_MaxScreenY)   LimitY2 = Windows_MaxScreenY;

    RatioX = (float)(CAM_DimX) / (float)(CAM_HwDimX);
    RatioY = (float)(CAM_DimY) / (float)(CAM_HwDimY);
    
    InvRatioX = 1.0/RatioX;
    InvRatioY = 1.0/RatioY;
    
    switch (CAM_TransferMode)
    {
           case 24:
           case 16:
                {
                    for (CounterY=LimitY1, DstPixelY=0; CounterY<LimitY2; CounterY++, DstPixelY++)
                    {
                        for (CounterX=LimitX1, DstPixelX=0; CounterX<LimitX2; CounterX++, DstPixelX++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            BaseScreen [CounterY][CounterX] = ((B & 0xF8)<<7) + ((G & 0xF8)<<2) + ((R & 0xF8)>>3) + 0x8000;
                        }
                    }
                    
                    break;
                };
                
           case 8:
                {
                    for (CounterY=LimitY1, DstPixelY=0; CounterY<LimitY2; CounterY++, DstPixelY++)
                    {
                        for (CounterX=LimitX1, DstPixelX=0; CounterX<LimitX2; CounterX++, DstPixelX++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            AV = (B+G+R)/3;
                            
                            BaseScreen [CounterY][CounterX] = ((AV & 0xF8)<<7) + ((AV & 0xF8)<<2) + ((AV & 0xF8)>>3) + 0x8000;
                        }
                    }
                    
                    break;
                };
    }
}

static void INTERNAL_GOCAM_CAMtoBS_ExecuteRender (void)
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

static char INTERNAL_GOCAM_CAMtoNDIMG_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
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





static inline void INTERNAL_GOCAM_CAMtoNDIMG_ProvideToRenderImage (void)
/*
   Questa routine ha il compito di renderizzare le immagini catturate
   dalla cam all'interno delle immagini Nanodesktop.
*/
{
    register int LimitX1,     LimitY1;
    register int LimitX2,     LimitY2;
    register int RealDimX,    RealDimY;
    register int CounterX,    CounterY;
    register int DstPixelX,   DstPixelY;
    register int SrcPixelX,   SrcPixelY;
    
    register int Code32Bit;
    
    register float RatioX,    RatioY;
    register float InvRatioX, InvRatioY;
    
    register unsigned char R, G, B;
    register int  AV;
    
    register unsigned char *PntToMem;
    
    LimitX1 = CAM_TargetPosX;
    LimitY1 = CAM_TargetPosY;
    LimitX2 = CAM_TargetPosX + CAM_DimX;
    LimitY2 = CAM_TargetPosY + CAM_DimY;
    
    if (LimitX2 > CAM_ndImgTarget->LenX)   LimitX2 = CAM_ndImgTarget->LenX;
    if (LimitY2 > CAM_ndImgTarget->LenY)   LimitY2 = CAM_ndImgTarget->LenY;

    RatioX = (float)(CAM_DimX) / (float)(CAM_HwDimX);
    RatioY = (float)(CAM_DimY) / (float)(CAM_HwDimY);
    
    InvRatioX = 1.0/RatioX;
    InvRatioY = 1.0/RatioY;
    
    switch (CAM_TransferMode)
    {
           case 24:
           case 16:
                {
                    for (CounterY=LimitY1, DstPixelY=0; CounterY<LimitY2; CounterY++, DstPixelY++)
                    {
                        for (CounterX=LimitX1, DstPixelX=0; CounterX<LimitX2; CounterX++, DstPixelX++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            // I dati sono sottoposti ad un trattamento diverso, a seconda
                            // delle caratteristiche dell'immagine
                            
                            switch (CAM_ndImgTarget->ColorFormat)
                            {
                                     case NDMGKNB:
                                     {
                                          *(CAM_ndImgTarget->Data + CounterY*CAM_ndImgTarget->LenX + CounterX) = ((B & 0xF8)<<7) + ((G & 0xF8)<<2) + ((R & 0xF8)>>3) + 0x8000;
                                          // Non è necessaria moltiplicazione per sizeof (TypeColor)
                                          // perchè MyImage->Data non è di tipo char e quindi ci
                                          // pensa il compilatore
                                          
                                          break;
                                     }
                                    
                                     case NDRGB:
                                     {
                                          PntToMem = (unsigned char *)(CAM_ndImgTarget->Data + CounterY*CAM_ndImgTarget->LenX + CounterX);
                                          
                                          *(PntToMem + 0)  =  R;
                                          *(PntToMem + 1)  =  G;
                                          *(PntToMem + 2)  =  B;
                                          
                                          break;
                                     }
                                    
                                    
                            }   
                        }
                    }
                    
                    break;
                };
                
           case 8:
                {
                    for (CounterY=LimitY1, DstPixelY=0; CounterY<LimitY2; CounterY++, DstPixelY++)
                    {
                        for (CounterX=LimitX1, DstPixelX=0; CounterX<LimitX2; CounterX++, DstPixelX++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            AV = (B+G+R)/3;
                            
                            // I dati sono sottoposti ad un trattamento diverso, a seconda
                            // delle caratteristiche dell'immagine
                            
                            switch (CAM_ndImgTarget->ColorFormat)
                            {
                                     case NDMGKNB:
                                     {
                                          *(CAM_ndImgTarget->Data + CounterY*CAM_ndImgTarget->LenX + CounterX) = ((AV & 0xF8)<<7) + ((AV & 0xF8)<<2) + ((AV & 0xF8)>>3) + 0x8000;
                                          // Non è necessaria moltiplicazione per sizeof (TypeColor)
                                          // perchè MyImage->Data non è di tipo char e quindi ci
                                          // pensa il compilatore
                                          
                                          break;
                                     }
                                    
                                     case NDRGB:
                                     {
                                          PntToMem = (unsigned char *)(CAM_ndImgTarget->Data + CounterY*CAM_ndImgTarget->LenX + CounterX);
                                          
                                          *(PntToMem + 0)  =  AV;
                                          *(PntToMem + 1)  =  AV;
                                          *(PntToMem + 2)  =  AV;
                                          
                                          break;
                                     }
                                    
                                    
                            }   
                        }
                    }
                    
                    break;
                };
    } 
}

static void INTERNAL_GOCAM_CAMtoNDIMG_ExecuteRender (void)
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





static char INTERNAL_GOCAM_CAMtoCVIMG_ValidateParameters (int _UsbHandleCam, int _AuxValue, ndint64 _Options)
/*
    Questa routine ha il compito di validare i parametri passati alla routine
    principale quando il target è una immagine IplImage. 
    
    Ricordiamo che in questo caso AuxValue è l'indirizzo dell'immagine.
    
    La routine provvede anche a registrare i parametri che saranno necessari
    per il funzionamento del renderer
*/
{
    // Registra l'handle USB
    
    CAM_UsbHandle = _UsbHandleCam;
    
    // Adesso registra i parametri TargetPosX e TargetPosY
    
    CAM_TargetPosX = 0;
    CAM_TargetPosY = 0;
    
    // Memorizza l'indirizzo dell'immagine
    
    CAM_IplImageTarget = _AuxValue;
    
    CAM_IplLenX        = CAM_IplImageTarget->width;
    CAM_IplLenY        = CAM_IplImageTarget->height;
    
    CAM_IplWidthStep   = CAM_IplImageTarget->widthStep;
    
    CAM_ImageData      = CAM_IplImageTarget->imageData;
    
    // Tutto ok. Restituisci 0 alla routine chiamante
    
    return 0;  
}



static inline void INTERNAL_GOCAM_CAMtoCVIMG_ProvideToRenderImage (void)
/*
   Questa routine ha il compito di renderizzate una immagine all'interno di una
   immagine OpenCV
*/
{
    register int LimitX1,     LimitY1;
    register int LimitX2,     LimitY2;
    register int CounterX,    CounterY;
    register int DstPixelX,   DstPixelY;
    register int SrcPixelX,   SrcPixelY;
    
    register int Code32Bit;
    
    register float RatioX,    RatioY;
    register float InvRatioX, InvRatioY;
    
    register unsigned char R, G, B;
    
    register char* CAM_ImageDataY;
    register char* CAM_ImageDataYX;
   
    LimitX1 = 0;
    LimitY1 = 0;
    LimitX2 = CAM_DimX;
    LimitY2 = CAM_DimY;
    
    if (LimitX2 > CAM_IplLenX)   LimitX2 = CAM_IplLenX;
    if (LimitY2 > CAM_IplLenY)   LimitY2 = CAM_IplLenY;

    RatioX = (float)(CAM_DimX) / (float)(CAM_HwDimX);
    RatioY = (float)(CAM_DimY) / (float)(CAM_HwDimY);
    
    InvRatioX = 1.0/RatioX;
    InvRatioY = 1.0/RatioY;
    
    switch (CAM_TransferMode)
    {
           case 24:
           case 16:
                {
                    for (CounterY=0, DstPixelY=0, CAM_ImageDataY = CAM_ImageData; CounterY<LimitY2; CounterY++, DstPixelY++, CAM_ImageDataY += CAM_IplWidthStep)
                    {
                        for (CounterX=0, DstPixelX=0, CAM_ImageDataYX = CAM_ImageDataY; CounterX<LimitX2; CounterX++, DstPixelX++, CAM_ImageDataYX += 3)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            // I dati sono sottoposti ad un trattamento diverso, a seconda
                            // delle caratteristiche dell'immagine
                            
                            *(CAM_ImageDataYX + 0) = B;
                            *(CAM_ImageDataYX + 1) = G;
                            *(CAM_ImageDataYX + 2) = R;  
                        }
                    }
                    
                    break;
                };
                
           case 8:
                {
                    for (CounterY=0, DstPixelY=0, CAM_ImageDataY = CAM_ImageData; CounterY<LimitY2; CounterY++, DstPixelY++, CAM_ImageDataY += CAM_IplWidthStep)
                    {
                        for (CounterX=0, DstPixelX=0, CAM_ImageDataYX = CAM_ImageDataY; CounterX<LimitX2; CounterX++, DstPixelX++, CAM_ImageDataYX++)
                        {
                            SrcPixelX = DstPixelX*InvRatioX;
                            SrcPixelY = DstPixelY*InvRatioY;
                            
                            Code32Bit = CAM_FrameBuffer [CAM_HwDimX*SrcPixelY + SrcPixelX];
                            
                            B = (Code32Bit >> 16) & 0xFF;
                            G = (Code32Bit >>  8) & 0xFF;
                            R = (Code32Bit      ) & 0xFF;
                            
                            *CAM_ImageDataYX = (B+G+R)/3; 
                        }
                    }
                     
                    break;
                };
    }      
}


static void INTERNAL_GOCAM_CAMtoCVIMG_ExecuteRender (void)
{  
}



/*
   ----------------------------------------------------------------------------
   7. ROUTINES DI SUPPORTO PER ndHAL_GrabNewImage (seconda parte)
   ----------------------------------------------------------------------------
*/

static void INTERNAL_GOCAM_SetupPointers (int Target)
{
   switch (Target)
   {
          case ND_CAM_TO_NULL:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_GOCAM_CAMtoNULL_ValidateParameters);
              INTERNAL_PntToProvideToRenderImage = &(INTERNAL_GOCAM_CAMtoNULL_ProvideToRenderImage);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_GOCAM_CAMtoNULL_ExecuteRender);
              break;
          } 
               
          case ND_CAM_TO_WINDOW:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_GOCAM_CAMtoWND_ValidateParameters);
              INTERNAL_PntToProvideToRenderImage = &(INTERNAL_GOCAM_CAMtoWND_ProvideToRenderImage);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_GOCAM_CAMtoWND_ExecuteRender);
              break;
          } 
          
          case ND_CAM_TO_BASESCREEN:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_GOCAM_CAMtoBS_ValidateParameters);
              INTERNAL_PntToProvideToRenderImage = &(INTERNAL_GOCAM_CAMtoBS_ProvideToRenderImage);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_GOCAM_CAMtoBS_ExecuteRender);
              break; 
          }     
          
          case ND_CAM_TO_NDIMAGE_STRUCT:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_GOCAM_CAMtoNDIMG_ValidateParameters);
              INTERNAL_PntToProvideToRenderImage = &(INTERNAL_GOCAM_CAMtoNDIMG_ProvideToRenderImage);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_GOCAM_CAMtoNDIMG_ExecuteRender);
              break; 
          } 
          
          case ND_CAM_TO_CVIMAGE:
          {
              INTERNAL_PntToValidateParameters  = &(INTERNAL_GOCAM_CAMtoCVIMG_ValidateParameters);
              INTERNAL_PntToProvideToRenderImage = &(INTERNAL_GOCAM_CAMtoCVIMG_ProvideToRenderImage);
              INTERNAL_PntToExecuteRender       = &(INTERNAL_GOCAM_CAMtoCVIMG_ExecuteRender);
              break;  
          }
   } 
}




















/*
   -----------------------------------------------------------------------------
   WEBCAM API per la GoCam
   -----------------------------------------------------------------------------
*/

char INTERNAL_GOCAM_ActivateCamera (void)
/*
     Routine specifica per l'Eyeserver. Provvede ad attivarlo usando la sezione
     specifica di inizializzazione USB
*/
{
     static struct usb_device devCamera;
     char UsbHandle;
     
     devCamera.descriptor.idVendor  = GOCAM_VENDOR_ID;
     devCamera.descriptor.idProduct = GOCAM_PRODUCT_ID;
            
     UsbHandle = ndHAL_USB_OpenDevice (&devCamera);   // Tenta la connessione alla GoCam
     if (UsbHandle>=0) return UsbHandle;              // Se è riuscito ritorna l'USB handle       
}

 

char INTERNAL_GOCAM_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow)
/*
    Questa routine provvede a chiedere alla GoCam l'invio di un'immagine.
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
       
   
*/
{
    int ErrRep, result;
    
    if (!ND_HWMODE_DisableGoCamService)
    {
        if (!ndUsb_IsFailure) 
        {
               if (!ndJpegDecoderIsFailure)  
               {
                       // Inizio del codice di elaborazione
                       
                       if ((Target>=0) && (Target<=4))
                       {
                                 INTERNAL_GOCAM_SetupPointers (Target);
            
                                 ErrRep = (*INTERNAL_PntToValidateParameters)(UsbHandleCamera, AuxValue, Options);
            
                                 if (!ErrRep)            // Non ci sono errori in fase di validazione
                                 {
                                            // Adesso preleviamo i dati: prima li catturiamo in formato JPEG....
                                    		result = sceUsbCamReadVideoFrameBlocking(CAM_JpegBuffer, MAX_VIDEO_FRAME_JPG_SIZE);
                                    		
                                            if (result>=0)     // Nessun errore
                                            {
                                                  // ... poi passiamo al decodificatore hardware JPEG
                                                  result = ndHAL_HWJPG_DecodeMJpeg (CAM_JpegBuffer, result, CAM_FrameBuffer);
                                            
                                                  if (result>=0)
                                                  {
                                                        // Copiamo i dati che ci servono nell'oggetto di destinazione
                                                        
                                                        (*INTERNAL_PntToProvideToRenderImage)();
                                                        
                                                        // ... provvediamo al rendering dell'immagine
                                                        
                                                        if (RenderNow) (*INTERNAL_PntToExecuteRender)();
                                                        
                                                        // ... incrementiamo il contatore frame
                                                        
                                                        FrameCounter++;
                                                        
                                                        return 0;               
                                                  }
                                                  else
                                                  {
                                                      return ERR_IO_ERROR;
                                                  }
                                            }
                                            else
                                            {
                                                  return ERR_IO_ERROR;
                                            }
                                            
                                            
                                 }
                                 else                 // Errore durante la fase di validazione dei parametri
                                 return ErrRep;       // Riporta l'errore di validazione riscontrato      
                                 
                       }
                       else
                         return ERR_WRONG_PARAMETERS; 
                         
               }              
               else    // Disabilitato per problemi nel decoder JPEG
                 return ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC; 
                                 
        }
        else        // Disabilitato per failure nell'usb 
          return ERR_FUNCTION_HAS_BEEN_DISABLED;
                                         
    }
    else    // Supporto GoCam disabilitato
      return ERR_GOCAM_SUPPORT_IS_DISABLED;
     
} 
    


char INTERNAL_GOCAM_ChangeResolution (int UsbHandleCam, int NewSizeX, int NewSizeY)
/*
   Questa routine informa il sistema che è necessario cambiare la risoluzione
   con cui le immagini vengono inviate al client. 
   
   NewSizeX e NewSizeY rappresentano le nuove dimensioni. In uscita viene 
   normalmente restituito 0, se viene restituito un valore negativo siamo
   in presenza di un errore. Sono previsti i seguenti codici di errore:
   
           
      ERR_WRONG_PARAMETERS             Parametri NewSizeX o NewSizeY non corretti  
      
      ERR_IO_ERROR                     Errore di I/O          
*/
{
    if (!ND_HWMODE_DisableGoCamService)
    {
        if (!ndUsb_IsFailure) 
        {
               if (!ndJpegDecoderIsFailure)  
               {
                           if ((NewSizeX>0) && (NewSizeY>0))
                           {
                                 CAM_DimX = NewSizeX;
                                 CAM_DimY = NewSizeY;
                                 
                                 return 0;   
                           }
                           else
                             return ERR_WRONG_PARAMETERS;                       
               }              
               else    // Disabilitato per problemi nel decoder JPEG
                 return ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC; 
                                 
        }
        else        // Disabilitato per failure nell'usb 
          return ERR_FUNCTION_HAS_BEEN_DISABLED;
                                         
    }
    else    // Supporto GoCam disabilitato
      return ERR_GOCAM_SUPPORT_IS_DISABLED;   
}
 
 
char INTERNAL_GOCAM_ChangeTransferMode (int UsbHandleCam, int NewMode)
/*
    Questa routine permette di cambiare il transfer mode utilizzato
    dalla GoCam. 
    
    Il parametro Mode può assumere esclusivamente 3 valori: 8, 16, 24.
    
    I valori 24 e 16 consentono di avere dalla GoCam immagini a 
    colori, mentre il parametro 8 consente di avere immagini in
    toni di grigio
    
    L'utente può usare le costanti manifeste ND_RGB_TRASMISSION_MODE,
    ND_RGB555_TRASMISSION_MODE e ND_GRAY_TRASMISSION_MODE in luogo
    delle semplici costanti numeriche. 

    Sono previsti i seguenti codici di errore:
         
      ERR_WRONG_PARAMETERS             Parametri NewMode non corretto 
      
      ERR_IO_ERROR                     Errore di I/O      
*/
{
   if (!ND_HWMODE_DisableGoCamService)
    {
        if (!ndUsb_IsFailure) 
        {
               if (!ndJpegDecoderIsFailure)  
               {
                           if ( (NewMode==ND_RGB_TRASMISSION_MODE) || (NewMode==ND_RGB555_TRASMISSION_MODE) || (NewMode==ND_GRAY_TRASMISSION_MODE) )
                           {
                               CAM_TransferMode = NewMode;
                               return 0;     
                           }
                           else
                             return ERR_WRONG_PARAMETERS;                           
               }              
               else    // Disabilitato per problemi nel decoder JPEG
                 return ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC; 
                                 
        }
        else        // Disabilitato per failure nell'usb 
          return ERR_FUNCTION_HAS_BEEN_DISABLED;
                                         
    }
    else    // Supporto GoCam disabilitato
      return ERR_GOCAM_SUPPORT_IS_DISABLED;
      
}

char INTERNAL_GOCAM_GetInfo (int UsbHandleCam, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode)
/*
    Questa routine si occupa di prelevare le informazioni sulle immagini attualmente
    visualizzate dalla GoCam. 
    
    Sono previsti i seguenti codici di errore:
         
      ERR_USBHANDLE_IS_NOT_ACTIVE      L'handle USB passato non è attivo
      
      ERR_DEVICE_NOT_SUPPORTED         L'handle USB utilizzato non si riferisce ad una webcam
           
      ERR_IO_ERROR                     Errore di I/O      
*/
{
    if (!ND_HWMODE_DisableGoCamService)
    {
        if (!ndUsb_IsFailure) 
        {
               if (!ndJpegDecoderIsFailure)  
               {
                           *DimImgX      = CAM_DimX;
                           *DimImgY      = CAM_DimY;
                           *FrameCounter = FrameCounter;
                           *TransferMode = CAM_TransferMode;    
                           
                           return 0;              
               }              
               else    // Disabilitato per problemi nel decoder JPEG
                 return ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC; 
                                 
        }
        else        // Disabilitato per failure nell'usb 
          return ERR_FUNCTION_HAS_BEEN_DISABLED;
                                         
    }
    else    // Supporto GoCam disabilitato
      return ERR_GOCAM_SUPPORT_IS_DISABLED;
   
}

char INTERNAL_GOCAM_ChangeSensorMode (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la modalità di video del
     sensore della videocamera.
*/
{
    int FwResult;
    
    FwResult = sceUsbCamSetImageEffectMode (NewMode);
    
    if (!FwResult) return 0;
       else return ERR_INTERNAL_ERROR;
}


char INTERNAL_GOCAM_ChangeHwResolution (int UsbHandleCamera, int NewMode)
/*
     Questa routine provvede a cambiare la risoluzione del
     sensore della videocamera. 
     
     La routine non deve essere confusa con INTERNAL_GOCAM_ChangeResolution.
     
     Infatti, la routine INTERNAL_GOCAM_ChangeResolution accetta in
     input qualsiasi risoluzione: Nanodesktop provvederà ad uno scaling
     automatico per ottenere la risoluzione richiesta a partire dalla
     risoluzione del sensore. 
     
     Al contrario, INTERNAL_GOCAM_ChangeHwResolution accetta in ingresso
     solo un numero finito di codici di risoluzione, e setta direttamente
     la risoluzione del sensore
*/
{
    PspUsbCamSetupVideoParam GoCam_Parameters;    
    // Struttura destinata ad accogliere i dati di configurazione della
    // webcam 
	
    int PSPCodeResolution, _NewHwImgDimX, _NewHwImgDimY;
    int oldSaturation, oldBrightness, oldContrast, oldSharpness;
    int oldEvLevel, oldEffectMode; 
    int result;
    
    // Trasforma il codice NewMode in un codice PSP. Inoltre, ricava i valori
    // di _ImgDimX, _ImgDimY
    
    result=INTERNAL_GOCAM_GenerateHwResolutionCodes (NewMode, &PSPCodeResolution, &_NewHwImgDimX, &_NewHwImgDimY);
    if (result<0) return ERR_WRONG_PARAMETERS;  
    
    if ((CAM_HwDimX!=_NewHwImgDimX) || (CAM_HwDimY!=_NewHwImgDimY))
    {
        // Blocca lo streaming video dalla videocamera
        sceUsbCamStopVideo();
        
        // Disimpegna il decoder JPEG
        ndHAL_HWJPG_DisableJpegDecoder (); 	
    
        // Adesso provvedi a cambiare la configurazione del sistema di
        // compressione hardware JPEG
        
        result = ndHAL_HWJPG_EnableJpegDecoder (ND_JPEGDEC_GOCAM_SUPPORT, NewMode);
    
        // Recupera alcune vecchie impostazioni che devono essere conservate
        
        sceUsbCamGetSaturation (&oldSaturation);
        sceUsbCamGetBrightness (&oldBrightness);
        sceUsbCamGetContrast   (&oldContrast);
        sceUsbCamGetSharpness  (&oldSharpness);
        
        sceUsbCamGetEvLevel (&oldEvLevel);
        sceUsbCamGetImageEffectMode (&oldEffectMode);
        
        // Riconfigura il chip video
        
        if (result==0)
        {
        	memset(&GoCam_Parameters, 0, sizeof(GoCam_Parameters));
        	GoCam_Parameters.size = sizeof(GoCam_Parameters);
        	GoCam_Parameters.resolution = PSPCodeResolution;
        	GoCam_Parameters.framerate = PSP_USBCAM_FRAMERATE_15_FPS;
        	GoCam_Parameters.wb = PSP_USBCAM_WB_AUTO;
        	GoCam_Parameters.saturation = oldSaturation;
        	GoCam_Parameters.brightness = oldBrightness;
        	GoCam_Parameters.contrast   = oldContrast;
        	GoCam_Parameters.sharpness  = oldSharpness;
        	GoCam_Parameters.effectmode = oldEffectMode;
        	GoCam_Parameters.framesize = MAX_VIDEO_FRAME_JPG_SIZE;
        	GoCam_Parameters.evlevel = oldEvLevel;	
        	
        	result = sceUsbCamSetupVideo(&GoCam_Parameters, CAM_WorkAreaDriver, WORK_AREA_DRIVER_SIZE);	
            if (result < 0) return -1;
        	
        	sceUsbCamAutoImageReverseSW(1);
        
        	result = sceUsbCamStartVideo();	
        	if (result < 0) return -2;
        	
        	// Riprogramma i parametri di scaling
        	
            INTERNAL_GOCAM_RegisterNewInfoAboutCameraImages (_NewHwImgDimX, _NewHwImgDimY, _NewHwImgDimX, _NewHwImgDimY, CAM_TransferMode);
            return 0;
        }
        else               // Problema nel settare il decoder JPEG
          return -3;
    }
    else    // La risoluzione è uguale a quella attuale
       return 0;           // Simula successo
}


char INTERNAL_GOCAM_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness)
/*
    Consente di settare la luminosità della videocamera (0-255). 
*/
{
    if (sceUsbCamSetBrightness (NewBrightness)==0)
       return 0;
    else
       return ERR_IO_ERROR; 
}

char INTERNAL_GOCAM_ChangeHwContrast (int UsbHandleCamera, int NewContrast)
/*
    Consente di settare il contrasto della videocamera (0-255). 
*/
{
    if (sceUsbCamSetContrast (NewContrast)==0)
       return 0;
    else
       return ERR_IO_ERROR; 
}

char INTERNAL_GOCAM_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness)
/*
    Consente di settare la nitidezza della videocamera (0-255). 
*/
{
    if (sceUsbCamSetSharpness (NewSharpness)==0)
       return 0;
    else
       return ERR_IO_ERROR; 
}

char INTERNAL_GOCAM_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel)
/*
    Consente di settare il livello di esposizione (da -128 a +128) 
*/
{
    int NormEvLevel = 8 - (NewEvLevel/16);
    
    if ((sceUsbCamSetEvLevel (NormEvLevel))==0)
       return 0;
    else
       return ERR_IO_ERROR; 
}


int INTERNAL_GOCAM_GetSaturationLevel (int UsbHandleCamera)
{
    int Data;
    int ErrRep;
    
    ErrRep = sceUsbCamGetSaturation (&Data);
    
    if (!ErrRep) return Data;
    else return ERR_IO_ERROR; 
}


int INTERNAL_GOCAM_GetBrightness (int UsbHandleCamera)
{
    int Data;
    int ErrRep;
    
    ErrRep = sceUsbCamGetBrightness (&Data);
    
    if (!ErrRep) return Data;
    else return ERR_IO_ERROR; 
}


int INTERNAL_GOCAM_GetContrast (int UsbHandleCamera)
{
    int Data;
    int ErrRep;
    
    ErrRep = sceUsbCamGetContrast (&Data);
    
    if (!ErrRep) return Data;
    else return ERR_IO_ERROR; 
}



int INTERNAL_GOCAM_GetSharpness (int UsbHandleCamera)
{
    int Data;
    int ErrRep;
    
    ErrRep = sceUsbCamGetSharpness (&Data);
    
    if (!ErrRep) return Data;
    else return ERR_IO_ERROR; 
}



int INTERNAL_GOCAM_GetHwSensorMode (int UsbHandleCamera)
{
    int Data;
    int ErrRep;
    
    ErrRep = sceUsbCamGetImageEffectMode (&Data);
    
    if (!ErrRep) return Data;
    else return ERR_IO_ERROR; 
}


int INTERNAL_GOCAM_GetEvLevel (int UsbHandleCamera)
{
    int Data;
    int ErrRep;
    
    ErrRep = sceUsbCamGetEvLevel (&Data);
    
    if (!ErrRep) return Data;
    else return ERR_IO_ERROR; 
}



char INTERNAL_GOCAM_ActivateCamMicrophone (int WorkAreaAudioSize, int Frequency, int Gain)
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
    PspUsbCamSetupMicParam micparam;
	
    int result;
	
	memset(&micparam, 0, sizeof(micparam));
	micparam.size = sizeof(micparam);
	micparam.gain = Gain;
	micparam.frequency = Frequency;

	CAM_MicAreaDriver = malloc (WorkAreaAudioSize);
	
	if (CAM_MicAreaDriver!=NULL)
	{
        result = sceUsbCamSetupMic(&micparam, CAM_MicAreaDriver, WorkAreaAudioSize);
    	
        if (result>=0)
    	{
             result=sceUsbCamStartMic();   
             
             if (result>=0)
             {
                  CAM_MicrophoneIsOn=1;
                  return 0;         
             }
             else 
                return ERR_IO_ERROR;      
        }
        else    // Problema di inizializzazione del microfono
          return ERR_DEVICE_IS_NOT_OPERATIVE; 
    }
    else
       return ERR_MEMORY_ERROR; 
}
    
    
int INTERNAL_GOCAM_ReadAudioBlock (void *Buffer, int NrBytesToRead)
/*
    Questa routine è esclusivamente per uso interno.
    
    Questa routine legge un blocco di dati audio dal microfono
    e lo copia nel buffer *Buffer. 
    
    L'utente deve avere allocato in precedenza una area di memoria 
    opportuna, e ne deve passarne l'indirizzo alla routine 

    La routine restituisce in uscita il numero di bytes letti 
*/
{
    return sceUsbCamReadMicBlocking (Buffer, NrBytesToRead);
}    
    
void INTERNAL_GOCAM_DeActivateCamMicrophone (void)
/*
    Questa routine è esclusivamente per uso interno.
    Provvede a disattivare il microfono associato alla webcam    
*/  
{
    if (CAM_MicrophoneIsOn)
    {
       sceUsbCamStopMic();
       free (CAM_MicAreaDriver);
       
       CAM_MicrophoneIsOn=0;
    }
}


void INTERNAL_GOCAM_DisableCamera (int UsbHandleCamera)
/*
    Questa routine è esclusivamente per uso interno.
    
    Usa la funzione del sottosistema Usb associata per liberare l'handle
    e per scaricare i drivers associati
*/
{
    // Chiudi il dispositivo USB
    ndHAL_USB_CloseDevice (UsbHandleCamera);
}


/*
   ----------------------------------------------------------------------------
   ROUTINES DI REGISTRAZIONE VOCALE
   ----------------------------------------------------------------------------
*/

int  INTERNAL_AudioRecordingThread (SceSize args, void *argp)
{
    struct MicRec_WAVHeader_Type CAM_MicRec_WavHeader;
    int    BytesRead, NrSamplesRead;
    
    // Inizia a registrare l'header del file wave
        
    CAM_MicRec_WavHeader.ChunkID = 0x46464952; // "RIFF"
	CAM_MicRec_WavHeader.ChunkSize = 0; // Not known yet
	CAM_MicRec_WavHeader.Format = 0x45564157; // "WAVE"
	CAM_MicRec_WavHeader.SubChunk1ID = 0x20746d66; // "fmt "
	CAM_MicRec_WavHeader.SubChunk1Size = 16;
	CAM_MicRec_WavHeader.AudioFormat = 1; // PCM
	CAM_MicRec_WavHeader.NumChannels = 1; // mono
	CAM_MicRec_WavHeader.SampleRate = 44100;
	CAM_MicRec_WavHeader.ByteRate = CAM_MicRec_WavHeader.SampleRate * CAM_MicRec_WavHeader.NumChannels * 2;
	CAM_MicRec_WavHeader.BlockAlign = CAM_MicRec_WavHeader.NumChannels * 2;
	CAM_MicRec_WavHeader.BitsPerSample = 16;
	CAM_MicRec_WavHeader.SubChunk2ID = 0x61746164; // "data"
	CAM_MicRec_WavHeader.SubChunk2Size = 0; // Not known yet

	write (CAM_MicRec_FHandle, &CAM_MicRec_WavHeader, sizeof(CAM_MicRec_WavHeader));

    // Inizia la registrazione
    CAM_MicRec_YouCanExit = 0;
    NrSamplesRead         = 0;

    while (!CAM_MicRec_YouCanExit)
    {
         BytesRead = INTERNAL_GOCAM_ReadAudioBlock (CAM_MicRec_AudioBuffer, 4096);
         write (CAM_MicRec_FHandle, CAM_MicRec_AudioBuffer, BytesRead);
                
         NrSamplesRead += BytesRead; 
    }
    
    // Arrivati a questo punto, la registrazione è completata
    
    CAM_MicRec_WavHeader.ChunkSize = NrSamplesRead + sizeof(CAM_MicRec_WavHeader) - 8;
    CAM_MicRec_WavHeader.SubChunk2Size = NrSamplesRead;

    lseek (CAM_MicRec_FHandle, 0, SEEK_SET);
    write (CAM_MicRec_FHandle, &CAM_MicRec_WavHeader, sizeof(CAM_MicRec_WavHeader));
    close (CAM_MicRec_FHandle);
    
    // Disalloca il buffer audio
    free (CAM_MicRec_AudioBuffer);
    
    // Disattiva il microfono
    INTERNAL_GOCAM_DeActivateCamMicrophone ();
    CAM_MicrophoneIsOn=0;
    
    // Segnala che non c'è più alcun thread di registrazione
    CAM_MicRecordIsOn=0;    
    return 0;
}

char INTERNAL_GOCAM_StartAudioRecording (char *NameFile, int Frequency, int Gain)
/*
     Questa routine è esclusivamente per uso interno.
     
     Questa routine inizia a registrare il flusso audio proveniente dalla webcam su un
     file di nome NameFile in formato WAV. 
     
     Viene restituito il valore 0 se il processo di avvio del thread audio
     è andato a buon fine, altrimenti viene restituito un codice di errore. 
     
     E' possibile avviare un solo thread di registrazione alla volta. 
     Quando l'utente vuole arrestare il processo di registrazione, deve
     usare la routine ndHAL_CAM_StopAudioRecording che chiude il file
     WAV e dealloca il thread di registrazione dopo averlo terminato.
     
     Sono previsti i seguenti codici di errore:
          ERR_DEVICE_IS_NOT_OPERATIVE   Problemi per l'inizializzazione del
                                        microfono
          ERR_SYSTEM_IS_BUSY            C'è già un thread di registrazione attivo
          ERR_FILE_NOT_FOUND            Non riesco ad aprire questo file per la
                                        registrazione
          ERR_MEMORY_OVERFLOW           Overflow della memoria
          ERR_INTERNAL_ERROR            Problemi ad avviare il thread di registrazione
*/
{
     int ErrRep;
     
     if (!CAM_MicRecordIsOn)
     {
         // Se il microfono era attivo, disattivalo: in questo modo la vecchia
         // workarea del driver audio verrà disattivata
         if (CAM_MicrophoneIsOn) INTERNAL_GOCAM_DeActivateCamMicrophone ();
         
         ErrRep=INTERNAL_GOCAM_ActivateCamMicrophone (4096, Frequency, Gain);
         
         if (!ErrRep)
         {
              CAM_MicrophoneIsOn=1;    
              CAM_MicRec_FHandle = open (NameFile, O_WRONLY | O_CREAT | O_TRUNC, 0777); 
              
              if (CAM_MicRec_FHandle>=0)
              {
                  CAM_MicRec_AudioBuffer = malloc (4096);
                  
                  if (CAM_MicRec_AudioBuffer!=0)
                  {
                       CAM_MicRec_ThreadID = sceKernelCreateThread("MicRecThreadGoCam", INTERNAL_AudioRecordingThread, 10, 16000, 0, NULL);
                       
                       if (CAM_MicRec_ThreadID>=0)
                       {
                           sceKernelStartThread (CAM_MicRec_ThreadID, 0, 0);
                           CAM_MicRecordIsOn = 1;
                           
                           return 0;
                       }
                       else  // Problemi nella creazione del thread di registrazione
                       {
                           free (CAM_MicRec_AudioBuffer);
                           close (CAM_MicRec_FHandle);
                     
                           INTERNAL_GOCAM_DeActivateCamMicrophone ();
                           CAM_MicrophoneIsOn=0;
                 
                           return ERR_INTERNAL_ERROR;       
                       }      
                  }
                  else   // Problemi nell'allocazione del buffer audio 
                  {
                     close (CAM_MicRec_FHandle);
                     
                     INTERNAL_GOCAM_DeActivateCamMicrophone ();
                     CAM_MicrophoneIsOn=0;
                 
                     return ERR_MEMORY_OVERFLOW;     
                  }                        
              }
              else  // Problemi nell'apertura del file wav richiesto
              {
                  INTERNAL_GOCAM_DeActivateCamMicrophone ();
                  CAM_MicrophoneIsOn=0;
                 
                  return ERR_FILE_NOT_FOUND; 
              }  
         }
         else
           return ERR_DEVICE_IS_NOT_OPERATIVE; 
     }
     else   // C'è già un thread di registrazione attivo
        return ERR_SYSTEM_IS_BUSY;
}


void INTERNAL_GOCAM_StopAudioRecording (void)
/*
    Arresta il processo di registrazione
*/
{
    if (CAM_MicRecordIsOn)
    {
        CAM_MicRec_YouCanExit = 1;
        
        sceKernelWaitThreadEnd (CAM_MicRec_ThreadID, 0);
        sceKernelTerminateDeleteThread (CAM_MicRec_ThreadID);
        
        CAM_MicRecordIsOn=0;              
    } 
}



/*
   ----------------------------------------------------------------------------
   FUNZIONI AUSILIARIE: GENERAZIONE CODICI DI RISOLUZIONE
   ----------------------------------------------------------------------------
*/

char INTERNAL_GOCAM_GenerateHwResolutionCodes (int NewMode, int *PSPCodeResolution, int *HwImgDimX, int *HwImgDimY)
{
    switch (NewMode)
    {
         /*
         case ND_RESOLUTION_CODE_160_120:     // Inibito
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_160_120;
              *HwImgDimX = 160; *HwImgDimY = 120;
              
              return 0; 
         };
         */
           
   	     case ND_RESOLUTION_CODE_176_144:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_176_144;
              *HwImgDimX = 176; *HwImgDimY = 144;
              
              return 0; 
         };
           
   	     case ND_RESOLUTION_CODE_320_240:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_320_240;
              *HwImgDimX = 320; *HwImgDimY = 240;
              
              return 0; 
         };
           
         case ND_RESOLUTION_CODE_352_288:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_352_288;
              *HwImgDimX = 352; *HwImgDimY = 288;
              
              return 0; 
         };
           
   	     case ND_RESOLUTION_CODE_640_480:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_640_480;
              *HwImgDimX = 640; *HwImgDimY = 480;
              
              return 0; 
         };
             
   	     case ND_RESOLUTION_CODE_1024_768:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_1024_768;
              *HwImgDimX = 1024; *HwImgDimY = 768;
              
              return 0; 
         };
          
         /* 
   	     case ND_RESOLUTION_CODE_1280_960:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_1280_960;
              *HwImgDimX = 1280; *HwImgDimY = 960;
              
              return 0; 
         };
         */
          
   	     case ND_RESOLUTION_CODE_480_272:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_480_272;
              *HwImgDimX = 480; *HwImgDimY = 272;
              
              return 0; 
         };
           
         /*                     // Inibito  
           
   	     case ND_RESOLUTION_CODE_360_272:
         {
              *PSPCodeResolution=PSP_USBCAM_RESOLUTION_360_272;
              *HwImgDimX = 360; *HwImgDimY = 272;
              
              return 0; 
         };
         
         */
               
         
         default:
         {
              return -1;   
         }                          
    }
}
