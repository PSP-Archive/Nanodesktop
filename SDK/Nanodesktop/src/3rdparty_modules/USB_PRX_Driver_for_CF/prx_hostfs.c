/* ------------------------------------------------------------------------
   USB Driver PRX for Nanodesktop PSP - Version for CF
   Version 1.0
   ------------------------------------------------------------------------
   This is the driver for USB support in Nanodesktop PSP (ndUsbDriver.PRX).
   
   This driver is partially based on USB HostFS Driver of PSPLINK. 
   
   PSPLink is a component under BSD license:         
   Copyright (c) 2006 James Forshaw <tyranid@gmail.com> 
   file:///home/svn/psp/trunk/psplink/usbhostfs/prx_hostfs.c

   Filippo Battaglia has introduced many modifications to the Forshaw original 
   code, obtaining this Nanodesktop USB Driver. He recognizes to James Forshaw,
   all rights about the original code. 
   
   This Nanodesktop USB driver is released under BSD license. 
   ----------------------------------------------------------------------------
*/
   
#include <pspkernel.h>
#include <pspdebug.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <nanodesktop.h>



static int USB_PRX_hfsIoInit(PspIoDrvArg *arg)
{
        /* Nothing to do */
        return 0;
}

static int USB_PRX_hfsIoExit(PspIoDrvArg *arg)
{
        /* Nothing to do */
        return 0;
}

static int USB_PRX_hfsOpen(PspIoDrvFileArg *arg, char *file, int mode, SceMode mask)
{
        int ret = -1;
        struct HostFsOpenCmd cmd;
        struct HostFsOpenResp resp;

        if(file == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file name (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_OPEN;
        cmd.cmd.extralen = strlen(file)+1;
        cmd.mode = mode;
        cmd.mask = mask;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), file, strlen(file)+1, NULL, 0, 0))
                {
                        /* Set the resultant fid into the arg structure */
                        if(resp.res >= 0)
                        {
                                arg->arg = (void *) (resp.res);
                                ret = 0;
                        }
                        else
                        {
                                ret = resp.res;
                        }

                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned fid %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending open command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsClose(PspIoDrvFileArg *arg)
{
        int ret = -1;
        struct HostFsCloseCmd cmd;
        struct HostFsCloseResp resp;

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_CLOSE;
        cmd.cmd.extralen = 0;
        cmd.fid = (int) (arg->arg);

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, NULL, 0, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending close command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

int USB_PRX_hfsReadData(int fd, void *data, int len)
{
        struct HostFsReadCmd cmd;
        struct HostFsReadResp resp;
        int blocks;
        int residual;
        int ret = 0;

        if(len < 0)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid read length (%d)\n", len);
                return -1;
        }

        if(len == 0)
        {
                return 0;
        }

        if(data == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"NULL data pointer\n");
                return -1;
        }

        blocks = len / HOSTFS_MAX_BLOCK;
        residual = len % HOSTFS_MAX_BLOCK;

        while(blocks > 0)
        {
                xmemset(&cmd, 0, sizeof(cmd));
                xmemset(&resp, 0, sizeof(resp));
                cmd.cmd.magic = HOSTFS_MAGIC;
                cmd.cmd.command = HOSTFS_CMD_READ;
                cmd.cmd.extralen = 0;
                cmd.len = HOSTFS_MAX_BLOCK;
                cmd.fid = fd;

                if(USB_PRX_IsUsbConnected())
                {
                        if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, data, HOSTFS_MAX_BLOCK, 0))
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Read: Returned result %d\n", resp.res);
                                if(resp.res > 0)
                                {
                                        data += resp.res;
                                        ret += resp.res;
                                }
                                else
                                {
                                        /* If we had an error straight away */
                                        if((resp.res < 0) && (ret == 0))
                                        {
                                                ret = resp.res;
                                        }

                                        /* Otherwise just return how much we managed to read */
                                        break;
                                }
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending read command\n");
                                ret = -1;
                                break;
                        }
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
                        ret = -1;
                        break;
                }

                blocks--;
        }

        do
        {
                if((blocks == 0) && (residual > 0))
                {
                        xmemset(&cmd, 0, sizeof(cmd));
                        xmemset(&resp, 0, sizeof(resp));
                        cmd.cmd.magic = HOSTFS_MAGIC;
                        cmd.cmd.command = HOSTFS_CMD_READ;
                        cmd.cmd.extralen = 0;
                        cmd.len = residual;
                        cmd.fid = fd;

                        if(USB_PRX_IsUsbConnected())
                        {
                                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, data, residual, 0))
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Read: Returned result %d\n", resp.res);
                                        if(resp.res > 0)
                                        {
                                                data += resp.res;
                                                ret += resp.res;
                                        }
                                        else
                                        {
                                                /* If we had an error straight away */
                                                if((resp.res < 0) && (ret == 0))
                                                {
                                                        ret = resp.res;
                                                }

                                                /* Otherwise just return how much we managed to read */
                                                break;
                                        }
                                }
                                else
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending read command\n");
                                        ret = -1;
                                        break;
                                }
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
                                ret = -1;
                                break;
                        }
                }
        }
        while(0);

        return ret;
}

static int USB_PRX_hfsRead(PspIoDrvFileArg *arg, char *data, int len)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"read: arg %p, data %p, len %d\n", arg, data, len);

        return USB_PRX_hfsReadData((int) arg->arg, data, len);
}

int USB_PRX_hfsWriteData(int fd, const void *data, int len)
{
        struct HostFsWriteCmd cmd;
        struct HostFsWriteResp resp;
        int blocks;
        int residual;
        int ret = 0;

        if(len < 0)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid write length (%d)\n", len);
                return -1;
        }

        if(len == 0)
        {
                return 0;
        }

        if(data == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"NULL data pointer\n");
                return -1;
        }


        blocks = len / HOSTFS_MAX_BLOCK;
        residual = len % HOSTFS_MAX_BLOCK;

        while(blocks > 0)
        {
                xmemset(&cmd, 0, sizeof(cmd));
                xmemset(&resp, 0, sizeof(resp));
                cmd.cmd.magic = HOSTFS_MAGIC;
                cmd.cmd.command = HOSTFS_CMD_WRITE;
                cmd.cmd.extralen = HOSTFS_MAX_BLOCK;
                cmd.fid = fd;

                if(USB_PRX_IsUsbConnected())
                {
                        if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), data, HOSTFS_MAX_BLOCK, NULL, 0, 0))
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Write: Returned result %d\n", resp.res);
                                if(resp.res > 0)
                                {
                                        data += resp.res;
                                        ret += resp.res;
                                }
                                else
                                {
                                        /* If we had an error straight away */
                                        if((resp.res < 0) && (ret == 0))
                                        {
                                                ret = resp.res;
                                        }

                                        /* Otherwise just return how much we managed to write */

                                        break;
                                }
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending write command\n");
                                ret = -1;
                                break;
                        }
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
                        ret = -1;
                        break;
                }

                blocks--;
        }

        do
        {
                if((blocks == 0) && (residual > 0))
                {
                        xmemset(&cmd, 0, sizeof(cmd));
                        xmemset(&resp, 0, sizeof(resp));
                        cmd.cmd.magic = HOSTFS_MAGIC;
                        cmd.cmd.command = HOSTFS_CMD_WRITE;
                        cmd.cmd.extralen = residual;
                        cmd.fid = fd;

                        if(USB_PRX_IsUsbConnected())
                        {
                                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), data, residual, NULL, 0, 0))
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Write: Returned result %d\n", resp.res);
                                        if(resp.res > 0)
                                        {
                                                ret += resp.res;
                                        }
                                        else
                                        {
                                                /* If we had an error straight away */
                                                if((resp.res < 0) && (ret == 0))
                                                {
                                                        ret = resp.res;
                                                }

                                                /* Otherwise just return how much we managed to write */
                                                break;
                                        }
                                }
                                else
                                {
                                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending write command\n");
                                        ret = -1;
                                        break;
                                }
                        }
                        else
                        {
                                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
                                ret = -1;
                                break;
                        }
                }
        }
        while(0);

        return ret;
}

static int USB_PRX_hfsWrite(PspIoDrvFileArg *arg, const char *data, int len)
{
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"write: arg %p, data %p, len %d\n", arg, data, len);

        return USB_PRX_hfsWriteData((int) arg->arg, data, len);
}

static SceOff USB_PRX_hfsLSeek(PspIoDrvFileArg *arg, SceOff ofs, int whence)
{
        SceOff ret = -1;
        struct HostFsLseekCmd cmd;
        struct HostFsLseekResp resp;

        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"lseek: ofs %d, whence %d\n", (int) ofs, whence);

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_LSEEK;
        cmd.fid = (int) (arg->arg);
        cmd.ofs = ofs;
        cmd.whence = whence;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, NULL, 0, 0))
                {
                        if(resp.res >= 0)
                        {
                                ret = resp.ofs;
                        }

                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Lseek returned res %d\n", ret);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending lseek command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsIOCTL(PspIoDrvFileArg *arg, unsigned int cmdno, void *indata, int inlen, void *outdata, int outlen)
{
        /* Do nothing atm */
        int ret = -1;
        struct HostFsIoctlCmd cmd;
        struct HostFsIoctlResp resp;

        /* Ensure our lengths are zeroed */
        if(indata == NULL)
        {
                inlen = 0;
        }

        if(outdata == NULL)
        {
                outlen = 0;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_IOCTL;
        cmd.cmd.extralen = inlen;
        cmd.cmdno = cmdno;
        cmd.fid = (int) (arg->arg);
        cmd.outlen = outlen;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), indata, inlen, outdata, outlen, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending ioctl command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsRemove(PspIoDrvFileArg *arg, const char *name)
{
        int ret = -1;
        struct HostFsRemoveCmd cmd;
        struct HostFsRemoveResp resp;

        if(name == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file name (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_REMOVE;
        cmd.cmd.extralen = strlen(name)+1;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), name, strlen(name)+1, NULL, 0, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending remove command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsMkDir(PspIoDrvFileArg *arg, const char *name, SceMode mode)
{
        int ret = -1;
        struct HostFsMkdirCmd cmd;
        struct HostFsMkdirResp resp;

        if(name == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file name (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_MKDIR;
        cmd.cmd.extralen = strlen(name)+1;
        cmd.mode = mode;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), name, strlen(name)+1, NULL, 0, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending mkdir command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsRmDir(PspIoDrvFileArg *arg, const char *name)
{
        int ret = -1;
        struct HostFsRmdirCmd cmd;
        struct HostFsRmdirResp resp;

        if(name == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file name (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_RMDIR;
        cmd.cmd.extralen = strlen(name)+1;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), name, strlen(name)+1, NULL, 0, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending rmdir command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsDOpen(PspIoDrvFileArg *arg, const char *dir)
{
        int ret = -1;
        struct HostFsDopenCmd cmd;
        struct HostFsDopenResp resp;

        if(dir == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid dir name (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_DOPEN;
        cmd.cmd.extralen = strlen(dir)+1;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), dir, strlen(dir)+1, NULL, 0, 0))
                {
                        /* Set the resultant did into the arg structure */
                        if(resp.res >= 0)
                        {
                                arg->arg = (void *) (resp.res);
                                ret = 0;
                        }
                        else
                        {
                                ret = resp.res;
                        }

                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned did %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending dopen command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsDClose(PspIoDrvFileArg *arg)
{
        int ret = -1;
        struct HostFsDcloseCmd cmd;
        struct HostFsDcloseResp resp;

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_DCLOSE;
        cmd.cmd.extralen = 0;
        cmd.did = (int) (arg->arg);

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, NULL, 0, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned result %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending dclose command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsDRead(PspIoDrvFileArg *arg, SceIoDirent *dir)
{
        int ret = -1;
        struct HostFsDreadCmd cmd;
        struct HostFsDreadResp resp;

        if(dir == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid dir pointer (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_DREAD;
        cmd.cmd.extralen = 0;
        cmd.did = (int) (arg->arg);

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), NULL, 0, dir, sizeof(SceIoDirent), 0))
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Dread: Returned result %d\n", resp.res);
                        ret = resp.res;
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending read command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsGetStat(PspIoDrvFileArg *arg, const char *file, SceIoStat *stat)
{
        int ret = -1;
        struct HostFsGetstatCmd cmd;
        struct HostFsGetstatResp resp;

        if(file == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file name (NULL)\n");
                return -1;
        }

        if(stat == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid stat pointer\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_GETSTAT;
        cmd.cmd.extralen = strlen(file)+1;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), file, strlen(file)+1, stat, sizeof(SceIoStat), 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending getstat command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsChStat(PspIoDrvFileArg *arg, const char *file, SceIoStat *stat, int bits)
{
        int ret = -1;
        struct HostFsChstatCmd cmd;
        struct HostFsChstatResp resp;

        if(file == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file name (NULL)\n");
                return -1;
        }

        if(stat == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid stat pointer (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_CHSTAT;
        cmd.cmd.extralen = strlen(file)+1;
        cmd.bits = bits;
        cmd.mode = stat->st_mode;
        cmd.size = stat->st_size;
        cmd.atime.year = stat->st_atime.year;
        cmd.atime.month = stat->st_atime.month;
        cmd.atime.day = stat->st_atime.day;
        cmd.atime.hour = stat->st_atime.hour;
        cmd.atime.minute = stat->st_mtime.minute;
        cmd.atime.second = stat->st_mtime.second;
        cmd.mtime.year = stat->st_mtime.year;
        cmd.mtime.month = stat->st_mtime.month;
        cmd.mtime.day = stat->st_mtime.day;
        cmd.mtime.hour = stat->st_mtime.hour;
        cmd.mtime.minute = stat->st_mtime.minute;
        cmd.mtime.second = stat->st_mtime.second;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), file, strlen(file)+1, NULL, 0, 0))
                {
                        /* Set the resultant did into the arg structure */
                        ret = resp.res;

                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending chstat command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsRename(PspIoDrvFileArg *arg, const char *oldname, const char *newname)
{
        char buf[HOSTFS_RENAME_BUFSIZE];
        int ret = -1;
        struct HostFsRenameCmd cmd;
        struct HostFsRenameResp resp;
        int size;

        if((oldname == NULL) || (newname == NULL))
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid file names for rename command\n");
                return -1;
        }

        size = strlen(oldname) + strlen(newname) + 2;
        if(size > sizeof(buf))
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Not enough buffer space for rename command, size %d, bufsize %d\n", size, sizeof(buf));
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_RENAME;
        cmd.cmd.extralen = size;
        cmd.fsnum = arg->fs_num;

        /* Fill buffer */
        strcpy(buf, oldname);
        strcpy(buf+strlen(oldname)+1, newname);

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), buf, size, NULL, 0, 0))
                {
                        /* Set the resultant fid into the arg structure */
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending rename command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsChDir(PspIoDrvFileArg *arg, const char *dir)
{
        int ret = -1;
        struct HostFsChdirCmd cmd;
        struct HostFsChdirResp resp;

        if(dir == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid dir name (NULL)\n");
                return -1;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_CHDIR;
        cmd.cmd.extralen = strlen(dir)+1;
        cmd.fsnum = arg->fs_num;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), dir, strlen(dir)+1, NULL, 0, 0))
                {
                        ret = resp.res;

                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending chdir command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsMount(PspIoDrvFileArg *arg)
{
        /* Do nothing atm */
        return -1;
}

static int USB_PRX_hfsUMount(PspIoDrvFileArg *arg)
{
        /* Do nothing atm */
        return -1;
}

static int USB_PRX_hfsDevCtl(PspIoDrvFileArg *arg, const char *name, unsigned int cmdno, void *indata, int inlen, void *outdata, int outlen)
{
        int ret = -1;
        struct HostFsDevctlCmd cmd;
        struct HostFsDevctlResp resp;

        if(name == NULL)
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Invalid name (NULL)\n");
                return -1;
        }

        /* Handle the get info devctl */
        if(cmdno == DEVCTL_GET_INFO)
        {
                void **p = (void **) indata;
                if((p) && (*p))
                {
                        outdata = *p;
                        outlen = sizeof(struct DevctlGetInfo);
                        indata = NULL;
                        inlen = 0;
                }
                else
                {
                        return -1;
                }
        }

        /* Ensure our lengths are zeroed */
        if(indata == NULL)
        {
                inlen = 0;
        }

        if(outdata == NULL)
        {
                outlen = 0;
        }

        xmemset(&cmd, 0, sizeof(cmd));
        xmemset(&resp, 0, sizeof(resp));
        cmd.cmd.magic = HOSTFS_MAGIC;
        cmd.cmd.command = HOSTFS_CMD_DEVCTL;
        cmd.cmd.extralen = inlen;
        cmd.cmdno = cmdno;
        cmd.fsnum = arg->fs_num;
        cmd.outlen = outlen;

        if(USB_PRX_IsUsbConnected())
        {
                if(USB_PRX_CommXChg(&cmd, sizeof(cmd), &resp, sizeof(resp), indata, inlen, outdata, outlen, 0))
                {
                        ret = resp.res;
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_DEBUG,"Returned res %d\n", resp.res);
                }
                else
                {
                        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Error in sending devctl command\n");
                }
        }
        else
        {
                USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"%s: Error PC side not connected\n", __FUNCTION__);
        }

        return ret;
}

static int USB_PRX_hfsUnknown(PspIoDrvFileArg *arg)
{
        /* Do nothing atm */
        USB_PRX_INTERNAL_SendMessageToNd (USBMSG_SERVICE,"Unknown command called\n");
        return -1;
}

static PspIoDrvFuncs host_funcs = 
{
        USB_PRX_hfsIoInit,
        USB_PRX_hfsIoExit,
        USB_PRX_hfsOpen,
        USB_PRX_hfsClose,
        USB_PRX_hfsRead,
        USB_PRX_hfsWrite,
        USB_PRX_hfsLSeek,
        USB_PRX_hfsIOCTL,
        USB_PRX_hfsRemove,
        USB_PRX_hfsMkDir,
        USB_PRX_hfsRmDir,
        USB_PRX_hfsDOpen,
        USB_PRX_hfsDClose,
        USB_PRX_hfsDRead,
        USB_PRX_hfsGetStat,
        USB_PRX_hfsChStat,
        USB_PRX_hfsRename,
        USB_PRX_hfsChDir,
        USB_PRX_hfsMount,
        USB_PRX_hfsUMount,
        USB_PRX_hfsDevCtl,
        USB_PRX_hfsUnknown,
};

static PspIoDrv host_driver = 
{
        "host", 0x10, 0x800, "HOST", &host_funcs
};





int USB_PRX_hfsInit(void)
{
        int ret;

        (void) sceIoDelDrv("host"); /* Ignore error */
        ret = sceIoAddDrv(&host_driver);
        if(ret < 0)
        {
                return ret;
        }

        return 0;
}

void USB_PRX_hfsTerminate(void)
{
        (void) sceIoDelDrv("host");
}

 



