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
    FILE SPECIALE DI DEFINIZIONE DELLE VARIABILI PER HAL (PSP CFW mode)
    ---------------------------------------------------------------------------
    Questo file speciale può essere richiamato in due casi:
           a) dal modulo di inizializzazione ndHAL_Init.c di Nanodesktop 
              durante la compilazione della libreria. In tal caso, il 
              modulo deve prima definire la costante manifesta
              HAL_I_NEED_TO_DECLARE_VARIABLES. Questa costante manifesta farà
              sì che le variabili siano dichiarate (comprese quelle statiche),
              e che il loro simbolo rimanga memorizzato nel modulo di
              inizializzazione
              
           b) da un modulo di Nanodesktop che è diverso dal modulo di 
              inizializzazione, o dal codice di un'applicazione attraverso
              nanodesktop.h. In questi casi non viene definita la 
              costante manifesta HAL_I_NEED_TO_DECLARE_VARIABLES, e
              quindi le variabili static saranno invisibili, mentre le 
              variabili non static saranno dichiarate automaticamente extern
    --------------------------------------------------------------------------- 
*/


#ifndef _NDHAL_VARIABLES_CONTAINER_
      #define  _NDHAL_VARIABLES_CONTAINER_
      
      // Impedirà che questo file possa essere incluso due volte nella compilazione


#ifdef HAL_I_NEED_TO_DECLARE_VARIABLES
       
       #define NDEXTERN             // Vuoto in modo che la variabile sia effettivamente dichiarata
       
#else  // Solo variabili extern

       #define NDEXTERN extern

#endif
       
                                /*
                                        ------------------------------------------------------------------------
                                        DEFINIZIONE DI NUOVI TIPI DATI
                                        ------------------------------------------------------------------------
                                */
                                    
                                        typedef long long int ndint64;
                                        // Intero a 64 bit
                                    
                                /*
                                        ------------------------------------------------------------------------
                                        RIFERIMENTO EXTERN AL SEED DELLA PIATTAFORMA
                                        ------------------------------------------------------------------------
                                */
                                
                                        extern int ndPspCfwSeed;
                                    
                                    
       
                                /*  
                                    ----------------------------------------------------------------------------
                                    SEZIONE MAXLISTELEMENTALLOWED
                                    ----------------------------------------------------------------------------
                                */
                                
                                // #define NO_FILELIST_OVERFLOW_CHECK
                                
                                // Decommentare la riga precedente su quelle piattaforme nelle quali è possibile
                                // bypassare il controllo overflow in ndDB_AppendFileToList.
                                
                                #define MAXLISTELEMENTALLOWED 15000
                                  
                                /*  
                                    -------------------------------------------------------------------------------
                                    HAL: DEFINIZIONE DI COSTANTI PER PORTABILITA' RAPIDA
                                    -------------------------------------------------------------------------------
                                    Le librerie NANODesktop sono progettate per potere essere portate con rapidità
                                    su un architettura differente. In molti casi, è sufficiente modificare le
                                    seguenti costanti, e riscrivere le routine di tipo Low Level
                                    --------------------------------------------------------------------------------
                                */
                                
                                #define __NRWINDOWSALLOCABLE 24
                                #define __MAXSCREENX 480
                                #define __MAXSCREENY 272
                                #define __VMAXSCREENX 512
                                #define __VMAXSCREENY 272
                                #define __MAXCURSX 60
                                #define __MAXCURSY 33
                                #define __NRBUTTONSALLOCABLE 66
                                #define __NRTRACKBARSALLOCABLE 10
                                #define __NRTEXTBOXALLOCABLE 32
                                #define __NRLISTBOXALLOCABLE 32
                                #define __USERAREASIZE 32
                                #define __VKB1_POSX1   90
                                #define __VKB1_POSY1  110
                                #define __VKB1_LENX   350
                                #define __VKB1_LENY   140
                                #define __BASESCREENX 480
                                #define __BASESCREENY 272
                                
                                
                                #define __MINWNDSIZEX_WITHOUT_MENU     48
                                #define __MINWNDSIZEY_WITHOUT_MENU     48
                                #define __MINWNDSIZEX_WITH_MENU        110
                                #define __MINWNDSIZEY_WITH_MENU        110
                                
                                #define __NNM_POSX1   20
                                #define __NNM_POSY1   10
                                #define __NNM_LENX   390
                                #define __NNM_LENY    70
                                
                                #define __LENX_FILEMAN_ERRORBOX_VOID  270
                                #define __LENY_FILEMAN_ERRORBOX_VOID  100
                                
                                #define __LENX_FILEMAN_ERRORBOX_MKDIR 300
                                #define __LENY_FILEMAN_ERRORBOX_MKDIR 140
                                
                                #define __LENX_FILEMAN_ERRORBOX_CONFIRMDEL 300
                                #define __LENY_FILEMAN_ERRORBOX_CONFIRMDEL 140
                                
                                #define __LENX_FILEMAN_ERRORBOX_ERRDEL 300
                                #define __LENY_FILEMAN_ERRORBOX_ERRDEL 140
                                
                                #define __LENX_FILEMAN_ERRORBOX_CONFIRMREN 300
                                #define __LENY_FILEMAN_ERRORBOX_CONFIRMREN 180
                                
                                #define __LENX_FILEMAN_ERRORBOX_ERRREN 300
                                #define __LENY_FILEMAN_ERRORBOX_ERRREN 140
                                
                                #define __MOUSE_DELAY 0.0024840000000185803200001389807936/2
                                #define __DELAY_SCALE 80515297.906                       // Non è più necessario perchè sostituito da routine RTC
                                
                                #define __DIMY_BOXFMT1  45
                                // Dimensione Y dei box nel file manager di tipo 1
                                
                                /*
                                   -------------------------------------------------------------------------
                                   HAL: VARIABILI PER LE ROUTINE GRAFICHE A LIVELLO LOW-LEVEL
                                   -------------------------------------------------------------------------
                                
                                   Lo schermo della PSP è gestito in modo un pò... strano.
                                   Lo schermo della PSP, dal punto di vista fisico, deve essere visto come un
                                   framebuffer a 16 bit di 512*272 pixel. Questo significa che l'ampiezza in
                                   byte del framebuffer è di 278.528 byte (in esadecimale 0x44000). Tuttavia,
                                   l'area effettivamente visualizzabile è di soli 480*272 pixel: vuol dire che
                                   32 pixel a destra restano costantemente off-screen.
                                
                                   Poichè i caratteri hanno una dimensione di 8x8 pixel, vuol dire che sono
                                   visualizzabili 60x38 caratteri
                                
                                   Le seguenti costanti simboliche servono alle routine di accesso al
                                   video che sono hardware specifiche
                                   -------------------------------------------------------------------------
                                */
                                
                                 
                                 
                            #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES 
                                 unsigned int       PageSize = 0x44000;
                            #else
                                 NDEXTERN unsigned int PageSize;
                            #endif
                                
                                //   Le seguenti variabili si occupano di gestire il calcolo dell'indirizzo
                                //   del framebuffer relativo ad ogni pixel.
                                
                                 NDEXTERN unsigned short int       ActiveFBPage;                                  // La pagina attiva in quel momento. Sono
                                                                                                                  // previste due pagine (0 e 1).
                                
                            #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES 
                                 int                               PSPEBaseRegister= 0x40000000;                  // PSPE Base Register: questo indirizzo serve per compatibilità
                            #else                                                                                 // con la vera PSP e con l'emulatore. E' uno scostamento che deve
                                 NDEXTERN  int                     PSPEBaseRegister;                              // essere sommato ad ogni indirizzo
                            #endif
                                                                   
                            #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES                                       
                                 int                               XAddrVideoMemory = 0x04000000;                 // Questo è l'indirizzo "virtuale" della memoria video in Ram.
                            #else
                                 NDEXTERN int                      XAddrVideoMemory;                              // E' un indirizzo virtuale perchè tutti gli indirizzi devono
                            #endif                                                                                // essere traslati di una quantità pari al PSPEBaseRegister;
                                
                                 NDEXTERN int                      AddrVideoMemory;                               // E' destinata ad accogliere il vero indirizzo della memoria video,
                                                                                                                  // dato dalla somma del PSPEBaseRegister e di XAddrVideoMemory;
                                
                                 NDEXTERN int                      LastFBPageRead,    WRCurrentPageAddr,          // Per non ricalcolarlo ogni volta, l'indirizzo della pagina
                                                                   LastFBPageWritten, RDCurrentPageAddr;          // del framebuffer dove è avvenuta l'ultima scrittura viene memorizzato in una
                                                                                                                  // variabile globale. Inoltre, viene memorizzata la pagina dove è
                                                                                                                  // avvenuta l'ultima scrittura, in modo che sia possibile ricalcolare
                                                                                                                  // l'indirizzo di pagina solo quando si cerca di scrivere in una pagina
                                                                                                                  // diversa
                                
                                 NDEXTERN int                      LastLineWritten, LastLineRead;                 // Poichè nel funzionamento di questa libreria sono
                                                                                                                  // richieste notevoli quantità di calcoli per generare
                                                                                                                  // gli indirizzi dei pixel, viene mantenuta una linea
                                                                                                                  // corrente, ed insieme un offset di linea. Solo quando
                                                                                                                  // la richiesta di scrivere un pixel è eseguita su una
                                                                                                                  // riga differente rispetto a quella corrente, viene
                                                                                                                  // eseguito il ricalcolo dell'indirizzo offset di linea,
                                                                                                                  // altrimenti viene usato quello già calcolato
                                
                                 NDEXTERN int                      WRCurrentLineAddr, RDCurrentLineAddr;          // Indirizzo dei dati della linea corrente
                                
                                 
                                /* 
                                   ---------------------------------------------------------------------------
                                   HAL: VARIABILI PER L'ENGINE WINDOWS RENDER
                                   ---------------------------------------------------------------------------
                                */
                                
                                #define __MAXVERTEXALLOCABLE 2*(__NRWINDOWSALLOCABLE+1)
                                #define __MAXTILESALLOCABLE  (__MAXVERTEXALLOCABLE+1)*(__MAXVERTEXALLOCABLE+1)
                                
                                struct ndXVertex_Type
                                {
                                    unsigned short int PosX;
                                    char Enabled;
                                };
                                
                                struct ndYVertex_Type
                                {
                                    unsigned short int PosY;
                                    char Enabled;
                                };
                                
                                NDEXTERN struct ndXVertex_Type ndXVertexBuffer [__MAXVERTEXALLOCABLE];
                                NDEXTERN struct ndYVertex_Type ndYVertexBuffer [__MAXVERTEXALLOCABLE];
                                
                                NDEXTERN int NrXVertexEnabled;
                                NDEXTERN int NrYVertexEnabled;
                                
                                struct ndNanoTile_Type
                                {
                                    unsigned short int PosX1, PosY1, PosX2, PosY2;
                                    char Enabled;
                                    
                                    unsigned char Level1;
                                    unsigned char Level2;
                                };
                                
                                NDEXTERN struct ndNanoTile_Type ndNanoTile [__MAXTILESALLOCABLE];
                                
                                NDEXTERN int NrNanoTilesEnabled;
                                
                                NDEXTERN unsigned char OwnerArray [__VMAXSCREENX][__VMAXSCREENY][2];
                                
                           #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES  
                                int ndNrVideoFrame = 0;
                           #else
                                NDEXTERN int ndNrVideoFrame;
                           #endif      
                                
                                
                                /*
                                   ---------------------------------------------------------------------------
                                   HAL: DEFINIZIONE DELLE STRUTTURE DATI PER IL CONTROLLO DEL MOUSE E PER LA
                                   GESTIONE DEI PULSANTI
                                   ---------------------------------------------------------------------------
                                */
                                
                                
                                
                                // Definizione delle costanti per la gestione del controller
                                
                                #define CTRL_ANALOG_X   0
                                #define CTRL_ANALOG_Y   1
                                
                                /* Button bit masks */
                                #define PSP_CTRL_SQUARE     0x8000
                                #define PSP_CTRL_TRIANGLE   0x1000
                                #define PSP_CTRL_CIRCLE     0x2000
                                #define PSP_CTRL_CROSS      0x4000
                                #define PSP_CTRL_UP         0x0010
                                #define PSP_CTRL_DOWN       0x0040
                                #define PSP_CTRL_LEFT       0x0080
                                #define PSP_CTRL_RIGHT      0x0020
                                #define PSP_CTRL_START      0x0008
                                #define PSP_CTRL_SELECT     0x0001
                                #define PSP_CTRL_LTRIGGER   0x0100
                                #define PSP_CTRL_RTRIGGER   0x0200
                                
                            #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES                                       
                                 char                              PadAnalogModeEnabled = 0;
                            #else
                                 NDEXTERN char                     PadAnalogModeEnabled;
                            #endif                                                                                // essere traslati di una quantità pari al PSPEBaseRegister;
                              
                                 NDEXTERN SceCtrlData IndirectPadRecord;
                                       
                                
                                
                                /*
                                  --------------------------------------------------------------------------
                                  HAL: COSTANTI COLORE
                                
                                  Le routine NANODesktop supportano colori a 16 bit. Tuttavia, per facilitare
                                  il compito al programmatore, sono definite 16 costanti manifeste che
                                  riportano gli errori più comuni, ed è presente l'array ndColorVector
                                  che contiene tali colori
                                  --------------------------------------------------------------------------
                                */
                                
                                typedef unsigned short int TypeColor;
                                
                                #define    COLOR_BLACK      0x0000
                                #define    COLOR_SSBLACK    0x0001
                                #define    COLOR_RED        0x1111
                                #define    COLOR_GREEN      0x2222
                                #define    COLOR_LGREEN     0x3333
                                #define    COLOR_BLUE       0x4444
                                #define    COLOR_PINK       0x5555
                                #define    COLOR_LBLUE      0x6666
                                #define    COLOR_LLBLUE     0x7777
                                #define    COLOR_DRED       0x8888
                                #define    COLOR_ORANGE     0x9999
                                #define    COLOR_DGREEN     0xAAAA
                                #define    COLOR_YELLOW     0xBBBB
                                #define    COLOR_VIOLET     0xCCCC
                                #define    COLOR_LPINK      0xDDDD
                                #define    COLOR_CYAN       0xEEEE
                                #define    COLOR_WHITE      0xFFFF
                                #define    COLOR_GRAY       0xC210
                                
                                #define    COLOR_GRAY01     1*1057
                                #define    COLOR_GRAY02     2*1057
                                #define    COLOR_GRAY03     3*1057
                                #define    COLOR_GRAY04     4*1057
                                #define    COLOR_GRAY05     5*1057
                                #define    COLOR_GRAY06     6*1057
                                #define    COLOR_GRAY07     7*1057
                                #define    COLOR_GRAY08     8*1057
                                #define    COLOR_GRAY09     9*1057
                                #define    COLOR_GRAY10    10*1057
                                #define    COLOR_GRAY11    11*1057
                                #define    COLOR_GRAY12    12*1057
                                #define    COLOR_GRAY13    13*1057
                                #define    COLOR_GRAY14    14*1057
                                #define    COLOR_GRAY15    15*1057
                                #define    COLOR_GRAY16    16*1057
                                #define    COLOR_GRAY17    17*1057
                                #define    COLOR_GRAY18    18*1057
                                #define    COLOR_GRAY19    19*1057
                                #define    COLOR_GRAY20    20*1057
                                #define    COLOR_GRAY21    21*1057
                                #define    COLOR_GRAY22    22*1057
                                #define    COLOR_GRAY23    23*1057
                                #define    COLOR_GRAY24    24*1057
                                #define    COLOR_GRAY25    25*1057
                                #define    COLOR_GRAY26    26*1057
                                #define    COLOR_GRAY27    27*1057
                                #define    COLOR_GRAY28    28*1057
                                #define    COLOR_GRAY29    29*1057
                                #define    COLOR_GRAY30    30*1057
                                #define    COLOR_GRAY31    31*1057
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES
                                       TypeColor ndWndColorVector[16]=
                                               { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_LGREEN,
                                                 COLOR_BLUE, COLOR_PINK, COLOR_LBLUE, COLOR_LLBLUE,
                                                 COLOR_DRED, COLOR_ORANGE, COLOR_DGREEN, COLOR_YELLOW,
                                                 COLOR_VIOLET, COLOR_LPINK, COLOR_CYAN, COLOR_WHITE };
                                #else
                                       NDEXTERN TypeColor ndWndColorVector[16];
                                #endif
                                
                                /*
                                   ----------------------------------------------------------------------------------
                                   COSTANTI PER RICHIESTA DELAY
                                   ----------------------------------------------------------------------------------
                                */
                                
                                #define ND_REQDELAY_FOR_VKB0        0
                                #define ND_REQDELAY_MOUSECTRL       1
                                #define ND_REQDELAY_WINMENUGRAPH    2
                                #define ND_REQDELAY_WMI1PRESSED     3
                                #define ND_REQDELAY_WMI2PRESSED     4
                                #define ND_REQDELAY_CLOSE_VKB       5
                                        
                                 
                                 
                                 
                                 
                                /*
                                   -----------------------------------------------------------------------------------
                                   STRUTTURE DATI PER IL SUPPORTO VIRTUAL KEYBOARD TIPO 1
                                   -----------------------------------------------------------------------------------
                                */
                                
                                   #define VKB1_ERR_I_CANNOT_CREATE_WINDOW     1
                                   #define VKB1_ERR_I_CANNOT_CREATE_BUTTONS    2

                                   struct VKB1_InfoRec_Type
                                   { 
                                       char               CapsLockOn, FuncLockOn;
    
                                       unsigned short int kbPosX1, kbPosY1, kbPosX2, kbPosY2;      
                                   };

                                
                                /* 
                                   -----------------------------------------------------------------------------------
                                   STRUTTURE DATI PER IL SUPPORTO DELLA API GENERALE DI VIRTUAL KEYBOARD
                                   -----------------------------------------------------------------------------------
                                */
                                   
                                   #define __NRVIRTUALKEYBOARDS_ALLOWED    32
                                   
                                   // Codici della tastiera
                                   
                                   #define ND_DEFAULT_KEYBOARD             0
                                   #define ND_VIDEO_KEYBOARD               1
                                   
                                   // Caratteristiche della tastiera
                                   
                                   #define ND_KEYBOARD_CALLBACK_BASED          1
                                   #define ND_ASK_ONLY_A_KEY                   2
                                   #define ND_KEYBOARD_NO_AUTOCLOSE            4

                                   struct VKB_InfoRec_Type
                                   {
                                       char           WndHandle;
                                       
                                       char           IsEnabled;
                                       char           TypeKeyboard;
                                       
                                       char           MessageToUser [255];
                                       
                                       ndint64        SystemOptions;
                                       ndint64        KeyboardStyle;
                                       
                                       void           *KeyCallback;  
                                       void           *KeyCallbackData;
                                       void           *EndCallback;  
                                       void           *EndCallbackData;
                                       void           *BreakCallback;
                                       void           *BreakCallbackData;
                                       
                                       int            NrCharsRead;
                                       char           YouCanExit;
                                       
                                       struct         VKB1_InfoRec_Type     kb1;     
                                   };

                                   NDEXTERN struct VKB_InfoRec_Type    SysKeyboardsRegister [__NRVIRTUALKEYBOARDS_ALLOWED];
                                   
                                   NDEXTERN char   ndDefaultKeybType;
                                   
                                   // Codici generali pulsante
                                   
                                   #define  ND_GENERIC_SPECIALKEY    0x80
                                   #define  ND_GENERIC_FUNCKEY       0x40  
                                   #define  ND_KEY_HAS_BEEN_RELEASED 0x01
                                   #define  ND_NORMAL_KEY            0x00
                                   
                                   
                                
                                /*
                                   ------------------------------------------------------------------------------------ 
                                   VARIABILI PER IL GESTORE PHOENIX MOUSE THREAD
                                   ------------------------------------------------------------------------------------
                                */
                                   
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                    char HAL_MouseThreadStatus = 0;
                                    // Posto a 0 per thread non ancora avviato, ad 1 per thread
                                    // avviato ma sospeso, ed a 2 per thread avviato ed attivo
                                #else
                                    NDEXTERN char HAL_MouseThreadStatus;
                                #endif 
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                    char HAL_MouseEmuSystemEnabled = 0;
                                #else
                                    NDEXTERN char HAL_MouseEmuSystemEnabled;
                                #endif 
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES 
                                    char HAL_IsTheFirstTimeForMouse = 1;
                                #else
                                    NDEXTERN char HAL_IsTheFirstTimeForMouse;
                                #endif    
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES 
                                    char HAL_MouseIsControlledByAnalog = 0;
                                #else
                                    NDEXTERN char HAL_MouseIsControlledByAnalog;
                                #endif    
                                
                           
                                NDEXTERN unsigned short int    HAL_MouseEmu_PosX,        HAL_MouseEmu_PosY;
                                NDEXTERN unsigned short int    HAL_MouseEmu_OldPosX,     HAL_MouseEmu_OldPosY;
                                NDEXTERN unsigned short int    HAL_MouseButtons;
                                NDEXTERN int                   HAL_MouseVectorX, HAL_MouseVectorY;
                                 
                                NDEXTERN char phnMouse_SystemStop;
                                NDEXTERN int  phnMouse_ThreadID;
                                
                                /*
                                   ------------------------------------------------------------------------------------ 
                                   VARIABILI PER LA GESTIONE DI MOUSECONTROL
                                   ------------------------------------------------------------------------------------
                                */
                                
                                // Registro richieste per MouseControl
                                
                                struct phnMouse_ReqBufferType
                                {
                                    char   IsEnabled;
                                    struct ndEvent_Type *AddressData;
                                    int    ThreadID_AuthorOfRequest;   
                                    char   IsWaiting;
                                    int    WaitForCBEEnd;
                                };
                                
                                #define __NR_MOUSECTRL_RQ_ALLOWED           256
                                
                                NDEXTERN struct phnMouse_ReqBufferType phnMouse_ReqBufferStdArray [__NR_MOUSECTRL_RQ_ALLOWED];
                                
                                // Eventi registro CBE
                                
                                NDEXTERN int  phnMouse_Ev0_CbeRegister;
                                
                                #define PHNMOUSE_CBEREG_IS_FREE       1
                                
                                // Registro CBE
                                
                                #define __NR_MOUSECTRL_CBE_ALLOWED          128
                                
                                struct phnMouse_CbeSlotType
                                {
                                    char   IsEnabled;
                                    int    ThreadID; 
                                };
                                
                                NDEXTERN struct phnMouse_CbeSlotType phnMouse_CbeRegister [__NR_MOUSECTRL_CBE_ALLOWED];
                                
                                
                                /*
                                   ------------------------------------------------------------------------------------ 
                                   VARIABILI PER IL GESTORE PHOENIX GRAPH THREAD
                                   ------------------------------------------------------------------------------------
                                */
                                
                                #define __NR_THREADS_AVAILABLE 8
                                
                                // Eventi registro Phoenix Graph Callers
                                
                                NDEXTERN int  phnGraph_Ev0_ReqRegister;
                                
                                #define PHNGRAPH_SVR_IS_NOT_BUSY         1
                                
                                // Phoenix Graph
                                
                                NDEXTERN char phnGraph_SystemStop;
                                NDEXTERN int  phnGraph_ThreadID;
                                
                                struct phnGraph_Callers_Type
                                {
                                       char IsEnabled;
                                       int  ThreadID;
                                };
                                
                                NDEXTERN struct phnGraph_Callers_Type phnGraph_Callers [__NR_THREADS_AVAILABLE];
                                
                                // Richieste di rendering
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES  
                                       char phnGraph_WindowsRender_ReqOn = 0;
                                #else
                                       NDEXTERN char phnGraph_WindowsRender_ReqOn;
                                #endif
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES 
                                        char phnGraph_UpdWindowsRenderRegs_ReqOn = 0;
                                #else
                                        NDEXTERN char phnGraph_UpdWindowsRenderRegs_ReqOn;
                                #endif
                                
                                // Il simbolo ND_SWMODE_DisablePhoenixSubsystem è definito nel file
                                // ndHAL_PlatformHeaders: tale simbolo consente di
                                // disabilitare completamente il sistema Phoenix per recuperare
                                // velocità. La libreria include solo il riferimento extern
                                
                                /*
                                   -----------------------------------------------------------------------------------
                                   VARIABILI PER GESTORE CFW
                                   -----------------------------------------------------------------------------------
                                */
                                  
                                   #define CFW_SLEEP_PRIORITY 80
                                   
                                   NDEXTERN int  ndUserMainThreadID;
                                   NDEXTERN char ndServicesLoopExit;
                      
                                   NDEXTERN int  ndCFWThreadID, ndCFWStdPriority;
                                   
                                /*
                                   ------------------------------------------------------------------------------------ 
                                   VARIABILI PER GENERAZIONE SEGNALE RAW-IR
                                   ------------------------------------------------------------------------------------
                                */
                                
                                  #define RAWIR_TIMEOUT 5000000
                                 
                                  NDEXTERN int  ndRawIRThreadRq;
                                  NDEXTERN char RawIRFailure;
                                  
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                   char RawIRMonitorEnabled = 0; 
                                #else
                                   NDEXTERN char RawIRMonitorEnabled;
                                #endif
                                     
                                  NDEXTERN char RawIRMonitorHandle;
                                  
                                  struct ndRawIRConfigBlock_Type
                                  {
                                         char NegLogic; 
                                         char BitNumber; 
                                         char MoreSignif; 
                                         char Frequency;
                                         char SeqBitStart; 
                                         int  Start0, Start0Len; 
                                         int  Start1, Start1Len;
                                         int  Dbit0_A, Dbit0_A_Len, Dbit0_B, Dbit0_B_Len;
                                         int  Dbit1_A, Dbit1_A_Len, Dbit1_B, Dbit1_B_Len;
                                         char SeqBitStop; 
                                         int  Stop0, Stop0Len; 
                                         int  Stop1, Stop1Len;
                                         char MonitorEnabled;
                                  };
                                  
                                /*
                                   ------------------------------------------------------------------------------------ 
                                   VARIABILI PER GENERAZIONE SEGNALE IRDA
                                   ------------------------------------------------------------------------------------
                                */
                                
                                #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES    
                                   char IrdaMonitorEnabled = 0; 
                                #else
                                   NDEXTERN char IrdaMonitorEnabled;
                                #endif
                                   
                                   NDEXTERN char IrdaMonitorHandle;
                                  
                                   struct ndIRDAConfigBlock_Type
                                   {
                                      char MonitorEnabled;   
                                   };
                                  
                                  
                                 
                                /*
                                   ------------------------------------------------------------------------------------
                                   VARIABILI PER API INFRAROSSI
                                   ------------------------------------------------------------------------------------
                                */
                                
                                  #define __IRDACHANNELSSUPPORTED 1
                                  
                                  #define ND_IR_NULL_MODE 0
                                  #define ND_IR_IRDA_MODE 1
                                  #define ND_IR_RAW_MODE  2
                                  
                                  struct ndIrChannel_Type
                                  {
                                         char IsEnabled;
                                         int  DevHandle;
                                         char Mode;
                                         int  IrError;
                                         struct ndRawIRConfigBlock_Type RawIRcfg;
                                         struct ndIRDAConfigBlock_Type IRDAcfg;
                                  };    
                                
                                  NDEXTERN struct ndIrChannel_Type ndIrChannelsArray [__IRDACHANNELSSUPPORTED];
                                  
                                  /*
                                   ------------------------------------------------------------------------------------
                                   VARIABILI PER SUPPORTO USB (low level)
                                   ------------------------------------------------------------------------------------
                                  */
                                
                                  
                                  NDEXTERN int  ndUsb_IsFailure;
                                  NDEXTERN char ndUsb_BusIsBusy;
                                  
                                  NDEXTERN int ndUsb_Failure_MainErrCode;
                                  NDEXTERN int ndUsb_Failure_SecErrCode;
                                  
                                  // Codici di errore USB riconosciuti (codici primari)
                                  
                                  #define ERR_USB_FAILED_LOADING_PRX_DRIVER     0x0001
                                  #define ERR_USB_FAILED_STARTING_PRX_DRIVER    0x0002 
                                  #define ERR_USB_FAILED_LOADING_USB_BUS_DRIVER 0x0003
                                  
                                  // Codici di stato per l'icona
                                  
                                  #define USBSTATE_FAILURE                      0x0001
                                  #define USBSTATE_ATTACHED                     0x0002
                                  #define USBSTATE_DETACHED                     0x0003
                                  
                                  #define GOCAM_FAILURE                         0x0005
                                  #define GOCAM_ATTACHED                        0x0006
                                  #define GOCAM_DETACHED                        0x0007
                                  
                                  #define USBBUS_FAILURE                        0x0008
                                  
                                  // Struttura di comunicazione con il driver USB
                                  
                                  struct ndUsb_CommArea_Type
                                  {
                                      char   GraphSysIsLocked;
                                      int    UsbState, UsbStateIsChanged;
                                      
                                      void   (*PntWindowsRender);
                                      void   (*PntVxPrintf);
                                      void   (*PntSout);
                                      
                                      int    MessageCode;
                                      char   StrMessage [512];
                                      
                                      char   MonitorEnabled;
                                      int    MonitorThread;
                                      
                                      char   WndHandle, MonitorWndIsOpen;
                                      
                                      char   MemoryLockState;
                                      char   ManagerLoopExit;
                                      
                                      int    UsbThread;
                                      
                                      int    ndPspFwVersion;
                                      int    ndPspFwEmuVersion;
                                  };
                                  
                                  
                                  NDEXTERN struct ndUsb_CommArea_Type  ndUsb_CommArea;
                                  
                                  
                                  
                                  
                                  
                                  /*
                                   ------------------------------------------------------------------------------------
                                   VARIABILI PER SUPPORTO USB (high level)
                                   ------------------------------------------------------------------------------------
                                  */
                                
                                  // Include un file che contiene i prototipi per la gestione del bus USB
                                  #include <3rdparty_modules\USB_PRX_Driver\_usb.h>
                                  
                                  
                                  
                                  // Descrittori di dispositivo USB
                                  #define ND_DEVICES_DESCRIPTORS   1
                                  
                                  // Tipi di dispositivo USB
                                  
                                  #define NDDEVTYPE_EYESERVER_CAM      0x0001
                                  #define NDDEVTYPE_GOCAM              0x0002
                                  
                                  // Definizione dei descrittori USB
                                  
                                  struct ndUsb_DeviceDescriptors_Type
                                  {
                                         char IsActive;
                                         int  TypeDevice;
                                  };
                                  
                                  NDEXTERN struct ndUsb_DeviceDescriptors_Type ndUsb_Dev [ND_DEVICES_DESCRIPTORS];
                                  
                                  
                                  /* 
                                     ---------------------------------------------------------
                                     ---------------------------------------------------------
                                     EYESERVER
                                     ---------------------------------------------------------
                                     ---------------------------------------------------------
                                  */
                                  
                                  // Codice di identificazione dell'Eyeserver
                                  
                                  #define EYESERVER_VENDOR_ID   0x054C
                                  #define EYESERVER_PRODUCT_ID  0x01CA
                                  
                                  // Descrittori degli errori
                                  
                                  NDEXTERN int ndEyesvr_IsFailure;
                                  
                                  NDEXTERN int ndEyesvrFailure_MainErrCode;
                                  NDEXTERN int ndEyesvrFailure_SecErrCode;
                                  
                                  // Codici di errore
                                  
                                  #define ERR_USB_FAILED_INIT_HOSTFS_DRIVER     0x0004
                                  #define ERR_USB_FAILED_ACTIVATE_USB_PID       0x0005
                                  
                                  // Profondità di caricamento
                                  
                                  NDEXTERN char ndEyesvrLoadDepth;
                                  
                                  /* ----------------------------------------------------
                                      INTERAZIONE CON EYESERVER
                                     ---------------------------------------------------- */
                                  
                                  #include <3rdparty_modules\USB_PRX_Driver_for_CF\ndUSBdriver.h>
                                    
                                  // Codici messaggio
                                  
                                  #define USBMSG_DEBUG                0x0000
                                  #define USBMSG_SERVICE              0x0001
                                  
                                  // Costanti di funzionamento
                
                                    #define WEBCAM_TILE_DIMX                 16
                                    #define WEBCAM_TILE_DIMY                 8                                                  
                                    #define WEBCAM_PACKET_SIZE               90       
                                  
                                  // Codici comando
                                  
                                    #define WEBCAM_MAGIC                     HOSTFS_MAGIC
        
                                    #define CMD_HELLO_IMAGESERVER            0x0001
                                    
                                            #define CMD_RESPONSE_TO_HELLO    0xF001
                                    
                                    #define CMD_START_NEW_IMAGE              0x0002
                                    
                                    #define CMD_SEND_ME_A_TILESBLOCK         0x0003
                                    
                                    #define CMD_CHANGE_RESOLUTION            0x0004
                                    
                                            #define CMD_RESPONSE_TO_CHG_RES  0xF002
                                            
                                    #define CMD_CHANGE_TRANSFER_MODE         0x0005
                                    
                                            #define CMD_RESPONSE_TO_CHG_TMODE 0xF003
                                            
                                    #define CMD_GET_INFO                     0x0006
                
                                            #define CMD_RESPONSE_TO_GET_INFO  0xF004
                                    
                                  // Codici messaggio
                                  
                                    #define MSG_OK                           0x0001 
                                    #define MSG_IMPOSSIBLE_TO_USE_WEBCAM     0x0002
                                 
                                  
                                  /*   ---------------------------------------------------------
                                       PACCHETTI PER LA COMUNICAZIONE CON EYESERVER
                                       --------------------------------------------------------- */
                                    
                                    struct ndPixelElement_Type
                                    {
                                           unsigned char Red, Green, Blue;
                                    };
                                    
                                    struct ndUsbWCPacket_Type
                                    {
                                           struct HostFsCmd                cmd;
                                           int                             Message;
                                           int                             NrImage;
                                           int                             Value [8];
                                    };
                    
                                    struct ndUsbWCPacket24_Type
                                    {
                                           struct HostFsCmd                cmd;
                                           int                             Message;
                                           int                             NrImage;
                       
                                           struct ndPixelElement_Type      Pixel     [WEBCAM_PACKET_SIZE][WEBCAM_TILE_DIMY][WEBCAM_TILE_DIMX];
                                           int                             NextTileX [WEBCAM_PACKET_SIZE];
                                           int                             NextTileY [WEBCAM_PACKET_SIZE]; 
                                    };

                    
                                    struct ndUsbWCPacket16_Type
                                    {
                                           struct HostFsCmd                cmd;
                                           int                             Message;
                                           int                             NrImage;
                                           
                                           unsigned short int              Pixel [WEBCAM_PACKET_SIZE][WEBCAM_TILE_DIMY][WEBCAM_TILE_DIMX];
                                           int                             NextTileX [WEBCAM_PACKET_SIZE];
                                           int                             NextTileY [WEBCAM_PACKET_SIZE]; 
                                    };
                    
                                    struct ndUsbWCPacket8_Type
                                    {
                                           struct HostFsCmd                cmd;
                                           int                             Message;
                                           int                             NrImage;
                                           
                                           unsigned char                   Pixel [WEBCAM_PACKET_SIZE][WEBCAM_TILE_DIMY][WEBCAM_TILE_DIMX];
                                           int                             NextTileX [WEBCAM_PACKET_SIZE];
                                           int                             NextTileY [WEBCAM_PACKET_SIZE]; 
                                    };                    
                                    
                                    
                                    
                                    NDEXTERN struct ndUsbWCPacket24_Type  UsbWCPacketResp24;
                                    NDEXTERN struct ndUsbWCPacket16_Type  UsbWCPacketResp16;
                                    NDEXTERN struct ndUsbWCPacket8_Type   UsbWCPacketResp8;

                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                        struct ndUsbWCPacket_Type    __attribute__((aligned(16))) UsbWCPacketRq;
                                        struct ndUsbWCPacket_Type    __attribute__((aligned(16))) UsbWCPacketResp;
                                    #else    
                                        NDEXTERN struct ndUsbWCPacket_Type    UsbWCPacketRq;
                                        NDEXTERN struct ndUsbWCPacket_Type    UsbWCPacketResp;
                                    #endif
                                    
                                    /*
                                       ---------------------------------------------------------
                                       ---------------------------------------------------------
                                       SUPPORTO SONY GO-CAM
                                       ---------------------------------------------------------
                                       ---------------------------------------------------------
                                    */
                                    
                                       // Codice di identificazione della GoCam
                                       
                                       #define GOCAM_VENDOR_ID       0x054D
                                       #define GOCAM_PRODUCT_ID      0x0001
                                
                                       // Codici di errore
                                    
                                       #define ND_FAILURE_TO_LOAD_USBACC          0x00101
                                       #define ND_FAILURE_TO_LOAD_USBMODULECAM    0x00102
                                       #define ND_FAILURE_TO_INIT_USBACC          0x00103
                                       #define ND_FAILURE_TO_INIT_USBCAMDRV       0x00104
                                       #define ND_FAILURE_TO_INIT_USBCAMMICDRV    0x00105
                                       #define ND_FAILURE_TO_ACTIVATE_NEW_CAMERA  0x00106
                                       
                                       NDEXTERN char ndGoCamIsFailure; 
                                       NDEXTERN int  ndGoCamFailureState;
                                       NDEXTERN int  ndGoCamSecondaryErrorCode;
                                    
                                       // Profondità di caricamento
                                  
                                       NDEXTERN char ndGoCamLoadDepth;
                                 
                                       // Simboli per inizializzazione e gestione dispositivo

                                       #define MAX_VIDEO_FRAME_JPG_SIZE	    (32*1024)
                                       #define MAX_VIDEO_FRAME_DECOMP_SIZE	(512*1024)
                                       #define FRAMEBUFFER_SIZE              1024*768
                                        
                                       NDEXTERN char *CAM_JpegBuffer;
                                       NDEXTERN u32  *CAM_FrameBuffer;
                                       NDEXTERN u8   *CAM_WorkAreaDriver;
                                        
                                       #define WORK_AREA_DRIVER_SIZE       68*1024

                                       NDEXTERN int   CAM_HwResCode;
                                       
                                    /*
                                       ---------------------------------------------------------
                                       ---------------------------------------------------------
                                       COSTANTI MANIFESTE PER L'API WEBCAM
                                       ---------------------------------------------------------
                                       ---------------------------------------------------------
                                    */
                                    
                                    // Codice per forzare l'uso della GoCam
                                    
                                    #define ND_USE_PSP_GOCAM         0x00010000
                                    
                                    // Codice per forzare l'uso di Eyeserver
                                    
                                    #define ND_USE_PSP_EYESVR        0x00000001
                                    
                                    // Codici target per la funzione  ndHAL_CAM_GrabNewImage
                                  
                                    #define ND_CAM_TO_NULL             0 
                                    #define ND_CAM_TO_WINDOW           1
                                    #define ND_CAM_TO_BASESCREEN       2
                                    #define ND_CAM_TO_NDIMAGE_STRUCT   3 
                                    #define ND_CAM_TO_CVIMAGE          4          
                                  
                                    // Costanti per il parametro Options di ndHAL_CAM_GrabNewImage
                                    
                                    #define ND_FORCECAM          0x800000   // bit 23
                                    
                                    // Codici per il trasmission mode
                                    
                                    #define ND_RGB_TRASMISSION_MODE      24
                                    #define ND_RGB555_TRASMISSION_MODE   16
                                    #define ND_GRAY_TRASMISSION_MODE      8
                                    
                                    // Codici per modalità videocamera
                                    
                                    #define ND_USBCAM_EFFECTMODE_NORMAL      0
	                                #define ND_USBCAM_EFFECTMODE_NEGATIVE    1
	                                #define ND_USBCAM_EFFECTMODE_BLACKWHITE  2
	                                #define ND_USBCAM_EFFECTMODE_SEPIA       3
	                                #define ND_USBCAM_EFFECTMODE_BLUE        4
	                                #define ND_USBCAM_EFFECTMODE_RED         5
	                                #define ND_USBCAM_EFFECTMODE_GREEN       6
                                    
                                    /*
                                       ----------------------------------------------------------
                                       EXIT MANAGER
                                       ----------------------------------------------------------
                                    */
                                    
                                    NDEXTERN int  ExitController_ThreadID;
                                    NDEXTERN char ExitController_Exit;
                                    
                                    NDEXTERN struct SceKernelLoadExecVSHParam ndLoadExecStdStruct;
                                    
                                    NDEXTERN const char   ndTransExecutionFileName [128];
                                    
                                    
                                    /*
                                       ----------------------------------------------------------
                                       SUPPORTO XFPU
                                       ----------------------------------------------------------
                                    */
                                    
                                    struct ndXFPU_Matrix_Type
                                    {
                                           float value [5][5];
                                    };
                                    
                                    NDEXTERN struct ndXFPU_Matrix_Type XFPUMatrix [7];
                                    NDEXTERN float  CarryIn, CarryOut;
                                    
                                    /*
                                       ----------------------------------------------------------
                                       NETWORK (low level)
                                       ----------------------------------------------------------
                                    */
                                    
                                    NDEXTERN int ndNetInit_IsFailure;
                                  
                                    NDEXTERN int ndNetInit_Failure_MainErrCode;
                                    NDEXTERN int ndNetInit_Failure_SecErrCode;
                                    
                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                        char ndNetworkStateIsChanged = 0;
                                        int  ndNrNetworkConnectionsActive = 0;
                                    #else
                                        NDEXTERN char ndNetworkStateIsChanged;
                                        NDEXTERN int  ndNrNetworkConnectionsActive;
                                    #endif 
                                    
                                    
                                    
                                    /*
                                       ----------------------------------------------------------
                                       NETWORK (API ad alto livello)
                                       ----------------------------------------------------------
                                    */
                                    
                                    // La struttura sockaddr è definita in sys/socket.h
                                    // e la struttura sockaddr_in è definita in 
                                    // netinet/in.h
                                    
                                    
                                    
                                    // Union per la gestione di indirizzi IPv4
                                    
                                    union ndIPV4
                                    {
                                          int           Code32;
                                          unsigned char Value [4];
                                    };
                                                                
                                    // Altre variabili
                                    
                                    NDEXTERN int ndNetLayer_KernelThreadForInit;
                                    
                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                        int ndNetLayer_HasBeenInit = 0;
                                    #else
                                        NDEXTERN int ndNetLayer_HasBeenInit;
                                    #endif 
                                    
                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                        char ndNetMonitorEnabled = 0;
                                    #else
                                        NDEXTERN char ndNetMonitorEnabled;
                                    #endif 
                                    
                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                        int ndStdNetworkTimeOut = 6000000;
                                    #else
                                        NDEXTERN int ndStdNetworkTimeOut;
                                    #endif 
                                    
                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES   
                                        char ndNetworkMonitorIsOpen = 0;
                                    #else
                                        NDEXTERN char ndNetworkMonitorIsOpen;
                                    #endif 
                                    
                                    NDEXTERN char ndNetworkMonitorHandle;
                                    
                                    struct ndNetworkObject_PSP_Type
                                    {
                                           int  NrConfigInFirmw;
                                           char WndDialogBox0_IsOpen;
                                           char WndDialogBox0_Handle;
                                           char WndDialogBox1_IsOpen;
                                           char WndDialogBox1_Handle;
                                           char BtnHandle0, BtnHandle1;
                                           int  Ok_Switcher;
                                           
                                           char  Name [128];
                                           char  SSID [64];
                                           char  SECURE [64];
                                           char  WEPKEY [64];
                                           char  IS_STATIC_IP;
                                           
                                           union ndIPV4 IP;
                                           union ndIPV4 NETMASK;
                                           
                                           union ndIPV4 ROUTE;
                                           union ndIPV4 PRIMARY_DNS;
                                           union ndIPV4 SECONDARY_DNS;
                                           
                                           char PROXY_USER [128];
                                           char PROXY_PASS [128];
                                           
                                           unsigned char USE_PROXY;
                                           
                                           union ndIPV4 PROXY_SERVER;
                                           unsigned short PROXY_PORT;
                                    };
                                    
                                    struct ndNetworkObject_Type
                                    {
                                           struct ndNetworkObject_PSP_Type Psp;
                                           
                                           // Common data
                                           unsigned char IPIsKnown;
                                           unsigned char IPV4str [32];
                                           union ndIPV4  IPV4;
                                    };
                                    
                                    #define ND_SILENT_AP_ACCESS   1
                                    
                                    #define NET_ATTEMPT_TIMER    20
                                    
                                    /*
                                       ----------------------------------------------------------
                                       NETWORK (API STD)
                                       ----------------------------------------------------------
                                    */
                                    
                                    #ifdef HAL_I_NEED_TO_DECLARE_VARIABLES  
                                           int h_errno = NETDB_SUCCESS;
                                    #else
                                           NDEXTERN int h_errno;
                                    #endif
                                  
                                    /*
                                       ----------------------------------------------------------
                                       FIRMWARE MANAGER
                                       ----------------------------------------------------------
                                    */
                                    
                                    NDEXTERN char FwBuffer [256], FwBuffer0 [16], FwBuffer1 [10], FwBuffer2 [10];
                                    
                                    NDEXTERN int ndPspFwVersion;
                                    NDEXTERN int ndPspFwEmuVersion;
                                    NDEXTERN int ndIsPspSlim;
                                    
                                    // Codici firmware
                                    
                                    #define NDPSPFW_UNKNOWN 0x00000000
                                    #define NDPSPFW_SONY    0x00010000
                                    #define NDPSPFW_CF      0x00020000
                                    
                                    // Codici emulazione firmware
                                    
                                    #define NDPSPEMUFW100   0x01000300
                                    #define NDPSPEMUFW150   0x01050001
                                    
                                    /*
                                        ------------------------------------------------------------
                                        GESTORE AUDIO WAV
                                        ------------------------------------------------------------
                                    */
                                    
                                    
                                    // Configurazione del gestore audio wav
                                    
                                    #define SAMPLES_FOR_SUBBUFFER     16384
                                    #define NR_SUBBUFFER_FOR_BUFFER   8
                                    #define NR_BUFFER                 4
                                   
                                    #define NR_AUDIO_DESCRIPTORS      8
                                    
                                    // Stati del buffer 
                                    
                                    #define BUFFER_FREE_FOR_WRITE                          1
                                    #define BUFFER_WRITTEN_AND_READY_FOR_PLAY              2
                                    #define BUFFER_IS_BUSY_FOR_WRITE                       4
                                    #define BUFFER_IS_BUSY_FOR_READ                        8

                                    // Stati del thread di riproduzione
                                    
                                    #define SND_STATE_IDLE         1
                                    #define SND_STATE_PLAYING      2
                                    #define SND_STATE_COERCITIVE   4
                                    
                                    // Definizione delle strutture fondamentali
                                    
                                    struct AudioSingleSample_Type
                                    {
                                           unsigned short int l,r;
                                    };
                                    
                                    struct AudioBuffer_Type
                                    {
                                           struct AudioSingleSample_Type    Buffer [NR_BUFFER][NR_SUBBUFFER_FOR_BUFFER][SAMPLES_FOR_SUBBUFFER];
                                           int    State [NR_BUFFER];
                                           int    SubBuffersReady [NR_BUFFER];
                                    };

                                    struct ndAudioDescriptor_Type
                                    {
                                           char IsActive;
                                           struct AudioBuffer_Type *SoundData;
                                           char NrChannels;
                                           int  VFreq;
                                           int  NrBits;
                                           int  ChannelHdr;
                                           int  ThreadID;
                                           int  EventID;
                                           int  GeneralState;
                                           int  RD_SubBuffer;
                                           int  WR_SubBuffer;
                                           int  RD_Buffer;
                                           int  WR_Buffer;
                                           int  RD_Offset;
                                           int  WR_Offset;
                                           
                                           char CoercitiveStop;
                                           
                                           int  HW_VolumeDX;
                                           int  HW_VolumeSX;
                                    };
                                    
                                    NDEXTERN struct ndAudioDescriptor_Type ndAudioDescriptor [NR_AUDIO_DESCRIPTORS];
                                    
                                    // Opzioni per l'API di riproduzione
                                    
                                    #define ND_AUDIO_MONO   1
                                    #define ND_AUDIO_STEREO 2
                                    
                                    #define ND_AUDIO_8BIT   8
                                    #define ND_AUDIO_16BIT  16
                                    
                                    // Opzioni per l'API di riproduzione
                                    
                                    #define NO_CHECK_FREQUENCY_CORRECTNESS    1

                                    
                                    /*
                                        ------------------------------------------------------------
                                        GESTORE MEDIA
                                        ------------------------------------------------------------
                                    */
                                    
                                    // Dimensioni buffers
                                    
                                    #define  MP3_L1_BUFFER_SIZE  1152
                                    #define  WAV_L1_BUFFER_SIZE  4096 
                                    
                                    // Tipi media
                                    
                                    #define  MEDIA_TYPE_WAV    0
                                    #define  MEDIA_TYPE_MP3    1
                                    
                                    // Codici di stato del decoder MP3
                                    
                                    #define  MP3_STATE_IDLE     1
                                    #define  MP3_STATE_RESET    2
                                    #define  MP3_STATE_PLAYING  4
                                    #define  MP3_STATE_EOF      8
                                    #define  MP3_STATE_STOP     16
                                    #define  MP3_STATE_INT      32
                                    
                                    // Codici di stato del decoder WAV
                                    
                                    #define  WAV_STATE_IDLE     1
                                    #define  WAV_STATE_RESET    2
                                    #define  WAV_STATE_PLAYING  4
                                    #define  WAV_STATE_EOF      8
                                    #define  WAV_STATE_STOP     16
                                    #define  WAV_STATE_INT      32
                                    
                                    
                                    // Codici di errore del decoder
                                    
                                    #define  I_CANNOT_TELL_FILE_FEATURES    1
                                    #define  FILE_IS_NOT_COMPLETE           2
                                    #define  DECODER_EOF                    3
                                    #define  DECODER_ERROR                  4
                                    
                                    // Codici flags del decoder
                                    
                                    #define  FLAG_AUTOREWIND_ON             1
                                    
                                    
                                    // Struttura object media
                                    
                                    struct ndMediaObj_Type
                                    {
                                        char  NameFile [262];
                                        char  Type;
                                        
                                        int   ndSndChannel;
                                        
                                        int   DecThreadID;
                                        int   DecEventID;
                                        
                                        int   DecoderState; 
                                        int   old_DecoderState;
                                        
                                        int   DecoderError;
                                        int   DecoderExitRq;
                                        
                                        void*   CallBack;
                                        ndint64 InfoField;
                                        ndint64 Flags;
                                        
                                        int   NrTimesPlay;
                                        
                                        int   MediaHandle;   
                                        
                                        int                     MP3_Channels;            // 1 - mono, 2 - stereo
                                        int                     MP3_SampleRate;          // 8000..44100
                                        int                     MP3_HwSampleRate;        
                                        int                     MP3_HwSampleSize;
                                        int                     MP3_HwOutputMode;        // ND_MONO, ND_STEREO
                                        int                     MP3_SamplesForFrame;
                                        int                     MP3_FrameSize;
                                        int                     MP3_StereoMode;          // 0 Stereo, 1 Joint stereo, 2 Dual channel, 3 Mono
                                        int                     MP3_GetEDRAM;
                                        int                     MP3_Version;
                                        int                     MP3_Layer;
                                        int                     MP3_BitRateSeq;
                                        int                     MP3_BitRateIndex;
                                        int                     MP3_BitRate;
                                        int                     MP3_Padding;
                                        int                     MP3_FrameCostant;
                                        int                     MP3_SamplesDecoded;
                                        
                                        void                   *MP3_DataStart;
                                        unsigned long          *MP3_CommCodecArea;
                                        short int              *MP3_L1Buffer;
                                        char                   *MP3_FlexibleDataBuffer; 
                                        char                    MP3_FlexibleDataBufferCreated;
                                        int                     MP3_FlexibleDataBufferOldSize;
                                        
                                        int                     WAV_Channels;            // 1 - mono, 2 - stereo
                                        int                     WAV_SampleRate;          // 8000..44100
                                        int                     WAV_HwSampleRate;        
                                        int                     WAV_HwSampleSize;
                                        int                     WAV_HwOutputMode;        // ND_MONO, ND_STEREO
                                        int                     WAV_HwOutputBitRate;     // 8-16
                                        int                     WAV_BlockSize;
                                        int                     WAV_BitsForSample;
                                        int                     WAV_DataLength;
                                        int                     WAV_PosStartData;
                                        int                     WAV_SamplesDecoded;
                                        int                     WAV_SamplesRead;
                                        int                     WAV_SamplesInTotal;
                                        int                     WAV_HeaderError;
                                        
                                        void                    *WAV_L1Buffer;
                                        
                                    };
                                    
                                    // Opzioni per il player: PLAY
                                    
                                    #define ND_PLAY_AUTOREWIND          1
                                    #define ND_LOCK_UNTIL_SONG_FINISH   2
                                    
                                    /*
                                        ------------------------------------------------------------
                                        GESTORE JPEG IN HARDWARE
                                        ------------------------------------------------------------
                                    */
                                    
                                        #define ND_FAILURE_TO_INIT_JPEGDEC     0x0001
                                        #define ND_FAILURE_TO_CREATE_MJPEGBUF  0x0002
                                        #define ND_JPEGDEC_DISABLED_BY_USER    0x00FF 
                                    
                                        #define ND_JPEGDEC_NOACTOR             0x0000
                                        #define ND_JPEGDEC_GOCAM_SUPPORT       0x0001
                                    
                                        NDEXTERN char ndJpegDecoderIsFailure;
                                        NDEXTERN int  ndJpegDecoderState;
                                        NDEXTERN int  ndJpegDecoderSecondaryErrorCode;
                                        
                                        NDEXTERN int  ndJpegSoftwareEmulation;
                                        NDEXTERN int  ndJpegHardwareResCode;
                                        NDEXTERN int  ndJpegActualActor;                // Tiene traccia dell'utilizzatore attuale 
                                                                                        // del decoder JPEG hardware
                                    
                                        /*
                                           -----------------------------------------------------------------------
                                           CODICI DI RISOLUZIONE HARDWARE
                                           -----------------------------------------------------------------------
                                        */                                
                                                                                                      
                                        #define ND_RESOLUTION_CODE_160_120  0
                                    	#define ND_RESOLUTION_CODE_176_144  1
                                    	#define ND_RESOLUTION_CODE_320_240  2
                                    	#define ND_RESOLUTION_CODE_352_288  3
                                    	#define ND_RESOLUTION_CODE_640_480  4
                                    	#define ND_RESOLUTION_CODE_1024_768 5
                                    	#define ND_RESOLUTION_CODE_1280_960 6
                                    	#define ND_RESOLUTION_CODE_480_272  7
                                    	#define ND_RESOLUTION_CODE_360_272  8
                                                                        
                                    
                                    
                                    
                                    
                                    
                                    /*
                                        ------------------------------------------------------------
                                        KERNEL EXTENDER API
                                        ------------------------------------------------------------
                                    */
                                    
                                    NDEXTERN char ndKernelExtenderIsInError;
                                    
                                    
                                    
                                                      
     #undef NDEXTERN                            
  
#endif     // _NDHAL_VARIABLES_CONTAINER_
  
