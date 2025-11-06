/**
 * @file IOReadLineCommand.hpp
 * @brief Defines the IOReadLineCommand class for reading an entire line of input.
 * 
 * The IOReadLineCommand class extends IOReadBizCommand to provide functionality 
 * for reading a full line of text input (up to 1023 characters) from standard input. 
 * It allocates a character buffer and generates LLVM IR to call `scanf` with the 
 * format specifier `%1023[^\n]`, effectively reading until a newline character.
 * 
 * Example usage:
 * ```cpp
 * string line = system.io.in();
 * ```
 * 
 * This command is typically used for reading user-entered text or strings and 
 * integrates directly with the Lynx I/O module's LLVM IR code generation process.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_LINE_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_LINE_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadLineCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {                
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
    
                // Allocate 1KB buffer for reading a line
                auto* bufferType = builder.getInt8Ty();
                auto* bufferSize = builder.getInt32(1024);
                auto* inputBuffer = builder.CreateAlloca(bufferType, bufferSize, "input_buffer");
    
                // Prepare format string for reading until newline
                auto* fmtStr = builder.CreateGlobalStringPtr("%1023[^\n]", "read_line_fmt");
                auto* scanfFunc = getOrCreateScanf(builder.getContext(), module);
    
                // Emit scanf call
                builder.CreateCall(scanfFunc, { fmtStr, inputBuffer }, "scanf_read_line");
    
                // Return pointer to buffer
                return inputBuffer;    
            }
    };
        
}

#endif
