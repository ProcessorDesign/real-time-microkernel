/*
 * neutrino.c
 *
 *  Created on: 11 paź 2013
 *      Author: zabkiewi
 */

#include "neutrino.h"


int ChannelCreate(int flags) {
    int rc, semid, shmid, i;
    key_t semkey, shmkey;

    struct sembuf operations[2];
    struct shmid_ds shmid_struct;
    short  sarray[NUMSEMS];

	//create shared memory segment
    shmid = shmget(shmkey, SIZEOFSHMSEG, 0666 | IPC_CREAT | IPC_EXCL);
    if (shmid == -1)
      {
        printf("main: shmget() failed\n");
        return -1;
      }

    shm_address = shmat(shmid, NULL, 0);
    if ( shm_address==NULL )
      {
        printf("main: shmat() failed\n");
        return -1;
      }


	return 0;
}

int ConnectAttach(int pid, int chid, int flags) {
    int rc, semid, shmid, i;
    key_t semkey, shmkey;

    struct sembuf operations[2];
    struct shmid_ds shmid_struct;
    short  sarray[NUMSEMS];


	//zaznacz ze dany proces chce otrzymywac info z kanalu
    shmid = shmget(shmkey, SIZEOFSHMSEG, 0666);
    if (shmid == -1)
      {
        printf("main: shmget() failed\n");
        return -1;
      }

    /* Attach the shared memory segment to the client process.       */
    shm_address = shmat(shmid, NULL, 0);
    if ( shm_address==NULL )
      {
        printf("main: shmat() failed\n");
        return -1;
      }

	return 0;
}


int MsgSend( int coid,
             const void* smsg,
             int sbytes,
             void* rmsg,
             int rbytes ) {

	//zapis informacji do pamieci wspoldzielonej


	return 0;
}

int MsgReply( int rcvid,
              int status,
              const void* msg,
              int size ) {

	//odpowiedz na wiadomosc w pamieci wspoldzielonej

	return 0;
}

int MsgReceive( int chid,
                void * msg,
                int bytes,
                struct _msg_info * info ) {

	//odczytanie wiadomosci z pamieci wspoldzielonej bez odpowiedzi

	return 0;
}

