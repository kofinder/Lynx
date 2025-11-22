#include "StaticMethodCallNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <types/visitor/TypeMethodRegistry.hpp>
#include <types/visitor/TypeMethodCallVisitor.hpp>

namespace LynxAst {

    llvm::Value* StaticMethodCallNode::generateCode(std::shared_ptr<AstContext> astContext) {

        const std::string typeName = varType->name;
        auto baseType = astContext->findType(typeName);
        if (!baseType.get()) {
            std::string msg = "Runtime Error: Variable '" + typeName + "' has no associated type in the type system.";
            throw std::runtime_error(msg);
        }
    
        auto& registry = astContext->getMethodTypeRegistry();
        const TypeName type{typeName};
        const MethodName method{methodName};
    
        if (!registry.hasMethod(type, method)) {
            std::string msg = "Runtime Error: Static method '" + methodName + "' does not exist on type '" + typeName + "'.";
            throw std::runtime_error(msg);
        }

        if (!registry.validateMethodCall(type, method, arguments->size())) {
            std::string msg = "Runtime Error: Static method '" + methodName + "' on type '" + typeName +
                                "' expects " + std::to_string(registry.getExpectedParamCount(type, method)) +
                                " arguments, but " + std::to_string(arguments->size()) + " were provided.";
            throw std::runtime_error(msg);
        }
    
        std::vector<llvm::Value*> argValues;
        argValues.reserve(arguments->size());
        for (auto& arg : *arguments) {
            argValues.push_back(arg->generateCode(astContext->createContext()));
        }
    
        TypeMethodCallVisitor visitor(methodName, argValues);
        baseType->accept(visitor);

        if (!visitor.result) {
            std::string msg = "Runtime Error: Failed to execute method '" + methodName  + "' of type '" + typeName + "'.";
            throw std::runtime_error(msg);
        }

        return visitor.result;
    }

    std::unique_ptr<Node> StaticMethodCallNode::clone() const  {
        using namespace Cloneable;
        auto clonedArgs = cloneNodeVector(arguments);
        auto clonedNode = std::make_unique<StaticMethodCallNode>(methodName, varType, std::move(clonedArgs));
        return clonedNode;
    }
}
