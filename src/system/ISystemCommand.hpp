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
