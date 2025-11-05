/**
 * @file SystemModuleFacade.hpp
 * @brief Defines the SystemModuleFacade class for unified access to system modules.
 * 
 * The SystemModuleFacade class provides a simplified interface for invoking methods 
 * on registered system modules within the Lynx system. It acts as a bridge between 
 * the abstract syntax tree (AST) execution context and the underlying module 
 * implementations, handling module lookup and method dispatch.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_MODULE_FACADE_HPP
#define LYNX_SYSTEM_MODULE_FACADE_HPP

#include <llvm/IR/IRBuilder.h>
#include <context/AstContext.hpp>
#include "SystemModuleRegistry.hpp"

namespace LynxSystem {

    using LynxContext::AstContext;

    class SystemModuleFacade {

        public:

            llvm::Value* invokeMethod(std::shared_ptr<AstContext> context, const std::string& moduleName, const std::string& methodName, std::vector<llvm::Value*> calleeArgs) {
                auto module = SystemModuleRegistry::getModule(moduleName);
                if (!module) return nullptr;
                return module->invoke(context, methodName, calleeArgs);            
            }
    };
}

#endif
