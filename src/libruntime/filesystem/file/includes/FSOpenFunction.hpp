#ifndef LYNX_LIB_RUNTIME_FS_OPEN_FUNCTION_HPP
#define LYNX_LIB_RUNTIME_FS_OPEN_FUNCTION_HPP

#include "RuntimeFunction.hpp"
#include "filesystem/file/FileFunctionTypes.hpp" 

namespace LynxLibRuntime {

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

                // Get the struct type
                auto* fileType = llvm::cast<llvm::PointerType>(optionalValue->getType())->getPointerElementType();
                if (!fileType || !fileType->isStructTy()) {
                    LOG_ERROR("Invalid file struct type");
                    return nullptr;
                }

                // Load the file path: struct field 0
                llvm::Value* filePathPtr = builder.CreateStructGEP(fileType, optionalValue, 0, "filePathPtr");
                llvm::Value* filePath = builder.CreateLoad(llvm::Type::getInt8PtrTy(llvmCtx), filePathPtr, "filePath");

                llvm::FunctionType* funcType = FileOps::getOpenFuncType(llvmCtx);
                llvm::FunctionCallee callee = module->getOrInsertFunction("file_open_default", funcType);

                // Call file_open_default(path)
                llvm::Value* fileHandle = builder.CreateCall(callee, { filePath }, "fileHandle");

                // Store the result in field 1 (handle)
                llvm::Value* fileHandlePtr = builder.CreateStructGEP(fileType, optionalValue, 1, "fileHandlePtr");
                builder.CreateStore(fileHandle, fileHandlePtr);

                return optionalValue; // Return updated struct
            }
    };
    
}

#endif 


