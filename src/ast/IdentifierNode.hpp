/**
 * @file IdentifierNode.hpp
 * @brief Declares the IdentifierNode class, representing identifiers (variable or symbol names) in the Lynx AST.
 * 
 * The IdentifierNode class models names of variables, functions, classes, or other symbols. It stores
 * the identifier name and supports LLVM IR generation and deep cloning for AST transformations.
 * 
 * **Key Responsibilities:**
 * - Stores the identifier name.
 * - Provides deep cloning functionality.
 * - Generates LLVM IR references for the identifier during code generation.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - Code generation for variable access, function calls, or symbol resolution.
 * 
 * @see Node, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_IDENTIFIER_NODE_HPP
#define LYNX_IDENTIFIER_NODE_HPP

#include <string>
#include "Node.hpp"


namespace LynxAst {

    using namespace LynxContext;
    using namespace LynxConstants;


    class IdentifierNode : public Node {

        public:
        
            std::string identifierName;
        
        public:
            
            explicit IdentifierNode(const std::string& name) : identifierName(name) {}

            std::unique_ptr<Node> clone() const override;
                
            NodeType getNodeType() override { return NodeType::IDENTIFIER_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            const std::string& getName() const { return identifierName; }
    };
}

#endif
