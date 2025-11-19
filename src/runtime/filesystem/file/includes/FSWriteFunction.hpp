#ifndef LYNX_RUNTIME_FS_WRITE_FUNCTION_HPP
#define LYNX_RUNTIME_FS_WRITE_FUNCTION_HPP

#include "RuntimeFunction.hpp"
#include "filesystem/file/FileFunctionTypes.hpp" 

namespace LynxRuntime {

    class FSWriteFunction : public RuntimeFunction {
        
        public:
        
            llvm::Value* call(
                std::shared_ptr<AstContext> astContext,
                std::vector<std::unique_ptr<ExpressionNode>> args
            ) override {
                LOG_ERROR("Invoked Call");
                return nullptr;
            }

            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args, llvm::Value* file) override {
                LOG_INFO("Invoked Call with optional value");
                if(args.empty() || args.size() < 1 ) {
                    LOG_ERROR("write expects 1 arguments: file and content");
                    return nullptr;
                }

                auto& builder = astContext->getBuilder();
                auto& llvmCtx = astContext->getLLVMContext();
                auto* module = astContext->getModule();

                llvm::Value* contentArg = args.at(0)->generateCode(astContext);
                if (!contentArg) {
                    LOG_ERROR("Failed to generate content argument");
                    return nullptr;
                }

                contentArg = builder.CreateBitCast(contentArg, llvm::PointerType::get(llvmCtx, 0));

                llvm::Function* writeFunc = module->getFunction("file_write");

                if (!writeFunc) {
                    LOG_ERROR("NOT FOUND FUNCTION");
                    llvm::FunctionType* funcType = FileOps::getWriteFuncType(llvmCtx);
                    writeFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "file_write", module);
                }

                // Cast file to void* if needed
                if (file->getType() != llvm::PointerType::get(llvmCtx, 0)) {
                    file = builder.CreateBitCast(file, llvm::PointerType::get(llvmCtx, 0), "file_cast");
                }
            
                // Call file_write(file, content)
                return builder.CreateCall(writeFunc, { file, contentArg });
            }
        };
}

#endif 
