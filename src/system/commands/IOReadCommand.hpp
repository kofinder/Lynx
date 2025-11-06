/**
 * @file IOReadCommand.hpp
 * @brief Defines the IOReadCommand class for handling system input operations.
 * 
 * The IOReadCommand class implements the ISystemCommand interface to provide 
 * input functionality within the Lynx system. It facilitates reading data from 
 * standard input or other sources, integrating the operation into the LLVM IR 
 * generation process.
 * 
 * In its current form, the command demonstrates basic LLVM IR construction by 
 * generating a call to a low-level I/O routine using a format string. Future 
 * versions may extend this to support reading various data types or input targets.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_COMMAND_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemModule.hpp"
#include "system/ISystemCommand.hpp"
#include "utils/SystemModuleUtils.hpp"
#include <context/AstContext.hpp>

namespace LynxSystem {

    using namespace LynxSystem::utils;

    class IOReadCommand : public ISystemCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                std::cout << "IP Print command \n";
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                return createReadIntCall(builder, module);            
            }
    };
        
}

#endif
