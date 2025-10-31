#include "TopologicalSort.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxCodegen {

    void TopologicalSort::setDependencyGraph(const std::unordered_map<std::string, std::vector<std::string>>& dependencyGraph) {
        fileImportMap = dependencyGraph;
    }

    void TopologicalSort::dfsTopoSort(const std::string& filePath) {
        if (visited.count(filePath)) return;
    
        if (onStack.count(filePath)) {
            throw std::runtime_error("[TopologicalSort Error] Detected a cycle involving: " + filePath);
        }
    
        onStack.insert(filePath);
    
        for (const auto& dependency : fileImportMap[filePath]) {
            dfsTopoSort(dependency);
        }
    
        onStack.erase(filePath);
        visited.insert(filePath);
        sortedModules.push_back(filePath);
    }
    

    std::vector<std::string> TopologicalSort::getTopologicalModuleOrder() {
        visited.clear();
        onStack.clear();
        sortedModules.clear();
    
        for (const auto& [file, _] : fileImportMap) {
            if (!visited.count(file)) {
                dfsTopoSort(file);
            }
        }
    
        return sortedModules;
    }
    
}
