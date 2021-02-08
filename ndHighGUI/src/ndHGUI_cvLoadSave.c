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
     ROUTINES FOR LOADING/SAVING IMAGES
    ---------------------------------------------------------------------------   
*/

#include <$_NDHGUI_for_NDHGUI.h>



IplImage *cvConvertNDItoIPLImage (struct ndImage_Type *MyImage, int ColorDepth, int NrChannels, int Magn)
/*------------------------------------------------------------------------------------------------------
  Routine cvConvertNDItoIPLImage                                                             Pubblica
  -------------------------------------------------------------------------------------------------------
  La presente routine provvede a trasformare un immagine che sia stata memorizzata nel sistema ed
  allocata in una struttura di tipo ndImage_Type in una immagine di tipo IPL compatibile con le
  routine OpenCV.
  
  Se il parametro NrChannels viene posto a 1, la routine produce un'immagine a toni di grigio. Se
  invece il parametro Channels è posto a 3, viene prodotta un'immagine RGB.
  
  Sono previsti i seguenti codici di errore:
      NDHGUI_IMAGE_NOT_INITIALIZED            Immagine non inizializzata
      NDHGUI_WRONG_COLOR_DEPTH                Profondità di colore errata
      NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS      Nr. di canali errato
      NDHGUI_NDIMAGE_NOT_INITIALIZED          L'immagine Nanodesktop non è inizializzata
  --------------------------------------------------------------------------------------------------------
*/

{
    unsigned short int CounterX, CounterY, CounterZX, CounterZY;
    unsigned short int *PointerToData;
    char ErrRep;
            
    CvSize ImageSize;
    
    IplImage *ImageDest;
    
    
    // --------------
    
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
            
    if (MyImage->Data!=0)                                       // Verifica che l'immagine sia già stata allocata
    {                                                         
          ImageSize.width  = Magn * MyImage->LenX;
          ImageSize.height = Magn * MyImage->LenY;
                    
          ImageDest = cvCreateImage (ImageSize, ColorDepth, NrChannels);
          
          ErrRep=INTERNAL_SetupHGUIDstRegisters (ImageDest);            // Setta i registri dst
          
          if (!ErrRep)
          {
                  for (CounterY=0; CounterY<MyImage->LenY; CounterY++)
                  {
                      for (CounterX=0; CounterX<MyImage->LenX; CounterX++)
                      {
                          ndIMG_GetPixelFromImage_RGB (MyImage, CounterX, CounterY, &HGUI_ColorValue [0], &HGUI_ColorValue [1], &HGUI_ColorValue [2]);
                          // Preleva il valore RGB dall'immagine Nanodesktop
                          
                          INTERNAL_ConvertCurrentPixel(); 
                          // Converte il pixel in un formato compatibile con l'immagine di destinazione:
                          // Adesso nei registri DST ci sono i dati richiesti
                          
                          for (CounterZY=0; CounterZY<Magn; CounterZY++)
                          {
                                for (CounterZX=0; CounterZX<Magn; CounterZX++)
                                {
                                    INTERNAL_PutPixelToCurrentImage (Magn*CounterX+CounterZX, Magn*CounterY+CounterZY);
                                }
                          }            
                      }
                  }
                  
                  HGUI_SystemError=0;
                  return ImageDest;
          }     
          else  // Si è verificato un errore nel gestire i registri di destinazione
          {
              switch (ErrRep)
              {
                       case NDHGUI_MSG_IMAGE_NOT_ALLOCATED:
                       {
                           HGUI_NotifyError (-2, 0, "cvConvertNDItoIPLImage", "Image is not initialized");
                                                   
                           HGUI_SystemError=NDHGUI_IMAGE_NOT_INITIALIZED;           
                           return 0;  
                       }
                       
                       case NDHGUI_MSG_WRONG_COLOR_DEPTH:
                       {
                           HGUI_NotifyError (-2, 0, "cvConvertNDItoIPLImage", "Error in color depth");
                             
                           HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH;
                           return 0; 
                       }
                         
                       case NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS:
                       {
                           HGUI_NotifyError (-2, 0, "cvConvertNDItoIPLImage", "Wrong number of channel");
                             
                           HGUI_SystemError = NDHGUI_WRONG_NR_CHANNELS;
                           return 0;
                       }
                         
                       default:
                       {
                           HGUI_NotifyError (-2, 0, "cvConvertNDItoIPLImage", "Unknown error in image");
                           // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                           // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                           // non prima però di avere generato un errore 
                             
                           HGUI_SystemError = NDHGUI_UNKNOWN_ERROR;
                           return 0;
                       }     
               }  
          }

    } 
    else
    {
        HGUI_NotifyError (-2, 0, "cvConvertNDItoIPLImage", "Nanodesktop image not initialized");
                             
        HGUI_SystemError = NDHGUI_NDIMAGE_NOT_INITIALIZED;
        return 0;
    }  

}


char ndIMG_ConvertIPLtoNDIImage (struct ndImage_Type *MyImage, IplImage *MyIPLImage, float ScaleX, float ScaleY, char IsColor, char ColorFormat)
/*
    -----------------------------------------------------------------------------------
    Routine ndIMG_ConvertIPLtoNDIImage                                      Pubblica
    -----------------------------------------------------------------------------------
    Questa routine provvede a trasformare una certa immagine di tipo IPL (generata dalle
    OpenCV) nella corrispondente immagine di tipo Nanodesktop, gestibile dalle normali
    routine di visualizzazione. 
    
    L'allocazione dello spazio necessario in memoria avviene automaticamente: si tenga
    presente che se in MyImage era già stata allocata un immagine, questa verrà 
    distrutta e sostituita dall'equivalente dell'immagine IPLImage.
    
    La routine tiene conto di eventuali ROI e COI impostate nell'immagine. Inoltre,
    la routine tiene conto dei parametri ScaledX e ScaledY che consentono di
    ottenere un'immagine più grande o più piccola dell'immagine di partenza. 
    
    Il parametro IsColor deve essere posto a 0, se si vuole un'immagine in bianco e
    nero. Malgrado l'eventuale conversione in bianco e nero però, dal punto di vista
    di Nanodesktop l'immagine sarà sempre a colori. Il formato colore dell'equivalente
    Nanodesktop viene deciso dal parametro ColorFormat: può essere NDMGKNB o NDRGB.
    
    Questa routine è da considerarsi come facente parte di Nanodesktop piuttosto che
    di ndHighGUI: non avviene notifica degli errori, ma questi invece vengono 
    segnalati per mezzo di appositi exitcode (nello stile delle routines di Nanodesktop...)
    
       ERR_MEMORY_OVERFLOW          Non c'è abbastanza memoria per contenere l'immagine
       ERR_WRONG_PARAMETERS         I parametri di larghezza o altezza immagine sono sbagliati
                                    oppure ColorFormat non è riconosciuto
       ERR_IPLIMAGE_NOT_ALLOCATED   L'immagine IPL non è allocata o esiste qualche errore 
                                    all'interno di essa
    ------------------------------------------------------------------------------------------
*/
{
    char ErrRep;
    
    int ImageLenX, ImageLenY;
    int ROIX1, ROIY1, ROIX2, ROIY2, COI;
    
    int CounterX, CounterY;
    
    
    // Inizio del codice
    
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
    
    ErrRep=INTERNAL_SetupHGUIRegisters (MyIPLImage);
     
    if (!ErrRep)
    {
        if ((ScaleX!=0) && (ScaleY!=0))
        {
                   if ((MyIPLImage->roi)==0)
                   {
                      ROIX1=0;
                      ROIY1=0;
                      ROIX2=MyIPLImage->width;
                      ROIY2=MyIPLImage->height;
                      COI=0;
                   }
                   else
                   {
                      ROIX1=MyIPLImage->roi->xOffset;
                      ROIY1=MyIPLImage->roi->yOffset;
                      ROIX2=MyIPLImage->roi->xOffset + MyIPLImage->roi->width;
                      ROIY2=MyIPLImage->roi->yOffset + MyIPLImage->roi->height;
                      COI=MyIPLImage->roi->coi;
                   }
                
                   ImageLenX = (float)(ROIX2 - ROIX1)*ScaleX;
                   ImageLenY = (float)(ROIY2 - ROIY1)*ScaleY;
                   
                   ErrRep=ndIMG_CreateImage (MyImage, ImageLenX, ImageLenY, ColorFormat);
                   
                   if (!ErrRep)
                   {
                      for (CounterY=0; CounterY<ImageLenY; CounterY++)
                      {
                          for (CounterX=0; CounterX<ImageLenX; CounterX++)
                          {
                              INTERNAL_GetPixelFromCurrentImage ( ROIX1 + (float)CounterX/(float)ScaleX, ROIY1 + (float)CounterY/(float)ScaleY );
                              
                              if (IsColor==1) 
                              {
                                 HGUI_NrChannels_Dst=3;
                                 HGUI_ColorDepth_Dst=IPL_DEPTH_8U;
                              } 
                              else 
                              {
                                 HGUI_NrChannels_Dst=1;
                                 HGUI_ColorDepth_Dst=IPL_DEPTH_8U;
                              }
                         
                              INTERNAL_ConvertCurrentPixel ();
                              // Converte i valori dell'immagine di partenza in modo che il risultato sia sempre una immagine
                              // a RGB con 256 livelli per canale. I nuovi valori sono nei registri dst
                         
                              if (COI != 0) 
                              {
                                 if (COI != 1) HGUI_ColorValue_Dst [0]=0;
                                 if (COI != 2) HGUI_ColorValue_Dst [1]=0;
                                 if (COI != 3) HGUI_ColorValue_Dst [2]=0;
                              };
                              // Provvede ad annullare tutti i canali che non corrispondono all'eventuale channel of
                              // interest settato
                         
                              ndIMG_PutPixelToImage_RGB (MyImage, CounterX, CounterY, HGUI_ColorValue_Dst [0], HGUI_ColorValue_Dst [1], HGUI_ColorValue_Dst [2]);
                              // Salva il pixel nell'immagine
                         }
                       }
                       
                       return 0;  
                   }      
                   else   // Il processo di allocazione della ndImage temporanea è fallito  
                      return ErrRep;
                      
        }
        else          // Uno dei parametri scale è a 0
          return   ERR_WRONG_PARAMETERS;
       
    }     // C'è stato un problema nell'inizializzazione dei registri HGUI
    else     
      return ERR_IPLIMAGE_NOT_ALLOCATED;
}



void cvConvertImage (IplImage *ImageSrc, IplImage *ImageDest, int flags)
/* ---------------------------------------------------------------------------
   Routine cvConvertImage                                      Pubblica
   ---------------------------------------------------------------------------
   La routine cvConvertImage provvede a convertire un'immagine da un formato
   ad un altro. Il comportamento è identico a quello della cvConvertImage
   delle Win32 OpenCV.
   
   Sono supportati due flags:
        CV_CVTIMG_FLIP       Inverte verticalmente l'immagine
        CV_CVTIMG_SWAP_RB    Scambia i canali rosso e blu
        
   Sono previsti i seguenti codici di errore:
        NDHGUI_THE_IMAGE_MUST_HAVE_SAME_DIMS  Le immagini devono avere le stesse 
                                              dimensioni
        NDHGUI_UNKNOWN_ERROR_IMG_SRC          Errore sconosciuto nell'immagine
                                              sorgente
        NDHGUI_WRONG_NRCHANNELS_IMG_SRC       Numero di canali errato nel codice
                                              sorgente
        NDHGUI_WRONG_COLOR_DEPTH_IMG_SRC      Profondità di colore errata nell'
                                              immagine sorgente
        NDHGUI_IMAGE_SRC_NOT_INITIALIZED      Immagine sorgente non inizializzata
        NDHGUI_UNKNOWN_ERROR_IMG_DST          Errore sconosciuto nell'immagine
                                              destinazione
        NDHGUI_WRONG_NRCHANNELS_IMG_DST       Numero di canali errato nell'immagine
                                              di destinazione
        NDHGUI_WRONG_COLOR_DEPTH_IMG_DST      Profondità di colore errata 
                                              nell'immagine di destinazione
        NDHGUI_IMAGE_DST_NOT_INITIALIZED      Immagine dest non inizializzata
   ----------------------------------------------------------------------------
*/
{

   int CounterX, CounterY;
   int PixelDestX, PixelDestY;
   
   float FlipCh0, FlipCh1, FlipCh2;
   
   unsigned short int FlipChannel, FlipImage;
   char ErrRep;

   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
   
   
   if ((ImageSrc->width==ImageDest->width) && (ImageSrc->height==ImageDest->height))      // Verifica che siano uguali le dimensioni dell'immagine
   {
      FlipImage   = (flags & CV_CVTIMG_FLIP);
      FlipChannel = (flags & CV_CVTIMG_SWAP_RB);
      
      ErrRep=INTERNAL_SetupHGUIRegisters (ImageSrc);
      // Setta gli pseudoregistri HGUI sorgente
      
      if (!ErrRep)
      {
          ErrRep=INTERNAL_SetupHGUIDstRegisters (ImageDest);
          // Setta gli pseudoregistri HGUI di destinazione
          
          if (!ErrRep)
          {                                               
                                                                                              
              for (CounterY=0; CounterY<ImageSrc->height; CounterY++)
              {
                  for (CounterX=0; CounterX<ImageSrc->width; CounterX++)
                  {
                      INTERNAL_GetPixelFromCurrentImage (CounterX, CounterY);
                      // Adesso negli pseudoregistri HGUI ci sono i valori canale del pixel che
                      // stiamo leggendo
                      
                      INTERNAL_ConvertCurrentPixel ();
                      // Esegue la conversione: i canali convertiti sono negli pseudoregistri 
                      // HGUI dst
                      
                      if (!FlipImage)
                      {
                           PixelDestX=CounterX;
                           PixelDestY=CounterY;
                      }
                      else
                      {
                           PixelDestX=CounterX;
                           PixelDestY=ImageSrc->height - CounterY - 1;
                      }
                      
                      if (FlipChannel)
                      {
                           if (ImageDest->depth!=IPL_DEPTH_32F)
                           {
                               // Copia i dati nei registri flip-channel in modo da poterli invertire                                
                                                               
                               FlipCh0 = HGUI_ColorValue_Dst [0];
                               FlipCh1 = HGUI_ColorValue_Dst [1];
                               FlipCh2 = HGUI_ColorValue_Dst [2]; 
                               
                               // Ricopia i dati nei registri HGUI invertiti
                               
                               HGUI_ColorValue_Dst [0]=FlipCh2;
                               HGUI_ColorValue_Dst [1]=FlipCh1;
                               HGUI_ColorValue_Dst [2]=FlipCh0;
                           }
                           else
                           {
                               // Copia i dati nei registri flip-channel in modo da poterli invertire                                
                                                               
                               FlipCh0 = HGUI_FlColorValue_Dst [0];
                               FlipCh1 = HGUI_FlColorValue_Dst [1];
                               FlipCh2 = HGUI_FlColorValue_Dst [2]; 
                               
                               // Ricopia i dati nei registri HGUI invertiti
                               
                               HGUI_FlColorValue_Dst [0]=FlipCh2;
                               HGUI_FlColorValue_Dst [1]=FlipCh1;
                               HGUI_FlColorValue_Dst [2]=FlipCh0; 
                           }
                      }
                      

                      INTERNAL_PutPixelToCurrentImage (PixelDestX, PixelDestY);
                      // Scrive i dati convertiti nell'immagine di destinazione 
                                                                                                                                          
                  }     // Chiude i due cicli FOR nidificati
              }      // Chiude i due cicli FOR nidificati

              HGUI_SystemError = 0;        // Segnala che è tutto ok
              return;

          }
          else   // Gestisci gli errori nell'immagine destinazione
          {
                   switch (ErrRep)
                   {
                     case NDHGUI_MSG_IMAGE_NOT_ALLOCATED:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Image dest is not initialized");
                         
                         HGUI_SystemError = NDHGUI_IMAGE_DST_NOT_INITIALIZED;
                         return;
                     } 
                     
                     case NDHGUI_MSG_WRONG_COLOR_DEPTH:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Error in color depth in image dest");
                         
                         HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH_IMG_DST;
                         return; 
                     }
                     
                     case NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Wrong number of channel in image dest");
                         
                         HGUI_SystemError = NDHGUI_WRONG_NRCHANNELS_IMG_DST;
                         return;
                     }
                     
                     default:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Unknown error in image dest");
                         // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                         // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                         // non prima però di avere generato un errore 
                         
                         HGUI_SystemError = NDHGUI_UNKNOWN_ERROR_IMG_DST;
                         return;
                     }
                   }
              
          }       
      
      }
      else // Gestisci gli errori nell'immagine sorgente
      {
                   switch (ErrRep)
                   {
                     case NDHGUI_MSG_IMAGE_NOT_ALLOCATED:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Image src is not initialized");
                         
                         HGUI_SystemError = NDHGUI_IMAGE_SRC_NOT_INITIALIZED;
                         return;
                     } 
                     
                     case NDHGUI_MSG_WRONG_COLOR_DEPTH:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Error in color depth in image src");
                         
                         HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH_IMG_SRC;
                         return; 
                     }
                     
                     case NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Wrong number of channel in image src");
                         
                         HGUI_SystemError = NDHGUI_WRONG_NRCHANNELS_IMG_SRC;
                         return;
                     }
                     
                     default:
                     {
                         HGUI_NotifyError (-2, 0, "cvConvertImage", "Unknown error in image src");
                         // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                         // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                         // non prima però di avere generato un errore 
                         
                         HGUI_SystemError = NDHGUI_UNKNOWN_ERROR_IMG_SRC;
                         return;
                     }
                   }
              
      }                                                  
   }
   else
   {
      HGUI_NotifyError (-2, 0, "cvConvertImage", "The images must have same dimensions");   
      
      HGUI_SystemError = NDHGUI_THE_IMAGE_MUST_HAVE_SAME_DIMS;
      return;
   } 
                           
}



/* 
   ----------------------------------------------------------------------------
   ROUTINES PER IL CARICAMENTO ED IL SALVATAGGIO DELLE IMMAGINI DA DISCO
   ----------------------------------------------------------------------------
*/

    
IplImage *cvLoadImage (char *filename, int iscolor )
/* ----------------------------------------------------------------------------
   Routine cvLoadImage                                           Pubblica
   ----------------------------------------------------------------------------
   Questa routine consente di caricare un file.
   
   La routine si appoggia sull'API per il caricamento delle immagini di
   Nanodesktop. Potranno essere aperte solo le immagini che sono
   supportate da Nanodesktop. 
   
   Pertanto, ove Nanodesktop fosse stato compilato, senza supporto per
   Dev-IL, sarà possibile aprire solo file bitmap, mentre se Nanodesktop
   è stato compilato con il supporto per Dev-IL, sarà possibile utilizzare
   i file che sono supportati da tale libreria.
   
   Se si pone iscolor pari a 0, la struttura IPLImage restituita sarà in
   toni di grigio, anche se l'immagine di partenza è a colori.
   
   In tutti gli altri casi l'immagine di uscita sarà in colori BGR, con profondità
   di colore 8 bit per canale. L'utente, dopo avere richiamato cvLoadImage, 
   può richiamare cvCvtColor o cvConvertImage per cambiare la profondità di colore o
   il numero di bit per pixel.
   
   In uscita viene fornito l'indirizzo della struttura IplImage che
   è stata allocate per contenere l'immagine. 
   
   Qualora invece si fosse verificato un errore, in uscita viene fornito
   il valore negativo -1 e viene settata la variabile HGUI_SystemError al
   codice di errore opportuno.
   
   Sono previsti i seguenti codici di errore:
      NDHGUI_FILE_NOT_FOUND        File non trovato
      NDHGUI_FORMAT_NOT_SUPPORTED  Formato non supportato 
      NDHGUI_IO_ERROR              Errore di I/O o sconosciuto
   ----------------------------------------------------------------------------
*/
{
                                                                               
   struct ndImage_Type ndImage;
   char ErrRep;
   
   int CounterX, CounterY;
   
   unsigned char ndChannelR, ndChannelG, ndChannelB;
   
   CvSize ImageSize;
   IplImage *NewIPLImage;
   
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
     
   ErrRep=ndIMG_LoadImage (&ndImage, filename, NDRGB);
   
   if (!ErrRep)
   {
     ImageSize.width  = ndImage.LenX;
     ImageSize.height = ndImage.LenY; 
                                           
     if (iscolor)
     {
        NewIPLImage = cvCreateImage ( ImageSize, 8, 3 );
        // I dati vengono inizialmente posti in una immagine temporanea con 3 canali,
        // profondità di colore 8 bit per canale (244 bpp).
     }
     else
     {
        NewIPLImage = cvCreateImage ( ImageSize, 8, 1 );
        // Salvo che IsColor sia a zero. In tal caso l'immagine sarà a toni di grigio
     }        
     
     INTERNAL_SetupHGUIDstRegisters (NewIPLImage);      // Esegui il setup dei registri HGUI
                         
     for (CounterY=0; CounterY<ndImage.LenY; CounterY++)
     {
         for (CounterX=0; CounterX<ndImage.LenX; CounterX++)
         {
            ndIMG_GetPixelFromImage_RGB (&ndImage, CounterX, CounterY, &ndChannelR, &ndChannelG, &ndChannelB);
            
            HGUI_ColorValue_Dst [0]=ndChannelR;
            HGUI_ColorValue_Dst [1]=ndChannelG;
            HGUI_ColorValue_Dst [2]=ndChannelB;
            
            INTERNAL_PutPixelToCurrentImage (CounterX, CounterY);                         
         }
     }    
   
     ndIMG_ReleaseImage (&ndImage);
     
     HGUI_SystemError = 0;         // Segnala che è tutto ok
     return NewIPLImage;
 
   }
   else
   {
       switch (ErrRep)
       {
               case ERR_FILE_NOT_FOUND:
               {
                  HGUI_NotifyError (-2, 0, "cvLoadImage", "File not found");    
                  
                  HGUI_SystemError=NDHGUI_FILE_NOT_FOUND; 
                  return -1;
               };
               
               case ERR_FORMAT_NOT_SUPPORTED:
               {
                  HGUI_NotifyError (-210, 0, "cvLoadImage", "Unsupported format");
                  
                  HGUI_SystemError=NDHGUI_FORMAT_NOT_SUPPORTED;
                  return -1;
               };
               
               default:
               {
                  HGUI_NotifyError (-2, 0, "cvLoadImage", "Errore di IO o non identificato");
                  
                  HGUI_SystemError=NDHGUI_IO_ERROR;
                  return -1;
               }
       }
   }
}   
   


void cvSaveImage (char *filename, IplImage *image)
/* 
   ----------------------------------------------------------------------------------------
   Routine cvSaveImage                                                  Pubblica
   ----------------------------------------------------------------------------------------
   Questa routine salva su disco l'immagine contenuta nella struttura IplImage, in un
   file il cui percorso completo è contenuto in filename. 
   
   L'immagine salvata sarà sempre a colori, anche se l'immagine di partenza è a toni 
   di grigio. 
   
   Se l'immagine di partenza è a toni di grigio, verrà applicato un algoritmo per 
   fare apparire l'immagine come se fosse a toni di grigio (malgrado essa continui 
   a mantenere 3 canali).
   
   La routine è sensibile ai parametri COI e ROI. Di conseguenza, è possibile salvare su
   disco anche solo un canale ovvero una parte dell'immagine. 
   
   Non sono previsti valori in uscita, ma il buon esito dell'operazione può essere
   verificato attraverso la variabile HGUI_SystemError, che sarà impostata ad un
   valore diverso da 0 in caso di errore.
   
   Sono previsti i seguenti codici di errore:
       NDHGUI_IMAGE_NOT_INITIALIZED            Immagine non inizializzata 
       NDHGUI_WRONG_COLOR_DEPTH                Profondità di colore immagine errata
       NDHGUI_WRONG_NR_CHANNELS                Nr. di channels errato
       NDHGUI_OUT_OF_MEMORY                    Fuori dalla memoria
       NDHGUI_FORMAT_NOT_SUPPORTED             Formato non supportato
       NDHGUI_CANNOT_CREATE_FILE               Non posso creare il file
       NDHGUI_UNKNOWN_ERROR_IN_FILE_WRITING    Errore sconosciuto in scrittura
   ----------------------------------------------------------------------------------------
*/   

{
    
    int ImageLenX, ImageLenY;
    int ROIX1, ROIY1, ROIX2, ROIY2, COI;
    
    int CounterX, CounterY;
    char ErrRep;
    
    struct ndImage_Type TempImage; 
    
    // Inizio del codice
    
    if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
    ErrRep=INTERNAL_SetupHGUIRegisters (image);
    // Setta i pseudoregistri per l'analisi dell'immagine di partenza         
          
    if (!ErrRep)
    {
           if ((image->roi)==0)
           {
              ROIX1=0;
              ROIY1=0;
              ROIX2=image->width;
              ROIY2=image->height;
              COI=0;
           }
           else
           {
              ROIX1=image->roi->xOffset;
              ROIY1=image->roi->yOffset;
              ROIX2=image->roi->xOffset + image->roi->width;
              ROIY2=image->roi->yOffset + image->roi->height;
              COI=image->roi->coi;
           }
        
           ImageLenX = ROIX2 - ROIX1;
           ImageLenY = ROIY2 - ROIY1;
           
           ErrRep=ndIMG_CreateImage (&TempImage, ImageLenX, ImageLenY, NDRGB);
           // Crea un'immagine Nanodesktop in modo da richiamare l'API per il salvataggio
           // delle immagini
           
           if (!ErrRep)
           {
                       
              for (CounterY=0; CounterY<ImageLenY; CounterY++)
              {
                  for (CounterX=0; CounterX<ImageLenX; CounterX++)
                  {
                      INTERNAL_GetPixelFromCurrentImage (ROIX1 + CounterX, ROIY1 + CounterY);
                      // Deposita le informazioni dentro gli pseudoregistri
                      
                      HGUI_ColorDepth_Dst = 8;
                      HGUI_NrChannels_Dst = 3;
                      
                      INTERNAL_ConvertCurrentPixel ();
                      // Adesso nei registri Dst c'è l'equivalente del pixel ad 8 bit 3 canali
                      
                      // Provvede ad annullare tutti i canali che non corrispondono all'eventuale channel of
                      // interest settato
                 
                      if (COI != 0) 
                      {
                         if (COI != 1) HGUI_ColorValue_Dst [0]=0;
                         if (COI != 2) HGUI_ColorValue_Dst [1]=0;
                         if (COI != 3) HGUI_ColorValue_Dst [2]=0;
                      };
                      
                      // Salva l'immagine nella TempImage
                      
                      ndIMG_PutPixelToImage_RGB (&TempImage, CounterX, CounterY, 
                                HGUI_ColorValue_Dst [0], HGUI_ColorValue_Dst [1], HGUI_ColorValue_Dst [2]);
                      
                 }
               }
               
               ErrRep = ndIMG_SaveImage (&TempImage, filename);
               // Provvede al salvataggio usando l'API Nanodesktop
               
               if (!ErrRep)
               {
                    ndIMG_ReleaseImage (&TempImage);
                    // Indipendentemente da come sono andate le cose, disalloca l'immagine temporanea   
                           
                    HGUI_SystemError=0;
                    return;
               }
               else
               {
                   ndIMG_ReleaseImage (&TempImage);
                   // Indipendentemente da come sono andate le cose, disalloca l'immagine temporanea   
               
                   switch (ErrRep)
                   {
                          case ERR_FORMAT_NOT_SUPPORTED:
                          {
                               HGUI_NotifyError (-210, 0, "cvSaveImage", "Unsupported format");
                               
                               HGUI_SystemError = NDHGUI_FORMAT_NOT_SUPPORTED;
                               return;
                          }
                          
                          case ERR_CANNOT_CREATE_FILE:
                          {
                               HGUI_NotifyError (-210, 0, "cvSaveImage", "I cannot create file on disk");
                               
                               HGUI_SystemError = NDHGUI_CANNOT_CREATE_FILE;
                               return;
                          }
                          
                          default:
                          {
                               HGUI_NotifyError (-2, 0, "cvSaveImage", "Unknown error in file writing");
                               
                               HGUI_SystemError = NDHGUI_UNKNOWN_ERROR_IN_FILE_WRITING;
                               return;
                          }
                   }
               }
                 
           }      
           else   // Il processo di allocazione della ndImage temporanea è fallito  
           {
              HGUI_NotifyError (-2, 0, "cvSaveImage", "Out of memory");
              
              HGUI_SystemError=NDHGUI_OUT_OF_MEMORY;
              return; 
           }
           
         
    }
    else   // Errore nel settaggio dei pseudoregistri
    {
        switch (ErrRep)
        {
               case NDHGUI_MSG_IMAGE_NOT_ALLOCATED:
               {
                   HGUI_NotifyError (-2, 0, "cvSaveImage", "Image is not initialized");
                                           
                   HGUI_SystemError=NDHGUI_IMAGE_NOT_INITIALIZED;           
                   return;  
               }
               
               case NDHGUI_MSG_WRONG_COLOR_DEPTH:
               {
                   HGUI_NotifyError (-2, 0, "cvSaveImage", "Error in color depth");
                     
                   HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH;
                   return; 
               }
                 
               case NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS:
               {
                   HGUI_NotifyError (-2, 0, "cvSaveImage", "Wrong number of channel");
                     
                   HGUI_SystemError = NDHGUI_WRONG_NR_CHANNELS;
                   return;
               }
                 
               default:
               {
                   HGUI_NotifyError (-2, 0, "cvSaveImage", "Unknown error in image");
                   // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                   // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                   // non prima però di avere generato un errore 
                     
                   HGUI_SystemError = NDHGUI_UNKNOWN_ERROR;
                   return;
               }
        
        }          // End of switch           
    }                                            
}

