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
