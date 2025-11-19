#ifndef LYNX_RUNTIME_FS_EXISTS_FUNCTION_HPP
#define LYNX_RUNTIME_FS_EXISTS_FUNCTION_HPP

#include "RuntimeFunction.hpp"

namespace LynxRuntime {
    
    class FSExistsFunction : public RuntimeFunction {

        public:

            llvm::Value* call(
                std::shared_ptr<AstContext> astContext,
                std::vector<std::unique_ptr<ExpressionNode>> args
            ) override {
                LOG_ERROR("Invoked Call");
                return nullptr;
            }

            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args, llvm::Value* optionalValue) override {
                LOG_INFO("Invoked Call with optional value");
                return nullptr;
            }

    };
    
}

#endif 
