#include <logger/Logger.hpp>
#include "llvm/IR/Verifier.h"
#include "ClazzDeclarationNode.hpp"
#include <context/VirtualTable.hpp>
#include "utils/StringUtils.hpp"
#include "utils/MetadataUtils.hpp"
#include <types/tmpl/TypeCaster.hpp>
#include "tmpl/ManglerTemplate.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "utils/LLVMFunctionUtils.hpp"
#include <constants/LinkageType.hpp>
#include "tmpl/VTableTemplate.hpp"
#include "tmpl/ImportSymbolTemplate.hpp"
#include "tmpl/TypeConventionTemplate.hpp"
#include "InterfaceDeclarationNode.hpp"
#include "tmpl/MemberProcessorTemplate.hpp"


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxConstants;
    using namespace LLVMFunction;

    void ClazzDeclarationNode::generateDefaultCtor() {
        methods = std::make_unique<std::vector<std::unique_ptr<FunctionNode>>>();
        fields = std::make_unique<std::vector<std::unique_ptr<VariableDeclarationNode>>>();
        constructors = std::make_unique<std::vector<std::unique_ptr<ClazzConstructorNode>>>();
        auto defaultCtor = std::make_unique<ClazzConstructorNode>(
            AccessModifierType::PUBLIC, 
            className, 
            std::make_shared<std::vector<std::shared_ptr<Parameter>>>(), 
            std::make_unique<std::vector<std::unique_ptr<Node>>>()
        );
        constructors->push_back(std::move(defaultCtor));  
    }

    void ClazzDeclarationNode::processMembers(std::unique_ptr<std::vector<std::unique_ptr<Node>>> members) {
        Members::Processor<ClazzDeclarationNode>::process(this, std::move(members), methods, fields, constructors);
    }
    
    llvm::Value* ClazzDeclarationNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked ... {}", className);
        
        auto clazzType = std::make_unique<ClassType>(astContext.get(), className);

        buildClassType(*astContext, *clazzType);

        llvmStructType = llvm::cast<llvm::StructType>(clazzType->getLLVMType());
        llvmPointerType = llvm::cast<llvm::PointerType>(clazzType->getLLVMPointerType());
        astContext->registerCustomType(className, std::move(clazzType));

        auto symbol = astContext->getGlobalContext();
        auto vtable = emitBaseClass(*astContext, *symbol);

        emitBaseInterfaces(*astContext, *symbol, *vtable);

        emitComposedMixins(*astContext);

        emitFields(*astContext);

        emitConstructors(*astContext);

        emitMethods(*astContext, *vtable);
      
        emitVTableGlobal(*astContext, *vtable);
        
        std::string vName = getQualifiedClassName();
        symbol->registerVTable(vName, vtable);

        return nullptr;
    }

    void ClazzDeclarationNode::emitComposedMixins(const AstContext& astContext) {
        if (!hasMixins()) return;
        auto clazzType = TypeCasting::castType<ClassType>(astContext.findType(className).get());
        auto* classStructType = llvm::cast<llvm::StructType>(clazzType->getLLVMType());
        auto* classPointerType = llvm::PointerType::getUnqual(classStructType);

        for(auto[mixinOffset, mixinName, methodMap] : clazzType->getAllMixinMethods()) {
            std::cout << "Mixin Offset ====>" << mixinOffset << std::endl;
            for(const auto& [mangledName, method] : methodMap) {
                emitMixinMethods(astContext, mangledName, mixinName, method, mixinOffset, classStructType, classPointerType);
            }
        }
    }
    
    void ClazzDeclarationNode::emitMixinMethods(
        const AstContext& astContext,
        const std::string& mangledName,
        const std::string& mixinName,
        const MethodType* method,
        unsigned int mixinOffset,
        llvm::StructType* classStructType,
        llvm::PointerType* classPointerType
    ) {
        auto* module = astContext.getModule();
        auto& builder = astContext.getBuilder();
        auto& context = astContext.getLLVMContext();
    
        // Ensure original mixin function exists
        llvm::Function* mixinFunc = module->getFunction(mangledName);
        if (!mixinFunc) {
            mixinFunc = ensureFunctionDeclared(module, context, method, mangledName);
        }
    
        // Build wrapper function type (%class.Zoo* + other args)
        std::vector<llvm::Type*> paramTypes;
        paramTypes.push_back(classPointerType); // first arg = Zoo*
    
        auto* mixinFnType = mixinFunc->getFunctionType();
        for (unsigned i = 1; i < mixinFnType->getNumParams(); ++i) {
            paramTypes.push_back(mixinFnType->getParamType(i));
        }
    
        auto* returnType = method->getReturnType()->getLLVMType();
        auto* fnType = llvm::FunctionType::get(returnType, paramTypes, false);
    
        auto wrapperName = Mangle::get(ManglerKind::MEMBER_FUNCTION, className, method->getName(), method->getParameterRawTypes());
    
        auto* classFunc = llvm::Function::Create(fnType, linkOnceODRType, wrapperName, module);
        classFunc->setDSOLocal(true);
        classFunc->setComdat(module->getOrInsertComdat(classFunc->getName()));
        classFunc->addFnAttr(llvm::Attribute::NoUnwind);
        classFunc->addFnAttr(llvm::Attribute::UWTable);
        classFunc->addFnAttr(llvm::Attribute::NoInline); // optional
        classFunc->addParamAttr(0, llvm::Attribute::NonNull);
        classFunc->addParamAttr(0, llvm::Attribute::NoAlias);

        // Entry block
        auto* entry = LLVMFunction::createEntryBlock(context, classFunc);
        builder.SetInsertPoint(entry);
    
        // First arg = Zoo* this
        auto argIt = classFunc->arg_begin();
        llvm::Value* thisPtr = &*argIt;
    
        // GEP to the last mixin that defines this method
        auto* mixinPtr = builder.CreateStructGEP(classStructType, thisPtr, mixinOffset, mixinName + "_ptr");

        // If the mixin function expects a different pointer type, cast it
        llvm::Value* targetPtr = mixinPtr;
        auto* expectedPtrType = llvm::cast<llvm::PointerType>(mixinFunc->getFunctionType()->getParamType(0));
        if (targetPtr->getType() != expectedPtrType) {
            targetPtr = builder.CreateBitCast(targetPtr, expectedPtrType, mixinName + "_cast");
        }

        // Forward remaining args
        std::vector<llvm::Value*> args = { targetPtr };
        for (++argIt; argIt != classFunc->arg_end(); ++argIt) {
            args.push_back(&*argIt);
        }
    
        builder.CreateCall(mixinFunc, args);
    
        finalizeFunction(classFunc, module, returnType, builder);
    }
    
    std::shared_ptr<VirtualTable> ClazzDeclarationNode::emitBaseClass(const AstContext& astContext, const GlobalSymbolContext& symbol) {
        LOG_INFO("Invoked ........{}", getQualifiedClassName());

        std::string vName = getQualifiedClassName();
        std::shared_ptr<VirtualTable> vtable;
        if(hasBaseClass()) {
            auto* module = astContext.getModule();
            auto& context = astContext.getLLVMContext();
            const auto& baseClassName = inheritClass->baseClass.value();
            std::string baseMangled = Mangle::get(ManglerKind::CLASS, baseClassName);
            auto parentVtable = symbol.findVTable(baseMangled);
            if (parentVtable) {
                vtable = std::make_shared<VirtualTable>(vName, parentVtable);
                auto baseType = astContext.findType(baseClassName);
                const auto* clazzType = TypeCasting::castType<const ClassType>(baseType.get());
                for (const auto& [mangledName, fn] : parentVtable->getMethodTable()) {
                    auto* dispatchMethod = module->getFunction(mangledName);
                    if(!dispatchMethod) {
                        const auto& method = clazzType->getMethod(mangledName);
                        dispatchMethod = LLVMFunction::ensureFunctionDeclared(module, context, method, mangledName);
                    }
                    std::string sig = parentVtable->getSignature(mangledName);
                    vtable->setSignature(mangledName, sig);
                    vtable->addMethod(mangledName, dispatchMethod);
                }    
            } else {
                LOG_WARN("Parent vtable '{}' not found while generating '{}'; creating empty vtable and continuing.", baseMangled, vName);
                vtable = std::make_shared<VirtualTable>(vName);
            }
        } else {
            vtable = std::make_shared<VirtualTable>(vName);
        }

        return vtable;
    }

    void ClazzDeclarationNode::emitBaseInterfaces(const AstContext& astContext, const GlobalSymbolContext& symbol, VirtualTable& vtable) {
        LOG_INFO("Invoked ........{}", className);

        if (!hasInterfaces()) return;
        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();   
        for (const std::string& baseName : inheritClass->interfaces) {
            std::string baseMangled = Mangle::get(ManglerKind::CLASS, baseName);
            auto parentVtable = symbol.findVTable(baseMangled);
            if (!parentVtable) {
                LOG_WARN("Base interface '{}' not found (mangled: {}).", baseName, baseMangled);
                continue;
            }

            auto baseType = astContext.findType(baseName);
            const auto* ifaceType = TypeCasting::castType<const InterfaceType>(baseType.get());
            for (const auto& [ifaceMangled, methodPtr] : ifaceType->getMethods()) {
                auto* dispatchMethod = module->getFunction(ifaceMangled);
                if(!dispatchMethod) {
                    const auto& method = ifaceType->getMethod(ifaceMangled);
                    dispatchMethod = LLVMFunction::ensureFunctionDeclared(module, context, method, ifaceMangled);
                }
                std::string sig = parentVtable->getSignature(ifaceMangled);
                vtable.setSignature(ifaceMangled, sig);
                vtable.addMethod(ifaceMangled, dispatchMethod);
            }
        }
    }
    
    void ClazzDeclarationNode::emitFields(const AstContext& astContext) {
        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();

        std::vector<llvm::Metadata*> metadatas;
        for(const auto& field : *fields) {
            auto* clsName = llvm::MDString::get(context, className);
            auto* modifier = llvm::MDString::get(context, parseModifierTypeToString(field->getAccessModifier()));
            auto* dataType = llvm::MDString::get(context, field->getVariableType()->name);
            auto* name = llvm::MDString::get(context, field->getVariableName());
            metadatas.push_back(llvm::MDNode::get(context, { clsName, modifier, dataType, name }));
        }
        auto* metadataNodes = llvm::MDNode::get(context, metadatas);
        auto* namedMethodsNode = module->getOrInsertNamedMetadata("lynx.fields");
        namedMethodsNode->addOperand(metadataNodes);
    }

    void ClazzDeclarationNode::emitConstructors(const AstContext& astContext) {
        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();

        std::vector<llvm::Metadata*> metadatas;
        for (auto& constructorNode : *constructors) {
            auto* ctorValue = constructorNode->generateCode(astContext.createContext());
            if(auto* ctorFn = llvm::dyn_cast<llvm::Function>(ctorValue)) {

                ctorFn->setComdat(module->getOrInsertComdat(ctorFn->getName()));
                ctorFn->setDSOLocal(true);
        
                ctorFn->addFnAttr(llvm::Attribute::NoUnwind);
                ctorFn->addFnAttr(llvm::Attribute::UWTable);
                ctorFn->addFnAttr(llvm::Attribute::NoInline); // optional
                
                ctorFn->addParamAttr(0, llvm::Attribute::NonNull);
                ctorFn->addParamAttr(0, llvm::Attribute::NoAlias);

                auto* ctorName = llvm::MDString::get(context, className);
                auto* modifier = llvm::MDString::get(context, "public");
                auto* tag = llvm::MDString::get(context, "ctor");
                metadatas.push_back(llvm::MDNode::get(context, { ctorName, modifier, tag }));
            }
        }

        auto* metadataNodes = llvm::MDNode::get(context, metadatas);
        auto* namedMethodsNode = module->getOrInsertNamedMetadata("lynx.methods");
        namedMethodsNode->addOperand(metadataNodes);
    }
    
    void ClazzDeclarationNode::emitMethods(const AstContext& astContext, VirtualTable& vtable) {

        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();

        auto baseType = astContext.findType(className);
        auto clazzType = TypeCasting::castType<ClassType>(baseType.get());

        std::vector<llvm::Metadata*> metadatas;

        if (clazzType->hasBaseClass()) {
            auto baseVTable = astContext.getGlobalContext()->findVTable(clazzType->getBaseClass()->qualifiedName());
            vtable.inheritFrom(baseVTable);
        }

        for (const auto& iface : clazzType->getAllInterfaces()) {
            auto ifaceVTable = astContext.getGlobalContext()->findVTable(iface->qualifiedName());
            vtable.inheritFrom(ifaceVTable);
        }
    
        for (const auto& mixin : clazzType->getAllMixins()) {
            auto mixinVTable = astContext.getGlobalContext()->findVTable(mixin->qualifiedName());
            vtable.inheritFrom(mixinVTable);
        }
    
        for (const auto& method : *methods) {
            auto* llvmFunc = emitMemberFunction(astContext, *clazzType, method.get());

            const std::string& accessType = parseModifierTypeToString(method->getAccessModifier());
            const std::string& mangledFnName = llvmFunc->getName().str();
            const std::string& sig = method->getSignatureString();
            int slot = vtable.getMethodIndexBySignature(sig);

            if (method->isVirtual || method->isOverride) {
                if (slot >= 0) {
                    // Override existing slot
                    std::string prevMangled = vtable.getSlotOrder()[slot];
                    vtable.addMethod(prevMangled, llvmFunc);
                } else {
                    // New virtual method
                    vtable.setSignature(mangledFnName, sig);
                    vtable.addMethod(mangledFnName, llvmFunc);
                }
    
                // Mark LLVM attributes
                if (method->isOverride) llvmFunc->addFnAttr("override", "true");
                llvmFunc->addFnAttr("virtual", "true");
                llvmFunc->addFnAttr("access_modifier", accessType);
            } else {
                // Non-virtual
                llvmFunc->addFnAttr("access_modifier", accessType);
            }

            auto* clsName = llvm::MDString::get(context, className);
            auto* modifier = llvm::MDString::get(context, accessType);
            auto* retType = llvm::MDString::get(context, method->getReturnType()->name);
            auto* modName = llvm::MDString::get(context, mangledFnName);
            auto* signature = llvm::MDString::get(context, sig);
            metadatas.push_back(llvm::MDNode::get(context, { clsName, modifier, retType, modName, signature }));
        }

        auto* metadataNodes = llvm::MDNode::get(context, metadatas);
        auto* namedMethodsNode = module->getOrInsertNamedMetadata("lynx.methods");
        namedMethodsNode->addOperand(metadataNodes);
    }

    void ClazzDeclarationNode::buildClassType(const AstContext& astContext, ClassType& classType) {

        if(hasBaseClass()) {
            const auto& baseClassName = inheritClass->baseClass.value();
            auto baseType = astContext.findType(baseClassName);
            auto* parentClassType = dynamic_cast<ClassType*>(baseType.get());
            if(!parentClassType) throw std::runtime_error("Base " + baseClassName + " is not a class.");
            classType.setBaseClass(parentClassType);
        }

        if(hasInterfaces()) {
            for (const std::string& ifaceName : inheritClass->interfaces) {
                auto baseType = astContext.findType(ifaceName);
                if (!baseType) throw std::runtime_error("Base " + ifaceName + " is not an interface.");
                classType.addInterface(TypeCasting::cloneAs<InterfaceType>(baseType.get())); 
            }
        }

        if(hasMixins()) {
            for (const std::string& mixinName : inheritClass->mixins) {
                auto baseType = astContext.findType(mixinName);
                if (!baseType) throw std::runtime_error("Base " + mixinName + " is not an interface.");
                classType.addMixin(TypeCasting::cloneAs<MixinType>(baseType.get())); 
            }
        }

        unsigned fieldIdx = 0;
        for (const auto& field : *fields) {
            std::string methodName = field->getVariableName();
            auto vType = TypeConv::convertToBaseType(astContext, *field->getVariableType());
            auto fieldType = std::make_unique<FieldType>(
                field->getAccessModifier(),
                methodName,
                std::move(vType),
                fieldIdx++
            );
        
            classType.addField(field->getVariableName(), std::move(fieldType));            
        }

        unsigned methodIdx = 0;
        for (const auto& method : *methods) {
            std::vector<std::unique_ptr<BaseType>> parameterTypes;
            std::vector<BaseType*> mangleParams;
            const auto& funcParams = *method->getFunctionParameter();
            for (const auto& param : funcParams) {
                auto pType = TypeConv::convertToBaseType(astContext, *param->type);
                mangleParams.push_back(pType.get());
                parameterTypes.push_back(std::move(pType));
            }
            
            std::string methodName = method->getFunctionName();
            auto returnType = TypeConv::convertToBaseType(astContext, *method->getReturnType());
            auto methodType = std::make_unique<MethodType>(
                method->getAccessModifier(),
                methodName, 
                std::move(returnType), 
                std::move(parameterTypes), 
                methodIdx++,
                false, 
                false, 
                method->isVirtual || method->isOverride
            );
            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, className, methodName, mangleParams);
            classType.addMethod(mangledName, std::move(methodType));
        }

        for(const auto& ctor : *constructors) {
            std::vector<BaseType*> signatureTypes;
            std::vector<std::unique_ptr<BaseType>> parameterTypes;
            for (const auto& param : *ctor->getParameters()) {
                auto pType = TypeConv::convertToBaseType(astContext, *param->type);
                signatureTypes.push_back(pType.get());
                parameterTypes.push_back(std::move(pType));  
            }

            auto mangledName = ctor->getQualifiedName(astContext);
            auto constructor = std::make_unique<ConstructorType>(mangledName, std::move(parameterTypes));
            classType.addConstructor(mangledName, std::move(constructor));
        }

    }

    void ClazzDeclarationNode::emitVTableGlobal(const AstContext& astContext, VirtualTable& vtable) {
        using namespace Vtable;
        auto& context = astContext.getLLVMContext();
        auto* module = astContext.getModule();
        std::string qualifiedClassName = getQualifiedClassName();
    
        auto baseType = astContext.findType(className);
        auto* classType = TypeCasting::castType<ClassType>(baseType.get());
    
        VTableType vType;
        if (classType->hasBaseClass()) {
            vType = emitVTable(module, context, qualifiedClassName, ClassAdapter{classType}, vtable);
        }
    
        if (classType->hasInterfaces()) {
            for (const auto& iface : classType->getAllInterfaces()) {
                vType = emitVTable(module, context, qualifiedClassName, InterfaceAdapter{iface.get()}, vtable);
            }
        }

        classType->buildVTable(vType);
    }

    llvm::Function* ClazzDeclarationNode::emitMemberFunction(const AstContext& astContext, const ClassType& clazzType, FunctionNode* fnNode) {
        auto symbol = astContext.getGlobalContext();
        symbol->addFunction(fnNode);

        const auto& funcName = fnNode->getFunctionName();
        std::vector<BaseType*> mangleParams;
        const auto& funcParams = *fnNode->getFunctionParameter();
        for (const auto& param : funcParams) {
            auto pType = TypeConv::convertToBaseRawType(astContext, *param->type);
            mangleParams.push_back(pType);
        }

        auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, className, funcName, mangleParams);
        auto llvmFunction = LLVMFunction::create(astContext, *fnNode, mangledName, llvmPointerType);

        auto* module = astContext.getModule();
        llvmFunction->setComdat(module->getOrInsertComdat(llvmFunction->getName()));
        llvmFunction->setDSOLocal(true);

        symbol->removeFunction();
        symbol->clearLocalVariables();
        
        return llvmFunction;
    }

    std::unique_ptr<Node> ClazzDeclarationNode::clone() const {
        auto clonedConstructors = Cloneable::cloneNodeVector(constructors);
        auto clonedMethods = Cloneable::cloneNodeVector(methods);
        auto clonedFields = Cloneable::cloneNodeVector(fields);
    
        // Create cloned node
        auto cloned = std::make_unique<ClazzDeclarationNode>(className);
        if (inheritClass)  cloned->inheritClass = std::make_unique<ClassInheritanceType>(*inheritClass);
        cloned->constructors = std::move(clonedConstructors);
        cloned->methods = std::move(clonedMethods);
        cloned->fields = std::move(clonedFields);
        return cloned;
    }
} 





// void ClazzDeclarationNode::detectCycleMixin() {
//     if (!hasMixins()) return;

    // // --- Step 1: Linearize mixins (DFS)
    // std::vector<MixinType*> mixinLinearization;
    // std::unordered_set<std::string> visited;

    // std::function<void(MixinType*)> dfs;
    // dfs = [&](MixinType* mixin) {
    //     if (!visited.insert(mixin->originalName()).second) return;
    //     for (auto* parent : mixin->getParents()) {
    //         dfs(const_cast<MixinType*>(parent));  // safe if you know parent is mutable
    //     }
    //     mixinLinearization.push_back(mixin);
    // };

    // for (const auto& mixinName : inheritClass->mixins) {
    //     auto mixinBase = astContext.findType(mixinName);
    //     if (!mixinBase) continue;
    //     dfs(TypeCasting::castType<MixinType>(mixinBase.get())); // safe extraction from unique_ptr
    // }

    // // --- Step 2: Flatten methods for override resolution
    // std::unordered_map<std::string, const MixinType*> lastOwner;
    // std::unordered_map<std::string, const MethodType*> lastMethod;

    // for (auto* mixin : mixinLinearization) {
    //     mixin->flattenMethodsAndFields(); // ensure flattenedMethods exist
    //     for (const auto& methodName : mixin->getFlattenedMethodOrder()) {
    //         const MethodType* method = mixin->getFlattenedMethod(methodName);
    //         lastOwner[methodName] = mixin;
    //         lastMethod[methodName] = method;
    //     }
    // }

    // // --- Step 3: Generate wrapper functions in class
    // for (const auto& [mangledName, mixinType] : lastOwner) {
    //     const MethodType* method = lastMethod[mangledName];
    //     emitMixinMethods(astContext, mangledName, *mixinType, method);
    // }
// }