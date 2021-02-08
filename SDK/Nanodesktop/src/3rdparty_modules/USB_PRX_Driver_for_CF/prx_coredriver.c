/* ------------------------------------------------------------------------
   USB Driver PRX for Nanodesktop PSP - Version for CF
   Version 1.0
   ------------------------------------------------------------------------
   This is the driver for USB support in Nanodesktop PSP (ndUsbDriver.PRX).
   
   This driver is partially based on USB HostFS Driver of PSPLINK. 
   
   PSPLink is a component under BSD license:         
   Copyright (c) 2006 James Forshaw <tyranid@gmail.com> 
   file:///home/svn/psp/trunk/psplink/usbhostfs/

   Filippo Battaglia has introduced many modifications to the Forshaw original 
   code, obtaining this Nanodesktop USB Driver. He recognizes to James Forshaw,
   all rights about the original code. 
   
   This Nanodesktop USB driver is released under BSD license. 
   ----------------------------------------------------------------------------
*/


#include <nanodesktop.h>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspkdebug.h>
#include <pspsdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspusb.h>
#include <pspusbbus.h>




PSP_MODULE_INFO(ND001_MODULE_NAME, PSP_MODULE_KERNEL, 1, 0);



/* Read/Write buffer */
static volatile unsigned char tx_buf [16*1024] __attribute__((aligned(64)));



/* Main USB event flags */
enum UsbEvents
{
        USB_EVENT_ATTACH = 1,
        USB_EVENT_DETACH = 2,
        USB_EVENT_ASYNC  = 4,
        USB_EVENT_CONNECT = 8,
        USB_EVENT_ALL = 0xFFFFFFFF
};

/* USB transfer event flags */
enum UsbTransEvents
{
        USB_TRANSEVENT_BULKOUT_DONE = 1,
        USB_TRANSEVENT_BULKIN_DONE = 2,
};

/* Main USB thread id */
static SceUID g_thid = -1;
/* Main USB event flag */
static SceUID g_mainevent = -1;
/* Main USB transfer event flag */
static SceUID g_transevent = -1;
/* Asynchronous input event flag */
static SceUID g_asyncevent = -1;
/* Main USB semaphore */
static SceUID g_mainsema   = -1;
/* Static bulkin request structure */
static struct UsbdDeviceReq g_bulkin_req;
/* Static bulkout request structure */
static struct UsbdDeviceReq g_bulkout_req;
/* Async request */
static struct UsbdDeviceReq g_async_req;
/* Indicates we have a connection to the PC */
static int UsbConnectedFlag = 0;
/* Buffers for async data */
static struct AsyncEndpoint *g_async_chan[MAX_ASYNC_CHANNELS];
/* Da impostare a uno per uscire dal thread USB */
static char ExitUsbThread = 0;



/* HI-Speed device descriptor */
struct DeviceDescriptor devdesc_hi = 
{
        .bLength = 18, 
        .bDescriptorType = 0x01, 
        .bcdUSB = 0x200, 
        .bDeviceClass = 0, 
        .bDeviceSubClass = 0, 
        .bDeviceProtocol = 0, 
        .bMaxPacketSize = 64, 
        .idVendor = 0, 
        .idProduct = 0, 
        .bcdDevice = 0x100, 
        .iManufacturer = 0, 
        .iProduct = 0, 
        .iSerialNumber = 0,
        .bNumConfigurations = 1
};

/* Hi-Speed configuration descriptor */
struct ConfigDescriptor confdesc_hi = 
{
        .bLength = 9, 
        .bDescriptorType = 2, 
        .wTotalLength = (9+9+(3*7)), 
        .bNumInterfaces = 1, 
        .bConfigurationValue = 1, 
        .iConfiguration = 0, 
        .bmAttributes = 0xC0, 
        .bMaxPower = 0
};

/* Hi-Speed interface descriptor */
struct InterfaceDescriptor interdesc_hi = 
{
        .bLength = 9, 
        .bDescriptorType = 4, 
        .bInterfaceNumber = 0, 
        .bAlternateSetting = 0, 
        .bNumEndpoints = 3, 
        .bInterfaceClass = 0xFF, 
        .bInterfaceSubClass = 0x1, 
        .bInterfaceProtocol = 0xFF, 
        .iInterface = 1
};

/* Hi-Speed endpoint descriptors */
struct EndpointDescriptor endpdesc_hi[3] = 
{
        {
                .bLength = 7, 
                .bDescriptorType = 5, 
                .bEndpointAddress = 0x81, 
                .bmAttributes = 2, 
                .wMaxPacketSize = 512, 
                .bInterval = 0 
        },
        {
                .bLength = 7, 
                .bDescriptorType = 5, 
                .bEndpointAddress = 2, 
                .bmAttributes = 2, 
                .wMaxPacketSize = 512, 
                .bInterval = 0 
        },
        {
                .bLength = 7, 
                .bDescriptorType = 5, 
                .bEndpointAddress = 3, 
                .bmAttributes = 2, 
                .wMaxPacketSize = 512, 
                .bInterval = 0 
        },
};

/* Full-Speed device descriptor */
struct DeviceDescriptor devdesc_full = 
{
        .bLength = 18, 
        .bDescriptorType = 0x01, 
        .bcdUSB = 0x200, 
        .bDeviceClass = 0, 
        .bDeviceSubClass = 0, 
        .bDeviceProtocol = 0, 
        .bMaxPacketSize = 64, 
        .idVendor = 0, 
        .idProduct = 0, 
        .bcdDevice = 0x100, 
        .iManufacturer = 0, 
        .iProduct = 0, 
        .iSerialNumber = 0,
        .bNumConfigurations = 1
};

/* Full-Speed configuration descriptor */
struct ConfigDescriptor confdesc_full = 
{
        .bLength = 9, 
        .bDescriptorType = 2, 
        .wTotalLength = (9+9+(3*7)), 
        .bNumInterfaces = 1, 
        .bConfigurationValue = 1, 
        .iConfiguration = 0, 
        .bmAttributes = 0xC0, 
        .bMaxPower = 0
};

/* Full-Speed interface descriptor */
struct InterfaceDescriptor interdesc_full = 
{
        .bLength = 9, 
        .bDescriptorType = 4, 
        .bInterfaceNumber = 0, 
        .bAlternateSetting = 0, 
        .bNumEndpoints = 3, 
        .bInterfaceClass = 0xFF, 
        .bInterfaceSubClass = 0x1, 
        .bInterfaceProtocol = 0xFF, 
        .iInterface = 1
};

/* Full-Speed endpoint descriptors */
struct EndpointDescriptor endpdesc_full[3] = 
{
        {
                .bLength = 7, 
                .bDescriptorType = 5, 
                .bEndpointAddress = 0x81, 
                .bmAttributes = 2, 
                .wMaxPacketSize = 64, 
                .bInterval = 0 
        },
        {
                .bLength = 7, 
                .bDescriptorType = 5, 
                .bEndpointAddress = 2, 
                .bmAttributes = 2, 
                .wMaxPacketSize = 64, 
                .bInterval = 0 
        },
        {
                .bLength = 7, 
                .bDescriptorType = 5, 
                .bEndpointAddress = 3, 
                .bmAttributes = 2, 
                .wMaxPacketSize = 64, 
                .bInterval = 0 
        },
};

/* String descriptor */
unsigned char strp[] = 
{
        0x8, 0x3, '<', 0, '>', 0, 0, 0
};

/* Endpoint blocks */
struct UsbEndpoint endp[4] = {
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 0, 0 },
        { 3, 0, 0 },
};

/* Intefaces */
struct UsbInterface intp = {
        0xFFFFFFFF, 0, 1,
};

/* Device request */
int USB_PRX_cbRequest(int arg1, int arg2, struct DeviceRequest *req)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "func24 a1 %08X, a2 %08X, a3 %p", arg1, arg2, req);
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "ReqType %d, Request %d, Value %x, Index %x, Length %x", 
                        req->bmRequestType, req->bRequest, req->wValue, req->wIndex,
                        req->wLength);
        return 0;
}

/* Unknown callback */
int USB_PRX_cbFunc28(int arg1, int arg2, int arg3)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_cbFunc28 a1 %08X, a2 %08X, a3 %08X", arg1, arg2, arg3);
        return 0;
}

/* Attach callback, speed 1=full, 2=hi  */
int USB_PRX_cbUsbAttach(int speed, void *arg2, void *arg3)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_cbUsbAttach: speed %d, a2 %p, a3 %p", speed, arg2, arg3);
        sceKernelSetEventFlag(g_mainevent, USB_EVENT_ATTACH);

        return 0;
}

/* Detach callback */
int USB_PRX_cbUsbDetach(int arg1, int arg2, int arg3)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_cbUsbDetach: a1 %08X, a2 %08X, a3 %08X", arg1, arg2, arg3);
        sceKernelSetEventFlag(g_mainevent, USB_EVENT_DETACH);

        return 0;
}

/* Forward define the driver structure */
extern struct UsbDriver g_driver;

/* USB data structures for hi and full speed endpoints */
struct UsbData usbdata[2];

/* Callback for when a bulkin request is done */
int USB_PRX_cbBulkInReqDone(struct UsbdDeviceReq *req, int arg2, int arg3)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_cbBulkInReqDone:\n");
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "size %08X, unkc %08X, recvsize %08X", req->size, req->unkc, req->recvsize);
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "retcode %08X, unk1c %08X, arg %p", req->retcode, req->unk1c, req->arg);
        sceKernelSetEventFlag(g_transevent, USB_TRANSEVENT_BULKIN_DONE);
        return 0;
}

/* Callback for when a bulkout request is done */
int USB_PRX_cbBulkOutReqDone(struct UsbdDeviceReq *req, int arg2, int arg3)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_cbBulkOutReqDone:");
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "size %08X, unkc %08X, recvsize %08X", req->size, req->unkc, req->recvsize);
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "retcode %08X, unk1c %08X, arg %p", req->retcode, req->unk1c, req->arg);
        sceKernelSetEventFlag(g_transevent, USB_TRANSEVENT_BULKOUT_DONE);
        return 0;
}

/* Callback for when a bulkout request is done */
int USB_PRX_cbAsyncReqDone(struct UsbdDeviceReq *req, int arg2, int arg3)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_cbAsyncReqDone:");
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "size %08X, unkc %08X, recvsize %08X", req->size, req->unkc, req->recvsize);
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "retcode %08X, unk1c %08X, arg %p", req->retcode, req->unk1c, req->arg);
        sceKernelSetEventFlag(g_mainevent, USB_EVENT_ASYNC);
        return 0;
}

/* Setup a bulkin request */
int USB_PRX_SetupBulkInReq(void *data, int size)
{
        sceKernelDcacheWritebackRange(data, size);
        xmemset(&g_bulkin_req, 0, sizeof(g_bulkin_req));
        g_bulkin_req.endp = &endp[1];
        g_bulkin_req.data = data;
        g_bulkin_req.size = size;
        g_bulkin_req.func = USB_PRX_cbBulkInReqDone;
        sceKernelClearEventFlag(g_transevent, ~USB_TRANSEVENT_BULKIN_DONE);
        return sceUsbbdReqSend(&g_bulkin_req);
}

/* Setup a bulkout request */
int USB_PRX_SetupBulkOutReq(void *data, int size)
{
        u32 addr;
        u32 blockaddr;
        u32 topaddr;

        /* Ensure address is uncached */
        addr = (u32) data;
        blockaddr = (addr & ~63);
        topaddr = (addr + size + 63) & ~63;

        if(blockaddr != addr)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error read data not cache aligned\n");
                return -1;
        }

        /* Invalidate range */
        sceKernelDcacheInvalidateRange((void*) blockaddr, topaddr - blockaddr);
        xmemset(&g_bulkout_req, 0, sizeof(g_bulkout_req));
        g_bulkout_req.endp = &endp[2];
        g_bulkout_req.data = (void *) addr;
        g_bulkout_req.size = size;
        g_bulkout_req.func = USB_PRX_cbBulkOutReqDone;
        sceKernelClearEventFlag(g_transevent, ~USB_TRANSEVENT_BULKOUT_DONE);
        return sceUsbbdReqRecv(&g_bulkout_req);
}


/* Read a block of data from the USB bus */
int USB_PRX_ReadData(void *data, int size, int TimeOut)
{
    static int RegTimeOut;
    RegTimeOut = TimeOut;
        
    if (UsbConnectedFlag)
    {    
        int nextsize = 0;
        int readlen = 0;
        int ret;
        u32 result;

        while(readlen < size)
        {
                nextsize = (size - readlen) > sizeof(tx_buf) ? sizeof(tx_buf) : (size - readlen);
                if(USB_PRX_SetupBulkOutReq(tx_buf, nextsize) < 0)
                {
                        return -1;
                }

                ret = sceKernelWaitEventFlag(g_transevent, USB_TRANSEVENT_BULKOUT_DONE, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &result, &RegTimeOut);
                if(ret == 0)
                {
                        if((g_bulkout_req.retcode == 0) && (g_bulkout_req.recvsize > 0))
                        {
                                readlen += g_bulkout_req.recvsize;
                                xmemcpy(data, tx_buf, g_bulkout_req.recvsize);
                                data += g_bulkout_req.recvsize;
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Error in BULKOUT request %d, %d", g_bulkout_req.retcode, g_bulkout_req.recvsize);
                                return -1;
                        }
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error waiting for BULKOUT %08X\n", ret);
                        return -1;
                }
        }

        return readlen;
    }
    else
      return -1;
}

int USB_PRX_WriteData(const void *data, int size, int TimeOut)
{
    static int RegTimeOut;
    RegTimeOut = TimeOut;
    
    if (UsbConnectedFlag)
    {    
        int nextsize = 0;
        int writelen = 0;
        int ret;
        u32 result;

        if((u32) data & 63)
        {
                while(writelen < size)
                {
                        nextsize = (size - writelen) > sizeof(tx_buf) ? sizeof(tx_buf) : (size - writelen);
                        xmemcpy(tx_buf, data, nextsize);
                        USB_PRX_SetupBulkInReq(tx_buf, nextsize);
                        /* TODO: Add a timeout to the event flag wait */
                        ret = sceKernelWaitEventFlag(g_transevent, USB_TRANSEVENT_BULKIN_DONE, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &result, &RegTimeOut);
                        if(ret == 0)
                        {
                                if((g_bulkin_req.retcode == 0) && (g_bulkin_req.recvsize > 0))
                                {
                                        writelen += g_bulkin_req.recvsize;
                                        data += g_bulkin_req.recvsize;
                                }
                                else
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error in BULKIN request %d", g_bulkin_req.retcode);
                                        return -1;
                                }
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error waiting for BULKIN %08X", ret);
                                return -1;
                        }
                }
        }
        else
        {
                while(writelen < size)
                {
                        nextsize = (size - writelen) > sizeof(tx_buf) ? sizeof(tx_buf) : (size - writelen);
                        USB_PRX_SetupBulkInReq((char *) data, nextsize);
                        /* TODO: Add a timeout to the event flag wait */
                        ret = sceKernelWaitEventFlag(g_transevent, USB_TRANSEVENT_BULKIN_DONE, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &result, &RegTimeOut);
                        if(ret == 0)
                        {
                                if((g_bulkin_req.retcode == 0) && (g_bulkin_req.recvsize > 0))
                                {
                                        writelen += g_bulkin_req.recvsize;
                                        data += g_bulkin_req.recvsize;
                                }
                                else
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error in BULKIN request %d\n", g_bulkin_req.retcode);
                                        return -1;
                                }
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error waiting for BULKIN %08X\n", ret);
                                return -1;
                        }
                }
        }

        return writelen;
    }
    else
      return -1;
}

/* Exchange a HOSTFS command with the PC host */
int USB_PRX_CommXChg(void *outcmd, int outcmdlen, void *incmd, int incmdlen, const void *outdata, 
                int outlen, void *indata, int inlen, int TimeOut)
{
   static int RegTimeOut;
                
   struct HostFsCmd *cmd;
   struct HostFsCmd *resp;
   int ret = 0;
   int err = 0;
        
   if (UsbConnectedFlag)
   {    
        /* TODO: Set timeout on semaphore */
   
        RegTimeOut = TimeOut;
        err = sceKernelWaitSema(g_mainsema, 1, &(RegTimeOut) );
        
        if(err < 0)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error waiting on xchg semaphore %08X\n", err);
                return 0;
        }

        do
        {
                cmd = (struct HostFsCmd *) outcmd;
                resp = (struct HostFsCmd *) incmd;

                if(outcmdlen > 0)
                {
                        err = USB_PRX_WriteData(outcmd, outcmdlen, TimeOut);
                        if(err != outcmdlen)
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error writing command %08X %d\n", cmd->command, err);
                                break;
                        }
                }

                if(outlen > 0)
                {
                        err = USB_PRX_WriteData(outdata, outlen, TimeOut);
                        if(err != outlen)
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error writing command data %08X, %d\n", cmd->command, err);
                                break;
                        }
                }

                if(incmdlen > 0)
                {
                        err = USB_PRX_ReadData(incmd, incmdlen, TimeOut);
                        if(err != incmdlen)
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error reading response for %08X %d\n", cmd->command, err);
                                break;
                        }

                        if((resp->magic != HOSTFS_MAGIC) && (resp->command != cmd->command))
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Invalid response packet magic: %08X, command: %08X\n", resp->magic, resp->command);
                                break;
                        }

                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "resp->magic %08X, resp->command %08X, resp->extralen %d", 
                                        resp->magic, resp->command, resp->extralen);

                        /* TODO: Should add checks for inlen being less that extra len */
                        if((resp->extralen > 0) && (inlen > 0))
                        {
                                err = USB_PRX_ReadData(indata, resp->extralen, TimeOut);
                                if(err != resp->extralen)
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error reading input data %08X, %d\n", cmd->command, err);
                                        break;
                                }
                        }
                }

                ret = 1;
        }
        while(0);

        (void) sceKernelSignalSema(g_mainsema, 1);
        return ret;
        
    }
    else
      return 0;
}

/* Send an async write */
int USB_PRX_SendAsyncWrite(void *data, int len)
{
   if (UsbConnectedFlag)
   {    
        int ret = 0;
        int err = 0;

        /* TODO: Set timeout on semaphore */
        err = sceKernelWaitSema(g_mainsema, 1, NULL);
        if(err < 0)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error waiting on xchg semaphore %08X\n", err);
                return 0;
        }

        do
        {
                if((data) && (len > 0))
                {
                        err = USB_PRX_WriteData(data, len, 0);
                        if(err != len)
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error writing async command %d\n", err);
                                break;
                        }
                }

                ret = 1;
        }
        while(0);

        (void) sceKernelSignalSema(g_mainsema, 1);

        return ret;
    }
    else
      return 0;
}

/* Send the hello command, indicates we are here */
int USB_PRX_SendHelloCmdToHfsServer(void)
{
        struct HostFsHelloCmd cmd;
        struct HostFsHelloResp resp;

        xmemset(&cmd, 0, sizeof(cmd));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_HELLO;

        return USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, NULL, 0, 1000000);
}

/* Setup a async request */
int USB_PRX_SetupAsyncReq(void *data, int size)
{
        u32 addr;
        u32 blockaddr;
        u32 topaddr;

        /* Ensure address is uncached */
        addr = (u32) data;
        blockaddr = (addr & ~63);
        topaddr = (addr + size + 63) & ~63;

        if(blockaddr != addr)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error read data not cache aligned\n");
                return -1;
        }

        /* Invalidate range */
        sceKernelDcacheInvalidateRange((void*) blockaddr, topaddr - blockaddr);
        xmemset(&g_async_req, 0, sizeof(g_async_req));
        g_async_req.endp = &endp[3];
        g_async_req.data = (void *) addr;
        g_async_req.size = size;
        g_async_req.func = USB_PRX_cbAsyncReqDone;
        sceKernelClearEventFlag(g_mainevent, ~USB_EVENT_ASYNC);
        return sceUsbbdReqRecv(&g_async_req);
   
}

/* Call to ensure we are connected to the USB host */
int USB_PRX_IsUsbConnected(void)
{	
        return UsbConnectedFlag;
}

char async_data[512] __attribute__((aligned(64)));

void USB_PRX_FillAsyncBuffer(void *async_data, int len)
{
        struct AsyncCommand *cmd;
        unsigned char *data;
        int sizeleft;
        int intc;

        if(len > sizeof(struct AsyncCommand))
        {
                len -= sizeof(struct AsyncCommand);
                data = async_data + sizeof(struct AsyncCommand);
                cmd = (struct AsyncCommand *) async_data;

                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "magic %08X, channel %d", cmd->magic, cmd->channel);
                intc = pspSdkDisableInterrupts();
                if((cmd->magic == ASYNC_MAGIC) && (cmd->channel >= 0) && (cmd->channel < MAX_ASYNC_CHANNELS) && (g_async_chan[cmd->channel]))
                {
                        struct AsyncEndpoint *pEndp = g_async_chan[cmd->channel];
                        sizeleft = len < (MAX_ASYNC_BUFFER - pEndp->size) ? len 
                                                : (MAX_ASYNC_BUFFER - pEndp->size);
                        while(sizeleft > 0)
                        {
                                pEndp->buffer[pEndp->write_pos++] = *data++;
                                pEndp->write_pos %= MAX_ASYNC_BUFFER;
                                pEndp->size++;
                                sizeleft--;
                        }
                        sceKernelSetEventFlag(g_asyncevent, (1 << cmd->channel));
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Async chan %d - read_pos %d - write_pos %d - size %d", cmd->channel, 
                                        pEndp->read_pos, pEndp->write_pos,
                                        pEndp->size);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error in command header\n");
                }
                pspSdkEnableInterrupts(intc);
        }
}

int USB_PRX_AsyncRegister(unsigned int chan, struct AsyncEndpoint *endp)
{
        int intc;
        int ret = -1;

        intc = pspSdkDisableInterrupts();
        do
        {
                if(endp == NULL)
                {
                        break;
                }

                if(chan == ASYNC_ALLOC_CHAN)
                {
                        int i;

                        for(i = ASYNC_USER; i < MAX_ASYNC_CHANNELS; i++)
                        {
                                if(g_async_chan[i] == NULL)
                                {
                                        chan = i;
                                        break;
                                }
                        }

                        if(i == MAX_ASYNC_CHANNELS)
                        {
                                break;
                        }
                }
                else
                {
                        if((chan >= MAX_ASYNC_CHANNELS) || (g_async_chan[chan] != NULL))
                        {
                                break;
                        }
                }

                g_async_chan[chan] = endp;
                USB_PRX_AsyncFlush(chan);

                ret = chan;
        }
        while(0);
        pspSdkEnableInterrupts(intc);

        return ret;
}

int USB_PRX_AsyncUnregister(unsigned int chan)
{
        int intc;
        int ret = -1;

        intc = pspSdkDisableInterrupts();
        do
        {
                if((chan >= MAX_ASYNC_CHANNELS) || (g_async_chan[chan] == NULL))
                {
                        break;
                }

                g_async_chan[chan] = NULL;

                ret = 0;
        }
        while(0);
        pspSdkEnableInterrupts(intc);

        return ret;
}

int USB_PRX_AsyncRead(unsigned int chan, unsigned char *data, int len)
{
    if (UsbConnectedFlag)
    {    
        int ret;
        int intc;
        int i;
        int k1;

        k1 = psplinkSetK1(0);

        if((chan >= MAX_ASYNC_CHANNELS) || (g_async_chan[chan] == NULL))
        {
                return -1;
        }

        ret = sceKernelWaitEventFlag(g_asyncevent, 1 << chan, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, NULL, NULL);
        if(ret < 0)
        {
                return -1;
        }

        intc = pspSdkDisableInterrupts();
        len = len < g_async_chan[chan]->size ? len : g_async_chan[chan]->size;
        for(i = 0; i < len; i++)
        {
                data[i] = g_async_chan[chan]->buffer[g_async_chan[chan]->read_pos++];
                g_async_chan[chan]->read_pos %= MAX_ASYNC_BUFFER;
                g_async_chan[chan]->size--;
        }

        if(g_async_chan[chan]->size != 0)
        {
                sceKernelSetEventFlag(g_asyncevent, 1 << chan);
        }
        pspSdkEnableInterrupts(intc);

        psplinkSetK1(k1);

        return len;
    }
    else
      return -1;
}

void USB_PRX_AsyncFlush(unsigned int chan)
{
        int intc;

        if((chan >= MAX_ASYNC_CHANNELS) || (g_async_chan[chan] == NULL))
        {
                return;
        }

        intc = pspSdkDisableInterrupts();
        g_async_chan[chan]->size = 0;
        g_async_chan[chan]->read_pos = 0;
        g_async_chan[chan]->write_pos = 0;
        sceKernelClearEventFlag(g_asyncevent, ~(1 << chan));
        pspSdkEnableInterrupts(intc);
}

int USB_PRX_AsyncWrite(unsigned int chan, const void *data, int len)
{
    if (UsbConnectedFlag)
    {    
        int ret = -1;
        char buffer[512];
        struct AsyncCommand *cmd;
        int written = 0;
        int k1;

        k1 = psplinkSetK1(0);

        do
        {
                if(!USB_PRX_IsUsbConnected())
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Error PC side not connected\n");
                        break;
                }

                cmd = (struct AsyncCommand *) buffer;
                cmd->magic = ASYNC_MAGIC;
                cmd->channel = chan;

                while(written < len)
                {
                        int size;

                        size = (len-written) > (sizeof(buffer)-sizeof(struct AsyncCommand)) ? (sizeof(buffer)-sizeof(struct AsyncCommand)) : (len-written);
                        xmemcpy(&buffer[sizeof(struct AsyncCommand)], data+written, size);
                        if(USB_PRX_SendAsyncWrite(buffer, size + sizeof(struct AsyncCommand)))
                        {
                                written += size;
                        }
                        else
                        {
                                break;
                        }
                }

                ret = written;
        }
        while(0);

        psplinkSetK1(k1);

        return ret;
    }
    else
      return -1;
}

int USB_PRX_WriteBulkData(const void *data, int len)
{
    if (UsbConnectedFlag)
    {        
        int ret = -1;
        int err;
        struct BulkCommand cmd;
        int k1;

        k1 = psplinkSetK1(0);

        do
        {
                if(!USB_PRX_IsUsbConnected())
                {
                        USB_PRX_INTERNAL_SendMessageToNd (0, "Error PC side not connected");
                        break;
                }

                if((len <= 0) || (len > HOSTFS_BULK_MAXWRITE))
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Invalid length %d\n", len);
                        break;
                }

                cmd.magic = BULK_MAGIC;
                cmd.size = len;

                /* TODO: Set timeout on semaphore */
                err = sceKernelWaitSema(g_mainsema, 1, NULL);
                if(err < 0)
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error waiting on xchg semaphore %08X\n", err);
                        break;
                }

                do
                {
                        err = USB_PRX_WriteData (&cmd, sizeof(cmd), 0);
                        if(err != sizeof(cmd))
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error writing bulk header %d\n", err);
                                err = -1;
                                break;
                        }

                        err = USB_PRX_WriteData (data, len, 0);
                        if(err != len)
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Error writing bulk data %d\n", err);
                                err = -1;
                                break;
                        }
                }
                while(0);

                (void) sceKernelSignalSema(g_mainsema, 1);
                if(err >= 0)
                {
                        ret = len;
                }
        }
        while(0);


        psplinkSetK1(k1);
        return ret;
    }
    else
      return -1; 
}

int USB_PRX_WaitForConnect (int TimeOut)
{
        int RegTimeOut;
        int ret;

        while(g_mainevent < 0)
        {
                sceKernelDelayThread(100000);
        }

        RegTimeOut = TimeOut;  
        ret = sceKernelWaitEventFlag(g_mainevent, USB_EVENT_CONNECT, PSP_EVENT_WAITOR, NULL, &RegTimeOut);
        
        if(ret == 0)
        {
                return 1;
        }

        return 0;
}






/* USB thread to handle attach/detach */
int usb_thread(SceSize size, void *argp)
{
        int ret;
        u32 result;
        
        while(!ExitUsbThread)
        {
                ret = sceKernelWaitEventFlag(g_mainevent, USB_EVENT_ATTACH | USB_EVENT_DETACH | USB_EVENT_ASYNC
                                , PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &result, NULL);
                if(ret < 0)
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Error waiting on event flag %08X", ret);
                        sceKernelExitDeleteThread(0);
                }

                if(result & USB_EVENT_ASYNC)
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Async Request Done %d %d", g_async_req.retcode, g_async_req.recvsize);
                        if((g_async_req.retcode == 0) && (g_async_req.recvsize > 0))
                        {
                                USB_PRX_FillAsyncBuffer(async_data, g_async_req.recvsize);
                                USB_PRX_SetupAsyncReq(async_data, sizeof(async_data));
                        }
                }

                if(result & USB_EVENT_DETACH)
                {
                        UsbConnectedFlag = 0;
                        sceKernelClearEventFlag(g_mainevent, ~USB_EVENT_CONNECT);
                        
                        USB_PRX_INTERNAL_SignalUsbDetach ();
                }

                if(result & USB_EVENT_ATTACH)
                {
                        uint32_t magic;
                        UsbConnectedFlag = 1;
                        sceKernelClearEventFlag(g_mainevent, ~USB_EVENT_CONNECT);
                        
                        USB_PRX_INTERNAL_SignalUsbAttach ();
                        
                        if(USB_PRX_ReadData(&magic, sizeof(magic), 1000000) == sizeof(magic))
                        {
                                if(magic == HOSTFS_MAGIC)
                                {
                                        if(USB_PRX_SendHelloCmdToHfsServer())
                                        {
                                                USB_PRX_SetupAsyncReq(async_data, sizeof(async_data));
                                                sceKernelSetEventFlag(g_mainevent, USB_EVENT_CONNECT);
                                        }
                                }
                        }
                        
                        
                }
        
                sceDisplayWaitVblankStart ();
                // Necessario per impedire il blocco dello scheduler
        
        }

        return 0;
}

/* USB start function */
int start_func(int size, void *p)
{
        int ret;

        // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Start Function %p", p);

        /* Fill in the descriptor tables */
        xmemset(usbdata, 0, sizeof(usbdata));

        xmemcpy (usbdata[0].devdesc, &devdesc_hi, sizeof(devdesc_hi));
        
        usbdata[0].config.pconfdesc = &usbdata[0].confdesc;
        usbdata[0].config.pinterfaces = &usbdata[0].interfaces;
        usbdata[0].config.pinterdesc = &usbdata[0].interdesc;
        usbdata[0].config.pendp = usbdata[0].endp;
        xmemcpy(usbdata[0].confdesc.desc, &confdesc_hi,  sizeof(confdesc_hi));
        usbdata[0].confdesc.pinterfaces = &usbdata[0].interfaces;
        usbdata[0].interfaces.pinterdesc[0] = &usbdata[0].interdesc;
        usbdata[0].interfaces.intcount = 1;
        xmemcpy(usbdata[0].interdesc.desc, &interdesc_hi, sizeof(interdesc_hi));
        usbdata[0].interdesc.pendp = usbdata[0].endp;
        xmemcpy(usbdata[0].endp[0].desc, &endpdesc_hi[0], sizeof(endpdesc_hi[0]));
        xmemcpy(usbdata[0].endp[1].desc, &endpdesc_hi[1], sizeof(endpdesc_hi[1]));
        xmemcpy(usbdata[0].endp[2].desc, &endpdesc_hi[2], sizeof(endpdesc_hi[2]));

        xmemcpy(usbdata[1].devdesc, &devdesc_full, sizeof(devdesc_full));
        usbdata[1].config.pconfdesc = &usbdata[1].confdesc;
        usbdata[1].config.pinterfaces = &usbdata[1].interfaces;
        usbdata[1].config.pinterdesc = &usbdata[1].interdesc;
        usbdata[1].config.pendp = usbdata[1].endp;
        xmemcpy(usbdata[1].confdesc.desc, &confdesc_full,  sizeof(confdesc_full));
        usbdata[1].confdesc.pinterfaces = &usbdata[1].interfaces;
        usbdata[1].interfaces.pinterdesc[0] = &usbdata[1].interdesc;
        usbdata[1].interfaces.intcount = 1;
        xmemcpy(usbdata[1].interdesc.desc, &interdesc_full, sizeof(interdesc_full));
        usbdata[1].interdesc.pendp = usbdata[1].endp;
        xmemcpy(usbdata[1].endp[0].desc, &endpdesc_full[0], sizeof(endpdesc_full[0]));
        xmemcpy(usbdata[1].endp[1].desc, &endpdesc_full[1], sizeof(endpdesc_full[1]));
        xmemcpy(usbdata[1].endp[2].desc, &endpdesc_full[2], sizeof(endpdesc_full[2]));

        g_driver.devp_hi = usbdata[0].devdesc;
        g_driver.confp_hi = &usbdata[0].config;
        g_driver.devp = usbdata[1].devdesc;
        g_driver.confp = &usbdata[1].config;

        // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "pusbdata %p", &usbdata);

        xmemset(g_async_chan, 0, sizeof(g_async_chan));

        g_mainevent = sceKernelCreateEventFlag("USBEvent", 0x200, 0, NULL);
        if(g_mainevent < 0)
        {
               // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Couldn't create event flag %08X\n", g_mainevent);
                return -1;
        }

        g_transevent = sceKernelCreateEventFlag("USBEventTrans", 0x200, 0, NULL);
        if(g_transevent < 0)
        {
               // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Couldn't create trans event flag %08X\n", g_transevent);
                return -2;
        }

        g_asyncevent = sceKernelCreateEventFlag("USBEventAsync", 0x200, 0, NULL);
        if(g_asyncevent < 0)
        {
               // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Couldn't create async event flag %08X\n", g_asyncevent);
                return -3;
        }

        g_mainsema = sceKernelCreateSema("USBSemaphore", 0, 1, 1, NULL);
        if(g_mainsema < 0)
        {
               // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Couldn't create semaphore %08X\n", g_mainsema);
                return -4;
        }

        g_thid = sceKernelCreateThread("USBThread", usb_thread, 2, 0x4000, 0, NULL);
        if(g_thid < 0)
        {
               // USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Couldn't create usb thread %08X\n", g_thid);
                return -5;
        }

        ret = USB_PRX_hfsInit();
        //USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "USB_PRX_hfsInit: %d\n", ret);

        if(sceKernelStartThread(g_thid, 0, NULL))
        {
             //   USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE, "Couldn't start usb thread\n");
                return -6;
        }
        
        PntCommArea->UsbThread = g_thid;
        // Il thread USB viene memorizzato nella CommArea

        return 0;
}

/* USB stop function */
int stop_func(int size, void *p)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "Stop function %p", p);
        
        
        if(g_thid >= 0)
        {
                ExitUsbThread=1;
                sceKernelDelayThread (1000000);  // Attende la terminazione del thread USB
                
                /* Should really just signal for completion */
                sceKernelTerminateDeleteThread(g_thid);
                g_thid = -1;
        }

        if(g_mainevent >= 0)
        {
                sceKernelDeleteEventFlag(g_mainevent);
                g_mainevent = -1;
        }

        if(g_transevent >= 0)
        {
                sceKernelDeleteEventFlag(g_transevent);
                g_mainevent = -1;
        }

        if(g_asyncevent >= 0)
        {
                sceKernelDeleteEventFlag(g_asyncevent);
                g_asyncevent = -1;
        }

        if(g_mainsema >= 0)
        {
                sceKernelDeleteSema(g_mainsema);
                g_mainsema = -1;
        }

        return 0;
}

/* USB host driver */
struct UsbDriver g_driver = 
{
        HOSTFSDRIVER_NAME,
        4,
        endp,
        &intp,
        NULL, NULL, NULL, NULL,
        (struct StringDescriptor *) strp,
        USB_PRX_cbRequest, USB_PRX_cbFunc28, USB_PRX_cbUsbAttach, USB_PRX_cbUsbDetach,
        0, 
        start_func,
        stop_func,
        NULL
};



/* Entry point */
int module_start(SceSize args, void *argp)
{
        int ret;

        ret = sceUsbbdRegister(&g_driver);
        xmemset(g_async_chan, 0, sizeof(g_async_chan));
        
        return 0;
}

/* Module stop entry */
int module_stop(SceSize args, void *argp)
{
        int ret;

        ret = sceUsbbdUnregister(&g_driver);
        //USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG, "sceUsbbdUnregister %08X", ret);
        USB_PRX_hfsTerminate();

        return 0;
}

