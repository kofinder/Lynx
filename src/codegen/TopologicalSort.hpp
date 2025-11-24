/**
 * @file TopologicalSort.hpp
 * @brief Declares the TopologicalSort class for computing compilation order of Lynx modules.
 * 
 * The TopologicalSort class provides functionality to analyze module import dependencies
 * and produce a topologically sorted order of files. This ensures that each module
 * is processed only after all of its dependencies have been resolved, which is critical
 * for correct LLVM IR generation and compilation.
 * 
 * **Key Responsibilities:**
 * - Maintains a dependency graph mapping files to the modules they import.
 * - Performs depth-first search (DFS) to detect cyclic dependencies.
 * - Produces a topologically sorted list of module paths.
 * 
 * **Used By:**
 * - IRPlanner to determine compilation order for code generation.
 * - IRGenerator to orchestrate module-level LLVM IR generation.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/


#ifndef LYNX_TOPOLOGICAL_SORT_HPP
#define LYNX_TOPOLOGICAL_SORT_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace LynxCodegen {

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
