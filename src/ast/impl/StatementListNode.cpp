#include "StatementListNode.hpp"
#include <logger/Logger.hpp>
#include <context/GlobalSymbolContext.hpp>
using namespace LynxLogger;

namespace LynxAst {

    void StatementListNode::createLocal(const std::string &varName, VariableDeclarationNode* value) {
        LOG_INFO("Dispatched{}", varName);
        this->locals.insert(std::pair<std::string, VariableDeclarationNode*>(varName, value));
    }

    VariableDeclarationNode* StatementListNode::findLocal(const std::string& varName) {
        auto val = this->locals.find(varName);
        if (val != this->locals.end()) {
            return val->second;
        }

        LOG_ERROR("Local variable '{}' not found", varName);
        return nullptr;
    }

    llvm::Value* StatementListNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked ....");
        
        auto& builder = astContext->getBuilder();
        auto scopeContext = astContext->getGlobalContext();
        scopeContext->addStatement(this);

        llvm::Value* finaLValueType = nullptr;
        for (auto const& value : this->statements) { // cannot read access memory
            finaLValueType = value->generateCode(astContext->createContext());
            if (scopeContext->getFunctionNode() && builder.GetInsertBlock()->getTerminator()) {
                break;
            }
        }

        scopeContext->removeStatement();
        return finaLValueType;
    }

    std::unique_ptr<Node> StatementListNode::clone() const {
        auto clone = std::make_unique<StatementListNode>();

        for (const auto& stmt : statements) {
            if (stmt) {
                try {
                    auto clonedStmt = stmt->clone();
                    clone->pushStatement(std::move(clonedStmt));
                } catch (const std::exception& e) {
                    std::cerr << "Exception during statement clone: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Warning: encountered null statement in statement list." << std::endl;
            }
        }
    
        for (const auto& local : locals) {
            auto clonedVar = static_cast<VariableDeclarationNode*>(local.second->clone().release());
            clone->createLocal(local.first, clonedVar);
        }
    
        return clone;
    }    
}