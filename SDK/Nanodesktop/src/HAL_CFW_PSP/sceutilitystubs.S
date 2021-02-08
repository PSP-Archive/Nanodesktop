#include "pspstub.s"


STUB_START "sceUtility",0x40010000,0x00020005 
    STUB_FUNC 0xC629AF26,sceUtilityLoadAvModule 
    STUB_FUNC 0xF7D8D092,sceUtilityUnloadAvModule 
STUB_END 

  