/**
 * @file IOPrintlnCommand.hpp
 * @brief Defines the IOPrintlnCommand class for printing output with newline termination.
 * 
 * The IOPrintlnCommand class extends IOWriteBizCommand to provide formatted output 
 * similar to `println` in high-level languages. It appends a newline automatically 
 * after printing all provided arguments. Each argument’s LLVM type determines its 
 * corresponding format specifier for output generation.
 * 
 * Example usage:
 * ```cpp
 * system.io.println("Result:", value);
 * ```
 * 
 * This command supports multiple argument types and ensures consistent, 
 * human-readable console output while integrating seamlessly with LLVM IR.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_PRINTLN_COMMAND_HPP
#define LYNX_SYSTEM_IO_PRINTLN_COMMAND_HPP

#include "system/commands/io/IOWriteBizCommand.hpp"

namespace LynxSystem {

    class IOPrintlnCommand final : public IOWriteBizCommand {

        public:

            IOPrintlnCommand() noexcept = default;
            ~IOPrintlnCommand() noexcept override = default;

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
    
                // if (calleeArgs.empty()) {
                //     auto* fmtStr = builder.CreateGlobalString("\n", "newline");
                //     return emitPrintfCall(builder, module, { fmtStr });
                // }
    
                auto fmt = buildFormatString(calleeArgs, true);

                auto args = buildPrintfArgs(builder, module, fmt, calleeArgs);

                return emitPrintfCall(builder, module, args);
            }
    };
        
}

#endif
