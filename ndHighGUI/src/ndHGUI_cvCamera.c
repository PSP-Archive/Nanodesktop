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
    ---------------------------------------------------------------------------
     ndHighGUI WEBCAM SUPPORT
    ---------------------------------------------------------------------------   
*/

#include <$_NDHGUI_for_NDHGUI.h>

CvCapture *cvCaptureFromCAM (int WebCamID)
/* 
   ----------------------------------------------------------------------------
   Routine cvCaptureFromCAM                                      Pubblica
   ----------------------------------------------------------------------------
   Questa routine reinizializza la webcam definita da WebCamID, preparandola per
   ricevere i dati. Se non ci sono errori, alloca dinamicamente la struttura
   cvCapture e ne restituisce l'indirizzo alla routine chiamante. 

   Se si pone WebCamID a -1, verrà usata la prima webcam disponibile
   
   Se ci sono degli errori, la routine restituisce 0 e setta la variabile
   HGUI_SystemError al codice di errore corrispondente. 
   
   Sono previsti i seguenti codici di errore:
     NDHGUI_CAMERA_IS_NOT_AVAILABLE   La webcam non è disponibile
     NDHGUI_MEMORY_ALLOCATION_ERROR   Errore di allocazione della memoria
   ----------------------------------------------------------------------------
*/
{
   int ndCameraHandle; 
   char ErrRep;
     
   CvCapture *PntToNewCaptureStruct;
   
         
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto 
   
   if (WebCamID==-1) WebCamID=0;
   
   /*
      L'API Nanodesktop prevede che la prima videocamera, se c'è, abbia usb handle 0.
      Quando l'utente pone il parametro a -1, significa che richiede una qualsiasi
      webcam e noi scegliamo la webcam con usb handle 0.
   */ 
   
   ndCameraHandle = ndHAL_CAM_ActivateCamera (WebCamID);
   
   if (ndCameraHandle>=0)       // Nessun errore
   {
       // Alloca in memoria la struttura richiesta
       PntToNewCaptureStruct = malloc (sizeof (CvCapture));
    
       if (PntToNewCaptureStruct!=0)          // Sono riuscito a creare la struct cvCapture necessaria
       {
           PntToNewCaptureStruct->CamHandle           = ndCameraHandle;
           PntToNewCaptureStruct->cvGrabFrameStatus   = 0;
           
           return PntToNewCaptureStruct;
       }
       else       // Non sono riuscito ad allocare la struttura CvCapture
       {
           HGUI_NotifyError (-2, 0, "cvCaptureFromCAM", "Error in CvCapture allocation");               
           HGUI_SystemError = NDHGUI_MEMORY_ALLOCATION_ERROR;
           return 0;  
       }
   }        
   else
   {
      HGUI_NotifyError (-2, 0, "cvCaptureFromCAM", "Camera is not available");
                             
      HGUI_SystemError = NDHGUI_CAMERA_IS_NOT_AVAILABLE;
      return 0;  
   }
}



int cvGrabFrame (CvCapture *PntToCvCapture)
/*
    ----------------------------------------------------------------------------
    Routine cvGrabFrame                                      Pubblica
    ----------------------------------------------------------------------------
    Questa routine provvede a grabbare un frame dalla webcam. Verrà usata
    la webcam il cui ID è contenuto nella struttura CvCapture che viene
    puntata dal parametro PntToCvCapture. 
    
    La routine restituisce 1 se l'operazione di grab va a buon fine e
    0 in caso contrario.
    
    Se ci sono degli errori, la routine restituisce 0 e setta la variabile
    HGUI_SystemError al codice di errore corrispondente. 
   
    Sono previsti i seguenti codici di errore:
         
    NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO Non riesco ad ottenere le informazioni
                                      di configurazione dall'Eyeserver   
    NDHGUI_I_CANNOT_EXEC_NULL_FLASH   Non è possibile eseguire il primo scatto
                                      in NULL      
    NDHGUI_MEMORY_ALLOCATION_ERROR    Errore di allocazione della memoria 
    NDHGUI_I_CANNOT_GRAB_THE_IMAGE    Non posso grabbare l'immagine
    ----------------------------------------------------------------------------   
*/
{
    int ndCam_FrameCounter, ndCam_TransferMode;
   
    char ErrRep;
    
    IplImage  *PntToGrabbedFrame;
    CvSize    ImageSize;
    
    // Esegui, ove necessario, la reinizializzazione del frame buffer
    
    
    if (PntToCvCapture->cvGrabFrameStatus == 0)      // Il grab frame non è stato inizializzato ancora
    {
        // Comanda all'Eyeserver di eseguire uno scatto in NULL. Questa operazione è
        // necessaria perchè in questo modo l'Eyeserver potrà inizializzare la
        // webcam collegata e settare automaticamente le impostazioni relative 
        // alle dimensioni del frame                          
                                    
        ErrRep = ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_NULL, 0, 0, 0);
        
        if (!ErrRep)
        {
                // Richiedi ad Eyeserver di fornirti informazioni sulla modalità attualmente settata:
                // si noti che ora le informazioni di dimensione sono settate grazie al fatto che
                // prima abbiamo grabbato in NULL. 
                    
                ErrRep = ndHAL_CAM_GetInfo (PntToCvCapture->CamHandle, &(ImageSize.width), &(ImageSize.height), &(ndCam_FrameCounter), &(ndCam_TransferMode)  );
                  
                if (!ErrRep)
                {
                              // Crea un'immagine che abbia le dimensioni dell'immagine rilevata dalla webcam
                                
                              if ( (ndCam_TransferMode==16) || (ndCam_TransferMode==24) )
                              {
                                 PntToGrabbedFrame = cvCreateImage (ImageSize, 8, 3);
                              }
                              
                              if (ndCam_TransferMode==8)
                              {
                                 PntToGrabbedFrame = cvCreateImage (ImageSize, 8, 1); 
                              }
                      
                              if (PntToGrabbedFrame!=0)         // Sono riuscito a creare l'immagine in memoria
                              {
                                   PntToCvCapture->CamHandle         = PntToCvCapture->CamHandle;
                                   PntToCvCapture->CamSizeX          = ImageSize.width;
                                   PntToCvCapture->CamSizeY          = ImageSize.height;
                                   PntToCvCapture->GrabbedFrame      = PntToGrabbedFrame;
                                   
                                   PntToCvCapture->cvGrabFrameStatus = 1;
                              }  
                              else     // Non sono riuscito a creare il grabbed frame buffer
                              {
                                  HGUI_NotifyError (-2, 0, "cvGrabFrame", "Error in framebuffer allocation");
                                                     
                                  HGUI_SystemError = NDHGUI_MEMORY_ALLOCATION_ERROR;
                                  return 0;  
                              }        
                }
                else    // Non riesco ad ottenere informazioni dall'Eyeserver
                {
                        HGUI_NotifyError (-2, 0, "cvGrabFrame", "I cannot retrieve Eyeserver info");
                                             
                        HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                        return 0;  
                }   
        }
        else
        {
                HGUI_NotifyError (-2, 0, "cvGrabFrame", "Impossible to execute null flash");
                                 
                HGUI_SystemError = NDHGUI_I_CANNOT_EXEC_NULL_FLASH;
                return 0; 
        }
    }
    
    
    
    // A questo punto, il sistema avrà di sicuro reinizializzato il grabbed frame buffer 
    
    switch (PntToCvCapture->cvGrabFrameStatus)
    {
           case 1:
           {
                ErrRep=ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_CVIMAGE, PntToCvCapture->GrabbedFrame, ND_FORCECAM, RENDER);
                
                if (!ErrRep)                  // Capture a buon fine
                {
                     PntToCvCapture->cvGrabFrameStatus=2;
                     return 1;
                }
                else
                {
                     HGUI_NotifyError (-2, -ErrRep, "cvGrabFrame", "Impossible to grab the image");
                             
                     HGUI_SystemError = NDHGUI_I_CANNOT_GRAB_THE_IMAGE;
                     return 0; 
                }
            
                break;
           }
           
           case 2:
           {
                ErrRep=ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_CVIMAGE, PntToCvCapture->GrabbedFrame, 0, RENDER); 
                
                if (!ErrRep)                  // Capture a buon fine
                {
                     return 1;
                }
                else
                {
                     HGUI_NotifyError (-2, -ErrRep, "cvGrabFrame", "Impossible to grab the image");
                             
                     HGUI_SystemError = NDHGUI_I_CANNOT_GRAB_THE_IMAGE;
                     return 0; 
                }
                
                break;
           }
    }
      
} 


IplImage *cvRetrieveFrame (CvCapture *PntToCvCapture)
/*
    ----------------------------------------------------------------------------
    Routine cvRetrieveFrame                                      Pubblica
    ----------------------------------------------------------------------------
    Questa routine restituisce l'indirizzo del grab frame buffer 
    relativo alla struttura CvCapture.
    ----------------------------------------------------------------------------   
*/
{
    if (PntToCvCapture->cvGrabFrameStatus != 0)
    {
        return PntToCvCapture->GrabbedFrame;
    }
    else
    {
        return 0;
    }
}



IplImage *cvQueryFrame (CvCapture *PntToCvCapture)
/*
    ----------------------------------------------------------------------------
    Routine cvQueryFrame                                      Pubblica
    ----------------------------------------------------------------------------
    Questa routine esegue un grab dalla videocamera ed in seguito restituisce 
    l'indirizzo del grab frame buffer relativo alla struttura CvCapture.
    ----------------------------------------------------------------------------   
*/
{
    int HGUIRep;
    
    HGUIRep = cvGrabFrame (PntToCvCapture);
    
    if (HGUIRep==1)          // Se è a 1 vuol dire che è stata grabbata un'immagine
    {
       return cvRetrieveFrame (PntToCvCapture);  
    }
    else                     // Condizione di errore
    {
       return 0;
    }
}



void cvReleaseCapture (CvCapture **PntToPntToCvCapture)
/*
    ----------------------------------------------------------------------------
    Routine cvReleaseCapture                                 Pubblica
    ----------------------------------------------------------------------------
    Questa routine si occupa di rilasciare la struttura cvCapture che era
    stata allocata in precedenza per accedere alla webcam
    ----------------------------------------------------------------------------   
*/
{
   CvCapture *PntToCvCapture;
   
   PntToCvCapture = *PntToPntToCvCapture;
   
   if (PntToCvCapture!=0)
   {
      ndHAL_CAM_DisableCamera (PntToCvCapture->CamHandle);
   } 
}





int cvSetCaptureProperty (CvCapture *PntToCvCapture, int property_id, double Value)
/*
    ----------------------------------------------------------------------------
    Routine cvSetCaptureProperty                                 Pubblica
    ----------------------------------------------------------------------------
    Questa routine si occupa di settare le caratteristiche dell'immagine. 
    Il parametro property_id può assumere uno tra i possibili valori dati
    dalle seguenti costanti manifeste:
          
    CV_CAP_PROP_TRASMISSION_MODE      Modalità di trasmissione: double 8,16,24
    CV_CAP_PROP_FRAME_WIDTH           Setta la width dell'immagine
    CV_CAP_PROP_FRAME_HEIGHT          Setta la height dell'immagine
    ----------------------------------------------------------------------------   
*/
{
    int ErrRep;
    
    int EyeSvr_CamSizeX, EyeSvr_CamSizeY;
    int EyeSvr_FrameCounter, EyeSvr_TransferMode;
    
    switch (property_id)
    {
           case CV_CAP_PROP_TRASMISSION_MODE:
           {
                        ErrRep=ndHAL_CAM_ChangeTransferMode (PntToCvCapture->CamHandle, Value);
    
                        if (!ErrRep)
                        {
                             if (PntToCvCapture->cvGrabFrameStatus>0)
                             {
                                  cvReleaseImage ( &(PntToCvCapture->GrabbedFrame) );
                                  
                                  PntToCvCapture->cvGrabFrameStatus = 0;                // Farà in modo che venga creato un nuovo framebuffer
                             }                                                          // alla prossima chiamata
                        }    
                
                        return 1;
                        break;
           } 
           
           case CV_CAP_PROP_POS_MSEC:
           {
                        return -1;             
           } 
           
           case CV_CAP_PROP_POS_FRAMES:
           {
                        return -1;              
           }
           
           case CV_CAP_PROP_POS_AVI_RATIO:
           {
                        return -1;                  
           }
           
           case CV_CAP_PROP_FRAME_WIDTH:
           {
                        if (PntToCvCapture->cvGrabFrameStatus==0)    // Non è stato ancora creato un Grab Frame Buffer
                        {
                             ErrRep = ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_NULL, 0, 0, 0);                                    
                        
                             if (!ErrRep)
                             {
                                  // Richiedi ad Eyeserver di fornirti informazioni sulla modalità attualmente settata:
                                  // si noti che ora le informazioni di dimensione sono settate grazie al fatto che
                                  // prima abbiamo grabbato in NULL. 
                    
                                  ErrRep = ndHAL_CAM_GetInfo (PntToCvCapture->CamHandle, &(EyeSvr_CamSizeX), &(EyeSvr_CamSizeY), &(EyeSvr_FrameCounter), &(EyeSvr_TransferMode)  );
                       
                                  if (ErrRep)
                                  {
                                       HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "I cannot retrieve Eyeserver info");
                                             
                                       HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                       return -1;        
                                  }
                             }
                             else
                             {
                                 HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "Impossible to execute null flash");
                                             
                                 HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                 return -1; 
                             }
                        }
                        else // cvGrabFrameStatus è posto ad 1 o a 2: le dimensioni delle immagini sono in EyeSvr_CamSizeX o in EyeSvr_CamSizeY
                        {
                            EyeSvr_CamSizeX = PntToCvCapture->CamSizeX;
                            EyeSvr_CamSizeY = PntToCvCapture->CamSizeY;
                        }   
                        
                        // Adesso abbiamo in EyeSvr_CamSizeX e EyeSvr_CamSizeY le coordinate X ed Y
                        
                        ErrRep = ndHAL_CAM_ChangeResolution (PntToCvCapture->CamHandle, Value, EyeSvr_CamSizeY); 
                        
                        if (!ErrRep)
                        {
                             if (PntToCvCapture->cvGrabFrameStatus>0)
                             {
                                  cvReleaseImage ( &(PntToCvCapture->GrabbedFrame) );
                                  
                                  PntToCvCapture->cvGrabFrameStatus = 0;                // Farà in modo che venga creato un nuovo framebuffer
                             }                                                          // alla prossima chiamata
                        
                             return 1;     // Segnala successo
                        }
                        else
                        {
                             HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "ndHAL_CAM: I cannot change resolution");
                                             
                             HGUI_SystemError = NDHGUI_I_CANNOT_CHANGE_RESOLUTION;
                             return -1;
                        }
                        
                        break;                                                             
           }
           
           case CV_CAP_PROP_FRAME_HEIGHT:
           {
                        if (PntToCvCapture->cvGrabFrameStatus==0)    // Non è stato ancora creato un Grab Frame Buffer
                        {
                             ErrRep = ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_NULL, 0, 0, 0);                                    
                        
                             if (!ErrRep)
                             {
                                  // Richiedi ad Eyeserver di fornirti informazioni sulla modalità attualmente settata:
                                  // si noti che ora le informazioni di dimensione sono settate grazie al fatto che
                                  // prima abbiamo grabbato in NULL. 
                    
                                  ErrRep = ndHAL_CAM_GetInfo (PntToCvCapture->CamHandle, &(EyeSvr_CamSizeX), &(EyeSvr_CamSizeY), &(EyeSvr_FrameCounter), &(EyeSvr_TransferMode)  );
                       
                                  if (ErrRep)
                                  {
                                       HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "I cannot retrieve Eyeserver info");
                                             
                                       HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                       return -1;        
                                  }
                             }
                             else
                             {
                                 HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "Impossible to execute null flash");
                                             
                                 HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                 return -1; 
                             }
                        }
                        else // cvGrabFrameStatus è posto ad 1 o a 2: le dimensioni delle immagini sono in EyeSvr_CamSizeX o in EyeSvr_CamSizeY
                        {
                            EyeSvr_CamSizeX = PntToCvCapture->CamSizeX;
                            EyeSvr_CamSizeY = PntToCvCapture->CamSizeY;
                        }   
                        
                        // Adesso abbiamo in EyeSvr_CamSizeX e EyeSvr_CamSizeY le coordinate X ed Y
                        
                        ErrRep = ndHAL_CAM_ChangeResolution (PntToCvCapture->CamHandle, EyeSvr_CamSizeX, Value); 
                        
                        if (!ErrRep)
                        {
                             if (PntToCvCapture->cvGrabFrameStatus>0)
                             {
                                  cvReleaseImage ( &(PntToCvCapture->GrabbedFrame) );
                                  
                                  PntToCvCapture->cvGrabFrameStatus = 0;                // Farà in modo che venga creato un nuovo framebuffer
                             }                                                          // alla prossima chiamata
                        
                             return 1;     // Segnala successo
                        }
                        else
                        {
                             HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "ndHAL_CAM: I cannot change resolution");
                                             
                             HGUI_SystemError = NDHGUI_I_CANNOT_CHANGE_RESOLUTION;
                             return -1;
                        }
                        
                        break;                              
           }
           
           case CV_CAP_PROP_FPS:
           {
                        return 8;        
           }
           
           case CV_CAP_PROP_FOURCC:
           {
                        return -1;           
           }
           
           case CV_CAP_SENSOR_MODE:
           {
                        ErrRep=ndHAL_CAM_ChangeSensorMode (PntToCvCapture->CamHandle, Value);
                        
                        if (!ErrRep)
                        {
                             return 1;     // Segnala successo      
                        }
                        else
                        {
                             HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "ndHAL_CAM: I cannot change CCD mode");
                             HGUI_SystemError = NDHGUI_I_CANNOT_CHANGE_CCD_MODE;
                             
                             return -1;
                        }
           } 
           
           case CV_CAP_HW_RESOLUTION:
           {
                        ErrRep=ndHAL_CAM_ChangeHwResolution (PntToCvCapture->CamHandle, Value);
                        
                        if (!ErrRep)
                        {
                             if (PntToCvCapture->cvGrabFrameStatus>0)
                             {
                                  cvReleaseImage ( &(PntToCvCapture->GrabbedFrame) );
                                  
                                  PntToCvCapture->cvGrabFrameStatus = 0;                // Farà in modo che venga creato un nuovo framebuffer
                             }                                                          // alla prossima chiamata
                             
                             return 0;  
                        }    
                        else
                        {
                             HGUI_NotifyError (-2, 0, "cvSetCaptureProperty", "ndHAL_CAM: I cannot change CCD resolution");
                             HGUI_SystemError = NDHGUI_I_CANNOT_CHANGE_CCD_RESOLUTION;
                             
                             return -1;
                        }         
           }
           
           
    }
}



int cvGetCaptureProperty (CvCapture *PntToCvCapture, int property_id)
/*
    ----------------------------------------------------------------------------
    Routine cvGetCaptureProperty                                 Pubblica
    ----------------------------------------------------------------------------
    Questa routine si occupa di prelevare informazioni sullo stato attuale
    della webcam. 
    Il parametro property_id può assumere uno tra i possibili valori dati
    dalle seguenti costanti manifeste:
          
    CV_CAP_PROP_TRASMISSION_MODE      Modalità di trasmissione: double 8,16,24
    CV_CAP_PROP_FRAME_WIDTH           Restituisce la width dell'immagine
    CV_CAP_PROP_FRAME_HEIGHT          Restituisce la height dell'immagine
    ----------------------------------------------------------------------------   
*/
{
    int ErrRep;
    
    int EyeSvr_CamSizeX,     EyeSvr_CamSizeY;
    int EyeSvr_FrameCounter, EyeSvr_TransferMode;
    
    switch (property_id)
    {
           case CV_CAP_PROP_TRASMISSION_MODE:
           {
                        ErrRep = ndHAL_CAM_GetInfo (PntToCvCapture->CamHandle, &(EyeSvr_CamSizeX), &(EyeSvr_CamSizeY), &(EyeSvr_FrameCounter), &(EyeSvr_TransferMode)  );
                       
                        if (!ErrRep)
                           return (EyeSvr_TransferMode);                                                                                      
                        else
                           return -1;
                        
                        break;
           } 
           
           case CV_CAP_PROP_POS_MSEC:
           {
                        return -1;             
           } 
           
           case CV_CAP_PROP_POS_FRAMES:
           {
                        return -1;              
           }
           
           case CV_CAP_PROP_POS_AVI_RATIO:
           {
                        return -1;                  
           }
           
           case CV_CAP_PROP_FRAME_WIDTH:
           {
                        if (PntToCvCapture->cvGrabFrameStatus==0)    // Non è stato ancora creato un Grab Frame Buffer
                        {
                             ErrRep = ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_NULL, 0, 0, 0);                                    
                        
                             if (!ErrRep)
                             {
                                  // Richiedi ad Eyeserver di fornirti informazioni sulla modalità attualmente settata:
                                  // si noti che ora le informazioni di dimensione sono settate grazie al fatto che
                                  // prima abbiamo grabbato in NULL. 
                    
                                  ErrRep = ndHAL_CAM_GetInfo (PntToCvCapture->CamHandle, &(EyeSvr_CamSizeX), &(EyeSvr_CamSizeY), &(EyeSvr_FrameCounter), &(EyeSvr_TransferMode)  );
                       
                                  if (!ErrRep)
                                  {
                                       return EyeSvr_CamSizeX;       
                                  }
                                  else
                                  {
                                       HGUI_NotifyError (-2, 0, "cvGetCaptureProperty", "I cannot retrieve Eyeserver info");
                                             
                                       HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                       return -1;        
                                  }
                             }
                             else
                             {
                                 HGUI_NotifyError (-2, 0, "cvGetCaptureProperty", "Impossible to execute null flash");
                                             
                                 HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                 return -1; 
                             }
                        }
                        else // cvGrabFrameStatus è posto ad 1 o a 2: le dimensioni delle immagini sono in EyeSvr_CamSizeX o in EyeSvr_CamSizeY
                        {
                            return PntToCvCapture->CamSizeX;  
                        }   
                        
                        
                        break;                                                             
           }
           
           case CV_CAP_PROP_FRAME_HEIGHT:
           {
                        if (PntToCvCapture->cvGrabFrameStatus==0)    // Non è stato ancora creato un Grab Frame Buffer
                        {
                             ErrRep = ndHAL_CAM_GrabNewImage (PntToCvCapture->CamHandle, ND_CAM_TO_NULL, 0, 0, 0);                                    
                        
                             if (!ErrRep)
                             {
                                  // Richiedi ad Eyeserver di fornirti informazioni sulla modalità attualmente settata:
                                  // si noti che ora le informazioni di dimensione sono settate grazie al fatto che
                                  // prima abbiamo grabbato in NULL. 
                    
                                  ErrRep = ndHAL_CAM_GetInfo (PntToCvCapture->CamHandle, &(EyeSvr_CamSizeX), &(EyeSvr_CamSizeY), &(EyeSvr_FrameCounter), &(EyeSvr_TransferMode)  );
                       
                                  if (!ErrRep)
                                  {
                                       return EyeSvr_CamSizeY;       
                                  }
                                  else
                                  {
                                       HGUI_NotifyError (-2, 0, "cvGetCaptureProperty", "I cannot retrieve Eyeserver info");
                                             
                                       HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                       return -1;        
                                  }
                             }
                             else
                             {
                                 HGUI_NotifyError (-2, 0, "cvGetCaptureProperty", "Impossible to execute null flash");
                                             
                                 HGUI_SystemError = NDHGUI_I_CANNOT_RETRIEVE_EYESERVER_INFO;
                                 return -1; 
                             }
                             
                        }
                        else // cvGrabFrameStatus è posto ad 1 o a 2: le dimensioni delle immagini è in EyeSvr_CamSizeX o in EyeSvr_CamSizeY
                        {
                            return PntToCvCapture->CamSizeY; 
                        }   
                        
                        break;                              
           }
           
           case CV_CAP_PROP_FPS:
           {
                        return 8;        
           }
           
           case CV_CAP_PROP_FOURCC:
           {
                        return -1;           
           } 
    }
}


CvCapture *cvCaptureFromFile (char *NameFile)
/*
    ----------------------------------------------------------------------------
    Routine cvCaptureFromFile                                 Pubblica
    ----------------------------------------------------------------------------
    Questa routine dovrebbe occuparsi di eseguire il grabbing da un file. 
    
    Allo stato attuale ndOpenCV non supporta la cattura da file. La routine
    viene mantenuta qui, fittiziamente, solo per esigenze di compatibilità
    con il codice OpenCV. 
    
    La routine restituisce sempre 0 (nelle OpenCV originali questo equivale
    ad un errore)
    ----------------------------------------------------------------------------
*/
{
    return 0;
}
   
