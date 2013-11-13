
/*
//1.odbierac polecenia z sieci
//2.ustawiac stany wyjsc
//3.wysylac stany wyjsc
//4.przetwarzac przez logike
//5.logike zaczytywac z configa
//6.pozwolic na rekonfiguracje przez siec
//7.przetwarzac logike na poziom wyżej i wysylac stany urzadzen
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define IL_CMD 255
#define BUFFERSIZE 255
#define ILFUNC 8
#define log() if(debug==1)printf


static int debug=0;


//NOT
bool notg(bool s) {
    return !s;
}

//AND
bool andg(bool s,bool c) {
    return (s && c);
}

//NAND
bool nandg(bool s, bool c) {
    return !(s && c);
}

//OR
bool org(bool s, bool c) {
    return (s || c);
}

//NOR
bool norg(bool s, bool c) {
    return !(s || c);
}

//XOR
bool xorg(bool s, bool c) {
    return (s ^ c);
}
//XNOR
bool nxorg(bool s, bool c) {
    return !(s ^ c);
}

//TIMER
bool timer(bool s, unsigned char sec) {
    sleep(sec);
    return s;
}

//NO
bool nog(bool s, bool c) {
    if(c) return !s; else return s;
}
//NC
bool ncg(bool s, bool c) {
    if(c) return s; else return !s;
}



typedef struct _command {
    char in1[3];
    char in2[3];
    char gate[5];
    char out[3];
} CMD;

typedef struct _ramka {
    int id;
    int type;
    char msg[1];
} RAMKA;

typedef struct _buffer {
    int readPtr;
    int writePtr;
    RAMKA buf[BUFFERSIZE];
    void (*writeBuf)(RAMKA *rmk, struct _buffer *buf);
    void (*readBuf)(RAMKA *rmk, struct _buffer *buf);
} BUFFER;

void writeBuf(RAMKA *rmk, struct _buffer *buf) {
    buf->buf[buf->writePtr]=*rmk;
    buf->writePtr++;
    if(buf->writePtr == BUFFERSIZE) buf->writePtr=0;
}
void readBuf(RAMKA *rmk, struct _buffer *buf) {
    *rmk=buf->buf[buf->readPtr];
    if(buf->readPtr < buf->writePtr) buf->readPtr++;
    if(buf->readPtr == BUFFERSIZE) buf->readPtr=0;
}

BUFFER *initBuffer() {

    BUFFER *buf = (BUFFER*)malloc(sizeof(BUFFER));
    buf->readBuf = readBuf;
    buf->writeBuf = writeBuf;
    buf->readPtr=0;
    buf->writePtr=0;

    return buf;
}


CMD cmdtab[IL_CMD];
BUFFER *sendbuf;
BUFFER *recvbuf;

bool inputs[10];
bool outputs[10];
bool temps[10];


typedef struct _pfunc {
    bool (*pfunc)(bool s,bool c);
    char name[6];
} PFUNC;


PFUNC pFuncTab[ILFUNC];



void init() {

    int i=0;
    char line[45];

    FILE *f = fopen("config.cfg","r");
    if(f==NULL) {
        printf("fail: open config file\n");
        exit(1);
    }

    while(*fgets(line,45,f)!=(char)EOF) {
        //linia po linii
        CMD cmd;
        scanf(line,"(%s %s %s) -> (%s)",cmd.in1,cmd.gate,cmd.in2,cmd.out);
        cmdtab[i]=cmd;
        i++;
    }

    fclose(f);
}


//tcp
void send() {
    for(;;) {
        //wyslij ramke z bufora
        sleep(1);
    }
}
void recv() {
    for(;;) {
        //odbierz ramke i wpisz do bufora
        sleep(1);
    }
}


bool getVar(char *ch) {
    int subscript = atoi((const char*)ch[1]);
    switch(ch[0]) {
        case 'i':
        {
        return inputs[subscript];
        }
        break;
        case 't':
        {
        return temps[subscript];
        }
        break;
        case 'o':
        {
            return outputs[subscript];
        }
            break;
        }
    return false;
}


void setVar(char *ch,bool b) {
    int subscript=atoi(ch[1]);
    switch(ch[0]) {
    case 't':
    {
        temps[subscript]=b;
    }
        break;
    case 'o':
    {
        outputs[subscript]=b;
    }
        break;
    }
}


bool runfunc(char *func, bool s,bool c ) {
    int i;
    for(i=0; i<ILFUNC; i++) {
        if(!strcmp(pFuncTab[i].name,func)) {
            return pFuncTab[i].pfunc(s,c);
        }
    }
    return false;
}


int main(int argc, char *argv[])
{
    int i;

    if(argc < 1) {
        printf("usage: %s -d\n",argv[0]);
    }
    if(argc > 0) {
        debug=1;
    }

    printf("starting....\n");

    sendbuf = initBuffer();
    recvbuf = initBuffer();


    pthread_t threadSend;
    pthread_t threadRecv;

    pthread_create(&threadSend,NULL,send,NULL);
    pthread_create(&threadRecv,NULL,recv,NULL);

    pFuncTab[0].pfunc = andg;
    strcpy(pFuncTab[0].name,"AND");



    for(;;) {

        for(i=0; i<IL_CMD; i++) {

            if(!strcmp(cmdtab[i].gate,"NOT")) {
                setVar(cmdtab[i].out,notg(getVar(cmdtab[i].in1)));
            } else if(!strcmp(cmdtab[i].gate,"TIMER")) {
                //df
            } else {
                setVar(cmdtab[i].out,runfunc(cmdtab[i].gate,getVar(cmdtab[i].in1),getVar(cmdtab[i].in2)));
            }
        }

        sleep(1);
    }


    return 0;
}

