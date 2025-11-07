/**
 * @file ISystemCommand.hpp
 * @brief Declares the ISystemCommand interface for executable system commands.
 * 
 * The ISystemCommand interface defines the contract for implementing system-level 
 * commands within the Lynx system. Each command must implement the `execute` method, 
 * which performs the desired operation using the provided AST execution context 
 * and LLVM IR arguments.
 * 
 * This interface provides a uniform way to define and execute commands that can 
 * interact with the compiler’s runtime or internal systems, enabling modular and 
 * extensible command behavior.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ISYSTEM_COMMAND_HPP
#define LYNX_ISYSTEM_COMMAND_HPP


#include <llvm/IR/IRBuilder.h>
#include <context/AstContext.hpp>
#include <ast/ExpressionNode.hpp>

namespace LynxSystem {

    using LynxContext::AstContext;

    class ISystemCommand {

        public:

            virtual ~ISystemCommand() = default;
        
            virtual llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) = 0;
    };    
    
}

#endif
