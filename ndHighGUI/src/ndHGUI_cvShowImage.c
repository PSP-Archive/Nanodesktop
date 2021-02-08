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
    MANIPULATION IMAGES ROUTINES
    -----------------------------------------------------------------------------
*/

#include <$_NDHGUI_for_NDHGUI.h>

/*
    -------------------------------------------------------------------------
    STRUTTURE DI GESTIONE INTERNA
    -------------------------------------------------------------------------
*/

struct cvShwImgPacket_Type 
{
    int IplSizeX, IplSizeY;
    int ColorDepth;
    int COI;
    int NrChannels;
    int ROI_OffsetX, ROI_OffsetY; 
    int ROI_Height,  ROI_Width;  
    int WndHandle;
    int NewWndX1, NewWndY1;
    int NewWndX2, NewWndY2;
    
    TypeColor NewColorTitle, NewColorBGTitle;
    TypeColor NewColorBGWS,  NewColorBorder;
};

/*
   ----------------------------------------------------------------------------
   ROUTINES DI SUPPORTO PER CONVERSIONE PIXEL PER PIXEL (HGUI pseudoregisters)
   ----------------------------------------------------------------------------
*/



inline char INTERNAL_SetupHGUIRegisters (IplImage *MyIPLImage)
/*
    Questa routine è esclusivamente per uso interno. 
    
    Essa provvede a settare il valore dei registri HGUI in base a certe caratteristiche
    dell'immagine. 
        
    Questa routine è interna, quindi non imposta un codice di ritorno nella variabile
    HGUI_SystemError (quest'ultimo è il metodo standard usato dalle routines di
    ndHighGUI per segnalare una condizione di errore). 
    
    Al contrario, questa routine restituisce alla routine chiamante un message code.
    Se non ci sono errori, il message code restituito è 0, altrimenti sono previsti 
    i seguenti codici messaggio:  
               
    NDHGUI_MSG_IMAGE_NOT_ALLOCATED        L'immagine non è allocata
    NDHGUI_MSG_WRONG_COLOR_DEPTH
    NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS
*/
{
    
    if (MyIPLImage->imageData)               // Per prima cosa verifichiamo che l'immagine
    {                                        // sia già stata allocata da CvCreateImage            
               
             HGUI_ColorDepth = MyIPLImage->depth;
             HGUI_NrChannels = MyIPLImage->nChannels;
             HGUI_WidthStep  = MyIPLImage->widthStep;
                  
             HGUI_AdrDataImage = (int) MyIPLImage->imageData;
             
             // Ora calcoliamo la size del singolo valore canale
                        
             switch (HGUI_ColorDepth)           // Consideriamo i diversi casi
             {
                       case IPL_DEPTH_8U:
                            {
                            HGUI_SizeValue=sizeof (unsigned char);
                            break;
                            }
                       case IPL_DEPTH_8S:
                            {
                            HGUI_SizeValue=sizeof (char);
                            break;
                            }
                       case IPL_DEPTH_16U:
                            {
                            HGUI_SizeValue=sizeof (unsigned short int);
                            break;
                            }
                       case IPL_DEPTH_16S:
                            {
                            HGUI_SizeValue=sizeof (short int);
                            break;
                            }
                       case IPL_DEPTH_32S:
                            {
                            HGUI_SizeValue=sizeof (int);
                            break;
                            }
                       case IPL_DEPTH_32F:
                            {
                            HGUI_SizeValue=sizeof (float);
                            break;
                            }
                       default:
                            {
                               return NDHGUI_MSG_WRONG_COLOR_DEPTH;
                               // Segnala che la profondità di colore è sbagliata
                            }
             }   
    
             if ((HGUI_NrChannels==3) || (HGUI_NrChannels==4))
             {
                 // Se l'immagine ha 3 o 4 canali, occorre vedere il codice RGBA 
                 // nell'immagine per settare la destinazione dei vari valori
                 // canale
                 
                 char CounterX, CounterCode;
                 
                 for (CounterX=0; CounterX<HGUI_NrChannels; CounterX++)
                 {
                     for (CounterCode=0; CounterCode<4; CounterCode++)
                     {
                         if ( MyIPLImage->channelSeq [CounterX] == HGUI_ColorSeq [CounterCode] )
                         {
                              HGUI_DestValue [CounterX] = CounterCode;
                              // CounterCode contiene un codice che sarà 0 se viene trovata la
                              // lettera R, 1 per la lettera G, 2 per la lettera B, 3 per 
                              // la lettera A. Alla fine, in HighGUI DestValue c'è un codice
                              // di quattro numeri che dice come devono essere posizionati
                              // i dati letti da un'immagine
                         }
                     }
                 }
             }
             else    // I canali non sono 3 o 4
             {
                 if (HGUI_NrChannels==1)
                 {
                    HGUI_DestValue [0]=0;
                    HGUI_DestValue [1]=0;
                    HGUI_DestValue [2]=0;
                    HGUI_DestValue [3]=0;
                 }
                 else
                     return NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS;
             }
             
             return 0;      // Segnala che non ci sono errori
    
    }
    else   // L'immagine non è stata allocata
      return NDHGUI_MSG_IMAGE_NOT_ALLOCATED;              
}



inline char INTERNAL_SetupHGUIDstRegisters (IplImage *MyIPLImage)
/*
    Questa routine è esclusivamente per uso interno. 
    
    Essa provvede a settare il valore dei registri HGUI Dst in base a certe caratteristiche
    dell'immagine. 
        
    Questa routine è interna, quindi non imposta un codice di ritorno nella variabile
    HGUI_SystemError (quest'ultimo è il metodo standard usato dalle routines di
    ndHighGUI per segnalare una condizione di errore). 
    
    Al contrario, questa routine restituisce alla routine chiamante un message code.
    Se non ci sono errori, il message code restituito è 0, altrimenti sono previsti 
    i seguenti codici messaggio:  
               
    NDHGUI_MSG_IMAGE_NOT_ALLOCATED        L'immagine non è allocata
    NDHGUI_MSG_WRONG_COLOR_DEPTH
    NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS
*/
{
    
    if (MyIPLImage->imageData)               // Per prima cosa verifichiamo che l'immagine
    {                                        // sia già stata allocata da CvCreateImage            
               
             HGUI_ColorDepth_Dst = MyIPLImage->depth;
             HGUI_NrChannels_Dst = MyIPLImage->nChannels;
             HGUI_WidthStep_Dst  = MyIPLImage->widthStep;
                  
             HGUI_AdrDataImage_Dst = (int) MyIPLImage->imageData;
             
             // Ora calcoliamo la size del singolo valore canale
                        
             switch (HGUI_ColorDepth_Dst)           // Consideriamo i diversi casi
             {
                       case IPL_DEPTH_8U:
                            {
                            HGUI_SizeValue_Dst=sizeof (unsigned char);
                            break;
                            }
                       case IPL_DEPTH_8S:
                            {
                            HGUI_SizeValue_Dst=sizeof (char);
                            break;
                            }
                       case IPL_DEPTH_16U:
                            {
                            HGUI_SizeValue_Dst=sizeof (unsigned short int);
                            break;
                            }
                       case IPL_DEPTH_16S:
                            {
                            HGUI_SizeValue_Dst=sizeof (short int);
                            break;
                            }
                       case IPL_DEPTH_32S:
                            {
                            HGUI_SizeValue_Dst=sizeof (int);
                            break;
                            }
                       case IPL_DEPTH_32F:
                            {
                            HGUI_SizeValue_Dst=sizeof (float);
                            break;
                            }
                       default:
                            {
                               return NDHGUI_MSG_WRONG_COLOR_DEPTH;
                               // Segnala che la profondità di colore è sbagliata
                            }
             }   
    
             if ((HGUI_NrChannels_Dst==3) || (HGUI_NrChannels_Dst==4))
             {
                 // Se l'immagine ha 3 o 4 canali, occorre vedere il codice RGBA 
                 // nell'immagine per settare la destinazione dei vari valori
                 // canale
                 
                 char CounterX, CounterCode;
                 
                 for (CounterX=0; CounterX<HGUI_NrChannels_Dst; CounterX++)
                 {
                     for (CounterCode=0; CounterCode<4; CounterCode++)
                     {
                         if ( MyIPLImage->channelSeq [CounterX] == HGUI_ColorSeq [CounterCode] )
                         {
                              HGUI_DestValue_Dst [CounterX] = CounterCode;
                              // CounterCode contiene un codice che sarà 0 se viene trovata la
                              // lettera R, 1 per la lettera G, 2 per la lettera B, 3 per 
                              // la lettera A. Alla fine, in HighGUI DestValue c'è un codice
                              // di quattro numeri che dice come devono essere posizionati
                              // i dati letti da un'immagine
                         }
                     }
                 }
             }
             else    // I canali non sono 3 o 4
             {
                 if (HGUI_NrChannels_Dst==1)
                 {
                    HGUI_DestValue_Dst [0]=0;
                    HGUI_DestValue_Dst [1]=0;
                    HGUI_DestValue_Dst [2]=0;
                    HGUI_DestValue_Dst [3]=0;
                 }
                 else
                     return NDHGUI_MSG_WRONG_NR_COLOR_CHANNELS;
             }
             
             return 0;      // Segnala che non ci sono errori
    
    }
    else   // L'immagine non è stata allocata
      return NDHGUI_MSG_IMAGE_NOT_ALLOCATED;              
}

/*
   ----------------------------------------------------------------------------
   ROUTINES GETPIXEL/PUTPIXEL
   ----------------------------------------------------------------------------
*/


inline void INTERNAL_GetPixelFromCurrentImage (int PosX, int PosY)
/*
     Questa routine è esclusivamente per uso interno.
     
     Questa routine provvede ad estrarre le informazioni su un pixel da
     un'immagine IPLImage. Le coordinate del pixel vengono passate per mezzo
     dei parametri PosX e PosY. 
     
     La routine è progettata per essere molto veloce: per questo motivo non
     esegue nessun controllo sull'immagine. Il compito di eseguire dei 
     controlli sull'immagine è demandato alla routine chiamante. La
     routine chiamante ha anche il compito di settare, prima di richiamare
     questa routines, i seguenti valori:
             
     a) nello pseudoregistro int HGUI_AdrDataImage, deve essere inserito l'indirizzo
        dei dati dell'immagine
     b) nello pseudoregistro int HGUI_SizeValue, deve essere inserito il numero di
        byte occupato da ciascun valore canale all'interno del pixel
     b) nello pseudoregistro int HGUI_NrChannels, deve essere inserito il numero dei
        canali
     c) nei quattro pseudoregistri char HGUI_DestValue [4] deve essere inserita 
        la destinazione del valore canale letto (TORED, TOGREEN, TOBLUE, TOALPHA) 
     
     Se l'immagine è a valori integer, verranno depositati i dati sulle tre
     componenti RGB (non normalizzate) nei quattro pseudoregistri integer
     HGUI_ColorValue [4], ovvero, qualora l'immagine sia monocromatica, 
     nel solo registro HGUI_ColorValue [0].
     
     Se l'immagine è a valori float, verranno depositati i dati sulle tre
     componenti RGB (non normalizzate) nei quattro pseudoregistri float
     HGUI_FlColorValue [4], ovvero, qualora l'immagine sia monocromatica, 
     nel solo registro HGUI_FlColorValue [0].
     
     Non è previsto alcun codice messaggio per questa routine.
*/
{
     register int Counter, _DestValue;
     register int ADR, ADR0;
     register int _NrChannel;
     
     // Calcoliamo l'indirizzo del dato che vogliamo trovare. Si tenga presente
     // che questa routine viene richiamata migliaia di volte al secondo: per questo,
     // è progettata per la massima velocità. Per questo motivo si sottopone a
     // diverso trattamento il caso in cui c'è un singolo canale dal caso in cui
     // ne sono presenti 3/4. Nel caso di immagine monocromatica, questo risparmia
     // i cicli di clock necessari ad eseguire il ciclo for Counter ed i calcoli
     // relativi per la determinazione dell'ADR
     
     if ((HGUI_NrChannels==3) || (HGUI_NrChannels==4))
     {         
             ADR = HGUI_AdrDataImage + HGUI_WidthStep*PosY + HGUI_SizeValue*HGUI_NrChannels*PosX;  
          
             for (Counter=0; Counter<HGUI_NrChannels; Counter++)
             {
                 ADR0 = ADR + HGUI_SizeValue*Counter;
             
                 // In Counter c'è il numero d'ordine del valore colore che verrà letto dall'immagine
                 // (0, per esempio, corrisponde al primo byte, o word ecc. 1 al secondo ecc.).
                 // Non è detto che il primo byte corrisponda al rosso. Per tale ragione, si
                 // esegue una conversione stabilendo il canale di destinazione
                 
                 _DestValue = HGUI_DestValue [Counter];
                 
                 switch (HGUI_ColorDepth)
                 {
                       case IPL_DEPTH_8U:
                            {
                                HGUI_ColorValue [_DestValue] = *(unsigned char *)(ADR0);         
                                break;         
                            }
                       
                       case IPL_DEPTH_8S:
                            {
                                HGUI_ColorValue [_DestValue] = *(char *)(ADR0);         
                                break;          
                            }
                       
                       case IPL_DEPTH_16U:
                            {
                                 HGUI_ColorValue [_DestValue] = *(unsigned short *)(ADR0);           
                                 break;
                            }
                       
                       case IPL_DEPTH_16S:
                            {
                                 HGUI_ColorValue [_DestValue] = *(short int *)(ADR0);                       
                                 break;
                            }
                       
                       case IPL_DEPTH_32S:
                            {
                                 HGUI_ColorValue [_DestValue] = *(int *)(ADR0);  
                                 break;
                            }
                            
                       case IPL_DEPTH_32F:
                            {
                                 HGUI_FlColorValue [_DestValue] = *(float *)(ADR0); 
                                 break;
                            }
                 
                 }               // End of switch
                           
             }
         
     }
     else   // C'è un solo canale (immagine monocromatica)
     {
                 
             ADR = HGUI_AdrDataImage + HGUI_WidthStep*PosY + HGUI_SizeValue*PosX;  
             // Il numero di canali è 1
         
             switch (HGUI_ColorDepth)
             {
                   case IPL_DEPTH_8U:
                        {
                            HGUI_ColorValue [0] = *(unsigned char *)(ADR);         
                            break;         
                        }
                   
                   case IPL_DEPTH_8S:
                        {
                            HGUI_ColorValue [0] = *(char *)(ADR);         
                            break;          
                        }
                   
                   case IPL_DEPTH_16U:
                        {
                             HGUI_ColorValue [0] = *(unsigned short *)(ADR);           
                             break;
                        }
                   
                   case IPL_DEPTH_16S:
                        {
                             HGUI_ColorValue [0] = *(short int *)(ADR);                       
                             break;
                        }
                   
                   case IPL_DEPTH_32S:
                       {
                             HGUI_ColorValue [0] = *(int *)(ADR);  
                             break;
                        }
                   case IPL_DEPTH_32F:
                        {
                             HGUI_FlColorValue [0] = *(float *)(ADR); 
                             break;
                        }
             }               // End of switch
 
     }  // if (NrChannels==3)
}


inline void INTERNAL_PutPixelToCurrentImage (int PosX, int PosY)
/*
     Questa routine è esclusivamente per uso interno.
     
     Questa routine provvede ad scrivere le informazioni su un pixel in
     un'immagine IPLImage. Le coordinate del pixel vengono passate per mezzo
     dei parametri PosX e PosY. 
     
     La routine è progettata per essere molto veloce: per questo motivo non
     esegue nessun controllo sull'immagine. Il compito di eseguire dei 
     controlli sull'immagine è demandato alla routine chiamante. La
     routine chiamante ha anche il compito di settare, prima di richiamare
     questa routines, i seguenti valori:
             
     a) nello pseudoregistro int HGUI_AdrDataImage_Dst, deve essere inserito l'indirizzo
        dei dati dell'immagine
     b) nello pseudoregistro int HGUI_SizeValue_Dst, deve essere inserito il numero di
        byte occupato da ciascun valore canale all'interno del pixel
     b) nello pseudoregistro int HGUI_NrChannels_Dst, deve essere inserito il numero dei
        canali
     c) nei quattro pseudoregistri char HGUI_DestValue_Dst [4] deve essere inserita 
        la destinazione del valore canale letto (TORED, TOGREEN, TOBLUE, TOALPHA) 
     
     Se l'immagine è a valori integer, i dati sulle tre componenti RGB (non normalizzate) 
     saranno letti dai quattro pseudoregistri integer
     HGUI_ColorValue_Dst [4], ovvero, qualora l'immagine sia monocromatica, 
     dal solo registro HGUI_ColorValue_Dst [0].
     
     Se l'immagine è a valori float,  i dati sulle tre componenti RGB (non normalizzate) 
     saranno letti dai quattro pseudoregistri float
     HGUI_FlColorValue_Dst [4], ovvero, qualora l'immagine sia monocromatica, 
     nel solo registro HGUI_FlColorValue_Dst [0].
     
     Non è previsto alcun codice messaggio per questa routine.
*/
{
     register int Counter, _DestValue;
     register int ADR, ADR0;
     
     // Calcoliamo l'indirizzo del dato che vogliamo trovare
     
     ADR = HGUI_AdrDataImage_Dst + HGUI_WidthStep_Dst*PosY + HGUI_SizeValue_Dst*HGUI_NrChannels_Dst*PosX;
     
     // Per la routine PutPixel il problema della velocità è meno critico, perchè non
     // viene usata da ShowImage. Per questo, usiamo un ciclo for che gestisce anche
     // il caso di immagine monocromatica
     
     for (Counter=0; Counter<HGUI_NrChannels_Dst; Counter++)
     {
         ADR0 = ADR + HGUI_SizeValue_Dst*Counter;
     
         // In Counter c'è il numero d'ordine del valore colore che verrà scritto nell'immagine
         // (0, per esempio, corrisponde al primo byte, o word ecc. 1 al secondo ecc.).
         // Non è detto che il primo byte corrisponda al rosso. Per tale ragione, si
         // esegue una conversione stabilendo il canale di destinazione
         
         _DestValue = HGUI_DestValue_Dst [Counter];
         
         switch (HGUI_ColorDepth_Dst)
         {
               case IPL_DEPTH_8U:
                    {
                        *(unsigned char *)(ADR0) = HGUI_ColorValue_Dst [_DestValue];         
                        break;         
                    }
               
               case IPL_DEPTH_8S:
                    {
                        *(char *)(ADR0) = HGUI_ColorValue_Dst [_DestValue];         
                        break;          
                    }
               
               case IPL_DEPTH_16U:
                    {
                         *(unsigned short *)(ADR0) = HGUI_ColorValue_Dst [_DestValue];           
                         break;
                    }
               
               case IPL_DEPTH_16S:
                    {
                         *(short int *)(ADR0) = HGUI_ColorValue_Dst [_DestValue];                       
                         break;
                    }
               
               case IPL_DEPTH_32S:
                    {
                         *(int *)(ADR0) = HGUI_ColorValue_Dst [_DestValue];  
                         break;
                    }
                    
               case IPL_DEPTH_32F:
                    {
                         *(float *)(ADR0) = HGUI_FlColorValue_Dst [_DestValue]; 
                         break;
                    }
         
         }               // End of switch
                   
     }
 
 
}





inline TypeColor INTERNAL_CurrentRGBToMagicNumber ()
/*
   Questa routine è esclusivamente per uso interno. 
   
   Essa analizza i valori presenti negli pseudoregistri HighGUI e provvede a 
   restituire un MagicNumber che sia visualizzabile da Nanodesktop

   Non sono previsti codici messaggio per questa routine. Essa è progettata
   per eseguire la correzione il più velocemente possibile.
*/
{
   if ((HGUI_NrChannels==3) || (HGUI_NrChannels==4))
   {
       switch (HGUI_ColorDepth)
       {
               case IPL_DEPTH_8U:
                        break;
               case IPL_DEPTH_8S:
                        break;
               case IPL_DEPTH_16U:
                    {    
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 8;
                        HGUI_ColorValue [1] = HGUI_ColorValue [1] >> 8;
                        HGUI_ColorValue [2] = HGUI_ColorValue [2] >> 8;
                        break;
                    }
               case IPL_DEPTH_16S:
                    {
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 8;
                        HGUI_ColorValue [1] = HGUI_ColorValue [1] >> 8;
                        HGUI_ColorValue [2] = HGUI_ColorValue [2] >> 8;
                        break;
                    }
               case IPL_DEPTH_32S:
                    {
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 24;
                        HGUI_ColorValue [1] = HGUI_ColorValue [1] >> 24;
                        HGUI_ColorValue [2] = HGUI_ColorValue [2] >> 24;
                        break;
                    }
               case IPL_DEPTH_32F:
                    {
                        // Trasferisci negli pseudoregistri interi il valore float dopo
                        // averlo convertito: uso un doppio passaggio per evitare un
                        // crash di PSPE
                        HGUI_ColorValue [0] = (int)(HGUI_FlColorValue [0]);
                        HGUI_ColorValue [1] = (int)(HGUI_FlColorValue [1]);
                        HGUI_ColorValue [2] = (int)(HGUI_FlColorValue [2]);
                        
                        // Shift delle informazioni
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 24;
                        HGUI_ColorValue [1] = HGUI_ColorValue [1] >> 24;
                        HGUI_ColorValue [2] = HGUI_ColorValue [2] >> 24;
                        
                        break;       
                    }  
       }
       
       return ndHAL_RGBToMagicNumber (HGUI_ColorValue [0], HGUI_ColorValue [1], HGUI_ColorValue [2]);
   } 
   
   
   
   if (HGUI_NrChannels==1)
   {
       switch (HGUI_ColorDepth)
       {
               case IPL_DEPTH_8U:
                        break;
               case IPL_DEPTH_8S:
                        break;
               case IPL_DEPTH_16U:
                    {    
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 8;
                        break;
                    }
               case IPL_DEPTH_16S:
                    {
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 8;
                        break;
                    }
               case IPL_DEPTH_32S:
                    {
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 24;
                        break;
                    }
               case IPL_DEPTH_32F:
                    {
                        // Trasferisci negli pseudoregistri interi il valore float dopo
                        // averlo convertito: uso un doppio passaggio per evitare un
                        // crash di PSPE
                        HGUI_ColorValue [0] = (int)(HGUI_FlColorValue [0]);
                        
                        // Shift delle informazioni
                        HGUI_ColorValue [0] = HGUI_ColorValue [0] >> 24;
                        
                        break;       
                    }  
       }
       
       return ndHAL_RGBToMagicNumber (HGUI_ColorValue [0], HGUI_ColorValue [0], HGUI_ColorValue [0]);
   } 
  
   // Se il numero di canali è errato ritorna zero: in realtà non può accadere mai se non
   // ci sono bug nel codice
   
   return 0;
} 


inline void INTERNAL_ConvertCurrentPixel (void)
/* 
   Questa routine è esclusivamente per uso interno. 
   
   Essa provvede a convertire i valori colore relativi ad un pixel, caratterizzati
   da una certa profondità e da un certo numero di canali, in valori colore 
   equivalenti con una diversa profondità ed un diverso numero di canali. 
   
   La routine è progettata per avere la maggiore velocità possibile. Per tale
   ragione, non viene eseguito alcun controllo.
   
   Non sono previsti codici messaggio per questa routine 
*/
{
   char NrBitSrc, NrBitDest, NrBitDiff;
   
   int Tmp_ColorValue [4];
   
   // Per prima cosa si provvede a trasformare i valori dei canali in int,
   // qualora i valori di partenza fossero float.          
            
   if (HGUI_ColorDepth==IPL_DEPTH_32F)
   {
       HGUI_ColorValue [0] = (int)(HGUI_FlColorValue [0]);
       HGUI_ColorValue [1] = (int)(HGUI_FlColorValue [1]);
       HGUI_ColorValue [2] = (int)(HGUI_FlColorValue [2]);
   }
   
   switch (HGUI_ColorDepth)
   {
       case IPL_DEPTH_8U:
       case IPL_DEPTH_8S:
            NrBitSrc=8;
            break;
       case IPL_DEPTH_16U:
       case IPL_DEPTH_16S:
            NrBitSrc=16;
            break;
       case IPL_DEPTH_32S:
       case IPL_DEPTH_32F:
            NrBitSrc=32;
            break;          
   }
   
   switch (HGUI_ColorDepth_Dst)
   {
       case IPL_DEPTH_8U:
       case IPL_DEPTH_8S:
            NrBitDest=8;
            break;
       case IPL_DEPTH_16U:
       case IPL_DEPTH_16S:
            NrBitDest=16;
            break;
       case IPL_DEPTH_32S:
       case IPL_DEPTH_32F:
            NrBitDest=32;
            break;        
   }
   
   NrBitDiff = NrBitSrc - NrBitDest;
     
   if (NrBitDiff >= 0)
   {
      Tmp_ColorValue [0] = HGUI_ColorValue [0] >> NrBitDiff;
      Tmp_ColorValue [1] = HGUI_ColorValue [1] >> NrBitDiff;
      Tmp_ColorValue [2] = HGUI_ColorValue [2] >> NrBitDiff;
   }
   else
   {
      Tmp_ColorValue [0] = HGUI_ColorValue [0] << (-NrBitDiff);
      Tmp_ColorValue [1] = HGUI_ColorValue [1] << (-NrBitDiff);
      Tmp_ColorValue [2] = HGUI_ColorValue [2] << (-NrBitDiff);
   }
   
   
   // Ok, nei registri tmp stanno i tre valori colore opportunamente convertiti,
   // almeno per quanto riguarda la profondità
   
   if ( (HGUI_NrChannels_Dst==1) && (HGUI_NrChannels==1) )
   {
        HGUI_ColorValue_Dst [0] = Tmp_ColorValue [0];
   }
   
   if ( (HGUI_NrChannels_Dst==3) && (HGUI_NrChannels==3) )
   {
      HGUI_ColorValue_Dst [0] = Tmp_ColorValue [0];
      HGUI_ColorValue_Dst [1] = Tmp_ColorValue [1];
      HGUI_ColorValue_Dst [2] = Tmp_ColorValue [2];
   } 
   
   if ( (HGUI_NrChannels_Dst==3) && (HGUI_NrChannels==1) )
   {
      HGUI_ColorValue_Dst [0] = Tmp_ColorValue [0];
      HGUI_ColorValue_Dst [1] = Tmp_ColorValue [0];
      HGUI_ColorValue_Dst [2] = Tmp_ColorValue [0];
   }

   if ( (HGUI_NrChannels_Dst==1) && (HGUI_NrChannels==3) )
   {
      HGUI_ColorValue_Dst [0] = (int)(0.212671*Tmp_ColorValue[0] + 0.715160*Tmp_ColorValue[1]+ 0.072169*Tmp_ColorValue[2]);
   }
   
       
   if (HGUI_ColorDepth_Dst==IPL_DEPTH_32F)
   {
       HGUI_FlColorValue_Dst [0] = HGUI_ColorValue_Dst [0];
       HGUI_FlColorValue_Dst [1] = HGUI_ColorValue_Dst [1];
       HGUI_FlColorValue_Dst [2] = HGUI_ColorValue_Dst [2];
   }  
}


/* 
   ---------------------------------------------------------------------------
   CVSHOWIMAGE
   ---------------------------------------------------------------------------
*/


static inline int _min (unsigned short int a, unsigned short int b)
{
   return (a<b ? a : b);
}

static inline int _max (unsigned short int a, unsigned short int b)
{
   return (a>b ? a : b);
}


static inline void INTERNAL_CVSHWIMG_SetupImageParameters (struct cvShwImgPacket_Type *Packet, IplImage *Image)
/*
    Questa routine si occupa di impostare i parametri all'inizio dell'elaborazione eseguita da
    cvShowImage.
*/
{
    if (Image!=0)
     {
         if (Image->roi == 0)                                   // Verifica che la ROI sia a 0     
         {
            Packet->IplSizeX=Image->width;                              // Se non è settata la ROI, allora Packet->IplSizeX e Packet->IplSizeY
            Packet->IplSizeY=Image->height;                             // hanno lo stesso valore di ImageSizeX e ImageSizeY
      
            Packet->ColorDepth=Image->depth;                            // Profondità di colore e numero di canali sono
            Packet->NrChannels=Image->nChannels;                        // posti ai valori standard
            
            Packet->COI=0;
            
            Packet->ROI_OffsetX = 0;
            Packet->ROI_OffsetY = 0;
            Packet->ROI_Width  = Image->width;
            Packet->ROI_Height = Image->height;
         }   
         else
         {  
            Packet->IplSizeX=Image->width;
            Packet->IplSizeY=Image->height;
              
            Packet->ColorDepth=Image->depth;
            Packet->COI=Image->roi->coi;
              
            Packet->ROI_OffsetX = Image->roi->xOffset; 
            Packet->ROI_OffsetY = Image->roi->yOffset; 
            Packet->ROI_Width   = Image->roi->width;
            Packet->ROI_Height  = Image->roi->height;
         }; 
     }
     else
     {
        Packet->IplSizeX=0;
        Packet->IplSizeY=0;
              
        Packet->ColorDepth=0;
        Packet->COI=0;
              
        Packet->ROI_OffsetX = 0; 
        Packet->ROI_OffsetY = 0; 
        Packet->ROI_Width   = 0;
        Packet->ROI_Height  = 0; 
     }  
}


static inline char INTERNAL_CVSHWIMG_ManageWindowAutoAllocation (struct cvShwImgPacket_Type *Packet,  IplImage *Image, char *WindowTitle)
/*
    Questa routine si occupa di eseguire l'operazione di autoallocazione cvNamedWindow quando
    richiamata da cvShowImage.
    
    Restituisce 0 se non ci sono errori e -1 in caso di errore
*/
{
    Packet->WndHandle=INTERNAL_FindWndByName (WindowTitle);
               
     if (Packet->WndHandle == NDHGUI_MSG_WINDOW_NOT_FOUND)
     {
        cvNamedWindow (WindowTitle, CVKEY_SETSIZE (Packet->ROI_Width, Packet->ROI_Height) || CV_FORCE );
        Packet->WndHandle = INTERNAL_FindWndByName (WindowTitle);
        
        if (Packet->WndHandle == NDHGUI_MSG_WINDOW_NOT_FOUND)
        {
           HGUI_NotifyError (-2, 0, "cvShowImage", "Impossible to proceed to window auto-allocation");
           
           HGUI_SystemError = NDHGUI_I_CANNOT_DO_WND_AUTOALLOCATION;
           return -1;
        }           
     }              
     
     return 0;                                                                                                            
}                                                               

static inline void INTERNAL_CVSHWIMG_ReculcateRenderParams (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, IplImage *Image);
// Riferimento a routine definita successivamente


static inline char INTERNAL_CVSHWIMG_ManageWindowAllocation_NOFORCE_WSWND (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, 
                                                               IplImage *Image, char *WindowTitle)
/*
    Questa routine si occupa di eseguire l'allocazione della finestra quando non è attivata
    l'opzione FORCE ed è attivata la funzione USEWSWND.
    
    La routine restituisce 0 se non ci sono errori e -1 se invece ci sono.
*/
{
     int  OverDimX,      OverDimY;
     int  ThWSWndLength, ThWSWndHeight;
     int  ThWndLength,   ThWndHeight;
     char ErrRep;
     
     int  MaxDimX, MaxDimY;
     int  MinDimX, MinDimY;
     
     ndint64 ndAttribute;

     OverDimX = ndINFO_GetValue (NDGET_OVERDIMX, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
     OverDimY = ndINFO_GetValue (NDGET_OVERDIMY, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
     
     // Qui è necessario fare attenzione. Nanodesktop non consente che le dimensioni
     // della finestra siano inferiori ad un certo valore. Questi check impediscono
     // errori. In particolare, i controlli precedenti assicurano che siano rispettate la
     // disequazioni:
     //
     //    MinWndSizeX < TSWndLength < MaxDim_WSLength
     //    MinWndSizeY < TSWndHeight < MaxDim_WSHeight
     
     
     MinDimX    = ndLP_GetMinWndSizeX (WindowData [Packet->WndHandle].MenuSupported);
     MaxDimX    = ndINFO_GetValue (NDGET_MAXDIM_WSLENGTH, 0, 0, 0);
     
     if (Packet->ROI_Width < MinDimX)
        ThWSWndLength = MinDimX;
     else
     {
        if (Packet->ROI_Width > MaxDimX)
            ThWSWndLength = MaxDimX;
        else
            ThWSWndLength = Packet->ROI_Width;
     } 
     
     // Controllo eseguito sulla coordinata Y
     
     MinDimY     = ndLP_GetMinWndSizeY (WindowData [Packet->WndHandle].MenuSupported);
     MaxDimY     = ndINFO_GetValue (NDGET_MAXDIM_WSHEIGHT, 0, 0, 0);
                                                      
     if (Packet->ROI_Height < MinDimY)
        ThWSWndHeight = MinDimY;
     else
     {
         if (Packet->ROI_Height > MaxDimY) 
            ThWSWndHeight = MaxDimY;
         else
            ThWSWndHeight = Packet->ROI_Height; 
     }
      
     // Sommiamo gli overdim. Grazie alle disequazioni precedenti, è impossibile
     // che la finestra che si ottiene sia troppo piccola.
     
     ThWndLength = ThWSWndLength + OverDimX;
     if (ThWndLength>Windows_MaxScreenX)  ThWndLength=Windows_MaxScreenX-1;
     
     ThWndHeight = ThWSWndHeight + OverDimY;
     if (ThWndHeight>Windows_MaxScreenY)  ThWndHeight=Windows_MaxScreenY-1;
     
     cvUserArea->cvWindowData->PictX1=2;
     cvUserArea->cvWindowData->PictY1=2;
     cvUserArea->cvWindowData->PictX2=ThWSWndLength-2;
     cvUserArea->cvWindowData->PictY2=ThWSWndHeight-2;
     
     cvUserArea->cvWindowData->PictLenX=cvUserArea->cvWindowData->PictX2 - cvUserArea->cvWindowData->PictX1;
     cvUserArea->cvWindowData->PictLenY=cvUserArea->cvWindowData->PictY2 - cvUserArea->cvWindowData->PictY1;
     
     // Adesso vediamo quali colori avrà la nuova finestra
     INTERNAL_ProvideColor (& (Packet->NewColorTitle), & (Packet->NewColorBGTitle), & (Packet->NewColorBGWS), & (Packet->NewColorBorder) );

     // Fatti fornire le nuove coordinate da Nanodesktop ExplodePosition
     ndPOSGEN_ExplodePosition (WindowData [Packet->WndHandle].WndLimitX1, WindowData [Packet->WndHandle].WndLimitY1, 
                                   ThWndLength, ThWndHeight, &(Packet->NewWndX1), &(Packet->NewWndY1), &(Packet->NewWndX2), &(Packet->NewWndY2) );
              
     ndAttribute=0;
     if (WindowData [Packet->WndHandle].MenuSupported) ndAttribute = ndAttribute | MENUSUPPORTED;  
     // Se la finestra già allocata supportava i menù, l'opzione dev'essere confermata
   
     ErrRep=ndLP_ChangeWindowParameters (  (Packet->NewWndX1), (Packet->NewWndY1), (Packet->NewWndX2), (Packet->NewWndY2), WindowTitle, 
                                           Packet->NewColorTitle, Packet->NewColorBGTitle, Packet->NewColorBGWS, Packet->NewColorBorder, 
                                           ThWSWndLength, ThWSWndHeight, Packet->WndHandle, 1, ndAttribute);
     
     
                 
     if (ErrRep)                     // Il processo di creazione di modifica dei parametri della finestra non è andato a buon fine
     {  
        HGUI_NotifyError (-2, 0, "cvShowImage", "Error in new window allocation");
        
        HGUI_SystemError = NDHGUI_I_CANNOT_DO_WINDOW_ALLOCATION;
        return -1;
     }  
     
     // ATTENZIONE: Dopo avere eseguito il change window parameters, i dati della
     // vecchia user area sono stati preservati.  

     cvUserArea->cvWindowData->WndStateIsChanged = 0;
     // Impedirà che venga eseguito la prossima volta, salvo che in caso di modifica delle dimensioni
      
     INTERNAL_CVSHWIMG_ReculcateRenderParams (Packet, cvUserArea, Image);       
     return 0;
}



static inline char INTERNAL_CVSHWIMG_ManageWindowAllocation_NOFORCE_NOWSWND (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, 
                                                               IplImage *Image, char *WindowTitle)
/*
    Questa routine si occupa di eseguire l'allocazione della finestra quando non è attivata
    l'opzione FORCE ed è disattivata la funzione USEWSWND.
    
    La routine restituisce 0 se non ci sono errori e -1 se invece ci sono.
*/
{
    int  OverDimX,      OverDimY;
    int  ThWndLength,   ThWndHeight;
    char ErrRep;
    
    ndint64 ndAttribute;
    ndint64 MyTrackAttribute;
    
    float RatioX, RatioY;
      
    unsigned short TBArea_X1, TBArea_X2, TBArea_Y1;
    unsigned short TBSingle_X1, TBSingle_X2, TBSingle_Y1, TBSingle_Y2;
      
    unsigned short int MinWindowX, MinWindowY;
     
    char Counter, NrTBDrawn, NDHandle;
    char YouCanExit;
    
    float  DiaLen;
    
    int TBOverDimX, TBOverDimY;
    int TryLenX, TryLenY;
    
    int FoundPictLenX, FoundPictLenY;
    int CorrFoundLenX, CorrFoundLenY;
    
    int MargineX;
    
    unsigned short int LenYTrackBar = 40;
    
    // Inizio del codice
    
    OverDimX = ndINFO_GetValue (NDGET_OVERDIMX, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
    OverDimY = ndINFO_GetValue (NDGET_OVERDIMY, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
    
    TBOverDimX = 4;
    TBOverDimY = LenYTrackBar * cvUserArea->cvWindowData->NrcvTrackBarsAllocated + 2;
   
    YouCanExit = 0;
   
    if (TBOverDimY<Windows_MaxScreenY-1)
    {   
            if ( (Packet->ROI_Width!=0) || (Packet->ROI_Height!=0) )
            {
                    DiaLen = ndHAL_XFPU_sqrtf (Packet->ROI_Width*Packet->ROI_Width + Packet->ROI_Height*Packet->ROI_Height);
                    RatioX = Packet->ROI_Width  / DiaLen;
                    RatioY = Packet->ROI_Height / DiaLen;
         
                    while (!YouCanExit)
                    {
                       TryLenX = DiaLen * RatioX;
                       TryLenY = DiaLen * RatioY;
                      
                       if ( (TryLenX + OverDimX + TBOverDimX < Windows_MaxScreenX) && (TryLenY + OverDimY + TBOverDimY < Windows_MaxScreenY) )
                       {
                           FoundPictLenX = TryLenX;
                           FoundPictLenY = TryLenY;
                           break; 
                       }   
                       else
                       {
                          DiaLen = DiaLen - 0.5;
                          
                          if (DiaLen<=0)
                          {
                              FoundPictLenX = 0;
                              FoundPictLenY = 0;
                              break;
                          }
                       }
                    }
            }
            else    //Packet->ROI_Width e Packet->ROI_Height entrambi a 0
            {
                FoundPictLenX = 0;
                FoundPictLenY = 0;              
            }
                   
            // In FoundPictLenX e FoundPictLenY ci sono le dimensioni della picture area. 
            // Tali dimensioni sono già normalizzate in modo che lo schermo possa
            // contenere l'immagine e le trackbars, però possono esserci problemi 
            // verso **il basso**. Se per esempio, non ci fossero trackbars e l'immagine
            // fosse troppo piccola, il sistema determinerebbe coordinate troppo piccole
            // che sarebbero rifiutate da Nanodesktop. 
            
            // Quindi, è necessario eseguire un nuovo controllo per normalizzare in basso
            // i valori
            
            MinWindowX = _max (ndTBAR_ValidateLenXTrackBar (0) + 10 + OverDimX, ndLP_GetMinWndSizeX (WindowData [Packet->WndHandle].MenuSupported));
            MinWindowY = _max (ndTBAR_ValidateLenYTrackBar (0) + 16 + OverDimY, ndLP_GetMinWndSizeY (WindowData [Packet->WndHandle].MenuSupported));
            
            // E' importante capire cosa esattamente implica questo valore. Il nostro problema è
            // che anche in presenza di una immagine piccolissima (o dell'immagine NUL), una
            // finestra con le trackbars deve comunque essere visualizzata. Allora, MinWindow
            // contiene il valore più grande tra la minima estensione che può essere assunta
            // dalla finestra quando vi sono trackbar, che è data da ndTBAR_ValidateLenXTrackBar
            // + 10 pixel per l'overspace di label, + l'overspace di finestra, e la minima
            // dimensione che può essere assunta dalla finestra. Il risultato del confronto non
            // è detto che sia sempre lo stesso, perchè la finestra può contenere o meno i
            // menù.
            
            if ((FoundPictLenX + TBOverDimX + OverDimX) < (MinWindowX))
            {
               CorrFoundLenX = MinWindowX - TBOverDimX - OverDimX;
            }
            else
            {
               CorrFoundLenX = FoundPictLenX;
            } 
            
            if ((FoundPictLenY + TBOverDimY + OverDimY) < (MinWindowY))
            {
               CorrFoundLenY = MinWindowY - TBOverDimY - OverDimY;
            }
            else
            {
               CorrFoundLenY = FoundPictLenY;
            }
            
            // La condizione che noi vogliamo correggere si esprime matematicamente come:
            //
            // FoundPictLenX + 10 + OverDimX < MinWindowX
            // 
            // Noi vogliamo che in questo caso FoundPictLenX venga alzato tanto da 
            // trasformare una disequazione in una equazione, ossia
            //
            // FoundPictLenX + 10 + OverDimX = MinWindowX
            //
            // che però equivale a:
            //
            // FoundPictLenX = MinWindowX - 10 - OverDimX
            
            
            
            // In FoundPictLenX e FoundPictLenY ci sono le lunghezze dell'area Pict
            // Registriamo le coordinate nella UserArea
            
            MargineX = abs (CorrFoundLenX - FoundPictLenX)/2;
            
            cvUserArea->cvWindowData->PictX1=2+MargineX;
            cvUserArea->cvWindowData->PictY1=0;
            cvUserArea->cvWindowData->PictX2=2+MargineX+FoundPictLenX;
            cvUserArea->cvWindowData->PictY2=0+CorrFoundLenY;
             
            cvUserArea->cvWindowData->PictLenX=FoundPictLenX;
            cvUserArea->cvWindowData->PictLenY=CorrFoundLenY;
             
            // Calcoliamo le lunghezze della finestra
            
            ThWndLength = CorrFoundLenX + OverDimX + TBOverDimX;
            ThWndHeight = CorrFoundLenY + OverDimY + TBOverDimY;
            
            // Adesso vediamo quali colori avrà la nuova finestra
            INTERNAL_ProvideColor (& (Packet->NewColorTitle), & (Packet->NewColorBGTitle), & (Packet->NewColorBGWS), & (Packet->NewColorBorder) );
            
            // Fatti fornire le nuove coordinate da Nanodesktop ExplodePosition
            ndPOSGEN_ExplodePosition (WindowData [Packet->WndHandle].WndLimitX1, WindowData [Packet->WndHandle].WndLimitY1, 
                                       ThWndLength, ThWndHeight, &(Packet->NewWndX1), &(Packet->NewWndY1), &(Packet->NewWndX2), &(Packet->NewWndY2));
                  
                         
            ndAttribute=AUTOFIX;
            if (WindowData [Packet->WndHandle].MenuSupported) ndAttribute = ndAttribute | MENUSUPPORTED;  
            // Se la finestra già allocata supportava i menù, l'opzione dev'essere confermata
                     
            ErrRep=ndLP_ChangeWindowParameters (Packet->NewWndX1, Packet->NewWndY1, Packet->NewWndX2, Packet->NewWndY2, WindowTitle, 
                                                   Packet->NewColorTitle, Packet->NewColorBGTitle, Packet->NewColorBGWS, Packet->NewColorBorder, 
                                                   0, 0, Packet->WndHandle, 1, ndAttribute);
             
            // ATTENZIONE: Dopo avere eseguito il change window parameters, i dati della
            // vecchia user area sono stati preservati.  

            if (!ErrRep)
            {
                 // Settiamo i parametri TBArea
                 
                 TBArea_X1 = 2;
                 TBArea_X2 = 2+CorrFoundLenX;
                 TBArea_Y1 = cvUserArea->cvWindowData->PictY2 + 2;
            
                 NrTBDrawn=0;
             
                 for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
                 {
                     if (cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive)
                     {
                                NrTBDrawn++;
                                
                                TBSingle_X1 = TBArea_X1;
                                TBSingle_X2 = TBArea_X2;
                                TBSingle_Y1 = TBArea_Y1 + (NrTBDrawn-1)*LenYTrackBar;
                                TBSingle_Y2 = TBArea_Y1 + (NrTBDrawn)*LenYTrackBar;
                                                                            
                                cvUserArea->cvWindowData->cvTrackBarData [Counter].Value = *(cvUserArea->cvWindowData->cvTrackBarData [Counter].PointerToValue);
                                // Aggiorna in Value il valore: in questo modo, se nel frattempo è stato modificato
                                // da del codice utente, la trackbar rifletterà il cambiamento
                    
                                /*               
                                 Dalla documentazione di Nanodesktop, si ricava che ndTBAR_CreateTrackBar ha un prototipo
                                   del tipo:
                                       
                                       char ndTBAR_CreateTrackBar (unsigned short int RRPosX1, unsigned short int RRPosY1,
                                       unsigned short int RRPosX2, unsigned short int RRPosY2,
                                       TypeColor TextColor, TypeColor PBarColor, TypeColor PBarBkgrColor,
                                       TypeColor PBarBorderColor, TypeColor TBarColor, 
                                       TypeColor TBarBorderColor,
                                       float Value, float MinValue, float MaxValue,
                                       ndint64 TrackAttribute, int TB_CallBack, int InfoCallBack, 
                                       char Packet->WndHandler, char RenderNow)
                                       
                                   dove TB_Callback è la trackbar callback che viene richiamata quando avviene
                                   un cambiamento del valore della trackbar, ossia una funzione con prototipo:
                                                  
                                   void TB_Callback (float Value, int TB_InfoField, unsigned char Packet->WndHandler);
                                   
                                   e dove il campo TB_InfoField è la parte a 32 bit più significativa del
                                   parametro TrackAttribute. 
                                   
                                   ndHighGUI programma le trackbar di Nanodesktop in modo che richiamino 
                                   il gestore INTERNAL_ndHGUI_TrackBar_Manager. Tale gestore, per potere
                                   funzionare, ha bisogno dell'handle della finestra (Packet->WndHandle) e dell'handle
                                   della HGUITrackBar (l'indice di cvUserArea->cvWindowData->cvTrackBarData).
                                   
                                   La prima informazione viene passata automaticamente dal sistema, per la
                                   seconda è necessario usare gli 8 bit meno significativi del campo
                                   TB_InfoField.
                                */          
                                
                                // Prepara un opportuno valore MyTrackAttribute per passare l'handle HGUITrackBar
                                // ad INTERNAL_ndHGUI_TrackBar_Manager.
                             
                                
                                MyTrackAttribute = ((ndint64)(Counter) << 32);
                         
                                NDHandle=ndTBAR_CreateLabeledTrackBar (TBSingle_X1, TBSingle_Y1, TBSingle_X2, TBSingle_Y2,
                                                COLOR_WHITE, COLOR_RED, COLOR_BLACK, COLOR_GRAY, COLOR_GRAY12, COLOR_GRAY12, 
                                                cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName,
                                                COLOR_BLACK, COLOR_GRAY, COLOR_BLACK,
                                                cvUserArea->cvWindowData->cvTrackBarData [Counter].Value, 
                                                0, cvUserArea->cvWindowData->cvTrackBarData [Counter].MaxValue, 
                                                MyTrackAttribute, &(INTERNAL_ndHGUI_TrackBar_Manager), 0, Packet->WndHandle, NORENDER);
                                
                                                       
                                if (NDHandle>=0)
                                {
                                   cvUserArea->cvWindowData->cvTrackBarData [Counter].NDHandle = NDHandle;
                                }
                                else   // C'è stato un errore nell'allocare la trackbar
                                {
                                   HGUI_NotifyError (-2, -NDHandle, "cvShowImage", "Error in Nanodesktop trackbar allocation");
                                   
                                   HGUI_SystemError = NDHGUI_ERR_IN_NDTRACKBAR_ALLOCATION;
                                   return -1;                                     
                                }
                                                        
                     }       // End if active
                 }       // End ciclo for 
             
                 cvUserArea->cvWindowData->WndStateIsChanged = 0;
                 // Impedirà che venga eseguito la prossima volta, salvo che in caso di modifica delle dimensioni
            
                 INTERNAL_CVSHWIMG_ReculcateRenderParams (Packet, cvUserArea, Image);     
                 return 0;            // Segnala alla routine chiamante che non ci sono errori
                 
             }        
             else     // ErrRep!=0 nella modifica parametri della nuova finestra
             {  
                 HGUI_NotifyError (-2, -ErrRep, "cvShowImage", "Error in new window allocation");
                 
                 HGUI_SystemError = NDHGUI_I_CANNOT_DO_WINDOW_ALLOCATION;
                 return -1;
             }  
            
                    
    } 
    else                // Non c'è lo spazio per disegnare tutte le trackbars
    {
        HGUI_NotifyError (-2, 0, "cvShowImage", "I cannot draw all trackbars in this window");
        
        HGUI_SystemError = NDHGUI_I_CANNOT_DRAW_ALL_TRACKBARS;
        return -1;                                                                      
    }
                                        
    
}




static inline char INTERNAL_CVSHWIMG_ManageWindowAllocation_FORCE_WSWND (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, 
                                                               IplImage *Image, char *WindowTitle)
/*
    Questa routine si occupa di eseguire l'allocazione della finestra quando è attivata
    l'opzione FORCE ed è attivata la funzione USEWSWND.
    
    La routine restituisce 0 se non ci sono errori e -1 se invece ci sono.
*/
{
     int OverDimX, OverDimY;
     int MaxDimX,  MaxDimY;
     ndint64 ndAttribute;
     int ThWSWndLength, ThWSWndHeight;
     char ErrRep;
     
     // Inizio del codice
     
     OverDimX = ndINFO_GetValue (NDGET_OVERDIMX, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
     OverDimY = ndINFO_GetValue (NDGET_OVERDIMY, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
     
     // Bound checks
     
     MaxDimX = ndINFO_GetValue (NDGET_MAXDIM_WSLENGTH, 0, 0, 0);
     MaxDimY = ndINFO_GetValue (NDGET_MAXDIM_WSHEIGHT, 0, 0, 0);
     
     if (Packet->ROI_Width <= MaxDimX)
        ThWSWndLength = Packet->ROI_Width;
     else
        ThWSWndLength = MaxDimX;
        
     if (Packet->ROI_Height <= MaxDimY)
        ThWSWndHeight = Packet->ROI_Height;
     else
        ThWSWndHeight = MaxDimY;   
                                                      
     cvUserArea->cvWindowData->PictX1=2;
     cvUserArea->cvWindowData->PictY1=2;
     cvUserArea->cvWindowData->PictX2=ThWSWndLength-2;
     cvUserArea->cvWindowData->PictY2=ThWSWndHeight-2;
     
     cvUserArea->cvWindowData->PictLenX=cvUserArea->cvWindowData->PictX2 - cvUserArea->cvWindowData->PictX1;
     cvUserArea->cvWindowData->PictLenY=cvUserArea->cvWindowData->PictY2 - cvUserArea->cvWindowData->PictY1;
     
     // Adesso vediamo quali colori avrà la nuova finestra
     INTERNAL_ProvideColor (& (Packet->NewColorTitle), & (Packet->NewColorBGTitle), & (Packet->NewColorBGWS), & (Packet->NewColorBorder) );

     // L'opzione FORCE è attivata: devi ricavare le coordinate dalle precedenti
     // coordinate di finestra, senza passare dal Position Generator.
     
     Packet->NewWndX1 = WindowData [Packet->WndHandle].WndLimitX1;
     Packet->NewWndY1 = WindowData [Packet->WndHandle].WndLimitY1;
     Packet->NewWndX2 = WindowData [Packet->WndHandle].WndLimitX2;
     Packet->NewWndY2 = WindowData [Packet->WndHandle].WndLimitY2;
              
     ndAttribute=0;
     if (WindowData [Packet->WndHandle].MenuSupported) ndAttribute = ndAttribute | MENUSUPPORTED;  
     // Se la finestra già allocata supportava i menù, l'opzione dev'essere confermata
   
     ErrRep=ndLP_ChangeWindowParameters (Packet->NewWndX1, Packet->NewWndY1, Packet->NewWndX2, Packet->NewWndY2, WindowTitle, 
                                           Packet->NewColorTitle, Packet->NewColorBGTitle, Packet->NewColorBGWS, Packet->NewColorBorder, 
                                           ThWSWndLength, ThWSWndHeight, Packet->WndHandle, 1, ndAttribute);
     
     // Il change window parameters servirà solo a cambiare i colori, cancellare le
     // vecchie trackbar e pulire la user-area. Dopo avere eseguito il change window parameters, i dati della
     // vecchia user area sono stati preservati.    
     
                 
     if (ErrRep)                     // Il processo di creazione di modifica dei parametri della finestra non è andato a buon fine
     {  
        HGUI_NotifyError (-2, -ErrRep, "cvShowImage", "Error in new window allocation");
        
        HGUI_SystemError = NDHGUI_I_CANNOT_DO_WINDOW_ALLOCATION;
        return -1;
     }  
     
     cvUserArea->cvWindowData->WndStateIsChanged = 0;
     // Impedirà che venga eseguito la prossima volta, salvo che in caso di modifica delle dimensioni
     
     INTERNAL_CVSHWIMG_ReculcateRenderParams (Packet, cvUserArea, Image);            
     return 0;                                                     
}




static inline char INTERNAL_CVSHWIMG_ManageWindowAllocation_FORCE_NOWSWND (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, 
                                                               IplImage *Image, char *WindowTitle)
/*
    Questa routine si occupa di eseguire l'allocazione della finestra quando è attivata
    l'opzione FORCE ed è disattivata la funzione USEWSWND.
    
    La routine restituisce 0 se non ci sono errori e -1 se invece ci sono.
*/
{
    int OverDimX, OverDimY;
    int MaxDimX,  MaxDimY;
    
    ndint64 ndAttribute;
    ndint64 MyTrackAttribute;
    
    int ThWSWndLength, ThWSWndHeight;
    int ThWndLength,   ThWndHeight;
     
    char ErrRep;
    
    float RatioX, RatioY;
      
    unsigned short TBArea_X1, TBArea_X2, TBArea_Y1, TBArea_Y2;
    unsigned short TBSingle_X1, TBSingle_X2, TBSingle_Y1, TBSingle_Y2;
      
    unsigned short int MinWindowX, MinWindowY;
     
    char Counter, NrTBDrawn, NDHandle;
    char YouCanExit;
    
    float  DiaLen;
    
    int TBOverDimX, TBOverDimY;
    int TryLenX, TryLenY;
    
    int FoundPictLenX, FoundPictLenY;
    int CorrFoundLenX, CorrFoundLenY;
    
    int MargineX;
    
    unsigned short int LenYTrackBar = 40;
    
    // Inizio del codice

     
    OverDimX = ndINFO_GetValue (NDGET_OVERDIMX, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
    OverDimY = ndINFO_GetValue (NDGET_OVERDIMY, WindowData [Packet->WndHandle].MenuSupported, 0, 0);
    
    TBOverDimX = 2;
    TBOverDimY = LenYTrackBar * cvUserArea->cvWindowData->NrcvTrackBarsAllocated + 2;
                                                  
    ThWndLength = WindowData [Packet->WndHandle].WndLength;
    ThWndHeight = WindowData [Packet->WndHandle].WndHeight;
    
    // Verifica che ci sia abbastanza spazio per allocare tutte le trackbar
    if (( ThWndHeight - OverDimY - TBOverDimY )>0)
    {
         cvUserArea->cvWindowData->PictX1=2;
         cvUserArea->cvWindowData->PictY1=2;
         cvUserArea->cvWindowData->PictX2=ThWndLength - OverDimX - TBOverDimX;
         cvUserArea->cvWindowData->PictY2=ThWndHeight - OverDimY - TBOverDimY;
         
         cvUserArea->cvWindowData->PictLenX=cvUserArea->cvWindowData->PictX2 - cvUserArea->cvWindowData->PictX1;
         cvUserArea->cvWindowData->PictLenY=cvUserArea->cvWindowData->PictY2 - cvUserArea->cvWindowData->PictY1;
         
         TBArea_X1 = 0;
         TBArea_Y1 = cvUserArea->cvWindowData->PictY2 + 2;
         TBArea_X2 = ThWndLength - OverDimX;
         TBArea_Y2 = ThWndHeight - OverDimY;
         
         // Adesso vediamo quali colori avrà la nuova finestra
         INTERNAL_ProvideColor (& (Packet->NewColorTitle), & (Packet->NewColorBGTitle), & (Packet->NewColorBGWS), & (Packet->NewColorBorder) );
        
         // L'opzione FORCE è attivata
         
         Packet->NewWndX1 = WindowData [Packet->WndHandle].WndLimitX1;
         Packet->NewWndY1 = WindowData [Packet->WndHandle].WndLimitY1;
         Packet->NewWndX2 = WindowData [Packet->WndHandle].WndLimitX2;
         Packet->NewWndY2 = WindowData [Packet->WndHandle].WndLimitY2;
          
                     
         ndAttribute=AUTOFIX;
         if (WindowData [Packet->WndHandle].MenuSupported) ndAttribute = ndAttribute | MENUSUPPORTED;  
         // Se la finestra già allocata supportava i menù, l'opzione dev'essere confermata
               
         ErrRep=ndLP_ChangeWindowParameters (Packet->NewWndX1, Packet->NewWndY1, Packet->NewWndX2, Packet->NewWndY2, WindowTitle, 
                                               Packet->NewColorTitle, Packet->NewColorBGTitle, Packet->NewColorBGWS, Packet->NewColorBorder, 
                                               0, 0, Packet->WndHandle, 1, ndAttribute);
                       
         if (!ErrRep)                     // Il processo di modifica dei parametri della finestra è andato a buon fine
         {     
             // Iniziamo ad allocare le trackbars di finestra
        
             NrTBDrawn=0;
         
             for (Counter=0; Counter<__NRTRACKBARSALLOCABLE; Counter++)
             {
                 if (cvUserArea->cvWindowData->cvTrackBarData [Counter].IsActive)
                 {
                    NrTBDrawn++;
                    
                    TBSingle_X1 = TBArea_X1 + 2;
                    TBSingle_X2 = TBArea_X2 - 2;
                    TBSingle_Y1 = TBArea_Y1 + (NrTBDrawn-1)*LenYTrackBar;
                    TBSingle_Y2 = TBArea_Y1 + (NrTBDrawn)  *LenYTrackBar;
                                                                
                    cvUserArea->cvWindowData->cvTrackBarData [Counter].Value = *(cvUserArea->cvWindowData->cvTrackBarData [Counter].PointerToValue);
                    // Aggiorna in Value il valore: in questo modo, se nel frattempo è stato modificato
                    // da del codice utente, la trackbar rifletterà il cambiamento
                    
                    /* Dalla documentazione di Nanodesktop, si ricava che ndTBAR_CreateTrackBar ha un prototipo
                       del tipo:
                           
                           char ndTBAR_CreateTrackBar (unsigned short int RRPosX1, unsigned short int RRPosY1,
                           unsigned short int RRPosX2, unsigned short int RRPosY2,
                           TypeColor TextColor, TypeColor PBarColor, TypeColor PBarBkgrColor,
                           TypeColor PBarBorderColor, TypeColor TBarColor, 
                           TypeColor TBarBorderColor,
                           float Value, float MinValue, float MaxValue,
                           ndint64 TrackAttribute, int TB_CallBack, int InfoCallBack, 
                           char Packet->WndHandler, char RenderNow)
                           
                       dove TB_Callback è la trackbar callback che viene richiamata quando avviene
                       un cambiamento del valore della trackbar, ossia una funzione con prototipo:
                                      
                       void TB_Callback (float Value, int TB_InfoField, unsigned char Packet->WndHandler);
                       
                       e dove il campo TB_InfoField è la parte a 32 bit più significativa del
                       parametro TrackAttribute. 
                       
                       ndHighGUI programma le trackbar di Nanodesktop in modo che richiamino 
                       il gestore INTERNAL_ndHGUI_TrackBar_Manager. Tale gestore, per potere
                       funzionare, ha bisogno dell'handle della finestra (Packet->WndHandle) e dell'handle
                       della HGUITrackBar (l'indice di cvUserArea->cvWindowData->cvTrackBarData).
                       
                       La prima informazione viene passata automaticamente dal sistema, per la
                       seconda è necessario usare gli 8 bit meno significativi del campo
                       TB_InfoField.
                     */          
                    
                    // Prepara un opportuno valore MyTrackAttribute per passare l'handle HGUITrackBar
                    // ad INTERNAL_ndHGUI_TrackBar_Manager.
                 
                    MyTrackAttribute = ((ndint64)(Counter) << 32);
                    
                    NDHandle=ndTBAR_CreateLabeledTrackBar (TBSingle_X1, TBSingle_Y1, TBSingle_X2, TBSingle_Y2,
                                    COLOR_WHITE, COLOR_RED, COLOR_BLACK, COLOR_GRAY, COLOR_GRAY12, COLOR_GRAY12, 
                                    cvUserArea->cvWindowData->cvTrackBarData [Counter].TBName,
                                    COLOR_BLACK, COLOR_GRAY, COLOR_BLACK,
                                    cvUserArea->cvWindowData->cvTrackBarData [Counter].Value, 
                                    0, cvUserArea->cvWindowData->cvTrackBarData [Counter].MaxValue, 
                                    MyTrackAttribute, &(INTERNAL_ndHGUI_TrackBar_Manager), 0, Packet->WndHandle, NORENDER);
                                    
                    if (NDHandle>=0)
                    {
                       cvUserArea->cvWindowData->cvTrackBarData [Counter].NDHandle = NDHandle;
                    }
                    else   // C'è stato un errore nell'allocare la trackbar
                    {
                       HGUI_NotifyError (-2, 0, "cvShowImage", "Error in Nanodesktop trackbar allocation");
                       
                       HGUI_SystemError = NDHGUI_ERR_IN_NDTRACKBAR_ALLOCATION;
                       return -1;                                     
                    }
                                                    
                 }       // End if active
             }       // End ciclo for 
             
             cvUserArea->cvWindowData->WndStateIsChanged = 0;
             // Impedirà che venga eseguito la prossima volta, salvo che in caso di modifica delle dimensioni
            
             INTERNAL_CVSHWIMG_ReculcateRenderParams (Packet, cvUserArea, Image);     
             return 0;
         }        
         else     // ErrRep!=0 nella modifica parametri della nuova finestra
         {  
             HGUI_NotifyError (-2, -ErrRep, "cvShowImage", "Error in new window allocation");
             
             HGUI_SystemError = NDHGUI_I_CANNOT_DO_WINDOW_ALLOCATION;
             return -1;
         }  
             
         // ATTENZIONE: Dopo avere eseguito il change window parameters, i dati della
         // vecchia user area sono stati preservati.
             
    }        
    else   // Non c'è spazio sufficiente per allocare le trackbar
    {
        HGUI_NotifyError (-2, 0, "cvShowImage", "I cannot draw all trackbars in this window");
        
        HGUI_SystemError = NDHGUI_I_CANNOT_DRAW_ALL_TRACKBARS;
        return -1;                                                                      
    }

}


static inline char INTERNAL_CVSHWIMG_ManageChangeState (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, 
                                                                IplImage *Image, char *WindowTitle)
/*
   Questa routine provvede a gestire il cambiamento di stato delle finestre. La routine smista le
   chiamate verso altre routines, a seconda delle opzioni di funzionamento della finestra
   decise a suo tempo attraverso cvNamedWindow

   La routine restituisce 0 in caso di mancanza di errori, e -1 in caso di errore
*/
{
    if ( (cvUserArea->cvWindowData->WndStateIsChanged) || 
            (cvUserArea->cvWindowData->OLD_cvLenX != Packet->ROI_Width) || 
               (cvUserArea->cvWindowData->OLD_cvLenY != Packet->ROI_Height) )
    {        
           // Il cambiamento del WndState indica che è successo qualcosa di nuovo nella finestra: o
           // è stata introdotta una nuova trackbar, oppure sono state cambiate le dimensioni
           // dell'immagine
           
           cvUserArea->cvWindowData->OLD_cvLenX = Packet->ROI_Width;
           cvUserArea->cvWindowData->OLD_cvLenY = Packet->ROI_Height;
           
           if (!cvUserArea->cvWindowData->UseForce)        // Non è attivata l'opzione force
           {
                   if (cvUserArea->cvWindowData->CanUseNanodesktopWSWnd)
                   {
                        return INTERNAL_CVSHWIMG_ManageWindowAllocation_NOFORCE_WSWND (Packet, cvUserArea, Image, WindowTitle);    
                   }
                   else  // E' disattivata l'opzione UseWSWnd
                   {
                        return INTERNAL_CVSHWIMG_ManageWindowAllocation_NOFORCE_NOWSWND (Packet, cvUserArea, Image, WindowTitle);
                   }   
    
            }
            else  // Opzione force attivata
            {
                   if (cvUserArea->cvWindowData->CanUseNanodesktopWSWnd)
                   {
                        return INTERNAL_CVSHWIMG_ManageWindowAllocation_FORCE_WSWND  (Packet, cvUserArea, Image, WindowTitle); 
                   } 
                   else  // E' disattivata l'opzione UseWSWnd (with Force)
                   {
                        return INTERNAL_CVSHWIMG_ManageWindowAllocation_FORCE_NOWSWND (Packet, cvUserArea, Image, WindowTitle);
                   }   
                      
            }    // if (force enabled)
                                                
                       
    }   // End if ChangeState
    
    // Se non ci sono cambiamenti di stato provvedi semplicemente ad inviare 0 alla routine
    // chiamante in modo che il processo continui
    
    return 0;                                                           
} 


static inline void INTERNAL_CVSHWIMG_ReculcateRenderParams (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, IplImage *Image)
/*
    Questa routine viene richiamata ogniqualvolta viene cambiata la configurazione di
    una finestra. Essa provvede al ricalcolo dei parametri Pict. 
    
    Il ricalcolo viene eseguito solo quando c'è un cambiamento: questo consente di
    aumentare la velocità del sistema
*/
{
   float TmpRenderRatioX, TmpRenderRatioY;
   int   ShowDiffX,       ShowDiffY;
        
   if (Image!=0)
   {
        TmpRenderRatioX = ((float) (cvUserArea->cvWindowData->PictLenX)) / ((float) (Packet->ROI_Width ));
        TmpRenderRatioY = ((float) (cvUserArea->cvWindowData->PictLenY)) / ((float) (Packet->ROI_Height));
      
        if ((TmpRenderRatioX>=1) && (TmpRenderRatioY>=1))    // Non è necessario alcun tipo di ridimensionamento
            cvUserArea->cvWindowData->RenderRatioX = cvUserArea->cvWindowData->RenderRatioY = 1;
        else                                                 // Si sceglie il rapporto di compressione più piccolo e lo  
        {                                                    // si applica sia sull'asse x che sull'asse y
            if (TmpRenderRatioX<TmpRenderRatioY)            
              cvUserArea->cvWindowData->RenderRatioX = cvUserArea->cvWindowData->RenderRatioY = TmpRenderRatioX;         
            else
              cvUserArea->cvWindowData->RenderRatioX = cvUserArea->cvWindowData->RenderRatioY = TmpRenderRatioY;
        }
        
        cvUserArea->cvWindowData->RenderScaledLenX = (cvUserArea->cvWindowData->RenderRatioX * Packet->ROI_Width);
        cvUserArea->cvWindowData->RenderScaledLenY = (cvUserArea->cvWindowData->RenderRatioY * Packet->ROI_Height);
        
        ShowDiffX = (cvUserArea->cvWindowData->PictLenX - cvUserArea->cvWindowData->RenderScaledLenX)/2;
        ShowDiffY = (cvUserArea->cvWindowData->PictLenY - cvUserArea->cvWindowData->RenderScaledLenY)/2;
        
        cvUserArea->cvWindowData->RenderShowX1 = cvUserArea->cvWindowData->PictX1 + ShowDiffX;
        cvUserArea->cvWindowData->RenderShowY1 = cvUserArea->cvWindowData->PictY1 + ShowDiffY;
   }      
}



static inline void INTERNAL_CVSHWIMG_ExecuteRenderingWithScale (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, IplImage *Image)
/*
    Questa routine provvede ad eseguire il rendering di una immagine quando i parametri
    RenderRatioX e RenderRatioY nella UserArea non sono entrambi posti ad 1 (in quel caso
    è necessario eseguire uno scaling)
*/
{
    register int   _CounterX,    _CounterY;
    register int   _BaseInX,     _BaseInY;
    register int   _BaseOutX,    _BaseOutY;
    
    register float _RatioX, _RatioY;
    register char  _WndHandle;
    
    register int   _RenderScaledLenX, _RenderScaledLenY;
    
    // Inizio del codice
    
    _RatioX = cvUserArea->cvWindowData->RenderRatioX;
    _RatioY = cvUserArea->cvWindowData->RenderRatioY;
                
    _BaseInX   = Packet->ROI_OffsetX;
    _BaseInY   = Packet->ROI_OffsetY;
                    
    _BaseOutX  = cvUserArea->cvWindowData->RenderShowX1;
    _BaseOutY  = cvUserArea->cvWindowData->RenderShowY1;
                
    _WndHandle = Packet->WndHandle;
    
    _RenderScaledLenX = cvUserArea->cvWindowData->RenderScaledLenX;
    _RenderScaledLenY = cvUserArea->cvWindowData->RenderScaledLenY;
    
                
    if (Packet->COI == 0)
    {               
                for ( _CounterY = 0, _BaseOutY = cvUserArea->cvWindowData->RenderShowY1; _CounterY < _RenderScaledLenY; _CounterY++, _BaseOutY++ )
                {  
                      for ( _CounterX = 0, _BaseOutX  = cvUserArea->cvWindowData->RenderShowX1; _CounterX < _RenderScaledLenX; _CounterX++, _BaseOutX++ )
                      {
                         // Se VideoX = x*cvUserArea->cvWindowData->RenderRatioX allora x = VideoX/cvUserArea->cvWindowData->RenderRatioX 
                         // Deposita il valore che serve negli pseudoregistri 
                         
                         INTERNAL_GetPixelFromCurrentImage (_BaseInX + (float)(_CounterX)/(_RatioX), 
                                                            _BaseInY + (float)(_CounterY)/(_RatioY)   );
                           
                         ndWS_PutPixel_NORENDER_FAST (_BaseOutX, _BaseOutY, INTERNAL_CurrentRGBToMagicNumber(), _WndHandle);
                      }
                }                                 
    }
    else        // COI attivato
    {
                for ( _CounterY = 0, _BaseOutY = cvUserArea->cvWindowData->RenderShowY1; _CounterY < _RenderScaledLenY; _CounterY++, _BaseOutY++ )
                {  
                      for ( _CounterX = 0, _BaseOutX  = cvUserArea->cvWindowData->RenderShowX1; _CounterX < _RenderScaledLenX; _CounterX++, _BaseOutX++ )
                      {
                         // Se VideoX = x*cvUserArea->cvWindowData->RenderRatioX allora x = VideoX/cvUserArea->cvWindowData->RenderRatioX 
                         // Deposita il valore che serve negli pseudoregistri 
                         
                         INTERNAL_GetPixelFromCurrentImage (_BaseInX + (float)(_CounterX)/(_RatioX), 
                                                            _BaseInY + (float)(_CounterY)/(_RatioY)   );
                           
                         if (Packet->ColorDepth!=IPL_DEPTH_32F)
                         {
                            if (Packet->COI != 1) HGUI_ColorValue [0]=0;
                            if (Packet->COI != 2) HGUI_ColorValue [1]=0;
                            if (Packet->COI != 3) HGUI_ColorValue [2]=0;
                         }
                         else
                         {
                            if (Packet->COI != 1) HGUI_FlColorValue [0]=0;
                            if (Packet->COI != 2) HGUI_FlColorValue [1]=0;
                            if (Packet->COI != 3) HGUI_FlColorValue [2]=0;
                         }
                 
                         ndWS_PutPixel_NORENDER_FAST (_BaseOutX, _BaseOutY, INTERNAL_CurrentRGBToMagicNumber(), _WndHandle);
                      }
                }  
    }
     
}



static inline void INTERNAL_CVSHWIMG_ExecuteRenderingWithoutScale (struct cvShwImgPacket_Type *Packet, struct cvUserArea_Type *cvUserArea, IplImage *Image)
/*
    Questa routine provvede ad eseguire il rendering di una immagine quando i parametri
    RenderRatioX e RenderRatioY nella UserArea sono entrambi posti ad 1 (in quel caso
    non è necessario eseguire uno scaling)
*/
{
   register int   _CounterX,    _CounterY;
   register int   _BaseInX,     _BaseInY;
   register int   _BaseOutX,    _BaseOutY;
   
   register int   _IndexInX,    _IndexInY;
   register int   _IndexOutX,   _IndexOutY;
   
   register int   _WndHandle;
   
   register int   _RenderScaledLenX, _RenderScaledLenY;
   
   // Inizio del codice
    
   _BaseInX  = Packet->ROI_OffsetX;
   _BaseInY  = Packet->ROI_OffsetY;
   _BaseOutX = cvUserArea->cvWindowData->RenderShowX1;
   _BaseOutY = cvUserArea->cvWindowData->RenderShowY1;
   
   _RenderScaledLenX = cvUserArea->cvWindowData->RenderScaledLenX;
   _RenderScaledLenY = cvUserArea->cvWindowData->RenderScaledLenY;
   
   _WndHandle = Packet->WndHandle;
   
   if (Packet->COI==0)
   {
      for ( _CounterY = 0, _IndexInY = _BaseInY, _IndexOutY = _BaseOutY; _CounterY < _RenderScaledLenY; _CounterY++, _IndexInY++, _IndexOutY++ )
      {    
              for ( _CounterX = 0, _IndexInX = _BaseInX, _IndexOutX = _BaseOutX; _CounterX < _RenderScaledLenX; _CounterX++, _IndexInX++, _IndexOutX++ )
              {
                     // Se VideoX = x*cvUserArea->cvWindowData->RenderRatioX allora x = VideoX/cvUserArea->cvWindowData->RenderRatioX  
                     // Deposita il valore che serve negli pseudoregistri
                     
                     INTERNAL_GetPixelFromCurrentImage (_IndexInX, _IndexInY);      
                     ndWS_PutPixel_NORENDER_FAST (_IndexOutX, _IndexOutY, INTERNAL_CurrentRGBToMagicNumber(), _WndHandle);
                     
              }                
           
      }
   }
   else    // COI attivato
   {
      for ( _CounterY = 0, _IndexInY = _BaseInY, _IndexOutY = _BaseOutY; _CounterY < _RenderScaledLenY; _CounterY++, _IndexInY++, _IndexOutY++ )
      {    
              for ( _CounterX = 0, _IndexInX = _BaseInX, _IndexOutX = _BaseOutX; _CounterX < _RenderScaledLenX; _CounterX++, _IndexInX++, _IndexOutX++ )
              {
                     // Se VideoX = x*cvUserArea->cvWindowData->RenderRatioX allora x = VideoX/cvUserArea->cvWindowData->RenderRatioX  
                     // Deposita il valore che serve negli pseudoregistri
                     
                     INTERNAL_GetPixelFromCurrentImage (_IndexInX, _IndexInY); 
                     
                     if (Packet->ColorDepth!=IPL_DEPTH_32F)
                     {
                           if (Packet->COI != 1) HGUI_ColorValue [0]=0;
                           if (Packet->COI != 2) HGUI_ColorValue [1]=0;
                           if (Packet->COI != 3) HGUI_ColorValue [2]=0;
                     }
                     else
                     {
                           if (Packet->COI != 1) HGUI_FlColorValue [0]=0;
                           if (Packet->COI != 2) HGUI_FlColorValue [1]=0;
                           if (Packet->COI != 3) HGUI_FlColorValue [2]=0;
                     }
                          
                     ndWS_PutPixel_NORENDER_FAST (_IndexOutX, _IndexOutY, INTERNAL_CurrentRGBToMagicNumber(), _WndHandle);
              }                
      }
   }     
}









void cvShowImage (char* WindowTitle, IplImage *Image)
/*   ------------------------------------------------------------------------------------------------
     Routine cvShowImage                                                              Pubblica              
     -------------------------------------------------------------------------------------------------
     Questa routine mostra nella finestra con nome NameWindow l'immagine puntata dal puntatore 
     image.
     
     Inoltre vengono mostrate le eventuali trackbars che sono state associate all'immagine
     attraverso la routine cvCreateTrackBar.
     
     Il comportamento della routine dipende anche dai parametri che sono stati usati quando è
     stata creata una finestra usando cvNamedWindow. 
     
     Se allora era stato settato il flag CV_USE_NDWSWND, ndHighGUI tenterà di usare tutto il
     window space a disposizione di ciascuna finestra, visualizzando eventualmente solo una
     parte dell'immagine sullo schermo e lasciando all'utente la possibilità di scorrere
     il resto dell'immagine usando le barre di scroll. Solo se le dimensioni dell'immagine 
     sono maggiori della massima dimensione del window space consentita da Nanodesktop,
     la routine cvShowImage eseguirà uno scaling dell'immagine per adattarla alle 
     dimensioni del window space. Si tenga inoltre presente che una finestra in cui il
     flag CV_USE_NDWSWND è stato settato, **non può** contenere trackbar. 
     
     Se al contrario, al momento della creazione della finestra per mezzo della routine
     cvNamedWindow, non era stato usato il flag CV_USE_NDWSWND, allora la routine
     eseguirà dei calcoli preliminari allo scopo di calcolare quando spazio è
     possibile concedere all'immagine, visualizzandola insieme alle eventuali trackbars,
     e senza usare le barre di scrolling. Dopo avere eseguito i calcoli, la routine
     esegue uno scaling opportuno, allo scopo di consentire la visualizzazione 
     dell'immagine.
     
     Se nell'immagine è stata settata una ROI (Region of interest), allora cvShowImage
     visualizzerà solo la ROI in luogo dell'immagine. 
     
     Durante la fase di ridimensionamento verrà comunque mantenuto il rapporto tra 
     larghezza ed altezza dell'immagine. 
     
     Si tenga presente che il ridimensionamento riguarda solo l'immagine visualizzata, 
     e non il contenuto della struct IplImage image.
     
     Se la finestra è minimizzata, essa viene automaticamente massimizzata.
     
     Se l'utente non aveva allocato una finestra apposita per contenere l'immagine, essa
     viene automaticamente allocata.
     
     La routine non restituisce codici di errore in uscita, però, se si verificano errori,
     essi vengono segnalati restituendo un apposito codice di errore nella variabile di
     sistema HGUI_SystemError. 
     
     Sono previsti i seguenti codici di errore:
          
     NDHGUI_I_CANNOT_DO_WND_AUTOALLOCATION     Impossibile eseguire l'autoallocazione della
                                               finestra
     NDHGUI_I_CANNOT_DO_WINDOW_ALLOCATION      Impossibile eseguire l'allocazione della finestra
     NDHGUI_ERR_IN_NDTRACKBAR_ALLOCATION       Errore in Nanodesktop trackbar allocation 
     NDHGUI_I_CANNOT_DRAW_ALL_TRACKBARS        Non c'è lo spazio sufficiente per disegnare tutte
                                               le trackbars
     --------------------------------------------------------------------------------------------------
*/
{ 
     char ErrHGUIRegisters, ErrRep;
     
     struct cvShwImgPacket_Type Packet;
     struct cvUserArea_Type     *cvUserArea;
     
     int    UserAreaSize;
        
     // Inizio del codice
     
     if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
     
     // Determina i parametri relativi all'immagine
          
     INTERNAL_CVSHWIMG_SetupImageParameters (&Packet, Image);
         
     // Provvedi eventualmente alla window autoallocation
     
     ErrRep=INTERNAL_CVSHWIMG_ManageWindowAutoAllocation (&Packet, Image, WindowTitle);
     if (ErrRep) return;
     
     // A questo punto, di sicuro una finestra allocata ci sarà: preleva i dati relativi
     // alla UserArea della finestra. Questi dati saranno passati a tutte le routines
     // successive
     
     ndLP_GetUserAreaInfo (Packet.WndHandle, &cvUserArea, &UserAreaSize);
    
     // Gestisci l'eventuale cambiamento di stato della finestra e ricalcola eventualmente
     // i parametri Pict
    
    
     ErrRep=INTERNAL_CVSHWIMG_ManageChangeState (&Packet, cvUserArea, Image, WindowTitle);
     if (ErrRep) return;                

     
    
     // Arrivati a questo punto, le coordinate Pict sono state determinate.
     
     if (Image!=0)            // Si chiede di visualizzare un immagine non nulla
     {                                                           
        // Provvediamo a visualizzare l'immagine
        
        ErrHGUIRegisters = INTERNAL_SetupHGUIRegisters (Image);    // Reinizializza gli pseudoregistri
        
        if (!ErrHGUIRegisters)        // Non ci sono problemi nella gestione dell'immagine
        {                                             
                    if ((cvUserArea->cvWindowData->RenderRatioX!=1) || (cvUserArea->cvWindowData->RenderRatioY!=1))
                    {               
                        INTERNAL_CVSHWIMG_ExecuteRenderingWithScale (&Packet, cvUserArea, Image);
                    }
                    else  // cvUserArea->cvWindowData->RenderRatioX e cvUserArea->cvWindowData->RenderRatioY sono entrambi a 1
                    {   
                        INTERNAL_CVSHWIMG_ExecuteRenderingWithoutScale (&Packet, cvUserArea, Image);
                    }

        }     // end if (!ErrHGUIRegisters)

     }  
     else      // Si richiede di visualizzare un'immagine nulla
     {
          ndWS_DrawRectangle (cvUserArea->cvWindowData->PictX1, cvUserArea->cvWindowData->PictY1, 
                                  cvUserArea->cvWindowData->PictX2, cvUserArea->cvWindowData->PictY2, 
                                      COLOR_BLACK, COLOR_BLACK, Packet.WndHandle, NORENDER);
     }
       
       
                              
     cvUserArea->cvWindowData->LastIplImage = Image;
     // Registra l'ultima immagine visualizzata

     XWindowRender (Packet.WndHandle);
     // Esegue l'x-window render della finestra
      
     ndLP_MaximizeWindow (Packet.WndHandle);
     // Massimizza la finestra: se questa è già massimizzata non avverrà nulla.
      
     HGUI_SystemError = 0;      // Segnala che l'operazione è terminata con successo
     
     
     
     return;     
}

/*
   ----------------------------------------------------------------------------
   ALTRE FUNZIONI
   ----------------------------------------------------------------------------
*/



void cvForceTrackbarRender ( const char* window_name )
/*  ---------------------------------------------------------------------------
    Routine cvForceTrackbarRender                                   Pubblica            
    ---------------------------------------------------------------------------
    Questa routine non fa parte del normale corredo delle librerie HighGUI di
    Intel. 
    
    La routine, se usata in modo non corretto, può causare crash di sistema,
    pertanto è necessario che l'utente sappia che cosa sta facendo. 
    
    In pratica, la routine forza la finestra con nome window_name ad eseguire
    un nuovo rendering, ossia a disegnare le nuove trackbars di finestra.
    
    Ciò è utile qualora la configurazione delle trackbar associate ad una
    finestra sia stata modificata, o perchè l'utente ha aggiunto delle
    nuove barre con cvCreateTrackbar o perchè l'utente ne ha eliminate
    alcune cvDestroyTrackbar.
    
    Si tenga presente che il renderer eseguirà anche il disegno dell'ultima
    immagine che era stata visualizzata con una cvShowImage, e se tale immagine
    nel frattempo è stata disallocata, il sistema avrà un crash. 
    
    ndHighGUI non ha modo di eseguire un controllo preliminare per sapere se
    l'ultima immagine è ancora in memoria o meno, perchè le librerie HighGUI
    e OpenCV di Intel prevedono che un'immagine si possa cancellare dalla
    memoria con un semplice free (che ndHighGUI non può intercettare e 
    gestire). Pertanto deve essere l'utente a sapere cosa sta facendo. 
    
    La routine non restituisce valori in uscita, ma qualora si verificasse un
    errore, verrà memorizzato un apposito codice nella variabile di ambiente
    HGUI_SystemError. 
    
    Sono previsti i seguenti codici di errore:
         NDHGUI_WINDOW_NOT_FOUND    La finestra non è stata trovata
    ---------------------------------------------------------------------------
*/
{
  if (!cvInitSystemCalled) cvInitSystem(0, 0);           // Richiama cvInitSystem se già non è stato fatto       
    
  struct cvUserArea_Type *cvUserArea;
  int UserAreaSize;
  char WndHandle;
  
  WndHandle = INTERNAL_FindWndByName (window_name);
  
  if (WndHandle!=NDHGUI_MSG_WINDOW_NOT_FOUND)
  { 
     ndLP_GetUserAreaInfo (WndHandle, &cvUserArea, &UserAreaSize);
  
     cvUserArea->cvWindowData->WndStateIsChanged=1;
     // Forza il renderer ad attivarsi
     
     cvShowImage (window_name, cvUserArea->cvWindowData->LastIplImage);
     // Qui potrebbe verificarsi il crash. Attenzione.
     
     return;         // Si noti che HGUI_SystemError potrebbe essere ad un valore !=0
                     // se c'è stato qualche errore
  } 
  else   // La finestra non è stata trovata
  {
     HGUI_NotifyError (-2, 0, "cvGetTrackbarPos", "ndHighGUI window not found");
     
     HGUI_SystemError=NDHGUI_WINDOW_NOT_FOUND;  
     return;    
  }  
   
} 




