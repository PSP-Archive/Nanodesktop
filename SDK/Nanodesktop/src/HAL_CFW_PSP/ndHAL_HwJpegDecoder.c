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
    ---------------------------------------------------------------------------
    HAL: FUNZIONI DECODIFICA FORMATO JPEG (PSP CFW mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>
#include <il.h>

/*
    ---------------------------------------------------------------------------
    VARIABILI DELL'EMULATORE DEVIL
    ---------------------------------------------------------------------------
*/


static int EMU_DEVIL_JPG_Image;        // Codice immagine DevIL 

static int  _SoftJpegDimX, _SoftJpegDimY;
static char DevILIsEnable;

/*
    ---------------------------------------------------------------------------
    PUNTATORI 
    ---------------------------------------------------------------------------
*/

static void     (*xilGenImages)(int Num, int *Images);
static void     (*xilBindImage)(int Image);
static void     (*xilLoadL)(ILenum Type, const ILvoid *Lump, int Size);
static int      (*xilCopyPixels)(int XOff, int YOff, int ZOff, int Width, int Height, int Depth, ILenum Format, ILenum Type, void *Data);
static void     (*xilDeleteImages)(int Num, const ILuint *Images);

/*
    ---------------------------------------------------------------------------
    CODE
    ---------------------------------------------------------------------------
*/


void ndHAL_InitJpegDecoder (void)
/*
    Provvede a reinizializzare il decoder JPEG hardware
*/
{
    int result;
    
    if (!ND_HWMODE_DisableMediaService)
    {
        result = sceJpegInitMJpeg();
    	if (result < 0)
    	{
    	   ndJpegDecoderIsFailure           = 1;
           ndJpegDecoderState               = ND_FAILURE_TO_INIT_JPEGDEC;
    	   ndJpegDecoderSecondaryErrorCode  = result;
           return;		
    	}
                 
        ndJpegDecoderIsFailure = 0;
        ndJpegDecoderState = 0;
        ndJpegDecoderSecondaryErrorCode = 0;

        ndJpegActualActor = ND_JPEGDEC_NOACTOR;
        ndJpegHardwareResCode = 0;
        
        // Per ragioni di corretto linkaggio, il sistema non accede direttamente a
        // Dev-IL, ma usa un insieme di puntatori a funzione che vengono reinizializzati
        // all'avvio. 
        
        DevILIsEnable   = ndINFO_GetValue (NDDEVIL_SUPPORT_IS_OPERATIVE, 0, 0, 0);
        xilGenImages    = ndINFO_GetValue (NDDEVIL_ILGENIMAGE_ADR, 0, 0, 0);
        xilBindImage    = ndINFO_GetValue (NDDEVIL_ILBINDIMAGE_ADR, 0, 0, 0);
        xilLoadL        = ndINFO_GetValue (NDDEVIL_ILLOADL_ADR, 0, 0, 0);
        xilCopyPixels   = ndINFO_GetValue (NDDEVIL_ILCOPYPIXEL_ADR, 0, 0, 0);
        xilDeleteImages = ndINFO_GetValue (NDDEVIL_ILDELETEIMAGES_ADR, 0, 0, 0);
        
        return;                  
    }
    else     // I servizi multimediali sono disattivati: segnala il failure         
    {
     ndJpegDecoderIsFailure           = 1;
     ndJpegDecoderState               = ND_JPEGDEC_DISABLED_BY_USER;
     ndJpegDecoderSecondaryErrorCode  = 0;
     return;     
    } 
}


char ndHAL_HWJPG_EnableJpegDecoder (int NewActor, int NewHwResCode)
/*
     Questa funzione provvede a cambiare la risoluzione attuale del gestore
     di decodifica JPEG hardware. La routine provvede anche a lockare la
     risorsa in modo che altri sistemi non possano cambiare i parametri 
     di sistema finchè il sistema che ha impegnato il decoder non ha
     terminato il proprio lavoro. 
     
     La funzione restituisce 0 se non ci sono errori ed un valore negativo
     in caso di errore. Sono previsti i seguenti codici di errore:
           
     ERR_SYSTEM_IS_BUSY            Un altro sistema sta utilizzando il decoder
     ERR_WRONG_PARAMETERS          Il codice di risoluzione è errato
     ERR_IO_ERROR                  Errore nel decodificatore JPEG
*/
{
    int _JpegDimX, _JpegDimY, PSPCodeResolution;
    int result;
    
    if ( (ndJpegActualActor==ND_JPEGDEC_NOACTOR) || (ndJpegActualActor==NewActor) )
    {
         if (ndJpegHardwareResCode!=NewHwResCode)
         {
                result=INTERNAL_GOCAM_GenerateHwResolutionCodes (NewHwResCode, &PSPCodeResolution, &_JpegDimX, &_JpegDimY);
                if (result<0) return ERR_WRONG_PARAMETERS;  

                switch (NewHwResCode)
                {
                       case ND_RESOLUTION_CODE_176_144:
                       case ND_RESOLUTION_CODE_320_240:
                       case ND_RESOLUTION_CODE_352_288:
                       case ND_RESOLUTION_CODE_640_480:               
                       case ND_RESOLUTION_CODE_480_272:
                       {
                            // Arrivati a questo punto, il sistema deve verificare se già era
                            // attiva una sessione del decodificatore JPEG hardware
                            
                            if (ndJpegActualActor!=ND_JPEGDEC_NOACTOR)
                            {
                                sceJpegDeleteMJpeg ();
                                ndJpegActualActor = ND_JPEGDEC_NOACTOR;                                      
                            }
                            
                            // Ora provvedi ad attivare la nuova istanza del decodificatore JPEG
                            
                            result = sceJpegCreateMJpeg (_JpegDimX, _JpegDimY);
                    	    if (result >= 0)
                    	    {
                                 // Impegna il decodificatore hardware      
                                 ndJpegActualActor = NewActor;      
                                 
                                 // Registra il nuovo codice di risoluzione
                                 ndJpegHardwareResCode = NewHwResCode;
                                 
                                 // Emulazione software JPEG disattivata
                                 ndJpegSoftwareEmulation = 0;
                                 
                                 // Segnala successo
                                 return 0;
                            }
                            else
                               return ERR_IO_ERROR; 
                       }
    
                       case ND_RESOLUTION_CODE_1024_768:
                       case ND_RESOLUTION_CODE_1280_960:
                       {
                            if (DevILIsEnable)
                            {
                                    // Il firmware della PSP non consente l'utilizzo del motore hardware per
                                    // la decompressione JPEG per immagini di dimensioni superiori a 640*480
                                    // pixel. Per tale ragione, per tale tipo di immagini la decompressione
                                    // viene eseguita via software per mezzo dell'interfaccia DevIL
                
                                    // Genera il codice che identifica l'immagine DevIL 
                                    (*xilGenImages)(1, &EMU_DEVIL_JPG_Image);
                                    
                                    // Predispone la libreria DevIL per ricevere le successive chiamate ed usarle
                                    // sull'immagine 
                                    (*xilBindImage)(EMU_DEVIL_JPG_Image);
                                    
                                    // Registra le dimensioni necessarie dentro i due pseudoregistri dedicati 
                                    _SoftJpegDimX=_JpegDimX;
                                    _SoftJpegDimY=_JpegDimY;
                                    
                                    // Impegna il decodificatore hardware      
                                    ndJpegActualActor = NewActor;      
                                 
                                    // Registra il nuovo codice di risoluzione
                                    ndJpegHardwareResCode = NewHwResCode;
                                    
                                    // Emulazione software JPEG attivata
                                    ndJpegSoftwareEmulation = 1;
                                    
                                    // Segnala successo
                                    return 0;
                            }
                            else    // Il supporto Dev-IL non è disponibile
                               return ERR_FUNCTION_HAS_BEEN_DISABLED;
                            
                       }
                }           // End of switch                                          
         }
         else              // Non ci sono cambiamenti nel codice di risoluzione: non fare nulla
           return 0;       // Simula successo
    }
    else    // Qualcun altro ha impegnato il decoder hardware  
      return ERR_SYSTEM_IS_BUSY;
}


int ndHAL_HWJPG_DecodeMJpeg (char *jpegbuf,	int size, void *rgba)
/*
    Esegue in hardware la decodifica di una immagine JPEG. L'immagine è
    localizzata nel buffer jpegbuf, ha dimensioni pari a size, e 
    *rgba è un puntatore ad un'area di memoria dove l'immagine sarà
    decodificata in formato RGBA
*/
{
    if (size>0)
    {
        if (!ndJpegSoftwareEmulation)
        {
            return sceJpegDecodeMJpeg (jpegbuf, size, rgba, 0);
        }
        else
        {
            if (DevILIsEnable)
            {        
                    (*xilLoadL)(IL_JPG, jpegbuf, FRAMEBUFFER_SIZE);
                    // Questa operazione provvederà a decomprimere le immagini in formato JPG
                    // L'immagine viene caricata dal buffer lump (dove è compressa) dentro 
                    // l'immagine DevIL 
          
                    (*xilCopyPixels)(0, 0, 0, _SoftJpegDimX, _SoftJpegDimY, 1, IL_RGBA, IL_UNSIGNED_BYTE, CAM_FrameBuffer );
                    
                    return 0;                                  // Simula successo 
             }
             else 
               return ERR_IO_ERROR; 
        }
    }
    else                 // Inviata una immagine di size pari a 0. Simula successo
      return 0;
} 


void ndHAL_HWJPG_DisableJpegDecoder (void)
/*
     Provvede a disabilitare il decoder JPEG hardware del sistema
*/
{
    if (ndJpegActualActor!=ND_JPEGDEC_NOACTOR) 
    {
         if (!ndJpegSoftwareEmulation)
         {
            sceJpegDeleteMJpeg ();
         }
         else
         {
            if (DevILIsEnable)
            {
               (*xilDeleteImages)(1, &EMU_DEVIL_JPG_Image);
            }
         }
         
         ndJpegActualActor = ND_JPEGDEC_NOACTOR;                                             
    }
}





void INTERNAL_TerminateMJPEGDecoder (void)
{
    if (!ND_HWMODE_DisableMediaService)
    {
         if (!ndJpegDecoderIsFailure)
         {
            // Chiudi eventuali sessioni del decoder JPEG hardware attive
            ndHAL_HWJPG_DisableJpegDecoder ();
            
            // Termina il decoder JPEG hardware
            sceJpegFinishMJpeg();
         } 
    }  
}
