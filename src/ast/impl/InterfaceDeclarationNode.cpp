#include <logger/Logger.hpp>
#include "llvm/IR/Verifier.h"
#include "InterfaceDeclarationNode.hpp"
#include <constants/VariableType.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include <constants/LinkageType.hpp>
#include "tmpl/CloneNodeTemplate.hpp"
#include "utils/LLVMFunctionUtils.hpp"
#include "tmpl/VTableTemplate.hpp"
#include "tmpl/ImportSymbolTemplate.hpp"
#include "tmpl/MemberProcessorTemplate.hpp"
#include <context/GlobalSymbolContext.hpp>


namespace LynxAst {
    
    using namespace Cloneable;
    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxConstants;

    void InterfaceDeclarationNode::processMembers(std::unique_ptr<std::vector<std::unique_ptr<Node>>> members) {
        Members::Processor<InterfaceDeclarationNode>::process(this, std::move(members), methods, fields);  
    }

    llvm::Value* InterfaceDeclarationNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked ... {}", interfaceName);

        auto* module = astContext->getModule();
        auto symbolTable = astContext->getGlobalContext();

        auto ifaceType = std::make_unique<InterfaceType>(astContext.get(), interfaceName);
        buildInterfaceType(*astContext, *ifaceType);

        llvmStructType = llvm::cast<llvm::StructType>(ifaceType->getLLVMType());
        llvmPointerType = llvm::cast<llvm::PointerType>(ifaceType->getLLVMPointerType());

        std::string qualifiedIfaceName = getQualifiedName();
        auto vtable = std::make_shared<VirtualTable>(qualifiedIfaceName);

        emitBaseInterfaces(*astContext, *symbolTable, *vtable);

        emitMethods(*astContext, *ifaceType, *vtable);

        emitVTableGlobal(*astContext, std::move(ifaceType), *vtable);

        symbolTable->registerVTable(qualifiedIfaceName, vtable);

        return nullptr;
    }

    void InterfaceDeclarationNode::emitBaseInterfaces(const AstContext& astContext, const GlobalSymbolContext& symbol, VirtualTable& vtable) {
        if (!hasParentInterfaces()) return;

        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();
        for (const std::string& baseName : inheritIfaces) {
            auto baseMangled = Mangle::get(ManglerKind::CLASS, baseName);
            auto parentVtable = symbol.findVTable(baseMangled);
            if(!parentVtable) continue;

            vtable.inheritFrom(parentVtable);
            auto baseType = astContext.findType(baseName);
            const auto* ifaceType = TypeCasting::castType<const InterfaceType>(baseType.get());
            for(const auto& [mangledName, method] : ifaceType->getMethods()) {
                auto* dispatchMethod = module->getFunction(mangledName);
                if(!dispatchMethod) {
                    dispatchMethod = LLVMFunction::ensureFunctionDeclared(module, context, method.get(), mangledName);
                }
                std::string sig = parentVtable->getSignature(mangledName);
                vtable.setSignature(mangledName, sig);
                vtable.addMethod(mangledName, dispatchMethod);
            }    
        }
    }

    void InterfaceDeclarationNode::emitMethods(const AstContext& astContext, const InterfaceType& ifaceType, VirtualTable& vtable) {
        for (const auto& method : *methods) {
            auto* llvmFunction = generateMemberFunction(astContext, ifaceType, method.get());
            const auto& mangledFnName = llvmFunction->getName().str();
            std::string sig = method->getSignatureString();
            std::cout << "mangled fnName ============>" << sig << std::endl;
            vtable.setSignature(mangledFnName, sig);
            vtable.addMethod(mangledFnName, llvmFunction);
            attachFunctionSignature(llvmFunction, sig); 
        }
    }

    void InterfaceDeclarationNode::buildInterfaceType(const AstContext& astContext, InterfaceType& ifaceType) {

        // Process base interfaces (multi-inheritance)
        if (hasParentInterfaces()) {
            for (const std::string& baseName : inheritIfaces) {
                auto baseType = astContext.findType(baseName);
                const auto* baseInterface = TypeCasting::castType<const InterfaceType>(baseType.get());
                ifaceType.addParentInterface(baseInterface);
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
            ifaceType.addField(field->getVariableName(), std::move(fieldType));        
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
            
            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, interfaceName, methodName, mangleParams);
            ifaceType.addMethod(mangledName, std::move(methodType));
        }
    }

    void InterfaceDeclarationNode::emitVTableGlobal(AstContext& astContext, std::unique_ptr<InterfaceType> ifaceType, VirtualTable& vtable) {
        using namespace Vtable;
        auto& context = astContext.getLLVMContext();
        auto* module = astContext.getModule();
        std::string qualifiedClassName = getQualifiedName();
        
        // error begining here
        auto vType = emitVTable(module, context, qualifiedClassName, InterfaceAdapter{ifaceType.get()}, vtable);
        ifaceType->buildVTable(vType);
        astContext.registerCustomType(interfaceName, std::move(ifaceType));
    }

    void InterfaceDeclarationNode::attachFunctionSignature(llvm::Function* func, const std::string& signature) {
        func->addFnAttr("virtual", "true");
        llvm::LLVMContext& ctx = func->getContext();
        llvm::MDString* sigMD = llvm::MDString::get(ctx, signature);
        llvm::MDNode* node = llvm::MDNode::get(ctx, sigMD);
        func->setMetadata("lynx.signature", node);
    }
    
    llvm::Function* InterfaceDeclarationNode::generateMemberFunction(const AstContext& astContext, const InterfaceType& ifaceType, FunctionNode* fnNode) {
        auto symbol = astContext.getGlobalContext();
        symbol->addFunction(fnNode);

        const auto& funcName = fnNode->getFunctionName();
        std::vector<BaseType*> mangleParams;
        const auto& funcParams = *fnNode->getFunctionParameter();
        for (const auto& param : funcParams) {
            auto pType = TypeConv::convertToBaseRawType(astContext, *param->type);
            mangleParams.push_back(pType);
        }

        auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, interfaceName, funcName, mangleParams);
        auto llvmFunction = LLVMFunction::create(astContext, *fnNode, mangledName, llvmPointerType);

        auto* module = astContext.getModule();
        llvmFunction->setComdat(module->getOrInsertComdat(llvmFunction->getName()));
        llvmFunction->setDSOLocal(true);
        llvmFunction->addFnAttr("virtual", "true");

        symbol->removeFunction();
        symbol->clearLocalVariables();

        return llvmFunction;
    }

    std::unique_ptr<Node> InterfaceDeclarationNode::clone() const {
        auto clonedMethods = cloneNodeVector(methods);
        auto clonedFields = cloneNodeVector(fields);

        auto cloned = std::make_unique<InterfaceDeclarationNode>(interfaceName);
        cloned->inheritIfaces = inheritIfaces;
        cloned->methods = std::move(clonedMethods);
        cloned->fields = std::move(clonedFields);
        return cloned;
    }
}
