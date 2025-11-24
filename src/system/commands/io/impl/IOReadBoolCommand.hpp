/**
 * @file IOReadBoolCommand.hpp
 * @brief Defines the IOReadBoolCommand class for reading boolean input values.
 * 
 * The IOReadBoolCommand class extends IOReadBizCommand to handle reading 
 * boolean values from standard input. Internally, it reads an integer using 
 * `scanf("%d")` and converts it to a boolean comparison (`!= 0`) at the LLVM IR level.
 * 
 * Example usage:
 * ```cpp
 * boolean flag = system.io.getBoolean();
 * ```
 * 
 * This command provides a robust bridge between textual input and LLVM’s 
 * boolean representation, enabling logical data handling in the Lynx runtime.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_BOOLEAN_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_BOOLEAN_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadBoolCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();

                auto* tmp = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "bool_tmp");
                auto* intVal = emitScanfRead(builder, module, "%d", builder.getInt32Ty());
                builder.CreateStore(intVal, tmp);
                
                auto* loaded = builder.CreateLoad(builder.getInt32Ty(), tmp, "bool_loaded");
                return builder.CreateICmpNE(loaded, builder.getInt32(0), "bool_val");
            }

    };
        
}

#endif



