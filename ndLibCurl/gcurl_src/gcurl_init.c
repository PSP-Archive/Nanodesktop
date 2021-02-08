/* 
    ----------------------------------------------------------------------------
    Libreria ndGCURL - Versione 1.0
    Copyright 2007-2012 Filippo Battaglia 
    ----------------------------------------------------------------------------
    The ndGCURL (Graphical LibCurl for Nanodesktop) is a library that provides
    some graphical methods, called "jobs", for proceeding with file downloads and
    other curl operations on the network. The jobs can be made using a simple 
    programming interface: they manage also visual feedback and Nanodesktop
    dialog box. 
    
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
    ROUTINES DI INIZIALIZZAZIONE GCURL
    -----------------------------------------------------------------------------
*/


#include <$$GCurl_Internal.h>
// Include i simboli interni della libreria (richiamerà anche definizioni di
// simboli pubblici

void ndGCURL_GlobalInit (void)
{
   curl_global_init(CURL_GLOBAL_DEFAULT);
}

void ndGCURL_GlobalDeInit (void)
{
   curl_global_cleanup(); 
}  

