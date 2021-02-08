.set noreorder 

#include "pspimport.s"

   IMPORT_START   "sceAudio_driver",0x00010000 
   IMPORT_FUNC    "sceAudio_driver",0xA2BEAA6C,sceAudioSetFrequency 
   
   
   IMPORT_START  "sceCtrl_driver",0x00010000 
   IMPORT_FUNC   "sceCtrl_driver",0x6D74BF08,FW371_sceCtrlSetSamplingCycle 
   IMPORT_FUNC   "sceCtrl_driver",0x6A2774F3,FW3XX_sceCtrlSetSamplingCycle 
       
   IMPORT_FUNC   "sceCtrl_driver",0x28E71A16,FW371_sceCtrlSetSamplingMode 
   IMPORT_FUNC   "sceCtrl_driver",0x1F4011E6,FW3XX_sceCtrlSetSamplingMode 
   
   IMPORT_FUNC   "sceCtrl_driver",0x454455AC,FW371_sceCtrlReadBufferPositive 
   IMPORT_FUNC   "sceCtrl_driver",0x1F803938,FW3XX_sceCtrlReadBufferPositive
   