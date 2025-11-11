#include "StaticMethodCallNode.hpp"
#include <resolver/TypeResolverFactory.hpp>
#include "tmpl/CloneNodeTemplate.hpp"

namespace LynxAst {

    using LynxResolver::TypeResolverFactory;
    using LynxConstants::DataType;

    llvm::Value* StaticMethodCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("IR Code Generation ...{} {}", typeName, methodName);
        
        auto dataType = parseDataType(typeName);
        auto resolver = TypeResolverFactory::forType(dataType);
        if (!resolver) {
            LOG_ERROR("Unknown type resolver for type: {}", typeName);
            return nullptr;
        }

        std::vector<llvm::Value*> argValues;
        argValues.reserve(arguments->size());
        for (auto& arg : *arguments) {
            llvm::Value* value = arg->generateCode(astContext);
            argValues.push_back(value);
        }

        auto* result = resolver->resolveMethod(methodName, nullptr, argValues, std::move(astContext));

        if (!result) {
            LOG_ERROR("Failed to resolve static method '{}.{}'", typeName, methodName);
            return nullptr;
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