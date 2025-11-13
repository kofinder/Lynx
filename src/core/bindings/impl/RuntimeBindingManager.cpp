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

extern "C" void* LYNX_GC_ALLOC_generic(uint64_t size);

namespace LynxCore {

    static MemoryManager* g_boundMemoryManager = nullptr;

    void RuntimeBindingManager::setMemoryManager(MemoryManager* mgr) {
        g_boundMemoryManager = mgr;
    }

    MemoryManager* RuntimeBindingManager::getBoundMemoryManager() {
        return g_boundMemoryManager;
    }

    void RuntimeBindingManager::declareAll(llvm::Module* module) {
        if (!module) {
            std::cerr << "[RuntimeBindingManager] Error: module is null in declareAll\n";
            return;
        }
    
        auto& ctx = module->getContext();
    
        // opaque i8* for GC_malloc
        auto* i8PtrTy = llvm::PointerType::get(ctx, 0); 
    
        // void* GC_malloc(size_t)
        auto* mallocType = llvm::FunctionType::get(
            i8PtrTy,
            { llvm::Type::getInt64Ty(ctx) },
            false
        );
        module->getOrInsertFunction("GC_malloc", mallocType);
    
        // int pthread_create(pthread_t*, const pthread_attr_t*, void*(*)(void*), void*)
        auto* pthreadCreateType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx),
            {
                i8PtrTy, // opaque pthread_t*
                i8PtrTy, // opaque pthread_attr_t*
                llvm::PointerType::get(
                    llvm::FunctionType::get(
                        i8PtrTy, // returns void*
                        { i8PtrTy }, // argument void*
                        false
                    )->getContext(),
                    0
                ),
                i8PtrTy // void* argument
            },
            false
        );
        module->getOrInsertFunction("pthread_create", pthreadCreateType);
    
        // int pthread_join(pthread_t, void**)
        auto* pthreadJoinType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx),
            {
                llvm::Type::getInt64Ty(ctx),           // pthread_t as uint64_t
                llvm::PointerType::get(i8PtrTy->getContext(), 0) // void**
            },
            false
        );
        module->getOrInsertFunction("pthread_join", pthreadJoinType);
    
        // pthread_self()
        auto* pthreadSelfType = llvm::FunctionType::get(
            llvm::Type::getInt64Ty(ctx),
            {},
            false
        );
        module->getOrInsertFunction("pthread_self", pthreadSelfType);
    
        // pthread_equal(pthread_t, pthread_t)
        auto* pthreadEqualType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx),
            { llvm::Type::getInt64Ty(ctx), llvm::Type::getInt64Ty(ctx) },
            false
        );
        module->getOrInsertFunction("pthread_equal", pthreadEqualType);
    }
       

    void RuntimeBindingManager::declareGCAllocFunction(llvm::Module* module, const std::string& typeName) {
        if (!module) return;
        auto& ctx = module->getContext();
    
        auto* int64Ty = llvm::Type::getInt64Ty(ctx);
        llvm::Type* i8Ty = llvm::Type::getInt8Ty(ctx);               // i8
        llvm::PointerType* voidPtrTy = llvm::PointerType::get(i8Ty->getContext(), 0); // i8*

        // auto* voidPtrTy = llvm::PointerType::get(ctx, 0);
    
        auto* allocFnType = llvm::FunctionType::get(voidPtrTy, { int64Ty }, false);
        std::string fnName = "LYNX_GC_ALLOC_" + typeName;
    
        module->getOrInsertFunction(fnName, allocFnType);
    }    

    void RuntimeBindingManager::registerAll() {
        llvm::sys::DynamicLibrary::AddSymbol("GC_malloc", (void*)&GC_malloc);
        llvm::sys::DynamicLibrary::AddSymbol("GC_realloc", (void*)&GC_realloc);
        llvm::sys::DynamicLibrary::AddSymbol("GC_free", (void*)&GC_free);    
        llvm::sys::DynamicLibrary::AddSymbol("pthread_create", (void*)&pthread_create);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_join", (void*)&pthread_join);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_self", (void*)&pthread_self);
        llvm::sys::DynamicLibrary::AddSymbol("pthread_equal", (void*)&pthread_equal);
    }

    void RuntimeBindingManager::registerGCAllocFunction(const std::string& typeName) {
        std::string fnName = "LYNX_GC_ALLOC_" + typeName;
        llvm::sys::DynamicLibrary::AddSymbol(fnName, (void*)&LYNX_GC_ALLOC_generic);
        std::cout << "[RuntimeBindingManager] Registered GC alloc symbol: " << fnName << std::endl;
    }
    
    void RuntimeBindingManager::setupGCForClasses(llvm::Module* module, const std::vector<std::string>& classNames) {
        for (const auto& name : classNames) {
            declareGCAllocFunction(module, name);
            registerGCAllocFunction(name);
        }
    }

}
