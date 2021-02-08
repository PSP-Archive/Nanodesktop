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
    FILE SPECIALE DI DEFINIZIONE DELLE VARIABILI (Livello CODE)
    ---------------------------------------------------------------------------
    Questo file speciale può essere richiamato in due casi:
           a) dal modulo di inizializzazione ndCODE_Init.c di Nanodesktop 
              durante la compilazione della libreria. In tal caso, il 
              modulo deve prima definire la costante manifesta
              I_NEED_TO_DECLARE_VARIABLES. Questa costante manifesta farà
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

#ifndef _VARIABLES_CONTAINER_
      #define  _VARIABLES_CONTAINER_

      
      // Impedirà che questo file possa essere incluso due volte nella compilazione


        #ifdef I_NEED_TO_DECLARE_VARIABLES
               #define NDEXTERN             // Vuoto in modo che la variabile sia effettivamente dichiarata
        #else                               // Solo variabili extern
               #define NDEXTERN extern
        #endif



                                    
                                    
                                    /* 
                                       ----------------------------------------------------------------------------
                                       STRUTTURE PER LA GESTIONE DEI FONT
                                       ----------------------------------------------------------------------------
                                    */
                                    
                                    #ifdef ND_ENABLE_TTF_SUPPORT 
                                    
                                           NDEXTERN FT_Library TTF_Library;
                                    #endif   
                              
                                    
                                    
                                    #define NRFONTS_USABLE  254
                                    
                                    #define NDTEXTFONT    0
                                    #define NDGRAPHICFONT 1
                                    
                                    #define NDUSERFONT    0
                                    #define NDSYSTEMFONT  2
                                    
                                    #define NDDYNAMICFONT 0
                                    #define NDSTATICFONT  4
                                    
                                    
                                    struct FontHandle_Type
                                    {
                                        char IsActive;
                                        unsigned short int features;
                                        unsigned char *Data;
                                        
                                        char IsTrueType;
                                        
                                        #ifdef ND_ENABLE_TTF_SUPPORT 
                                           FT_Face TTF_Face;
                                           int     TTF_Size;
                                        #endif
                                    };
                                    
                                    /*
                                        Il campo Features è un campo a 16 bit che contiene informazioni sul tipo di
                                        font. Questo viene ottenuto mappando i singoli bit:
                                              
                                        0     Questo bit viene posto a 0 in caso di TextFont e viene posto a 1 in
                                              caso di GraphicFont
                                        
                                        1     Questo bit viene posto a 1 in caso di font di sistema 
                                              
                                        2     Questo bit viene posto a 1 in caso di font di tipo static e viene 
                                              posto a 0 in caso contrario
                                    */
                                        
                                    NDEXTERN struct FontHandle_Type FontHandlesArray [NRFONTS_USABLE];
                                    
                                    NDEXTERN char _FntHandle_Console;
                                    NDEXTERN char _FntHandle_Terminator;
                                    NDEXTERN char _FntHandle_SAsteroids1;
                                    NDEXTERN char _FntHandle_SAsteroids2;
                                    
                                    // Variabili che memorizzano i font correnti: altre sono presenti nella struct
                                    // WindowData
                                    
                                    NDEXTERN char ndLL_CurrentFntHandle;
                                    NDEXTERN char ndBS_CurrentFntHandle;
                                    
                                    
                                    
                                    /* 
                                       ----------------------------------------------------------------------------
                                       ELEMENTI GRAFICI
                                       ----------------------------------------------------------------------------
                                    */
                                    
                              #ifdef I_NEED_TO_DECLARE_VARIABLES   
                                     unsigned char MouseTheme [17][17] =
                                     { "BB..............",
                                       "BWBB............",
                                       ".BWWBB..........",
                                       ".BWWWWBB........",
                                       "..BWWWWWBB......",
                                       "..BWWWWWWWB.....",
                                       "...BWWWWWB......",
                                       "...BWWWWWB......",
                                       "....BWWWWWB.....",
                                       "....BWBBWWWB....",
                                       ".....B..BWWWB...",
                                       ".........BWWWB..",
                                       "..........BWWWB.",
                                       "...........BWB..",
                                       "............B...",
                                       "................" };
                              #else
                                   NDEXTERN unsigned char MouseTheme [17][17];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_Enlarge [9][22] =
                                    { "WWWWWBWWWWWWWWWBWWWWW",
                                      "WWWWBBWWWWWWWWWBBWWWW",
                                      "WWWBBBWWWWWWWWWBBBWWW",
                                      "WWBBBBBBBWWWBBBBBBBWW",
                                      "WWBBBBBBBWWWBBBBBBBWW",
                                      "WWWBBBWWWWWWWWWBBBWWW",
                                      "WWWWBBWWWWWWWWWBBWWWW",
                                      "WWWWWBWWWWWWWWWBWWWWW" };
                              #else
                                   NDEXTERN unsigned char Button_Enlarge [9][22];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_Smaller [9][22] =
                                    { "WWWWWBWWWWWWWWWBWWWWW",
                                      "WWWWWBBWWWWWWWBBWWWWW",
                                      "WWWWWBBBWWWWWBBBWWWWW",
                                      "WWBBBBBBBWWWBBBBBBBWW",
                                      "WWBBBBBBBWWWBBBBBBBWW",
                                      "WWWWWBBBWWWWWBBBWWWWW",
                                      "WWWWWBBWWWWWWWBBWWWWW",
                                      "WWWWWBWWWWWWWWWBWWWWW" };
                              #else
                                   NDEXTERN unsigned char Button_Smaller [9][22];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_CloseWindow [10][11] =
                                    {
                                      "BBBBBBBBBB",
                                      "BRRRRRRRRB",
                                      "BRWRRRRWRB",
                                      "BRRWRRWRRB",
                                      "BRRRWWRRRB",
                                      "BRRRWWRRRB",
                                      "BRRWRRWRRB",
                                      "BRWRRRRWRB",
                                      "BRRRRRRRRB",
                                      "BBBBBBBBBB" };
                              #else
                                    NDEXTERN unsigned char Button_CloseWindow [10][11];
                              #endif
                              
                                   
                              #ifdef I_NEED_TO_DECLARE_VARIABLES     
                                    unsigned char Button_MinimizeWindow [10][11] =
                                    {
                                      "BBBBBBBBBB",
                                      "BLLLLLLLLB",
                                      "BLLLLLLLLB",
                                      "BLLLLLLLLB",
                                      "BLLLLLLLLB",
                                      "BLBBBBBBLB",
                                      "BLBBBBBBLB",
                                      "BLBBBBBBLB",
                                      "BLLLLLLLLB",
                                      "BBBBBBBBBB" };
                              #else
                                    NDEXTERN unsigned char Button_MinimizeWindow [10][11];
                              #endif
                              
                                    
                              #ifdef I_NEED_TO_DECLARE_VARIABLES       
                                    unsigned char Button_ScrollUp [9][10] =
                                    {
                                      ".........",
                                      "....x....",
                                      "...xxx...",
                                      "..xxxxx..",
                                      ".xxxxxxx.",
                                      "...xxx...",
                                      "...xxx...",
                                      "...xxx..." };
                              #else
                                    NDEXTERN unsigned char Button_ScrollUp [9][10];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_ScrollDown [9][10] =
                                    {
                                      ".........",
                                      "...xxx...",
                                      "...xxx...",
                                      "...xxx...",
                                      ".xxxxxxx.",
                                      "..xxxxx..",
                                      "...xxx...",
                                      "....x...."};
                              #else
                                   NDEXTERN unsigned char Button_ScrollDown [9][10];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_ScrollLeft [9][10] =
                                    {
                                      ".........",
                                      "...x.....",
                                      "..xx.....",
                                      ".xxxxxxx.",
                                      ".xxxxxxx.",
                                      "..xx.....",
                                      "...x.....",
                                      "........." };
                              #else
                                    NDEXTERN unsigned char Button_ScrollLeft [9][10];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_ScrollRight [9][10] =
                                    {
                                      ".........",
                                      ".....x...",
                                      ".....xx..",
                                      ".xxxxxxx.",
                                      ".xxxxxxx.",
                                      ".....xx..",
                                      ".....x...",
                                      "........." };
                              #else
                                    NDEXTERN unsigned char Button_ScrollRight [9][10];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES     
                                    unsigned char Button_PrevMenu [9][10] =
                                    {
                                      ".........",
                                      "....xxx..",
                                      "..xxxxx..",
                                      ".xxxxxx..",
                                      "xxxxxxx..",
                                      ".xxxxxx..",
                                      "..xxxxx..",
                                      "....xxx.." };
                              #else
                                    NDEXTERN unsigned char Button_PrevMenu [9][10];
                              #endif
                              
                                   
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned char Button_Cross [18][19] =
                                    {
                                      ".......WWWW.......",
                                      ".....WWggggWW.....",
                                      "...WWggggggggWW...",
                                      "..WggggggggggggW..",
                                      "..WgGGggggggGGgW..",
                                      ".WgggGGggggGGgggW.",
                                      ".WggggGGggGGggggW.",
                                      "WggggggGGGGggggggW",
                                      "WgggggggGGgggggggW",
                                      "WggggggGGGGggggggW",
                                      ".WggggGGggGGggggW.",
                                      ".WgggGGggggGGgggW.",
                                      "..WgGGggggggGGgW..",
                                      "..WggggggggggggW..",
                                      "...WWggggggggWW...",
                                      ".....WWggggWW.....",
                                      ".......WWWW......."};
                              #else
                                    NDEXTERN unsigned char Button_Cross [18][19];
                              #endif
                                    
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                    unsigned char Button_Square [18][19] =
                                    {
                                      ".......WWWW.......",
                                      ".....WWggggWW.....",
                                      "...WWggggggggWW...",
                                      "..WggggggggggggW..",
                                      "..WggYYYYYYYYggW..",
                                      ".WgggYYYYYYYYgggW.",
                                      ".WgggYYggggYYgggW.",
                                      "WggggYYggggYYggggW",
                                      "WggggYYggggYYggggW",
                                      "WggggYYggggYYggggW",
                                      ".WgggYYggggYYgggW.",
                                      ".WgggYYYYYYYYgggW.",
                                      "..WggYYYYYYYYggW..",
                                      "..WggggggggggggW..",
                                      "...WWggggggggWW...",
                                      ".....WWggggWW.....",
                                      ".......WWWW......."};
                              #else
                                   NDEXTERN unsigned char Button_Square [18][19];
                              #endif
                                    
                                    
                              #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                    unsigned char Button_Circle [18][19] =
                                    {
                                      ".......WWWW.......",
                                      ".....WWggggWW.....",
                                      "...WWggggggggWW...",
                                      "..WggggggggggggW..",
                                      "..WgggBBBBBBgggW..",
                                      ".WgggBBBBBBBBgggW.",
                                      ".WggBBBggggBBBggW.",
                                      "WgggBBBggggBBBgggW",
                                      "WgggBBBggggBBBgggW",
                                      "WgggBBBggggBBBgggW",
                                      ".WggBBBggggBBBggW.",
                                      ".WgggBBBBBBBBgggW.",
                                      "..WgggBBBBBBgggW..",
                                      "..WggggggggggggW..",
                                      "...WWggggggggWW...",
                                      ".....WWggggWW.....",
                                      ".......WWWW......."};
                              #else
                                   NDEXTERN unsigned char Button_Circle [18][19];
                              #endif
                              
                               
                              #ifdef I_NEED_TO_DECLARE_VARIABLES      
                                    unsigned char Button_Triangle [18][19] =
                                    {
                                      ".......WWWW.......",
                                      ".....WWggggWW.....",
                                      "...WWggggggggWW...",
                                      "..WggggggggggggW..",
                                      "..WgggggRRgggggW..",
                                      ".WggggggRRggggggW.",
                                      ".WggggRRggRRggggW.",
                                      "WgggggRRggRRgggggW",
                                      "WgggRRggggggRRgggW",
                                      "WgggRRggggggRRgggW",
                                      ".WggRRRRRRRRRRggW.",
                                      ".WggRRRRRRRRRRggW.",
                                      "..WggggggggggggW..",
                                      "..WggggggggggggW..",
                                      "...WWggggggggWW...",
                                      ".....WWggggWW.....",
                                      ".......WWWW......."};
                              #else
                                   NDEXTERN unsigned char Button_Triangle [18][19];
                              #endif
                              
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES      
                                    unsigned char Button_Select [18][19] =
                                    {
                                      ".......WWWW.......",
                                      ".....WWbbbbWW.....",
                                      "...WWbbbbbbbbWW...",
                                      "..WbbbbbbbbbbbbW..",
                                      "..WbbbbbbbbbbbbW..",
                                      ".WbbbWWbWWWbWbbbW.",
                                      ".WbbWbbbWbbbWbbbW.",
                                      "WbbbWbbbWbbbWbbbbW",
                                      "WbbbbWbbWWbbWbbbbW",
                                      "WbbbbbWbWbbbWbbbbW",
                                      ".WbbbbWbWbbbWbbbW.",
                                      ".WbbWWbbWWWbWWWbW.",
                                      "..WbbbbbbbbbbbbW..",
                                      "..WbbbbbbbbbbbbW..",
                                      "...WWbbbbbbbbWW...",
                                      ".....WWbbbbWW.....",
                                      ".......WWWW......."};
                              #else
                                   NDEXTERN unsigned char Button_Select [18][19];
                              #endif
                                      
                                          
                              #ifdef I_NEED_TO_DECLARE_VARIABLES        
                                    unsigned char CapsLockOn [8][40] =
                                    {
                                      "......................................",
                                      "..WWWW.WWWW.WWWW.WWWW.......WWWW.WW.W.",
                                      "..W....W..W.W..W.W..........W..W.W.WW.",
                                      "..W....WWWW.WWWW.WWWW.......W..W.W..W.",
                                      "..W....W..W.W.......W.......W..W.W..W.",
                                      "..WWWW.W..W.W....WWWW.......WWWW.W..W.",
                                      "......................................"
                                    };
                              #else
                                    NDEXTERN unsigned char CapsLockOn [8][40];
                              #endif

                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES       
                                    unsigned char CapsLockOff [8][50] =
                                    {
                                      "...........................................",
                                      "..WWWW.WWWW.WWWW.WWWW.......WWWW.WWWW.WWWW.",
                                      "..W....W..W.W..W.W..........W..W.W....W....",
                                      "..W....WWWW.WWWW.WWWW.......W..W.WWW..WWW..",
                                      "..W....W..W.W.......W.......W..W.W....W....",
                                      "..WWWW.W..W.W....WWWW.......WWWW.W....W....",
                                      "..........................................."
                                    };
                              #else
                                    NDEXTERN unsigned char CapsLockOff [8][50];
                              #endif
                                    
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                     unsigned char TheUsbIcon [12][40] =
                                     {
                                       "                                       ",
                                       "   XX     XX   XXXXX   XXXXX           ",
                                       "   XX     XX  XXXXXXX  XXXXXXX         ",
                                       "   XX     XX XX     XX XX    XX        ",
                                       "   XX     XX XX        XX    XX        ",
                                       "   XX     XX  XXXXXXX  XXXXXXX         ",
                                       "   XX     XX        XX XX    XX        ",
                                       "   XXX   XXX XX     XX XX    XX        ",
                                       "    XXXXXXX   XXXXXXX  XXXXXXX         ",
                                       "     XXXXX     XXXXX   XXXXX           ",
                                       "                                       "
                                     };
                              #else
                                     NDEXTERN unsigned char TheUsbIcon [12][40];
                              #endif
           
                              
                              #ifdef I_NEED_TO_DECLARE_VARIABLES
                                     unsigned char TheNetworkIcon [12][40] =
                                     {
                                       "                                       ",
                                       "   XX     XX XXXXXXXX XXXXXXXX        ",
                                       "   XXX    XX XXXXXXX  XXXXXXXX        ",
                                       "   XXXX   XX XX          XX           ",
                                       "   XX XX  XX XX          XX           ",
                                       "   XX  XX XX XXXXXX      XX           ",
                                       "   XX   XXXX XX          XX           ",
                                       "   XX    XXX XX          XX           ",
                                       "   XX     XX XXXXXXX     XX           ",
                                       "   XX     XX XXXXXXXX    XX           ",
                                       "                                       "
                                     };
                              #else
                                     NDEXTERN unsigned char TheNetworkIcon [12][40];
                              #endif
                 
                                    
                                    
                                    /* 
                                       ----------------------------------------------------------------------------
                                       DEFINIZIONE DIMENSIONI MASSIME DEL WINDOW SPACE
                                       ----------------------------------------------------------------------------
                                    */
                                    
                                    #define MAXDIM_WSLENGTH  1024
                                    #define MAXDIM_WSHEIGHT   768
                                    
                                    
                                    /* 
                                       ----------------------------------------------------------------------------
                                       STRUTTURE DATI PER LA GESTIONE DEL MOUSE
                                       ----------------------------------------------------------------------------
                                    */
                                    
                               #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                    unsigned char MousePointerEnabled = 1;
                               #else
                                    NDEXTERN char MousePointerEnabled;
                               #endif       
                                    
                                    NDEXTERN TypeColor BackgroundMouse [16][16];
                                    NDEXTERN int BackgroundMouseFrame;
                                    
                               #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                    unsigned char IsTheFirstTimeForMouse = 1;
                               #else  
                                    NDEXTERN unsigned char IsTheFirstTimeForMouse;
                               #endif
                                    
                                    NDEXTERN unsigned short int MousePointer_PosX;
                                    NDEXTERN unsigned short int MousePointer_PosY;
                                    NDEXTERN unsigned short int OLD_MousePointer_PosX;
                                    NDEXTERN unsigned short int OLD_MousePointer_PosY;
                                    
                                    struct ndMouseState_Type
                                    {
                                        unsigned short int MousePointer_PosX;
                                        unsigned short int MousePointer_PosY;
                                        unsigned short int Buttons;
                                        int VectorX;
                                        int VectorY;
                                    };
                                    
                                    
                                    /* --------------------------------------------------------------------------------
                                       STRUTTURE PER LA GESTIONE DEI CONTROLLI 
                                       --------------------------------------------------------------------------------
                                       Queste strutture consentono alle routine di Nanodesktop di funzionare senza
                                       preoccuparsi di quale sia l'effettivo hardware di controllo sottostante. 
                                       
                                       Diverse routine richiamano internamente procedure di HAL per accedere 
                                       all'hardware di controllo, sia esso tastiera, mouse o pad di consolle,
                                       e poi rilasciano dei codici sotto forma di una struttura del tipo
                                       ndDataControllerType. 
                                       ----------------------------------------------------------------------------------
                                    */
                                    
                                    // Definizione di costanti 
                                    
                                    #define ND_BUTTON1         0x8000
                                    #define ND_BUTTON2         0x1000
                                    #define ND_BUTTON3         0x2000
                                    #define ND_BUTTON4         0x4000
                                    #define ND_BUTTON5         0x0001      // Corrisponde al tasto SELECT
                                    #define ND_CTRL_UP         0x0010
                                    #define ND_CTRL_DOWN       0x0040
                                    #define ND_CTRL_LEFT       0x0080
                                    #define ND_CTRL_RIGHT      0x0020
                                    
                                    struct ndDataControllerType
                                    {
                                       unsigned int 	TimeStamp;   // Nr campione letto dal pad 
                                       unsigned int 	Buttons;     
                                       unsigned char 	Lx;          // Normalmente usato per asse x analogico
                                       unsigned char 	Ly;          // Normalmente usato per asse y analogico
                                       char             Analog;      // Posto a 1 quando il pad è in analogico	
                                    };
                                    
                                    
                                    /* 
                                       -------------------------------------------------------------------------
                                       STRUTTURE PER ROUTINE DI GESTIONE IMMAGINI
                                       -------------------------------------------------------------------------
                                    */
                                    
                                    #define NDRGB     0
                                    #define NDMGKNB   1
                                    
                                    struct ndImage_Type
                                    {
                                       unsigned short int LenX, LenY;
                                       char ColorFormat;
                                       char IsStatic;
                                       TypeColor *Data;                      // Puntatore ai dati (MagicNumber) dell'immagine.
                                    };
                                    
                                    
                                    
                                    
                                    /* 
                                       --------------------------------------------------------------------------------
                                       STRUTTURE DATI PER LA GESTIONE DEI PULSANTI
                                       --------------------------------------------------------------------------------
                                        Le righe successive definiscono i buffer per la gestione dei pulsanti.
                                    
                                        Nell'ambiente NANODesktop, i pulsanti sono divisi in diverse classi.
                                    
                                        A) Pulsanti di classe 1.
                                    
                                           Sono i pulsanti che compaiono nella MinWindowsBar, e che consentono la
                                           massimizzazione di una finestra minimizzata. I dati relativi a questi
                                           pulsanti sono contenuti in un array apposito di 8 elementi.
                                    
                                           Ogni pulsante è caratterizzato da un ID, che identifica semplicemente
                                           la finestra a cui fa riferimento il pulsante.
                                    
                                        B) Pulsanti di classe 2.
                                    
                                           Sono i pulsanti che compaiono nella MinWindowsBar, e che consentono il
                                           cambiamento delle dimensioni della finestra che si trova in primo
                                           piano. I dati relativi a questi pulsanti sono contenuti in un array
                                           apposito di 4 elementi.
                                    
                                           Ogni pulsante è caratterizzato da un ID, con un preciso significato:
                                                0 - Aumenta la dimensione X della finestra in primo piano;
                                                1 - Diminuisce la dimensione X della finestra in primo piano;
                                                2 - Aumenta la dimensione Y della finestra in primo piano;
                                                3 - Diminuisce la dimensione Y della finestra in primo piano;
                                    
                                        C) Pulsanti di classe 3.
                                    
                                           Sono i pulsanti che consentono di minimizzare o chiudere una finestra.
                                           I dati relativi a tali pulsanti sono contenuti in appositi
                                           campi all'interno dell'array WindowData. Ognuno di questi pulsanti
                                           è caratterizzato da un ID, con il seguente significato:
                                               0 - Pseudopulsante per il trascinamento della finestra
                                               1 - Minimizza la finestra
                                               2 - Chiude la finestra
                                               3 - Scrolla in alto la barra di scrolling verticale
                                               4 - Scrolla in basso la barra di scrolling verticale
                                               5 - Scrolla a destra la barra di scrolling orizzontale
                                               6 - Scrolla a sinistra la barra di scrolling orizzontale
                                    
                                        D) Pulsanti di classe 4.
                                    
                                           Sono i pulsanti che vengono definiti dall'utente, e giaciono nel
                                           Window Space della finestra.
                                    
                                           I dati relativi a tali pulsanti sono contenuti in appositi campi
                                           all'interno dell'array WindowData.
                                    
                                           Ogni utente può definire fino a 16 pulsanti di classe 4 per
                                           finestra.
                                       ---------------------------------------------------------------------------
                                    */
                                    
                                    
                                    
                                    #define NOAUTOCLOSE    0
                                    #define AUTOCLOSE      1
                                    
                                    
                                    struct ButtonClass1_DataType
                                    {
                                           unsigned short int ButtonID;
                                           unsigned char ButtonIsActivated;
                                           unsigned short int x1;
                                           unsigned short int x2;
                                           unsigned short int y1;
                                           unsigned short int y2;
                                    };
                                    
                                    NDEXTERN struct ButtonClass1_DataType ButtonClass1_Data [__NRWINDOWSALLOCABLE];
                                                                                                // Un array che contiene le informazioni relative ai pulsanti di
                                                                                                // classe 1. I pulsanti di classe 1 sono quei pulsanti che
                                                                                                // vengono usati per massimizzare le finestre che sono
                                                                                                // state minimizzate
                                #ifdef I_NEED_TO_DECLARE_VARIABLES     
                                    unsigned short int MinWindowsBar_x1 = 5;
                                #else
                                    NDEXTERN  unsigned short int MinWindowsBar_x1;
                                #endif
                                    
                                #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned short int MinWindowsBar_y1 = __MAXSCREENY - 13;
                                #else
                                    NDEXTERN unsigned short int MinWindowsBar_y1;
                                #endif
                                     
                                #ifdef I_NEED_TO_DECLARE_VARIABLES    
                                    unsigned short int MinWindowsBar_x2 = __MAXSCREENX - 10 - 80 - 30 - 30;
                                #else
                                    NDEXTERN unsigned short int MinWindowsBar_x2;
                                #endif 
                                    
                                #ifdef I_NEED_TO_DECLARE_VARIABLES
                                    unsigned short int MinWindowsBar_y2 = __MAXSCREENY - 1;
                                #else
                                    NDEXTERN unsigned short int MinWindowsBar_y2; 
                                #endif
                                    
                                    NDEXTERN unsigned short int MinWindowsBar_LenX, MinWindowsBar_LenY;
                                    
                                    NDEXTERN unsigned char MinWindowsBarIsChanged;
                                    NDEXTERN unsigned char OtherFBPageIsNotUpgraded;
                                    
                                    
                                    // Codici di identificazione per i pulsanti di classe 2
                                    
                                    #define ENLARGE_X   1
                                    #define SMALLER_X   2
                                    #define ENLARGE_Y   3
                                    #define SMALLER_Y   4
                                    
                                    
                                    struct ButtonClass2_DataType
                                    {
                                           unsigned short int ButtonID;
                                           unsigned char ButtonIsActivated;
                                           unsigned short int x1;
                                           unsigned short int x2;
                                           unsigned short int y1;
                                           unsigned short int y2;
                                    };
                                    
                                    NDEXTERN struct ButtonClass2_DataType ButtonClass2_Data [4];       // Un array che contiene le informazioni relative ai pulsanti di
                                                                                                // classe 2.
                                    
                                    // Codici di identificazione per i pulsanti di classe 3
                                    
                                    #define   TITLEBAR_BUTTON           0
                                    #define   MINIMIZEWINDOW_BUTTON     1
                                    #define   CLOSEWINDOW_BUTTON        2
                                    #define   SCROLL_UP_BUTTON          3
                                    #define   SCROLL_DOWN_BUTTON        4
                                    #define   SCROLL_LEFT_BUTTON        5
                                    #define   SCROLL_RIGHT_BUTTON       6
                                    
                                    struct ButtonClass3_DataType
                                    {
                                           unsigned short int ButtonID;
                                           unsigned char ButtonIsActivated;
                                           unsigned short int x1;
                                           unsigned short int x2;
                                           unsigned short int y1;
                                           unsigned short int y2;
                                    };
                                    
                                    
                                    // Codice per i pulsanti di classe 4
                                    
                                    #define ND_BUTTON_NOT_INHIBITED        0
                                    #define ND_BUTTON_INHIBITED            1
                                    
                                    #define ND_BUTTON_RECTANGLE            0
                                    #define ND_BUTTON_ROUNDED              2
                                    #define ND_BUTTON_LOGIC_MODE           4
                                    #define ND_TRASPARENT_CALLBACK         8
                                    #define ND_CALLBACK_IN_NEW_CONTEXT     16
                                    
                                    struct ButtonClass4_DataType
                                    {
                                           char ButtonIsActivated;
                                           char ButtonIsInhibited;
                                           unsigned short int RRPosX1, RRPosY1, RRPosX2, RRPosY2;
                                           unsigned short int Q1PosX1, Q1PosY1, Q1PosX2, Q1PosY2;
                                           unsigned short int Q2PosX1, Q2PosY1, Q2PosX2, Q2PosY2;
                                           
                                           struct ndImage_Type IconImage;
                                           
                                           TypeColor ColorText, ColorButton, ColorBorderButton;
                                           unsigned short int FntHandle, FntSize;
                                           
                                           char StringID     [32];
                                           char *LabelButton1;
                                           char *LabelButton2;
                                           
                                           int ButtonStyle;
                                           int Callback;
                                           ndint64 InfoField;
                                           
                                           char AuxDataArea [32];
                                    };
                                    
                                    
                                    // Le seguenti strutture si occupano della gestione dei winmenu
                                    
                                    #define WMI_GO_BACK           0
                                    #define SCROLL_WMI1_TO_RIGHT  1
                                    #define SCROLL_WMI1_TO_LEFT   2
                                    
                                    struct ButtonWMI1Serv_DataType
                                    {
                                           char ButtonIsActivated;
                                           unsigned short int MBPosX1, MBPosY1, MBPosX2, MBPosY2;
                                    };
                                    
                                    #define NRMAX_WMI1_BUTTON_ALLOCABLE 16
                                    
                                    struct ButtonWMI1_DataType
                                    {
                                           char ButtonIsActivated;
                                           unsigned short int MBPosX1, MBPosY1, MBPosX2, MBPosY2;
                                           char *StringID;
                                    };
                                    
                                    #define SCROLL_WMI2_TO_UP     0
                                    #define SCROLL_WMI2_TO_DOWN   1
                                    
                                    struct ButtonWMI2Serv_DataType
                                    {
                                           char ButtonIsActivated;
                                           unsigned short int SSPosX1, SSPosY1, SSPosX2, SSPosY2;
                                    };
                                    
                                    #define NRMAX_WMI2_BUTTON_ALLOCABLE 16
                                    
                                    struct ButtonWMI2_DataType
                                    {
                                           char ButtonIsActivated;
                                           unsigned short int SSPosX1, SSPosY1, SSPosX2, SSPosY2;
                                           char *StringID;
                                    };
                                    
                                    
                                    
                                    
                                    /* ----------------------------------------------------------------------------
                                       STRUTTURE PER LA GESTIONE DEGLI EVENTI
                                       ----------------------------------------------------------------------------
                                       Routines come MouseControl restituiscono alla routine chiamante delle 
                                       informazioni sui pulsanti che sono stati premuti dall'utente, sulle voci
                                       del menù che sono state attivate, e sugli eventi che avvengono nel
                                       sistema grafico.
                                       
                                       Una struct di tipo ndEvent_Type possiede i seguenti campi:
                                       
                                       __TypeEvent:       Questo int contiene informazioni sul tipo di evento che
                                                          si è verificato: i valori possibili sono:
                                                               
                                                          0 NO_EVENT
                                                          
                                                          Gli eventi sono stati tutti gestiti internamente dal 
                                                          sottosistema grafico. 
                                                          
                                                          -2 CLOSEWNDCALLBACK_HAS_BEEN_EXEC
                                                          
                                                          E' stata eseguita una CloseWindow callback. Questo
                                                          messaggio viene recapitato a condizione che l'utente
                                                          non abbia impostato ad 1 il flag NoNotifyToMouseControl
                                                          relativo alla finestra per mezzo della routine 
                                                          ndLP_SetupCloseWndCallback.
                                                          
                                                          Nel campo __WindowID è presente l'handle della finestra
                                                          
                                                          Inoltre, verrà posto ad 1 il campo __CallbackExecuted 
                                                          ed il campo a 64 bit __CallbackAnswer conterrà il risultato a 64 bit della
                                                          Callback (può eventualmente essere un indirizzo). 
                                                          
                                                          -1 A_WINDOWS_WANTS_TO_BE_CLOSED
                                                          
                                                          Una finestra ha chiesto di essere chiusa. Il numero
                                                          della finestra è contenuto nel campo __WindowID
                                                          della struct ndEvent_Type
                                                          
                                                          1 CUSTOM_BUTTON_PRESSED
                                                          
                                                          E' stato premuto un pulsante definito dall'utente.
                                                          Nel campo __WindowID è presente l'handle della finestra
                                                          e nel campo __ButtonID è presente l'handle del 
                                                          pulsante. Nel campo __StringID viene posto l'identificatore
                                                          stringa del pulsante. 
                                                          
                                                          Se il sistema ha eseguito una callback prima di riportare
                                                          il controllo alla routine chiamante, verrà posto ad 1
                                                          il campo __CallbackExecuted ed il campo a 64 bit
                                                          __CallbackAnswer conterrà il risultato a 64 bit della
                                                          Callback (può eventualmente essere un indirizzo). 
                                                          
                                                          2 L1WMI_BUTTON_PRESSED
                                                          
                                                          E' stato premuto un elemento del menù L1WMI. Nel campo
                                                          __WindowID è presente l'handle della finestra. Nel
                                                          campo __ZString è sempre presente una copia della
                                                          ZString associata al pulsante.
                                                          
                                                          Se il sistema ha eseguito una callback prima di riportare
                                                          il controllo alla routine chiamante, verrà posto ad 1
                                                          il campo __CallbackExecuted ed il campo a 64 bit
                                                          __CallbackAnswer conterrà il risultato a 64 bit della
                                                          Callback (può eventualmente essere un indirizzo). 
                                                          
                                                          3 L2WMI_BUTTON_PRESSED
                                                          
                                                          E' stato premuto un elemento del menù L2WMI. Nel campo
                                                          __WindowID è presente l'handle della finestra. Nel
                                                          campo __ZString è sempre presente una copia della
                                                          ZString associata al pulsante.
                                                          
                                                          Se il sistema ha eseguito una callback prima di riportare
                                                          il controllo alla routine chiamante, verrà posto ad 1
                                                          il campo __CallbackExecuted ed il campo a 64 bit
                                                          __CallbackAnswer conterrà il risultato a 64 bit della
                                                          Callback (può eventualmente essere un indirizzo).  
                                                               
                                      __WindowID          L'ID della finestra dove è avvenuto l'evento
                                      
                                      __ButtonID          L'ID del pulsante definito dall'utente
                                      
                                      __StringID          L'identificatore stringa del pulsante
                                      
                                      __CallbackExecuted  Dice alla routine chiamante se è stata eseguita una callback
                                                          e quindi se il risultato a 64 bit in CallbackAnswer deve 
                                                          essere interpretato oppure no.
                                                          
                                      __CallbackAnswer    La risposta a 64 bit della Callback che è stata eseguita
                                      
                                      __ZString           La ZString che identifica l'elemento L1WMI/L2WMI che è 
                                                          stato premuto.
                                                         
                                      -------------------------------------------------------------------------------
                                    */                  
                                           
                                    #define SPECIAL_KEY_PRESSED                -3
                                    #define CLOSEWNDCALLBACK_HAS_BEEN_EXEC     -2
                                    #define A_WINDOW_WANTS_TO_BE_CLOSED        -1
                                    #define NO_EVENT                            0
                                    #define CUSTOM_BUTTON_PRESSED               1
                                    #define L1WMI_BUTTON_PRESSED                2
                                    #define L2WMI_BUTTON_PRESSED                3
                                    
                                    
                                    struct ndEvent_Type
                                    {
                                           char __TypeEvent;
                                           char __WindowID;
                                           char __ButtonID;
                                           char __StringID [32];
                                           char __CallbackExecuted;
                                           ndint64 __CallbackAnswer;
                                           char __ZString [512];
                                    };
                                    
                                    
                                    #define ND_IS_EQUAL              1
                                    #define ND_IS_DIFFERENT          2
                                    #define ND_IS_GREATER            3
                                    #define ND_IS_GREATER_OR_EQUAL   4
                                    #define ND_IS_SMALLER            5
                                    #define ND_IS_SMALLER_OR_EQUAL   6
                                    #define ND_NOT                 128
                                    
                                    /* 
                                       -------------------------------------------------------------------------
                                       VARIABILI PER IL FUNZIONAMENTO DEL RESIZER
                                       -------------------------------------------------------------------------
                                    */
                                    
                                       NDEXTERN char ResizerFocusActive;
                                       NDEXTERN char ResizerFocusWindow;
                                    
                                    
                                    /*
                                       -------------------------------------------------------------------------
                                       VARIABILI PER LE ROUTINE GRAFICHE A LIVELLO LOW-LEVEL
                                       -------------------------------------------------------------------------
                                       Le seguenti costanti simboliche servono a definire le dimensioni dello
                                       schermo. Si parla di dimensioni virtuali e di dimensioni reali. Le
                                       dimensioni virtuali si riferiscono all'area complessiva reale del
                                       framebuffer, mentre le dimensioni virtuali si riferiscono alla sola
                                       area visualizzabile.
                                       --------------------------------------------------------------------------
                                    */
                                    
                                     
                                     #ifdef I_NEED_TO_DECLARE_VARIABLES
                                            char ndInitSystemExecuted = 0;
                                    
                                            unsigned short int MaxScreenX = __MAXSCREENX,
                                                               MaxScreenY = __MAXSCREENY,
                                    
                                                               VMaxScreenX = __VMAXSCREENX,
                                                               VMaxScreenY = __VMAXSCREENY,
                                    
                                                               MaxCursX=__MAXCURSX,
                                                               MaxCursY=__MAXCURSY,
                                    
                                                               Mouse_MaxScreenX = __MAXSCREENX - 8,
                                                               Mouse_MaxScreenY = __MAXSCREENY - 8,
                                    
                                                               Windows_MaxScreenX = __MAXSCREENX - 14,
                                                               Windows_MaxScreenY = __MAXSCREENY - 14;
                                     #else
                                            NDEXTERN char ndInitSystemExecuted;
                                    
                                            NDEXTERN unsigned short int MaxScreenX, MaxScreenY;
                                            NDEXTERN unsigned short int VMaxScreenX, VMaxScreenY;
                                            NDEXTERN unsigned short int MaxCursX, MaxCursY;
                                            NDEXTERN unsigned short int Mouse_MaxScreenX, Mouse_MaxScreenY;
                                            NDEXTERN unsigned short int Windows_MaxScreenX, Windows_MaxScreenY;
                                     #endif
                                    
                                    
                                    // Le seguenti variabili si occupano di contenere il basescreen. Si osservi che in
                                    // teoria il basescreen dovrebbe avere dimensioni pari a Windows_MaxScreenX, 
                                    // Windows_MaxScreenY (ossia diminuite di 14 sia a destra che in basso). 
                                    // Però questo non può essere fatto perchè alcuni acceleratori grafici richiedono
                                    // che i componenti che devono essere trattati dal blitter abbiano dimensioni
                                    // multiple di un certo valore (per esempio 16 per il chip grafico della PSP). 
                                    // Per tale ragione, il base screen viene normalmente sovradimensionato. Le
                                    // dimensioni dell'array che dovrà contenere il basescreen vengono stabilite
                                    // dalle costanti manifeste (definite in HAL) __BASESCREENX e __BASESCREENY. 
                                    // Tuttavia, le routine di accesso al basescreen continuano a vedere un 
                                    // rettangolo di dimensioni Windows_MaxScreenX e Windows_MaxScreenY
                                    
                                    // Si noti inoltre che è necessario che l'indice per la coordinata X sia posto
                                    // dopo l'indice per la coordinata Y in modo da presentare i dati in memoria
                                    // in modo compatibile con il chip grafico della PSP 
                                    
                                    // Alcuni chip grafici inoltre, pretendono che l'indirizzo del buffer sia un
                                    // multiplo di un certo valore, altrimenti si verifica un malfunzionamento (è sempre
                                    // il caso di PSP). Ecco perchè qui si provvede a dire al compilatore di allineare 
                                    // lo stack a 16 bit
                                     
                                     
                                     #ifdef I_NEED_TO_DECLARE_VARIABLES
                                            TypeColor                 __attribute__((aligned(16))) BaseScreen [__BASESCREENY][__BASESCREENX];
                                     #else
                                            NDEXTERN TypeColor        BaseScreen [__BASESCREENY][__BASESCREENX];
                                     #endif
                                     
                                     NDEXTERN char                    BaseScreenIsActive;
                                    
                                    // La seguente variabile dice al sistema se le trasparenze sono attive o meno
                                    
                                     NDEXTERN char                    TrasparencyEnabled;
                                    
                                    // La seguente variabile dice al sistema se le thumbnail per il file manager di 
                                    // tipo 1 sono attive o meno.
                                    
                                     NDEXTERN char                    ThumbnailEnabled;
                                    
                                    // Le seguenti variabili si occupano di tenere traccia della posizione del cursore
                                    // per le routine di scrittura carattere che lavorano a livello LOW.
                                    
                                     NDEXTERN unsigned short int      LL_PosCursX, LL_PosCursY;
                                    
                                    
                                    
                                    /*
                                       -----------------------------------------------------------------------------
                                       VARIABILI PER LE ROUTINE CHE LAVORANO AL LIVELLO LOGICAL PAGE
                                       -----------------------------------------------------------------------------
                                       Ogni finestra scrive i pixel su un array logico che viene mantenuto in
                                       memoria e che prende il nome di logical page.
                                       
                                       Una logical page contiene anche lo spazio per il bordo della finestra,
                                       per il titolo, per la barra del titolo. La necessità delle logical pages
                                       nasce dal fatto che alcune finestre possono essere visualizzate, altre
                                       possono essere minimizzate, ed alcune finestre sono sovrapposte
                                       parzialmente ad altre finestre.
                                       -----------------------------------------------------------------------------
                                    */
                                    
                                    
                                     NDEXTERN unsigned char WindowsVector[__NRWINDOWSALLOCABLE];                          // Questo array è un vettore. Riporta l'ordine delle
                                                                                                                          // finestre sullo schermo
                                     NDEXTERN unsigned char NrWindowsAllocated;                                           // Contiene il numero delle finestre già allocate
                                    
                                    
                                    /*
                                     ------------------------------------------------------------------------
                                     ALTRI SIMBOLI
                                     ------------------------------------------------------------------------
                                    */
                                    
                                    #define  NORENDER     0
                                    #define  RENDER       1
                                    #define  TRASP        2
                                    
                                    
                                    /*
                                     ------------------------------------------------------------------------
                                     COSTANTI MANIFESTE PER ndLP_AllocateWindowComplex
                                     ------------------------------------------------------------------------
                                    */
                                    
                                    #define  AUTOFIX             1
                                    #define  NO_RESIZEABLE       2
                                    #define  MENUSUPPORTED       4
                                    #define  USE_POSGEN          8
                                    
                                    /*
                                      ------------------------------------------------------------------------
                                      GESTIONE DELLE ROI
                                      ------------------------------------------------------------------------
                                    */
                                    
                                    #define  ROI_POSX                1
                                    #define  ROI_POSY                2
                                    #define  ROI_FRAC                4
                                    #define  ROI_SMART               8                                                                  
                                    
                                    /*
                                       -----------------------------------------------------------------------------------
                                       STRUTTURE DATI PER IL NEW-NAME MANAGER
                                       -----------------------------------------------------------------------------------
                                    */
                                    
                                    /*
                                       I parametri relativi al generatore di nuovi nomi non sono memorizzati in variabili
                                       statiche, ma sono invece memorizzati in una struct di tipo NNData_Type che ogni
                                       istanza di GetNewName salva nel proprio stack.
                                    
                                       Le routine richiamate da GetNewName ricevono poi l'indirizzo di tale struct
                                       come parametro. In tal modo è possibile avere diverse istanze di GetNewName.
                                    */
                                    
                                    struct NNMData_Type
                                    {
                                       char *FileOutputAddr;
                                       
                                       unsigned short int A1_X1, A1_Y1, A1_X2, A1_Y2;        // Le coordinate della finestra NewName
                                       unsigned short int A2_X1, A2_Y1, A2_X2, A2_Y2;        // Coordinate della gray-bar
                                       unsigned short int A3_X1, A3_Y1, A3_X2, A3_Y2;        // Coordinate dell'area caratteri
                                    
                                       unsigned short int MaxLenChar;
                                    
                                       char WndHandle;
                                       unsigned char FirstCharShown;                         // Il primo carattere che viene mostrato
                                       unsigned char PosCursor;                              // La posizione del cursore
                                       char PrevArrowLeftStatus;                             // Lo stato precedente della freccia a sinistra.
                                    
                                       char Display [255];                                   // Il contenuto della GrayBar
                                    
                                       // Campi usati per buffer Callback Data
                                       
                                       char VKB_YouCanExit;
                                       char VKB_UserInterruption;
                                       
                                    };
                                    
                                    /* ------------------------------------------------------------------------------------------
                                       COSTANTI MANIFESTE PER LA GESTIONE DI ELEMENTI GRAFICI
                                       ------------------------------------------------------------------------------------------
                                    */
                                    
                                    #define ND_VOID 1
                                    #define ND_ROUNDED 2
                                    
                                    
                                    /* 
                                       -------------------------------------------------------------------------------------------
                                       TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE GENERICHE DI INDIRIZZI
                                       -------------------------------------------------------------------------------------------
                                    */
                                    
                                    
                                    struct HEADLIST_Generic_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;
                                    };
                                    
                                    struct ELEMENT_Generic_Type
                                    {
                                      int Addr;
                                      int NextElementAddr;
                                    };
                                    
                                    /*
                                       -------------------------------------------------------------------------------------------
                                       TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI CLASSI 
                                       -------------------------------------------------------------------------------------------
                                    */
                                    
                                    struct HEADLIST_Class_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;
                                    };
                                    
                                    struct ELEMENT_Class_Type
                                    {
                                      char NameClass    [64];
                                      char Extensions   [255];
                                      char StringIDIcon [12];     // La stringID dell'icona associata al tipo            
                                      int  ThumbFunction;         // Indirizzo di una funzione che esegue una preview (thumbnail) del file in questione
                                    };
                                    
                                    NDEXTERN struct HEADLIST_Class_Type SystemClasses;            // Struct che contiene le CLASSI di sistema
                                    
                                    /*
                                       -------------------------------------------------------------------------------------------
                                       TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI ICONE 
                                       -------------------------------------------------------------------------------------------
                                    */
                                    
                                    struct HEADLIST_Icon_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;
                                    };
                                    
                                    struct ELEMENT_Icon_Type
                                    {
                                      char                        StringID [12];
                                      char                        Format;             // Segnala se l'icona è 8x8, 16x16 o 22x22 pixel
                                      TypeColor                   MNPixel [22][22];   // Magic Number dei pixel                     
                                    };
                                    
                                    NDEXTERN struct HEADLIST_Icon_Type SystemIcons;            // Struct che contiene le icone di sistema
                                    
                                    
                                    
                                    
                                    
                                    /*
                                       ---------------------------------------------------------------------------------------
                                         STRUTTURE DATI PER ROUTINE DI GESTIONE DEGLI L2WMI (L2 WinMenu Item)
                                       ---------------------------------------------------------------------------------------
                                    */
                                    
                                    #define ND_WMI_INHIBITED    1 
                                    
                                    struct HEADLIST_L2WMI_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;   
                                       char Dynamic; 
                                    };
                                    
                                    
                                    struct ELEMENT_L2WMI_Type
                                    {
                                       char StringID [128];
                                       char NameItem [128];
                                       char Item_Inhibited;
                                       char ExecInNewContext;
                                       void *CallbackFunction;    
                                    };
                                    
                                    /*
                                       ---------------------------------------------------------------------------------------
                                        STRUTTURE DATI PER ROUTINE DI GESTIONE DEGLI L1WMI (L1 WinMenu Item)
                                       ---------------------------------------------------------------------------------------
                                    */
                                    
                                    struct HEADLIST_L1WMI_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;   
                                       char Dynamic; 
                                    };
                                    
                                    
                                    struct ELEMENT_L1WMI_Type
                                    {
                                       char StringID [128];
                                       char NameItem [128];
                                       char Item_Inhibited;
                                       char ExecInNewContext;
                                       char Item_Expanded;
                                       struct HEADLIST_L1WMI_Type *L1SubMenu;
                                       struct HEADLIST_L2WMI_Type *L2SubMenu;
                                       void *CallbackFunction;    
                                    };
                                    
                                    
                                    /*  
                                       ----------------------------------------------------------------------------------------
                                        STRUTTURE DATI PER ROUTINE DI GESTIONE FILE
                                       ----------------------------------------------------------------------------------------
                                    */
                                    
                                    #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                           char ndFSInitExecuted = 0;
                                    #else
                                           NDEXTERN char ndFSInitExecuted;
                                    #endif
                                    
                                    struct ndFS_DateTime_Type
                                    {
                                      unsigned short  year;
                                      unsigned short  month;
                                      unsigned short  day;
                                      unsigned short  hour;
                                      unsigned short  min;
                                      unsigned short  sec;
                                      unsigned int    msec;
                                      char            available;
                                    };
                                    
                                    
                                    struct ndFS_Status_Type
                                    {
                                      int                        mode;
                                      unsigned int               attr;
                                      int                        size;
                                    
                                      struct ndFS_DateTime_Type  ctime;       // Data creazione
                                      struct ndFS_DateTime_Type  atime;       // Data accesso
                                      struct ndFS_DateTime_Type  mtime;       // Data modifica
                                    
                                      unsigned int  st_private [6];           // Device specific file
                                    };
                                    
                                    
                                    struct ndFS_DirEntry_Type
                                    {
                                      char   d_name [256];                     // Nome del file
                                      struct ndFS_Status_Type Status;          // Stato del file
                                      void   *d_private;                       // Puntatore a dati specifici del dispositivo.
                                    };
                                    
                                    
                                    /*
                                       -------------------------------------------------------------------------------------------
                                       TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTE DI FILE 
                                       -------------------------------------------------------------------------------------------
                                    */
                                    
                                    
                                    struct HEADLIST_File_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;
                                    };
                                    
                                    struct ELEMENT_File_Type
                                    {
                                      char                        Name [256];
                                      char                        Ext  [4];
                                      int                         Size;
                                      char                        IsDir;
                                      struct ndFS_DateTime_Type   DateTime;
                                    };
                                    
                                    /*
                                       -------------------------------------------------------------------------------------------
                                       TIPI VARIABILI E ROUTINE PER LA GESTIONE DELLE LISTBOX 
                                       -------------------------------------------------------------------------------------------
                                    */
                                    
                                    
                                    struct HEADLIST_LBoxItem_Type
                                    {
                                       int FirstElementAddr;
                                       int LastElementAddr;
                                       int NrElements;
                                    };
                                    
                                    struct ELEMENT_LBoxItem_Type
                                    {
                                      char      Name [64];
                                      void      *LBoxCallback;
                                      ndint64   LBoxCode;
                                      char      IsInhibited;
                                      char      IsInEvidence;
                                    };
                                    
                                    
                                    
                                    /*
                                        ----------------------------------------------------------------------------------------
                                        GESTORE POSGEN
                                        ----------------------------------------------------------------------------------------
                                    */
                                    
                                    
                                    struct PosGenData_Type
                                    {
                                           unsigned short BestX1, BestY1, BestX2, BestY2;
                                           ndint64  BestSigma;
                                           ndint64  BestEpsilon;
                                           char TheFirstCmp;
                                    };
                                    
                                    
                                    /*
                                        ----------------------------------------------------------------------------------------
                                        STRUTTURE DATI FILE MANAGER T1 E T2
                                        ----------------------------------------------------------------------------------------
                                    */
                                    
                                    // I parametri per il funzionamento del file manager sono memorizzati in una struct.
                                    // Ogni istanza del FileManager di tipo 1 salva come variabile locale una struct di
                                    // tipo FileManagerData_Type (che è allocata in stack) e poi richiama una serie
                                    // di routine interne a cui passa sempre l'indirizzo di tale struttura.
                                    // Questo consente di avere istante multiple del FileManager

                                                                       
                                    struct T1FMData_Type
                                    {
                                         char WndHandle1;
                                        
                                         unsigned short int B1_X1, B1_Y1, B1_X2, B1_Y2;      // Zona dello schermo dove verranno disegnati i vari elementi
                                         unsigned short int B2_X1, B2_Y1, B2_X2, B2_Y2;      // Limiti della barra del percorso
                                         unsigned short int B3_X1, B3_Y1, B3_X2, B3_Y2;      // Limiti dell'area file attiva (livello 1)
                                        
                                         unsigned short int LenActiveAreaX, LenActiveAreaY;
                                        
                                         unsigned short int VLenCharX;                        // Nr. di caratteri visualizzabili nella
                                                                                              // barra del titolo
                                        
                                         unsigned short int VCharX1, VCharX2;
                                        
                                         unsigned short int ButtonHandle_MainButton;
                                         unsigned short int ButtonHandle_MkDir;
                                         unsigned short int ButtonHandle_Delete;
                                         unsigned short int ButtonHandle_New;
                                         unsigned short int ButtonHandle_Rename;
                                        
                                         unsigned short int ButtonHandle_ScrollFileUp, ButtonHandle_ScrollFileDown; 
                                         unsigned short int ButtonHandle_ChDirPointPoint;
                                        
                                         unsigned short int C1_X1, C1_Y1, C1_X2, C1_Y2;       // Area attiva per i file (livello 2 o Box Area)
                                         unsigned short int LenBoxAreaX, LenBoxAreaY;
                                         unsigned short int MargineBoxX, MargineBoxY;
                                          
                                         unsigned short int NrItemsVisualizable;              // Nr di elementi visualizzabili
                                         
                                         unsigned short int C2_X1, C2_Y1, C2_X2, C2_Y2;       // Area attiva per i file (livello 3 o InternalBoxArea)
                                         unsigned short int LenBoxX, LenBoxY;
                                        
                                         unsigned short int MaxLenNmFile;
                                         unsigned short int MaxLenPathAllowed;
                                        
                                         unsigned short int GraphArea_X1,  GraphArea_Y1,  GraphArea_X2,  GraphArea_Y2;
                                         unsigned short int SystemArea_X1, SystemArea_Y1, SystemArea_X2, SystemArea_Y2;
                                        
                                         unsigned short int FirstRowPosY, SecondRowPosY, FirstColumnPosX;
                                         
                                         unsigned short int PrevStatusArrowUp;
                                         unsigned short int PrevStatusArrowDown;
                                        
                                         unsigned short int FirstElementVisualized;
                                         unsigned short int LastElementVisualized;
                                        
                                         char PrevStatusChDirButton;
                                         char SaveMode;
                                        
                                         unsigned short int PosIconBoxX, PosIconBoxY;
                                         unsigned short int DimIconBox;
                                         
                                         struct ndImage_Type ThumbnailCached [20];
                                         char                NameThumbnailCached [20][256];
                                         char                ThumbnailCachePointer;
                                    };
                                    
                                    // I parametri per il funzionamento del file manager sono memorizzati in una struct.
                                    // Ogni istanza del FileManager di tipo 2 salva come variabile locale una struct di
                                    // tipo FileManagerData_Type (che è allocata in stack) e poi richiama una serie
                                    // di routine interne a cui passa sempre l'indirizzo di tale struttura.
                                    // Questo consente di avere istante multiple del FileManager

                                    struct T2FMData_Type
                                    {
                                         char WndHandle1;
                                        
                                         unsigned short int B1_X1, B1_Y1, B1_X2, B1_Y2;      // Zona dello schermo dove verranno disegnati i vari elementi
                                         unsigned short int B2_X1, B2_Y1, B2_X2, B2_Y2;      // Limiti della barra del percorso
                                         unsigned short int B3_X1, B3_Y1, B3_X2, B3_Y2;      // Limiti dell'area file attiva (livello 1)
                                        
                                         unsigned short int ButtonHandle_MainButton;
                                         unsigned short int ButtonHandle_MkDir;
                                         unsigned short int ButtonHandle_Delete;
                                         unsigned short int ButtonHandle_New;
                                         unsigned short int ButtonHandle_Rename;
                                        
                                         unsigned short int ButtonHandle_ScrollFileUp, ButtonHandle_ScrollFileDown;
                                         unsigned short int ButtonHandle_ChDirPointPoint;
                                        
                                         unsigned short int VCharX1, VCharX2, VCharY1, VCharY2;  // Limiti mappa caratteri virtuale
                                         unsigned short int VLenCharX, VLenCharY;
                                         unsigned short int Tab1, Tab2, Tab3, Tab4;
                                         unsigned short int MaxLenNmFile;
                                         unsigned short int MaxLenPathAllowed;
                                        
                                         unsigned short int PrevStatusArrowUp;
                                         unsigned short int PrevStatusArrowDown;
                                        
                                         unsigned short int FirstElementVisualized;
                                         unsigned short int LastElementVisualized;
                                        
                                         char PrevStatusChDirButton;
                                         char SaveMode;
                                    };
                                    
                                    
                                    #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                         char ndFileManCurrPath [255] = "ms0:/";
                                    #else
                                         NDEXTERN char ndFileManCurrPath [255];                                 
                                    #endif     

                                    /*
                                        ----------------------------------------------------------------------------------------
                                        GESTORE DELLE PROGRESS-BAR
                                        ----------------------------------------------------------------------------------------
                                    */
                                    
                                    #define PBAR_PERCENT        32
                                    #define PBAR_NOT_LABEL     128
                                    
                                    struct TrackBar_Type
                                    {
                                           float  Value;
                                           float  MaxValue;
                                           float  MinValue;
                                           
                                           char   IsActive;
                                           char   StepCode;
                                           char   StepButtonOn;
                                           
                                           char   Register;
                                           char   IsInhibited;
                                           char   PervasiveCallback;
                                           
                                           unsigned short int TBar_X1, TBar_Y1;
                                           unsigned short int TBar_X2, TBar_Y2;
                                           
                                           TypeColor TextColor; 
                                           TypeColor PBarColor, PBarBkgrColor, PBarBorderColor;
                                           TypeColor TBarColor, TBarBorderColor;
                                           
                                           unsigned short int Attribute;
                                           unsigned char FntHandle;
                                           
                                           char PlusButtonHandle, MinusButtonHandle, HelpButtonHandle;
                                           char ChStepPlusButtonHandle, ChStepMinusButtonHandle;
                                           
                                           int TB_CallBack, TB_InfoField;   
                                    };
                                    
                                    #define TBBAR_PERCENT        32
                                    #define CHANGE_STEP_ENABLED  64
                                    #define TBBAR_NOT_LABEL     128
                                    
                                    #define TBBAR_STEP_1        0*256
                                    #define TBBAR_STEP_10       1*256
                                    #define TBBAR_STEP_100      2*256
                                    #define TBBAR_STEP_1000     3*256
                                    #define TBBAR_STEP_10000    4*256
                                    
                                    #define TBBAR_STEP_1_E01    5*256
                                    #define TBBAR_STEP_1_E02    6*256
                                    #define TBBAR_STEP_1_E03    7*256
                                    #define TBBAR_STEP_1_E04    8*256 
                                    
                                    #define TBBAR_INHIBITED    16*256 
                                    #define TBBAR_PERVASIVE_CALLBACK    32*256 
                                    
                                    /*
                                        ----------------------------------------------------------------------------------------
                                        TEXT-BOX, TEXT-AREA
                                        ----------------------------------------------------------------------------------------
                                    */
                                    
                                    struct TextBox_Type
                                    {
                                       char IsActive;
                                       unsigned short int PosX1, PosY1, PosX2, PosY2;
                                       unsigned short int LenX, LenY;
                                       unsigned short int mLenX, mLenY;
                                       
                                       char *DescriptorString;
                                       
                                       TypeColor TextColorOff, BgTextColorOff;
                                       TypeColor TextColorOn,  BgTextColorOn;
                                       TypeColor TextColorInh, BgTextColorInh;
                                       
                                       int  PosCursX, PosCursY;
                                       int  FirstCharX, FirstCharY;
                                       
                                       int  NrCharsX, NrCharsY;
                                       int  ShownCharsX, ShownCharsY;
                                       
                                       int  MargineX, MargineY;
                                       
                                       unsigned char FntHandle;
                                       
                                       char  DimCharX,  DimCharY;
                                       char  sDimCharX, sDimCharY;
                                       
                                       void *TextData, *ReserveBuffer;   
                                       int  IUseInternalBuffer; 
                                       
                                       int  ResPosCursX, ResPosCursY;
                                       
                                       char ButtonHandle;  
                                       
                                       char KeybIsOpened, TextAreaIsOn;  
                                       char IsInhibited;
                                       
                                       void *cbProcessValueBefore;
                                       void *cbProcessValueAfter;
                                       char *cbProcessValueForValidation;
                                       void *ProcessValueData;
                                       
                                       int  VKBHandle;
                                       int  TextBoxHandle, TextBoxWndHandle;        // Necessari
                                    };
                                    
                                    #define ND_TEXTAREA_INHIBITED   0x10000        // Costante manifesta per l'API
                                    
                                    /*
                                        ----------------------------------------------------------------------------------------
                                        LIST-BOX
                                        ----------------------------------------------------------------------------------------
                                    */
                                    
                                    struct ListBox_Type
                                    {
                                        char IsActive;
                                        unsigned short int PosX1, PosY1, PosX2, PosY2;
                                        unsigned short int LenX, LenY;
                                        
                                        int NrSlotsVisualizable;
                                        int FirstItemVisualized;
                                        int MarginY;
                                        
                                        char BtnForScrollUp;
                                        char BtnForScrollDown;
                                        
                                        TypeColor ColorBorder;
                                        TypeColor ColorBackground; 
                                        TypeColor ColorBorderSlot; 
                                        TypeColor ColorSlot;
                                        TypeColor ColorSlotActive; 
                                        TypeColor ColorSlotInh;
                                        TypeColor ColorTextSlot;
                                        TypeColor ColorTextSlotActive; 
                                        TypeColor ColorTextSlotInh;  
                                        
                                        unsigned char FntHandle;
                                        char EvidStrategy;
                                        
                                        struct HEADLIST_LBoxItem_Type Core;    
                                    };
                                    
                                    struct ndListBoxColors_Type
                                    {
                                       TypeColor ColorBorder;
                                       TypeColor ColorBackground; 
                                       TypeColor ColorBorderSlot; 
                                       TypeColor ColorSlot;
                                       TypeColor ColorSlotActive; 
                                       TypeColor ColorSlotInh;
                                       TypeColor ColorTextSlot;
                                       TypeColor ColorTextSlotActive; 
                                       TypeColor ColorTextSlotInh;                           
                                    };

                                    #define ND_LBOX_STABLE_EVIDENCE 0
                                    #define ND_LBOX_FLASH_EVIDENCE  1
                                    #define ND_LBOX_MULTI_EVIDENCE  2
                                    #define ND_LBOX_NULL_EVIDENCE   3

                                    // Strutture contenenti i colori di default

                                    #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                        struct ndListBoxColors_Type ndLBoxDefaultColor0 =
                                        {
                                            COLOR_WHITE,            // ColorBorder
                                            COLOR_BLACK,            // ColorBackground
                                            COLOR_GRAY04,           // ColorBorderSlot
                                            COLOR_GRAY,             // ColorSlot
                                            COLOR_BLUE,             // ColorSlotActive
                                            COLOR_GRAY02,           // ColorSlotInh
                                            COLOR_BLACK,            // ColorTextSlot
                                            COLOR_WHITE,            // ColorTextSlotActive
                                            COLOR_BLACK             // ColorTextSlotInh
                                        };   
                                           
                                        struct ndListBoxColors_Type ndLBoxDefaultColor1 =
                                        {
                                            COLOR_BLACK,            // ColorBorder
                                            COLOR_WHITE,            // ColorBackground
                                            COLOR_GRAY13,           // ColorBorderSlot
                                            COLOR_WHITE,            // ColorSlot
                                            COLOR_BLUE,             // ColorSlotActive
                                            COLOR_WHITE,            // ColorSlotInh
                                            COLOR_BLACK,            // ColorTextSlot
                                            COLOR_WHITE,            // ColorTextSlotActive 
                                            COLOR_GRAY28            // ColorTextSlotInh                           
                                        };   
                                    #else
                                           NDEXTERN struct ndListBoxColors_Type ndLBoxDefaultColor0;
                                           NDEXTERN struct ndListBoxColors_Type ndLBoxDefaultColor1; 
                                    #endif
                                    
                                    /*
                                        ----------------------------------------------------------------------------------------
                                        C-BOX
                                        ----------------------------------------------------------------------------------------
                                    */
                                    
                                    #define ND_CBOX_INHIBITED      1
                                    
                                    struct ndCBoxAuxInfo_Type
                                    {
                                         int       *AdrVariable;
                                         
                                         char      IsOn;
                                         char      IsInhibited;
                                         TypeColor MainColor;
                                         void*     CBoxCallback;  
                                    };
                                    
                                    #ifdef I_NEED_TO_DECLARE_VARIABLES 
                                    
                                     unsigned char CBoxCross [14][14] =
                                     {
                                          ".............",
                                          ".............",
                                          "..xx.....xx..",
                                          "..xxx...xxx..",
                                          "...xxx.xxx...",
                                          "....xxxxx....",
                                          ".....xxx.....",
                                          "....xxxxx....",
                                          "...xxx.xxx...",
                                          "..xxx...xxx..",
                                          "..xx.....xx..",
                                          ".............",
                                          ".............",
                                          "............."
                                     };

                                     #else
                                          NDEXTERN char CBoxCross [14][14];
                                     #endif
                                     
                                    
                                    /*
                                       -----------------------------------------------------------------------------------------
                                       STRUTTURA WindowData
                                       -----------------------------------------------------------------------------------------
                                       Tutte le informazioni sulle finestre del sistema vengono memorizzate nella struct 
                                       WindowData.
                                       -----------------------------------------------------------------------------------------
                                    */
                                    
                                    struct WindowDataType
                                    {
                                           char *WindowTitle;                                                   // Puntatore al titolo della finestra
                                           char WindowTitleChar [__MAXCURSX];                                   // Array interno per salvataggio caratteri
                                    
                                           TypeColor ColorTitle, ColorBGTitle, ColorBorder, ColorBGWS;          // Memorizzano i colori della finestra
                                    
                                           unsigned short int WndLimitX1, WndLimitY1, WndLimitX2, WndLimitY2;    // Limiti delle finestre sullo schermo
                                           unsigned short int WndLength, WndHeight;                              // Lunghezza e larghezza della finestra
                                           unsigned short int HwWndLength, HwWndHeight;                          // Lunghezza e larghezza della finestra per l'acceleratore hardware
                                           unsigned short int WSWndLength, WSWndHeight;                          // Lunghezza e larghezza del Window Space
                                           unsigned short int ROI_PosX, ROI_PosY;                                // Posizione della region of interest del WS visualizzata
                                           unsigned short int ROI_LenX, ROI_LenY;                                // Dimensione della Region of Interest
                                    
                                           unsigned short int ROI_PosX1, ROI_PosY1;
                                           unsigned short int ROI_PosX2, ROI_PosY2;
                                    
                                           unsigned short int AA1_X1, AA1_X2, AA1_Y1, AA1_Y2, AA1_LenX, AA1_LenY; // Posizione e dimensioni dell'area attiva livello 1
                                           unsigned short int AA2_X1, AA2_X2, AA2_Y1, AA2_Y2, AA2_LenX, AA2_LenY; // Posizione e dimensioni dell'area attiva livello 2
                                           unsigned short int AA3_X1, AA3_X2, AA3_Y1, AA3_Y2, AA3_LenX, AA3_LenY; // Posizione e dimensioni dell'area attiva livello 3
                                    
                                           unsigned short int OrizzBar_PosX1, OrizzBar_PosY1;                     // Coordinate della barra orizzontale
                                           unsigned short int OrizzBar_PosX2, OrizzBar_PosY2;
                                           
                                           unsigned short int VertBar_PosX1, VertBar_PosY1;                       // Coordinate della barra verticale
                                           unsigned short int VertBar_PosX2, VertBar_PosY2;
                                           
                                           TypeColor *_p;                                                         // Puntatore ai dati della pagina logica    
                                           TypeColor *_ws;                                                        // Puntatore ai dati del window space
                                           TypeColor *_ss;                                                        // Puntatore ai dati del service space
                                           unsigned char *_CharMap;                                               // Puntatore alla mappa caratteri
                                           
                                           TypeColor *_ColorCharMap;                                              // Puntatore alla color char map
                                           
                                           unsigned char WindowIsActive;                                         // Posta a 1 quando la finestra è attiva
                                           unsigned char Minimize;                                               // Posto a 1 quando la finestra viene minimizzata;
                                           
                                           unsigned short int WS_MaxCursX, WS_MaxCursY;                          // Posizione massima del cursore nella finestra
                                           unsigned short int WS_PosCursX, WS_PosCursY;                          // Posizione del cursore nella finestra
                                           
                                           unsigned short int MargineCharX, MargineCharY;                        // Margini per la visualizzazione caratteri
                                           unsigned short int CharArea_PosX1, CharArea_PosY1;
                                           unsigned short int CharArea_PosX2, CharArea_PosY2;
                                           
                                           struct ButtonClass3_DataType ButtonClass3_Data[7];                  // Array che contiene le informazioni inerenti ai pulsanti di classe 3
                                    
                                           char NrButtonsAllocated;
                                           
                                           struct ButtonClass4_DataType ButtonClass4_Data[__NRBUTTONSALLOCABLE]; // Array che contiene le informazioni inerenti ai pulsanti di classe 4  
                                    
                                           unsigned char NoResizeable;
                                           
                                           unsigned char MenuSupported;
                                           
                                           unsigned short int MenuBar_PosX1, MenuBar_PosY1;
                                           unsigned short int MenuBar_PosX2, MenuBar_PosY2;
                                           unsigned short int MenuBar_LenX,  MenuBar_LenY;
                                           
                                           unsigned short int MenuBar_LA_PosX1, MenuBar_LA_PosY1;
                                           unsigned short int MenuBar_LA_PosX2, MenuBar_LA_PosY2;
                                           unsigned short int MenuBar_LA_LenX,  MenuBar_LA_LenY;
                                           
                                           unsigned char SS_Supported;
                                           unsigned char SS_Enabled;
                                           
                                           unsigned short int SS_LenX, SS_LenY;
                                           
                                           struct HEADLIST_L1WMI_Type MainWinMenu;
                                           
                                           char ZString [512];             // ZString attuale
                                           
                                           unsigned short int NrFirstL1WMIButton; 
                                           
                                           char OpenL2WMIMenu, PrevOpenL2WMIMenu;
                                           unsigned short NrL1WMIButtonOpened;
                                           
                                           char L2WMIMenuScrollActive;
                                           unsigned short int L2WMIScrollElement;
                                           
                                           char MenuBarIsChanged;
                                           
                                           struct ButtonWMI1Serv_DataType ButtonWMI1Serv [4];
                                           
                                           char NrWMI1ButtonsAllocated;
                                           struct ButtonWMI1_DataType ButtonWMI1 [NRMAX_WMI1_BUTTON_ALLOCABLE];
                                           
                                           struct ButtonWMI2Serv_DataType ButtonWMI2Serv [2];
                                           
                                           char NrWMI2ButtonsAllocated;
                                           struct ButtonWMI2_DataType ButtonWMI2 [NRMAX_WMI2_BUTTON_ALLOCABLE];
                                           
                                           char NrTrackBarsAllocated;       
                                           struct TrackBar_Type TrackBar [__NRTRACKBARSALLOCABLE];
                                           
                                           char NrTextBoxAllocated;
                                           struct TextBox_Type TextBox [__NRTEXTBOXALLOCABLE];
                                           
                                           char NrListBoxAllocated;
                                           struct ListBox_Type ListBox [__NRLISTBOXALLOCABLE];
                                           
                                           char UserArea [__USERAREASIZE];
                                           
                                           ndint64 (*PntToCloseWndCallback)(char WndHandle, ndint64 WndInfoField);
                                           ndint64  WndInfoField;
                                           
                                           char NoNotifyToMouseControl;
                                           
                                           char ndWS_CurrentFntHandle;
                                           char ndMB_CurrentFntHandle;
                                           char ndLP_CurrentFntHandle;
                                    };
                                    
                                    NDEXTERN struct WindowDataType WindowData[__NRWINDOWSALLOCABLE];                     // Genera un array di 8 strutture di tipo WindowDataType
                                    
                                    
                                    
                                    /* 
                                       -----------------------------------------------------------------------------------------
                                        STD-IN
                                       -----------------------------------------------------------------------------------------
                                    */ 
                                    
                                    struct StdInDataArea_Type
                                    {
                                           int KeyPressedCode;
                                           unsigned char AsciiCode;
                                    };
                                    
                                    NDEXTERN int  SystemKeyboardType;
                                    NDEXTERN char SystemKeyboardAutoClose;
                                    
                                    NDEXTERN char SystemKeyboardEnabled;
                                    NDEXTERN char SystemKeyboardHandle;
                                    
                                    
                                    
                                    /* 
                                       -----------------------------------------------------------------------------------------
                                        DIALOG-BOX STD-OUT, STD-ERR
                                       -----------------------------------------------------------------------------------------
                                    */ 
                                    
                                    
                                    NDEXTERN char StdOutWindowActivated;
                                    
                                    NDEXTERN unsigned short int StdOutPosX1, StdOutPosY1, 
                                                                StdOutPosX2, StdOutPosY2;
                                                                
                                    NDEXTERN char StdOutCustomDimsEnabled;
                                    NDEXTERN int  StdOutCustomDimX, StdOutCustomDimY;
                                                       
                                    NDEXTERN TypeColor          StdOutColorTitle, 
                                                                StdOutColorBGTitle, 
                                                                StdOutColorWindow, 
                                                                StdOutColorBorderWindow;
                                    
                                    NDEXTERN TypeColor          StdOutColorText, StdOutColorTextErr;
                                    
                                    NDEXTERN char               StdOutWindowHandle;
                                    
                                    
                                    /* 
                                       --------------------------------------------------------------------------------------------
                                       BUFFER INTERNO PER IL FUNZIONAMENTO DI ndLL_PRINTF, ndWS_PRINTF
                                       --------------------------------------------------------------------------------------------
                                    */
                                    
                                    NDEXTERN char IntBufPrintF [512];
                                    
                                   
                                    
       #undef NDEXTERN

#endif   // #define  _VARIABLES_CONTAINER_
       
