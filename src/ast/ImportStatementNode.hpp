#ifndef LYNX_IMPORT_STATEMENT_NODE_HPP
#define LYNX_IMPORT_STATEMENT_NODE_HPP

#include <optional>
#include "Node.hpp"
#include "helper/ImportSymbol.hpp"
#include <constants/ImportType.hpp>
#include <constants/runtime/RumtimeModuleType.hpp>

using namespace LynxConstants;

namespace LynxAst {
    
    class ImportStatementNode: public Node {
        
        private:

            std::string moduleName;

            ImportType importType;

            std::string defaultImportAlias;

            std::optional<std::vector<std::unique_ptr<ImportSymbol>>> namedSymbols;

            void initializeRuntimeClassesForModule(AstContext* astContext, RuntimeModuleType moduleType);
            
        public:

            explicit ImportStatementNode(
                const std::string& module, 
                ImportType type
            ) : moduleName(module), importType(type) {}

            ImportStatementNode(
                const std::string& module, 
                ImportType type, 
                const std::string& defaultAlias
            ) : moduleName(module), importType(type), defaultImportAlias(defaultAlias) {}

            ImportStatementNode(
                const std::string& module, ImportType type,
                std::vector<std::unique_ptr<ImportSymbol>> symbols,
                const std::string& defaultAlias = ""
            ) : moduleName(module), importType(type), namedSymbols(std::move(symbols)), defaultImportAlias(defaultAlias) {}

            NodeType getNodeType() override { return NodeType::IMPORT_STATEMENT_NODE; }

            std::unique_ptr<Node> clone() const override;

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            const std::string& getModuleName() const { return moduleName; }

            const std::optional<std::vector<std::unique_ptr<ImportSymbol>>>& getNamedSymbols() const { return namedSymbols; }

            const std::string& getDefaultImportAlias() const { return defaultImportAlias; }

            constexpr ImportType getImportType() const { return importType; }
        
            ~ImportStatementNode() override = default;
    };
}

#endif 
