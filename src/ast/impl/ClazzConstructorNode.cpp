#include <logger/Logger.hpp>
#include "llvm/IR/Verifier.h"
#include "ClazzConstructorNode.hpp"
#include "SuperConstructorCallNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <types/tmpl/TypeCaster.hpp>
#include "tmpl/TypeConventionTemplate.hpp"
#include "ClazzConstructorExpressionNode.hpp"


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxTypes;

    llvm::Value* ClazzConstructorNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Generating constructor for class: {}", constructorName);

        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        auto symbol = astContext->getGlobalContext();

        auto baseType = astContext->findType(constructorName);
        if(!baseType) throw std::runtime_error("Error, Class Type not found" + constructorName);
        
        auto clazzType = TypeCasting::castType<const ClassType>(baseType.get());
        auto* llvmStructType = llvm::dyn_cast<llvm::StructType>(clazzType->getLLVMType());
        auto* llvmPointerType = llvm::dyn_cast<llvm::PointerType>(clazzType->getLLVMPointerType());

        if (!llvmStructType || !llvmPointerType) {
            throw std::runtime_error("Invalid LLVM type for class: " + constructorName);
        }

        std::vector<llvm::Type*> argumentTypes = TypeConv::convertToLLVMTypes(*astContext, *parameters);
        argumentTypes.insert(argumentTypes.begin(), llvmPointerType);

        std::string qualifiedName = getQualifiedName(*astContext);
        auto* ctorType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), argumentTypes, false);
        auto* ctorFn = llvm::Function::Create(ctorType, llvm::Function::LinkOnceODRLinkage, qualifiedName, module);

        auto* entryBB = llvm::BasicBlock::Create(context, "entry", ctorFn);    
        builder.SetInsertPoint(entryBB);

        auto argIter = ctorFn->arg_begin();
        argIter->setName("this");
        llvm::Value* thisPtr = &*argIter++;
        symbol->registerLocalVariable("this", thisPtr);
        
        std::vector<llvm::Value*> argOfValues;
        std::vector<std::string> argOfNames;
        for(auto& param: *parameters) {
            argIter->setName(param->name);
            argOfValues.push_back(&*argIter);
            argOfNames.push_back(param->name);
            ++argIter;
        }

        if(superCallNode.has_value() && superCallNode.value() && clazzType->hasBaseClass()) {
            auto* baseClazz = clazzType->getBaseClass();
            superCallNode.value()->generate(*astContext, *baseClazz, argOfValues, argOfNames);
        }

        std::unordered_set<std::string> explicitlyAssigned;
        for(const auto& expNode: *expressions) {
            if (auto* assignNode = dynamic_cast<ClazzConstructorExpressionNode*>(expNode.get())) {
                assignNode->generate(*astContext, *clazzType, argOfValues, argOfNames);
                explicitlyAssigned.insert(assignNode->getFieldName());
            } else {
                LOG_ERROR("Unexpected node type in constructor body");
            }
        }

        // Zero-init all fields not covered by constructor expressions
        const auto& fields = clazzType->getFields();
        for(const auto& [fieldName, field]: fields) {
            if (explicitlyAssigned.contains(fieldName)) continue;

            llvm::Value* fieldType = field->getType()->getDefaultValue();
            if (!fieldType) {
                LOG_WARN("No default value for field '{}'", fieldName);
                continue;
            }

            int fieldIndex = clazzType->getFieldIndex(fieldName);
            auto* fieldPtr = builder.CreateStructGEP(llvmStructType, thisPtr, fieldIndex, fieldName + "_ptr");
            builder.CreateStore(fieldType, fieldPtr);
        }

        builder.CreateRetVoid();

        symbol->clearLocalVariables();

        llvm::verifyFunction(*ctorFn, &llvm::errs());
        llvm::verifyModule(*module);

        return ctorFn;
    }

    std::string ClazzConstructorNode::getQualifiedName(const AstContext& context) const {
        std::vector<BaseType*> mangleParams;
        for (const auto& param : *parameters) {
            auto baseType = TypeConv::convertToBaseRawType(context, *param->type);
            mangleParams.push_back(baseType);
        }
        return Mangle::get(ManglerKind::CONSTRUCTOR, constructorName, mangleParams); 
    }

    std::unique_ptr<Node> ClazzConstructorNode::clone() const {   
        auto clonedExpressions = Cloneable::cloneNodeVector(expressions);
        auto clonedSuperCallNode = Cloneable::cloneOptionalNode(superCallNode);
        auto cloned = std::make_unique<ClazzConstructorNode>(
            accessModifierType,
            constructorName,
            parameters,
            std::move(clonedExpressions),
            std::move(clonedSuperCallNode)
        );

        return cloned;
    }
}