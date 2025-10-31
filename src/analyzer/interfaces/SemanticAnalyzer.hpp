#ifndef LYNX_SEMANTIC_ANALYZER_HPP
#define LYNX_SEMANTIC_ANALYZER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include <ast/Node.hpp>
#include "SemanticState.hpp"
#include "SemanticThreadPool.hpp"
#include "SemanticStateFactory.hpp"

namespace LynxAnalyzer {

    using namespace LynxAst;

    class SemanticAnalyzer {

        private:

            std::mutex symbolMutex;
        
            std::vector<std::unique_ptr<SemanticState>> activeStates;

            std::unordered_map<std::string, std::string> symbolTable;

            // Thread-safe error storage
            mutable std::mutex errorMutex;
            std::vector<std::string> errors;

        public:

            SemanticAnalyzer() {
                activeStates = SemanticStateFactory::createAllStates();
            }
            
            bool symbolExists(const std::string& name); 

            void analyzeASTParallel(const Node* node);

            void analyzeNodeParallel(Node* node, SemanticThreadPool& pool);

            void addSymbolThreadSafe(const std::string& name, const std::string& type);

            const std::unordered_map<std::string, std::string>& getSymbolTable() const;

            void reportError(const std::string& msg, int lineNo);

            const std::vector<std::string>& getErrors() const { return errors; }

            bool hasErrors() const;

    };

}

#endif
