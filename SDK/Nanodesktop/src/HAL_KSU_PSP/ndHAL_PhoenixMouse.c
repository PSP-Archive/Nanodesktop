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
    ----------------------------------------------------------------------------
    ROUTINE PHOENIX MOUSE THREAD (PSP KSU mode)
    ----------------------------------------------------------------------------
*/

#include <HAL_KSU_PSP\$_HAL_NDforND.h>

unsigned short int VMouseAcceleration=0;


void ndHAL_MouseEmu (void)
/*
     Questa routine provvede ad implementare l'emulazione del mouse, sulle
     piattaforme che non supportano un puntatore hardware.
*/
{
   struct ndDataControllerType Pad;
   
   register int Theor_MousePosX;
   register int Theor_MousePosY;
   
   if (!HAL_MouseIsControlledByAnalog)
   {                      
               register int MStep;
               
               // Provvede a leggere lo stato del mouse, richiamando la routine core
               CORE_GetPad (&Pad);
               
               // Salva il contenuto del registro pad che è stato letto. Viene utile se qualche
               // altro thread esegue una ndHAL_GetPad con il mouse attivato
               
               IndirectPadRecord.TimeStamp    = Pad.TimeStamp;
               IndirectPadRecord.Buttons      = Pad.Buttons;
               IndirectPadRecord.Lx           = Pad.Lx;
               IndirectPadRecord.Ly           = Pad.Ly;
               
               // Inizia a processare i tasti richiesti
               
               if (Pad.Buttons != 0)
               {
                   HAL_MouseButtons=0;         
                               
                   if ( (!(Pad.Buttons & PSP_CTRL_CROSS)) && (!(Pad.Buttons & PSP_CTRL_CIRCLE)) && (!(Pad.Buttons & PSP_CTRL_SELECT)) )           
                   {
                          HAL_MouseVectorX=0;
                          HAL_MouseVectorY=0;
                          
                          VMouseAcceleration=VMouseAcceleration+1;
                          
                          if (VMouseAcceleration>60) VMouseAcceleration=60;
                          MStep = (VMouseAcceleration/10)+1;
                          
                          if (Pad.Buttons & PSP_CTRL_UP)
                          {
                             Theor_MousePosY=HAL_MouseEmu_PosY - MStep;
                             
                             if (Theor_MousePosY>=0) 
                                HAL_MouseEmu_PosY = Theor_MousePosY;
                             else
                                HAL_MouseEmu_PosY = 0; 
                          }
                             
                          if (Pad.Buttons & PSP_CTRL_DOWN)
                          {
                             Theor_MousePosY=HAL_MouseEmu_PosY + MStep;
                             
                             if (Theor_MousePosY<=Mouse_MaxScreenY) 
                                HAL_MouseEmu_PosY = Theor_MousePosY;
                             else
                                HAL_MouseEmu_PosY = Mouse_MaxScreenY; 
                          }   
                             
                          if (Pad.Buttons & PSP_CTRL_LEFT)
                          {
                             Theor_MousePosX=HAL_MouseEmu_PosX - MStep;
                             
                             if (Theor_MousePosX>=0) 
                                HAL_MouseEmu_PosX = Theor_MousePosX;
                             else
                                HAL_MouseEmu_PosX = 0;
                          }  
                          
                          if (Pad.Buttons & PSP_CTRL_RIGHT)
                          {
                             Theor_MousePosX=HAL_MouseEmu_PosX + MStep;
                             
                             if (Theor_MousePosX<=Mouse_MaxScreenX) 
                                HAL_MouseEmu_PosX = Theor_MousePosX;
                             else
                                HAL_MouseEmu_PosX = Mouse_MaxScreenX;     
                          }
                          
                          return;
                   }
                   
                   if (Pad.Buttons & PSP_CTRL_CROSS)
                   {
                          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON1;
                          HAL_MouseVectorX=0;
                          HAL_MouseVectorY=0;
                          
                          if (Pad.Buttons & PSP_CTRL_UP)
                          {
                             HAL_MouseVectorY=-10; 
                          }
                             
                          if (Pad.Buttons & PSP_CTRL_DOWN)
                          {
                             HAL_MouseVectorY=+10; 
                          }   
                             
                          if (Pad.Buttons & PSP_CTRL_LEFT)
                          {
                             HAL_MouseVectorX=-10;
                          }  
                          
                          if (Pad.Buttons & PSP_CTRL_RIGHT)
                          {
                             HAL_MouseVectorX=+10;     
                          }
                          
                          if ((HAL_MouseVectorX==0) && (HAL_MouseVectorY==0)) 
                             ndHAL_RequestDelay (ND_REQDELAY_MOUSECTRL);
                          
                          return;              
                   }
                   
                   if (Pad.Buttons & PSP_CTRL_CIRCLE)
                   {
                          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON4;
                          HAL_MouseVectorX=0;
                          HAL_MouseVectorY=0;
                          
                          if (Pad.Buttons & PSP_CTRL_UP)
                          {
                             HAL_MouseVectorY=-1; 
                          }
                             
                          if (Pad.Buttons & PSP_CTRL_DOWN)
                          {
                             HAL_MouseVectorY=+1; 
                          }   
                             
                          if (Pad.Buttons & PSP_CTRL_LEFT)
                          {
                             HAL_MouseVectorX=-1;
                          }  
                          
                          if (Pad.Buttons & PSP_CTRL_RIGHT)
                          {
                             HAL_MouseVectorX=+1;     
                          }
                          
                          return;              
                   }
                   
                   if (Pad.Buttons & PSP_CTRL_SELECT)
                   {
                          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON5;
                          HAL_MouseVectorX=0;
                          HAL_MouseVectorY=0;
                          
                          return;
                   }
                   
               }
               else   // Non è stato premuto alcun tasto
               {
                      HAL_MouseButtons=0;
                      HAL_MouseVectorX=0;
                      HAL_MouseVectorY=0;  
                      
                      VMouseAcceleration=0;            // Accelerazione vmouse reinizializzata
                         
                      return;
               }                                                                 
   }
   else    // Mouse controllato per via analogica
   {
               register int MStepX, MStepY;
               
               // Provvede a leggere lo stato del mouse, richiamando la routine core in analogico
               CORE_GetPadAnalog (&Pad);
                   
               // Salva il contenuto del registro pad che è stato letto. Viene utile se qualche
               // altro thread esegue una ndHAL_GetPad con il mouse attivato
               
               IndirectPadRecord.TimeStamp    = Pad.TimeStamp;
               IndirectPadRecord.Buttons      = Pad.Buttons;
               IndirectPadRecord.Lx           = Pad.Lx;
               IndirectPadRecord.Ly           = Pad.Ly;
               
               // Determina i valori di MStepX e MStepY
               MStepX = (Pad.Lx - 128)/24;
               MStepY = (Pad.Ly - 128)/24;
               
               // Inizia a processare i tasti richiesti
               
               if ((Pad.Buttons != 0) || (MStepX!=0) || (MStepY!=0))
               {
                   HAL_MouseButtons=0;         
                               
                   if ( (!(Pad.Buttons & PSP_CTRL_CROSS)) && (!(Pad.Buttons & PSP_CTRL_CIRCLE)) && (!(Pad.Buttons & PSP_CTRL_SELECT)) )           
                   {
                          HAL_MouseVectorX=0;
                          HAL_MouseVectorY=0;
                          
                          if (MStepY<0)
                          {
                             Theor_MousePosY=HAL_MouseEmu_PosY + MStepY;
                             
                             if (Theor_MousePosY>=0) 
                                HAL_MouseEmu_PosY = Theor_MousePosY;
                             else
                                HAL_MouseEmu_PosY = 0; 
                          }
                             
                          if (MStepY>0)
                          {
                             Theor_MousePosY=HAL_MouseEmu_PosY + MStepY;
                             
                             if (Theor_MousePosY<=Mouse_MaxScreenY) 
                                HAL_MouseEmu_PosY = Theor_MousePosY;
                             else
                                HAL_MouseEmu_PosY = Mouse_MaxScreenY; 
                          }   
                             
                          if (MStepX<0)
                          {
                             Theor_MousePosX=HAL_MouseEmu_PosX + MStepX;
                             
                             if (Theor_MousePosX>=0) 
                                HAL_MouseEmu_PosX = Theor_MousePosX;
                             else
                                HAL_MouseEmu_PosX = 0;
                          }  
                          
                          if (MStepX>0)
                          {
                             Theor_MousePosX=HAL_MouseEmu_PosX + MStepX;
                             
                             if (Theor_MousePosX<=Mouse_MaxScreenX) 
                                HAL_MouseEmu_PosX = Theor_MousePosX;
                             else
                                HAL_MouseEmu_PosX = Mouse_MaxScreenX;     
                          }
                          
                          return;
                   }
                   
                   if (Pad.Buttons & PSP_CTRL_CROSS)
                   {
                          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON1;
                          HAL_MouseVectorX=2*MStepX;
                          HAL_MouseVectorY=2*MStepY;
                          
                          if ((HAL_MouseVectorX==0) && (HAL_MouseVectorY==0)) 
                             ndHAL_RequestDelay (ND_REQDELAY_MOUSECTRL);
                          
                          return;              
                   }
                   
                   if (Pad.Buttons & PSP_CTRL_CIRCLE)
                   {
                          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON4;
                          HAL_MouseVectorX=sgn (MStepX);
                          HAL_MouseVectorY=sgn (MStepY);
                   
                          return;              
                   }
                   
                   if (Pad.Buttons & PSP_CTRL_SELECT)
                   {
                          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON5;
                          HAL_MouseVectorX=0;
                          HAL_MouseVectorY=0;
                          
                          return;
                   }
                   
               }
               else   // Non è stato premuto alcun tasto
               {
                      HAL_MouseButtons=0;
                      HAL_MouseVectorX=0;
                      HAL_MouseVectorY=0;  
                      
                      VMouseAcceleration=0;            // Accelerazione vmouse reinizializzata
                         
                      return;
               }    
               
   }
}


void ndHAL_MouseControlledByAnalog (char IsOn)
{
   HAL_MouseIsControlledByAnalog = IsOn;   
}



void PhoenixMouse_SERVER (SceSize args, void *argp)
{
   static struct ndMouseState_Type MouseState;
   
   while (!phnMouse_SystemStop)
   {
         ndHAL_GetMouseState (&MouseState);         
         // Ricava le informazioni sullo stato del mouse. Provvede anche a richiamare
         // ndHAL_MouseEmu internamente
         
         INTERNAL_PHOENIX_MouseControl_SERVER (&MouseState);
         // Richiama il componente server MouseControl: viene passata una struttura 
         // contenente lo stato attuale del mouse. 
   
         sceDisplayWaitVblankStart ();
         // Necessario per consentire allo scheduler il cambio di task
   } 
}

void ndHAL_StartPhoenixMouseThread (void)
{
   phnMouse_SystemStop = 0;
   
   HAL_MouseEmu_PosX          = Windows_MaxScreenX/2;
   HAL_MouseEmu_PosY          = Windows_MaxScreenY/2;
   HAL_IsTheFirstTimeForMouse = 1;
   
   sceKernelSuspendThread (phnGraph_ThreadID);     // Sospendi il thread di gestione grafica per sicurezza
   
   // Crea il registro Eventi Ev0 per Phoenix Mouse
   
   phnMouse_Ev0_CbeRegister = sceKernelCreateEventFlag ("Ev0_PhoenixMouse", PSP_EVENT_WAITMULTIPLE, PHNMOUSE_CBEREG_IS_FREE, 0);  
   
   // Crea il Phoenix Mouse Thread

   phnMouse_ThreadID = sceKernelCreateThread ("PhoenixMouse", PhoenixMouse_SERVER, 34, 65535, THREAD_ATTR_USER | THREAD_ATTR_VFPU, 0);
   sceKernelStartThread (phnMouse_ThreadID, 0, 0); 
   
   HAL_MouseEmuSystemEnabled  = 1;
   HAL_MouseThreadStatus      = 2;                  // Segnala che il thread del mouse è stato avviato ed è attivo
   
   sceKernelResumeThread (phnGraph_ThreadID);       // Riprendi il thread di gestione grafica 
   ndHAL_HardDelay (0.1);                           // Un piccolo ritardo per consentire al sistema di stabilizzarsi 
}


void ndHAL_SuspendPhoenixMouseThread (void)
{
   sceKernelSuspendThread (phnGraph_ThreadID);     // Sospendi il thread di gestione grafica per sicurezza
   
   sceKernelSuspendThread (phnMouse_ThreadID);     // Sospende il thread del mouse  
   HAL_MouseThreadStatus      = 1;                 // Segnala che il thread del mouse è in stato di sospensione
   
   HAL_MouseEmuSystemEnabled  = 0;                 // Indurrà il prossimo WindowsRender richiamato dal Phoenix Thread a cancellare il puntatore        
   
   sceKernelResumeThread (phnGraph_ThreadID);      // Riavvia il thread di gestione grafica
   ndHAL_WindowsRender_CLIENT ();                  // Una chiamata al client indurrà il sistema a ridisegnare il
                                                   // frame cancellando la freccia del mouse
}


void ndHAL_ResumePhoenixMouseThread (void)
{
   HAL_MouseEmu_PosX          = Windows_MaxScreenX/2;
   HAL_MouseEmu_PosY          = Windows_MaxScreenY/2;
   HAL_IsTheFirstTimeForMouse = 1;
   
   sceKernelSuspendThread (phnGraph_ThreadID);     // Sospendi il thread di gestione grafica per sicurezza
   
   sceKernelResumeThread (phnMouse_ThreadID);      // Riavvia il thread relativo al mouse
   
   HAL_MouseEmuSystemEnabled  = 1;
   HAL_MouseThreadStatus      = 2;                 // Segnala che il thread del mouse è stato avviato ed è attivo
   
   sceKernelResumeThread (phnGraph_ThreadID);      // Riprendi il thread di gestione grafica   
   
   ndHAL_HardDelay (0.1);                          // Un piccolo ritardo per consentire al sistema di stabilizzarsi 
}
