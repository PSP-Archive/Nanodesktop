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
    Nanodesktop HAL Platform Headers (PSP KSU mode)
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
    ------------------------------------------------------------------------------
    INFORMAZIONI SULLA MODALITA' KSU:
    ------------------------------------------------------------------------------
                 
    L'utente potrebbe voler generare delle applicazioni in user-mode, ma avere la
    necessità di funzionalità (come il raw-ir) che sono disponibili solo in kernel
    mode. A tale scopo, l'utente può usare la versione KSU delle librerie 
    Nanodesktop per PSP.
    
    Questa versione usa una main custom che viene integrata dal file 
    ndHAL_PlatformHeaders.h nel codice sorgente dell'applicazione, 
    e che ha la funzione di avviare il thread principale in kernel mode, e usarlo 
    esclusivamente per alcune routines server che possono girare solo
    nel main thread in kernel mode (RawIRServer è un esempio). Dopo avere avviato
    tali routines server, il main richiama la routine UserMain, e la lancia in
    un thread di tipo user. 
   
    L'utente deve esclusivamente ricordarsi che il suo codice deve iniziare con
    una routine UserMain e non con una routine main: tutto il resto del processo
    è totalmente automatico e trasparente all'applicazione
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
                        
                        #include <HAL_KSU_PSP/$$_HAL_NetworkPatch.h>

                        // Supporto audio
                        
                        #include <pspaudio.h>
                        #include <pspaudiolib.h>
                        #include <pspaudiocodec.h>
                        
                        // Supporto caricamento altri homebrew
                        
                        #include <psploadexec.h>
                        
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
                
                // Per la modalità KSU vengono creati alcuni simboli per la configurazione del
                // sistema KSU, insieme ad una macro di piattaforma speciale
                
                #ifdef NDHAL_PTHD_I_NEED_PLATFORM_MACRO
                
                      PSP_MODULE_INFO("NanodesktopApp", 0x1000, 1, 1);               // Abilita l'accesso alla memoria del kernel
                      PSP_MAIN_THREAD_ATTR(0);                                       // Main thread in kernel mode   
                      PSP_MAIN_THREAD_PRIORITY (38); 
                      
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
                                  
                       // USB SERVICE:
                              
                                  extern char ND_HWMODE_DisableUsbService;
                                  
                       // PHOENIX SUBSYSTEM:
                                  
                                  extern char ND_SWMODE_DisablePhoenixSubsystem;
                                  
                       // NETWORK SERVICES
                       
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
                
                      // RAWIR Server
                                   
                      #ifdef ND_DISABLE_RAWIR_SERVER
                               char ND_HWMODE_DisableRawIRService = 1;
                      #else
                               char ND_HWMODE_DisableRawIRService = 0;
                      #endif
                      
                      // USB Service
                                   
                      #ifdef ND_DISABLE_USB_SERVICES
                               char ND_HWMODE_DisableUsbService = 1;
                      #else
                               char ND_HWMODE_DisableUsbService = 0;
                      #endif
                      
                      // Phoenix Subsystem
                      
                      #ifdef ND_DISABLE_PHOENIX_SUBSYSTEM
                               char ND_SWMODE_DisablePhoenixSubsystem = 1;
                      #else
                               char ND_SWMODE_DisablePhoenixSubsystem = 0;
                      #endif
                      
                      // Network services
                      
                      #ifdef ND_DISABLE_NETWORK_SERVICES
                               char ND_HWMODE_DisableNetworkService = 1;
                      #else
                               char ND_HWMODE_DisableNetworkService = 0;
                      #endif
                      
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
                
                        
                      // GESTORE LOADER-INIT
                      // Questa sezione del codice si occupa di patchare il kernel della
                      // PSP in modo da rendere possibile il caricamento dei moduli del
                      // kernel: necessario per il corretto funzionamento del driver
                      // USB. Queste istruzioni sono eseguite in automatico da crt0.c
                      // prima di richiamare il codice del main    
                          
                          __attribute__ ((constructor)) void loaderInit()
                          {
                                pspKernelSetKernelPC();
                                pspSdkInstallNoPlainModuleCheckPatch();
                                pspSdkInstallNoDeviceCheckPatch();
                            	pspSdkInstallKernelLoadModulePatch();
                          }

                      // SEZIONE MAIN
                      // Questa sezione contiene il vero main dell'homebrew. In modalità 
                      // KSU, il vero main fa prima partire i servizi del loop KSU, e
                      // successivamente fa parte il codice utente contenuto dentro
                      // UserMain
                                                     
                        #ifdef __cplusplus
                        extern "C" {
                        #endif


                          void ndHAL_KSU_BootManager ();
                          
                          extern struct       SceKernelLoadExecParam ndLoadExecStdStruct;
                          extern const char   ndTransExecutionFileName [128];
                          extern char         ndTransExecutionRequested;          
                          
                          int main (void)
                          {
                              ndHAL_KSU_BootManager ();
                              
                              // Esegue eventuale transesecuzione
              
                              if (ndTransExecutionRequested)    
                              {  
                                 sceKernelLoadExec ( (const char *)(&ndTransExecutionFileName), &ndLoadExecStdStruct);  
                                 sceKernelDelayThread (2000000);
                              }
                              
                              // Arresta il sistema
                              
                              return 0;
                          }
                        
                        
                          
                        #ifdef __cplusplus
                        }
                        #endif
                
                #endif
                
                /*
                    --------------------------------------------------------------------------------
                    SIMBOLO DI START ndMain
                    --------------------------------------------------------------------------------
                */
                 
                // ndMain non viene wrappato in un main da una macro, come negli altri HAL, 
                // ma deve essere definito esplicitamente dall'utente
                
                
                
                
#endif          // #ifndef NDHAL_PTHD_ALREADY_COMPILED
