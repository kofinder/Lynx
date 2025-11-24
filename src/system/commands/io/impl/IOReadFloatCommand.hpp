/**
 * @file IOReadFloatCommand.hpp
 * @brief Defines the IOReadFloatCommand class for reading floating-point input.
 * 
 * The IOReadFloatCommand class extends IOReadBizCommand to handle reading a 
 * single-precision floating-point (`float`) value from standard input. It emits 
 * LLVM IR for a `scanf` call using the `%f` format specifier and returns the 
 * resulting value.
 * 
 * Example usage:
 * ```cpp
 * float value = system.io.getFloat();
 * ```
 * 
 * This command provides seamless integration with the Lynx I/O system for 
 * reading `float` values at runtime via LLVM IR generation.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_FLOAT_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_FLOAT_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadFloatCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                return emitScanfRead(builder, module, "%f", builder.getFloatTy());     
            }

    };
        
}

#endif



