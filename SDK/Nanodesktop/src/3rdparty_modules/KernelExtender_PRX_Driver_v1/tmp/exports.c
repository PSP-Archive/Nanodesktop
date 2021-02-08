#include <pspmoduleexport.h>
#define NULL ((void *) 0)

void extern module_start;
void extern module_stop;
void extern module_info;
static const unsigned int __syslib_exports[6] __attribute__((section(".rodata.sceResident"))) = {
	0xD632ACDB,
	0xCEE8593C,
	0xF01D73A7,
	(unsigned int) &module_start,
	(unsigned int) &module_stop,
	(unsigned int) &module_info,
};

void extern KRNEXT_PRX_pspAudioInit;
void extern KRNEXT_PRX_pspAudioEnd;
void extern KRNEXT_PRX_sceAudioSetFrequency;
void extern KRNEXT_PRX_sceCtrlReadBufferPositiveDetectHome;
static const unsigned int __ndKrnExtender_exports[8] __attribute__((section(".rodata.sceResident"))) = {
	0x82BA7282,
	0xC6992FE0,
	0xEF6EBEA7,
	0x5DF4339A,
	(unsigned int) &KRNEXT_PRX_pspAudioInit,
	(unsigned int) &KRNEXT_PRX_pspAudioEnd,
	(unsigned int) &KRNEXT_PRX_sceAudioSetFrequency,
	(unsigned int) &KRNEXT_PRX_sceCtrlReadBufferPositiveDetectHome,
};

const struct _PspLibraryEntry __library_exports[2] __attribute__((section(".lib.ent"), used)) = {
	{ NULL, 0x0000, 0x8000, 4, 1, 2, &__syslib_exports },
	{ "ndKrnExtender", 0x0000, 0x4001, 4, 0, 4, &__ndKrnExtender_exports },
};
