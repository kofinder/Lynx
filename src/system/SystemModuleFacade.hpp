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
