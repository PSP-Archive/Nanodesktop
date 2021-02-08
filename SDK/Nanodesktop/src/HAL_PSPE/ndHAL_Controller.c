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
    ROUTINES PER LA GESTIONE DEL CONTROLLER (PSPE)
    ---------------------------------------------------------------------------
*/   
   
#include <HAL_PSPE\$_HAL_NDforND.h>


char ndHAL_GetPad (struct ndDataControllerType *ndPadRecord)
/* 
 ----------------------------------------------------------------------------------
   Questa routine si aggancia alle routine del controller della piattaforma
   utilizzata e fornisce in uscita un codice che consente all'applicazione
   chiamante di determinare la posizione del PAD e l'eventuale pressione
   di tasti funzione.
 -----------------------------------------------------------------------------------
*/

{
  SceCtrlData MyPSPPadRecord;
                                                                                    
  sceCtrlRead (&MyPSPPadRecord, 1);
  // L'emulatore PSPE e il bios della PSP usano due diverse funzionalità per 
  // l'accesso a questa funzione: cambiando il nome della routine del kernel che
  // viene richiamata da ndCTRL_GetPad è possibile nascondere la differenza
  // al programma chiamante.
  
  ndPadRecord->TimeStamp = 0;
  ndPadRecord->Buttons   = MyPSPPadRecord.Buttons;
  ndPadRecord->Lx        = 0;
  ndPadRecord->Ly        = 0;
  ndPadRecord->Analog    = 0;

  return 0;
}

char ndHAL_GetPad_Analog (struct ndDataControllerType *ndPadRecord)
/* 
 ------------------------------------------------------------------------------
   Analoga alla ndHAL_GetPad, ma restituisce il valore di posizione del
   PAD analogico. Non è supportata in tutte le piattaforme.
 ------------------------------------------------------------------------------
*/

{
   return ERR_FUNCTION_NOT_SUPPORTED;  // La funzione non è supportata da PSPE
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



