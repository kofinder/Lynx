/**
 * @file IOReadLongCommand.hpp
 * @brief Defines the IOReadLongCommand class for reading long integer values from input.
 * 
 * The IOReadLongCommand class extends IOReadBizCommand to provide input functionality 
 * for reading 64-bit signed integer (`long`) values. It generates LLVM IR code that 
 * performs a formatted input read using the `%ld` specifier, ensuring correct type 
 * handling at the IR level.
 * 
 * Example usage:
 * ```cpp
 * long value = system.io.getLong();
 * ```
 * 
 * This command integrates seamlessly with the Lynx I/O system, supporting both 
 * interactive console input and compiled input handling through LLVM IR.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_LONG_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_LONG_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadLongCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                return emitScanfRead(builder, module, "%ld", builder.getInt64Ty());    
            }

    };
        
}

#endif



