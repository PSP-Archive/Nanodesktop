/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * pspnet.h - PSP networking libraries.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@0xd6.org>
 *
 * Portions based on PspPet's wifi_03 sample code.
 * 
 * $Id: pspnet.h 1663 2006-01-08 23:35:29Z tyranid $
 */

#ifndef PSPNET_H
#define PSPNET_H

#ifdef __cplusplus
extern "C" {
#endif

int sceNetInit(int unk1, int unk2, int unk3, int unk4, int unk5);
int sceNetTerm(void);

int sceNetEtherNtostr(unsigned char *mac, char *name);

#ifdef __cplusplus
}
#endif

#endif /* PSPNET_H */
