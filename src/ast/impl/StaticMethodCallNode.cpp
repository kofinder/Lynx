#include "StaticMethodCallNode.hpp"
#include <resolver/TypeResolverFactory.hpp>
#include "tmpl/CloneNodeTemplate.hpp"

namespace LynxAst {

    using LynxResolver::TypeResolverFactory;
    using LynxConstants::DataType;

    llvm::Value* StaticMethodCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("IR Code Generation ...{} {}", typeName, methodName);

        auto* baseType = astContext->findType(typeName).get();
        if (!baseType) {
            std::string msg = "Runtime Error: Variable '" + typeName + "' has no associated type in the type system.";
            throw std::runtime_error(msg);
        }
    
        auto resolver = baseType->createMethodResolver();
        if (!resolver) {
            std::string msg = "Runtime Error: Type '" + baseType->getDebugName() + "' does not provide a method resolver.";
            throw std::runtime_error(msg);
        }
    
        std::vector<llvm::Value*> argValues;
        argValues.reserve(arguments->size());
        for (auto& arg : *arguments) {
            llvm::Value* value = arg->generateCode(astContext);
            argValues.push_back(value);
        }

        if (!resolver->hasMethod(methodName)) {
            std::string msg = "Runtime Error: Static method '" + methodName + "' does not exist on type '" + typeName + "'.";
            throw std::runtime_error(msg);
        }

        if (!resolver->validateMethodCall(methodName, arguments->size())) {
            std::string msg = "Runtime Error: Static method '" + methodName + "' on type '" + typeName +
                              "' expects " + std::to_string(resolver->getExpectedParamCount(methodName)) +
                              " arguments, but " + std::to_string(arguments->size()) + " were provided.";
            throw std::runtime_error(msg);
        }
    
        auto* result = resolver->resolveMethod(methodName, nullptr, argValues, std::move(astContext));
        if (!result) {
            std::string msg = "Runtime Error: Failed to execute static method '" + typeName + "." + methodName + "'.";
            throw std::runtime_error(msg);
        }
    
        return result;
    }

    std::unique_ptr<Node> StaticMethodCallNode::clone() const  {
        using namespace Cloneable;
        auto clonedArgs = cloneNodeVector(arguments);
        auto clonedNode = std::make_unique<StaticMethodCallNode>(typeName, methodName, std::move(clonedArgs));
        return clonedNode;
    }
}