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
    --------------------------------------------------------------------------------------
    FILE ndHGUI for ndHGUI
    --------------------------------------------------------------------------------------   
    Questo file speciale viene richiamato esclusivamente dai moduli HGUI durante la fase
    di compilazione della libreria HighGUI. Esso provvede a richiamare una serie di
    files .h necessari per la corretta compilazione della libreria.
    --------------------------------------------------------------------------------------
*/

#ifndef _NDHGUI_FOR_NDHGUI_
        #define _NDHGUI_FOR_NDHGUI_

        #ifdef __cplusplus
        extern "C" {
        #endif

                #include <nanodesktop.h>
                #include <cv.h>
                #include <cxcore.h>
                
                #include <$$_HGUI_Variables.h>              // Include le variabili: si osservi che se è
                                                            // stato definito il simbolo NDHGUI_DEFINE_ALL_VARIABLES
                                                            // le variabili saranno effettivamente definite, mentre
                                                            // se non lo è verranno inseriti semplicemente dei
                                                            // riferimenti extern
                
                
                #define NDHGUI_I_NEED_ALL_PROTOTYPES        // Verranno definiti i prototipi di tutte le funzioni
                       #include <$$_HGUI_Prototypes.h>      // comprese quelle interne
                #undef  NDHGUI_I_NEED_ALL_PROTOTYPES 

        #ifdef __cplusplus
        }
        #endif


#endif



