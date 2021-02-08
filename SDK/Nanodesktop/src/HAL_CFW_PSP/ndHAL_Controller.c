/*  
    ---------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2008-2013 Filippo Battaglia 
    ---------------------------------------------------------------------------
    Version for PSP/PSPE
    ---------------------------------------------------------------------------
    Nanodesktop library provides a simple windows environment for PSP/PSPE
    platform.
    
    This work has been developed by the author as research project in Visilab
    Research Center - University of Messina - Italy
    ---------------------------------------------------------------------------
    IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

    By downloading, copying, installing or using the software you agree to 
    Nanodesktop license, a modified version of BSD license. Read the file
    license.doc in Nanodesktop folder for details.  

    If you do not agree to this license, do not download, install,
    copy or use the software.     
    ---------------------------------------------------------------------------
    ROUTINE PER LA GESTIONE DEL CONTROLLER (PSP CFW mode)
    ---------------------------------------------------------------------------
*/   
   
#include <HAL_CFW_PSP\$_HAL_NDforND.h>


char ndHAL_GetPad (struct ndDataControllerType *ndPadRecord)
/* 
   Questa routine si aggancia alle routine del controller della piattaforma
   utilizzata e fornisce in uscita un codice che consente all'applicazione
   chiamante di determinare la posizione del PAD e l'eventuale pressione
   di tasti funzione.
*/
{
   if (!HAL_MouseEmuSystemEnabled )
   {
        return (CORE_GetPad (ndPadRecord));
   }
   else
   {
        // Se il sistema di gestione del mouse è attivato, il phoenix mouse
        // thread deve restituire il valore della struct IndirectPadRecord
        // registrata dal Phoenix Mouse Thread
        
        ndPadRecord->TimeStamp = IndirectPadRecord.TimeStamp;
        ndPadRecord->Buttons   = IndirectPadRecord.Buttons;
        ndPadRecord->Lx        = IndirectPadRecord.Lx;
        ndPadRecord->Ly        = IndirectPadRecord.Ly;
        ndPadRecord->Analog    = 0;
        return 0;
   }
}

char ndHAL_GetPad_Analog (struct ndDataControllerType *ndPadRecord)
/* 
   Analoga alla ndHAL_GetPad, ma restituisce il valore di posizione del
   PAD analogico. Non è supportata in tutte le piattaforme.
*/
{
   if (!HAL_MouseEmuSystemEnabled )
   {
        return (CORE_GetPadAnalog (ndPadRecord));
   }
   else
   {
        // Se il sistema di gestione del mouse è attivato, il phoenix mouse
        // thread deve restituire il valore della struct IndirectPadRecord
        // registrata dal Phoenix Mouse Thread
        
        ndPadRecord->TimeStamp = IndirectPadRecord.TimeStamp;
        ndPadRecord->Buttons   = IndirectPadRecord.Buttons;
        ndPadRecord->Lx        = IndirectPadRecord.Lx;
        ndPadRecord->Ly        = IndirectPadRecord.Ly;
        ndPadRecord->Analog    = 1;
        return 0;
   } 
}





char CORE_GetPad (struct ndDataControllerType *ndPadRecord)
{
  SceCtrlData MyPSPPadRecord;
                                                                                    
  if (PadAnalogModeEnabled==1)    // Modalità pad analogico abilitata
  {
     sceCtrlSetSamplingMode (PSP_CTRL_MODE_DIGITAL);
     PadAnalogModeEnabled=0;
  }
  
  sceCtrlReadBufferPositive (&MyPSPPadRecord, 1);
  // L'emulatore PSPE e il bios della PSP usano due diverse funzionalità per 
  // l'accesso a questa funzione: cambiando il nome della routine del kernel che
  // viene richiamata da ndCTRL_GetPad è possibile nascondere la differenza
  // al programma chiamante.
  
  ndPadRecord->TimeStamp = MyPSPPadRecord.TimeStamp;
  ndPadRecord->Buttons   = MyPSPPadRecord.Buttons;
  ndPadRecord->Lx        = MyPSPPadRecord.Lx;
  ndPadRecord->Ly        = MyPSPPadRecord.Ly;
  ndPadRecord->Analog    = 0;
  
  return 0;
}

char CORE_GetPadAnalog (struct ndDataControllerType *ndPadRecord)
{
  SceCtrlData MyPSPPadRecord;
                                                                                    
  if (PadAnalogModeEnabled==0)    // Modalità pad analogico disabilitata
  {
     sceCtrlSetSamplingMode (PSP_CTRL_MODE_ANALOG);         // Abilitiamola
     PadAnalogModeEnabled=1;
  }
  
  sceCtrlReadBufferPositive (&MyPSPPadRecord, 1);
  // L'emulatore PSPE e il bios della PSP usano due diverse funzionalità per 
  // l'accesso a questa funzione: cambiando il nome della routine del kernel che
  // viene richiamata da ndCTRL_GetPad è possibile nascondere la differenza
  // al programma chiamante.
  
  ndPadRecord->TimeStamp = MyPSPPadRecord.TimeStamp;
  ndPadRecord->Buttons   = MyPSPPadRecord.Buttons;
  ndPadRecord->Lx        = MyPSPPadRecord.Lx;
  ndPadRecord->Ly        = MyPSPPadRecord.Ly;
  ndPadRecord->Analog    = 1;
  
  return 0;      
}


void ndHAL_WaitKey (int Param)
/* 
   Questa routine attende la pressione di un tasto
*/
{
   unsigned char Exit;
   struct ndDataControllerType Pad;
   
   Exit=0;
   
   while (!Exit)
   {
         ndHAL_GetPad (&Pad);
         
         if (Pad.Buttons != 0)
         {
			if (Pad.Buttons & ND_BUTTON1)
            {
               Exit=1;
            }
			
			if (Pad.Buttons & ND_BUTTON2)
            {
               Exit=1;             
			}
             
			if (Pad.Buttons & ND_BUTTON3)
            {
               Exit=1;             
			}
             
			if (Pad.Buttons & ND_BUTTON4)
            {
               Exit=1;
			} 

			if (Pad.Buttons & ND_CTRL_UP)
            {
               Exit=1;
            }
			
		    if (Pad.Buttons & ND_CTRL_DOWN)
		    {
               Exit=1;
            }
             
			if (Pad.Buttons & ND_CTRL_LEFT)
			{  
               Exit=1;
            }
             
            if (Pad.Buttons & ND_CTRL_RIGHT)
            {
			   Exit=1;
            }
			
                  
		}
	}
}



