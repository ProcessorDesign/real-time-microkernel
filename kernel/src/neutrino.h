#ifndef NEUTRINO_H_
#define NEUTRINO_H_

//return channel id
int ChannelCreate(int flags);

//return connection id
int ConnectAttach(int pid, int chid, int flags);


int MsgSend( int coid,
             const void* smsg,
             int sbytes,
             void* rmsg,
             int rbytes );

int MsgReply( int rcvid,
              int status,
              const void* msg,
              int size );

int MsgReceive( int chid,
                void * msg,
                int bytes,
                struct _msg_info * info );


#endif /*NEUTRINO_H_*/
