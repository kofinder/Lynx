#ifndef LYNX_DRIVER_HPP
#define LYNX_DRIVER_HPP

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "LexerState.hpp"
#include <ast/Node.hpp>
#include <ast/FunctionNode.hpp>
#include <ast/ClazzDeclarationNode.hpp>
#include <ast/InterfaceDeclarationNode.hpp>

#include <context/GlobalSymbolContext.hpp>

namespace fs = boost::filesystem;

using namespace LynxAst;
using namespace LynxContext;

namespace LynxLang {

    class Parser; 
    class location; 
    class LynxScanner;

    class LynxDriver {

        private:

            int type_nesting_depth = 0;

            int literal_nesting_depth = 0;
            
            static constexpr int MAX_NESTING_DEPTH = 64;
    

        private:

            std::set<std::string> imports;

            std::shared_ptr<GlobalSymbolContext> globalSymbols;

            std::vector<std::string> sourceFolders;

            std::unique_ptr<std::vector<Node*>> analyzeNodes;
            
            std::string currentModuleName;

            void setError(int errorCode);    
        
        public:

            LynxScanner* scanner;

            Parser* parser;
    
            location* cursor;
    
            std::unique_ptr<Node> astRoot;

            int error_ = 0;

            DataType expectedType = DataType::OTHER;

            std::istream* sourceStream = nullptr;

            // Granting access to private members for trusted classes
            friend class Parser;
            friend class LynxScanner;

        public:

            LynxDriver();

            void setSourceFolders(const std::vector<std::string>& sourceFolders);
            
            void setInputStream(std::istream &stream);

            void setInputStream(std::unique_ptr<std::istream> stream); // owning

            void stepCursor();

            int getError() const;

            location* getCursor() const;

            void reset();

            void setLexerState(LexerState state);

            void setAstRoot(std::unique_ptr<Node> node);

            void setExpectedType(DataType dataType);
            
            DataType getExpectedType() const;

            Node* getAstRoot();

            std::unique_ptr<Node> takeAstRoot();

            Node* parse();

            void clearImports();

            void addImport(const std::string& importName);

            void removeImport(const std::string& importName);

            std::string resolveImportPath(const std::string& importName);

            const std::set<std::string>& getImports() const;

            void registerFunction(FunctionNode* fnNode);

            void registerClazz(ClazzDeclarationNode* clazzNode);

            void registerInterface(InterfaceDeclarationNode* ifaceNode);
        
            std::shared_ptr<GlobalSymbolContext> getGlobalContext() const;

            const std::string& getCurrentModuleName() const;

            void setCurrentModuleName(const std::string& moduleName);

            void addAnalyzerNode(Node* node);

            const std::unique_ptr<std::vector<Node*>>& getAllAnalyzerNodes() const;
        
            ~LynxDriver();
    };
}

#endif
