#include <logger/Logger.hpp>
#include "llvm/IR/Verifier.h"
#include "MixinDeclarationNode.hpp"
#include <types/tmpl/TypeCaster.hpp>
#include "tmpl/CloneNodeTemplate.hpp"
#include "utils/LLVMFunctionUtils.hpp"
#include "tmpl/ImportSymbolTemplate.hpp"
#include "tmpl/MemberProcessorTemplate.hpp"
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxConstants;
    using namespace Cloneable;

    void MixinDeclarationNode::processMembers(std::unique_ptr<std::vector<std::unique_ptr<Node>>> members) {
        Members::Processor<MixinDeclarationNode>::process(this, std::move(members), methods, fields);  
    }

    llvm::Value* MixinDeclarationNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Executed ... {}", mixinName);

        auto mixinType = std::make_unique<MixinType>(astContext.get(), mixinName);
        buildMixinType(*astContext, *mixinType);

        llvmStructType = llvm::cast<llvm::StructType>(mixinType->getLLVMType());
        llvmPointerType = llvm::cast<llvm::PointerType>(mixinType->getLLVMPointerType());
        astContext->registerCustomType(mixinName, std::move(mixinType));

        emitMethods(*astContext, *mixinType);

        return nullptr;
    }

    void MixinDeclarationNode::emitMethods(const AstContext& astContext, const MixinType& mixinType) {
        for (const auto& method : *methods) {
            auto* llvmFunction = generateMemberFunction(astContext, mixinType, method.get());
            const auto& mangledFnName = llvmFunction->getName().str();
            auto& context = astContext.getLLVMContext();
            std::string sig = method->getSignatureString();
            auto* sigMD = llvm::MDString::get(context, sig);
            auto* node = llvm::MDNode::get(context, sigMD);
            llvmFunction->setMetadata("lynx.signature", node);
        }
    }

    llvm::Function* MixinDeclarationNode::generateMemberFunction(const AstContext& astContext, const MixinType& mixinType, FunctionNode* fnNode) {
        auto symbol = astContext.getGlobalContext();
        symbol->addFunction(fnNode);

        const auto& funcName = fnNode->getFunctionName();
        std::vector<BaseType*> mangleParams;
        const auto& funcParams = *fnNode->getFunctionParameter();
        for (const auto& param : funcParams) {
            auto pType = TypeConv::convertToBaseRawType(astContext, *param->type);
            mangleParams.push_back(pType);
        }

        auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, mixinName, funcName, mangleParams);
        auto llvmFunction = LLVMFunction::create(astContext, *fnNode, mangledName, llvmPointerType);

        auto* module = astContext.getModule();
        llvmFunction->setComdat(module->getOrInsertComdat(llvmFunction->getName()));
        llvmFunction->setDSOLocal(true);
        llvmFunction->addFnAttr(llvm::Attribute::AlwaysInline);  // encourages inlining
        llvmFunction->addFnAttr(llvm::Attribute::NoUnwind);      // no exceptions
        llvmFunction->addFnAttr(llvm::Attribute::ReadNone);      // if pure
        llvmFunction->addParamAttr(0, llvm::Attribute::NonNull);
        llvmFunction->addFnAttr("mixin", "true");

        symbol->removeFunction();
        symbol->clearLocalVariables();

        return llvmFunction;
    }

    void MixinDeclarationNode::buildMixinType(const AstContext& astContext, MixinType& mixinType) {
        // Process base mixins (multi-inheritance)
        if (hasParentMixins()) {
            for(const auto& baseName: inheritMixins) {
                auto baseType = astContext.findType(baseName);
                const auto* parentMixinType = TypeCasting::castType<const MixinType>(baseType.get());
                llvm::Type* parentLLVMType = parentMixinType->getLLVMType();
                assert(parentLLVMType->isStructTy() && "Parent mixin must lower to a StructType");
                mixinType.addParentMixin(parentMixinType);
            }
        }

        // Process fields
        unsigned fieldIdx = 0;
        for (const auto& field : *fields) {
            std::string methodName = field->getVariableName();
            auto variableType = astContext.findType(field->getVariableType()->type);
            auto fieldType = std::make_unique<FieldType>(
                field->getAccessModifier(),
                methodName, 
                std::move(variableType->clone()), 
                fieldIdx++
            );
            mixinType.addField(field->getVariableName(), std::move(fieldType));        
        }

        // Process methods
        unsigned methodIdx = 0;
        for (const auto& method : *methods) {
            const auto& funcParams = *method->getFunctionParameter();
            std::vector<std::unique_ptr<BaseType>> parameterTypes;
            std::vector<BaseType*> mangleParams;
            for (const auto& param : funcParams) {
                auto pType = TypeConv::convertToBaseType(astContext, *param->type);
                mangleParams.push_back(pType.get());
                parameterTypes.push_back(std::move(pType));
            }

            std::string methodName = method->getFunctionName();
            auto returnType = astContext.findType(method->getReturnType()->type);
            auto methodType = std::make_unique<MethodType>(
                method->getAccessModifier(),
                methodName, 
                std::move(returnType->clone()), 
                std::move(parameterTypes), 
                methodIdx++
            );
            
            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, mixinName, methodName, mangleParams);
            mixinType.addMethod(mangledName, std::move(methodType));
        }
    }

    std::unique_ptr<Node> MixinDeclarationNode::clone() const {
        auto clonedMethods = cloneNodeVector(methods);
        auto clonedFields = cloneNodeVector(fields);
        auto cloned = std::make_unique<MixinDeclarationNode>(mixinName);
        cloned->inheritMixins = inheritMixins;
        cloned->methods = std::move(clonedMethods);
        cloned->fields = std::move(clonedFields);
        return cloned;
    }
}