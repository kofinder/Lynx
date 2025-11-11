#include "QualifiedFunctionCallNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "utils/VariableUtils.hpp"
#include <resolver/TypeResolverFactory.hpp>
#include <libruntime/RuntimeModuleLoader.hpp>

namespace LynxAst {

    using namespace Cloneable;
    using namespace LynxContext;
    using namespace LynxLibRuntime;
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
        if (!resolvedVar.value) {
            std::string msg = "Runtime Error: Failed to resolve variable '" + varName + "' in the current scope.";
            throw std::runtime_error(msg);
        }
    
        auto* baseType = astContext->findType(resolvedVar.value).get();
        if (!baseType) {
            std::string msg = "Runtime Error: Variable '" + varName + "' has no associated type in the type system.";
            throw std::runtime_error(msg);
        }
    
        auto resolver = baseType->createMethodResolver();
        if (!resolver) {
            std::string msg = "Runtime Error: Type '" + baseType->getDebugName() + "' does not provide a method resolver.";
            throw std::runtime_error(msg);
        }
    
        if (!resolver->hasMethod(funcName)) {
            std::string msg = "Runtime Error: Method '" + funcName + "' does not exist on type '" + baseType->getDebugName() + "'.";
            throw std::runtime_error(msg);
        }
    
        if (!resolver->validateMethodCall(funcName, arguments->size())) {
            std::string msg = "Runtime Error: Method '" + funcName + "' on type '" + baseType->getDebugName() + 
                              "' expects " + std::to_string(resolver->getExpectedParamCount(funcName)) +
                              " arguments, but " + std::to_string(arguments->size()) + " were provided.";
            throw std::runtime_error(msg);
        }
    
        std::vector<llvm::Value*> argValues;
        argValues.reserve(arguments->size());
        for (auto& arg : *arguments) {
            llvm::Value* value = arg->generateCode(astContext);
            argValues.push_back(value);
        }
    
        auto* result = resolver->resolveMethod(funcName, resolvedVar.reference, argValues, std::move(astContext));
        if (!result) {
            std::string msg = "Runtime Error: Failed to execute method '" + funcName +
                              "' on instance '" + varName + "' of type '" + baseType->getDebugName() + "'.";
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