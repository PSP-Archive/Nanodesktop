/*  
    ---------------------------------------------------------------------------------------------------
    EMI - Enhanced Mathematical Interface 
    Copyright 2007-2012 Filippo Battaglia 
    ---------------------------------------------------------------------------------------------------
    Version for real PSP 
    ---------------------------------------------------------------------------------------------------
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
    ------------------------------------------------------------------------------------------------------
*/


#include <nanodesktop.h>

  
/*
    ***************************************************************************
    ***************************************************************************
    MACRO PER GENERAZIONE VARIANTI
    ***************************************************************************
    ***************************************************************************
*/


// Questa macro genera le 6 varianti di qualsiasi routine matematica, una per matrice

#define MACROGEN1d(_mpTypeFunc_, _NameProc_, _mpTag_)    \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6);        \

#define MACROGEN2d(_mpTypeFunc_, _NameProc_, _mpTag_)    \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6);        \

 
 
 
#define MACROGEN3d(_mpTypeFunc_, _NameProc_, _mpTag_)       \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 0, 6, 6);        \
                                                            \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 1, 6, 6);        \
                                                            \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 2, 6, 6);        \
                                                            \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 3, 6, 6);        \
                                                            \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 4, 6, 6);        \
                                                            \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 5, 6, 6);        \
                                                            \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 0, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 1, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 2, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 3, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 4, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 5, 6);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 0);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 1);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 2);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 3);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 4);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 5);        \
GEN___##_NameProc_ (_mpTypeFunc_, _mpTag_, 6, 6, 6);        \

/*
    ***************************************************************************
    ***************************************************************************
    ROUTINES MATRICIALI A BASSO LIVELLO
    ***************************************************************************
    ***************************************************************************
*/

 
/*
    ---------------------------------------------------------------------------
    Load16FloatsToMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a caricare 16 elementi di tipo float
    nella matrice NrMatrix. I 16 floats devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/ 
 
#define GEN___Load16FloatsToMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Load16FloatsToMatrix_m##_mpNrMatrix_ (float *Data) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "and    $5, %1, 0x0F\n" \
       "bne    $5, 0, _LOAD16FLOATSTOMATRIX_NOALLIGN_"#_mpNrMatrix_" \n" \
       \
       "lv.q   C"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a caricare usando il metodo veloce */    \
       "lv.q   C"#_mpNrMatrix_"10, 16 + %0\n"     \
       "lv.q   C"#_mpNrMatrix_"20, 32 + %0\n"     \
       "lv.q   C"#_mpNrMatrix_"30, 48 + %0\n"     \
       "b      _LOAD16FLOATSTOMATRIX_ENDFUNC_"#_mpNrMatrix_" \n" \
                                                  \
       "_LOAD16FLOATSTOMATRIX_NOALLIGN_"#_mpNrMatrix_": \n" \
                                                  \
       "lv.s   S"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a caricare usando il metodo più lento */    \
       "lv.s   S"#_mpNrMatrix_"01,  4 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"02,  8 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"03, 12 + %0\n"     \
                               \
       "lv.s   S"#_mpNrMatrix_"10, 16 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"11, 20 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"12, 24 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"13, 28 + %0\n"     \
                               \
       "lv.s   S"#_mpNrMatrix_"20, 32 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"21, 36 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"22, 40 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"23, 44 + %0\n"     \
                               \
       "lv.s   S"#_mpNrMatrix_"30, 48 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"31, 52 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"32, 56 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"33, 60 + %0\n"     \
                                                  \
       "_LOAD16FLOATSTOMATRIX_ENDFUNC_"#_mpNrMatrix_": \n" \
                                                      \
       :  : "m"(*Data), "r"(Data) : "$5");           \
                               \
       return;                 \
}

// End macro


MACROGEN1d(, Load16FloatsToMatrix, ndEMI_)



/*
   ---------------------------------------------------------------------------
   Load16IntsToMatrix
   ---------------------------------------------------------------------------
   Questa routine provvede a caricare 16 elementi di tipo int
   nella matrice NrMatrix. I 16 ints devono essere memorizzati
   a partire dall'indirizzo Data
   ---------------------------------------------------------------------------
*/ 
 
#define GEN___Load16IntsToMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Load16IntsToMatrix_m##_mpNrMatrix_ (int *Data) \
    {                                                                             \
       __asm__ volatile (                                                            \
                                                                                     \
       "and    $5, %1, 0x0F\n" \
       "bne    $5, 0, _LOAD16INTSTOMATRIX_NOALLIGN_"#_mpNrMatrix_" \n" \
       \
       "lv.q   C"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a caricare usando il metodo veloce */    \
       "lv.q   C"#_mpNrMatrix_"10, 16 + %0\n"     \
       "lv.q   C"#_mpNrMatrix_"20, 32 + %0\n"     \
       "lv.q   C"#_mpNrMatrix_"30, 48 + %0\n"     \
       "b      _LOAD16INTSTOMATRIX_ENDFUNC_"#_mpNrMatrix_" \n" \
                                                  \
       "_LOAD16INTSTOMATRIX_NOALLIGN_"#_mpNrMatrix_": \n" \
                                                  \
       "lv.s   S"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a caricare usando il metodo più lento */    \
       "lv.s   S"#_mpNrMatrix_"01,  4 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"02,  8 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"03, 12 + %0\n"     \
                               \
       "lv.s   S"#_mpNrMatrix_"10, 16 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"11, 20 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"12, 24 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"13, 28 + %0\n"     \
                               \
       "lv.s   S"#_mpNrMatrix_"20, 32 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"21, 36 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"22, 40 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"23, 44 + %0\n"     \
                               \
       "lv.s   S"#_mpNrMatrix_"30, 48 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"31, 52 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"32, 56 + %0\n"     \
       "lv.s   S"#_mpNrMatrix_"33, 60 + %0\n"     \
                                                  \
       "_LOAD16INTSTOMATRIX_ENDFUNC_"#_mpNrMatrix_": \n" \
                                                  \
       "vi2f.q C"#_mpNrMatrix_"00, C"#_mpNrMatrix_"00, 0\n" /* Esegue la conversione del contenuto del registro in float */\
       "vi2f.q C"#_mpNrMatrix_"10, C"#_mpNrMatrix_"10, 0\n" \
       "vi2f.q C"#_mpNrMatrix_"20, C"#_mpNrMatrix_"20, 0\n" \
       "vi2f.q C"#_mpNrMatrix_"30, C"#_mpNrMatrix_"30, 0\n" \
                                                  \
       :  : "m"(*Data), "r"(Data) : "$5");                          \
       return;                 \
}

// End macro


MACROGEN1d(, Load16IntsToMatrix, ndEMI_)


/*
   ---------------------------------------------------------------------------
   Load16UCharsToMatrix
   ---------------------------------------------------------------------------
   Questa routine provvede a caricare 16 elementi di tipo unsigned char
   nella matrice NrMatrix. I 16 uchars devono essere memorizzati
   a partire dall'indirizzo Data
   ---------------------------------------------------------------------------
*/ 
 

#define GEN___Load16UCharsToMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)               \
                                                                                    \
                                                                                    \
    _mpTypeFunc_ void _mpTag_##Load16UCharsToMatrix_m##_mpNrMatrix_ (unsigned char *Data)     \
    {                                                                               \
       __asm__ volatile (                                                           \
                                                                                    \
             "lw     $5,   %0       \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"00     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"01     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"02     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"03     \n"                               \
                                                                                    \
             "lw     $5,   4 + %0   \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"10     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"11     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"12     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"13     \n"                               \
                                                                                    \
             "lw     $5,   8 + %0   \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"20     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"21     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"22     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"23     \n"                               \
                                                                                    \
             "lw     $5,   12 + %0   \n"                                            \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"30     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"31     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"32     \n"                               \
             "srl    $5,   8        \n"                                             \
             "and    $6,   $5, 0xFF \n"                                             \
             "mtv    $6,   S"#_mpNrMatrix_"33     \n"                               \
                                                                                    \
             "vi2f.q C"#_mpNrMatrix_"00, C"#_mpNrMatrix_"00, 0\n" /* Esegue la conversione del contenuto del registro in float */\
             "vi2f.q C"#_mpNrMatrix_"10, C"#_mpNrMatrix_"10, 0\n" \
             "vi2f.q C"#_mpNrMatrix_"20, C"#_mpNrMatrix_"20, 0\n" \
             "vi2f.q C"#_mpNrMatrix_"30, C"#_mpNrMatrix_"30, 0\n" \
                                                                \
                                                            \
       :  : "m"(*Data), "r"(Data) : "$5");                  \
       return;                 \
}

// End macro







MACROGEN1d(, Load16UCharsToMatrix, ndEMI_)




/*
    ---------------------------------------------------------------------------
    Store16FloatsFromMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo float
    dalla matrice NrMatrix. I 16 floats devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/ 


#define GEN___Store16FloatsFromMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Store16FloatsFromMatrix_m##_mpNrMatrix_ (float *Data) \
    {                                                                             \
    __asm__ volatile (                                                            \
                                                                                  \
       "and    $5, %1, 0x0F\n" \
       "bne    $5, 0, _STORE16FLOATSFROMMATRIX_NOALLIGN_"#_mpNrMatrix_" \n" \
       \
       "sv.q   C"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a caricare usando il metodo veloce */    \
       "sv.q   C"#_mpNrMatrix_"10, 16 + %0\n"     \
       "sv.q   C"#_mpNrMatrix_"20, 32 + %0\n"     \
       "sv.q   C"#_mpNrMatrix_"30, 48 + %0\n"     \
       "b      _STORE16FLOATSFROMMATRIX_ENDFUNC_"#_mpNrMatrix_" \n" \
                                                  \
       "_STORE16FLOATSFROMMATRIX_NOALLIGN_"#_mpNrMatrix_": \n" \
                                                        \
       "sv.s   S"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a salvare i dati */    \
       "sv.s   S"#_mpNrMatrix_"01,  4 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"02,  8 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"03, 12 + %0\n"     \
                               \
       "sv.s   S"#_mpNrMatrix_"10, 16 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"11, 20 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"12, 24 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"13, 28 + %0\n"     \
                               \
       "sv.s   S"#_mpNrMatrix_"20, 32 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"21, 36 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"22, 40 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"23, 44 + %0\n"     \
                               \
       "sv.s   S"#_mpNrMatrix_"30, 48 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"31, 52 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"32, 56 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"33, 60 + %0\n"     \
                                                  \
       "_STORE16FLOATSFROMMATRIX_ENDFUNC_"#_mpNrMatrix_": \n" \
                                                     \
       :  : "m"(*Data), "r"(Data) : "$5");         \
                               \
       return;                 \
}

// End macro


MACROGEN1d(, Store16FloatsFromMatrix, ndEMI_)



/*
    ---------------------------------------------------------------------------
    Store16IntsFromMatrixDestr
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo int
    dalla matrice NrMatrix. I 16 ints devono essere memorizzati
    a partire dall'indirizzo Data.
    
    La routine è distruttiva sotto PSP. In altre parole, dopo che essa
    viene usata i dati nella matrice hardware della VFPU perdono 
    significato (in realtà sono convertiti in int), e l'utente può
    solo riempire la matrice con nuovi dati
    ---------------------------------------------------------------------------
*/ 


#define GEN___Store16IntsFromMatrixDestr(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Store16IntsFromMatrixDestr_m##_mpNrMatrix_ (int *Data) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vf2iz.q C"#_mpNrMatrix_"00, C"#_mpNrMatrix_"00, 0\n" /* Esegue la conversione del contenuto del registro in */ \
       "vf2iz.q C"#_mpNrMatrix_"10, C"#_mpNrMatrix_"10, 0\n" /* integer: questo distrugge il contenuto della matrice */ \
       "vf2iz.q C"#_mpNrMatrix_"20, C"#_mpNrMatrix_"20, 0\n" /* VFPU. */ \
       "vf2iz.q C"#_mpNrMatrix_"30, C"#_mpNrMatrix_"30, 0\n" \
                                                             \
       "and    $5, %1, 0x0F\n"                               \
       "bne    $5, 0, _STORE16INTSFROMMATRIXDESTR_NOALLIGN_"#_mpNrMatrix_" \n" \
       \
       "sv.q   C"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a caricare usando il metodo veloce */    \
       "sv.q   C"#_mpNrMatrix_"10, 16 + %0\n"     \
       "sv.q   C"#_mpNrMatrix_"20, 32 + %0\n"     \
       "sv.q   C"#_mpNrMatrix_"30, 48 + %0\n"     \
       "b      _STORE16INTSFROMMATRIXDESTR_ENDFUNC_"#_mpNrMatrix_" \n" \
                                                  \
       "_STORE16INTSFROMMATRIXDESTR_NOALLIGN_"#_mpNrMatrix_": \n" \
                                                        \
       "sv.s   S"#_mpNrMatrix_"00,  0 + %0\n"     /* Provvedi a salvare i dati */    \
       "sv.s   S"#_mpNrMatrix_"01,  4 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"02,  8 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"03, 12 + %0\n"     \
                               \
       "sv.s   S"#_mpNrMatrix_"10, 16 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"11, 20 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"12, 24 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"13, 28 + %0\n"     \
                               \
       "sv.s   S"#_mpNrMatrix_"20, 32 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"21, 36 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"22, 40 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"23, 44 + %0\n"     \
                               \
       "sv.s   S"#_mpNrMatrix_"30, 48 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"31, 52 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"32, 56 + %0\n"     \
       "sv.s   S"#_mpNrMatrix_"33, 60 + %0\n"     \
                                                  \
       "_STORE16INTSFROMMATRIXDESTR_ENDFUNC_"#_mpNrMatrix_": \n" \
                                                   \
       :  : "m"(*Data), "r"(Data) : "$5");         \
                                                   \
       return;                                     \
}

// End macro


MACROGEN1d(, Store16IntsFromMatrixDestr, ndEMI_)






/*
    ---------------------------------------------------------------------------
    Store16IntsFromMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo int
    dalla matrice NrMatrix. I 16 ints devono essere memorizzati
    a partire dall'indirizzo Data
    
    A differenza della precedente, questa routine non è distruttiva
    ---------------------------------------------------------------------------
*/ 


#define GEN___Store16IntsFromMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Store16IntsFromMatrix_m##_mpNrMatrix_ (int *Data) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vf2iz.q C700, C"#_mpNrMatrix_"00, 0\n" /* Esegue la conversione del contenuto del registro in */ \
       "vf2iz.q C710, C"#_mpNrMatrix_"10, 0\n" /* integer: per preservare i dati, i valori convertiti */ \
       "vf2iz.q C720, C"#_mpNrMatrix_"20, 0\n" /* sono prima trasferiti nella matrice di appoggio 7 */ \
       "vf2iz.q C730, C"#_mpNrMatrix_"30, 0\n" \
                                               \
       "and    $5, %1, 0x0F\n"                               \
       "bne    $5, 0, _STORE16INTSFROMMATRIX_NOALLIGN_"#_mpNrMatrix_" \n" \
       \
       "sv.q   C700,  0 + %0\n"     /* Provvedi a caricare usando il metodo veloce */    \
       "sv.q   C710, 16 + %0\n"     \
       "sv.q   C720, 32 + %0\n"     \
       "sv.q   C730, 48 + %0\n"     \
       "b      _STORE16INTSFROMMATRIX_ENDFUNC_"#_mpNrMatrix_" \n" \
                                                  \
       "_STORE16INTSFROMMATRIX_NOALLIGN_"#_mpNrMatrix_": \n" \
                                                        \
       "sv.s   S700,  0 + %0\n"     /* Provvedi a salvare i dati */    \
       "sv.s   S701,  4 + %0\n"     \
       "sv.s   S702,  8 + %0\n"     \
       "sv.s   S703, 12 + %0\n"     \
                               \
       "sv.s   S710, 16 + %0\n"     \
       "sv.s   S711, 20 + %0\n"     \
       "sv.s   S712, 24 + %0\n"     \
       "sv.s   S713, 28 + %0\n"     \
                               \
       "sv.s   S720, 32 + %0\n"     \
       "sv.s   S721, 36 + %0\n"     \
       "sv.s   S722, 40 + %0\n"     \
       "sv.s   S723, 44 + %0\n"     \
                               \
       "sv.s   S730, 48 + %0\n"     \
       "sv.s   S731, 52 + %0\n"     \
       "sv.s   S732, 56 + %0\n"     \
       "sv.s   S733, 60 + %0\n"     \
                                                  \
       "_STORE16INTSFROMMATRIX_ENDFUNC_"#_mpNrMatrix_": \n" \
                                                   \
       :  : "m"(*Data), "r"(Data) : "$5");         \
                                                   \
       return;                                     \
}

// End macro


MACROGEN1d(, Store16IntsFromMatrix, ndEMI_)


/*
    ---------------------------------------------------------------------------
    Store16UCharsFromMatrixDestr
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo uchar
    dalla matrice NrMatrix. I 16 unsigned chars devono essere memorizzati
    a partire dall'indirizzo Data
    
    La routine è distruttiva sotto PSP. In altre parole, dopo che essa
    viene usata i dati nella matrice hardware della VFPU perdono 
    significato (in realtà sono convertiti in int), e l'utente può
    solo riempire la matrice con nuovi dati
    ---------------------------------------------------------------------------
*/ 
 
 
 
#define GEN___Store16UCharsFromMatrixDestr(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Store16UCharsFromMatrixDestr_m##_mpNrMatrix_ (unsigned char *Data) \
    {                                                                         \
        __asm__ volatile (                                                            \
                                                                                      \
       "vf2iz.q C"#_mpNrMatrix_"00, C"#_mpNrMatrix_"00, 0\n" /* Esegue la conversione del contenuto del registro in float */\
       "vf2iz.q C"#_mpNrMatrix_"10, C"#_mpNrMatrix_"10, 0\n" \
       "vf2iz.q C"#_mpNrMatrix_"20, C"#_mpNrMatrix_"20, 0\n" \
       "vf2iz.q C"#_mpNrMatrix_"30, C"#_mpNrMatrix_"30, 0\n" \
                                                                                       \
       "li     $5, 0 \n"                                                                  \
       "mfv    $6, S"#_mpNrMatrix_"00\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"01\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"02\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"03\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 0 + %0 \n"                                                          \
                                                                                       \
       "li     $5, 0 \n"                                                                  \
       "mfv    $6, S"#_mpNrMatrix_"10\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"11\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"12\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"13\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 4 + %0 \n"                                                          \
                                                                                       \
       "li     $5, 0 \n"                                                                  \
       "mfv    $6, S"#_mpNrMatrix_"20\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"21\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"22\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"23\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 8 + %0 \n"                                                          \
                                                                                       \
       "li     $5, 0 \n"                                                                  \
       "mfv    $6, S"#_mpNrMatrix_"30\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"31\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"32\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S"#_mpNrMatrix_"33\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 12 + %0 \n"                                                         \
                                                                                    \
       :  : "m"(*Data), "r"(Data) : "$5", "$6");                  \
                                                   \
       return;                                     \
}
             
       

// End macro


MACROGEN1d(, Store16UCharsFromMatrixDestr, ndEMI_)


/*
    ---------------------------------------------------------------------------
    Store16UCharsFromMatrix
    ---------------------------------------------------------------------------
    Questa routine provvede a memorizzare 16 elementi di tipo uchar
    dalla matrice NrMatrix. I 16 unsigned chars devono essere memorizzati
    a partire dall'indirizzo Data
    ---------------------------------------------------------------------------
*/ 
 
 
 
 
#define GEN___Store16UCharsFromMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##Store16UCharsFromMatrix_m##_mpNrMatrix_ (unsigned char *Data) \
    {                                                                         \
        __asm__ volatile (                                                            \
                                                                                      \
       "vf2iz.q C700, C700, 0\n" /* Esegue la conversione del contenuto del registro in float */\
       "vf2iz.q C710, C710, 0\n" \
       "vf2iz.q C720, C720, 0\n" \
       "vf2iz.q C730, C730, 0\n" \
                                                                                       \
       "li     $5, 0 \n"                                                               \
       "mfv    $6, S700\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S701\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S702\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S703\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 0 + %0 \n"                                                          \
                                                                                       \
       "li     $5, 0 \n"                                                               \
       "mfv    $6, S710\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S711\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S712\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S713\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 4 + %0 \n"                                                          \
                                                                                       \
       "li     $5, 0 \n"                                                                  \
       "mfv    $6, S720\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S721\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S722\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S723\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 8 + %0 \n"                                                          \
                                                                                       \
       "li     $5, 0 \n"                                                                  \
       "mfv    $6, S730\n"                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S731\n"                                               \
       "sll    $6, 8 \n"                                                               \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S732\n"                                               \
       "sll    $6, 16 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "mfv    $6, S733\n"                                               \
       "sll    $6, 24 \n"                                                              \
       "or     $5, $5, $6 \n"                                                          \
       "sw     $5, 12 + %0 \n"                                                         \
                                                                                    \
       :  : "m"(*Data), "r"(Data) : "$5");                  \
                                                   \
       return;                                     \
}


// End macro


MACROGEN1d(, Store16UCharsFromMatrix, ndEMI_)



/*
   ---------------------------------------------------------------------------
   NullMatrix
   ---------------------------------------------------------------------------
   Riempie di zeri la matrice 
     
            matrix0 [i;j] = 0   
   ---------------------------------------------------------------------------
*/ 


#define GEN___NullMatrix(_mpTypeFunc_,_mpTag_,_mpNrMatrix_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##NullMatrix_m##_mpNrMatrix_ (void) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vmzero.q M"#_mpNrMatrix_"00 \n"                                             \
       :  : );         \
                               \
       return;                 \
}

// End macro


MACROGEN1d(, NullMatrix, ndEMI_)


/*
   ---------------------------------------------------------------------------
   MatrixSum
   ---------------------------------------------------------------------------
   Esegue la somma di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] + matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


#define GEN___MatrixSum(_mpTypeFunc_,_mpTag_,_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##MatrixSum_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ (void) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vadd.q C"#_mpNrMatrixDest_"00, C"#_mpNrMatrix0_"00, C"#_mpNrMatrix1_"00 \n"     \
       "vadd.q C"#_mpNrMatrixDest_"10, C"#_mpNrMatrix0_"10, C"#_mpNrMatrix1_"10 \n"     \
       "vadd.q C"#_mpNrMatrixDest_"20, C"#_mpNrMatrix0_"20, C"#_mpNrMatrix1_"20 \n"     \
       "vadd.q C"#_mpNrMatrixDest_"30, C"#_mpNrMatrix0_"30, C"#_mpNrMatrix1_"30 \n"     \
       :  : );         \
                               \
       return;                 \
}

// End macro


MACROGEN3d(, MatrixSum, ndEMI_)
// Genera le varianti della routine



/*
   ---------------------------------------------------------------------------
   MatrixSub
   ---------------------------------------------------------------------------
   Esegue la somma di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] - matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 

#define GEN___MatrixSub(_mpTypeFunc_,_mpTag_,_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##MatrixSub_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ (void) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vsub.q C"#_mpNrMatrixDest_"00, C"#_mpNrMatrix0_"00, C"#_mpNrMatrix1_"00 \n"     \
       "vsub.q C"#_mpNrMatrixDest_"10, C"#_mpNrMatrix0_"10, C"#_mpNrMatrix1_"10 \n"     \
       "vsub.q C"#_mpNrMatrixDest_"20, C"#_mpNrMatrix0_"20, C"#_mpNrMatrix1_"20 \n"     \
       "vsub.q C"#_mpNrMatrixDest_"30, C"#_mpNrMatrix0_"30, C"#_mpNrMatrix1_"30 \n"     \
       :  : );         \
                               \
       return;                 \
}

// End macro


MACROGEN3d(, MatrixSub, ndEMI_)
// Genera le varianti della routine



/*
   ---------------------------------------------------------------------------
    MatrixTrvMul
   ---------------------------------------------------------------------------
   Esegue lo pseudoprodotto termine a termine di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] * matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


#define GEN___MatrixTrvMul(_mpTypeFunc_,_mpTag_,_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##MatrixTrvMul_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ (void) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vmul.q C"#_mpNrMatrixDest_"00, C"#_mpNrMatrix0_"00, C"#_mpNrMatrix1_"00 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"10, C"#_mpNrMatrix0_"10, C"#_mpNrMatrix1_"10 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"20, C"#_mpNrMatrix0_"20, C"#_mpNrMatrix1_"20 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"30, C"#_mpNrMatrix0_"30, C"#_mpNrMatrix1_"30 \n"     \
       :  : );         \
                               \
       return;                 \
}

// End macro


MACROGEN3d(, MatrixTrvMul, ndEMI_)


/*
   ---------------------------------------------------------------------------
   MatrixTrvDiv
   ---------------------------------------------------------------------------
   Esegue la pseudodivisione di due matrici secondo la formula
     
            MatrixDest [i;j] = matrix0 [i;j] / matrix1 [i;j]   
   ---------------------------------------------------------------------------
*/ 


#define GEN___MatrixTrvDiv(_mpTypeFunc_,_mpTag_,_mpNrMatrix0_,_mpNrMatrix1_,_mpNrMatrixDest_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##MatrixTrvDiv_m##_mpNrMatrix0_##_m##_mpNrMatrix1_##_to_m##_mpNrMatrixDest_ (void) \
    {                                                                             \
    __asm__ volatile (                                                            \
       "vrcp.q C700, C"#_mpNrMatrix1_"00 \n"     \
       "vrcp.q C710, C"#_mpNrMatrix1_"10 \n"     \
       "vrcp.q C720, C"#_mpNrMatrix1_"20 \n"     \
       "vrcp.q C730, C"#_mpNrMatrix1_"30 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"00, C"#_mpNrMatrix0_"00, C700 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"10, C"#_mpNrMatrix0_"10, C710 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"20, C"#_mpNrMatrix0_"20, C720 \n"     \
       "vmul.q C"#_mpNrMatrixDest_"30, C"#_mpNrMatrix0_"30, C730 \n"     \
       :  : );         \
                               \
       return;                 \
}

// End macro


MACROGEN3d(, MatrixTrvDiv, ndEMI_)




/*
   ---------------------------------------------------------------------------
    MatrixCpy
   ---------------------------------------------------------------------------
    Copia due matrici:
          
          MatrixDest [i;j] = MatrixSrc [i;j]
   ---------------------------------------------------------------------------
*/ 


#define GEN___MatrixCpy(_mpTypeFunc_, _mpTag_, _mpMatrixSrc_, _mpMatrixDest_)                \
  /* Matrix0 è la matrice destinazione */                                                   \
                                                                                  \
_mpTypeFunc_ void _mpTag_##MatrixCpy_m##_mpMatrixSrc_##_to_m##_mpMatrixDest_ (void)   \
{                                                                                 \
    __asm__ volatile (                                                            \
       "vmmov.q M"#_mpMatrixDest_"00, M"#_mpMatrixSrc_"00 \n"     \
       :  : );         \
                               \
       return;                 \
                                                                             \
}                                                                                 

MACROGEN2d (, MatrixCpy, ndEMI_)
// Genera le varianti della routine



/*
   ---------------------------------------------------------------------------
   SequentialAdder 
   ---------------------------------------------------------------------------
    Questa routine è un sommatore sequenziale. E' in grado di eseguire somme di 16 
    elementi per volta. 
    
    Supponiamo che la matrice si possa vedere come un vettore di 4x4=16 elementi. 
    
    Per il primo elemento della matrice, il sistema preleva il float puntato da
    CarryIn e lo porta nel registro [0;0] della matrice NrMatrix0. 
    
    Dopo, esegue la somma del corrispondente elemento della matrice NrMatrix1 in 
    posizione [0;0] e memorizza il risultato nella matrice NrMatrix0 [0;0]. 
    
    In altre parole, per il primo elemento della matrice viene eseguita la
    formula
    
           Matrix0 [n] = *CarryIn + Matrix1 [n];
           
    Per gli altri elementi della matrice viene applicata invece la formula
    
           Matrix0 [n] = Matrix0 [n-1] + Matrix1 [n];
           
    L'ultimo elemento della Matrix0 viene memorizzato nell'indirizzo puntato da
    CarryOut 
   ---------------------------------------------------------------------------
*/ 


#define GEN___SequentialAdder(_mpTypeFunc_,_mpTag_, _mpNrMatrix0_, _mpNrMatrix1_)           \
                                                                                  \
                                                                                  \
    _mpTypeFunc_ void _mpTag_##SequentialAdder_m##_mpNrMatrix0_##_m##_mpNrMatrix1_ (float *CarryIn, float *CarryOut) \
    {                                                                             \
       __asm__ volatile (                                                            \
       "lv.s   S700, %0\n"   /* Carica nel registro di servizio il CarryIn */ \
       "vadd.s S"#_mpNrMatrix0_"00, S"#_mpNrMatrix1_"00, S700  \n"   \
       "vadd.s S"#_mpNrMatrix0_"01, S"#_mpNrMatrix1_"01, S"#_mpNrMatrix0_"00  \n"   \
       "vadd.s S"#_mpNrMatrix0_"02, S"#_mpNrMatrix1_"02, S"#_mpNrMatrix0_"01  \n"   \
       "vadd.s S"#_mpNrMatrix0_"03, S"#_mpNrMatrix1_"03, S"#_mpNrMatrix0_"02  \n"   \
                                      \
       "vadd.s S"#_mpNrMatrix0_"10, S"#_mpNrMatrix1_"10, S"#_mpNrMatrix0_"03  \n"   \
       "vadd.s S"#_mpNrMatrix0_"11, S"#_mpNrMatrix1_"11, S"#_mpNrMatrix0_"10  \n"   \
       "vadd.s S"#_mpNrMatrix0_"12, S"#_mpNrMatrix1_"12, S"#_mpNrMatrix0_"11  \n"   \
       "vadd.s S"#_mpNrMatrix0_"13, S"#_mpNrMatrix1_"13, S"#_mpNrMatrix0_"12  \n"   \
                                      \
       "vadd.s S"#_mpNrMatrix0_"20, S"#_mpNrMatrix1_"20, S"#_mpNrMatrix0_"13  \n"   \
       "vadd.s S"#_mpNrMatrix0_"21, S"#_mpNrMatrix1_"21, S"#_mpNrMatrix0_"20  \n"   \
       "vadd.s S"#_mpNrMatrix0_"22, S"#_mpNrMatrix1_"22, S"#_mpNrMatrix0_"21  \n"   \
       "vadd.s S"#_mpNrMatrix0_"23, S"#_mpNrMatrix1_"23, S"#_mpNrMatrix0_"22  \n"   \
                                      \
       "vadd.s S"#_mpNrMatrix0_"30, S"#_mpNrMatrix1_"30, S"#_mpNrMatrix0_"23  \n"   \
       "vadd.s S"#_mpNrMatrix0_"31, S"#_mpNrMatrix1_"31, S"#_mpNrMatrix0_"30  \n"   \
       "vadd.s S"#_mpNrMatrix0_"32, S"#_mpNrMatrix1_"32, S"#_mpNrMatrix0_"31  \n"   \
       "vadd.s S"#_mpNrMatrix0_"33, S"#_mpNrMatrix1_"33, S"#_mpNrMatrix0_"32  \n"   \
                                      \
       "sv.s   S"#_mpNrMatrix0_"33, %1\n" /* Salva nell'area di memoria richiesta il CarryOut */ \
                                              \
       :  : "m"(*CarryIn), "m"(*CarryOut));    \
       return;                 \
}

// End macro


MACROGEN2d (, SequentialAdder, ndEMI_)
// Genera le varianti della routine



















































































































































































































































































































































