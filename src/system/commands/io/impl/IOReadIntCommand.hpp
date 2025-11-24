/**
 * @file IOReadIntCommand.hpp
 * @brief Defines the IOReadIntCommand class for reading integer input.
 * 
 * The IOReadIntCommand class extends IOReadBizCommand to handle reading a 32-bit 
 * integer (`int`) value from standard input. It generates LLVM IR that emits a 
 * `scanf` call with the `%d` format specifier and returns the read integer value.
 * 
 * Example usage:
 * ```cpp
 * int value = system.io.getInt();
 * ```
 * 
 * This command integrates directly into the Lynx I/O system, supporting 
 * LLVM-based input operations for primitive integer types.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_INT_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_INT_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadIntCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                return emitScanfRead(builder, module, "%d", builder.getInt32Ty());
            }
    };
        
}

#endif



