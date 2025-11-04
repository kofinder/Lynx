#ifndef LYNX_ISYSTEM_MODULE_HPP
#define LYNX_ISYSTEM_MODULE_HPP


#include <llvm/IR/IRBuilder.h>
#include <context/AstContext.hpp>

namespace LynxSystem {

    using LynxContext::AstContext;

    class ISystemModule {

        public:
        
            virtual ~ISystemModule() = default;

            virtual llvm::Value* invoke(std::shared_ptr<AstContext> context, const std::string& methodName, std::vector<llvm::Value*> calleeArgs) = 0;
    };
        
}

#endif
