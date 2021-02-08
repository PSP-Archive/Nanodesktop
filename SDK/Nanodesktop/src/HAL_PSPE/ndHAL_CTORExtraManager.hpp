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
    GESTORE EXTRAMANAGER SEED PER GESTIONE COSTRUTTORI E DISTRUTTORI GLOBALI C++
    ----------------------------------------------------------------------------
    Il funzionamento di questo file è molto particolare. Esso viene incluso in
    ogni file .cpp compilato per PSPE automaticamente. 
    
    Si veda il file <ndenv path>\PSP\SDK\Tools\ndGenCtor\ndGENCTor.c per ulteriori
    informazioni
    ----------------------------------------------------------------------------
*/

#ifndef _CTOR_EXTRAMANAGER_ALREADY_DEFINED
        #define _CTOR_EXTRAMANAGER_ALREADY_DEFINED
     
       
        #ifndef NDHAL_NO_CTOR_DTOR_EXTRAMANAGER
                           
                           #ifdef __cplusplus
                           extern "C"
                           {
                                    // L'extramanager per i costruttori e per i distruttori deve
                                    // essere generato solo per i file CPP e non per i file C
                         
                                            
                                    /*
                                            gas code to access elf sections
                                    */
                                    #define CTORS_SECTION_ASM_OP ".section\t.ctors,\"aw\""
                                    #define DTORS_SECTION_ASM_OP ".section\t.dtors,\"aw\""
                                    
                                    /*
                                            create a __CTOR_LIST__ from the section .dtors
                                    */
                                    asm (CTORS_SECTION_ASM_OP);     /* cc1 doesn't know that we are switching! */
                                    static void (* __CTOR_LIST__[512]) (void) __attribute__ ((section (".ctors"))) = 
                                    { (void (*) (void)) -1 };
                                    asm (".previous"); /* go back */
                                    
                                    /*
                                            create a __DTOR_LIST__ from the section .dtors
                                    */
                                    asm (DTORS_SECTION_ASM_OP);     /* cc1 doesn't know that we are switching! */
                                    static void (* __DTOR_LIST__[512]) (void) __attribute__ ((section (".dtors"))) = 
                                    { (void (*) (void)) -1 };
                                    asm (".previous"); /* go back */
                                    
                                    typedef void (*func_ptr)(void);
                                    
                                    
                                    /*
                                    run global destructors
                                    */
                                    static volatile void __attribute__((used)) CTORMGR_PSPE_do_global_dtors_aux (void)
                                    {
                                    	static func_ptr *p = __DTOR_LIST__;
                                    	void (*q)();
                                    	
                                        static int cppctorsdtors_done =0;
                                    	
                                    	int CodeRegister0;
                                        int CodeRegister1;
                                        int CodeRegister2;
                                        
                                    	if (cppctorsdtors_done)
                                    	return;
                                    			
                                    	// La tabella è sempre costituita da una sola voce. E' valida
                                    	// a certe condizioni
                                    	
                                    	CodeRegister0 = (int)(*p);
                                    	p--;
                                    	CodeRegister1 = (int)(*p);
                                    	p--;
                                    	CodeRegister2 = (int)(*p);
                                    	
                                    	if ( (CodeRegister0==0xFFFFFFFF) && (CodeRegister1>0x8900000) ) 
                                    	{
                                            q = (func_ptr)(CodeRegister1);
                                            (*q)();           // Esegui la funzione di deinizializzazione                            
                                        }
                                    	
                                    	cppctorsdtors_done = 1;
                                    }
                                    
                                    /*
                                    run global constructors
                                    */
                                    static volatile void __attribute__((used)) CTORMGR_PSPE_do_global_ctors_aux (void)
                                    {
                                    	volatile static func_ptr *p = __CTOR_LIST__;
                                    	void (*q)();
                                        
                                        static int cppctorsdtors_done = 0;
                                    
                                        int CodeRegister0;
                                        int CodeRegister1;
                                        int CodeRegister2;
                                        
                                    	if (cppctorsdtors_done)
                                    	return;
                                    			
                                    	// La tabella è sempre costituita da una sola voce. E' valida
                                    	// a certe condizioni
                                    	
                                    	CodeRegister0 = (int)(*p);
                                    	p--;
                                    	CodeRegister1 = (int)(*p);
                                    	p--;
                                    	CodeRegister2 = (int)(*p);
                                    	
                                    	if ( (CodeRegister0==0xFFFFFFFF) && (CodeRegister1>0x8900000) ) 
                                    	{
                                            q = (func_ptr)(CodeRegister1);
                                            (*q)();           // Esegui la funzione di inizializzazione                            
                                        }
                                    	
                                    	cppctorsdtors_done = 1;
                                    }
                                           
                           }
                           #endif
                            
        
         #endif
        
#endif

