#include "codegen/IRPlanner.hpp"
#include "codegen/TopologicalSort.hpp"
#include <logger/Logger.hpp>
#include <ast/ImportStatementNode.hpp>
#include <ast/StatementListNode.hpp>

using namespace LynxLogger;

namespace LynxCodegen {

    void IRPlanner::planModuleOrder() {

        auto sorter = std::make_unique<TopologicalSort>();

        std::unordered_map<std::string, std::vector<std::string>> dependencyGraph;
        for (const auto& [filePath, node] : moduleAstMap) {
            std::vector<std::string> imports;
            if(auto stmtsNode = dynamic_cast<StatementListNode*>(node.get())) {
                for (const auto& stmtNode : stmtsNode->getStatements()) {
                    if(auto importNode = dynamic_cast<ImportStatementNode*>(stmtNode.get())) {
                        auto rawImportName = importNode->getModuleName();
                        auto resolvedImport = resolveImportPath(rawImportName);
                        if (!resolvedImport.empty() && resolvedImport != filePath) {
                            auto resolvedPath = resolvedImport.string();
                            LOG_INFO("The '{}' was imported by {}", resolvedPath, filePath);
                            imports.push_back(resolvedPath);
                        }
                    }
                }
            }

            dependencyGraph[filePath] = std::move(imports);
        }

        sorter->setDependencyGraph(dependencyGraph);
        orderedModules = sorter->getTopologicalModuleOrder();
    }

    fs::path IRPlanner::resolveImportPath(const std::string& importName) {
        fs::path importPath(importName);

        if (importPath.extension().empty()) {
            importPath += ".lynx";
        }
    
        for (const auto& folder : sourceFolders) {
            if (!fs::exists(folder) || !fs::is_directory(folder)) {
                std::cerr << "[Warning] Folder not found or not a directory!: " << folder << std::endl;
                continue;
            }
            for (const auto& entry : fs::recursive_directory_iterator(folder)) {
                if (!fs::is_regular_file(entry) || entry.path().extension() != ".lynx") continue;
                fs::path relativePath = fs::relative(entry.path(), folder);
                if (relativePath.filename() == importPath.filename()) {
                    return entry.path().string();
                }
            }
        }

        return "";
    }
    
    const std::vector<std::string>& IRPlanner::getOrderedModules() const {
        return orderedModules;
    }
}
