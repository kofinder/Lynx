/**
 * @file ImportStatementNode.hpp
 * @brief Declares the ImportStatementNode class, representing import statements in the Lynx AST.
 * 
 * The ImportStatementNode class models module import statements in Lynx, including default imports,
 * named imports, and import types (static, regular, or wildcard). It also integrates with the runtime
 * to initialize module classes as necessary.
 * 
 * **Key Responsibilities:**
 * - Stores module name, import type, default alias, and optionally named symbols.
 * - Provides functionality for runtime module initialization.
 * - Supports deep cloning of import statements.
 * - Generates LLVM IR or runtime setup code for imported modules.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - Runtime module and symbol initialization.
 * 
 * @see ImportType, ImportSymbol, RuntimeModuleType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_IMPORT_STATEMENT_NODE_HPP
#define LYNX_IMPORT_STATEMENT_NODE_HPP

#include <optional>
#include "Node.hpp"
#include "helper/ImportSymbol.hpp"
#include <constants/ImportType.hpp>
#include <constants/runtime/RumtimeModuleType.hpp>


namespace LynxAst {

    using namespace LynxConstants;
    
    class ImportStatementNode : public Node {
        
        private:

            std::string moduleName;

            ImportType importType;

            std::string defaultImportAlias;

            std::optional<std::vector<std::unique_ptr<ImportSymbol>>> namedSymbols;
        
        private:

            void initializeRuntimeClassesForModule(const AstContext& astContext, RuntimeModuleType moduleType);
            
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

            inline constexpr NodeType getNodeType() override { return NodeType::IMPORT_STATEMENT_NODE; }

            std::unique_ptr<Node> clone() const override;

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] const std::string& getModuleName() const { return moduleName; }

            [[nodiscard]] const std::optional<std::vector<std::unique_ptr<ImportSymbol>>>& getNamedSymbols() const noexcept { return namedSymbols; }

            [[nodiscard]] const std::string& getDefaultImportAlias() const noexcept { return defaultImportAlias; }

            [[nodiscard]] inline constexpr ImportType getImportType() const noexcept { return importType; }
        
            ~ImportStatementNode() override = default;
    };
}

#endif 
