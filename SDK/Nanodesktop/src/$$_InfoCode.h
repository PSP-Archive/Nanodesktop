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
    CODICI DI INFO DI NANODESKTOP (livello code)
    ----------------------------------------------------------------------------
    Questo file speciale contiene i codici info usati dal livello Code di
    Nanodesktop 
    
    Esso può essere richiamato durante la compilazione dei moduli, oppure durante
    la compilazione delle applicazioni attraverso Nanodesktop.h
    ----------------------------------------------------------------------------
*/

#ifndef _INFOCODE_CONTAINER
        #define _INFOCODE_CONTAINER

                #define NDGET___ERROR                          0x8000000000000000
                
                #define NDGET_HAL                              0x80000000
                
                #define NDGET_MAXSCREENX                       0x0001
                #define NDGET_MAXSCREENY                       0x0002
                #define NDGET_VMAXSCREENX                      0x0003
                #define NDGET_VMAXSCREENY                      0x0004
                #define NDGET_WINDOWS_MAXSCREENX               0x0005
                #define NDGET_WINDOWS_MAXSCREENY               0x0006
                #define NDGET_MAXCURSX                         0x0007
                #define NDGET_MAXCURSY                         0x0008
                
                #define NDGET_MINWINDOWSBAR_X1                 0x0009                 
                #define NDGET_MINWINDOWSBAR_Y1                 0x000A
                #define NDGET_MINWINDOWSBAR_X2                 0x000B
                #define NDGET_MINWINDOWSBAR_Y2                 0x000C
                #define NDGET_MINWINDOWSBARISCHANGED           0x000D
                #define NDGET_OTHERFBPAGEISNOTUPGRADED         0x000E
                #define NDGET_LL_POSCURSX                      0x000F
                #define NDGET_LL_POSCURSY                      0x0010
                
                #define NDGET_NRWINDOWSALLOCABLE               0x0100
                #define NDGET_NRBUTTONSALLOCABLE               0x0101
                #define NDGET_NRTRACKBARSALLOCABLE             0x0102
                #define NDGET_NRFONTS_USABLE                   0x0103
                #define NDGET_USERAREASIZE                     0x0104  
                
                #define NDGET_MOUSE_MAXSCREENX                 0x0200
                #define NDGET_MOUSE_MAXSCREENY                 0x0201  
                
                #define NDGET_NDLL_CURRENT_FONT_HANDLER        0x0300
                #define NDGET_NDLP_CURRENT_FONT_HANDLER        0x0301
                #define NDGET_NDBS_CURRENT_FONT_HANDLER        0x0302 
                #define NDGET_FONT_INFO_ADR                    0x0303
                
                #define NDGET_MOUSEPOINTER_POSX                0x0400
                #define NDGET_MOUSEPOINTER_POSY                0x0401
                #define NDGET_OLDMOUSEPOINTER_POSX             0x0402
                #define NDGET_OLDMOUSEPOINTER_POSY             0x0403
                #define NDGET_ISTHEFIRSTTIMEFORMOUSE           0x0404
                #define NDGET_BACKGROUNDMOUSE                  0x0405   
                
                #define NDGET_BASESCREENISACTIVE               0x0500
                #define NDGET_BASESCREEN                       0x0501            
                #define NDGET_TRASPARENCYENABLED               0x0502
                #define NDGET_OWNERARRAY                       0x0503
                #define NDGET_WINDOWSVECTOR                    0x0504
                #define NDGET_NRWINDOWSALLOCATED               0x0505
                
                #define NDGET_MAXDIM_WSLENGTH                  0x1001
                #define NDGET_MAXDIM_WSHEIGHT                  0x1002
                #define NDGET_OVERDIMX                         0x1003
                #define NDGET_OVERDIMY                         0x1004
                
                #define NDGET_THUMBNAILENABLED                 0x1100
                
                #define NDGET_STDOUTWINDOWACTIVATED            0x1300
                #define NDGET_STDOUTPOSX1                      0x1301
                #define NDGET_STDOUTPOSY1                      0x1302
                #define NDGET_STDOUTPOSX2                      0x1303
                #define NDGET_STDOUTPOSY2                      0x1304
                #define NDGET_STDOUTCOLORTITLE                 0x1305
                #define NDGET_STDOUTCOLORBGTITLE               0x1306
                #define NDGET_STDOUTCOLORWINDOW                0x1307
                #define NDGET_STDOUTCOLORBORDERWINDOW          0x1308
                #define NDGET_STDOUTCOLORTEXT                  0x1309
                #define NDGET_STDOUTCOLORTEXTERR               0x1310
                #define NDGET_STDOUTWNDHANDLER                 0x1311
                
                #define NDGET_SYSTEMKEYBOARDTYPE               0x2000
                #define NDGET_SYSTEMKEYBOARDAUTOCLOSE          0x2001
                #define NNM_SYSTEMRECORD_ADR                   0x2002
                
                #define NDGET_BUTTONCLASS1_ADR                 0x3000
                #define NDGET_BUTTONCLASS2_ADR                 0x3100
                
                #define NDDEVIL_SUPPORT_IS_OPERATIVE           0x4000
                #define NDDEVIL_ILGENIMAGE_ADR                 0x4001
                #define NDDEVIL_ILBINDIMAGE_ADR                0x4002
                #define NDDEVIL_ILLOADL_ADR                    0x4003
                #define NDDEVIL_ILCOPYPIXEL_ADR                0x4004
                #define NDDEVIL_ILDELETEIMAGES_ADR             0x4005
                
#endif


