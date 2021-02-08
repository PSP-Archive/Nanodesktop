  /*
  The oRTP LinPhone RTP library intends to provide basics for a RTP stack.
  Copyright (C) 2001  Simon MORLAT simon.morlat@linphone.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* this program shows how to send streams in paralel using the SessionSet api 
	and two threads only. */

#include <ortp/ortp.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _WIN32 
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <pspkernel.h>
#include <psputility_netmodules.h>
#include <pspctrl.h>
#include <netinet/in.h>

/* Connect to an access point */
int connect_to_apctl(int config)
{
	int err;
	int stateLast = -1;

	/* Connect using the first profile */
	err = sceNetApctlConnect(config);
	if (err != 0)
	{
		printf(": sceNetApctlConnect returns %08X\n", err);
		return 0;
	}

	printf(": Connecting...\n");
	while (1)
	{
		int state;
		err = sceNetApctlGetState(&state);
		if (err != 0)
		{
			printf(": sceNetApctlGetState returns $%x\n", err);
			break;
		}
		if (state > stateLast)
		{
			printf("  connection state %d of 4\n", state);
			stateLast = state;
		}
		if (state == 4)
			break;  // connected with static IP

		// wait a little before polling again
		sceKernelDelayThread(50*1000); // 50ms
	}
	printf(": Connected!\n");

	if(err != 0)
	{
		return 0;
	}

	return 1;
}

PSP_MODULE_INFO("RTP Timer Test", 0, 1, 0);
PSP_HEAP_SIZE_KB(20*1024);


int runcond=1;

void stophandler(int signum)
{
	runcond=0;
}

static char *help="usage: mrtpsend	filename ip port nstreams [--packet-size size] [--ts-inc value]\n";

#define STREAMS_COUNT 1000



int main(int argc, char *argv[])
{
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

	pspSdkInetInit();
	connect_to_apctl(1);

	RtpSession *session[STREAMS_COUNT];
	unsigned char *buffer;
	int packet_size=160;
	int ts_inc=160;
	int i;
	FILE *infile;
	char *ssrc;
	int port;
	uint32_t user_ts=0;
	int channels;
	SessionSet *set;
	
#if 0
	if (argc<5){
		printf(help);
		return -1;
	}
#endif

	channels=2;
	if (channels==0){
		printf(help);
		return -1;
	}
#if 0
	/* look at command line options */
	for (i=5;i<argc;i++)
	{
		if (strcmp(argv[i],"--packet-size")==0)
		{
			if ( i+1 < argc ){
				packet_size=atoi(argv[i+1]);
			}
			else {
				printf(help);
				return -1;
			}
			if (packet_size==0) {
				printf("Packet size can't be %s.\n",argv[i+1]);
				return -1;
			}
			i+=1;
			
		}
		else if (strcmp(argv[i],"--ts-inc")==0)
		{
			if ( i+1 < argc ){
				ts_inc=atoi(argv[i+1]);
			}
			else {
				printf(help);
				return -1;
			}
			if (ts_inc==0) {
				printf("Timestanp increment can't be %s.\n",argv[i+1]);
				return -1;
			}
			i+=1;
			
		}
	}
#endif
	printf("Timestamp increment will be %i\n",ts_inc);
	printf("Packet size will be %i\n",packet_size);
	buffer=ortp_malloc(packet_size);
	ortp_init();
	ortp_scheduler_init();
	printf("scheduler initialized\n");
	ssrc=getenv("SSRC");
	port=6760;
	for (i=0;i<channels;i++){
		printf("channel %d\n", i);
		session[i]=rtp_session_new(RTP_SESSION_SENDONLY);	
		rtp_session_set_scheduling_mode(session[i],1);
		rtp_session_set_blocking_mode(session[i],0);
		rtp_session_set_remote_addr(session[i],"192.168.0.4",port);
		rtp_session_set_payload_type(session[i],0);
		if (ssrc!=NULL) rtp_session_set_ssrc(session[i],atoi(ssrc));
		port+=2;
	}
	
	#ifndef _WIN32
	infile=fopen("host0:/test.wav","r");
	#else
	infile=fopen(argv[1],"rb");
	#endif
	if (infile==NULL) {
		perror("Cannot open file");
		return -1;
	}
	printf("open file\n");
	signal(SIGINT,stophandler);
	/* create a set */
	set=session_set_new();
	while( ((i=fread(buffer,1,packet_size,infile))>0) && (runcond) )
	{
		int k;
		//ortp_message("Sending packet.");
		for (k=0;k<channels;k++){	
			/* add the session to the set */
			session_set_set(set,session[k]);
		}
		/* and then suspend the process by selecting() */
		session_set_select(NULL,set,NULL);
		for (k=0;k<channels;k++){
			/* this is stupid to do this test, because all session work the same way,
			as the same user_ts is used for all sessions, here. */
			if (session_set_is_set(set,session[k])){
				rtp_session_send_with_ts(session[k],buffer,i,user_ts);
				//ortp_message("packet sended !");
			}
		}
		user_ts+=ts_inc;
	}
	fclose(infile);
	printf("close file\n");
	/*sleep a little to wait last packets to be sent */
	#ifndef _WIN32
	sleep(1);
	#else
	Sleep(1);
	#endif
	for (i=0;i<channels;i++)
		rtp_session_destroy(session[i]);
	session_set_destroy(set);
	ortp_free(buffer);
	ortp_exit();
	ortp_global_stats_display();
	return 0;
}
