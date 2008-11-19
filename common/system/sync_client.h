#ifndef SYNC_CLIENT_H
#define SYNC_CLIENT_H

#include "sync_api.h"
#include "packetize.h"

class Core;
class Network;

class SyncClient
{
  Core *_core;
  Network *_network;
  UnstructuredBuffer _send_buff, _recv_buff;

 public:

  SyncClient(Core*);
  ~SyncClient();

  void mutexInit(int commid, carbon_mutex_t *mux);
  void mutexLock(int commid, carbon_mutex_t *mux);
  void mutexUnlock(int commid, carbon_mutex_t *mux);

  void condInit(int commid, carbon_cond_t *cond);
  void condWait(int commid, carbon_cond_t *cond, carbon_mutex_t *mux);
  void condSignal(int commid, carbon_cond_t *cond);
  void condBroadcast(int commid, carbon_cond_t *cond);

  void barrierInit(int commid, carbon_barrier_t *barrier, UInt32 count);
  void barrierWait(int commid, carbon_barrier_t *barrier);

  /* Unique return codes for each function call
     - Note: It is NOT a mistake that 
       > COND_WAIT_RESPONSE == MUTEX_LOCK_RESPONSE

       This is necessary because when several threads wait on a
       condition variable and condBroadcast() is called, they will be
       woken by the mutexUnlock() of the thread that holds the lock.

  */
  static const unsigned int MUTEX_LOCK_RESPONSE   = 0xDEADBEEF;
  static const unsigned int MUTEX_UNLOCK_RESPONSE = 0xBABECAFE;
  static const unsigned int COND_WAIT_RESPONSE    = MUTEX_LOCK_RESPONSE;
  static const unsigned int COND_SIGNAL_RESPONSE  = 0xBEEFCAFE;
  static const unsigned int COND_BROADCAST_RESPONSE = 0xDEADCAFE;
  static const unsigned int BARRIER_WAIT_RESPONSE  = 0xCACACAFE;

};

#endif