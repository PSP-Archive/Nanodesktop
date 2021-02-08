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
    Nanodesktop HAL Platform Headers (PSP)
    ----------------------------------------------------------------------------
    Questo file speciale può essere richiamato da diversi files:
    a) da Nanodesktop.h, durante la fase di compilazione di un applicazione o
       di una libreria;
    b) da $_NDforND.h e da $_HAL_NDforND.h, durante la fase di compilazione
       di un modulo di livello CODE o di un modulo di livello HAL.
       
    I file Nanodesktop.h, $_NDforND.h, e $_HAL_NDforND.h configurano 
    automaticamente delle costanti simboliche prima di richiamare questo
    file: in questo modo esse adattano il file alle proprie esigenze. 
    
    Le costanti simboliche che vengono configurate sono le seguenti:
             
    ND_NOT_MAIN_SOURCE:
     Il file deve essere incluso nella compilazione di un sorgente che non è il
     sorgente primario dell'applicazione (il sorgente primario dell'applicazione
     è quello che contiene il main() del programma)         
             
    ND_LIBRARY:
     Il file deve essere incluso nella compilazione di un sorgente di una 
     libreria
     
    Se non viene usata nessuna di queste costanti manifeste, il codice passa
    in modalità MAIN_SOURCE, una condizione che implica l'attivazione di
    tutte le sezioni di ndHAL_PlatformHeaders.h          
    ----------------------------------------------------------------------------
*/

#ifndef NDHAL_PTHD_ALREADY_COMPILED

        #define NDHAL_PTHD_ALREADY_COMPILED
        // Impedirà che questo componente possa essere compilato due volte consecutive

                /*
                    --------------------------------------------------------------------------
                    SEZIONE DI CONFIGURAZIONE
                    --------------------------------------------------------------------------
                    Questa sezione definisce delle costanti manifeste per il compilatore,
                    che a loro volta condizionano la compilazione di altre sezioni del
                    medesimo file. 
                    
                    Le costanti manifeste che vengono definite sono le seguenti:
                                
                    NDHAL_PTHD_I_NEED_PLATFORM_HEADERS:
                      Quando è definita la costante simbolica NDHAL_PTHD_I_NEED_PLATFORM_HEADERS,
                      il compilatore richiamerà automaticamente degli headers che sono specifici
                      per la piattaforma utilizzata. 
            
                    NDHAL_PTHD_I_NEED_PLATFORM MACRO:
                      Quando è definita la costante simbolica NDHAL_PTHD_I_NEED_PLATFORM MACRO,
                      il compilatore richiamerà automaticamente la macro di piattaforma
                      corretta.         
            
                    NDHAL_PTHD_I_NEED_EXTERN_CONFIG_FLAGS
                      Quando è definita la costante simbolica NDHAL_PTHD_I_NEED_EXTERN_CONFIG_FLAGS,
                      il sistema include nel codice dei semplici riferimenti extern alle variabili
                      HWMODE e SWMODE che informano il codice delle condizioni di configurazione
                      dell'homebrew
                    
                    NDHAL_PTHD_I_NEED_CONFIG_FLAGS
                      Quando è definita la costante simbolica NDHAL_PTHD_I_NEED_CONFIG_FLAGS,
                      il sistema include nel codice le variabili HWMODE e SWMODE che informano
                      il codice delle condizioni di configurazione dell'homebrew
                      
                    #define NDHAL_PTHD_I_NEED_CORE_UNIT
                      Include il codice core unit        
                    --------------------------------------------------------------------------
                */
                              
                   #if ( (!defined ND_NOT_MAIN_SOURCE) && (!defined ND_LIBRARY) )
                  // Esegui questa sezione solo se la libreria viene richiamata in un source
                  // principale
                
                       #define NDHAL_PTHD_I_NEED_PLATFORM_MACRO 
                       // Inserisce la macro di piattaforma
                       
                       #define NDHAL_PTHD_I_NEED_PLATFORM_HEADERS
                       // Inserisce gli headers compatibili con la piattaforma
                       
                       #define NDHAL_PTHD_I_NEED_CONFIG_FLAGS
                       // Inserisce i virtual stubs e le variabili HWMODE e SWMODE di configurazione
                       
                       #define NDHAL_PTHD_I_NEED_CORE_UNIT
                       // Farà sì che venga inserita l'unità di inizializzazione 
                       // dell'applicativo
                           
                  #else      // Nanodesktop.h è stato richiamato da una libreria o da un sorgente secondario
                
                       #define NDHAL_PTHD_I_NEED_PLATFORM_HEADERS
                       // Inserisce esclusivamente gli headers compatibili con la piattaforma
               
                       #define NDHAL_PTHD_I_NEED_EXTERN_CONFIG_FLAGS
                       // Inserisce i config flags in modalità extern
                             
                  #endif
                    
        


                /* 
                   -----------------------------------------------------------------------------
                   INCLUDE DI ALTRI HEADERS COMPATIBILI CON LA PIATTAFORMA
                   -----------------------------------------------------------------------------
                */
                  
                
                #ifdef NDHAL_PTHD_I_NEED_PLATFORM_HEADERS
                
                        #include <pspkernel.h>        // Per accesso alle routine a basso livello del kernel   
                        #include <pspdebug.h>         // Per accesso a funzioni di debug del kernel
                
                        #include <nanoc.h>            // Include i file headers pspkerneltypes.h, errno.h, init.h,
                                                      // malloc.h, string.h, stdio.h, stdlib.h, time.h, assert.h
                        
                        #include <math.h>             // Necessario per accesso alle funzioni matematiche
                        
                        #include <pspgu.h>            // Per supporto accelerazione video hardware
                        #include <pspsircs.h>         // Per supporto IRDA
                        
                        #include <pspdisplay.h>       // Per l'accesso al framebuffer della PSP
                        #include <pspctrl.h>          // Primitive per la gestione del controller della PSP
                              
                        #include <psppower.h>
                        #include <pspsdk.h>
                        #include <pspusb.h>
                        
                        // Supporto LAN
                        
                        #include <pspwlan.h>
                        
                        // Supporto di rete: funzioni del firmware della PSP
                        
                        #include <pspnet.h>
                        #include <pspnet_inet.h>
                        #include <pspnet_apctl.h>
                        #include <pspnet_resolver.h>
                        #include <psputility.h>
                        
                        // Funzioni BSD like
                        
                        #include <arpa/inet.h>
                        #include <netinet/in.h>
                        #include <netinet/tcp.h>
                        #include <sys/socket.h>
                        #include <sys/select.h>
                        #include <netdb.h>

                        // Carica il file di patch della rete
                        
                        #include <HAL_PSP/$$_HAL_NetworkPatch.h>
                        
                        // Supporto audio
                        
                        #include <pspaudio.h>
                        #include <pspaudiolib.h>
                        #include <pspaudiocodec.h>
                        
                        // Estensioni per supporto custom firmware
                        
                        
                        #ifdef ND_PSP_ENABLE_FW371_EXTENSIONS
                                #include <kubridge.h>
                                #include <pspsysmem_kernel.h>
                        #endif  
                        
      
                #endif
                              
                /*
                   -----------------------------------------------------------------------------
                   MACRO DI PIATTAFORMA
                   -----------------------------------------------------------------------------
                */
                
                #ifdef NDHAL_PTHD_I_NEED_PLATFORM_MACRO
                
                       #ifndef ND_PSP_KERNEL_MODE_APPLICATION
                               
                               PSP_MODULE_INFO("NanodesktopApp", 0, 1, 1);
                               PSP_MAIN_THREAD_ATTR (THREAD_ATTR_USER | THREAD_ATTR_VFPU);
                               
                               PSP_MAIN_THREAD_PRIORITY (38); 
                       
                       #else
                       
                               PSP_MODULE_INFO("NanodesktopApp", 0, 1, 1);                         // Abilita l'accesso al kernel (no memory access)
                               PSP_MAIN_THREAD_ATTR (0);                                           // Main thread in kernel mode  
                               
                               PSP_MAIN_THREAD_PRIORITY (38); 
                       
                       #endif
                
                       // Simboli di configurazione: disattivazione sistema Phoenix
                       
                       
                
                #endif
                
                /*
                    --------------------------------------------------------------------------------
                    RIFERIMENTI EXTERN ALLE VARIABILI DI CONFIGURAZIONE
                    --------------------------------------------------------------------------------
                    Questa sezione contiene dei semplici riferimenti extern alle variabili HWMODE
                    e SWMODE
                    --------------------------------------------------------------------------------
                */
                
                #ifdef NDHAL_PTHD_I_NEED_EXTERN_CONFIG_FLAGS
                
                       // RAW-IR SERVER:           
                
                                  extern char ND_HWMODE_DisableRawIRService;
                                  
                       // PHOENIX SUBSYSTEM:
                                  
                                  extern char ND_SWMODE_DisablePhoenixSubsystem;
                                  
                       // WLAN
                       
                                  extern char ND_HWMODE_DisableNetworkService; 
                                  
                       // MEDIA SERVICE
                           
                                  extern char ND_HWMODE_DisableMediaService;         
                                  
                       
                #endif
                
                
                /*
                    --------------------------------------------------------------------------------
                    VIRTUAL STUBS E VARIABILI DI CONFIGURAZIONE
                    --------------------------------------------------------------------------------
                    Questa sezione contiene dei puntatori a funzione per gestire l'inclusione dei
                    moduli esterni di Nanodesktop (virtual stubs). 
                    
                    Inoltre, contiene la definizione delle variabili di configurazione SWMODE ed
                    HWMODE
                    
                    Questa sezione non deve essere inclusa nelle librerie o nei sorgenti secondari
                    delle applicazioni
                    --------------------------------------------------------------------------------
                */
                
                #ifdef NDHAL_PTHD_I_NEED_CONFIG_FLAGS
                
                       // RAW-IR SERVER
                       
                                  char ND_HWMODE_DisableRawIRService = 1;
                
                                  // Sotto PSP, la modalità RawIR non è disponibile: l'eventuale
                                  // tag ND_DISABLE_RAWIR_SERVER verrà sempre ignorato. 
                       
                       // PHOENIX SUBSYSTEM
                       
                                  #ifdef ND_DISABLE_PHOENIX_SUBSYSTEM
                                         char ND_SWMODE_DisablePhoenixSubsystem = 1;
                                  #else
                                         char ND_SWMODE_DisablePhoenixSubsystem = 0;
                                  #endif
                        
                       // WLAN
                       
                                  char ND_HWMODE_DisableNetworkService = 1;           
                                  // In modalità PSP le funzionalità WLAN sono sempre
                                  // disattivate
                                  
                       // Media service
                      
                                  #ifdef ND_DISABLE_MEDIA_SERVICES
                                           char ND_HWMODE_DisableMediaService = 1;
                                  #else
                                           char ND_HWMODE_DisableMediaService = 0;
                                  #endif
                                  
                       
                #endif 
                
                /*
                    --------------------------------------------------------------------------------
                    SEZIONE CORE
                    --------------------------------------------------------------------------------
                    La sezione core comprende del codice sorgente che viene incluso automaticamente
                    nel sorgente dell'homebrew, e che serve a gestire la modalità KSU oppure il
                    caricamento dei moduli del kernel da parte di ctr0.c. 
                    
                    Questa sezione non deve essere inclusa nelle librerie o nei sorgenti secondari
                    delle applicazioni
                    --------------------------------------------------------------------------------
                */
                
                #ifdef NDHAL_PTHD_I_NEED_CORE_UNIT 
                
                       
                
                #endif
                
                
                /*
                    --------------------------------------------------------------------------------
                    SIMBOLO DI START ndMain
                    --------------------------------------------------------------------------------
                */
                 
                #define ndMain  main
                
                
                
                
#endif          // #ifndef NDHAL_PTHD_ALREADY_COMPILED
