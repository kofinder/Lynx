/**
 * @file IOReadMapCommand.hpp
 * @brief Defines the IOReadMapCommand class for reading map-like key-value data structures.
 * 
 * The IOReadMapCommand class extends IOReadBizCommand to support deserialization of 
 * associative containers (e.g., `Map<Key, Value>`) from standard input or other input sources. 
 * Each map entry can be dynamically read and constructed using type-aware input functions 
 * such as `system.io.getString()` for keys and `system.io.getInt()` for values.
 * 
 * Example usage:
 * ```cpp
 * Map<String, Int> ages = system.io.getMap();
 * ```
 * 
 * Future implementations will integrate with `LynxTypes::MapType`, enabling 
 * dynamic allocation, iteration, and field population through LLVM IR code generation.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_MAP_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_MAP_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadMapCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> /*context*/, std::vector<llvm::Value*> /*calleeArgs*/) override {
                return nullptr;
            }
    };
        
}

#endif



