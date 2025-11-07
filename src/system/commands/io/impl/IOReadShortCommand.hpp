/**
 * @file IOReadShortCommand.hpp
 * @brief Defines the IOReadShortCommand class for reading short integer values from standard input.
 * 
 * The IOReadShortCommand class extends the IOReadBizCommand base to implement input functionality 
 * for 16-bit integer (short) values within the Lynx system. It integrates directly with the LLVM 
 * IR generation process by emitting a `scanf` call configured to read short integers.
 * 
 * This command enables system-level input operations such as:
 * 
 * ```cpp
 * short age = system.io.getShort();
 * ```
 * 
 * The resulting LLVM IR allocates a temporary variable, reads user input from standard input 
 * using the `%hd` format specifier, and returns the loaded short value for further use 
 * in expressions or assignments.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_SYSTEM_IO_READ_SHORT_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_SHORT_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadShortCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                return emitScanfRead(builder, module, "%hd", builder.getInt16Ty());
            }
    };
        
}

#endif



