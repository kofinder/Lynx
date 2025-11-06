/**
 * @file IOPrintCommand.hpp
 * @brief Defines the IOPrintCommand class for handling system print operations.
 * 
 * The IOPrintCommand class implements the ISystemCommand interface to provide 
 * print functionality within the Lynx system. This command is responsible for 
 * handling output operations, such as writing evaluated expression results or 
 * formatted text to the console or other output targets.
 * 
 * Currently, the `execute` method serves as a placeholder for future implementation 
 * that will process and emit LLVM IR instructions corresponding to print behavior.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_PRINT_COMMAND_HPP
#define LYNX_SYSTEM_IO_PRINT_COMMAND_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemModule.hpp"
#include "system/ISystemCommand.hpp"
#include "utils/SystemModuleUtils.hpp"

namespace LynxSystem {

    using namespace LynxSystem::utils;

    class IOPrintCommand : public ISystemCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                std::cout << "Execute" << std::endl;

                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                llvm::Value* format = builder.CreateGlobalStringPtr("%d", "scanf_fmt");

                return createPrintCall(builder, module, format);
            }
    };
        
}

#endif
