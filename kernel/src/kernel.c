#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <spawn.h>
#include <unistd.h>
#include <pthread.h>
//#include <process.h>

#include "neutrino.h"
#include "config.h"
#include "mes.h"

typedef struct _PROG {
	char *exefile;
	char *args;
	int pid;
	int ttl;
} PROG;

PROG progs[] =
{
		{ "console", "", 0, DEFAULTTTL },
		{ "driver", "", 0, DEFAULTTTL },
		{ "sensor", "", 0, DEFAULTTTL },
		{ "driver_cooler", "", 0, DEFAULTTTL },
		{ NULL, NULL, 0, DEFAULTTTL }
};

void start(int n) {
	progs[n].ttl = DEFAULTTTL;
	progs[n].pid = posix_spawn(0, progs[n].exefile, progs[n].exefile, progs[n].args, NULL);
}

void *decttl(void *arg) {
	for(;;) {
		int n;
		for(n = 0; progs[n].exefile; n++) {
			progs[n].ttl--;
			if(progs[n].ttl == 0) {
				if(progs[n].pid > 0) kill(progs[n].pid, SIGTERM);
				start(n);
			}
		}
		sleep(1);
	}
}

int main(int argc, char *argv[]) {
	printf("kernel start\n");

	int chid = ChannelCreate(0);
	printf("channel %d created\n", chid);
	char chidstr[16];
	sprintf(chidstr, "%d", chid);

	int n;
	for(n = 0; progs[n].exefile; n++) {
		progs[n].args = chidstr;
		printf("subsystem %s: ", progs[n].exefile); fflush(stdout);
		start(n);
		if(progs[n].pid > 0)
			printf("ok\n");
		else
			printf("error\n");
	}

	pthread_t tid;
	pthread_create(&tid, NULL, decttl, NULL);

	MESSAGE *pmsg = (MESSAGE *) malloc(MAXMSGSIZE);
	struct _msg_info info;
	int quit = 0;
	while(!quit) {
		int rcvid = MsgReceive(chid, pmsg, MAXMSGSIZE, &info);
		// if(pmsg->type != MSGPING)
		//	printf("message type %d len %d from %d\n", pmsg->type, pmsg->len, info.pid);
		switch(pmsg->type) {
		case MSGPING:
		{
			MESSAGE pong = { MSGPONG, 0 };
			for(n = 0; progs[n].exefile; n++) {
				if(progs[n].pid == info.pid) {
					progs[n].ttl = DEFAULTTTL;
					break;
				}
			}
			MsgReply(rcvid, 0, &pong, sizeof(pong));
		}
		break;
		case MSGCMD:
		{
			pmsg->body[pmsg->len] = 0;
			char cmd[MAXMSGSIZE];
			int par, n = sscanf(pmsg->body, "%s %d", cmd, &par);
			if(n < 1) cmd[0] = 0;
			if(n < 2) par = 0;
			//printf("cmd [%s] with arg [%d]\n", cmd, par);

			if(!strcmp(cmd, "")) {
				// nop
			}
			else if(!strcmp(cmd, "exit")) {
				// koniec pracy systemu
				quit = 1;
				break;
			}
			else if(!strcmp(cmd, "list")) {
				char line[81];
				pmsg->body[0] = 0;
				for(n = 0; progs[n].exefile; n++) {
					sprintf(line, "%s\t%d\t%d\n", progs[n].exefile, progs[n].pid, progs[n].ttl);
					strcat(pmsg->body, line);
				}
				pmsg->len = strlen(pmsg->body);
				MsgReply(rcvid, 0, pmsg, sizeof(MESSAGE) + pmsg->len);
				break;
			}
			else
			{
				strcpy(pmsg->body, "unrecognized command\n");
				pmsg->len = strlen(pmsg->body);
				MsgReply(rcvid, 0, pmsg, sizeof(MESSAGE) + pmsg->len);
				break;
			}

			MsgReply(rcvid, 0, NULL, 0);
		}
		break;
//		case MSGCOLD:
//			/* printf("Za zimno\n"); */
//			MsgReply(rcvid, 0, NULL, 0);
//			break;
//		case MSGHOT:
//			/* printf("Za cieplo\n"); */
//			MsgReply(rcvid, 0, NULL, 0);
//			break;
		default: MsgReply(rcvid, 0, NULL, 0);
		}
	}
	printf("system down\n");
	for(n = 0; progs[n].exefile; n++) {
		printf("terminate %s\n", progs[n].exefile);
		if(progs[n].pid > 0) kill(progs[n].pid, SIGTERM);
	}
	return EXIT_SUCCESS;
}
