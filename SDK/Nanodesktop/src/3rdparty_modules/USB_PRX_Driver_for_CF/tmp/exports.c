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

void extern USB_PRX_AsyncRegister;
void extern USB_PRX_AsyncUnregister;
void extern USB_PRX_AsyncRead;
void extern USB_PRX_AsyncWrite;
void extern USB_PRX_AsyncFlush;
void extern USB_PRX_WaitForConnect;
void extern USB_PRX_WriteBulkData;
void extern USB_PRX_ReadData;
void extern USB_PRX_WriteData;
void extern USB_PRX_PassCommAreaAdr;
void extern USB_PRX_CommXChg;
static const unsigned int __ndUSBDriver_exports[22] __attribute__((section(".rodata.sceResident"))) = {
	0x91C14890,
	0x491B8B7C,
	0x24EE9E8C,
	0x1B948AD6,
	0x37286165,
	0x3BA6CFEC,
	0x81871496,
	0x709E6177,
	0x5092EDF0,
	0x949485A5,
	0x10207422,
	(unsigned int) &USB_PRX_AsyncRegister,
	(unsigned int) &USB_PRX_AsyncUnregister,
	(unsigned int) &USB_PRX_AsyncRead,
	(unsigned int) &USB_PRX_AsyncWrite,
	(unsigned int) &USB_PRX_AsyncFlush,
	(unsigned int) &USB_PRX_WaitForConnect,
	(unsigned int) &USB_PRX_WriteBulkData,
	(unsigned int) &USB_PRX_ReadData,
	(unsigned int) &USB_PRX_WriteData,
	(unsigned int) &USB_PRX_PassCommAreaAdr,
	(unsigned int) &USB_PRX_CommXChg,
};

const struct _PspLibraryEntry __library_exports[2] __attribute__((section(".lib.ent"), used)) = {
	{ NULL, 0x0000, 0x8000, 4, 1, 2, &__syslib_exports },
	{ "ndUSBDriver", 0x0000, 0x4001, 4, 0, 11, &__ndUSBDriver_exports },
};
