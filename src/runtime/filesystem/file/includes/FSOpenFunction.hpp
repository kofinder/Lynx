#ifndef LYNX_RUNTIME_FS_OPEN_FUNCTION_HPP
#define LYNX_RUNTIME_FS_OPEN_FUNCTION_HPP

#include "RuntimeFunction.hpp"
#include <llvm/IR/Module.h>
#include "filesystem/file/FileFunctionTypes.hpp" 

namespace LynxRuntime {

    class FSOpenFunction : public RuntimeFunction {
       
        public:

            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args) override {
                LOG_ERROR("Invoked Call");
                if (args.empty()) {
                    LOG_ERROR("No file argument provided");
                    return nullptr;
                }

                return nullptr;
            }

            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args, llvm::Value* optionalValue) override {
                LOG_INFO("Invoked FSOpenFunction::call with file struct");
               
                if(!optionalValue) {
                    LOG_ERROR("Expected a File struct instance as optionalValue");
                    return nullptr;
                }

                auto& builder = astContext->getBuilder();
                auto* module = astContext->getModule();
                auto& llvmCtx = astContext->getLLVMContext();
    

                auto* i8PtrTy = llvm::PointerType::get(llvmCtx, 0);

                // Load the file path: struct field 0
                llvm::Value* filePathPtr = builder.CreateStructGEP(i8PtrTy, optionalValue, 0, "filePathPtr");
                llvm::Value* filePath = builder.CreateLoad(i8PtrTy, filePathPtr, "filePath");

                llvm::FunctionType* funcType = FileOps::getOpenFuncType(llvmCtx);
                llvm::FunctionCallee callee = module->getOrInsertFunction("file_open_default", funcType);

                // Call file_open_default(path)
                llvm::Value* fileHandle = builder.CreateCall(callee, { filePath }, "fileHandle");

                // Store the result in field 1 (handle)
                llvm::Value* fileHandlePtr = builder.CreateStructGEP(i8PtrTy, optionalValue, 1, "fileHandlePtr");
                builder.CreateStore(fileHandle, fileHandlePtr);

                return optionalValue; // Return updated struct
            }
    };
    
}

#endif 


