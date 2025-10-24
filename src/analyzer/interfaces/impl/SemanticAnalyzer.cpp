#include "interfaces/SemanticAnalyzer.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;

namespace LynxAnalyzer {

    void SemanticAnalyzer::analyzeASTParallel(const Node* node) {
        if (!node) return;

        // Thread pool sized to hardware concurrency
        SemanticThreadPool pool(std::thread::hardware_concurrency());

        // Start analyzing the root node
        analyzeNodeParallel(const_cast<Node*>(node), pool);

        // Wait for all tasks to complete
        pool.waitForCompletion();

        std::cout << "[SemanticAnalyzer] Semantic analysis completed.\n";
    }

    void SemanticAnalyzer::analyzeNodeParallel(Node* node, SemanticThreadPool& pool) {
        LOG_ERROR("Invoked .....");

        if (!node) return;

        // Submit current node processing to the pool
        pool.enqueue([this, node, &pool]() {
            // Each active state handles the node
            for (auto& state : activeStates) {
                state->handleNode(*this, node);
            }

            // Recursively process child nodes
            // for (auto& child : node->getChildren()) {
            //     analyzeNodeParallel(child.get(), pool);
            // }
            
        });
    }
    
    void SemanticAnalyzer::addSymbolThreadSafe(const std::string& name, const std::string& type) {
        std::lock_guard<std::mutex> lock(symbolMutex);
        symbolTable[name] = type;
    }

    bool SemanticAnalyzer::symbolExists(const std::string& name) {
        std::lock_guard<std::mutex> lock(symbolMutex);
        return symbolTable.find(name) != symbolTable.end();
    }

    const std::unordered_map<std::string, std::string>& SemanticAnalyzer::getSymbolTable() const {
        return symbolTable;
    }

    void SemanticAnalyzer::reportError(const std::string& msg, int lineNo) {
        std::cout << "error message ===>" << msg << " line number ===>" << lineNo << std::endl;
        std::lock_guard<std::mutex> lock(errorMutex);
        std::string location  = "Line " + std::to_string(lineNo);
        errors.push_back(location + ": " + msg);
    }

    bool SemanticAnalyzer::hasErrors() const {
        std::lock_guard<std::mutex> lock(errorMutex);
        return !errors.empty();
    }

}
