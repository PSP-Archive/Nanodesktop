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
    VARIABILI E SIMBOLI GCURL
    -----------------------------------------------------------------------------
*/

#ifndef _GCURL_VARIABLES_
        #define _GCURL_VARIABLES_
        
/*
    -----------------------------------------------------------------------------
    VARIABILI E SIMBOLI GENERICI
    -----------------------------------------------------------------------------
*/        

        #define TRUE  1
        #define FALSE 0

/*
    -----------------------------------------------------------------------------
    GCURL JOB 1: TRANSFER FILE
    -----------------------------------------------------------------------------
*/        

        struct gCurl_FileTransfer_Options
        {
            char NoStdTitleColor;
            char NoStdMainColor;
            char NoStdSecondaryColor;
            char NoStdBarColor;
            char NoStdSecondaryBarColor;
            char NoStdTextColor;
            char NoStdTextBarColor;
            
            TypeColor OptTitleColor;
            TypeColor OptMainColor;
            TypeColor OptSecondaryColor;
            TypeColor OptBarColor;
            TypeColor OptSecondaryBarColor;
            TypeColor OptTextColor;
            TypeColor OptTextBarColor; 
            
            char NoStdPosition;
            unsigned short int OptPosX1, OptPosY1;
            
            char NoAutoCloseDBoxWhenFinish; 
            char BeVerbose; 
            char DoNotUseProxyAutoConfig;
            char HttpTunneling;
            
            char ManualProxy_Enabled;
            char ManualProxy_Address [32];
            char ManualProxy_Port    [8]; 
            
            char ManualProxy_User    [128];
            char ManualProxy_Pass    [128];
        };
        
        
        struct gCurl_FileTransfer_cbContainer
        {
            char  *FileName;
            char  *FileDest;
            
            FILE   *Stream;   
            char   WndHandle;
            char   BtnAbort, BtnOK;
            
            unsigned short int PosX1, PosY1; 
            unsigned short int PosX2, PosY2;
            
            TypeColor SetTitleColor;
            TypeColor SetMainColor;
            TypeColor SetSecondaryColor;
            TypeColor SetBarColor;
            TypeColor SetSecondaryBarColor;
            TypeColor SetTextColor;
            TypeColor SetTextBarColor;
            
            char   oldBytesRegistered;
            float  oldBytes;
            float  oldTime;
            
            int   SemaphoreForExit;
            int   AbortRequested;
        };



        
#endif
