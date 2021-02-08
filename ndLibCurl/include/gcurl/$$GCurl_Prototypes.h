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
    PROTOTIPI GCURL
    -----------------------------------------------------------------------------
*/

#ifndef _GCURL_PROTOTYPES_
        #define _GCURL_PROTOTYPES_
        
        // -----------------------------------------------------------------------------
        // GCURL JOB 1: TRANSFER FILE
        // -----------------------------------------------------------------------------
        
        #ifdef _GCURL_I_NEED_ALL_PROTOTYPES_
  
              static int     INTERNAL_FileTransfer_cbWrite (void *buffer, size_t size, size_t nmemb, void *stream); 
              static int     INTERNAL_FileTransfer_cbProgress (void *clientp, double dltotal, double dlnow, double ultotal, double ulnow); 
              extern int     INTERNAL_FileTransfer_DrawNetProgressBar (void);
              extern int     INTERNAL_FileTransfer_Message_PROGRESS (void);
              
              static ndint64 INTERNAL_FileTransfer_cbOKPressed (char *StringID, ndint64 InfoField, char WndHandle); 
              static ndint64 INTERNAL_FileTransfer_cbCloseWndDuringTransfer (char WndHandle, ndint64 WndInfoField); 
              static ndint64 INTERNAL_FileTransfer_cbCloseWndWorkComplete (char WndHandle, ndint64 WndInfoField); 
              static ndint64 INTERNAL_FileTransfer_cbAbort (char *StringID, ndint64 InfoField, char WndHandle); 
              
              extern void    INTERNAL_FileTransfer_SetupColors (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions); 
              extern void    INTERNAL_FileTransfer_SetupPosition (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions); 
              extern void    INTERNAL_FileTransfer_DrawNetProgressBar (float Value, struct gCurl_FileTransfer_cbContainer *MyCbContainer, char RenderNow); 
              extern void    INTERNAL_FileTransfer_Message_PROGRESS (struct gCurl_FileTransfer_cbContainer *MyCbContainer, int BytesNow, char RenderNow); 
              extern void    INTERNAL_FileTransfer_Message_COMPLETE_WITHOUT_ERRORS (struct gCurl_FileTransfer_cbContainer *MyCbContainer, char RenderNow); 
              extern void    INTERNAL_FileTransfer_Message_USERABORTED (struct gCurl_FileTransfer_cbContainer *MyCbContainer, char RenderNow); 
              extern void    INTERNAL_FileTransfer_Message_CURLERROR (struct gCurl_FileTransfer_cbContainer *MyCbContainer, int Error, char RenderNow); 
              extern int     INTERNAL_FileTransfer_CreateWnd (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions); 
              extern void    INTERNAL_FileTransfer_EndTransfer_SUCCESS (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions); 
              extern void    INTERNAL_FileTransfer_EndTransfer_USERABORTED (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions); 
              extern void    INTERNAL_FileTransfer_EndTransfer_CURLERROR (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions, int CurlError); 
              extern void    INTERNAL_FileTransfer_SetVerboseMode (struct gCurl_FileTransfer_Options *MyOptions, CURL *MyCurl); 
              extern void    INTERNAL_FileTransfer_SetProxyServer (struct gCurl_FileTransfer_cbContainer *MyCbContainer, struct gCurl_FileTransfer_Options *MyOptions, struct ndNetworkObject_Type *NetObj, CURL *MyCurl); 
              
         #endif
  
              extern int     ndGCURL_FileTransfer (struct ndNetworkObject_Type *NetObj, char *UrlSrcFile, char *DstFile, struct gCurl_FileTransfer_Options *Options); 

        
#endif
