/**
 * @file ImportSymbolTemplate.hpp
 * @brief Declares template utilities for resolving and retrieving imported 
 *        symbol nodes from external modules in the Lynx compiler.
 * 
 * The ImportSymbolTemplate utilities provide a type-safe mechanism to locate 
 * and retrieve AST nodes from imported modules within the global symbol context. 
 * These templates ensure that only valid Node-derived types are used and that 
 * symbol resolution is consistent across different import scopes.
 * 
 * **Key Responsibilities:**
 * - Defines generic symbol lookup functions for imported modules.
 * - Ensures type safety through compile-time Node inheritance validation.
 * - Searches for symbols by name and expected node type.
 * - Returns results wrapped in `std::optional` for safe null handling.
 * 
 * **Used By:**
 * - Import resolution, module linking, and semantic validation stages.
 * - Components interacting with the `GlobalSymbolContext` to resolve imports.
 * 
 * @see Node, GlobalSymbolContext, NodeType
 * 
 * @note These template utilities are designed for internal compiler use and 
 *       rely on consistent symbol registration within the global context.
 *       Type validation is enforced at compile time using `static_assert`.
 * 
 * @namespace LynxAst::Import
 * Provides template-based helpers for resolving imported AST symbols and 
 * ensuring Node type compatibility across modules.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_IMPORT_SYMBOL_TEMPLATE_HPP
#define LYNX_IMPORT_SYMBOL_TEMPLATE_HPP

#include <optional>
#include <constants/NodeType.hpp>
#include <context/GlobalSymbolContext.hpp>

using namespace LynxConstants;
using namespace LynxContext;

namespace LynxAst::Import {

    template <typename T>
    constexpr bool isNodeTypeCompatible() {
        return std::is_base_of_v<Node, T>;
    }

    /**
     * @brief Finds a symbol node of a specified type in imported modules.
     *
     * Searches all imported modules within the given symbol context for a symbol
     * with the specified name and node type. If found, returns a pointer to the node,
     * cast to the requested type `T*`. Otherwise, returns std::nullopt.
     *
     * @tparam T Expected node class type. Must derive from Node.
     * @param symbolCtx Pointer to the global symbol context containing import/symbol data.
     * @param symbolName The name of the symbol to find.
     * @param nodeType Expected node type (from NodeType enum).
     * @return std::optional<T*> Optional pointer to the found symbol, or std::nullopt.
     *
     * @note Requires T to derive from Node.
    */
    template <typename T>
    [[nodiscard]]
    inline std::optional<T*> findImportSymbolNode(GlobalSymbolContext* symbol, const std::string& symbolName, NodeType nodeType) {
        static_assert(isNodeTypeCompatible<T>(), "T must derive from Node");

        for (const auto& [nodeName, nodeMap] : symbol->getAllSymbolNodes()) {

            auto fileOpts = symbol->findImportKeyByValue(nodeName);

            if(!fileOpts.has_value()) continue;

            auto foundNode = symbol->findSymbolNode(nodeName, symbolName);

            if(!foundNode) continue;
            
            if (foundNode->getNodeType() == nodeType) {
                return static_cast<T*>(foundNode);
            }
        }

        return std::nullopt;
    }

}

#endif
