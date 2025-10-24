#include "RuntimeBindingManager.hpp"
#include "memory/MemoryManager.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>

#include <gc.h>
#include <pthread.h>
#include <iostream>
#include <unordered_map>

namespace LynxCore {

    void RuntimeBindingManager::declareAll(llvm::Module* module) {
        if (!module) {
            std::cerr << "[RuntimeBindingManager] Error: module is null in declareAll\n";
            return;
        }

        llvm::LLVMContext& ctx = module->getContext();

        // Declare: void* GC_malloc(size_t)
        llvm::FunctionType* mallocType = llvm::FunctionType::get(
            llvm::Type::getInt8PtrTy(ctx),
            { llvm::Type::getInt64Ty(ctx) },
            false
        );
        module->getOrInsertFunction("GC_malloc", mallocType);

        // Declare: int pthread_create(pthread_t*, const pthread_attr_t*, void*(*)(void*), void*)
        llvm::FunctionType* pthreadCreateType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx),
            {
                llvm::PointerType::getUnqual(llvm::Type::getInt8PtrTy(ctx)), // opaque pthread_t*
                llvm::PointerType::getUnqual(llvm::Type::getInt8PtrTy(ctx)), // pthread_attr_t*
                llvm::PointerType::getUnqual(llvm::FunctionType::get(
                    llvm::Type::getInt8PtrTy(ctx), { llvm::Type::getInt8PtrTy(ctx) }, false
                )),
                llvm::Type::getInt8PtrTy(ctx)
            },
            false
        );
        module->getOrInsertFunction("pthread_create", pthreadCreateType);

        // Declare: int pthread_join(pthread_t, void**)
        llvm::FunctionType* pthreadJoinType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx),
            {
                llvm::Type::getInt64Ty(ctx), // assuming pthread_t = uint64_t (adjust per target)
                llvm::PointerType::getUnqual(llvm::Type::getInt8PtrTy(ctx))
            },
            false
        );
        module->getOrInsertFunction("pthread_join", pthreadJoinType);

        // Similarly declare pthread_self and pthread_equal as needed
        llvm::FunctionType* pthreadSelfType = llvm::FunctionType::get(
            llvm::Type::getInt64Ty(ctx), {}, false
        );
        module->getOrInsertFunction("pthread_self", pthreadSelfType);

        llvm::FunctionType* pthreadEqualType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx),
            { llvm::Type::getInt64Ty(ctx), llvm::Type::getInt64Ty(ctx) },
            false
        );
        module->getOrInsertFunction("pthread_equal", pthreadEqualType);
    }

    void RuntimeBindingManager::registerAll() {
        llvm::sys::DynamicLibrary::AddSymbol("GC_malloc", (void*)&GC_malloc);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_create", (void*)&pthread_create);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_join", (void*)&pthread_join);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_self", (void*)&pthread_self);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_equal", (void*)&pthread_equal);
    }

}
