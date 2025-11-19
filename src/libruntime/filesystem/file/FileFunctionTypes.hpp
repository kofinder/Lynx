#ifndef LYNX_FILE_OPS_FUNCTION_TYPES_HPP
#define LYNX_FILE_OPS_FUNCTION_TYPES_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>

namespace LynxLibRuntime::FileOps {

    inline llvm::FunctionType* getOpenFuncType(llvm::LLVMContext& ctx) {
        static llvm::FunctionType* funcType = llvm::FunctionType::get(
            llvm::PointerType::get(ctx, 0),
            { llvm::PointerType::get(ctx, 0) },
            false
        );
        return funcType;
    }

    inline llvm::FunctionType* getWriteFuncType(llvm::LLVMContext& ctx) {
        static llvm::FunctionType* funcType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(ctx),
            { llvm::PointerType::get(ctx, 0), llvm::PointerType::get(ctx, 0) },
            false
        );
        return funcType;
    }

    inline llvm::FunctionType* getCloseFuncType(llvm::LLVMContext& ctx) {
        static llvm::FunctionType* funcType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(ctx),
            { llvm::PointerType::get(ctx, 0), },
            false
        );
        
        return funcType;
    }

}

#endif
