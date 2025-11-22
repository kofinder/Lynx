#include "QualifiedFunctionCallNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "utils/VariableUtils.hpp"
#include <runtime/RuntimeModuleLoader.hpp>
#include <types/visitor/TypeMethodRegistry.hpp>
#include <types/visitor/TypeMethodCallVisitor.hpp>

namespace LynxAst {

    using namespace Cloneable;
    using namespace LynxContext;
    using namespace LynxRuntime;
    using namespace VariableUtils;

    llvm::Value* QualifiedFunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Namespace Name {} Function {}", qualifiedPrefixType->getRawPrefix(), funcName);
        const std::string qualifiedFunction = qualifiedPrefixType->getRawPrefix() + "::" + funcName;
        std::cout << "qualified function ======>" << qualifiedFunction << std::endl;
        
        if(auto func = RuntimeFunctionRegistry::getInstance().resolveFunction(qualifiedFunction)) {
            return func->call(astContext, std::move(*arguments));
        }
        
        return dispatchInstanceExtensionMethod(std::move(astContext));
    }

    llvm::Value* QualifiedFunctionCallNode::dispatchInstanceExtensionMethod(std::shared_ptr<AstContext> astContext) {
        const std::string& varName = qualifiedPrefixType->getRawPrefix();
        auto resolvedVar = VariableUtils::resolveVariable(*astContext, varName);
        if (!resolvedVar.value || !resolvedVar.reference) {
            std::string msg = "Runtime Error: Failed to resolve variable '" + varName + "' in the current scope.";
            throw std::runtime_error(msg);
        }
    
        auto baseType = astContext->findType(resolvedVar.value);
        if (!baseType.get()) {
            std::string msg = "Runtime Error: Variable '" + varName + "' has no associated type in the type system.";
            throw std::runtime_error(msg);
        }

        const std::string typeName = dataTypeToString(baseType->getTypeTag());
        auto& registry = astContext->getMethodTypeRegistry();

        const TypeName type{typeName};
        const MethodName method{funcName};
    
        if (!registry.hasMethod(type, method)) {
            std::string msg = "Runtime Error: Instance method '" + funcName + "' does not exist on type '" + typeName + "'.";
            throw std::runtime_error(msg);
        }

        if (!registry.validateMethodCall(type, method, arguments->size())) {
            std::string msg = "Runtime Error: Instance method '" + funcName + "' on type '" + typeName +
                                "' expects " + std::to_string(registry.getExpectedParamCount(type, method)) +
                                " arguments, but " + std::to_string(arguments->size()) + " were provided.";
            throw std::runtime_error(msg);
        }
    
        std::vector<llvm::Value*> argValues;
        argValues.reserve(arguments->size());
        for (auto& arg : *arguments) {
            argValues.push_back(arg->generateCode(astContext->createContext()));
        }
    
        TypeMethodCallVisitor visitor(funcName, resolvedVar.value, resolvedVar.reference, argValues);
        baseType->accept(visitor);
        auto* result = visitor.getResult();

        if (!result) {
            std::string msg = "Runtime Error: Failed to execute method '" + funcName + "' on instance '" + varName + "' of type '" + typeName + "'.";
            throw std::runtime_error(msg);
        }

        return result;    
    }

    std::unique_ptr<Node> QualifiedFunctionCallNode::clone() const {
        auto clonedArgs = cloneNodeVector(arguments);
        return std::make_unique<QualifiedFunctionCallNode>(
            funcName, 
            qualifiedPrefixType ? std::make_unique<QualifiedPrefixType>(*qualifiedPrefixType) : nullptr,
            std::move(clonedArgs)
        );
    }
}