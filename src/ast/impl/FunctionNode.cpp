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
#include <attributes/interfaces/FunctionAttributeInferer.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxConstants;
    using namespace LynxFunctionAttr;

    llvm::Value* FunctionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Dispatched {}", fnName);

        auto* module = astContext->getModule();
        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
        auto symbol = astContext->getGlobalContext();
        
        guard::FunctionScope funcScope(*symbol, this);

        llvm::FunctionType* funcType = createFnSignature(*astContext);

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

        if (auto* currentBlock = builder.GetInsertBlock()) {
            builder.SetInsertPoint(currentBlock);
        }
    
        llvm::verifyFunction(*llvmFunction, &llvm::errs());
        llvm::verifyModule(*module, &llvm::errs());

        return llvmFunction;
    }

    llvm::FunctionType* FunctionNode::createFnSignature(AstContext& astContext) {
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


    llvm::Value* FunctionNode::finalizeExitBlock(AstContext& astContext) {
        LOG_ERROR("Invoked ...");

        auto& builder = astContext.getBuilder();
        auto* module = astContext.getModule();
        auto* retType = llvmFunction->getReturnType();

        // Insert a branch/return if the current block has no terminator.
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

        // If an exit block exists, finalize it.
        if(exitBlock) {
            builder.SetInsertPoint(exitBlock);
            if(returnValue) {
                auto valueType = returnValue->getType();
                if (!valueType->isPointerTy()) {
                    llvm::errs() << "ERROR: returnValue is not a pointer!\n";
                    returnValue->print(llvm::errs()); llvm::errs() << "\n";
                    throw std::runtime_error("returnValue must be a pointer to load from.");
                }

                auto* retPtr = returnValue->getType()->getPointerElementType();
                auto* retLoad = builder.CreateLoad(retPtr, returnValue, LynxLabelTypeConstants::lynxTempReturnValue);
                builder.CreateRet(returnValue);
            } else {
                if(retType->isVoidTy()) {
                    builder.CreateRetVoid();
                } else {
                    builder.CreateRet(returnValue);
                }
            }
        }

        return llvmFunction;
    }

    llvm::Value* FunctionNode::setReturnValue(std::shared_ptr<AstContext> astContext, llvm::Value* value) {
        LOG_ERROR("Invoked ...");

        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
        auto* module = astContext->getModule();

        if (builder.GetInsertBlock() == entryBlock) {
            return builder.CreateRet(value);
        }

        if (!returnValue) {
            llvm::Type* returnType = llvmFunction->getReturnType();
            llvm::Instruction* insertBefore = entryBlock->getFirstNonPHI();
            if (!insertBefore) {
                insertBefore = &entryBlock->back();
            }

            returnValue = new llvm::AllocaInst(returnType, 0, ".retValue", insertBefore);
        }
    
        if (!exitBlock) {
            exitBlock = llvm::BasicBlock::Create(context, "exit", llvmFunction);
        }
    
        builder.CreateStore(value, returnValue);

        return builder.CreateBr(exitBlock);
    }

    std::string FunctionNode::getCurrentClazzName() const {
        if(auto* clazz = dynamic_cast<ClazzDeclarationNode*>(clazzNode)) {
            return clazz->getOriginalName();
        }

        if(auto* mixin = dynamic_cast<MixinDeclarationNode*>(clazzNode)) {
            return mixin->getOriginalName();
        }

        return nullptr;
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

        // // Reset IR-related fields
        // clonedFunction->entryBlock = nullptr;
        // clonedFunction->exitBlock = nullptr;
        // clonedFunction->llvmFunction = nullptr;
        // clonedFunction->returnValue = nullptr;
        return clonedFunction;
    }
}