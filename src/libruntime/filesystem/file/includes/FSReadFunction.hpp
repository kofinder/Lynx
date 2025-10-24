#ifndef LYNX_LIB_RUNTIME_FS_READ_FUNCTION_HPP
#define LYNX_LIB_RUNTIME_FS_READ_FUNCTION_HPP

#include "RuntimeFunction.hpp"

namespace LynxLibRuntime {
    class FSReadFunction : public RuntimeFunction {
        
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
                LOG_INFO("FSReadFunction: Invoked call with optional file instance");

                if (!optionalValue) {
                    LOG_ERROR("FSReadFunction: Expected a File struct instance as value");
                    return nullptr;
                }
    
                auto& builder = astContext->getBuilder();
                auto* module = astContext->getModule();
                auto& llvmCtx = astContext->getLLVMContext();
    
                // Validate that optionalValue is a pointer to a struct (File)
                llvm::Type* valType = optionalValue->getType();
                if (!valType->isPointerTy() || !valType->getPointerElementType()->isStructTy()) {
                    LOG_ERROR("FSReadFunction: Invalid file struct type passed");
                    return nullptr;
                }
    
                // Extract the file handle pointer inside the struct: file handle is second element (index 1)
                llvm::Value* fileHandlePtr = builder.CreateStructGEP(valType->getPointerElementType(), optionalValue, 1, "file_handle_ptr");
                llvm::Value* fileHandle = builder.CreateLoad(builder.getInt8PtrTy(), fileHandlePtr, "file_handle");
    
                // Bitcast fileHandle to i8* (void*) if necessary (should already be i8*)
                llvm::Value* fileHandleCast = builder.CreateBitCast(fileHandle, builder.getInt8PtrTy(), "file_handle_cast");
    
                // Declare or get the file_read_all function: const char* file_read_all(void*)
                llvm::FunctionType* readAllFuncType = llvm::FunctionType::get(
                    builder.getInt8PtrTy(),         // returns char*
                    { builder.getInt8PtrTy() },    // takes void*
                    false
                );
    
                llvm::FunctionCallee readAllFunc = module->getOrInsertFunction("file_read_all", readAllFuncType);
    
                // Call file_read_all(fileHandle)
                llvm::Value* result = builder.CreateCall(readAllFunc, { fileHandleCast }, "read_all_call");
    
                return result;    
            }


        };
    
}

#endif 
