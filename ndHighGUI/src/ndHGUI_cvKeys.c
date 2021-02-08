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
     ROUTINES KEYS
    ---------------------------------------------------------------------------   
*/

#include <$_NDHGUI_for_NDHGUI.h>

ndint64 CVKEY_SETPOS (unsigned short int PosX, unsigned short int PosY)
/* 
   -------------------------------------------------------------------------------------------------
   Routine CVKEY_SETPOS                                    Pubblica
   -------------------------------------------------------------------------------------------------
   Questa routine crea la key necessaria per settare la posizione della finestra che verrà
   creata da cvNamedWindow.
   
   PosX e PosY devono avere un valore inferiore o uguale a 2047. Se questo non viene fatto, allora,
   la routine restituirà 0 e la posizione della finestra sarà settata dal Position Generator di
   Nanodesktop.
   
   Non sono previsti codici di errore per questa routine
   -------------------------------------------------------------------------------------------------
*/
{
    ndint64 Key, Temp2;
                                                                                                    
    if ((PosX<=2047) && (PosY<=2047))
    {
       Key = BIT63;
       // Segnala che si sta usando il force della posizione
       
       Temp2 = ((ndint64) (PosX)) << (63-11); 
       Key = Key | Temp2;
       
       Temp2 = ((ndint64) (PosY)) << (63-11-11); 
       Key = Key | Temp2;
       
       return Key;
    }
    else
       return 0;
}



ndint64 CVKEY_SETSIZE (unsigned short int LenX, unsigned short int LenY)
/* 
   -------------------------------------------------------------------------------------------------
   Routine CVKEY_SETSIZE                                   Pubblica
   -------------------------------------------------------------------------------------------------
   Questa routine crea la key necessaria per settare le dimensioni della finestra che verrà
   creata da cvNamedWindow.
   
   LenX e LenY devono avere un valore inferiore o uguale a 2047. Se questo non viene fatto, allora,
   la routine restituirà 0 e le dimensioni della finestra saranno settate al valore di default
   previste da HGUI_DefaultLenX e HGUI_DefaultLenY. 
   
   Non sono previsti codici di errore per questa routine
   -------------------------------------------------------------------------------------------------
*/
{
    ndint64 Key, Temp2;
                                                                                                    
    if ((LenX<=2047) && (LenY<=2047))
    {
       Key = BIT40;
       // Segnala che si sta usando il force delle dimensioni
       
       Temp2 = ((ndint64) (LenX)) << (40-11); 
       Key = Key | Temp2;
       
       Temp2 = ((ndint64) (LenY)) << (40-11-11); 
       Key = Key | Temp2;
       
       return Key;
    }
    else
       return 0;
}

