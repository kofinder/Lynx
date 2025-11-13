#include <llvm/IR/Verifier.h>
#include <llvm/IR/Instructions.h>

#include "FunctionNode.hpp"
#include <logger/Logger.hpp>
#include "ClazzDeclarationNode.hpp"
#include "MixinDeclarationNode.hpp"
#include "guard/FunctionScope.hpp"
#include "utils/StringUtils.hpp"
#include "utils/LLVMFunctionUtils.hpp"
#include <context/GlobalSymbolContext.hpp>
#include <constants/LinkageType.hpp>
#include "utils/NumericPromotion.hpp"
#include <attributes/FunctionAttributeInferer.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxConstants;
    using namespace LynxFunctionAttr;
    using namespace TypePromotion;

    llvm::Value* FunctionNode::generateCode(std::shared_ptr<AstContext> astContext) {

        auto* module = astContext->getModule();
        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
        auto symbol = astContext->getGlobalContext();
        
        guard::FunctionScope funcScope(*symbol, this);

        auto* funcType = createFnSignature(*astContext);
        if (!funcType) {
            LOG_ERROR("Function signature generation failed.");
            return nullptr;
        }

        const auto funcName = getFunctionName();
        llvmFunction = LLVMFunction::createPrototype(module, funcName, funcType, extType);

        auto* mdNode = llvm::MDNode::get(context, llvm::MDString::get(context, getSignatureString()));
        llvmFunction->setMetadata("lynx.signature", mdNode);
        llvmFunction->setDSOLocal(true);

        if (!statements) return llvmFunction;

        entryBlock = llvm::BasicBlock::Create(context, "entry", llvmFunction);
        builder.SetInsertPoint(entryBlock);

        LLVMFunction::bindParameters(*symbol, llvmFunction, *fnParams, false);
    
        statements->generateCode(astContext->createContext());

        finalizeExitBlock(*astContext);
       
        FunctionAttributeInferer inferer;
        inferer.inferAndApply(llvmFunction); 

        if (auto* currentBlock = builder.GetInsertBlock()) builder.SetInsertPoint(currentBlock);
    
        llvm::verifyFunction(*llvmFunction, &llvm::errs());
        llvm::verifyModule(*module, &llvm::errs());

        return llvmFunction;
    }

    llvm::FunctionType* FunctionNode::createFnSignature(const AstContext& astContext) {
        std::vector<llvm::Type*> argTypes;        
        if (fnParams && !fnParams->empty()) {
            for (auto& param : *fnParams) {
                auto variableType = *param->type;
                auto fieldType = astContext.findType(variableType)->getLLVMType();
                argTypes.push_back(fieldType);
            }
        }

        auto* fnReturnType = astContext.findType(*returnType)->getLLVMType();
        auto* functionType = llvm::FunctionType::get(fnReturnType, argTypes, false);

        return functionType;
    }


    llvm::Value* FunctionNode::finalizeExitBlock(const AstContext& astContext) {
        auto& builder = astContext.getBuilder();
        auto* module = astContext.getModule();
        auto* retType = llvmFunction->getReturnType();
    
        // well, will be refactor when i have time
        if (!builder.GetInsertBlock()->getTerminator()) {
            if (exitBlock) {
                builder.CreateBr(exitBlock);
            } else {
                if (retType->isVoidTy()) {
                    builder.CreateRetVoid();
                } else {
                    throw std::runtime_error("Function requires a non-void return value, but none was provided.");
                }
            }
        }
    
        if (exitBlock) {
            builder.SetInsertPoint(exitBlock);
    
            if (returnValue) {
                auto* valueType = returnValue->getType();
                if (!valueType->isPointerTy()) {
                    llvm::errs() << "ERROR: returnValue is not a pointer!\n";
                    returnValue->print(llvm::errs()); llvm::errs() << "\n";
                    throw std::runtime_error("returnValue must be a pointer to load from.");
                }

                llvm::Value* retLoad = builder.CreateLoad(valueType, returnValue, LynxLabelTypeConstants::lynxTempReturnValue);
                builder.CreateRet(retLoad);
            } else {
                if (retType->isVoidTy()) {
                    builder.CreateRetVoid();
                } else {
                    throw std::runtime_error("Function requires a non-void return value, but none was provided at exit block.");
                }
            }
        }
    
        return llvmFunction;
    }

    llvm::Value* FunctionNode::setReturnValue(const AstContext& astContext, llvm::Value* value) {
        auto& builder = astContext.getBuilder();
        auto& context = astContext.getLLVMContext();
    
        if (!value) {
            throw std::runtime_error("Attempt to set a null return value.");
        }
    
        llvm::Type* retType = llvmFunction->getReturnType();
    
        // Match numeric types or perform casting if needed
        if (isNumericType(value->getType()) && isNumericType(retType)) {
            value = matchConstantType(builder, value, retType);
    
            if (value->getType() != retType) {
                if (value->getType()->isIntegerTy() && retType->isIntegerTy()) {
                    value = builder.CreateIntCast(value, retType, true, ".ret_cast");
                } else if (value->getType()->isFloatingPointTy() && retType->isFloatingPointTy()) {
                    if (value->getType()->getPrimitiveSizeInBits() < retType->getPrimitiveSizeInBits()) {
                        value = builder.CreateFPExt(value, retType, ".ret_fpext");
                    } else {
                        value = builder.CreateFPTrunc(value, retType, ".ret_fptrunc");
                    }
                } else if (value->getType()->isIntegerTy() && retType->isFloatingPointTy()) {
                    value = builder.CreateSIToFP(value, retType, ".ret_sitofp");
                } else if (value->getType()->isFloatingPointTy() && retType->isIntegerTy()) {
                    value = builder.CreateFPToSI(value, retType, ".ret_fptosi");
                }
            }
        }
    
        // Direct return if we're still at the entry block
        if (builder.GetInsertBlock() == entryBlock) {
            return builder.CreateRet(value);
        }
    
        // Allocate space for the return value if not already done
        if (!returnValue) {
            if (retType->isVoidTy()) {
                throw std::runtime_error("Attempt to set return value for void function.");
            }
    
            // Insert alloca at the start of the entry block (modern LLVM 21 way)
            llvm::IRBuilder<> tmpBuilder(&entryBlock->front());
            returnValue = tmpBuilder.CreateAlloca(retType, nullptr, ".ret_value");
        }
    
        // Store the computed value
        builder.CreateStore(value, returnValue);
    
        // Create exit block if not yet created
        if (!exitBlock) {
            exitBlock = llvm::BasicBlock::Create(context, "exit", llvmFunction);
        }
    
        // Branch to exit block
        builder.CreateBr(exitBlock);
    
        return returnValue;
    }
    

    // llvm::Value* FunctionNode::setReturnValue(const AstContext& astContext, llvm::Value* value) {
    //     auto& builder = astContext.getBuilder();
    //     auto& context = astContext.getLLVMContext();
    
    //     if (!value) {
    //         throw std::runtime_error("Attempt to set a null return value.");
    //     }
    
    //     llvm::Type* retType = llvmFunction->getReturnType();
    //     if (isNumericType(value->getType()) && isNumericType(retType)) {
    //         value = matchConstantType(builder, value, retType);
    //         if (value->getType() != retType) {
    //             if (value->getType()->isIntegerTy() && retType->isIntegerTy()) {
    //                 value = builder.CreateIntCast(value, retType, true, ".ret_cast");
    //             } else if (value->getType()->isFloatingPointTy() && retType->isFloatingPointTy()) {
    //                 if (value->getType()->getPrimitiveSizeInBits() < retType->getPrimitiveSizeInBits()) {
    //                     value = builder.CreateFPExt(value, retType, ".ret_fpext");
    //                 } else {
    //                     value = builder.CreateFPTrunc(value, retType, ".ret_fptrunc");
    //                 }
    //             } else if (value->getType()->isIntegerTy() && retType->isFloatingPointTy()) {
    //                 value = builder.CreateSIToFP(value, retType, ".ret_sitofp");
    //             } else if (value->getType()->isFloatingPointTy() && retType->isIntegerTy()) {
    //                 value = builder.CreateFPToSI(value, retType, ".ret_fptosi");
    //             }
    //         }
    //     }
    
    //     if (builder.GetInsertBlock() == entryBlock) return builder.CreateRet(value);
    
    //     if (!returnValue) {
    //         if (retType->isVoidTy()) {
    //             throw std::runtime_error("Attempt to set return value for void function.");
    //         }
    
    //         llvm::Instruction* insertBefore = entryBlock->getFirstNonPHIIt();
    //         if (!insertBefore) insertBefore = &entryBlock->back();
    //         returnValue = new llvm::AllocaInst(retType, 0, ".ret_value", insertBefore);
    //     }
    
    //     builder.CreateStore(value, returnValue);

    //     if (!exitBlock) exitBlock = llvm::BasicBlock::Create(context, "exit", llvmFunction);

    //     return builder.CreateBr(exitBlock);
    // }

    std::string FunctionNode::getCurrentClazzName() const {
        if(auto* clazz = dynamic_cast<ClazzDeclarationNode*>(clazzNode)) {
            return clazz->getOriginalName();
        }

        if(auto* mixin = dynamic_cast<MixinDeclarationNode*>(clazzNode)) {
            return mixin->getOriginalName();
        }

        return "";
    }

    std::string FunctionNode::getSignatureString() const {
        auto fnSignature = StringUtils::fnSignatureString(*llvmFunction, fnName, returnType->name, *fnParams);
        return fnSignature;
    }

    std::unique_ptr<Node> FunctionNode::clone() const {
        auto clonedFunction = std::make_unique<FunctionNode>(returnType, fnName, fnParams);
        std::unique_ptr<StatementListNode> clonedStatements;
       
        if (statements) {
            auto clonedNode = statements->clone();
            auto clonedStatements = std::unique_ptr<StatementListNode>(static_cast<StatementListNode*>(clonedNode.release()));
            clonedFunction->setFunctionBody(std::move(clonedStatements));
        }
        
        clonedFunction->setAccessModifier(accessModifier);
        clonedFunction->setVirtual(isVirtual);
        clonedFunction->setOverride(isOverride);
        clonedFunction->setClazzNode(clazzNode); 

        return clonedFunction;
    }
}