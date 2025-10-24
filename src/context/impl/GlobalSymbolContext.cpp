#include "GlobalSymbolContext.hpp"

namespace LynxContext {

    Node* GlobalSymbolContext::findSymbolNode(const std::string& module, const std::string& nodeName) const {
        auto modIt = nodeMap.find(module);
        if (modIt == nodeMap.end()) return nullptr;
        auto symIt = modIt->second.find(nodeName);
        if (symIt == modIt->second.end()) return nullptr;

        return symIt->second;
    }

    std::shared_ptr<VirtualTable> GlobalSymbolContext::findVTable(const std::string& name) const {
        auto it = vtables.find(name);
        if (it == vtables.end()) return nullptr;

        return it->second;
    }

    std::optional<std::string> GlobalSymbolContext::findImportKeyByValue(const std::string& filePath) const {
        for (const auto& [key, value] : importMap) {
            if (value == filePath) {
                return key;
            }
        }

        return std::nullopt;    
    }

    std::optional<std::string> GlobalSymbolContext::findImportValueByKey(const std::string& moduleName) const {
        auto it = importMap.find(moduleName);
        if (it != importMap.end()) {
            return it->second;
        }

        return std::nullopt;    
    }

    VariableDeclarationNode* GlobalSymbolContext::findVariable(const std::string& name) const {
        for (auto it = statements.rbegin(); it != statements.rend(); ++it) {
            auto* stmtList = dynamic_cast<StatementListNode*>(*it);
            if (stmtList) {
                if (auto* var = stmtList->findLocal(name)) {
                    return var;
                }
            }
        }

        return nullptr;
    }

    llvm::Value* GlobalSymbolContext::findLocalVariable(const std::string& varName) const {
        auto it = localVariableMap.find(varName);
        return it != localVariableMap.end() ? it->second : nullptr;
    }
    
}