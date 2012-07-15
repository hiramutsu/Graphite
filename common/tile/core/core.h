#ifndef CORE_H
#define CORE_H

#include <string.h>

// some forward declarations for cross includes
class Tile;
class CoreModel;
class Network;
class MemoryManager;
class SyscallMdl;
class SyncClient;
class ClockSkewMinimizationClient;

// FIXME: Move this out of here eventually
class PinMemoryManager;

#include "mem_component.h"
#include "fixed_types.h"
#include "config.h"
#include "core_model.h"
#include "shmem_perf_model.h"
#include "capi.h"
#include "packet_type.h"


using namespace std;

class Core
{
public:
   enum State
   {
      RUNNING = 0,
      INITIALIZING,
      STALLED,
      SLEEPING,
      WAKING_UP,
      IDLE,
      NUM_STATES
   };

   enum lock_signal_t
   {
      NONE = 0,
      LOCK,
      UNLOCK
   };

   enum mem_op_t
   {
      READ = 0,
      READ_EX,
      WRITE
   };

   Core(Tile *tile);
   virtual ~Core();

   static Core *create(Tile* tile, core_type_t core_type = MAIN_CORE_TYPE);

   int coreSendW(int sender, int receiver, char *buffer, int size, carbon_network_t net_type);
   int coreRecvW(int sender, int receiver, char *buffer, int size, carbon_network_t net_type);
   
   virtual UInt64 readInstructionMemory(IntPtr address, UInt32 instruction_size) = 0;

   virtual pair<UInt32, UInt64> initiateMemoryAccess(MemComponent::Type mem_component,
                                                     lock_signal_t lock_signal,
                                                     mem_op_t mem_op_type,
                                                     IntPtr address,
                                                     Byte* data_buf, UInt32 data_size,
                                                     bool push_info = false,
                                                     UInt64 time = 0) = 0;
   
   virtual pair<UInt32, UInt64> accessMemory(lock_signal_t lock_signal, mem_op_t mem_op_type, IntPtr address,
                                             char* data_buffer, UInt32 data_size, bool push_info = false) = 0;
   pair<UInt32, UInt64> nativeMemOp(lock_signal_t lock_signal, mem_op_t mem_op_type, IntPtr address, char* data_buffer, UInt32 data_size);

   // network accessor since network is private
   int getTileId();
   core_id_t getCoreId() { return m_core_id; }
   Network *getNetwork();
   Tile *getTile() { return m_tile; }
   UInt32 getCoreType() { return m_core_id.core_type; }
   CoreModel *getPerformanceModel() { return m_core_model; }
   MemoryManager *getMemoryManager() { return m_memory_manager; } 
   virtual PinMemoryManager *getPinMemoryManager() = 0;
   virtual SyscallMdl *getSyscallMdl() = 0; 
   SyncClient *getSyncClient() { return m_sync_client; }
   virtual ClockSkewMinimizationClient* getClockSkewMinimizationClient() = 0;
   ShmemPerfModel* getShmemPerfModel() { return m_shmem_perf_model; }

   State getState();
   void setState(State core_state);

protected:
   Tile *m_tile;
   core_id_t m_core_id;
   CoreModel *m_core_model;
   MemoryManager *m_memory_manager;
   ShmemPerfModel* m_shmem_perf_model;
   SyncClient *m_sync_client;

   State m_core_state;
   Lock m_core_state_lock;

   static Lock m_global_core_lock;

   PacketType getPktTypeFromUserNetType(carbon_network_t net_type);
};

#endif
