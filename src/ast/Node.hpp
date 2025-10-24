#ifndef LYNX_NODE_HPP
#define LYNX_NODE_HPP

/**
 * @file Node.hpp
 * @brief Abstract base class representing a node in an abstract syntax tree (AST).
 * 
 * This class serves as the foundation for all nodes in the AST, providing a common interface
 * for code generation and type identification. Derived classes will implement specific node types,
 * such as expressions, literals, and declarations.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include <iostream>
#include "llvm/IR/Value.h"
#include <constants/NodeType.hpp>
#include <context/AstContext.hpp>

using namespace LynxContext;
using namespace LynxConstants;

namespace LynxAst {

    class Node {

        protected:
        
            int lineNumber; 
    
        public:

            // Virtual destructor to ensure proper cleanup of derived classes.
            virtual ~Node() {}

            /**
             * @brief Set the line number for this node.
             * 
             * This is used for error reporting and debugging.
             * @param lNo The line number to set.
             */
            void setLineNumber(int lNo);

            /**
             * @brief Prepare the node for code generation.
             * 
             * Typically initializes or configures the node for code generation processes.
             */
            void startCodeGen(std::shared_ptr<AstContext> astContext);

            /**
             * @brief Pure virtual function for generating code.
             * 
             * Must be implemented by derived classes to generate the necessary LLVM code for this AST node.
             * @param astContext Shared pointer to the AST context.
             * @return LLVM value representing the generated code for this node.
             */
            virtual llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) = 0;

            /**
             * @brief Pure virtual function to get the type of the node.
             * 
             * Must be implemented by derived classes to indicate what kind of node this is (e.g., expression, declaration).
             * @return The type of the node as a NodeType.
             */
            virtual NodeType getNodeType() = 0;

            /**
             * @brief Create a deep copy of the node.
             * 
             * This pure virtual method must be overridden by derived classes to provide
             * cloning logic for their specific node types. It enables safe copying of AST
             * subtrees, typically used for transformations, optimizations, or memoization.
             * 
             * @return A unique_ptr to the newly cloned node.
            */
            virtual std::unique_ptr<Node> clone() const = 0;

            int getLineNumber() const { return lineNumber; }
            
            void llvmPrint(llvm::Value* value);
    };
}

#endif
