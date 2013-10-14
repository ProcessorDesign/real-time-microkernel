/*
 * neutrino.c
 *
 *  Created on: 11 paź 2013
 *      Author: zabkiewi
 */

#include "neutrino.h"

int ChannelCreate(int flags) {

	//create shared memory segment

	return 0;
}

int ConnectAttach(int pid, int chid, int flags) {

	//zaznacz ze dany proces chce otrzymywac info z kanalu

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

