#ifndef LYNX_TOPOLOGICAL_SORT_HPP
#define LYNX_TOPOLOGICAL_SORT_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace LynxCodegen {

    /**
     * @brief Utility class for computing a topological order of source files based on import dependencies.
     *
     * This class is designed to resolve the order in which source modules should be processed,
     * such that dependencies (imports) are always handled before the modules that depend on them.
     * It detects cycles and avoids redundant traversal using DFS.
     */
    class TopologicalSort {

        private:
            /// A map of file paths to the list of files they import.
            std::unordered_map<std::string, std::vector<std::string>> fileImportMap;

            /// Tracks files that have already been visited during DFS.
            std::unordered_set<std::string> visited;

            /// Tracks files currently on the DFS stack to detect cycles.
            std::unordered_set<std::string> onStack;

            /// Stores the final sorted order of modules
            std::vector<std::string> sortedModules;

            /**
             * @brief Depth-first traversal for topological sorting.
             *
             * Recursively visits all dependencies of a given file, adding them to the output
             * in post-order (i.e., after all dependencies are resolved).
             * 
             * @param filePath The path to the file currently being visited.
             * @throws std::runtime_error If a cyclic dependency is detected.
             */
            void dfsTopoSort(const std::string& filePath);

        public:

            TopologicalSort() = default;

            /**
             * @brief Sets the import dependency graph.
             * 
             * Should be called before computing the topological order.
             *
             * @param dependencyGraph A map where each key is a file path and the value is a list of imported files.
             */
            void setDependencyGraph(const std::unordered_map<std::string, std::vector<std::string>>& dependencyGraph);

            /**
             * @brief Computes and returns the topological order of files.
             * 
             * Uses DFS to ensure that all dependencies are processed before their dependents.
             *
             * @return A list of file paths in topological order.
             * @throws std::runtime_error If a circular dependency is found.
             */
            std::vector<std::string> getTopologicalModuleOrder();
    };
}

#endif
