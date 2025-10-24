#include "LynxDriver.hpp"
#include <logger/Logger.hpp>
#include "LynxScanner.hpp"
#include "parser.hpp"
#include "location.hh"
#include <constants/ImportType.hpp>

using namespace LynxConstants;

namespace LynxLang {

    LynxDriver::LynxDriver() : 
        scanner(new LynxScanner()),
        parser(new Parser(*this)),
        cursor(new location()),
        globalSymbols(std::make_shared<GlobalSymbolContext>()) {}

    std::unique_ptr<Node> LynxDriver::takeAstRoot() { return std::move(astRoot); }

    void LynxDriver::setInputStream(std::istream &stream) { sourceStream = &stream;  }

    location* LynxDriver::getCursor() const { return cursor; } 

    void LynxDriver::setLexerState(LexerState state) { scanner->setLexerState(state); }

    int LynxDriver::getError() const { return error_; }

    void LynxDriver::setError(int errorCode) { error_ = errorCode; }

    void LynxDriver::setAstRoot(std::unique_ptr<Node> node) { astRoot = std::move(node); }

    void LynxDriver::setExpectedType(DataType varType) { expectedType = varType; }

    void LynxDriver::setCurrentModuleName(const std::string& moduleName) { currentModuleName = moduleName; }

    DataType LynxDriver::getExpectedType() const { return expectedType; }

    const std::string& LynxDriver::getCurrentModuleName() const { return currentModuleName; }

    std::shared_ptr<GlobalSymbolContext> LynxDriver::getGlobalContext() const { return globalSymbols; }
    
    const std::set<std::string>& LynxDriver::getImports() const { return imports; }

    const std::unique_ptr<std::vector<Node*>>& LynxDriver::getAllAnalyzerNodes() const { return analyzeNodes; }

    Node* LynxDriver::getAstRoot() { return astRoot.get(); }

    void LynxDriver::clearImports() { imports.clear(); }

    void LynxDriver::setSourceFolders(const std::vector<std::string>& folders) { 
        this->sourceFolders = folders; 
        globalSymbols->registerSources(folders); // double check dangling issue
    }

    std::string LynxDriver::resolveImportPath(const std::string& importName) {
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
                    auto resolvedPath = entry.path().string();
                    return resolvedPath;
                }
            }
        }

        return importName;
    }


    void LynxDriver::stepCursor() {
        if (cursor) {
            cursor->step();
        }
    }

    void LynxDriver::reset() {
        astRoot.reset();
        imports.clear();
        error_ = 0;
        if(cursor) delete cursor;
        cursor = new location();
    }
    
    Node* LynxDriver::parse() {

        if (!scanner || !cursor || !parser || !sourceStream) {
            throw std::runtime_error("One or more LynxDriver components are null.");
        }

        // Redirect the scanner to read from the specified input stream and report errors to std::cerr  
        this->scanner->switch_streams(this->sourceStream, &std::cerr);  

        // Prepare the cursor by processing the lines in the input stream  
        cursor->lines();  

        // Perform the parsing operation  
        this->parser->parse();  

        // Return the root of the AST after parsing  
        return this->getAstRoot();  
    }

    void LynxDriver::addAnalyzerNode(Node* node) {
        if (!analyzeNodes) {
            analyzeNodes = std::make_unique<std::vector<Node*>>();
        }
        analyzeNodes->push_back(node);    
    }

    void LynxDriver::addImport(const std::string& importName) { 
        if (std::find(runtime_import_files.begin(), runtime_import_files.end(), importName) != runtime_import_files.end()) {
            std::cout << "found file" << std::endl;
        } else {
            std::string resolvedPath = resolveImportPath(importName);
            imports.insert(resolvedPath); 
        }
    }

    void LynxDriver::removeImport(const std::string& importName) {
        auto it = imports.find(importName);
        if (it != imports.end()) {
            imports.erase(it);
            std::cout << "Removed import: " << importName << "\n";
        } else {
            std::cout << "Import not found: " << importName << "\n";
        }    
    }

    
    void LynxDriver::registerFunction(FunctionNode* fnNode) {
        if (!fnNode) return;
        if (currentModuleName.empty()) currentModuleName = "main";
        std::cout << "[Register] FunctionNode '" << fnNode->getFunctionName() << "' => " << currentModuleName << std::endl;
        globalSymbols->registerSymbolNode(currentModuleName, fnNode->getFunctionName(), fnNode);
        addAnalyzerNode(fnNode);
    }

    void LynxDriver::registerClazz(ClazzDeclarationNode* clazzNode) {
        if (!clazzNode) return;
        if (currentModuleName.empty()) currentModuleName = "main";
        std::cout << "[Register] ClazzDeclarationNode '" << clazzNode->getOriginalName() << "' => " << currentModuleName << std::endl;
        globalSymbols->registerSymbolNode(currentModuleName, clazzNode->getOriginalName(), clazzNode);
        addAnalyzerNode(clazzNode);
    }

    void LynxDriver::registerInterface(InterfaceDeclarationNode* ifaceNode) {
        if (!ifaceNode) return;
        if (currentModuleName.empty()) currentModuleName = "main";
        std::cout << "[Register] InterfaceDeclarationNode '" << ifaceNode->getInterfaceName() << "' => " << currentModuleName << std::endl;
        globalSymbols->registerSymbolNode(currentModuleName, ifaceNode->getInterfaceName(), ifaceNode);
        addAnalyzerNode(ifaceNode);
    }
    
    LynxDriver:: ~LynxDriver() {
        delete parser; 
        delete scanner;
        delete cursor;
    }

}