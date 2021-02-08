/*  ------------------------------------------------------------------------
    Kernel Extender Driver for Nanodesktop PSP
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

#define ND_NOT_MAIN_SOURCE
#include <nanodesktop.h>

#include <pspkernel.h>
#include <pspsdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ndkrnextender.h>




PSP_MODULE_INFO(ND002_MODULE_NAME, PSP_MODULE_KERNEL, 1, 0);
PSP_MAIN_THREAD_ATTR(0);
PSP_MAIN_THREAD_STACK_SIZE_KB (3);


int sceAudioSetFrequency (int frequency); 
int FW371_sceCtrlReadBufferPositive (SceCtrlData *pad_data, int count);
int FW3XX_sceCtrlReadBufferPositive (SceCtrlData *pad_data, int count);





void KRNEXT_PRX_pspAudioInit (void)
{
    int k1;

    k1 = psplinkSetK1 (0);
    pspAudioInit ();
    psplinkSetK1 (k1);
}

void KRNEXT_PRX_pspAudioEnd  (void)
{
    int k1;

    k1 = psplinkSetK1 (0);
    pspAudioEnd ();
    psplinkSetK1 (k1); 
}

int KRNEXT_PRX_sceAudioSetFrequency (int frequency)
{
    int k1, ret;

    k1 = psplinkSetK1 (0);
    ret = sceAudioSetFrequency (frequency);
    psplinkSetK1 (k1); 
    
    return ret;
}

int KRNEXT_PRX_sceCtrlReadBufferPositiveDetectHome (int FwVersion, void *MyPspPadRecord, int NrRecords)
{
    int k1, ret;

    k1 = psplinkSetK1 (0);
    
    if (FwVersion<0x03070110)
       ret = FW3XX_sceCtrlReadBufferPositive (MyPspPadRecord, 1);
    else
       ret = FW371_sceCtrlReadBufferPositive (MyPspPadRecord, 1); 
    
    psplinkSetK1 (k1); 
    
    return ret;  
}






/* Entry point */
int module_start(SceSize args, void *argp)
{       
        return 0;
}

/* Module stop entry */
int module_stop(SceSize args, void *argp)
{       
        return 0;
}

