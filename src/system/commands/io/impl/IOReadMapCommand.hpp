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
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_MAP_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_MAP_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadMapCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
        
                // For simplicity, assume <string, int> map
                llvm::Type* keyType = builder.getInt8PtrTy();
                llvm::Type* valueType = builder.getInt32Ty();
        
                // TODO: integrate with LynxTypes::MapType and allocate memory dynamically
                std::cout << "[IOReadMapCommand] Simulating map input read..." << std::endl;
        
                return nullptr; // Placeholder for LLVM struct/map object
            }
    };
        
}

#endif



