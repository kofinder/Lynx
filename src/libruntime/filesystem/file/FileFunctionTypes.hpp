#ifndef LYNX_FILE_OPS_FUNCTION_TYPES_HPP
#define LYNX_FILE_OPS_FUNCTION_TYPES_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>

namespace LynxLibRuntime::FileOps {

    inline llvm::FunctionType* getOpenFuncType(llvm::LLVMContext& ctx) {
        static llvm::FunctionType* funcType = llvm::FunctionType::get(
            llvm::Type::getInt8PtrTy(ctx),
            { llvm::Type::getInt8PtrTy(ctx) },
            false
        );
        return funcType;
    }

    inline llvm::FunctionType* getWriteFuncType(llvm::LLVMContext& ctx) {
        static llvm::FunctionType* funcType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(ctx),
            { llvm::Type::getInt8PtrTy(ctx), llvm::Type::getInt8PtrTy(ctx) },
            false
        );
        return funcType;
    }

    inline llvm::FunctionType* getCloseFuncType(llvm::LLVMContext& ctx) {
        static llvm::FunctionType* funcType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(ctx),
            { llvm::Type::getInt8PtrTy(ctx), },
            false
        );
        
        return funcType;
    }

}

#endif
