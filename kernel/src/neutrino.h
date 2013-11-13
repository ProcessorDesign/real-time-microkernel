#ifndef NEUTRINO_H_
#define NEUTRINO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */
#define SEMKEYID 1              /* Id used on ftok for semget key    */
#define SHMKEYPATH "/dev/null"  /* Path used on ftok for shmget key  */
#define SHMKEYID 1              /* Id used on ftok for shmget key    */

#define NUMSEMS 2               /* Num of sems in created sem set    */
#define SIZEOFSHMSEG 50         /* Size of the shared mem segment    */

#define NUMMSG 2                /* Server only doing two "receives"
                                   on shm segment                    */

    void *shm_address;

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
