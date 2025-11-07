#include "SystemCallExpressionNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <system/SystemModuleFacade.hpp>

namespace LynxAst {

    using LynxSystem::SystemModuleFacade;

    llvm::Value* SystemCallExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("IR Code Generation: {} {}", moduleName, methodName);

        if (methodName.empty()) {
            LOG_ERROR("Invalid system statement: missing method name");
            return nullptr;
        }    

        std::vector<llvm::Value*> calleeArgs;
        calleeArgs.reserve(arguments ? arguments->size() : 0);
        if (arguments) {
            for (auto& element : *arguments) {
                calleeArgs.push_back(element->generateCode(astContext->createContext()));
            }
        }

        SystemModuleFacade facade;
        auto result = facade.invokeMethod(astContext, moduleName, methodName, calleeArgs);
        return result;
    }

    std::unique_ptr<Node> SystemCallExpressionNode::clone() const {
        auto clonedArgs = Cloneable::cloneNodeVector(arguments);
        if (!moduleName.empty()) {
            return std::make_unique<SystemCallExpressionNode>(moduleName, methodName, std::move(clonedArgs));
        } else {
            return std::make_unique<SystemCallExpressionNode>(methodName, std::move(clonedArgs));
        }
    }

}