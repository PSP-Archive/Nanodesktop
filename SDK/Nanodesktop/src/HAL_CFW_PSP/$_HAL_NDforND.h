/*
    ----------------------------------------------------------------------------
    Nanodesktop Library (version 0.3.4)
    Copyright 2006-2011 Filippo Battaglia
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
    Nanodesktop for Nanodesktop file (HAL) (PSP CFW mode)
    ---------------------------------------------------------------------------
    Questo file speciale viene richiamato automaticamente da svariati altri
    files in diverse situazioni. Esso ha diverse funzioni:
          
    a) richiama dei files del SDK della PSP, in modo da far sì che determinate
       strutture del kernel della PSP siano riconoscibili per i moduli
       dell'HAL
    b) provvede a richiamare gli headers della NanoC in modo da rendere 
       riconoscibili i simboli del C ai moduli del sistema
    c) provvede a richiamare $$_HAL_Variables.h e poi $$_HAL_Prototypes, in
       modo da rendere disponibili le variabili ed i prototipi necessari
       all'HAL: è necessario che il programma chiamante abbia già eventualmente
       forzato (ove necessario), la dichiarazione reale dei simboli per mezzo
       dell'opzione HAL_I_NEED_TO_DECLARE_VARIABLES. I prototipi invece
       saranno sempre resi totalmente disponibili all'HAL. 
    d) provvede a richiamare $$_Variables.h e $$_Prototypes.h in modo da 
       rendere disponibili alle routines dell'HAL le variabili ed i prototipi 
       di livello CODE. Durante questa chiamata, verrà sempre richiesto che 
       tutte le variabili di livello CODE siano dichiarate EXTERN e che tutti
       i prototipi siano resi disponibili all'HAL. 
    e) provvede a richiamare $$_Macro.h per rendere disponibili le macro di
       Nanodesktop
    f) provvede a richiamare $$_Icons.h rendendo eventualmente disponibili anche
       alle routine dell'HAL i simboli relativi alle icone (tutti i riferimenti
       sono extern)
    g) provvede a richiamare $$_Fonts.h rendendo eventualmente disponibili anche
       alle routine dell'HAL i simboli relativi ai font di sistema (tutti i riferimenti
       sono extern)
    -------------------------------------------------------------------------------
    SI TENGA SEMPRE BENE PRESENTE CHE QUESTO FILE NON DEVE ASSOLUTAMENTE
    ESSERE RICHIAMATO DALL'UTENTE FINALE, CHE PUO' USARE SOLO NANODESKTOP.H
    -------------------------------------------------------------------------------
*/


#ifndef _NDHAL_ND_FOR_ND_

            #define _NDHAL_ND_FOR_ND_
      
            /* 
                ----------------------------------------------------------------------------
                DISCRIMINATORE PER LA PIATTAFORMA PSP/PSPE
                ----------------------------------------------------------------------------
                Questa sezione è progettata per fare in modo che i programmi generino un 
                errore di compilazione, se non viene dichiarata la piattaforma di 
                compilazione. Una sezione simile è presente dentro NanoC.h
                -----------------------------------------------------------------------------
            */
            
            #if ( (!defined PSPE_PLATFORM) && (!defined PSP_PLATFORM) )
        
                #error  No platform variabile defined. Use -D"PSP_PLATFORM" or -D"PSPE_PLATFORM" 
                // Questo genererà un errore di compilazione se non viene dichiarata la 
                // piattaforma di compilazione
              
            #endif
        
            
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI ALTRI HEADERS COMPATIBILI CON LA PIATTAFORMA
               -----------------------------------------------------------------------------
            */
  
            #define ND_LIBRARY                  
            // Include i files senza includere la macro di piattaforma o il core di sistema
            
            #define ND_PSP_ENABLE_FW371_EXTENSIONS
            // Include le estensioni per il custom firmware
            
            #include <HAL_CFW_PSP\ndHAL_PlatformHeaders.h>
            // Richiama il file PlatformHeaders per PSPE
            
            
                    
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI VARIABILI A LIVELLO HAL
               -----------------------------------------------------------------------------
            */
            
            // Include le variabili a livello HAL. Il programma che ha chiamato HAL_NDforND.h
            // deve già avere specificato se vuole che le variabili siano dichiarate tutte
            // come extern, o se vuole una vera dichiarazione 
            
            #include <HAL_CFW_PSP\$$_HAL_Variables.h>
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI VARIABILI A LIVELLO CODE
               -----------------------------------------------------------------------------
            */
            
            // Adesso include le variabili a livello CODE. I moduli dell'HAL hanno sempre accesso
            // a tutte le variabili come extern
            
            #include <$$_Variables.h>
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI CODICI DI ERRORE
               -----------------------------------------------------------------------------
            */
                            
            #include <$$_Errors.h>
            
            /* 
               -----------------------------------------------------------------------------
               CODICI INFO
               -----------------------------------------------------------------------------
            */
                            
            #include <$$_InfoCode.h>
            #include <HAL_CFW_PSP\$$_HAL_InfoCode.h>
                            
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI PROTOTIPI A LIVELLO HAL
               -----------------------------------------------------------------------------
            */
            
            // Adesso include i prototipi dell'HAL. I moduli hanno sempre accesso a tutte 
            // le funzionalità comprese quelle di tipo INTERNAL
            
            #define HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES
            
                    #include <HAL_CFW_PSP\$$_HAL_Prototypes.h>
                    
            #undef  HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI PROTOTIPI A LIVELLO CODE
               -----------------------------------------------------------------------------
            */
            
            // Adesso include i prototipi a livello CODE. I moduli hanno sempre accesso a
            // tutte le funzionalità comprese quelle di tipo INTERNAL
            
            #define I_NEED_TO_DEFINE_ALL_PROTOTYPES
            
                    #include <$$_Prototypes.h>
                    
            #undef  I_NEED_TO_DEFINE_ALL_PROTOTYPES
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI MACRO
               -----------------------------------------------------------------------------
            */
            
            // Adesso include le macro di sistema
            
                    #include <$$_Macro.h>
                    
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI RIFERIMENTI EXTERN PER LE ICONE
               -----------------------------------------------------------------------------
            */
            
            // Adesso include i riferimenti extern per le icone di sistema
            
                    #include <$$_Icons.h>
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI RIFERIMENTI EXTERN PER I FONT
               -----------------------------------------------------------------------------
            */
            
            // Adesso include i riferimenti extern per i font di sistema
            
                    #include <$$_Fonts.h>
            
            
        
#endif
