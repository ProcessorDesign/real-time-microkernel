#ifndef MESSAGE_H_
#define MESSAGE_H_

typedef struct _MSG {
	int type;
	int len;
	char body[0];
} MESSAGE;

struct _msg_info {
	int pid;
};

#define MSGPING 0
#define MSGPONG 1
#define MSGCMD 2

#define MSG_IAMALIVE 1
#define MSG_VALUECHANGED 2
#define MSG_REQUESTEDVALUE 3
#define MSG_IAMREADY 4

#endif /*MESSAGE_H_*/
