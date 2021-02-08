	.set noreorder

#include "pspstub.s"

	STUB_START "ndKrnExtender",0x40090000,0x00040005
	STUB_FUNC  0x82BA7282,KRNEXT_PRX_pspAudioInit
	STUB_FUNC  0xC6992FE0,KRNEXT_PRX_pspAudioEnd
	STUB_FUNC  0xEF6EBEA7,KRNEXT_PRX_sceAudioSetFrequency
	STUB_FUNC  0x5DF4339A,KRNEXT_PRX_sceCtrlReadBufferPositiveDetectHome
	STUB_END
