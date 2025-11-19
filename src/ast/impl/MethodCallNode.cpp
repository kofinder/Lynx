#include "MethodCallNode.hpp"
#include <logger/Logger.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Instructions.h>
#include "utils/PACCallerUtils.hpp"
#include <types/tmpl/TypeCaster.hpp>
#include "tmpl/CloneNodeTemplate.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include "utils/LLVMFunctionUtils.hpp"
#include <types/userdefined/ClassType.hpp>
#include <context/GlobalSymbolContext.hpp>
#include <libruntime/RuntimeClassRegistry.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxTypes;
    using namespace LynxLibRuntime;
    using namespace Cloneable;

    template <typename T>
    inline constexpr bool always_false = false;

    llvm::Value* MethodCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Executed ....");

        auto symbol = astContext->getGlobalContext();

        auto [funcName, argTypes, argVaules] = extractMethodCall(*astContext);

        symbol->addMethodCall(new MethodCall(funcName, argTypes));

        auto [objectValue, baseType, isUserDefined] = classifyMethodTarget(*astContext);

        llvm::Value* result = nullptr;
        if (isUserDefined) {
            result = dispatchUserDefinedMethod(*astContext, *baseType, objectValue, funcName, argTypes, argVaules);
        } else {
            result = dispatchBuiltinOrLibraryMethod(astContext, *baseType, objectValue);
        }

        symbol->removeMethodCall();

        return result;
    }
    
    llvm::Value* MethodCallNode::dispatchUserDefinedMethod(
        const AstContext& astContext, 
        const BaseType& baseType, 
        llvm::Value* objectValue,
        std::string funcName,
        std::vector<llvm::Type*> argTypes,
        std::vector<llvm::Value*> argValues
    ) {
        LOG_INFO("Emited ......");
        auto& builder = astContext.getBuilder();

        // LLVM 21-safe: check for pointer to pointer and load if necessary
        if (auto* ptrType = llvm::dyn_cast<llvm::PointerType>(objectValue->getType())) {
            objectValue = builder.CreateLoad(objectValue->getType(), objectValue, "load_deref");
        }


        // if (objectValue->getType()->isPointerTy() &&
        //     objectValue->getType()->getPointerElementType()->isPointerTy()) {
        //     objectValue = builder.CreateLoad(objectValue->getType()->getPointerElementType(), objectValue, "load_deref");
        // }

        argValues.insert(argValues.begin(), objectValue);

        auto dispatchForType = [&](auto* typePtr, const std::string& mangledName) -> llvm::Value* {
            using TypeT = std::remove_pointer_t<decltype(typePtr)>;
            auto& usrType = *typePtr;    
            if constexpr (std::is_base_of_v<ClassType, TypeT>) {

                MethodCallInfo<ClassType> info(astContext, usrType, objectValue, funcName, mangledName, argValues, argTypes);

                if (usrType.isVirtualFunction(funcName)) return dispatchMethod(info);
               
                if (auto* instr = llvm::dyn_cast<llvm::BitCastInst>(objectValue)) return invokeMethod(info);

                return createNullSafeCall(astContext, objectValue, [&]() -> llvm::Value* {
                    return invokeMethod(info);
                });

            } else if constexpr (std::is_base_of_v<InterfaceType, TypeT>) {
                MethodCallInfo<InterfaceType> info(astContext, usrType, objectValue, funcName, mangledName, argValues, argTypes);
                return dispatchMethod(info);
            } else if constexpr (std::is_base_of_v<MixinType, TypeT>) {
                MethodCallInfo<MixinType> info(astContext, usrType, objectValue, funcName, mangledName, argValues, argTypes);
                return invokeMethod(info);
            } else {
                static_assert(always_false<TypeT>, "Unsupported user-defined type in dispatch");
            }
        };

        if (auto* clazzType = TypeCasting::castType<ClassType>(baseType)) {
            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, clazzType->originalName(), funcName, argTypes);
            return dispatchForType(clazzType, mangledName);
        } else if (auto* ifaceType = TypeCasting::castType<InterfaceType>(baseType)) {
            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, ifaceType->originalName(), funcName, argTypes);
            return dispatchForType(ifaceType, mangledName);
        } else if(auto* mixinType = TypeCasting::castType<MixinType>(baseType)) {
            auto mangledName = Mangle::get(ManglerKind::MEMBER_FUNCTION, mixinType->originalName(), funcName, argTypes);
            return dispatchForType(mixinType, mangledName);
        }
    
        LOG_ERROR("Failed: Object type is not a user-defined type");
        throw std::runtime_error("Object type is not a user-defined type!");
    }


    template <typename Adapter>
    llvm::Value* MethodCallNode::dispatchMethod(const MethodCallInfo<Adapter>& methodInfo) const {
        LOG_INFO("Executed...");

        const auto& [astContext, usrDefinedType, instance, fnName, mangled, callArgs, argTypes] = methodInfo;
        auto* module = astContext.getModule();
        auto* methodFn = module->getFunction(mangled);
        if(!methodFn) {
            auto method = usrDefinedType.getMethod(mangled);
            auto* returnType = method->getReturnType()->getLLVMType();
            std::vector<llvm::Type*> localArgTypes = argTypes;
            localArgTypes.insert(localArgTypes.begin(), instance->getType());
            methodFn = LLVMFunction::ensureFunctionDeclared(module, returnType, localArgTypes, mangled);
        }

        auto* methodFnType = methodFn->getFunctionType();
        if (!methodFnType) {
            LOG_ERROR("[Error], Function '{}' has no valid function type!", mangled);
            throw std::runtime_error("Function has no valid function type!" + mangled);
        }    

        //Load vtable pointer from the object
        auto* vtablePtr = usrDefinedType.loadVTablePtr(instance);

        // Load method pointer from vtable
        auto* methodPtr = usrDefinedType.loadVirtualMethodPtr(vtablePtr, mangled);

        // Cast to the correct function type
        auto& builder = astContext.getBuilder();//  methodFnType->getPointerTo()
        auto* methodPtrCast = builder.CreateBitCast(methodPtr, llvm::PointerType::get(methodFnType->getContext(), 0), llvm::Twine(fnName + "_ptr_cast"));
        llvm::FunctionCallee callee(methodFnType, methodPtrCast);

        return builder.CreateCall(callee, callArgs);    
    }

    template <typename Adapter>
    llvm::Value* MethodCallNode::invokeMethod(const MethodCallInfo<Adapter>& methodInfo) const {
        LOG_INFO("Executed...");
        const auto& [astContext, usrDefinedType, instance, fnName, mangled, callArgs, argTypes] = methodInfo;
        auto mangledName = usrDefinedType.resolveMethodCall(MethodKind::METHOD, mangled, argTypes);
        auto* module = astContext.getModule();
        auto* methodFn = module->getFunction(mangledName);
        if(!methodFn) {
            auto method = usrDefinedType.getMethod(mangledName);
            auto* returnType = method->getReturnType()->getLLVMType();

            std::vector<llvm::Type*> localArgTypes = { instance->getType() };
            for(const auto& param: method->getParameterTypes()) {
                auto llvmType = TypeConv::convertToLLVMType(param.get());
                localArgTypes.push_back(llvmType);
            }
            methodFn = LLVMFunction::ensureFunctionDeclared(module, returnType, localArgTypes, mangledName);
        }

        auto* methodFnType = methodFn->getFunctionType();
        if (!methodFnType) {
            LOG_ERROR("Function '{}' has no valid function type!", mangledName);
            throw std::runtime_error("Function has no valid function type!" + mangledName);
        }    

        auto& builder = astContext.getBuilder();

        std::vector<llvm::Value*> localCallArgs;
        for (size_t i = 0; i < callArgs.size(); ++i) {
            llvm::Value* arg = callArgs[i];
            llvm::Type* actualType = arg->getType();
            llvm::Type* expectedType = methodFn->getFunctionType()->getParamType(i);

            if (actualType == expectedType) {
                localCallArgs.push_back(arg);
                continue;
            }

            if (actualType->isPointerTy() && expectedType->isPointerTy()) {
                arg = builder.CreateBitCast(arg, expectedType, "arg_cast");
                localCallArgs.push_back(arg);
                continue;
            }
            LOG_ERROR("Type mismatch in constructor call: wrong expected  ...... ");
            throw std::runtime_error("Type mismatch in constructor call: expected ");
        }

        return builder.CreateCall(methodFn, localCallArgs);
    }

    llvm::Value* MethodCallNode::dispatchBuiltinOrLibraryMethod(std::shared_ptr<AstContext> astContext, const BaseType& baseType, llvm::Value* objectValue) {
        LOG_ERROR("Library Call Method ..........");

        std::string typeName = "File";

        const auto& functionName = functionCallNode->getFunctionName();
        auto clazz = RuntimeClassRegistry::getInstance().resolveClass(typeName);
        if(!clazz) {
            LOG_ERROR("No resolve classe. {}", typeName);
            throw std::runtime_error(typeName + "No resolve classe");
        }

        auto argsPtr = functionCallNode->takeArguments();
        if (!argsPtr) {
            LOG_ERROR("No arguments present.");
            throw std::runtime_error(typeName + "No arguments present.");
        }
        
        auto method = clazz->getMethod(functionName);
        if (!method) {
            LOG_ERROR("Method '{}' not found in class.", functionName);
            throw std::runtime_error(functionName + "Method '{}' not found in class.");
        }
        
        return method->call(astContext, std::move(*argsPtr), objectValue);
    }

    llvm::Value* MethodCallNode::createNullSafeCall(const AstContext& astContext, llvm::Value* objectPtr, const std::function<llvm::Value*()>& generateCall) {

        std::cout << objectPtr->getName().str() << std::endl;

        auto& builder = astContext.getBuilder();
        auto& ctx = astContext.getLLVMContext();
        auto* parentFunc = builder.GetInsertBlock()->getParent();
    
        // --- Blocks ---
        auto* callBB = llvm::BasicBlock::Create(ctx, llvm::Twine(objectPtr->getName() + "_call"), parentFunc);
        auto* skipBB = llvm::BasicBlock::Create(ctx, llvm::Twine(objectPtr->getName() + "_skip"), parentFunc);
        auto* exitBB = llvm::BasicBlock::Create(ctx, llvm::Twine(objectPtr->getName() + "_exit"), parentFunc);
    
        // --- Null check ---
        auto* isNotNull = builder.CreateICmpNE(
            objectPtr,
            llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(objectPtr->getType())),
            llvm::Twine(objectPtr->getName() + "_isNotNull")
        );

        builder.CreateCondBr(isNotNull, callBB, skipBB);
    
        // --- Call block ---
        builder.SetInsertPoint(callBB);
        llvm::Value* callRes = generateCall();
        if (!callBB->getTerminator()) builder.CreateBr(exitBB);
    
        // --- Skip block ---
        builder.SetInsertPoint(skipBB);
        if (!skipBB->getTerminator()) builder.CreateBr(exitBB);
    
        // --- Continue block ---
        builder.SetInsertPoint(exitBB);

        if (!callRes || callRes->getType()->isVoidTy()) return exitBB;
        
        // --- Non-void PHI node ---
        llvm::Type* resTy = callRes->getType();
        llvm::PHINode* phi = builder.CreatePHI(resTy, 2, objectPtr->getName() + "_phi");

        llvm::Value* skipVal = nullptr;
        if (resTy->isPointerTy()) {
            skipVal = llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(resTy));
        } else if (resTy->isIntegerTy()) {
            skipVal = llvm::ConstantInt::get(resTy, 0);
        } else if (resTy->isFloatingPointTy()) {
            skipVal = llvm::ConstantFP::get(resTy, 0.0);
        } else {
            skipVal = llvm::UndefValue::get(resTy);
        }

        phi->addIncoming(callRes, callBB);
        phi->addIncoming(skipVal, skipBB);

        return phi;        
    }

    std::tuple<llvm::Value*, BaseType*, bool> MethodCallNode::classifyMethodTarget(const AstContext& astContext) const {
        auto objectValue = objectTargetNode->generateCode(astContext.createContext());
        if (!objectValue) {
            LOG_ERROR("Object target code generation failed!.");
            throw std::runtime_error("Object target code generation failed");
        }

        auto baseType = astContext.findType(objectValue);
        if (!baseType) {
            LOG_ERROR("Type name is empty, cannot resolve method.");
            throw std::runtime_error("Type name is empty, cannot resolve method");
        }

        switch (baseType->getTypeTag()) {
            case DataType::FILE:
            case DataType::ARRAY:
            case DataType::DATETIME: return std::make_tuple(objectValue, baseType.get(), false);
            default: break;
        }
    
        return std::make_tuple(objectValue, baseType.get(), true);
    }


    std::tuple<std::string, std::vector<llvm::Type*>, std::vector<llvm::Value*>> MethodCallNode::extractMethodCall(const AstContext& astContext) const {
        auto fnName = functionCallNode->getFunctionName();
        const auto& arguments = functionCallNode->getArguments();
        std::vector<llvm::Value*> argValues;
        std::vector<llvm::Type*> argTypes;
        for (auto& arg : arguments) {
            auto* argVal = arg->generateCode(astContext.createContext());
            argValues.push_back(argVal);
            argTypes.push_back(argVal->getType());
        }
        return std::make_tuple(fnName, argTypes, argValues);
    }

    std::unique_ptr<Node> MethodCallNode::clone() const {
        auto fnNode = cloneNode(functionCallNode);
        auto objNode = cloneNode(objectTargetNode);
        auto cloneNode = std::make_unique<MethodCallNode>(std::move(fnNode), std::move(objNode));
        return cloneNode;
    }   
}


