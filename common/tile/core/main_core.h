#ifndef MAIN_CORE_H
#define MAIN_CORE_H

#include <string.h>

// some forward declarations for cross includes
class CoreModel;

#include "core.h"

using namespace std;

class MainCore : protected Core
{
   public:

      MainCore(Tile* tile);
      ~MainCore();

      virtual UInt64 readInstructionMemory(IntPtr address, UInt32 instruction_size);
      virtual pair<UInt32, UInt64> accessMemory(lock_signal_t lock_signal, mem_op_t mem_op_type, IntPtr d_addr, char* data_buffer, UInt32 data_size, bool modeled = false);

      virtual pair<UInt32, UInt64> initiateMemoryAccess(
            MemComponent::component_t mem_component,
            lock_signal_t lock_signal, 
            mem_op_t mem_op_type, 
            IntPtr address, 
            Byte* data_buf, UInt32 data_size,
            bool modeled = false,
            UInt64 time = 0);
     
      virtual PinMemoryManager *getPinMemoryManager() { return m_pin_memory_manager; }
      
   private:
      PinMemoryManager *m_pin_memory_manager;
};

#endif
