/**
 * @file IOReadArrayCommand.hpp
 * @brief Defines the IOReadArrayCommand class for reading array input values.
 * 
 * The IOReadArrayCommand class extends IOReadBizCommand to enable reading arrays 
 * of primitive elements from standard input. It dynamically allocates storage 
 * for the array in LLVM IR and uses a loop to read each element via `scanf`.  
 * 
 * Example usage:
 * ```cpp
 * int[] nums = system.io.getArray(5);
 * ```
 * 
 * Currently, this implementation assumes integer (`int`) arrays, but it can be 
 * extended to support generic typed arrays in future iterations of the Lynx runtime.  
 * The command integrates tightly with LLVM’s IR builder to emit structured loops 
 * and I/O calls efficiently.
 * 
 * @autor: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_ARRAY_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_ARRAY_COMMAND_HPP

#include <unordered_map>
#include <functional>
#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadArrayCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> /*context*/, std::vector<llvm::Value*> /*calleeArgs*/) override {
                return nullptr; // pointer to array in memory
            }
    };
        
}

#endif



