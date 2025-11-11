#include <tuple>

#include "LiteralNode.hpp"
#include <logger/Logger.hpp>
#include "ObjectCreationNode.hpp"
#include <context/GlobalSymbolContext.hpp>
#include <types/userdefined/FileType.hpp>
#include "utils/DatetimeUtils.hpp"
#include "utils/StringUtils.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include "tmpl/ImportSymbolTemplate.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "tmpl/TypeConventionTemplate.hpp"
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {

    using namespace LynxTypes;
    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace Cloneable;

    llvm::Value* ObjectCreationNode::generateCode(std::shared_ptr<AstContext> astContext)  {
        LOG_WARN("IR Code Generation ...", variableType->name);

        auto callableInfo = resolveCallableLLVMInfo(*astContext);

        return generateWithGC(*astContext, callableInfo);
    }

    llvm::Value* ObjectCreationNode::generateWithGC(const AstContext& astContext, const CallableInfo& callableInfo) {

        auto [argTypes, argValues, objectType, baseType] = callableInfo;

        auto variableName = StringUtils::transformLowerCase(variableType->name);

        std::cout << "Variable Name ===>" << variableName << std::endl;

        auto& mutableContext = const_cast<AstContext&>(astContext);
        auto* newInstance = mutableContext.emitGCAllocCall(objectType, variableName);
        
       emitConstructorCall(astContext, newInstance, callableInfo);

        return newInstance;
    }

    void ObjectCreationNode::emitConstructorCall(const AstContext& astContext, llvm::Value* newInstance, const CallableInfo& callableInfo) {
        auto [argTypes, argValues, objectType, baseType] = callableInfo;

        auto clazzType = TypeCasting::castType<ClassType>(baseType);
        if(!clazzType) {
            LOG_ERROR("{} Class not found!", variableType->name);
            throw std::runtime_error("Constructor function not found for: " + variableType->name);
        }

        auto const& exactMangled = Mangle::get(ManglerKind::CONSTRUCTOR, variableType->name, argTypes);
        auto mangledName = clazzType->resolveMethodCall(MethodKind::CONSTRUCTOR, exactMangled, argTypes);

        std::vector<llvm::Type*> arguments = argTypes;
        arguments.insert(arguments.begin(), objectType->getPointerTo());

        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();
        auto* ctorType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), arguments, false);
        auto callee = module->getOrInsertFunction(mangledName, ctorType);
    
        llvm::Function* ctorFn = nullptr;
        if (auto* fn = llvm::dyn_cast<llvm::Function>(callee.getCallee())) {
            ctorFn = fn;
        } else {
            ctorFn = llvm::cast<llvm::Function>(callee.getCallee()->stripPointerCasts());
        }
    
        if (!ctorFn) {
            LOG_ERROR("Constructor function not found for ...... {}", mangledName);
            throw std::runtime_error("Constructor function not found for: " + mangledName);
        }
    
        auto& builder = astContext.getBuilder();
        std::vector<llvm::Value*> ctorArgs = { newInstance };
        for (size_t i = 0; i < argValues.size(); ++i) {
            llvm::Value* arg = argValues[i];
            llvm::Type* expectedType = ctorFn->getFunctionType()->getParamType(i + 1); // +1 because [0] is "this"
            llvm::Type* actualType = arg->getType();
    
            if (actualType == expectedType) {
                ctorArgs.push_back(arg); // exact match
                continue;
            }
            
            // Pointer mismatch (class vs interface) → bitcast
            if (actualType->isPointerTy() && expectedType->isPointerTy()) {
                arg = builder.CreateBitCast(arg, expectedType, "arg_cast");
                ctorArgs.push_back(arg);
                continue;
            }

            LOG_ERROR("Type mismatch in constructor call: wrong expected  ...... ");
            throw std::runtime_error("Type mismatch in constructor call: expected ");
        } 

        // 4. Emit final call
        builder.CreateCall(ctorFn, ctorArgs);
    }
    
    CallableInfo ObjectCreationNode::resolveCallableLLVMInfo(const AstContext& astContext) {
        std::vector<llvm::Value*> ctorArgs;
        std::vector<llvm::Type*> ctorArgTypes;

        auto [objectType, baseType] = convertToLLVMType(astContext);
        
        if(!objectType) {
            LOG_ERROR("Failed to resolve base type for constructor");
            throw std::runtime_error("Failed to resolve base type for constructor");
        }

        if (variableType->type == DataType::DATETIME) {
            std::pair<std::vector<llvm::Value*>, std::vector<llvm::Type*>> datetimeData;
            if (!arguments || arguments->empty()) {
                datetimeData = DateTimeUtils::generateCurrentDateTime(astContext.getLLVMContext());
            } else {
                datetimeData = DateTimeUtils::generateDateTime(astContext, *arguments);
            }
            ctorArgs = std::move(datetimeData.first);
            ctorArgTypes = std::move(datetimeData.second);
        } 

        if (arguments) {
            auto& builder = astContext.getBuilder();
            for (auto& arg : *arguments) {

                if (!arg) continue;

                auto* argValue = arg->generateCode(astContext.createContext());
                if (!argValue) continue;

                // If argument is a pointer but constructor expects value, load it
                llvm::Type* argType = argValue->getType();
                if (argType->isPointerTy()) {
                    llvm::Type* elemType = argType->getPointerElementType();
                    if (elemType->isStructTy()) {
                        auto* structType = llvm::dyn_cast<llvm::StructType>(elemType);
                        if (structType->getName().startswith("enum.")) {
                            LOG_ERROR("Likely a bitcast from struct pointer → safe to dereference") ;
                            argValue = builder.CreateLoad(elemType, argValue, "enum_val");                        
                        }                    
                    } 
                }        

                ctorArgs.push_back(argValue);
                ctorArgTypes.push_back(argType);
            }
        }

        return { ctorArgTypes, ctorArgs, objectType, baseType };
    }  

    std::string ObjectCreationNode::getMangleName(const CallableInfo& callableInfo) const {
        switch (variableType->type) {
            case DataType::BOOLEAN:     return "boolean_ctor";
            case DataType::BYTE:        return "byte_ctor";
            case DataType::SHORT:       return "short_ctor";
            case DataType::INT:         return "int_ctor";
            case DataType::LONG:        return "long_ctor";
            case DataType::FLOAT:       return "float_ctor";
            case DataType::DOUBLE:      return "double_ctor";
            case DataType::CHAR:        return "char_ctor";
            case DataType::STRING:      return "string_ctor";
            case DataType::DATETIME:    return "datetime_ctor";
            case DataType::FILE:        return "file_ctor"; 
            default: return variableType->name;
        }
    }

    std::tuple<llvm::Type*, BaseType*>  ObjectCreationNode::convertToLLVMType(const AstContext& astContext) {

        if (variableType->type == DataType::OTHER) {
            if (auto baseType = astContext.findType(variableType->name)) {
                return std::make_tuple(baseType->getLLVMType(), baseType.get());
            }
        } else {
            if (auto baseType = astContext.findType(variableType->type)) {
                return std::make_tuple(baseType->getLLVMType(), baseType.get());
            }
        }
    
        // If we reach here, type resolution failed
        LOG_ERROR("Failed to resolve LLVM type for object: {}", variableType->name);
        throw std::runtime_error("Failed to resolve LLVM type for object");
    }


    std::unique_ptr<Node> ObjectCreationNode::clone() const {
        auto clonedArgs = cloneNodeVector(arguments);
        auto clonedNode = std::make_unique<ObjectCreationNode>(variableType, std::move(clonedArgs));
        return clonedNode;
    }
    
}
