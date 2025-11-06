#ifndef LYNX_HEAP_ALLOCATOR_HPP
#define LYNX_HEAP_ALLOCATOR_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <context/AstContext.hpp>
#include <types/tmpl/TypeCaster.hpp>

namespace LynxContext {

    using namespace LynxTypes;

    /**
     * @class HeapAllocator
     * @brief Utility class for LLVM heap memory allocation using the Boehm GC.
     * 
     * Provides static methods to allocate LLVM types on the heap via `GC_malloc`.
     * Intended to be used during code generation when constructing complex objects.
     */
    class HeapAllocator {

        private:

            /**
             * @brief Allocates heap memory for a given LLVM type.
             *
             * Uses `GC_malloc` from the Boehm Garbage Collector to perform the allocation,
             * calculates the size based on the LLVM type and returns a pointer casted to
             * the appropriate type.
             * 
             * @param astContext The AST context containing the LLVM state.
             * @param llvmType The LLVM type to allocate memory for.
             * @param name Optional name prefix for the allocation instructions.
             * @return A pointer (as an LLVM Value) to the allocated memory block.
             * 
             * @throws If the LLVM type is not sized.
             */
            static llvm::Value* allocate(
                const AstContext& astContext,
                llvm::Type* llvmType,
                const std::string& name = "heap_alloc"
            ) {
                auto dataLayout = astContext.getDataLayout();
                auto& builder = astContext.getBuilder();
                auto& ctx = astContext.getLLVMContext();

                if (!llvmType->isSized()) {
                    llvm::errs() << "ERROR: LLVM type is unsized:\n";
                    llvmType->print(llvm::errs());
                    llvm::errs() << "\n";
                    llvm::report_fatal_error("Unsized type in heap allocation");
                }

                uint64_t typeSize = dataLayout.getTypeAllocSize(llvmType);
                auto* allocSize = llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), typeSize);

                auto* mallocFn = getOrInsertMallocFunction(astContext, allocSize->getType(), "GC_malloc");
                auto* mallocCall = builder.CreateCall(mallocFn, { allocSize }, name + "_raw");

                auto objectInstance = builder.CreateBitCast(mallocCall, llvmType->getPointerTo(), name + "_cast");

                auto baseType = astContext.findType(objectInstance);

                if(auto clazzType = TypeCasting::castType<ClassType>(baseType.get())) {
                    LOG_INFO("Binding Vtable to the {}", clazzType->qualifiedName());
                    clazzType->bindVTable(objectInstance);
                }

                return objectInstance;
            }

        public:
        
            /**
             * @brief Declares or retrieves a malloc function from the LLVM module.
             *
             * Ensures the Boehm GC malloc function (`GC_malloc`) is declared and returns
             * a function pointer to it.
             * 
             * @param astContext The AST context with access to the module and LLVM state.
             * @param sizeArgType The type of the size argument (usually i64).
             * @param fnName The name of the malloc function to retrieve (default: "GC_malloc").
             * @return A pointer to the LLVM function representing malloc.
             */
            static llvm::Function* getOrInsertMallocFunction(
                const AstContext& astContext,
                llvm::Type* sizeArgType,
                const std::string& fnName = "GC_malloc"
            ) {
                auto* module = astContext.getModule();
                auto& context = astContext.getLLVMContext();

                auto* voidPtrType = llvm::Type::getInt8PtrTy(context);

                auto mallocType = llvm::FunctionType::get(voidPtrType, { sizeArgType }, false);
                auto mallocCallee = module->getOrInsertFunction(fnName, mallocType);

                llvm::Function* mallocFn = nullptr;
                if (auto* func = llvm::dyn_cast<llvm::Function>(mallocCallee.getCallee())) {
                    mallocFn = func;
                } else {
                    mallocFn = llvm::cast<llvm::Function>(mallocCallee.getCallee()->stripPointerCasts());
                }

                return mallocFn;
            }

            /**
             * @brief Public interface to allocate a typed object on the heap.
             * 
             * Thin wrapper over the internal `allocate()` function for external use.
             * 
             * @param astContext The AST context.
             * @param llvmType The LLVM type to allocate.
             * @param name Optional name prefix.
             * @return A pointer to the allocated object (LLVM Value).
             */
            static llvm::Value* allocateTyped(
                const AstContext& astContext,
                llvm::Type* llvmType,
                const std::string& name = "heap_alloc"
            ) {
                return allocate(astContext, llvmType, name);
            }

            /**
             * @brief Inserts or retrieves GC_free from module.
             */
            static llvm::Function* getOrInsertFreeFunction(
                AstContext* astContext,
                const std::string& fnName = "GC_free"
            ) {
                auto* module = astContext->getModule();
                auto& ctx = astContext->getLLVMContext();
                
                auto* voidType = llvm::Type::getVoidTy(ctx);
                auto* voidPtrType = llvm::Type::getInt8PtrTy(ctx);

                auto freeType = llvm::FunctionType::get(voidType, { voidPtrType }, false);
                auto freeCallee = module->getOrInsertFunction(fnName, freeType);

                if (auto* func = llvm::dyn_cast<llvm::Function>(freeCallee.getCallee())) {
                    return func;
                }

                return llvm::cast<llvm::Function>(freeCallee.getCallee()->stripPointerCasts());
            }

            /**
             * @brief Emits a call to GC_free.
             * 
             * @param astContext The AST context.
             * @param ptr The pointer to free (must be castable to i8*).
             * @param name Optional instruction name.
             */
            static void free(
                AstContext* astContext,
                llvm::Value* ptr,
                const std::string& name = "heap_free"
            ) {
                auto& builder = astContext->getBuilder();
                auto& ctx = astContext->getLLVMContext();

                auto* voidPtrType = llvm::Type::getInt8PtrTy(ctx);
                llvm::Value* casted = builder.CreateBitCast(ptr, voidPtrType, name + "_cast");

                llvm::Function* freeFn = getOrInsertFreeFunction(astContext);
                builder.CreateCall(freeFn, { casted });
            }

            /**
             * @brief Registers a GC finalizer for a heap object.
             * 
             * @param astContext AST context for LLVM builder/module access
             * @param obj The object to register the finalizer for
             * @param finalizerFn LLVM function pointer to the finalizer: void(i8*, i8*)
             * @param clientData Optional client data (can be nullptr)
            */
            static void registerFinalizer(
                AstContext* astContext,
                llvm::Value* obj,
                llvm::Function* finalizerFn,
                llvm::Value* clientData = nullptr
            ) {
                auto& builder = astContext->getBuilder();
                auto* module = astContext->getModule();
                auto& ctx = astContext->getLLVMContext();
    
                // Function type: void(i8*, void (*)(i8*,i8*), i8*, void (**)(i8*,i8*), i8**)
                llvm::Type* voidTy = llvm::Type::getVoidTy(ctx);
                llvm::Type* i8Ty = llvm::Type::getInt8Ty(ctx);
                llvm::PointerType* i8PtrTy = i8Ty->getPointerTo();
                auto* regFnTy = llvm::FunctionType::get(voidTy, {
                    i8PtrTy,                 // object
                    llvm::PointerType::get(finalizerFn->getFunctionType(), 0), // finalizer function pointer
                    i8PtrTy,                 // client data
                    llvm::PointerType::get(llvm::PointerType::get(finalizerFn->getFunctionType(), 0), 0), // old finalizer ptr
                    llvm::PointerType::get(i8PtrTy, 0) // old client data ptr
                }, false);
    
                auto regFn = module->getOrInsertFunction("GC_register_finalizer_no_order", regFnTy);
    
                // Cast object to i8*
                auto* objI8 = builder.CreateBitCast(obj, i8PtrTy);
    
                // Null for optional old function/data
                auto* nullOldFn = llvm::ConstantPointerNull::get(llvm::PointerType::get(finalizerFn->getFunctionType(), 0));
                auto* nullOldData = llvm::ConstantPointerNull::get(i8PtrTy);
    
                // Default client data
                if (!clientData) {
                    clientData = llvm::ConstantPointerNull::get(i8PtrTy);
                }
    
                builder.CreateCall(regFn, { objI8, finalizerFn, clientData, nullOldFn, nullOldData });    
            }
    };

}

#endif
