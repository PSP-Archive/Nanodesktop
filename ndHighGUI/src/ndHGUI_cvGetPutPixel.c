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
    -----------------------------------------------------------------------------
    ROUTINES GETPIXEL, PUTPIXEL, CONVERTPIXEL
    -----------------------------------------------------------------------------
    Queste routines sono in grado di operare sui pixel di un'immagine. Le
    routines erano utilizzate in passato dalla libreria per operazioni come
    il caricamento, la visualizzazione o il salvataggio di immagini. 
    
    Nell'attuale versione della libreria, vengono usate delle routines interne
    per questo scopo, basate sul sistema dei registri HGUI: ciò consente di
    ottenere una maggiore velocità. Tuttavia, le routines precedenti sono
    state reingegnerizzate per usare anch'esse gli pseudoregistri HGUI e 
    vengono mantenute nella libreria per esclusive ragioni di compatibilità
    -----------------------------------------------------------------------------
*/

#include <$_NDHGUI_for_NDHGUI.h>


inline TypeColor cvRGBToMagicNumber (struct HGUIPixelType *MyHGUIPixel )
/* ----------------------------------------------------------------------------
   Routine cvRGBToMagicNumber                                      Pubblica
   ----------------------------------------------------------------------------
   Questa routine si occupa di convertire il colore di un pixel, espresso
   a toni grigio o per mezzo delle sue componenti RGB, in un valore detto
   MagicNumber, che è quello che deve essere immesso nella mappa colori 
   della piattaforma su cui gira in quel momento NanoDesktop.
   
   I dati necessari sono passati per mezzo della struttura HGUIPixelType,
   nel seguente modo:
       
       PosX e PosY sono ignorati;
       ColorDepth indica a cvRGBToMagicNumber la profondità di colore (e la presenza
                  o meno del segno)
       NrChannels indica il numero di canali (1 per pixel a toni di grigio, 4
                  per pixel il cui colore è espresso per mezzo delle componenti RGB)
       
       C1         indica il colore R (o il valore del grigio per immagini GRAY)
       C2         indica il colore G
       C3         indica il colore B
   
   Sono previsti i seguenti codici di errore:
      NDHGUI_WRONG_COLOR_DEPTH     Profondità di colore errata     
      NDHGUI_WRONG_NR_CHANNELS     Numero di canali errato
   ----------------------------------------------------------------------------
*/

{
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   // Per implementare la funzione, è necessario semplicemente copiare
   // i valori necessari nei corretti pseudoregistri HGUI.
                                    
   switch (MyHGUIPixel->NrChannels)
   {
          case 1:
            {
                   switch (HGUI_ColorDepth)
                   {
                           case IPL_DEPTH_8U:
                           case IPL_DEPTH_8S:
                           case IPL_DEPTH_16U:
                           case IPL_DEPTH_16S:
                           case IPL_DEPTH_32S:
                                {
                                    HGUI_ColorValue [0] = MyHGUIPixel->C1;
                                    
                                    HGUI_ColorDepth     = MyHGUIPixel->ColorDepth;
                                    HGUI_NrChannels     = 1;
                                    
                                    HGUI_SystemError=0;
                                    return  INTERNAL_CurrentRGBToMagicNumber ();
                                }
                           case IPL_DEPTH_32F:
                                {
                                    // Per convertire un valore float, i dati devono essere 
                                    // trasferiti negli pseudoregistri HGUI_FlColorValue,
                                    // perchè INTERNAL_CurrentRGBToMagicNumber si aspetta
                                    // di trovare là i dati
                                    
                                    HGUI_FlColorValue [0] = MyHGUIPixel->C1;
                                    
                                    HGUI_ColorDepth       = MyHGUIPixel->ColorDepth;
                                    HGUI_NrChannels       = 1;
                                    
                                    HGUI_SystemError=0;
                                    return  INTERNAL_CurrentRGBToMagicNumber ();       
                                } 
                           default:
                                {
                                    HGUI_NotifyError (-2, 0, "cvRGBToMagicNumber", "Wrong color depth");
                                    // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                                    // ha qualche errore. Allora, la routine si limita ad uscire restituendo 0,                          
                                    // non prima però di avere generato un errore 
                                    
                                    HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH; 
                                    return 0;
                                } 
                   }                // Fine dello switch di secondo livello A
                   
                   
            }
          case 3:
          case 4:
            {                              
                   switch (HGUI_ColorDepth)
                   {
                           case IPL_DEPTH_8U:
                           case IPL_DEPTH_8S:
                           case IPL_DEPTH_16U:
                           case IPL_DEPTH_16S:
                           case IPL_DEPTH_32S:
                                {
                                    HGUI_ColorValue [0] = MyHGUIPixel->C1;
                                    HGUI_ColorValue [1] = MyHGUIPixel->C2;
                                    HGUI_ColorValue [2] = MyHGUIPixel->C3;
                                    
                                    HGUI_ColorDepth     = MyHGUIPixel->ColorDepth;
                                    HGUI_NrChannels     = 3;
                                    
                                    HGUI_SystemError=0;
                                    return  INTERNAL_CurrentRGBToMagicNumber ();
                                }
                           case IPL_DEPTH_32F:
                                {
                                    // Per convertire un valore float, i dati devono essere 
                                    // trasferiti negli pseudoregistri HGUI_FlColorValue,
                                    // perchè INTERNAL_CurrentRGBToMagicNumber si aspetta
                                    // di trovare là i dati
                                    
                                    HGUI_FlColorValue [0] = MyHGUIPixel->C1;
                                    HGUI_FlColorValue [1] = MyHGUIPixel->C2;
                                    HGUI_FlColorValue [2] = MyHGUIPixel->C3;
                                    
                                    HGUI_ColorDepth       = MyHGUIPixel->ColorDepth;
                                    HGUI_NrChannels       = 3;
                                    
                                    HGUI_SystemError=0;
                                    return  INTERNAL_CurrentRGBToMagicNumber ();       
                                } 
                           default:
                                {
                                    HGUI_NotifyError (-2, 0, "cvRGBToMagicNumber", "Wrong color depth");
                                    // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                                    // ha qualche errore. Allora, la routine si limita ad uscire restituendo 0,                          
                                    // non prima però di avere generato un errore 
                                    
                                    HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH;
                                    return 0;
                                } 
                   }                // Fine dello switch di secondo livello B
                   
                   
            }
          default:        // Se il codice arriva qui, vuol dire che il numero di canali è errato
            {
                 HGUI_NotifyError (-2, 0, "cvRGBToMagicNumber", "Wrong number of channels");
                 
                 HGUI_SystemError = NDHGUI_WRONG_NR_CHANNELS;
                 return 0;  
            }
   }             // Fine dello switch di primo livello      

} 
   
   
   


inline void cvConvertPixel (struct HGUIPixelType *Record1, struct HGUIPixelType *Record2)
/*
   ----------------------------------------------------------------------------
   Routine cvConvertPixel                                       Pubblica
   ----------------------------------------------------------------------------
   Questa routine si occupa di convertire un pixel con una determinata
   profondità di colore/numero di canali, in un altro pixel con una
   diversa profondità di colore/numero di canali. La profondità di colore
   ed il numero di canali di ingresso e di uscita vengono prelevati 
   dai relativi campi nelle strutture Record1 e Record2. 
   
   E' compito della routine chiamante allocare (anche in stack), due strutture 
   di tipo struct HGUIPixelType e passarne l'indirizzo alla routine come parametro.
   
   I nuovi valori colore possono essere prelevati dai campi C1, C2 e C3 
   al ritorno dalla chiamata alla funzione.

   La routine supporta teoricamente anche conversioni in spazio colore a 
   4 canali, ma il dato relativo al quarto canale viene sempre ignorato.
   
   Non sono previsti codici di ritorno per questa routine, ma un eventuale
   errore viene memorizzato nella variabili HGUI_SystemError.
   
   Sono previsti i seguenti codici di errore:
       NDHGUI_WRONG_NRCHANNELS_IMG_SRC       Nr. di canali errato nell'immagine
                                             sorgente
       NDHGUI_WRONG_NRCHANNELS_IMG_DST       Nr. di canali errato nell'immagine
                                             destinazione
       NDHGUI_WRONG_COLOR_DEPTH_IMG_SRC      Profondità di colore errata nell'immagine
                                             sorgente
       NDHGUI_WRONG_COLOR_DEPTH_IMG_DST      Profondità di colore errata nell'immagine
                                             di destinazione                                     
   ---------------------------------------------------------------------------- 
*/

{
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   // Eseguiamo dei controlli sul numero di canali, in modo da segnalare un
   // eventuale condizione di errore                                                                             
   
   switch (Record1->NrChannels)
   {
          case 1:
          case 3:
               break;
          default:
               {
                  HGUI_NotifyError (-2, 0, "cvConvertPixel", "Wrong number of channels in src");   
                  
                  HGUI_SystemError = NDHGUI_WRONG_NRCHANNELS_IMG_SRC;
                  return;
               }   
   }
   
   switch (Record2->NrChannels)
   {
          case 1:
          case 3:
               break;
          default:
               {
                  HGUI_NotifyError (-2, 0, "cvConvertPixel", "Wrong number of channels in dest");   
                  
                  HGUI_SystemError = NDHGUI_WRONG_NRCHANNELS_IMG_DST;
                  return;
               }   
   }
   
   
   // Eseguiamo ora un ulteriore controllo sulle profondità di colore:
   
   switch (Record1->ColorDepth)
   {
          case IPL_DEPTH_8U:
          case IPL_DEPTH_8S:
          case IPL_DEPTH_16U:
          case IPL_DEPTH_16S:
          case IPL_DEPTH_32S:
          case IPL_DEPTH_32F:
               break;
      default:
      {
          HGUI_NotifyError (-2, 0, "cvConvertPixel", "Wrong color depth");
          
          HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH_IMG_SRC;
          return;
      }   
   }
   
   switch (Record2->ColorDepth)
   {
          case IPL_DEPTH_8U:
          case IPL_DEPTH_8S:
          case IPL_DEPTH_16U:
          case IPL_DEPTH_16S:
          case IPL_DEPTH_32S:
          case IPL_DEPTH_32F:
               break;
      default:
      {
          HGUI_NotifyError (-2, 0, "cvConvertPixel", "Wrong color depth");
          
          HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH_IMG_DST;
          return;
      }   
   }
   
   // Carichiamo i dati negli pseudo-registri  
    
   if (Record1->ColorDepth != IPL_DEPTH_32F)
   {
       HGUI_ColorValue [0] = Record1->C1;
       HGUI_ColorValue [1] = Record1->C2;
       HGUI_ColorValue [2] = Record1->C3;
   }
   else
   {
       HGUI_FlColorValue [0] = Record1->C1;
       HGUI_FlColorValue [1] = Record1->C2;
       HGUI_FlColorValue [2] = Record1->C3;
   }
   
   HGUI_ColorDepth = Record1->ColorDepth;
   HGUI_NrChannels = Record1->NrChannels;
   
   // Ed ora le informazioni relative ai valori che che si vogliono in
   // destinazione
   
   HGUI_ColorDepth_Dst = Record2->ColorDepth;
   HGUI_NrChannels_Dst = Record2->NrChannels;
   
   INTERNAL_ConvertCurrentPixel ();
   
   // Ora le informazioni che servono sono negli pseudoregistri di destinazione 
     
   if (Record2->ColorDepth != IPL_DEPTH_32F)
   {
         Record2->C1 = HGUI_ColorValue_Dst [0];
         Record2->C2 = HGUI_ColorValue_Dst [1];
         Record2->C3 = HGUI_ColorValue_Dst [2];
   }
   else
   {
         Record2->C1 = HGUI_FlColorValue_Dst [0];
         Record2->C2 = HGUI_FlColorValue_Dst [1];
         Record2->C3 = HGUI_FlColorValue_Dst [2];
   }
   
   HGUI_SystemError=0;        // Segnala che è tutto a posto
   return;
}




inline void cvPutPixelToImage  (IplImage *MyIPLImage, struct HGUIPixelType *MyHGUIPixel )                        
/* -----------------------------------------------------------------------------------
   Routine cvPutPixelToImage                                             Pubblica
   -----------------------------------------------------------------------------------
   Questa routine consente di inserire un punto all'interno di un'immagine la cui struct
   sia già stata creata ed allocata da cvCreateImage. La routine viene usata da routine
   come xcvConvertNDIMGtoCVImage per scrivere l'immagine all'interno dell'array in modo
   compatibile con le routine OpenCV.
   
   La posizione del punto viene memorizzata nei campi PosX e PosY della struttura
   MyHGUIPixel  PRIMA della chiamata. Vengono anche memorizzati i colori C1, C2, C3.
   
   La profondità di colore sarà sempre quella dell'immagine MyIPLImage, indipendentemente
   dal valore del campo ColorDepth della struttura MyHGUIPixel , che viene ignorato. 
   
   Non sono previsti codici di ritorno per questa routine: gli eventuali errori vengono
   restituiti per mezzo della variabile di ambiente HGUI_SystemError. 
   
   Sono previsti i seguenti codici di errore:
      NDHGUI_PIXEL_OUT_OF_IMAGE       Richiesta pixel fuori immagine
      NDHGUI_IMAGE_NOT_INITIALIZED    Immagine non inizializzata
      NDHGUI_WRONG_NR_CHANNELS        Numero di canali errato
      NDHGUI_WRONG_COLOR_DEPTH        Profondità di colore errata
      NDHGUI_UNKNOWN_ERROR            Errore sconosciuto
   ------------------------------------------------------------------------------------
*/ 

{  
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   char ErrRep;
   
   ErrRep=INTERNAL_SetupHGUIDstRegisters (MyIPLImage);
   // Setta gli pseudoregistri HGUI di destinazione
          
   if (!ErrRep)
   {
      if  ( (MyHGUIPixel->PosX<MyIPLImage->width) && (MyHGUIPixel->PosY<MyIPLImage->height) )
      // Verifichiamo che la richiesta non sia fuori immagine
      { 
              if (MyIPLImage->depth != IPL_DEPTH_32F)
              {
                   HGUI_ColorValue_Dst [0]   = MyHGUIPixel->C1;
                   HGUI_ColorValue_Dst [1]   = MyHGUIPixel->C2;
                   HGUI_ColorValue_Dst [2]   = MyHGUIPixel->C3;
              }
              else
              {
                   HGUI_FlColorValue_Dst [0] = MyHGUIPixel->C1;
                   HGUI_FlColorValue_Dst [1] = MyHGUIPixel->C2;
                   HGUI_FlColorValue_Dst [2] = MyHGUIPixel->C3;
              }
           
              INTERNAL_PutPixelToCurrentImage (MyHGUIPixel->PosX, MyHGUIPixel->PosY);
              
              HGUI_SystemError=0;             // Segnala che è tutto a posto
              return;
      }
      else    // Richiesta pixel fuori immagine
      {
          HGUI_NotifyError (-2, 0, "cvPutPixelToImage", "Pixel request is out of image");
          
          HGUI_SystemError = NDHGUI_PIXEL_OUT_OF_IMAGE;
          return;       
      }
   }
   else  // C'è un errore: eseguine la gestione
   {
       switch (ErrRep)
       {
             case NDHGUI_MSG_IMAGE_NOT_ALLOCATED:
             {
                 HGUI_NotifyError (-2, 0, "cvPutPixelToImage", "Image is not initialized");
                 
                 HGUI_SystemError = NDHGUI_IMAGE_NOT_INITIALIZED;
                 return;
             } 
             
             case NDHGUI_MSG_WRONG_COLOR_DEPTH:
             {
                 HGUI_NotifyError (-2, 0, "cvPutPixelToImage", "Error in color depth");
                 
                 HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH;
                 return; 
             }
             
             case NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS:
             {
                 HGUI_NotifyError (-2, 0, "cvPutPixelToImage", "Wrong number of channel");
                 
                 HGUI_SystemError = NDHGUI_WRONG_NR_CHANNELS;
                 return;
             }
             
             default:
             {
                 HGUI_NotifyError (-2, 0, "cvPutPixelToImage", "Unknown error in image");
                 // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                 // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                 // non prima però di avere generato un errore 
                 
                 HGUI_SystemError = NDHGUI_UNKNOWN_ERROR;
                 return;
             }
       }
   }
         
}        
                                                                                  



inline void cvGetPixelFromImage   (IplImage *MyIPLImage, struct HGUIPixelType *MyHGUIPixel )
/* -----------------------------------------------------------------------------------
   Routine cvGetPixelFromImage                                        Pubblica
   -----------------------------------------------------------------------------------
   Questa routine permette di estrarre le caratteristiche di un pixel da un 
   immagine che sia già stata reinizializzata per mezzo di una chiamata a
   cvCreateImage. 
   
   I dati sul pixel vengono depositati in una struct di tipo
   struct HGUIPixelType. E' compito della funzione chiamante passare a cvGetPixelFromImage
   l'indirizzo di una struttura di tipo struct HGUIPixelType che deve essere stata
   allocata in precedenza (anche in stack). 
   
   La posizione del pixel non viene passata come parametro, ma deve invece essere
   memorizzata dentro la struttura MyHGUIPixel  nei campi PosX e PosY prima di
   eseguire la chiamata. Al ritorno dalla funzione nella medesima struttura si
   troveranno i dati richiesti.
   
   La routine non ritorna valori in uscita. Tuttavia, se vi sono degli errori essi
   vengono segnalati per mezzo di un apposito codice di errore posto in 
   HGUI_SystemError. Sono previsti i seguenti codici di errore:
                     
   NDHGUI_PIXEL_OUT_OF_IMAGE       Richiesta di scrittura pixel fuori immagine
   NDHGUI_IMAGE_NOT_INITIALIZED    Immagine non inizializzata
   NDHGUI_WRONG_COLOR_DEPTH        Profondità di colore errata
   NDHGUI_WRONG_NR_CHANNELS        Numero di canali errato
   NDHGUI_UNKNOWN_ERROR            Errore sconosciuto
   ------------------------------------------------------------------------------------
*/ 

{  
   if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
   char ErrRep;
   
   ErrRep = INTERNAL_SetupHGUIRegisters (MyIPLImage);
   // Setta i pseudoregistri per la lettura dei pixels dell'immagine
   
   if (!ErrRep)
   {
       if  ( (MyHGUIPixel->PosX<MyIPLImage->width) && (MyHGUIPixel->PosY<MyIPLImage->height) )
       // Verifichiamo che la richiesta non sia fuori immagine
       {                                    
               INTERNAL_GetPixelFromCurrentImage (MyHGUIPixel->PosX, MyHGUIPixel->PosY);
               // Porta le informazioni di interesse nei pseudoregistri richiesti
               
               switch (HGUI_ColorDepth)
               {
                           case IPL_DEPTH_8U:
                           case IPL_DEPTH_8S:
                           case IPL_DEPTH_16U:
                           case IPL_DEPTH_16S:
                           case IPL_DEPTH_32S:
                                {
                                      if ((HGUI_NrChannels==3) || (HGUI_NrChannels==4))
                                      {
                                          MyHGUIPixel->ColorDepth = HGUI_ColorDepth;       
                                          MyHGUIPixel->NrChannels = 3;
                                          
                                          MyHGUIPixel->C1 = HGUI_ColorValue [0];
                                          MyHGUIPixel->C2 = HGUI_ColorValue [1];
                                          MyHGUIPixel->C3 = HGUI_ColorValue [2];
                                          break;
                                      }
                                      else
                                      {
                                          MyHGUIPixel->ColorDepth = HGUI_ColorDepth;       
                                          MyHGUIPixel->NrChannels = 1;
                                          
                                          MyHGUIPixel->C1 = HGUI_ColorValue [0];
                                          MyHGUIPixel->C2 = 0;
                                          MyHGUIPixel->C3 = 0;
                                          break;
                                      }
                                }
                                
                           case IPL_DEPTH_32F:
                                {
                                      if ((HGUI_NrChannels==3) || (HGUI_NrChannels==4))
                                      {
                                          MyHGUIPixel->ColorDepth = HGUI_ColorDepth;       
                                          MyHGUIPixel->NrChannels = 3;
                                          
                                          MyHGUIPixel->C1 = HGUI_FlColorValue [0];
                                          MyHGUIPixel->C2 = HGUI_FlColorValue [1];
                                          MyHGUIPixel->C3 = HGUI_FlColorValue [2];
                                          break;
                                      }
                                      else
                                      {
                                          MyHGUIPixel->ColorDepth = HGUI_ColorDepth;       
                                          MyHGUIPixel->NrChannels = 1;
                                          
                                          MyHGUIPixel->C1 = HGUI_FlColorValue [0];
                                          MyHGUIPixel->C2 = 0;
                                          MyHGUIPixel->C3 = 0;
                                          break;
                                      }
                                }  
                                
                             // Non è possibile che non venga trovato un valore di ColorDepth
                             // lecito perchè sennò l'inizializzazione degli pseudoregistri
                             // avrebbe rilevato l'errore          
               }
               
               HGUI_SystemError=0;
               return;
               
       }
       else    // Richiesta pixel fuori immagine
       {
            HGUI_NotifyError (-2, 0, "cvGetPixelFromImage", "Pixel request out of image");
            
            HGUI_SystemError = NDHGUI_PIXEL_OUT_OF_IMAGE;
            return;   
       }                                
   }
   else       // ErrRep diverso da 0 indica presenza di errori di nr.canali o profondità colore
   {
      switch (ErrRep)
      {
             case NDHGUI_MSG_IMAGE_NOT_ALLOCATED:
             {
                 HGUI_NotifyError (-2, 0, "cvGetPixelFromImage", "Image is not initialized");
                 
                 HGUI_SystemError = NDHGUI_IMAGE_NOT_INITIALIZED; 
                 return;
             } 
             
             case NDHGUI_MSG_WRONG_COLOR_DEPTH:
             {
                 HGUI_NotifyError (-2, 0, "cvGetPixelFromImage", "Error in color depth");
                 // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                 // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                 // non prima però di avere generato un errore 
                 
                 HGUI_SystemError = NDHGUI_WRONG_COLOR_DEPTH;
                 return; 
             }
             
             case NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS:
             {
                 HGUI_NotifyError (-2, 0, "cvGetPixelFromImage", "Wrong number of channel");
                 // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                 // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                 // non prima però di avere generato un errore 
                 
                 HGUI_SystemError = NDHGUI_WRONG_NR_CHANNELS;
                 return;
             }
             
             default:
             {
                 HGUI_NotifyError (-2, 0, "cvGetPixelFromImage", "Unknown error in image");
                 // Se nessuna delle condizioni precedenti si verifica, vuol dire che l'immagine                 
                 // ha qualche errore. Allora, la routine si limita ad uscire senza fare modifiche,                          
                 // non prima però di avere generato un errore 
                 
                 HGUI_SystemError = NDHGUI_UNKNOWN_ERROR; 
                 return;
             }
      }
   }
                
}        







