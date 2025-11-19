#include "Node.hpp"
#include <logger/Logger.hpp>

namespace LynxAst {
    
    using namespace LynxLogger;

    void Node::llvmPrint(llvm::Value* value) {
        llvm::errs() << "Result > "; value->print(llvm::outs()); llvm::errs() << "\n";
    }

    void Node::startCodeGen(std::shared_ptr<AstContext> astCtx) {  

        if (!astCtx) {
            throw std::runtime_error("AstContext cannot be null in startCodeGen");
        }

        // Initialize default types only if the context has not yet done so
        if (astCtx->currentDepth == 0) {
            astCtx->initializeDefaultTypes();
            astCtx->initializeTypeMethods();
        }

        // Create the next context based on the newly initialized context
        // This will inherit the depth from newCtx and share the same types map
        auto nextCtx = astCtx->createContext(); 

        // Generate code for this node using the next context, which contains the initialized types
        // This calls the pure virtual method codeGen, which must be implemented by derived classes
        generateCode(astCtx);
    } 
    
}