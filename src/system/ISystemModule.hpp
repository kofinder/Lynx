/**
 * @file ISystemModule.hpp
 * @brief Declares the ISystemModule interface for defining system module behavior.
 * 
 * The ISystemModule interface serves as the base contract for all system modules 
 * within the Lynx system. Each module implementing this interface must provide 
 * an implementation of the `invoke` method, which executes a specific method 
 * identified by name using the provided AST execution context and LLVM IR arguments.
 * 
 * This interface enables polymorphic interaction with various system modules, 
 * allowing dynamic dispatch and modular extensibility within the compiler or runtime.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_ISYSTEM_MODULE_HPP
#define LYNX_ISYSTEM_MODULE_HPP


#include <llvm/IR/IRBuilder.h>
#include <context/AstContext.hpp>

namespace LynxSystem {

    using LynxContext::AstContext;

    class ISystemModule {

        public:
        
            virtual ~ISystemModule() noexcept = default;

            virtual llvm::Value* invokeCommand(std::shared_ptr<AstContext> context, const std::string& methodName, std::vector<llvm::Value*> calleeArgs) = 0;
    };
        
}

#endif
