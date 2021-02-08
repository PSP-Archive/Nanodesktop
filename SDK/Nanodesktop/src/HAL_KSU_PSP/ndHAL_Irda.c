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
    API PER LA GESTIONE DI IRDA (PSP KSU mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

/* 
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                              Funzioni monitor per IRDA
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/

char ndHAL_IR_EnableIrdaMonitor ()
/*
    Questa routine provvede ad attivare il monitor per i segnali irda. 
    
    Sono previsti i seguenti codici di errore:
         ERR_INTERNAL_ERROR         Errore interno
*/
{
    if (!IrdaMonitorEnabled)
    {
        IrdaMonitorHandle = ndLP_CreateWindow (110, 5, 455, 255, "IRDA Monitor", COLOR_WHITE, COLOR_RED, COLOR_BLACK, COLOR_WHITE, 0);
                            
        if (IrdaMonitorHandle>=0)       // Non ci sono errori nell'allocazione della nuova finestra
        {
            ndLP_MaximizeWindow (IrdaMonitorHandle);
            IrdaMonitorEnabled=1;
            
            return 0;
        }
        else // Problema nell'allocazione della nuova finestra
          return ERR_INTERNAL_ERROR;
    } 
    
    // Il monitor è già attivato. Simula successo
    
    return 0;
}


void ndHAL_IR_DisableIrdaMonitor ()
/*
    Provvede a disattivare il monitor per i segnali irda
*/
{
     if (IrdaMonitorEnabled) 
     {
        ndLP_DestroyWindow (IrdaMonitorHandle);
        IrdaMonitorEnabled=0;
     }
}


/* 
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                              Funzioni send per IRDA
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/



int INTERNAL_ndHAL_SendIRDA_CLIENT (int IrHandleChannel, int Address, int Number, char MonitorEnabled)
/*
   Questo client si occupa di inviare i dati in formato IRDA eventualmente usando il
   monitor di sistema. 
   
   In uscita viene fornito il numero di bytes effettivamente inviati
*/
{
   static char IrdaDataBuffer;
   
   char _UseMonitor;
   char _Trasmission;
  
   int Counter, CharCounter;
   int IOResult;
   
   int BytesSent;
   
   int ErrMonitorRep;
   
   char *pntByte;
   unsigned char CharBuffer [8];
   int  CharBufferOffset;
   
   TypeColor HexColor;
   
   _UseMonitor=0;
   _Trasmission=1;
              
   if (MonitorEnabled==1)
   {
     _UseMonitor=1;
     _Trasmission=1;
   }
  
   if (MonitorEnabled==2)
   {
     _UseMonitor=1;
     _Trasmission=0;
   }
   
   if (_UseMonitor)
   {
      ErrMonitorRep = ndHAL_IR_EnableIrdaMonitor (); 
      if (ErrMonitorRep!=0) _UseMonitor=0;       // Ci sono problemi nell'allocazione. Disattiva il monitor
   }
   
   BytesSent=0;
   
   CharBufferOffset=0;
   
   ndIrChannelsArray [IrHandleChannel].IrError=0;
   
   for (Counter=0; Counter<Number; Counter++)
   {
      pntByte = Address + Counter;               // Punta al byte in memoria
      
      if (_Trasmission)
      {        
         IrdaDataBuffer = *pntByte;              // Il doppio passaggio impedisce un crash per disallineamento MIPS
         IOResult=sceIoWrite (ndIrChannelsArray [IrHandleChannel].DevHandle, &IrdaDataBuffer, 1);
      }
      else                   
         IOResult=1;         // Si è scelto di non trasmettere. Simula successo
      
      if (_UseMonitor)
      {
              if (CharBufferOffset<8)               
              {
                  CharBuffer [CharBufferOffset]=*pntByte;      // Memorizza il carattere
                  CharBufferOffset++;                          // ed incrementa il puntatore
              }
              else         // CharBufferOffset è al massimo: stampa e reinizializza
              {
                  ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, RENDER, "IRDA Send ");
                  
                  for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
                  {
                      ndWS_GoCursor (12+2*CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                      
                      if ((CharCounter%2)==0) HexColor=COLOR_WHITE;
                         else HexColor=COLOR_YELLOW;
                      
                      ndWS_Print (IrdaMonitorHandle, HexColor, NORENDER, "%X", CharBuffer [CharCounter]);
                  }
                  
                  for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
                  {
                      ndWS_GoCursor (30+CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                      
                      if (CharBuffer [CharCounter]!='\n')
                         ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, "%c", CharBuffer [CharCounter]);
                      else
                         ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, " ", CharBuffer [CharCounter]); 
                  }
                  
                  ndWS_CarriageReturn (IrdaMonitorHandle);
                  XWindowRender (IrdaMonitorHandle);
                  ndHAL_HardDelay (1);
                  
                  CharBufferOffset=0;
              }           
      }
      
      if (IOResult==1)
      {
           BytesSent++;
      }
      else            // Si è verificato un errore disco
      {
           ndIrChannelsArray [IrHandleChannel].IrError = ERR_IO_ERROR;
           break;            // Esci dal ciclo for         
      }
      
   }
   
   
   // Completa visualizzando degli eventuali caratteri rimasti in buffer
   
   if (_UseMonitor)
   {       
           ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, RENDER, "IRDA Send ");
           
           for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
           {
              ndWS_GoCursor (12+2*CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
              
              if ((CharCounter%2)==0) HexColor=COLOR_WHITE;
                  else HexColor=COLOR_YELLOW;
              
              ndWS_Print (IrdaMonitorHandle, HexColor, NORENDER, "%X", CharBuffer [CharCounter]);
           }
           
           for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
           {
              ndWS_GoCursor (30+CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                      
              if (CharBuffer [CharCounter]!='\n')
                  ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, "%c", CharBuffer [CharCounter]);
              else
                  ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, " ", CharBuffer [CharCounter]); 
           }
          
           ndWS_CarriageReturn (IrdaMonitorHandle);
           
           if (ndIrChannelsArray [IrHandleChannel].IrError!=0) 
           {
              ndWS_CarriageReturn (IrdaMonitorHandle);
              ndWS_PrintLn (IrdaMonitorHandle, COLOR_RED, NORENDER, "IRDA FAILURE");
              ndWS_CarriageReturn (IrdaMonitorHandle);
           }
           
           XWindowRender (IrdaMonitorHandle);
           ndHAL_HardDelay (1);
          
           CharBufferOffset=0;
   }
   
   return BytesSent;
}


/* 
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
                              Funzioni receiver per IRDA
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
*/


int INTERNAL_ndHAL_ReceiveIRDA_CLIENT (int IrHandleChannel, int Address, int Number, char MonitorEnabled)
/*
   Questo client si occupa di ricevere i dati in formato IRDA eventualmente usando il
   monitor di sistema. 
   
   In uscita viene fornito il numero di bytes effettivamente ricevuti
*/
{
   char _UseMonitor;
   char _Receiver;
  
   int Counter, CharCounter;
   int IOResult;
   
   int BytesReceived;
   
   int ErrMonitorRep;
   
   char *pntByte;
   unsigned char CharBuffer [8];
   int  CharBufferOffset;
   
   TypeColor HexColor;
   
   _UseMonitor=0;
   _Receiver=1;
              
   if (MonitorEnabled==1)
   {
     _UseMonitor=1;
     _Receiver=1;
   }
  
   if (MonitorEnabled==2)
   {
     _UseMonitor=1;
     _Receiver=0;
   }
   
   if (_UseMonitor)
   {
      ErrMonitorRep = ndHAL_IR_EnableIrdaMonitor (); 
      if (ErrMonitorRep!=0) _UseMonitor=0;       // Ci sono problemi nell'allocazione. Disattiva il monitor
   }
   
   BytesReceived=0;
   
   CharBufferOffset=0;
   
   ndIrChannelsArray [IrHandleChannel].IrError=0;
      
   if (_Receiver)
   {        
            // Tenta di leggere dal sensore IRDA un numero di bytes pari a Number
            IOResult=sceIoRead (ndIrChannelsArray [IrHandleChannel].DevHandle, Address, Number);
   }
   else     
   {
            // Si vuole semplicemente simulare la lettura. Riempi la memoria destinazione di
            // simboli per simulare successo.
            
            for (Counter=0; Counter<Number; Counter++)
            {
                pntByte  = Address + Counter;
                *pntByte = 0xFF;
            }  
                          
            IOResult=Number;         // Si è scelto di non ricevere. Simula successo
   }

   // A questo punto dentro IOResult c'è il numero di bytes ricevuti, oppure, se negativo
   // c'è una condizione di errore.
   
   if (IOResult>=0)
   {
        ndIrChannelsArray [IrHandleChannel].IrError = 0;
        BytesReceived=IOResult;
   }
   else
   {
        ndIrChannelsArray [IrHandleChannel].IrError = ERR_IO_ERROR;
        BytesReceived=0;
   }
   
   if (_UseMonitor)
   {
        if (IOResult>0)     // Non ci sono stati errori nella ricezione
        {
              CharBufferOffset=0;
              
              ndWS_PrintLn (IrdaMonitorHandle, COLOR_LBLUE, RENDER, "IRDA Receiver: nr bytes received: %d ", BytesReceived);
              ndWS_CarriageReturn (IrdaMonitorHandle);
              
              for (Counter=0; Counter<BytesReceived; Counter++)
              {
                      if (CharBufferOffset<8)               
                      {
                          pntByte  = Address + Counter;
                          
                          CharBuffer [CharBufferOffset]=*pntByte;      // Memorizza il carattere
                          CharBufferOffset++;                          // ed incrementa il puntatore
                      }
                      else         // CharBufferOffset è al massimo: stampa e reinizializza
                      {
                          ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, RENDER, "IRDA Recv ");
                          
                          for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
                          {
                              ndWS_GoCursor (12+2*CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                              
                              if ((CharCounter%2)==0) HexColor=COLOR_WHITE;
                                 else HexColor=COLOR_YELLOW;
                              
                              ndWS_Print (IrdaMonitorHandle, HexColor, NORENDER, "%X", CharBuffer [CharCounter]);
                          }
                          
                          for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
                          {
                              ndWS_GoCursor (30+CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                              
                              if (CharBuffer [CharCounter]!='\n')
                                 ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, "%c", CharBuffer [CharCounter]);
                              else
                                 ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, " ", CharBuffer [CharCounter]); 
                          }
                          
                          ndWS_CarriageReturn (IrdaMonitorHandle);
                          XWindowRender (IrdaMonitorHandle);
                          ndHAL_HardDelay (1);
                          
                          CharBufferOffset=0;
                      }           
              }                   // End ciclo for di visualizzazione
                
              // Provvedi a visualizzare gli ultimi bytes
                
              ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, RENDER, "IRDA Recv ");
           
              for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
              {
                          ndWS_GoCursor (12+2*CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                          
                          if ((CharCounter%2)==0) HexColor=COLOR_WHITE;
                              else HexColor=COLOR_YELLOW;
                          
                          ndWS_Print (IrdaMonitorHandle, HexColor, NORENDER, "%X", CharBuffer [CharCounter]);
              }
               
              for (CharCounter=0; CharCounter<CharBufferOffset; CharCounter++)
              {
                          ndWS_GoCursor (30+CharCounter, WindowData [IrdaMonitorHandle].WS_PosCursY, IrdaMonitorHandle);
                                  
                          if (CharBuffer [CharCounter]!='\n')
                              ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, "%c", CharBuffer [CharCounter]);
                          else
                              ndWS_Print (IrdaMonitorHandle, COLOR_LBLUE, NORENDER, " ", CharBuffer [CharCounter]); 
              }
              
              ndWS_CarriageReturn (IrdaMonitorHandle);
               
              XWindowRender (IrdaMonitorHandle);
              ndHAL_HardDelay (1);
        }   
               
        if (IOResult<0)       // Si è verificato un errore nella ricezione IRDA
        {
              ndWS_CarriageReturn (IrdaMonitorHandle);
              ndWS_PrintLn (IrdaMonitorHandle, COLOR_RED, NORENDER, "IRDA READ FAILURE");
              ndWS_CarriageReturn (IrdaMonitorHandle);
        }   
   }        // if (_UseMonitor)    
               
   return BytesReceived;            
}


