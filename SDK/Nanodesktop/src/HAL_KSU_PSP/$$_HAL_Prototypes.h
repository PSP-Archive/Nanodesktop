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
    FILE SPECIALE DI DEFINIZIONE DEI PROTOTIPI PER HAL (PSP KSU mode)
    ---------------------------------------------------------------------------
    Questo file speciale può essere richiamato in due casi:
           a) dai moduli di Nanodesktop durante la fase di compilazione: in tal
              caso esso fornisce tutti i prototipi delle varie funzioni 
              presenti nei moduli dell'HAL di Nanodesktop. I moduli di
              solito richiamano questo file dopo avere definito la costante
              manifesta HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES, che rende 
              disponibili anche i prototipi delle routine INTERNAL
              
           b) da Nanodesktop.h durante la compilazione delle normali applicazioni.
              In tal caso i prototipi delle routines internal non saranno
              forniti al compilatore.
    --------------------------------------------------------------------------- 
*/

#ifndef _NDHAL_PROTOTYPES_CONTAINER_
      #define  _NDHAL_PROTOTYPES_CONTAINER_
      
      // Impedirà che questo file possa essere incluso due volte nella compilazione

                                
                                
                                
                                // ---------------------------------------------------------------
                                // Routines di Delay
                                // ---------------------------------------------------------------
                                
                                extern void ndHAL_SoftDelay (float DelaySec);
                                extern void ndHAL_HardDelay (float DelaySec); 
                                extern void ndHAL_ClockDelay (float DelaySec); 
                                extern void ndHAL_Delay (float DelaySec); 
                                
                                extern void ndHAL_RequestDelay (unsigned char DelayType);
                                
                                
                                /// --------------------------------------------------------------
                                /// Routines per l'inizializzazione dei componenti del sistema
                                /// --------------------------------------------------------------
                                
                                extern void ndHAL_InitComponents (void); 
                                
                                
                                
                                /// --------------------------------------------------------------
                                /// Routines per la gestione dei colori
                                /// --------------------------------------------------------------
                                
                                extern TypeColor ndHAL_RGBToMagicNumber (unsigned char ChannelR, unsigned char ChannelG, unsigned char ChannelB); 
                                extern void ndHAL_MagicNumberToRGB (TypeColor MagicNumber, unsigned char *ChannelR, unsigned char *ChannelG, unsigned char *ChannelB); 
                                extern TypeColor ndHAL_ConvertNDFtoMagicNumber (short unsigned int NDFValue); 
                                extern void ndHAL_ConvertNDFToRGB (short unsigned int NDFValue, unsigned char *ChannelR, unsigned char *ChannelG, unsigned char *ChannelB); 
                                extern TypeColor ndHAL_MathBlend (TypeColor Color1, TypeColor Color2); 
                                extern char ndHAL_NDF_Identical_To_MGKNB (void); 
                                
                                /// --------------------------------------------------------------
                                /// Routines per gestione grafica
                                /// --------------------------------------------------------------
                                
                                extern void ndHAL_InitGraph (void); 
                                
                                #define ndLL_PutPixel                         ndHAL_PutPixel
                                #define ndLL_GetPixel                         ndHAL_GetPixel
                                #define ndLL_PutPixelToPage                   ndHAL_PutPixelToPage
                                #define ndLL_GetPixelFromPage                 ndHAL_GetPixelFromPage
                                #define ndLL_ChangeFrameBufferPage            ndHAL_ChangeFrameBufferPage

                                extern void ndHAL_PutPixel (short unsigned int PosPixelX, short unsigned int PosPixelY, TypeColor Color); 
                                extern void ndHAL_ChangeFrameBufferPage (void); 
                                extern void ndHAL_PutPixelToPage (short unsigned int PosPixelX, short unsigned int PosPixelY, TypeColor Color, unsigned char Page); 
                                extern TypeColor ndHAL_GetPixel (short unsigned int PosPixelX, short unsigned int PosPixelY); 
                                extern TypeColor ndHAL_GetPixelFromPage (short unsigned int PosPixelX, short unsigned int PosPixelY, short unsigned int Page); 
                                
                                /// --------------------------------------------------------------
                                /// Routines per la gestione del controller
                                /// --------------------------------------------------------------
                                
                                extern char ndHAL_GetPad (struct ndDataControllerType *ndPadRecord); 
                                extern char ndHAL_GetPad_Analog (struct ndDataControllerType *ndPadRecord);
                                
                           #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                extern char CORE_GetPad (struct ndDataControllerType *ndPadRecord); 
                                extern char CORE_GetPadAnalog (struct ndDataControllerType *ndPadRecord);
                           #endif
                                
                                extern void ndHAL_WaitKey (int Param); 
                                
                                /// --------------------------------------------------------------
                                /// Routines ausiliarie per il file-system
                                /// --------------------------------------------------------------
                                
                                
                                extern void ndHAL_InitFileSystem (void); 
                                
                                extern int ndHAL_Flen (char *NameFile); 
                                extern int ndHAL_Fexist (char *NameFile); 
                                
                                extern int ndHAL_Open (char *file, int flags, int mode); 
                                extern int ndHAL_Close (int VFHNumber); 
                                extern int ndHAL_Read (int VFHNumber, void *Data, int Size); 
                                extern int ndHAL_Write (int VFHNumber, void *Data, int Size); 
                                extern int ndHAL_Lseek (int VFHNumber, int Offset, int Whence); 
                                extern int ndHAL_Tell (int VFHNumber); 
                                extern int ndHAL_EraseFile (const char *file); 
                                extern int ndHAL_MakeDir (const char *dir, int mode); 
                                extern int ndHAL_RemoveDir (const char *dir); 
                                extern int ndHAL_RenameFile (const char *oldname, const char *newname); 
                                extern FILE *ndHAL_Fopen (const char *fname, const char *mode); 
                                extern FILE *ndHAL_Fdopen (int fd, const char *mode); 
                                extern int ndHAL_Fclose (FILE *stream); 
                                extern int ndHAL_Fcloseall (void); 
                                extern size_t ndHAL_Fread (void *buf, int r, int n, FILE *stream); 
                                extern size_t ndHAL_Fwrite (const void *buf, size_t r, size_t n, FILE *stream); 
                                extern int ndHAL_Fgetc (FILE *stream); 
                                extern int ndHAL_Xgetc (FILE *stream); 
                                extern char *ndHAL_Fgets (char *buf, int n, FILE *stream); 
                                extern int ndHAL_Getc (FILE *stream); 
                                extern int ndHAL_Getchar (void); 
                                extern int ndHAL_Fputc (int c, FILE *stream); 
                                extern int ndHAL_Fputs (const char *s, FILE *stream); 
                                extern int ndHAL_Putc (int c, FILE *stream); 
                                extern int ndHAL_Puts (const char *s); 
                                extern int ndHAL_Putchar (int c); 
                                extern int ndHAL_Fseek (FILE *stream, long int offset, int origin); 
                                extern long int ndHAL_Ftell (FILE *stream); 
                                extern int ndHAL_Fgetpos (FILE *stream, fpos_t *pos); 
                                extern int ndHAL_Fsetpos (FILE *stream, fpos_t *pos); 
                                extern void ndHAL_rewind (FILE *stream); 
                                extern int ndHAL_Rename (const char *name, const char *newname); 
                                extern int ndHAL_Remove (const char *s); 
                                extern int ndHAL_Ungetc (int c, FILE *stream); 
                                extern char ndHAL_Tmpnam (char *name); 
                                extern FILE *ndHAL_Tmpfile (void); 
                                extern int ndHAL_Fileno (FILE *f); 
                                extern int ndHAL_Feof (FILE *stream); 
                                extern int ndHAL_Ferror (FILE *stream); 
                                extern char *ndHAL_Strerror (int err); 
                                extern void ndHAL_Perror (const char *s); 
                                extern void ndHAL_Clearerr (FILE *stream); 
                                extern int ndHAL_Fflush (FILE *stream); 
                                extern int ndHAL_Fflushall (void); 
                                extern int ndHAL_Setvbuf (FILE *Stream, char *Buf, int Mode, size_t BufSize); 
                                extern int ndHAL_Setbuf (FILE *Stream, char *Buf); 
                                extern int ndHAL_Devctl (const char *dev, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen); 
                                extern int ndHAL_Dopen (const char *fn); 
                                extern int ndHAL_Dread (SceUID fd, struct ndFS_DirEntry_Type *NdDir); 
                                extern int ndHAL_Dclose (int fd); 
                                
                                /// ----------------------------------------------------------------------------
                                /// Routines MouseControl, Event Manager, WMI Event Manager (versione NoPhoenix)
                                /// ----------------------------------------------------------------------------
                          
                          #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES   
                                
                                // WMI Event manager (No Phoenix)
                                
                                extern void INTERNAL_NOPHOENIX_ChangeL2WMIMenuOpenState (unsigned char WndHandle, char Value); 
                                extern void INTERNAL_NOPHOENIX_Scroll_WMI1_To_Right (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_Scroll_WMI1_To_Left (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_Scroll_WMI2_To_Up (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_Scroll_WMI2_To_Down (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_GoToPreviousWMIMenu (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_EnterToNew_WMI1 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ndEvent_Type *EventAddress, char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_CloseWMI2MenuOpened_Resolutive (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_CloseWMI2MenuOpened_Not_Resolutive (char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_OpenWMI2Menu (char NrWMI1, char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_Execute_WMI1 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ndEvent_Type *EventAddress, char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_Execute_WMI2 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, struct ndEvent_Type *EventAddress, char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_WMI1_Pressed (char ButtonFoundID, struct ndEvent_Type *EventAddress, char WndHandle); 
                                extern void INTERNAL_NOPHOENIX_WMI2_Pressed (char ButtonFoundID, struct ndEvent_Type *EventAddress, char WndHandle); 
                      
                                char INTERNAL_NOPHOENIX_WMIEventManager (unsigned short int MousePointer_RPosX, unsigned short int MousePointer_RPosY, char WindowFoundID, 
                                          struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState);
                      
                                // Event manager (NoPhoenix)
                                
                                inline void INTERNAL_NOPHOENIX_EventManager_For_Button1 (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState);
                                inline void INTERNAL_NOPHOENIX_EventManager_For_Button4 (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState);
                                inline void INTERNAL_NOPHOENIX_EventManager_For_Button5 (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState);
                                inline void INTERNAL_NOPHOENIX_NoEventManager           (struct ndEvent_Type *EventAddress, struct ndMouseState_Type *MouseState);

                                // Gestore locale per Mouse Control (NoPhoenix)
                                
                                void INTERNAL_NOPHOENIX_MouseControl (struct ndEvent_Type *EventAddress);
                                
                          #endif 
                                
                                /// ----------------------------------------------------------------------------
                                /// Routines MouseControl, Event Manager, WMI Event Manager (versione Phoenix)
                                /// ----------------------------------------------------------------------------
                          
                          #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES        
                                
                                 // Consegna messaggi
                                 
                                     extern void INTERNAL_PHOENIX_DispatchMessage (struct ndEvent_Type *MyEvent); 
                                 
                                 // Threads CBE
                                 
                                     extern void INTERNAL_PHOENIX_RegisterNewCBE (int CBEThread, char TypeThread); 
                                     extern void INTERNAL_PHOENIX_UnRegisterCBE (int CBEThread); 
                                     extern void INTERNAL_PHOENIX_LockForNewCBE (int CBEThread); 
                                     extern void INTERNAL_PHOENIX_UnLockForCBE (int CBEThread); 
                                     extern void INTERNAL_PHOENIX_SuspendAllCBEThreads (void); 
                                     extern void INTERNAL_PHOENIX_ResumeAllCBEThreads (void); 
                                     
                                 //static void INTERNAL_PHOENIX_ExecuteButtonCallback_L2 (SceSize args, void *argp); 
                                 //static void INTERNAL_PHOENIX_ExecuteButtonCallback_L1 (void *ButtonCallback, char *StringID, ndint64 InfoField, char WindowFoundID, char ButtonFoundID, char TraspCallback); 
                                 //static void INTERNAL_PHOENIX_ExecuteButtonCallback_L0 (void *ButtonCallback, char *StringID, ndint64 InfoField, char WindowFoundID, char ButtonFoundID, char TraspCallback); 
                                 //static void INTERNAL_PHOENIX_ExecuteCloseWndCallback_L2 (SceSize args, void *argp); 
                                 //static void INTERNAL_PHOENIX_ExecuteCloseWndCallback_L1 (void *CloseWndCallback, ndint64 WndInfoField, char WindowFoundID, char NoNotifyToMouseControl); 
                                 //static void INTERNAL_PHOENIX_ExecuteCloseWndCallback_L0 (void *CloseWndCallback, ndint64 WndInfoField, char WindowFoundID, char NoNotifyToMouseControl); 
                                 //static void INTERNAL_PHOENIX_ExecuteWMI1Callback_L2 (SceSize args, void *argp); 
                                 //static void INTERNAL_PHOENIX_ExecuteWMI1Callback_L1 (struct ELEMENT_L1WMI_Type *MyL1WMI, char WindowFoundID); 
                                 //static void INTERNAL_PHOENIX_ExecuteWMI1Callback_L0 (struct ELEMENT_L1WMI_Type *MyL1WMI, char WindowFoundID); 
                                 //static void INTERNAL_PHOENIX_ExecuteWMI2Callback_L2 (SceSize args, void *argp); 
                                 //static void INTERNAL_PHOENIX_ExecuteWMI2Callback_L1 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, char WindowFoundID); 
                                 //static void INTERNAL_PHOENIX_ExecuteWMI2Callback_L0 (struct ELEMENT_L1WMI_Type *MyL1WMI, struct ELEMENT_L2WMI_Type *MyL2WMI, char WindowFoundID); 
                                 
                                 // WMI Event manager (Phoenix)
                                 
                                     extern void INTERNAL_PHOENIX_ChangeL2WMIMenuOpenState (unsigned char WndHandle, char Value); 
                                     extern void INTERNAL_PHOENIX_Scroll_WMI1_To_Right (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_Scroll_WMI1_To_Left (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_Scroll_WMI2_To_Up (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_Scroll_WMI2_To_Down (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_GoToPreviousWMIMenu (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_EnterToNew_WMI1 (struct ELEMENT_L1WMI_Type *MyL1WMI, char WndHandle); 
                                     extern void INTERNAL_PHOENIX_CloseWMI2MenuOpened_Resolutive (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_CloseWMI2MenuOpened_Not_Resolutive (char WndHandle); 
                                     extern void INTERNAL_PHOENIX_OpenWMI2Menu (char NrWMI1, char WndHandle); 
                                     extern void INTERNAL_PHOENIX_WMI1_Pressed (char ButtonFoundID, char WndHandle); 
                                     extern void INTERNAL_PHOENIX_WMI2_Pressed (char ButtonFoundID, char WndHandle); 
                                     extern char INTERNAL_PHOENIX_WMIEventManager (short unsigned int MousePointer_RPosX, short unsigned int MousePointer_RPosY, char WindowFoundID, struct ndMouseState_Type *MouseState); 
                                 
                                 // Event Manager (Phoenix)
                                 
                                 extern void INTERNAL_PHOENIX_EventManager_For_Button1 (struct ndMouseState_Type *MouseState); 
                                 extern void INTERNAL_PHOENIX_EventManager_For_Button4 (struct ndMouseState_Type *MouseState); 
                                 extern void INTERNAL_PHOENIX_EventManager_For_Button5 (void); 
                                 extern void INTERNAL_PHOENIX_NoEventManager (void); 
                                 
                                 // API pubblica di MouseControl Client/Server
                                 
                                 extern void ndHAL_InitPhoenixMouseControl (void); 
                                 extern void INTERNAL_PHOENIX_MouseControl_SERVER (struct ndMouseState_Type *MyMouseState); 
                                 extern void INTERNAL_PHOENIX_MouseControl_CLIENT (struct ndEvent_Type *EventAddress); 

                          #endif        
                                
                                
                                
                                /// ---------------------------------------------------
                                /// Routines per gestione mouse
                                /// ---------------------------------------------------
                                
                                extern char ndHAL_GetMouseState (struct ndMouseState_Type *MouseState);
                                
                                // Routines grafiche per disegno/spostamento/cancellazione del mouse
                                extern void ndHAL_GrRegisterMouseBackGr (unsigned short int PosX, unsigned short int PosY, unsigned char Page);
                                extern void ndHAL_GrDrawMousePointer (unsigned short int PosX, unsigned short int PosY, unsigned char Page);
                                extern void ndHAL_GrHideMousePointer (unsigned short int PosX, unsigned short int PosY, unsigned char Page);
                                
                                // Wrappers per gestione mouse da parte della sezione CODE
                                extern inline char ndHAL_MoveMousePointerToPos (unsigned short int PosX, unsigned short int PosY);
                                extern inline void ndHAL_HideMousePointer ();
                                extern inline void ndHAL_ShowMousePointer ();

                                // Routines per l'abilitazione/disabilitazione del puntatore del mouse
                                extern void ndHAL_EnableMousePointer ();
                                extern void ndHAL_DisableMousePointer ();
                                
                                extern void ndHAL_MouseControl (struct ndEvent_Type *EventAddress);
                                
                                
                                
                                /// ---------------------------------------------------
                                /// Routines per funzioni di sistema
                                /// ---------------------------------------------------
                                
                                extern void ndHAL_SystemHalt (int ExitCode); 
                                extern int  ndHAL_SystemExec (char *Name);
                                
                                /// ----------------------------------------------------
                                /// Routines per gestione finestre
                                /// ----------------------------------------------------
                                
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES   
                                extern void UShort_Swap (short unsigned int *, short unsigned int *);
                         #endif       
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                extern unsigned char INTERNAL_IsPixelInWindow (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char WndHandle);
                                extern void INTERNAL_FindOwnerOfPixel (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char *OwnerID);
                                extern void INTERNAL_FindTwoOwnersOfPixel (unsigned short int PosPixelX, unsigned short int PosPixelY, unsigned char *OwnerID1, unsigned char *OwnerID2);
                                
                                extern void INTERNAL_DrawMinWindowsBar ();
                                
                                extern void INTERNAL_FatalSystemErrorInWindowRender (void); 
                                extern void INTERNAL_ReInitVertexBuffer (void); 
                                extern void INTERNAL_PushElemToXVertexBuffer (short unsigned int X); 
                                extern void INTERNAL_PushElemToYVertexBuffer (short unsigned int Y); 
                                extern void INTERNAL_PushRectToVertexBuffer (short unsigned int x1, short unsigned int y1, short unsigned int x2, short unsigned int y2); 
                                extern void INTERNAL_OrderXVertexBuffer (void); 
                                extern void INTERNAL_OrderYVertexBuffer (void); 
                                extern void INTERNAL_RemakeNanoTileMatrix (void); 
                                extern void INTERNAL_RemakeOwnerPixelMatrixForContours (void);
                         #endif
                                 
                                extern void ndHAL_UpdateWindowsRenderRegisters (void); 
                                extern void ndHAL_UpdateWindowsRenderRegisters_Core (void);
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                extern void INTERNAL_WindowsRender_NOTRASP_NOBACKGR (void); 
                                extern void INTERNAL_WindowsRender_NOTRASP_BACKGR (void); 
                                extern void INTERNAL_WindowsRender_TRASP_NOBACKGR (void); 
                                extern void INTERNAL_WindowsRender_TRASP_BACKGR (void); 
                         #endif
                                
                                extern void ndHAL_DoWindowsRender (void);
                                extern void ndHAL_DoWindowsRenderNoSwap (void);
                                extern void ndHAL_WindowsRender_CLIENT (void); 
                                
                                /// --------------------------------------------------------
                                /// Routines per acquisizione delle informazioni di sistema
                                /// --------------------------------------------------------
                                
                                extern ndint64 ndINFO_HAL_GetValue (int Code, int Param1, int Param2, int Param3); 
                                extern int     ndINFO_HAL_GetPlatformCode (void);
                                
                                /// --------------------------------------------------------
                                /// Sottosistema Phoenix Mouse
                                /// --------------------------------------------------------
                                
                                extern void ndHAL_StartPhoenixMouseThread (void);
                                extern void ndHAL_SuspendPhoenixMouseThread (void);
                                extern void ndHAL_ResumePhoenixMouseThread (void);
                                extern void ndHAL_MouseEmu (void);
                                extern void ndHAL_MouseControlledByAnalog (char IsOn); 
                                
                                /// --------------------------------------------------------
                                /// Sottosistema Phoenix Graph
                                /// --------------------------------------------------------
                               
                                // Routine usate periodicamente dal Phoenix Graph Thread che
                                // si occupano di visualizzare in ogni frame l'eventuale
                                // puntatore del mouse
                         
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES 
                                
                                extern void INTERNAL_ManageDrawMousePointer_NewFrame (void);
                                extern void INTERNAL_ManageDrawMousePointer_NoNewFrame (void);
                                extern void PhoenixGraph_SERVER (SceSize args, void *argp);
                                
                         #endif
                                
                                extern void ndHAL_StartPhoenixGraphThread (void);
                                extern void ndHAL_LockPhoenixGraphThread (void);
                                extern void ndHAL_UnlockPhoenixGraphThread (void);
                               
                                
                                /// --------------------------------------------------------
                                /// API generale per gestione tastiere
                                /// --------------------------------------------------------
                                
                                extern void ndHAL_InitKeyboards (void);
                                
                                extern char ndHAL_VKB_ThisKeybTypeIsAvailable (int Type);
                                
                                extern int ndHAL_VKB_AssignMeKeyboard (char *MessageToUser, int Type, ndint64 SystemOptions, ndint64 KeyboardStyle, 
                                              void *KeyCallback,   void *KeyCallbackData,
                                              void *EndCallback,   void *EndCallbackData,
                                              void *BreakCallback, void *BreakCallbackData);
                                
                                extern char ndHAL_VKB_DisplayInVKeyboard (char *NewMessageToUser, int VKBHandle, char RenderNow);
                                              
                                extern char ndHAL_VKB_DestroyKeyboard (char VKBHandle);
                                extern int  ndHAL_VKB_ReUseAnOpenedKeyboard (ndint64 SystemOptions, int VKBHandle);
                                extern int  ndHAL_VKB_GetASingleKey (char *MessageToUser, int Type);
                                
                                
                                /// --------------------------------------------------------
                                /// API Virtual Keyboard 1
                                /// --------------------------------------------------------
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES       
                                extern void INTERNAL_VKB1_ChangeCapsLockStatus (int VKBHandle);
                                extern void INTERNAL_VKB1_ChangeFuncLockStatus (int VKBHandle);
                                extern void INTERNAL_VKB1_SendKeyCode (int VKBHandle, int KeyCode);
                                extern ndint64 INTERNAL_VKB1_StdCodeManager (char *StringID, ndint64 InfoField, char WndHandle);
                                extern ndint64 INTERNAL_VKB1_AutoCloseKeyboard (char WndHandle, ndint64 KeybIncloseInfoField);
                                extern char INTERNAL_VKB1_DrawMessageDisplay (int VKBHandle);
                                extern char INTERNAL_CapsLockDisplay (int VKBHandle, char CapsLockOn);
                                extern char INTERNAL_FuncLockDisplay (int VKBHandle, char FuncLockOn);
                                extern char INTERNAL_VKB1_DrawFirstButtons (int VKBHandle, ndint64 KeyOptions);
                                extern char INTERNAL_VKB1_DrawSecondButtons (int VKBHandle, ndint64 KeyOptions);
                                extern char INTERNAL_VKB1_DrawThirdButtons (int VKBHandle, ndint64 KeyOptions);
                                extern char INTERNAL_VKB1_DrawFourthButtons (int VKBHandle, ndint64 KeyOptions);
                                extern int  INTERNAL_VKB1_DrawKeyboard (int VKBHandle, char CallbackBased);
                                extern int  INTERNAL_VKB1_StartKeyboard (int VKBHandle, ndint64 KeyboardStyle, char CallbackBased);
                                extern int  INTERNAL_VKB1_BeginReadCharLoop (int VKBHandle, char CallbackBased, char AskOnlyAKey, char NoKeybAutoClose);
                                extern char INTERNAL_VKB1_DisplayInVKeyboard (char *NewMessageToUser, int VKBHandle, char RenderNow);
                                extern char INTERNAL_VKB1_DestroyKeyboard (int VKBHandle);
                         #endif
                                
                               
                                /// --------------------------------------------------------
                                /// Routines per gestione Kernel services to User
                                /// --------------------------------------------------------
                                
                                extern void ndHAL_KSU_InhibitKSULoop (void);
                                extern void ndHAL_KSU_DeInhibitKSULoop (void);
                                
                                extern void ndHAL_KSU_InitLoopServices (void);
                                extern void ndHAL_KSU_ExecuteLoopServices (void);
                                extern void ndHAL_KSU_BootManager (void);
                                extern void ndHAL_KSU_BootScreen (void);
                                
                                         
                                
                                /// --------------------------------------------------------
                                /// Routines per Gestione Raw-IR
                                /// --------------------------------------------------------
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                void INTERNAL_Init_RawIrdaSystem ();
                         #endif
                                
                                extern char ndHAL_IR_EnableIRRawMonitor (void); 
                                extern void ndHAL_IR_DisableIRRawMonitor (void); 
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                extern char INTERNAL_ndHAL_SendRawIR_CLIENT (int Value, char NegLogic, char BitNumber, char MoreSignif, char Frequency, char SeqBitStart, int Start0, int Start0Len, int Start1, int Start1Len, int Dbit0_A, int Dbit0_A_Len, int Dbit0_B, int Dbit0_B_Len, int Dbit1_A, int Dbit1_A_Len, int Dbit1_B, int Dbit1_B_Len, char SeqBitStop, int Stop0, int Stop0Len, int Stop1, int Stop1Len, char MonitorEnabled); 
                                extern void INTERNAL_ndHAL_SendRawIR_SERVER ();
                         #endif
                                
                                extern char ndHAL_IR_EnableIrdaMonitor (void); 
                                extern void ndHAL_IR_DisableIrdaMonitor (void);
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                extern int INTERNAL_ndHAL_SendIRDA_CLIENT (int IrHandleChannel, int Address, int Number, char MonitorEnabled); 
                                extern int INTERNAL_ndHAL_ReceiveIRDA_CLIENT (int IrHandleChannel, int Address, int Number, char MonitorEnabled);
                         #endif
                                
                                /// ---------------------------------------------------------
                                /// Api per gestione IR
                                /// ---------------------------------------------------------
                                
                                extern void ndHAL_InitIrSystem (void);
                                 
                                extern int ndHAL_IR_AssignChannel (ndint64 Code); 
                                extern char ndHAL_IR_ConfigureChannel (int IrHandleChannel, char Mode, int AdrStruct); 
                                extern int ndHAL_IR_SendData (int IrHandleChannel, int AdrData, int Number); 
                                extern int ndHAL_IR_ReceiveData (int IrHandleChannel, int AdrData, int Number); 
                                extern char ndHAL_IR_ReleaseChannel (int IrHandleChannel); 
                              
                                /// ---------------------------------------------------------
                                /// Api per XFPU
                                /// ---------------------------------------------------------

                                
                                // Richiama il wrapper per le funzioni matematiche XFPU
                                
                                #include <.\HAL_KSU_PSP\$$_HAL_FpuWrapper.h>
                                
                                
                                /// ---------------------------------------------------------
                                /// Api per gestione USB (low level)
                                /// ---------------------------------------------------------

                                
                                #include <3rdparty_modules\USB_PRX_Driver\ndUSBdriver.h>
                         
                                
                         #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                extern void INTERNAL_ManagerUsbIcon (char Page);
                         
                                extern void INTERNAL_PrepareUsbCommArea (void);
                                extern void INTERNAL_Init_UsbSystem (void);
                                extern void INTERNAL_UnlockGraphicalSystemForUSB (void);
                                extern void INTERNAL_UsbWindowsRender (void);
                                extern void INTERNAL_TerminateUsbSystem (void);
                         #endif
                         
                                void ndHAL_USB_EnableDriverMonitor ();
                                void ndHAL_USB_DisableDriverMonitor ();
                                
                                /// ---------------------------------------------------------
                                /// Primitive di funzioni importate dal driver PRX
                                /// ---------------------------------------------------------
                                
                                void USB_PRX_PassCommAreaAdr (struct ndUsb_CommArea_Type *AdrArea);
                                
                                /// ---------------------------------------------------------
                                /// API per gestione Universal Serial Bus (high level)
                                /// ---------------------------------------------------------
                                
                                void ndHAL_InitUSBApi();
                                
                                int  ndHAL_USB_OpenDevice (struct usb_device *dev);
                                void ndHAL_USB_CloseDevice (int UsbHandle);
                                
                                
                                /// ---------------------------------------------------------
                                /// Attivazione/deattivazione USB (Device 0)
                                /// ---------------------------------------------------------
                                
                                int  USBDEV0_EYESVR_OpenDevice (void);
                                void USBDEV0_EYESVR_CloseDevice (void);
                                
                                /// ---------------------------------------------------------
                                /// Attivazione/deattivazione USB (Device 1)
                                /// ---------------------------------------------------------
                                
                                int  USBDEV1_GOCAM_OpenDevice  (void);
                                void USBDEV1_GOCAM_CloseDevice (void);
                                
                                /// ---------------------------------------------------------
                                /// API per la gestione Webcam USB Eyeserver
                                /// ---------------------------------------------------------
                                
                                #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                     extern char INTERNAL_EYESVR_ActivateCamera (void);
                                     extern char INTERNAL_EYESVR_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow); 
                                     extern char INTERNAL_EYESVR_ChangeResolution (int UsbHandleCam, int NewSizeX, int NewSizeY); 
                                     extern char INTERNAL_EYESVR_ChangeTransferMode (int UsbHandleCam, int NewMode); 
                                     extern char INTERNAL_EYESVR_GetInfo (int UsbHandleCam, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode); 
                                     
                                     extern char INTERNAL_EYESVR_ChangeHwResolution (int UsbHandleCamera, int NewMode);
                                     extern char INTERNAL_EYESVR_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness);
                                     extern char INTERNAL_EYESVR_ChangeHwContrast (int UsbHandleCamera, int NewContrast);
                                     extern char INTERNAL_EYESVR_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness);
                                     extern char INTERNAL_EYESVR_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel);
                                     extern int  INTERNAL_EYESVR_GetSaturationLevel (int UsbHandleCamera);
                                     extern int  INTERNAL_EYESVR_GetBrightness (int UsbHandleCamera);
                                     extern int  INTERNAL_EYESVR_GetContrast (int UsbHandleCamera);
                                     extern int  INTERNAL_EYESVR_GetSharpness (int UsbHandleCamera);
                                     extern int  INTERNAL_EYESVR_GetHwSensorMode (int UsbHandleCamera);
                                     extern int  INTERNAL_EYESVR_GetEvLevel (int UsbHandleCamera);
                                     
                                     extern char INTERNAL_EYESVR_ChangeSensorMode (int UsbHandleCamera, int NewMode);
                                     extern char INTERNAL_EYESVR_ActivateCamMicrophone (int WorkAreaAudioSize, int Frequency, int Gain);
                                     extern int  INTERNAL_EYESVR_ReadAudioBlock (void *Buffer, int NrBytesToRead);
                                     extern void INTERNAL_EYESVR_DeActivateCamMicrophone (void);
                                     extern char INTERNAL_EYESVR_StartAudioRecording (char *NameFile, int Frequency, int Gain);
                                     extern void INTERNAL_EYESVR_StopAudioRecording (void);
                                     extern void INTERNAL_EYESVR_DisableCamera (int UsbHandleCamera);
                                #endif
                                
                                /// ---------------------------------------------------------
                                /// API per la gestione GoCam
                                /// ---------------------------------------------------------
                                
                                
                                #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES       
                                     extern char INTERNAL_GOCAM_ActivateCamera (void);
                                     extern char INTERNAL_GOCAM_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow);
                                     extern char INTERNAL_GOCAM_ChangeResolution (int UsbHandleCam, int NewSizeX, int NewSizeY);
                                     extern char INTERNAL_GOCAM_ChangeTransferMode (int UsbHandleCam, int NewMode);                               
                                     extern char INTERNAL_GOCAM_GetInfo (int UsbHandleCam, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode);
                                     
                                     extern char INTERNAL_GOCAM_ChangeHwResolution (int UsbHandleCamera, int NewMode);
                                     extern char INTERNAL_GOCAM_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness);
                                     extern char INTERNAL_GOCAM_ChangeHwContrast (int UsbHandleCamera, int NewContrast);
                                     extern char INTERNAL_GOCAM_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness);
                                     extern char INTERNAL_GOCAM_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel);
                                     extern int  INTERNAL_GOCAM_GetSaturationLevel (int UsbHandleCamera);
                                     extern int  INTERNAL_GOCAM_GetBrightness (int UsbHandleCamera);
                                     extern int  INTERNAL_GOCAM_GetContrast (int UsbHandleCamera);
                                     extern int  INTERNAL_GOCAM_GetSharpness (int UsbHandleCamera);
                                     extern int  INTERNAL_GOCAM_GetHwSensorMode (int UsbHandleCamera);
                                     extern int  INTERNAL_GOCAM_GetEvLevel (int UsbHandleCamera);
                                     
                                     extern char INTERNAL_GOCAM_ChangeSensorMode (int UsbHandleCamera, int NewMode);
                                     extern char INTERNAL_GOCAM_ActivateCamMicrophone (int WorkAreaAudioSize, int Frequency, int Gain);
                                     extern int  INTERNAL_GOCAM_ReadAudioBlock (void *Buffer, int NrBytesToRead);
                                     extern void INTERNAL_GOCAM_DeActivateCamMicrophone (void);
                                     extern void INTERNAL_GOCAM_DisableCamera (int UsbHandleCamera);
                                     extern char INTERNAL_GOCAM_StartAudioRecording (char *NameFile, int Frequency, int Gain);
                                     extern void INTERNAL_GOCAM_StopAudioRecording (void);
                                     extern void INTERNAL_GOCAM_RegisterNewInfoAboutCameraImages (int _DimImgX, int _DimImgY, int _HwDimImgX, int _HwDimImgY, char _TrasmissionMode);
                                #endif
                                
                                /// ----------------------------------------------------------
                                /// API PER LA GESTIONE DELLA WEBCAM USB
                                /// ----------------------------------------------------------
                                
                                     extern int  ndHAL_CAM_ActivateCamera (int NrCamera); 
                                     extern char ndHAL_CAM_GrabNewImage (int UsbHandleCamera, char Target, int AuxValue, ndint64 Options, char RenderNow); 
                                     extern char ndHAL_CAM_ChangeResolution (int UsbHandleCamera, int NewSizeX, int NewSizeY); 
                                     extern char ndHAL_CAM_ChangeTransferMode (int UsbHandleCamera, int NewMode); 
                                     extern char ndHAL_CAM_ChangeSensorMode (int UsbHandleCamera, int NewMode);
                                     
                                     extern char ndHAL_CAM_ChangeHwResolution (int UsbHandleCamera, int NewMode);
                                     extern char ndHAL_CAM_ChangeHwBrightness (int UsbHandleCamera, int NewBrightness);
                                     extern char ndHAL_CAM_ChangeHwContrast (int UsbHandleCamera, int NewContrast);
                                     extern char ndHAL_CAM_ChangeHwSharpness (int UsbHandleCamera, int NewSharpness);
                                     extern char ndHAL_CAM_ChangeHwEvLevel (int UsbHandleCamera, int NewEvLevel);
                                     extern int  ndHAL_CAM_GetSaturationLevel (int UsbHandleCamera);
                                     extern int  ndHAL_CAM_GetBrightness (int UsbHandleCamera);
                                     extern int  ndHAL_CAM_GetContrast (int UsbHandleCamera);
                                     extern int  ndHAL_CAM_GetSharpness (int UsbHandleCamera);
                                     extern int  ndHAL_CAM_GetHwSensorMode (int UsbHandleCamera);
                                     extern int  ndHAL_CAM_GetEvLevel (int UsbHandleCamera);
                                
                                     extern char ndHAL_CAM_GetInfo (int UsbHandleCamera, int *DimImgX, int *DimImgY, int *FrameCounter, int *TransferMode); 
                                     extern char ndHAL_CAM_ActivateCamMicrophone (int UsbHandleCamera, int WorkAreaAudioSize, int Frequency, int Gain);
                                     extern int  ndHAL_CAM_ReadAudioBlock (int UsbHandleCamera, void *Buffer, int NrBytesToRead);
                                     extern void ndHAL_CAM_DeActivateCamMicrophone (int UsbHandleCamera);
                                     extern char ndHAL_CAM_StartAudioRecording (int UsbHandleCamera, char *NameFile, int Frequency, int Gain);
                                     extern void ndHAL_CAM_StopAudioRecording (int UsbHandleCamera);
                                     extern void ndHAL_CAM_DisableCamera (int UsbHandleCamera); 
  
                                /// ----------------------------------------------------------
                                /// FUNZIONI DI GESTIONE DI RETE A BASSO LIVELLO
                                /// ----------------------------------------------------------
                                
                                #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES  
                                       extern void ndHAL_InitLowLevelNetwork (void);
                                       extern void ndHAL_TerminateLowLevelNetwork (void);
                                       extern void INTERNAL_DrawNetworkIcon (TypeColor Colore, char Page);
                                       extern void INTERNAL_ManagerNetworkIcon (char Page);
                                       extern void INTERNAL_ChangeStateNetIcon_AddConnection ();
                                       extern void INTERNAL_ChangeStateNetIcon_SubConnection ();
                                #endif
                                
                                /// ----------------------------------------------------------
                                /// FUNZIONI DI GESTIONE DI RETE AD ALTO LIVELLO
                                /// ----------------------------------------------------------
                                
                                extern int ndHAL_WLAN_InitNetworkLayer ();
                                extern int ndHAL_WLAN_PSPLanSwitchIsOpen ();
                                
                                extern int ndHAL_WLAN_GetAccessPointInfo (int NrAccessPoint, struct ndNetworkObject_Type *NetObj);
                                
                                extern int ndHAL_WLAN_OpenPSPAccessPoint (char NrAccessPoint, struct ndNetworkObject_Type *NetObj, ndint64 Options, int TimeOut);
                                extern int ndHAL_WLAN_ClosePSPAccessPoint (char NrAccessPoint);
                                
                                
                                
                                /// ----------------------------------------------------------
                                /// FUNZIONI DI GESTIONE DI RETE STD
                                /// ----------------------------------------------------------
                                
                                 #include <HAL_KSU_PSP/$$_HAL_NetworkWrapper.h>
                                 
                                 extern in_addr_t ndHAL_NET_InetInetAddr (char *String); 
                                 extern char *ndHAL_NET_InetToNtoa (struct in_addr in); 
                                 
                                 extern void ndHAL_NET_ConvertIPv4ToString (union ndIPV4 *IPV4, char *String); 
                                 extern void ndHAL_NET_ConvertStringToIPv4 (char *String, union ndIPV4 *IPV4);
                                 
                                 extern void ndHAL_NET_Convert_SockAddr__to__SockAddrIn (struct sockaddr *address, struct sockaddr_in *address_in); 
                                 
                                 #ifdef HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES 
                                      extern void INTERNAL_ShowNetMonitorMessage (char Code, char *String, int Value1, int Value2, char *StrCustom); 
                                 #endif
                                 
                                 extern int  ndHAL_NET_SetupNoBlockingConnection (int s, u32 NoBlock); 
                                 extern void ndHAL_NET_SetupStdNetworkTimeOutValue (int Limit); 
                                 extern void ndHAL_NET_EnableNetworkMonitor (void); 
                                 extern void ndHAL_NET_DisableNetworkMonitor (void); 
                                 
                                 extern int ndHAL_NET_Accept (int s, struct sockaddr *sockaddr, int *addrlen); 
                                 extern int ndHAL_NET_Bind (int s, const struct sockaddr *addr, int namelen); 
                                 extern int ndHAL_NET_ConnectWithTimeOut (int s, const struct sockaddr *address, int addrlen, int timeout); 
                                 extern int ndHAL_NET_Connect (int s, const struct sockaddr *address, int addrlen); 
                                 extern int ndHAL_NET_GetSockOpt (int s, int level, int optname, void *optval, socklen_t *optlen); 
                                 extern int ndHAL_NET_Listen (int s, int backlog); 
                                 extern int ndHAL_NET_Recv (int s, u8 *buf, int len, int flags); 
                                 extern int ndHAL_NET_RecvFrom (int s, u8 *buf, int len, int flags, void *sockaddr_from, int *fromlen); 
                                 extern ssize_t ndHAL_NET_RecvMsg (int s, struct msghdr *msg, int flags); 
                                 extern int ndHAL_NET_Send (int s, const void *buf, int len, int flags); 
                                 extern int ndHAL_NET_SendTo (int s, const void *buf, int len, int flags, const struct sockaddr *sockaddr_to, int tolen); 
                                 extern ssize_t ndHAL_NET_SendMsg (int s, const struct msghdr *msg, int flags); 
                                 extern int ndHAL_NET_SetSockOpt (int s, int level, int optname, const void *optval, socklen_t optlen); 
                                 extern int ndHAL_NET_Shutdown (int s, int how); 
                                 extern int ndHAL_NET_Socket (int domain, int type, int protocol); 
                                 extern int ndHAL_NET_Close (int s); 
                                 extern int ndHAL_NET_Select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout); 
                                 extern int ndHAL_NET_GetErrno (void); 
                                 extern int ndHAL_NET_GetSockName (int s, struct sockaddr *name, socklen_t *namelen); 
                                 extern int ndHAL_NET_GetPeerName (int s, struct sockaddr *name, socklen_t *namelen); 
                                 
                                 extern int ndHAL_NET_RecvUntilFull (int s, u8 *buf, int len); 
                                 
                                 extern struct hostent *ndHAL_NET_GetHostByAddr (const void *addr, int len, int type); 
                                 extern struct hostent *ndHAL_NET_GetHostByName (const char *name); 
                                                                
                                /// ----------------------------------------------------------
                                /// EXIT MANAGER
                                /// ----------------------------------------------------------
                                
                                void ndHAL_InitExitController (void);
                                
                                /// ----------------------------------------------------------
                                /// FIRMWARE MANAGER
                                /// ----------------------------------------------------------
                                
                                void ndHAL_InitFirmwareCodes (void);
                                
                                
                                /// ------------------------------------------------------------
                                /// GESTORE AUDIO WAV
                                /// ------------------------------------------------------------
                                    
                                int ndHAL_SND_OpenChannel (int NrDevice, int Frequency, int NrBits, int NrChannels, int VolumeSx, int VolumeDx, ndint64 Options);
                                int ndHAL_SND_WriteToChannel (int NrChannel, void *samples, int NrSamples, float AmpFactor);
                                int ndHAL_SND_SetVolume (int NrChannel, int VolumeSx, int VolumeDx);
                                int ndHAL_SND_CloseChannel (int NrChannel);
                                int ndHAL_SND_FlushChannel (int NrChannel);
                                int ndHAL_SND_Beep (int NrChannel, int Tone);
                                int ndHAL_SND_CoercitiveStop (int NrChannel);
                                
                                
                                /// ----------------------------------------------------------
                                /// MEDIA SERVICE
                                /// ----------------------------------------------------------
                                
                                void ndHAL_InitMediaService (void);
                                
                                int  ndHAL_MEDIA_LoadFile (char *NameFile, int NrChannel, struct ndMediaObj_Type *ndMediaObj, void *CallBack, ndint64 InfoField);
                                int  ndHAL_MEDIA_Play (struct ndMediaObj_Type *ndMediaObj, ndint64 Options);
                                int  ndHAL_MEDIA_Pause (struct ndMediaObj_Type *ndMediaObj);
                                int  ndHAL_MEDIA_Stop (struct ndMediaObj_Type *ndMediaObj);
                                int  ndHAL_MEDIA_GetNrSamples (struct ndMediaObj_Type *ndMediaObj);
                                int  ndHAL_MEDIA_GoToSample (struct ndMediaObj_Type *ndMediaObj, int NrSample);
                                int  ndHAL_MEDIA_GoToAnIstant (struct ndMediaObj_Type *ndMediaObj, int HH, int MM, float SS);
                                int  ndHAL_MEDIA_UnLoad (struct ndMediaObj_Type *ndMediaObj);
                                char ndHAL_MEDIA_ThisFileIsSupported (char *NameFile);
                                
                                /// ----------------------------------------------------------
                                /// Compatibilità POSIX
                                /// ----------------------------------------------------------
                                
                                void ndHAL_InitPosixEngine ();
                                
                                /// ----------------------------------------------------------
                                /// Timer alimentazione
                                /// ----------------------------------------------------------
                                
                                
                                void ndHAL_PWR_SuspendTimerOn (void);
                                void ndHAL_PWR_SuspendTimerOff (void);
                                
                                /// ----------------------------------------------------------
                                /// MANAGER STATICO DEI COSTRUTTORI E DISTRUTTORI GLOBALI PER IL C++
                                /// ----------------------------------------------------------
                                
                                void ndHAL_ExecuteCPPGlobalConstructors ();
                                void ndHAL_ExecuteCPPGlobalDestructors ();
                                
                                /// ---------------------------------------------------------
                                /// Api per inizializzazione della piattaforma
                                /// ---------------------------------------------------------
                                
                                extern void ndHAL_InitPlatform_StepA (void); 
                                extern void ndHAL_InitPlatform_StepB (void); 
                                extern void ndHAL_InitPlatform_StepC (void); 
                               
                                /// ---------------------------------------------------------
                                /// Api del kernel extender
                                /// ---------------------------------------------------------

                                // La routine è presente solo per coerenza HAL tra le varie
                                // piattaforme
                                
                                void ndHAL_SignalTroublesInKernelExtender (void);
                                

#endif      // _NDHAL_PROTOTYPES_CONTAINER_


