#ifndef LYNX_LIB_RUNTIME_FS_CLOSE_FUNCTIONS_HPP
#define LYNX_LIB_RUNTIME_FS_CLOSE_FUNCTIONS_HPP

#include "RuntimeFunction.hpp"
#include <llvm/IR/Module.h>
#include "filesystem/file/FileFunctionTypes.hpp" 

namespace LynxLibRuntime {

    class FSCloseFunction : public RuntimeFunction {

        public:
        
            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args) override {
                LOG_ERROR("Invoked Call");
                if (args.empty()) {
                    LOG_ERROR("No file argument provided");
                    return nullptr;
                }
                return nullptr;
            }

            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args, llvm::Value* file) override {
                LOG_INFO("Invoked Call with optional value");
                if(!file) {
                    LOG_ERROR("Expected a File struct instance as value");
                    return nullptr;
                }

                auto& builder = astContext->getBuilder();
                auto* module = astContext->getModule();
                auto& llvmCtx = astContext->getLLVMContext();

                // Validate struct pointer
                if (file->getType() != llvm::PointerType::get(llvmCtx, 0)) {
                    file = builder.CreateBitCast(file, llvm::PointerType::get(llvmCtx, 0), "file_cast");
                }

                llvm::Function* closeFunc = module->getFunction("file_close");
                if (!closeFunc) {
                    LOG_ERROR("NOT FOUND FUNCTION");
                    llvm::FunctionType* funcType = FileOps::getCloseFuncType(llvmCtx);
                    closeFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "file_close", module);
                }

                return builder.CreateCall(closeFunc, { file });
            }
    };  
}

#endif 





