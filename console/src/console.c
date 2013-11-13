#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../../kernel/src/neutrino.h"
#include "../../kernel/src/mes.h"
#include "../../kernel/src/config.h"

int coid;

void *ping(void *arg) {
	MESSAGE msg = { MSGPING, 0 }, reply;
	for(;;) {
		if(MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply)) < 0 || reply.type != MSGPONG) {
			printf("kernel not responsible\n");
			exit(1);
		}
		sleep(1);
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("use with channel_id\n");
		return 1;
	}
	int chid = atoi(argv[1]);
	sleep(1);
	coid = ConnectAttach(getppid(), chid, 0);
	if(coid < 0) {
		printf("kernel not responsible\n");
		return 2;
	}

	pthread_t tid;
	pthread_create(&tid, NULL, ping, NULL);

	printf("console\n");
	MESSAGE *pmsg = (MESSAGE *) malloc(MAXMSGSIZE);
	pmsg->type = MSGCMD;
	for(;;) {
		printf("%% "); fflush(stdout);
		if(!fgets(pmsg->body, MAXMSGSIZE - sizeof(MESSAGE) - 1, stdin)) break;
		pmsg->len = strlen(pmsg->body);
		MsgSend(coid, pmsg, sizeof(MESSAGE) + pmsg->len, pmsg, MAXMSGSIZE);
		if(pmsg->len < 1 || (pmsg->len == 1 && pmsg->body[0] == '\n')) continue;
		pmsg->body[pmsg->len] = 0;
		printf("kernel said:\n");
		printf("%s", pmsg->body);
	}
	printf("console closed\n");
	return EXIT_SUCCESS;
}
