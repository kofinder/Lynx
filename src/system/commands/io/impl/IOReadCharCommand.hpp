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

                // something need to refactor this
                return emitScanfRead(builder, module, "%c", builder.getInt8Ty(), true);
            }
    };
        
}

#endif
    


// auto& builder = context->getBuilder();
// auto* module = context->getModule();

// // Allocate char variable
// auto* tempChar = builder.CreateAlloca(builder.getInt8Ty(), nullptr, "char_tmp");

// // Attach metadata
// if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(tempChar)) {
//     auto* md = llvm::MDNode::get(builder.getContext(),
//                                  llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureCharType));
//     allocaInst->setMetadata(MetadataTypeConstants::lynxDataType, md);
// }

// // Call scanf
// auto* scanfFunc = getOrCreateScanf(builder.getContext(), module);
// auto* fmt = builder.CreateGlobalStringPtr("%c", "scanf_fmt");
// builder.CreateCall(scanfFunc, {fmt, tempChar}, "scanf_call");

// // Return the pointer (AllocaInst), metadata attached here
// return tempChar;
