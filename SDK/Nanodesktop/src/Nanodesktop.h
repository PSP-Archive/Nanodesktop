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
    File headers per le applicazioni Nanodesktop
    ----------------------------------------------------------------------------
*/

// Le seguenti righe di codice consentono di far sì che il presente file headers
// sia usato esclusivamente una volta, anche se viene richiamato più volte.

#ifndef _NANODESKTOP_H
        #define    _NANODESKTOP_H

        
        /* 
            ----------------------------------------------------------------------------
            DISCRIMINATORE PER LA PIATTAFORMA 
            ----------------------------------------------------------------------------
            Questa sezione è progettata per fare in modo che i programmi generino un 
            errore di compilazione, se non viene dichiarata la piattaforma di 
            compilazione. Una sezione simile è presente dentro NanoC.h
            
            Se la piattaforma di compilazione è stata dichiarata, il sistema provvederà
            a caricare il file ndHAL_PlatformHeaders opportuno
            -----------------------------------------------------------------------------
        */
        
        
        #if ( (!defined PSPE_PLATFORM) && (!defined PSP_PLATFORM) )
        
              #error  No platform variabile defined. Use -D"PSP_PLATFORM" or -D"PSPE_PLATFORM" 
              // Questo genererà un errore di compilazione se non viene dichiarata la 
              // piattaforma di compilazione
              
        #endif
        
        /* 
            ----------------------------------------------------------------------------
            DISCRIMINATORE PER I SIMBOLI ND_KERNELSERVICES_TO_USER_MAIN E 
            ND_DESIGNED_FOR_CFW
            ----------------------------------------------------------------------------
            Questa sezione è progettata per fare in modo che i programmi generino un 
            errore di compilazione, qualora si cerchi di usare contemporaneamente
            la modalità CFW e KSU
            -----------------------------------------------------------------------------
        */
        
        #if ( (defined ND_DESIGNED_FOR_CFW) && (defined ND_KERNELSERVICES_TO_USER_MAIN) )
    
            #error  You cannot use CFW mode and KSU mode in the same time 
            // Questo genererà un errore di compilazione se non viene dichiarata la 
            // piattaforma di compilazione
          
        #endif
    
        
        /* 
            ----------------------------------------------------------------------------
            GENERAZIONE DI META SIMBOLI DI PIATTAFORMA 
            ----------------------------------------------------------------------------
            Genera dei simboli di piattaforma che hanno la funzione di indirizzare il
            sistema nei passaggi successivi verso la scelta dell'header opportuno
            -----------------------------------------------------------------------------
        */
        
        #ifdef PSPE_PLATFORM
               #define __METACFG_PSPE
        #else
               #ifdef PSP_PLATFORM
               
                      #ifdef ND_DESIGNED_FOR_CFW
                             #define __METACFG_PSP_CFW
                      #else
                             #ifdef ND_KERNELSERVICES_TO_USER_MAIN
                                    #define __METACFG_PSP_KSU
                             #else
                                            #define __METACFG_PSP
                             #endif
                      
                      #endif
               
               #endif
        #endif
        
        
        /* 
            ----------------------------------------------------------------------------
            INCLUDE ndHAL_PlatformHeaders.h
            ----------------------------------------------------------------------------
            Se la piattaforma di compilazione è stata dichiarata, il sistema provvederà
            a caricare il file ndHAL_PlatformHeaders opportuno
            -----------------------------------------------------------------------------
        */
        
        #ifdef __METACFG_PSPE
               #include <HAL_PSPE\ndHAL_PlatformHeaders.h>
               // Richiama il file PlatformHeaders per PSPE
        #endif     
        
        #ifdef __METACFG_PSP  
               #include <HAL_PSP\ndHAL_PlatformHeaders.h>
               // Richiama il file PlatformHeaders per PSP (è possibile che sia
               // attivata la modalità kernel mode, ma la modalità KSU è disattivata)
        #endif

        #ifdef __METACFG_PSP_KSU 
               #include <HAL_KSU_PSP\ndHAL_PlatformHeaders.h>
               // Richiama il file PlatformHeaders per PSP (modalità KSU attivata)           
        #endif
        
        #ifdef __METACFG_PSP_CFW 
               #include <HAL_CFW_PSP\ndHAL_PlatformHeaders.h>
               // Richiama il file PlatformHeaders per PSP (custom firmware)           
        #endif
        
        
        /* 
           -----------------------------------------------------------------------------
           RIFERIMENTI AL FILE HEADER DI DEV-IL
           -----------------------------------------------------------------------------
           Normalmente Nanodesktop supporta il caricamento ed il salvataggio dei file 
           esclusivamente in formato bitmap. 
   
           Tuttavia, esso può supportare anche altri formati grafici, per mezzo di un'
           interfaccia che gli consente di appoggiarsi alle routine della libreria grafica
           DEV-IL. Per usare questa interfaccia, è necessario che l'utente ricompili
           la libreria Nanodesktop con l'opzione -DI_HAVE_DEV_IL (oppure, che si
           linki l'eseguibile con una versione binaria della libreria che è stata
           compilata con tale opzione, per esempio, Nanodesktop_DEVIL_PSPE per
           piattaforme PSPE).
           -----------------------------------------------------------------------------
        */
  
        #ifdef I_HAVE_DEV_IL
               #include  <il.h>       
        #endif

        /* 
           -----------------------------------------------------------------------------
           RIFERIMENTI AL FILE HEADER DI FREETYPE
           -----------------------------------------------------------------------------
           Necessario per il supporto TrueType
           -----------------------------------------------------------------------------
        */
  
        #ifdef ND_ENABLE_TTF_SUPPORT 
              #include <ft2build.h>
              #include <freetype/freetype.h>
              #include <ndflib.h> 
        #endif                      

        /*
         -----------------------------------------------------------------------------
         INIZIO DEL CODICE
         -----------------------------------------------------------------------------
        */
        
        #ifdef __cplusplus
        extern "C" {
        #endif

                
                            /* 
                               -----------------------------------------------------------------------------
                               INCLUDE DI VARIABILI A LIVELLO HAL
                               -----------------------------------------------------------------------------
                            */
                            
                            // Include le variabili a livello HAL. Le variabili sono specificate tutte come
                            // EXTERN 
                            
                            #ifdef __METACFG_PSPE
                               #include <HAL_PSPE\$$_HAL_Variables.h>
                            #endif
                            
                            #ifdef __METACFG_PSP
                               #include <HAL_PSP\$$_HAL_Variables.h>
                            #endif
                            
                            #ifdef __METACFG_PSP_KSU
                               #include <HAL_KSU_PSP\$$_HAL_Variables.h>
                            #endif
                            
                            #ifdef __METACFG_PSP_CFW
                               #include <HAL_CFW_PSP\$$_HAL_Variables.h>
                            #endif
                            
                            
                            /* 
                               -----------------------------------------------------------------------------
                               INCLUDE DI VARIABILI A LIVELLO CODE
                               -----------------------------------------------------------------------------
                            */
                            
                            // Adesso include le variabili a livello CODE. Il programma chiamante deve avere
                            // già specificato se le variabili devono essere realmente dichiarate o se
                            // deve essere inserito solo un riferimento extern
                            
                            #include <$$_Variables.h>
                            
                            /* 
                               -----------------------------------------------------------------------------
                               INCLUDE DI CODICI DI ERRORE
                               -----------------------------------------------------------------------------
                            */
                            
                            #include <$$_Errors.h>
                            
                            /* 
                               -----------------------------------------------------------------------------
                               CODICI INFO A LIVELLO CODE 
                               -----------------------------------------------------------------------------
                            */
                            
                            #include <$$_InfoCode.h>
                            
                            /*
                               -----------------------------------------------------------------------------
                               INCLUDE DI CODICI INFO SPECIFICI PER LA PIATTAFORMA
                               -----------------------------------------------------------------------------
                            */
                            
                            #ifdef __METACFG_PSPE
                               #include <HAL_PSPE\$$_HAL_InfoCode.h>
                            #endif
                            
                            #ifdef __METACFG_PSP
                               #include <HAL_PSP\$$_HAL_InfoCode.h>
                            #endif
                            
                            #ifdef __METACFG_PSP_KSU
                               #include <HAL_KSU_PSP\$$_HAL_InfoCode.h>
                            #endif
                            
                            #ifdef __METACFG_PSP_CFW
                               #include <HAL_CFW_PSP\$$_HAL_InfoCode.h>
                            #endif
                            
                            /* 
                               -----------------------------------------------------------------------------
                               INCLUDE DI PROTOTIPI A LIVELLO HAL
                               -----------------------------------------------------------------------------
                            */
                            
                            // Adesso include i prototipi dell'HAL. Le applicazioni esterne non avranno
                            // accesso alle routines di tipo INTERNAL
                            
                            #ifdef __METACFG_PSPE
                               #include <HAL_PSPE\$$_HAL_Prototypes.h>
                            #endif
                            
                            #ifdef __METACFG_PSP
                               #include <HAL_PSP\$$_HAL_Prototypes.h>
                            #endif
                            
                            #ifdef __METACFG_PSP_KSU
                               #include <HAL_KSU_PSP\$$_HAL_Prototypes.h>
                            #endif
                            
                            #ifdef __METACFG_PSP_CFW
                               #include <HAL_CFW_PSP\$$_HAL_Prototypes.h>
                            #endif
                            
                            /* 
                               -----------------------------------------------------------------------------
                               INCLUDE DI PROTOTIPI A LIVELLO CODE
                               -----------------------------------------------------------------------------
                            */
                            
                            // Adesso include i prototipi a livello CODE. Le applicazioni esterne non
                            // avranno accesso alle routines di tipo INTERNAL
                            
                                    #include <$$_Prototypes.h>
                            
                            
                            
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
                            
                            // Adesso include i dati per le icone di sistema. Verrà sempre inserito un
                            // riferimento extern
                            
                                    #include <$$_Icons.h>
                            
                            /* 
                               -----------------------------------------------------------------------------
                               INCLUDE DI RIFERIMENTI EXTERN PER I FONT
                               -----------------------------------------------------------------------------
                            */
                            
                            // Adesso include i dati per i font di sistema. Verrà sempre inserito un 
                            // riferimento extern
                            
                                    #include <$$_Fonts.h>
                            
                            
                          
        
        #ifdef __cplusplus
        }
        #endif

#endif    //  #ifndef _NANODESKTOP_H
