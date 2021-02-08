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
    HAL: FUNZIONI DI GESTIONE FIRMWARE (PSP CFW mode)
    ---------------------------------------------------------------------------
*/

#include <HAL_CFW_PSP\$_HAL_NDforND.h>


void INTERNAL_CheckIfPspSlim ()
{
    if (kuKernelGetModel() == PSP_MODEL_SLIM_AND_LITE) 
        ndIsPspSlim=1;
    else
        ndIsPspSlim=0;
}


int INTERNAL_CheckForFw150 (void)
/*
    Questa routine controlla se il firmware è un 1.50 della Sony. Se il
    controllo riesce, il sistema restituisce 1 alla routine chiamante, 
    altrimenti restituisce 0
*/
{
    int Version0, Version1;
    
    ndStrCpyBounded (&FwBuffer0, &FwBuffer, 0, 7, 0);
    if (strcmp (&FwBuffer0, "release:")!=0) return 0;    // Stringa non corrispondente
    
    ndStrCpyBounded (&FwBuffer1, &FwBuffer, 8, 8, 0);
    Version0 = atoi (&FwBuffer1);
    
    ndStrCpyBounded (&FwBuffer2, &FwBuffer, 10, 11, 0); 
    Version1 = atoi (&FwBuffer2);
    
    if ((Version0==1) && (Version1==50))
    {
       ndPspFwVersion    = NDPSPFW_SONY + (Version0*0x100) + Version1;
       ndPspFwEmuVersion = sceKernelDevkitVersion();
    
       return 1;      // Segnala successo   
    }
    else
       return 0;      // Il firmware non corrisponde. Questo non è un 1.50
}

int INTERNAL_CheckForCustomFirmware0 (void)
/*
    Questa routine controlla se il firmware è un custom firmware. Se il
    controllo riesce, il sistema restituisce 1 alla routine chiamante, 
    altrimenti restituisce 0
*/
{
    int Version0, Version1;

    ndStrCpyBounded (&FwBuffer0, &FwBuffer, 0, 7, 0);
    if (strcmp (&FwBuffer0, "release:")!=0) return 0;    // Stringa non corrispondente
    
    ndStrCpyBounded (&FwBuffer1, &FwBuffer, 8, 8, 0);
    Version0 = atoi (&FwBuffer1);
    
    ndStrCpyBounded (&FwBuffer2, &FwBuffer, 10, 11, 0); 
    Version1 = atoi (&FwBuffer2);
    
    ndPspFwVersion    = NDPSPFW_CF + (Version0*0x100) + Version1;
    ndPspFwEmuVersion = sceKernelDevkitVersion();
    
    return 1;      // Segnala successo   
}


void ndHAL_InitFirmwareCodes (void)
/*
    Questa routine provvede ad accedere a basso livello al firmware di sistema 
    e ne determina le caratteristiche. I risultati vengono memorizzati nelle
    variabili ndPspFwVersion e ndPspFwEmuVersion, due variabili a 32 bit.
    
    Questa routine viene eseguita una sola volta all'avvio del sistema
*/
{   
    int fhandle;
    
    INTERNAL_CheckIfPspSlim ();          // Controlla se si è su una PSP slim
    
    ndPspFwVersion = NDPSPFW_UNKNOWN;   // Se il sistema non riesce a determinare il firmware
                                        // resterà a firmware unknown
    
    fhandle = sceIoOpen ("flash0:/vsh/etc/version.txt", O_RDONLY, 777);
	
	if (fhandle>=0)
	{
        sceIoRead (fhandle, &FwBuffer, 32);
        // Porta i primi 32 caratteri nella stringa FwBuffer
        
        sceIoClose (fhandle);
    
        sceKernelDelayThread (500000);
    	
    	if (  INTERNAL_CheckForFw150()  ) return;
    	if (  INTERNAL_CheckForCustomFirmware0()  ) return;
    }	 	
}

/*
   ----------------------------------------------------------------------------
   CARICAMENTO MODULI
   ----------------------------------------------------------------------------
*/


int ndHAL_PseudoPspSdkLoadStartModule(char *path, int partition) 
/*
     Sui custom firmware non è possibile usare la normale funzione pspsdkLoadStartModule,
     perchè implicherebbe il caricamento della libreria pspsdk per soddisfare le dipendenze
     del linker e questo, a sua volta, costringerebbe l'eseguibile prx a commutare in kernel
     mode, provocando l'uscita dal sistema operativo della PSP con codice di errore
     8002014C. Per tale ragione, si usa questa routine alternativa
     
     Viene restituito 0 se non vi sono errori. 
*/
{ 
    SceKernelLMOption option;
    SceUID modid;

    memset(&option, 0, sizeof(option));
    option.size = sizeof(option);
    option.mpidtext = partition;
    option.mpiddata = partition;
    option.position = 0;
    option.access = 1;

    modid = kuKernelLoadModule (path, 0, &option);
    if (modid < 0)
        return modid;

    return sceKernelStartModule(modid, 0, NULL, NULL, NULL);   
} 



