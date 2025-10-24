#ifndef LYNX_COLLECTION_UTILS
#define LYNX_COLLECTION_UTILS

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace LynxAst::CollectionUtils {
    
    void emitMemcpyFromConst(
        AstContext* astContext,
        llvm::Value* destAlloca,                     // Alloca for runtime array
        llvm::Constant* constSource                 // Constant (e.g. ConstantStruct or Global)
    ) {

        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        // Get type of the constant (should be same as alloca)
        llvm::Type* type = constSource->getType();
    
        // Get pointer types
        llvm::Type* i8PtrTy = llvm::Type::getInt8PtrTy(context);
    
        // Bitcast both to i8*
        llvm::Value* destI8 = builder.CreateBitCast(destAlloca, i8PtrTy, "dest.cast");
        llvm::Value* srcI8 = builder.CreateBitCast(constSource, i8PtrTy, "src.cast");
    
        // Compute size from the type
        const llvm::DataLayout& dataLayout = module->getDataLayout();
        uint64_t sizeInBytes = dataLayout.getTypeAllocSize(type);
        llvm::Value* sizeVal = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), sizeInBytes);
    
        // Optional: alignment
        llvm::MaybeAlign align(llvm::Align(4));  // or compute from type
    
        // Emit memcpy
        builder.CreateMemCpy(destI8, align, srcI8, align, sizeVal);
    }
    
}

#endif 
