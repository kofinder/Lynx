/**
 * @file IOReadCharCommand.hpp
 * @brief Defines the IOReadCharCommand class for reading single character input.
 * 
 * The IOReadCharCommand class extends IOReadBizCommand to handle reading a single 
 * character (`char`) from standard input. It generates LLVM IR that invokes 
 * `scanf` with the `" %c"` format specifier to capture one character, ignoring 
 * any leading whitespace.
 * 
 * Example usage:
 * ```cpp
 * char ch = system.io.getChar();
 * ```
 * 
 * This command integrates character input functionality into the Lynx I/O 
 * system, supporting LLVM-based runtime input handling.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_CHAR_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_CHAR_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadCharCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                
                auto* tmp = builder.CreateAlloca(builder.getInt8Ty(), nullptr, "char_tmp");
                auto* readValue = emitScanfRead(builder, module, " %c", builder.getInt8Ty());
                builder.CreateStore(readValue, tmp);

                return builder.CreateLoad(builder.getInt8Ty(), tmp, "read_char");     
            }

    };
        
}

#endif



