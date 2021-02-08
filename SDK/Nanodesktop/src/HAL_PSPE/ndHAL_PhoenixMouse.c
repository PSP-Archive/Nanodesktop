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
    ROUTINE PHOENIX MOUSE THREAD (PSPE)
    ----------------------------------------------------------------------------
*/

#include <HAL_PSPE\$_HAL_NDforND.h>

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
   
   char MStep;
   
   ndHAL_GetPad (&Pad);
   
   if (Pad.Buttons != 0)
   {
       HAL_MouseButtons=0;         
                   
       if ( (!(Pad.Buttons & PSP_CTRL_CROSS)) && (!(Pad.Buttons & PSP_CTRL_CIRCLE)) && (!(Pad.Buttons & PSP_CTRL_SELECT)) )           
       {
          HAL_MouseVectorX=0;
          HAL_MouseVectorY=0;
          
          VMouseAcceleration=VMouseAcceleration+1;
          
          if (VMouseAcceleration>5) VMouseAcceleration=5;
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
          
          ndHAL_HardDelay (__MOUSE_DELAY);
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
                ndHAL_HardDelay (__MOUSE_DELAY);
          
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
          
          ndHAL_HardDelay (__MOUSE_DELAY);
          return;              
       }
       
       if (Pad.Buttons & PSP_CTRL_SELECT)
       {
          HAL_MouseButtons=HAL_MouseButtons | ND_BUTTON5;
          HAL_MouseVectorX=0;
          HAL_MouseVectorY=0;
          
          ndHAL_HardDelay (__MOUSE_DELAY);
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

void ndHAL_MouseControlledByAnalog (char IsOn)
{
    // Niente da fare. Non supportato sotto PSPE 
}


void ndHAL_StartPhoenixMouseThread (void)
{
     /*
         Sotto PSPE non viene usato il Phoenix Mouse Thread perchè l'emulatore
         PSPE non sembra supportare correttamente i threads.
     */
}

void ndHAL_SuspendPhoenixMouseThread (void)
{
     /*
         Sotto PSPE non viene usato il Phoenix Mouse Thread perchè l'emulatore
         PSPE non sembra supportare correttamente i threads.
     */
}

void ndHAL_ResumePhoenixMouseThread (void)
{
     /*
         Sotto PSPE non viene usato il Phoenix Mouse Thread perchè l'emulatore
         PSPE non sembra supportare correttamente i threads.
     */
}


