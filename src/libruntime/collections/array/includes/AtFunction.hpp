#ifndef LYNX_LIB_RUNTIME_ARRAY_AT_FUNCTION_HPP
#define LYNX_LIB_RUNTIME_ARRAY_AT_FUNCTION_HPP

#include <string>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

#include "RuntimeFunction.hpp"
#include <types/sequential/ArrayType.hpp>
#include <types/tmpl/TypeCaster.hpp>

namespace LynxLibRuntime {

    class AtFunction : public RuntimeFunction {

        private:
            // Emit runtime error and abort
            void callRuntimeError(llvm::IRBuilder<> &builder, llvm::Module *module, llvm::StringRef msg) {
                auto& ctx = builder.getContext();

                // Declare or get 'puts'
                llvm::Function* putsFunc = module->getFunction("puts");
                if (!putsFunc) {
                    auto putsType = llvm::FunctionType::get(builder.getInt32Ty(), llvm::PointerType::get(ctx, 0), false);
                    putsFunc = llvm::Function::Create(putsType, llvm::Function::ExternalLinkage, "puts", module);
                }

                // Declare or get 'abort'
                llvm::Function* abortFunc = module->getFunction("abort");
                if (!abortFunc) {
                    auto abortType = llvm::FunctionType::get(builder.getVoidTy(), false);
                    abortFunc = llvm::Function::Create(abortType, llvm::Function::ExternalLinkage, "abort", module);
                }

                // Emit global string for message
                llvm::Value* errMsg = builder.CreateGlobalString(msg, "runtime_error_msg");

                // Call puts(message)
                builder.CreateCall(putsFunc, errMsg);
                builder.CreateCall(abortFunc);
                builder.CreateUnreachable();
            }

            // Insert bounds check logic: aborts if index >= arraySize
            void validateArrayBound(llvm::Value* index, uint64_t arraySize, std::shared_ptr<AstContext> astContext) {
                auto &builder = astContext->getBuilder();
                llvm::LLVMContext &ctx = builder.getContext();
                llvm::Module *module = builder.GetInsertBlock()->getModule();

                llvm::Value* upperBound = llvm::ConstantInt::get(index->getType(), arraySize);
                llvm::Value* isValid = builder.CreateICmpULT(index, upperBound, "bounds_check");

                llvm::Function* func = builder.GetInsertBlock()->getParent();
                llvm::BasicBlock* okBlock = llvm::BasicBlock::Create(ctx, "in_bounds", func);
                llvm::BasicBlock* errorBlock = llvm::BasicBlock::Create(ctx, "out_of_bounds", func);

                builder.CreateCondBr(isValid, okBlock, errorBlock);

                // Fill error block
                builder.SetInsertPoint(errorBlock);
                std::string valueStr;
                llvm::raw_string_ostream rso(valueStr);
                index->print(rso);
                rso.flush();
                std::string msg = "Array.at(index = " + valueStr + "): Index out of bounds!";
                callRuntimeError(builder, module, msg);

                // Continue building in ok block
                builder.SetInsertPoint(okBlock);
            }

            // Get the array[index] element
            llvm::Value* getArrayElement(std::shared_ptr<AstContext> ctx, llvm::Value* arrayPtr, llvm::Value* index) {
                auto& builder = ctx->getBuilder();
                auto& llvmCtx = ctx->getLLVMContext();

                auto* i8PtrTy = llvm::PointerType::get(llvmCtx, 0);

                auto* innerArrayPtr = builder.CreateStructGEP(i8PtrTy, arrayPtr, 0, "array_inner");
                auto* elementPtr = builder.CreateGEP(i8PtrTy, innerArrayPtr, {builder.getInt32(0), index}, "array_element_ptr");
                return builder.CreateLoad(i8PtrTy, elementPtr, "array_element");
            }

        public:
        
            llvm::Value* call(std::shared_ptr<AstContext> astContext, std::vector<std::unique_ptr<ExpressionNode>> args) override {
                LOG_ERROR("Array.at requires array instance and index.");
                astContext->reportError(makeRuntimeError("Array.at requires array instance and index."));
                return nullptr;
            }

            llvm::Value* call(std::shared_ptr<AstContext> astContext,
                            std::vector<std::unique_ptr<ExpressionNode>> args,
                            llvm::Value* optionalValue) override {
                if (!optionalValue) {
                    LOG_ERROR("Array.at: array instance is null.");
                    astContext->reportError(makeRuntimeError("Array.at: array instance is null."));
                    return nullptr;
                }

                if (args.size() != 1) {
                    LOG_ERROR("Array.at: expected one argument (index).");
                    astContext->reportError(makeRuntimeError("Array.at: expected one argument (index)."));
                    return nullptr;
                }

                llvm::Value* index = args[0]->generateCode(astContext);
                if (!index) {
                    LOG_ERROR("Array.at: failed to evaluate index expression.");
                    astContext->reportError(makeRuntimeError("Array.at: failed to evaluate index expression."));
                    return nullptr;
                }

                auto baseType = astContext->findType(optionalValue);
                auto arrayType = TypeCasting::castType<ArrayType>(baseType.get());
                size_t arraySize = arrayType->getNumElements();

                validateArrayBound(index, arraySize, astContext);
                return getArrayElement(astContext, optionalValue, index);
        }
    };

}

#endif
