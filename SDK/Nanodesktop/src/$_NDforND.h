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
    Nanodesktop for Nanodesktop file - Code level
    ---------------------------------------------------------------------------
    Questo file speciale viene richiamato automaticamente da svariati altri
    files in diverse situazioni. Esso ha diverse funzioni:
          
    a) richiama dei files del SDK della PSP, in modo da far sì che determinate
       strutture del kernel della PSP siano riconoscibili per i moduli
       livello CODE
    b) provvede a richiamare gli headers della NanoC in modo da rendere 
       riconoscibili i simboli del C ai moduli del sistema
    c) provvede a richiamare $$_HAL_Variables.h e poi $$_HAL_Prototypes, in
       modo da rendere disponibili le variabili ed i prototipi necessari
       ai moduli di livello CODE. Le variabili dell'HAL saranno sempre
       dichiarate in extern, mentre tutti i prototipi dell'HAL vengono resi 
       disponibili ai moduli a livello CODE.
    d) provvede a richiamare $$_Variables.h e $$_Prototypes.h in modo da 
       rendere disponibili alle routines del livello CODE le variabili ed i 
       prototipi di livello CODE (ossia del medesimo livello). 
       E' necessario che il programma chiamante abbia già forzato
       (ove necessario), la dichiarazione reale dei simboli per mezzo
       dell'opzione I_NEED_TO_DECLARE_VARIABLES. I prototipi delle routines
       di livello CODE, invece, verranno sempre resi completamente disponibii: 
       non è necessario che sia immessa l'opzione I_NEED_TO_DEFINE_ALL_PROTOTYPES
    e) provvede a richiamare $$_Macro.h per rendere disponibili le macro di
       Nanodesktop
    f) provvede a richiamare $$_Icons.h rendendo eventualmente disponibili anche
       alle routine di livello CODE i simboli relativi alle icone. Se il programma
       chiamante ha definito I_NEED_TO_DECLARE_VARIABLES, i dati relativi alle
       icone saranno effettivamente immessi nel file oggetto risultante dalla compilazione,
       altrimenti verrà immesso un riferimento EXTERN. 
    g) provvede a richiamare $$_Fonts.h rendendo eventualmente disponibili anche
       alle routine di livello CODE i simboli relativi ai font di sistema. 
       Se il programma chiamante ha definito I_NEED_TO_DECLARE_VARIABLES, i dati relativi ai
       font saranno effettivamente immessi nel file oggetto risultante dalla compilazione,
       altrimenti verrà immesso un riferimento EXTERN. 
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
  
            #define ND_LIBRARY                  // Include i files senza includere la macro di piattaforma o il core di sistema
            
            #if (defined PSPE_PLATFORM)
            
                   #include <HAL_PSPE\ndHAL_PlatformHeaders.h>
                   // Richiama il file PlatformHeaders per PSPE
            
            #endif     
        
            #if ((defined PSP_PLATFORM) && (!defined ND_KERNELSERVICES_TO_USER_MAIN))
        
                   #include <HAL_PSP\ndHAL_PlatformHeaders.h>
                   // Richiama il file PlatformHeaders per PSP (è possibile che sia
                   // attivata la modalità kernel mode, ma la modalità KSU è 
                   // disattivata)
                   
            #endif
        
            #if ((defined PSP_PLATFORM) && (defined ND_KERNELSERVICES_TO_USER_MAIN))
        
                   #include <HAL_KSU_PSP\ndHAL_PlatformHeaders.h>
                   // Richiama il file PlatformHeaders per PSP (modalità KSU attivata)
                       
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
               INCLUDE DI VARIABILI A LIVELLO HAL
               -----------------------------------------------------------------------------
            */
            
            // Include le variabili a livello HAL. Le variabili sono specificate tutte come
            // EXTERN 
            
            
            #if (defined PSPE_PLATFORM)
            
                   #include <HAL_PSPE\$$_HAL_Variables.h>
                   // Richiama il file delle variabili HAL per PSPE
            
            #endif     
            
            
        
            #if ((defined PSP_PLATFORM) && (!defined ND_KERNELSERVICES_TO_USER_MAIN))
        
                   #include <HAL_PSP\$$_HAL_Variables.h>
                   // Richiama il file delle variabili HAL per PSP (senza KSU)
                   
            #endif
            
            
        
            #if ((defined PSP_PLATFORM) && (defined ND_KERNELSERVICES_TO_USER_MAIN))
        
                   #include <HAL_KSU_PSP\$$_HAL_Variables.h>
                   // Richiama il file delle variabili HAL per PSP (modalità KSU attivata)
                       
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
                CODICI INFO
                -----------------------------------------------------------------------------
             */
                            
             #include <$$_InfoCode.h>
             
             /*
                 -----------------------------------------------------------------------------
                 INCLUDE DI CODICI INFO SPECIFICI PER LA PIATTAFORMA
                 -----------------------------------------------------------------------------
             */
                            
            #if (defined PSPE_PLATFORM)
            
                   #include <HAL_PSPE\$$_HAL_InfoCode.h>
                   // Richiama il file dei codici info HAL per PSPE
            
            #endif     
            
            
        
            #if ((defined PSP_PLATFORM) && (!defined ND_KERNELSERVICES_TO_USER_MAIN))
        
                   #include <HAL_PSP\$$_HAL_InfoCode.h>
                   // Richiama il file dei codici info HAL per PSP (senza KSU)
                   
            #endif
            
            
        
            #if ((defined PSP_PLATFORM) && (defined ND_KERNELSERVICES_TO_USER_MAIN))
        
                   #include <HAL_KSU_PSP\$$_HAL_InfoCode.h>
                   // Richiama il file dei codici info HAL per PSP (modalità KSU attivata)
                       
            #endif
        
                           
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI PROTOTIPI A LIVELLO HAL
               -----------------------------------------------------------------------------
            */
            
            // Adesso include i prototipi dell'HAL. I moduli hanno sempre accesso a tutte 
            // le funzionalità comprese quelle di tipo INTERNAL
            
            #define HAL_I_NEED_TO_DEFINE_ALL_PROTOTYPES
            
                    
                    #if (defined PSPE_PLATFORM)
                    
                           #include <HAL_PSPE\$$_HAL_Prototypes.h>
                           // Richiama il file dei prototipi HAL per PSPE
                    
                    #endif     
                    
                    
                
                    #if ((defined PSP_PLATFORM) && (!defined ND_KERNELSERVICES_TO_USER_MAIN))
                
                           #include <HAL_PSP\$$_HAL_Prototypes.h>
                           // Richiama il file dei prototipi HAL per PSP (senza KSU)
                           
                    #endif
                    
                    
                
                    #if ((defined PSP_PLATFORM) && (defined ND_KERNELSERVICES_TO_USER_MAIN))
                
                           #include <HAL_KSU_PSP\$$_HAL_Prototypes.h>
                           // Richiama il file dei prototipi HAL per PSP (modalità KSU attivata)
                               
                    #endif
                      
             
             
                            
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
            
            // Adesso include i dati per le icone di sistema. Se l'utente ha settato la
            // variabile I_NEED_TO_DECLARE_VARIABLES, i dati relativi alle icone verranno
            // memorizzati nel file oggetto, altrimenti verrà inserito un riferimento
            // extern
            
                    #include <$$_Icons.h>
            
            /* 
               -----------------------------------------------------------------------------
               INCLUDE DI RIFERIMENTI EXTERN PER I FONT
               -----------------------------------------------------------------------------
            */
            
            // Adesso include i dati per i font di sistema. Se l'utente ha settato la
            // variabile I_NEED_TO_DECLARE_VARIABLES, i dati relativi ai font verranno
            // memorizzati nel file oggetto, altrimenti verrà inserito un riferimento
            // extern
            
            
                    #include <$$_Fonts.h>
            
        
#endif
