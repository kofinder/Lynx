#include "Node.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;

namespace LynxAst {
    
    void Node::setLineNumber(int lNo) {
        this->lineNumber = lNo;
    }

    void Node::startCodeGen(std::shared_ptr<AstContext> newCtx) {  
        // Initialize the types in the newly created context
        // This sets up any predefined types (like "int") that will be used in the AST
        newCtx->initializeDefaultTypes();

        // Create the next context based on the newly initialized context
        // This will inherit the depth from newCtx and share the same types map
        auto nextCtx = newCtx->createContext(); 

        // Generate code for this node using the next context, which contains the initialized types
        // This calls the pure virtual method codeGen, which must be implemented by derived classes
        this->generateCode(nextCtx);
    } 

    void Node::llvmPrint(llvm::Value* value) {
        llvm::errs() << "Result > "; value->print(llvm::outs()); llvm::errs() << "\n";
    }
}