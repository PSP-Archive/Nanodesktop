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
     MISC ROUTINES
    ---------------------------------------------------------------------------   
*/

#include <$_NDHGUI_for_NDHGUI.h>




int cvWaitKey (int TimeOut)
/* ------------------------------------------------------------------------------
   Routine cvWaitKey                                          Pubblica
   ------------------------------------------------------------------------------
   Attende la pressione di un tasto
   ------------------------------------------------------------------------------
*/
{
   static struct ndDataControllerType ndPadRecord;
   static struct ndEvent_Type MyEvent;
   int  TimeOutCounter;   
         
   if (TimeOut==0)             // Timeout disattivato. Fa apparire il puntatore del mouse
   {
       while (1)
       {
          MouseControl (&MyEvent);
          if (MyEvent.__TypeEvent==SPECIAL_KEY_PRESSED) return 'A';
       }
   }
   else                        // Timeout attivato. Nessun puntatore del mouse. Attende l'ultimo tasto
   {   
       while (TimeOutCounter<TimeOut) 
       {
          ndHAL_GetPad (&ndPadRecord);
          if ((ndPadRecord.Buttons & ND_BUTTON5)==ND_BUTTON5) return 'A';
          
          TimeOutCounter++;
       }
       
       return 0;
   }    
       
}

   
   
   
   
   
   
   
   
   
   
   
   
   

