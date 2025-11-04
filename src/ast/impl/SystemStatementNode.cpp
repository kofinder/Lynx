#include "SystemStatementNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <system/SystemModuleFacade.hpp>

namespace LynxAst {

    using LynxSystem::SystemModuleFacade;

    llvm::Value* SystemStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("IR Code Generation: {} {}", moduleName, methodName);

        if (moduleName.empty() || methodName.empty()) {
            LOG_ERROR("Invalid system statement: missing module/method name");
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

    std::unique_ptr<Node> SystemStatementNode::clone() const {
        auto clonedArgs = Cloneable::cloneNodeVector(arguments);
        if (!moduleName.empty()) {
            return std::make_unique<SystemStatementNode>(moduleName, methodName, std::move(clonedArgs));
        } else {
            return std::make_unique<SystemStatementNode>(methodName, std::move(clonedArgs));
        }
    }

}