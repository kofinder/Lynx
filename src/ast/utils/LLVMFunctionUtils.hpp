/**
 * @file LLVMFunctionUtils.hpp
 * @brief Utility functions to simplify LLVM function creation, parameter binding, and finalization.
 *
 * The LLVMFunction namespace provides helper functions to create LLVM function prototypes,
 * entry blocks, bind parameters, generate statements, and finalize functions in LLVM IR.
 *
 * **Key Responsibilities:**
 * - Create LLVM function prototypes and entry blocks.
 * - Bind AST function parameters to LLVM arguments.
 * - Generate LLVM code for function bodies.
 * - Ensure functions are declared in LLVM modules.
 *
 * **Used By:**
 * - FunctionNode code generation routines.
 * - Class method generation in the compiler backend.
 *
 * @see AstContext, FunctionNode, GlobalSymbolContext, TypeConventionTemplate
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_LLVM_FUNCTION_UTILS_HPP
#define LYNX_LLVM_FUNCTION_UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <optional>

#include "LiteralNode.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Verifier.h>
#include <constants/LinkageType.hpp>
#include "tmpl/TypeConventionTemplate.hpp"
#include <context/GlobalSymbolContext.hpp>


namespace LynxAst::LLVMFunction {

    using namespace LynxTypes;
    using namespace LynxContext;
    using namespace LynxConstants;

    /**
     * @brief Creates an LLVM function prototype without generating a body.
     * 
     * @param module    LLVM module where the function will be inserted.
     * @param fnName    Function name.
     * @param fnType    Function signature (LLVM type).
     * @param linkage   Linkage type (default: ExternalLinkage).
     * @return Pointer to the newly created LLVM function.
     */
    inline llvm::Function* createPrototype(llvm::Module* module, const std::string& fnName, llvm::FunctionType* fnType, llvm::GlobalValue::LinkageTypes linkage) {
        auto* fn = llvm::Function::Create(fnType, linkage, fnName, module);
        llvm::verifyFunction(*fn);
        return fn;
    }

    /**
     * @brief Creates an LLVM entry basic block for a function.
     * 
     * @param context   LLVM context.
     * @param fn        Target LLVM function.
     * @param blockName Name of the basic block (default: "entry").
     * @return Pointer to the created basic block.
     */
    inline llvm::BasicBlock* createEntryBlock(llvm::LLVMContext& context, llvm::Function* fn, const std::string& blockName = "entry") {
        return llvm::BasicBlock::Create(context, blockName, fn);
    }

    /**
     * @brief Binds function parameters to AST local variables.
     * 
     * @param astContext AST context where local variables are stored.
     * @param fn         LLVM function whose parameters will be bound.
     * @param params     Parameter list.
     * @param includeThis Whether to bind the first argument as "this".
     */
    inline void bindParameters(GlobalSymbolContext& symbol, llvm::Function* fn, const std::vector<std::shared_ptr<Parameter>>& params, bool includeThis = true) {
        auto argIter = fn->arg_begin();

        if (includeThis && argIter != fn->arg_end()) {
            llvm::Argument* thisArg = &*argIter++;
            thisArg->setName("this");
            symbol.registerLocalVariable("this", thisArg);
        }

        for (auto& param : params) {
            if (argIter == fn->arg_end()) break;
            llvm::Argument* llvmArg = &*argIter++;
            llvmArg->setName(param->name);
            symbol.registerLocalVariable(param->name, llvmArg);
        }
    }

    /**
     * @brief Finalizes an LLVM function by inserting a default return if missing
     *        and running LLVM verification.
     * 
     * @param fn         Target LLVM function.
     * @param module     LLVM module containing the function.
     * @param returnType Expected return type.
     * @param builder    LLVM IR builder.
     */
    inline void finalizeFunction(llvm::Function* fn, llvm::Module* module, llvm::Type* returnType, llvm::IRBuilder<>& builder) {
        for (auto& bb : *fn) {
            if (!bb.getTerminator()) {
                builder.SetInsertPoint(&bb);
                if (returnType->isVoidTy()) {
                    builder.CreateRetVoid();
                } else {
                    builder.CreateRet(llvm::Constant::getNullValue(returnType));
                }
            }
        }

        llvm::verifyFunction(*fn, &llvm::errs());
        llvm::verifyModule(*module, &llvm::errs());
    }

    /**
     * @brief Creates and defines a full LLVM function, binding parameters
     *        and generating the function body.
     * 
     * @param astContext    Current AST context.
     * @param functionName  Function name (unqualified).
     * @param qualifiedName Fully qualified function name.
     * @param statements    AST node list representing the function body.
     * @param llvmPointerType LLVM pointer type for "this".
     * @param returnType    LLVM return type.
     * @param parameters    Function parameters.
     * @return Pointer to the created LLVM function.
     */
    inline llvm::Function* create(
        const AstContext& astContext,
        FunctionNode& fnNode,
        const std::string& qualifiedName,
        llvm::PointerType* llvmPointerType
    ) {
        auto& context = astContext.getLLVMContext();
        auto& builder = astContext.getBuilder();
        auto* module = astContext.getModule();

        auto fnParams = fnNode.getFunctionParameter();
        auto* statements = fnNode.getFunctionStatements();
        const std::string& functionName = fnNode.getFunctionName();


        auto* returnType = TypeConv::convertToLLVMType(astContext, *fnNode.getReturnType()); 

        // Build argument type list
        std::vector<llvm::Type*> argTypes = TypeConv::convertToLLVMTypes(astContext, *fnParams);
        if (llvmPointerType) argTypes.insert(argTypes.begin(), llvmPointerType);
        
        // Create function prototype
        auto* functionType = llvm::FunctionType::get(returnType, argTypes, false);
        auto* fn = createPrototype(module, qualifiedName, functionType, linkOnceODRType);
        fnNode.setLLVMFunction(fn);

        auto* entry = createEntryBlock(context, fn);
        fnNode.setEntryBlock(entry); 
        builder.SetInsertPoint(entry);
        bindParameters(*astContext.getGlobalContext(), fn, *fnParams, true);

        // Generate statements
        if (statements) {
            statements->generateCode(astContext.createContext());
        } 
        
        // Finalize function
        finalizeFunction(fn, module, returnType, builder);
        
        return fn;
    }

   /**
     * @brief Ensures a function with the given signature exists in the LLVM module.
     *        If it doesn't exist, it is inserted.
     * 
     * @param module       LLVM module where the function should exist.
     * @param returnType   LLVM return type of the function.
     * @param paramTypes   LLVM parameter types.
     * @param mangledName  Fully qualified or mangled function name.
     * @param callingConv  Optional calling convention (default: C).
     * @return Pointer to the declared function, or nullptr on failure.
    */
    inline llvm::Function* ensureFunctionDeclared(
        llvm::Module* module,
        llvm::Type* returnType,
        const std::vector<llvm::Type*>& paramTypes,
        const std::string& mangledName,
        llvm::CallingConv::ID callingConv = llvm::CallingConv::C
    ) {
        if (!module || !returnType) return nullptr;

        auto* funcType = llvm::FunctionType::get(returnType, paramTypes, false);
        auto callee = module->getOrInsertFunction(mangledName, funcType);

        if (auto* func = llvm::dyn_cast<llvm::Function>(callee.getCallee())) {
            func->setLinkage(extType);
            func->setCallingConv(callingConv);
            return func;
        }

        return nullptr;
    }

    /**
     * @brief Convenience wrapper for MethodType metadata.
     *        Extracts return and parameter types automatically.
     * 
     * @param module       LLVM module where the function should exist.
     * @param method       Method type metadata.
     * @param mangledName  Fully qualified or mangled function name.
     * @param callingConv  Optional calling convention (default: C).
     * @return Pointer to the declared function, or nullptr on failure.
     */
    inline llvm::Function* ensureFunctionDeclared(
        llvm::Module* module,
        llvm::LLVMContext& context,
        const MethodType* method,
        const std::string& mangledName,
        llvm::CallingConv::ID callingConv = llvm::CallingConv::C
    ) {
        if (!method) return nullptr;

        llvm::Type* returnType = method->getReturnType()->getLLVMType();
        std::vector<llvm::Type*> params;
        for (const auto& p : method->getParameterTypes()) {
            params.push_back(p->getLLVMType());
        }

        return ensureFunctionDeclared(module, returnType, params, mangledName, callingConv);
    }

}

#endif
