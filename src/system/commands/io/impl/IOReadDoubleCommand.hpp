/**
 * @file IOReadDoubleCommand.hpp
 * @brief Defines the IOReadDoubleCommand class for reading double-precision input.
 * 
 * The IOReadDoubleCommand class extends IOReadBizCommand to handle reading a 
 * double-precision floating-point (`double`) value from standard input. It emits 
 * LLVM IR that invokes `scanf` using the `%lf` format specifier and returns the 
 * read value for further computation or assignment.
 * 
 * Example usage:
 * ```cpp
 * double value = system.io.getDouble();
 * ```
 * 
 * This command provides LLVM-based runtime integration for reading `double` 
 * values in the Lynx I/O subsystem.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_DOUBLE_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_DOUBLE_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadDoubleCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> /*calleeArgs*/) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                return emitScanfRead(builder, module, "%lf", builder.getDoubleTy());    
            }

    };
        
}

#endif



