/*  ------------------------------------------------------------------------
    Kernel Extender Driver for Nanodesktop PSP - File Header
    Version 1.0
    ------------------------------------------------------------------------
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
    This driver is able to work around the main limitations of 
    PSP Custom firmware: the calls to kernel functions can be 
    done only by another PRX in kernel mode. 
   
    So, when Nanodesktop CFW HAL need to call a kernel function, it can
    do this via Kernel Extender Driver
   -----------------------------------------------------------------------------
*/

#ifndef __NDKERNELEXTENDER
        #define __NDKERNELEXTENDER
        
        #define ND002_MODULE_NAME "ndKRNExtender"
        
/*      
        ------------------------------------------------------------------------
        STRUCTURES AND PROTOTYPES FOR KERNEL MODE ACCESS 
        ------------------------------------------------------------------------
        This functions are ASM routines defined in kMode.S
        ------------------------------------------------------------------------
*/
        
        int psplinkSetK1(int k1);
       
/*      
        ------------------------------------------------------------------------
        PROTOTYPES
        ------------------------------------------------------------------------
*/

        void KRNEXT_PRX_pspAudioInit (void);
        void KRNEXT_PRX_pspAudioEnd  (void);
        
        int  KRNEXT_PRX_sceAudioSetFrequency   (int frequency);
        int  KRNEXT_PRX_sceCtrlReadBufferPositiveDetectHome (int FwVersion, void *PspPadRecord, int NrRecords);

        
#endif
