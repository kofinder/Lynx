/**
 * @file IOReadArrayCommand.hpp
 * @brief Defines the IOReadArrayCommand class for reading array input values.
 * 
 * The IOReadArrayCommand class extends IOReadBizCommand to enable reading arrays 
 * of primitive elements from standard input. It dynamically allocates storage 
 * for the array in LLVM IR and uses a loop to read each element via `scanf`.  
 * 
 * Example usage:
 * ```cpp
 * int[] nums = system.io.getArray(5);
 * ```
 * 
 * Currently, this implementation assumes integer (`int`) arrays, but it can be 
 * extended to support generic typed arrays in future iterations of the Lynx runtime.  
 * The command integrates tightly with LLVM’s IR builder to emit structured loops 
 * and I/O calls efficiently.
 * 
 * @autor: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_ARRAY_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_ARRAY_COMMAND_HPP

#include <unordered_map>
#include <functional>
#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadArrayCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                if (calleeArgs.empty()) {
                    throw std::runtime_error("system.io.getArray<T>(count) requires element count argument.");
                }
        
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
                llvm::Value* countVal = calleeArgs[0];
        
                // Determine element type (for simplicity assume int)
                llvm::Type* elementType = builder.getInt32Ty();
        
                // Allocate array on the stack
                llvm::Value* arrayPtr = builder.CreateAlloca(elementType, countVal, "input_array");
        
                auto* scanfFunc = getOrCreateScanf(builder.getContext(), module);
                auto* fmt = builder.CreateGlobalStringPtr("%d", "fmt_int");
        
                llvm::BasicBlock* loopBody = llvm::BasicBlock::Create(builder.getContext(), "loop.body", builder.GetInsertBlock()->getParent());
                llvm::BasicBlock* loopEnd  = llvm::BasicBlock::Create(builder.getContext(), "loop.end", builder.GetInsertBlock()->getParent());
        
                llvm::Value* i = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "i");
                builder.CreateStore(builder.getInt32(0), i);
        
                builder.CreateBr(loopBody);
                builder.SetInsertPoint(loopBody);
        
                llvm::Value* idx = builder.CreateLoad(builder.getInt32Ty(), i);
                llvm::Value* elemPtr = builder.CreateGEP(elementType, arrayPtr, idx);
        
                builder.CreateCall(scanfFunc, {fmt, elemPtr});
        
                llvm::Value* next = builder.CreateAdd(idx, builder.getInt32(1));
                builder.CreateStore(next, i);
        
                llvm::Value* cond = builder.CreateICmpSLT(next, countVal);
                builder.CreateCondBr(cond, loopBody, loopEnd);
        
                builder.SetInsertPoint(loopEnd);
        
                return arrayPtr; // pointer to array in memory
            }
    };
        
}

#endif



