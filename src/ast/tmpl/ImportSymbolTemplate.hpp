#ifndef LYNX_IMPORT_SYMBOL_TEMPLATE_HPP
#define LYNX_IMPORT_SYMBOL_TEMPLATE_HPP

#include <optional>
#include <constants/NodeType.hpp>
#include <context/GlobalSymbolContext.hpp>

using namespace LynxConstants;
using namespace LynxContext;

namespace LynxAst::Import {

    /**
     * @brief Utility template to find a symbol node of a specified type in imported modules.
     * 
     * This function searches all imported modules within the given AST context for a symbol
     * with the specified name and node type. If found, it returns a pointer to the symbol node,
     * cast to the requested type `T*`. Otherwise, it returns std::nullopt.
     * 
     * @tparam T Expected node class type. Must derive from Node.
     * @param astContext Pointer to the current AST context containing import and symbol info.
     * @param symbolName The name of the symbol to find.
     * @param nodeType The expected node type (enum) for the symbol.
     * 
     * @return std::optional<T*> Optional pointer to the found symbol node, or std::nullopt if not found.
     * 
     * @note The template parameter `T` should correspond to the expected node type and
     * the `nodeType` parameter must match the node's `getNodeType()` value.
     * 
     * @throws static_assert at compile-time if `T` is not derived from `Node`.
     */
    template <typename T>
    inline std::optional<T*> findImportSymbolNode(GlobalSymbolContext* symbol, const std::string& symbolName, NodeType nodeType) {
        static_assert(std::is_base_of<Node, T>::value, "T must be derived from Node");

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
