#ifndef LYNX_HPP
#define LYNX_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <memory>

struct TypeRegistry {

    llvm::LLVMContext& ctx;

    // Basic types
    llvm::Type* i8;
    llvm::PointerType* i8Ptr;
    llvm::Type* i64;
    llvm::PointerType* i8PtrPtr;
    llvm::Type* voidTy;

    // Struct types (your runtime structs)
    llvm::StructType* CharStruct;
    llvm::StructType* StringStruct;
    llvm::StructType* IntStruct;
    llvm::StructType* BoolStruct;

    TypeRegistry(llvm::LLVMContext& context) : ctx(context) {
        // Primitive types
        i8 = llvm::Type::getInt8Ty(ctx);
        i8Ptr = llvm::PointerType::get(i8, 0);            // i8*
        i8PtrPtr = llvm::PointerType::get(i8Ptr, 0);     // i8**
        i64 = llvm::Type::getInt64Ty(ctx);
        voidTy = llvm::Type::getVoidTy(ctx);

        // Structs (opaque for now, can define body later)
        CharStruct = llvm::StructType::create(ctx, "Char");
        StringStruct = llvm::StructType::create(ctx, "String");
        IntStruct = llvm::StructType::create(ctx, "Int");
        BoolStruct = llvm::StructType::create(ctx, "Bool");
    }

    // Helper to get common function types
    llvm::FunctionType* getMallocFuncType() const {
        return llvm::FunctionType::get(i8Ptr, { i64 }, false);
    }

    llvm::FunctionType* getOpenFuncType() const {
        return llvm::FunctionType::get(i8Ptr, { i8Ptr }, false);
    }

    llvm::FunctionType* getWriteFuncType() const {
        return llvm::FunctionType::get(voidTy, { i8Ptr, i8Ptr }, false);
    }

    llvm::FunctionType* getCloseFuncType() const {
        return llvm::FunctionType::get(voidTy, { i8Ptr }, false);
    }
};

#endif