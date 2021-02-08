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
    ---------------------------------------------------------------------------
    FILE SPECIALE DI DEFINIZIONE DEGLI ERRORI DI SISTEMA
    ---------------------------------------------------------------------------
    Questo file speciale contiene i codici di errore usati da Nanodesktop. 
    
    Esso può essere richiamato durante la compilazione dei moduli, oppure durante
    la compilazione delle applicazioni attraverso Nanodesktop.h
    ----------------------------------------------------------------------------
*/

#ifndef _ERRORS_CONTAINER
        #define _ERRORS_CONTAINER
        
    //  I codici di errore sono definiti come costanti manifeste
     
    #define    ERR_NO_WINDOW_IS_AVAILABLE              -1
    #define    ERR_WINDOW_IS_TOO_SMALL                 -2
    #define    ERR_WINDOW_IS_NOT_ALLOCATED             -3
    #define    ERR_PIXEL_IS_OUT_OF_WINDOW              -4
    #define    ERR_ELEMENT_NOT_PRESENT                 -5
    #define    ERR_ELEMENT_ALREADY_PRESENT             -6
    #define    ERR_PIXEL_NOT_IN_WS                     -7
    #define    ERR_RECTANGLE_OUT_OF_WS                 -8
    #define    ERR_WSCURSOR_IS_OUT_OF_WS               -9
    #define    ERR_IMAGE_OUT_OF_WS                    -10
    #define    ERR_WINDOW_WAS_NOT_ACTIVE              -11
    #define    ERR_WS_SPACE_REQUEST_IS_TOO_LARGE      -12
    #define    ERR_WINDOW_WILL_BE_TOO_BIG             -13
    #define    ERR_WRONG_ROI_COORD                    -14
    #define    ERR_WINDOW_OVER_FUNC_AREA              -15
    #define    ERR_NO_BUTTON_RECORD_IS_AVAILABLE      -16
    #define    ERR_BUTTON_OUT_OF_WS                   -17
    #define    ERR_WRONG_BUTTON_PARAMETERS            -18
    #define    ERR_MEMORY_ERROR                       -19
    #define    ERR_MEMORY_OVERFLOW                    -20
    #define    ERR_LIST_IS_EMPTY                      -21
    #define    ERR_NR_ELEMENT_IS_INVALID              -22
    #define    ERR_SWAP_IS_NOT_ALLOWED                -23
    #define    ERR_I_CANNOT_OPEN_DIR                  -24
    #define    ERR_OPERATION_INTERRUPTED              -25
    #define    ERR_ACCESS_SYSTEM_PATH                 -26
    #define    ERR_WRONG_PARAMETERS                   -27
    #define    ERR_CANNOT_CREATE_FILE                 -28
    #define    ERR_FILE_NOT_FOUND                     -29
    #define    ERR_FORMAT_NOT_SUPPORTED               -30
    #define    ERR_IO_IMAGE                           -31
    #define    ERR_LOGICAL_IN_IMAGE                   -32
    #define    ERR_WRONG_ICON_FORMAT                  -33
    #define    ERR_SAME_STRINGID_IS_PRESENT           -34
    #define    ERR_IMAGE_IS_NOT_ALLOCATED             -35
    #define    ERR_STRINGID_SYS_DENIED                -36
    #define    ERR_STRINGID_TOO_LONG                  -37
    #define    ERR_ICON_NOT_FOUND                     -38
    #define    ERR_SAME_NAMECLASS_IS_PRESENT          -39
    #define    ERR_NAMECLASS_TOO_LONG                 -40
    #define    ERR_CLASS_NOT_FOUND                    -41
    #define    ERR_EXT_NOT_FOUND                      -42
    #define    ERR_I_CANNOT_CREATE_THUMBS             -43
    #define    ERR_MESSAGE_IS_TOO_LONG                -44
    #define    ERR_FILELIST_OVERFLOW                  -45
    #define    ERR_STDOUT_WINDOW_NOT_ACTIVE           -46
    #define    ERR_WINDOW_IS_NOT_RESIZEABLE           -47
    #define    ERR_PIXEL_NOT_IN_SS                    -48
    #define    ERR_SS_NOT_SUPPORTED                   -49 
    #define    ERR_RECTANGLE_OUT_OF_SS                -50
    #define    ERR_SAME_L1WMI_IS_PRESENT              -51
    #define    ERR_NAMEITEM_IS_TOO_LONG               -52
    #define    ERR_L1WMI_NOT_FOUND                    -53 
    #define    ERR_SAME_L2WMI_IS_PRESENT              -54
    #define    ERR_L2WMI_NOT_FOUND                    -55 
    #define    ERR_ZSTRING_TOO_LONG                   -56
    #define    ERR_SYNTAX_ERROR                       -57
    #define    ERR_INTERNAL_ERROR                     -58
    #define    ERR_INVALID_WINHANDLE                  -59
    #define    ERR_WINMENU_NOT_SUPPORTED              -60
    #define    ERR_BUTTON_IS_NOT_ACTIVE               -61
    #define    ERR_STRINGID_NOT_FOUND                 -62
    #define    ERR_MOUSE_NOT_CONNECTED                -63
    #define    ERR_VALUE_MISMATCH                     -64 
    #define    ERR_NO_TRACKBAR_RECORD_IS_AVAILABLE    -65
    #define    ERR_I_NEED_MORE_BUTTONS                -66   
    #define    ERR_TRACKBAR_SPACE_TOO_SMALL           -67   
    #define    ERR_TRACKBAR_IS_NOT_ALLOCATED          -68
    #define    ERR_IPLIMAGE_NOT_ALLOCATED             -69
    #define    ERR_NO_FONT_RECORD_IS_AVAILABLE        -70
    #define    ERR_FONTHDL_IS_NOT_ACTIVE              -71
    #define    ERR_GRAPHFONT_NOT_SUPPORTED            -72
    #define    ERR_FONT_FORMAT_NOT_SUPPORTED          -73
    #define    ERR_PIXEL_NOT_IN_BS                    -74
    #define    ERR_RECTANGLE_OUT_OF_BS                -75
    #define    ERR_IS_NOT_VALID_NTF_FILE              -76
    #define    ERR_I_CANNOT_DESTROY_THE_FONT          -77
    #define    ERR_IMAGE_OUT_OF_BS                    -78
    #define    ERR_FUNCTION_NOT_SUPPORTED             -79
    #define    ERR_NO_CHANNEL_AVAILABLE               -80
    #define    ERR_ERROR_IN_OPEN_DEVICE               -81
    #define    ERR_CHANNEL_NOT_ACTIVE                 -82
    #define    ERR_IO_ERROR                           -83
    #define    ERR_CHANNEL_NOT_CONFIGURED             -84
    #define    ERR_YOU_MUST_USE_KSU                   -85
    #define    ERR_RAWIR_NOT_AVAILABLE                -86
    #define    ERR_TIMEOUT                            -87
    #define    ERR_NO_VKB_AVAILABLE                   -88
    #define    ERR_VKB_WRONG_TYPE                     -89
    #define    ERR_I_CANNOT_CREATE_VKB                -90
    #define    ERR_INVALID_VKBHANDLE                  -91
    #define    ERR_NO_USB_HANDLE_IS_AVAILABLE         -92
    #define    ERR_DEVICE_NOT_SUPPORTED               -93
    #define    ERR_I_CANNOT_CONNECT_TO_EYESERVER      -94
    #define    ERR_LOGICAL_ERROR_IN_EYESERVER         -95
    #define    ERR_DATA_INTERRUPTED                   -96 
    #define    ERR_USBHANDLE_IS_NOT_ACTIVE            -97  
    #define    ERR_FUNCTION_HAS_BEEN_DISABLED         -98 
    #define    ERR_SYSTEM_IS_BUSY                     -99 
    #define    ERR_NO_TEXTBOX_HANDLE_IS_AVAILABLE     -100  
    #define    ERR_TEXTBOX_IS_TOO_SMALL               -101  
    #define    ERR_TEXTBOX_HANDLE_IS_NOT_ACTIVE       -102
    #define    ERR_LBOXLIST_OVERFLOW                  -103 
    #define    ERR_LISTBOX_IS_TOO_SMALL               -104   
    #define    ERR_NO_LISTBOX_HANDLE_IS_AVAILABLE     -105
    #define    ERR_LISTBOX_IS_NOT_ACTIVE              -106
    #define    ERR_NOWEBCAM_IS_AVAILABLE              -107
    #define    ERR_NO_HANDLE_AVAILABLE                -108
    #define    ERR_HANDLE_IS_NOT_ACTIVE               -109
    #define    ERR_DISABLED_FOR_TROUBLES_IN_KRNEXTENDER -110 
    #define    ERR_DISABLED_FOR_TROUBLES_IN_JPEGDEC     -111
    #define    ERR_GOCAM_SUPPORT_IS_DISABLED            -112
    #define    ERR_DEVICE_IS_NOT_OPERATIVE              -113
    #define    ERR_CANNOT_LOAD_DRIVERS                  -114
    #define    ERR_DISABLED_FOR_TROUBLES_IN_MODULE_LOAD -115

    
    #define    DIALOG_BOX_EXIT               100
    
    //  Codici di errore di rete
    
    #define    ERRNET_PSP__FAILED_TO_LOAD_IFHANDLE_MODULE     0x00010001
    #define    ERRNET_PSP__FAILED_TO_LOAD_NET_MODULE          0x00010002
    #define    ERRNET_PSP__FAILED_TO_LOAD_NETINET_MODULE      0x00010003
    #define    ERRNET_PSP__FAILED_TO_LOAD_APCTL_MODULE        0x00010004
    #define    ERRNET_PSP__FAILED_TO_LOAD_RESOLVER_MODULE     0x00010005
    #define    ERRNET_PSP__FIRMWARE_IS_NOT_SUPPORTED_FOR_NET  0x00010006
    #define    ERRNET_PSP__GENERIC_FAILURE_IN_LOAD            0x00010007
    #define    ERRNET_PSP__GENERIC_FAILURE_IN_LOAD_CFMOD0     0x00010008
    #define    ERRNET_PSP__GENERIC_FAILURE_IN_LOAD_CFMOD1     0x00010009
    
    #define    ERRNET_PSP__FAILED_TO_INIT_SDK_NETLAYER        0x00010010
    #define    ERRNET_PSP__FAILED_TO_INIT_SDK_NET             0x00010011
    #define    ERRNET_PSP__FAILED_TO_INIT_SDK_NETINET         0x00010012
    #define    ERRNET_PSP__FAILED_TO_INIT_SDK_RESOLVER        0x00010013
    #define    ERRNET_PSP__FAILED_TO_INIT_SDK_APCTL           0x00010014
    
    #define    ERRNET_PSP__NETWORK_HAS_BEEN_DISABLED_BY_SOFT   0x00010021
    #define    ERRNET_PSP__NETWORK_IS_FAILURE                  0x00010022
    #define    ERRNET_PSP__FAILURE_IN_APCTL_CONNECT            0x00010023
    #define    ERRNET_PSP__FAILURE_IN_APCTL_GETSTATE           0x00010024
    #define    ERRNET_PSP__TIMEOUT                             0x00010025
    #define    ERRNET_PSP__LAN_SWITCH_IS_OFF                   0x00010026
    #define    ERRNET_PSP__FAILURE_IN_APCTL_DISCONNECT         0x00010027
    
    #define    ERRNET_PSP__APCTL_CONFIG_NOT_EXIST              0x00010030
    
    // Codici di errore media
    
    #define    ERR_MEDIA_SUPPORT_HAS_BEEN_DISABLED                   -1
    #define    ERR_MEDIA_CHANNEL_ALREADY_BUSY                        -2
    #define    ERR_MEDIA_FILE_NOT_FOUND                              -3
    #define    ERR_MEDIA_I_CANNOT_OPEN_FILE                          -4
    #define    ERR_MEDIA_CODEC_HAS_NOT_MEMORY                        -5
    #define    ERR_MEDIA_TROUBLE_IN_EDRAM                            -6
    #define    ERR_MEDIA_TROUBLE_IN_MPEG_CODEC_INIT                  -7
    #define    ERR_MEDIA_WRONG_HEADER                                -8
    #define    ERR_MEDIA_UNSUPPORTED_TYPE                            -9
    #define    ERR_MEDIA_SAMPLE_IS_OUT_OF_FILE                       -10
    
    // Codici di errore per i file .WAV
    
    #define    ERR_WAV_WRONG_HEADER                                  -1        
    #define    ERR_WAV_IT_DOES_NOT_BEGIN_WITH_RIFF                   -2
    #define    ERR_WAV_TROUBLE_WITH_CODE_SEQUENCE                    -3 
    #define    ERR_WAV_TROUBLE_WITH_DATA_CODE_SEQUENCE               -4                 
    #define    ERR_WAV_FILE_TOO_SHORT                                -5   
    #define    ERR_WAV_ONLY_MONO_OR_STEREO_SUPPORTED                 -10
    #define    ERR_WAV_WRONG_SAMPLE_RATE                             -12
    #define    ERR_WAV_ONLY_16BIT_OR_8BIT_FOR_SAMPLE_ALLOWED         -13
    #define    ERR_WAV_INVALID_SAMPLE_COUNT                          -14

#endif     // #ifndef _ERRORS_CONTAINER
                                    

