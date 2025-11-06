/**
 * @file IOReadByteCommand.hpp
 * @brief Defines the IOReadByteCommand class for reading byte-sized integer input.
 * 
 * The IOReadByteCommand class extends IOReadBizCommand to handle reading an 
 * 8-bit signed integer (`byte`) from standard input. It generates LLVM IR that 
 * invokes `scanf` with the `"%hhd"` format specifier, ensuring correct byte-size 
 * conversion and memory handling.
 * 
 * Example usage:
 * ```cpp
 * byte b = system.io.getByte();
 * ```
 * 
 * This command enables precise byte-level input operations in the Lynx I/O 
 * subsystem, facilitating low-level data reading through LLVM IR integration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_BYTE_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_BYTE_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadByteCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                
                auto* tmp = builder.CreateAlloca(builder.getInt8Ty(), nullptr, "byte_tmp");
                auto* readValue = emitScanfRead(builder, module, "%hhd", builder.getInt8Ty());
                builder.CreateStore(readValue, tmp);
                
                return builder.CreateLoad(builder.getInt8Ty(), tmp, "read_byte");     
            }

    };
        
}

#endif



