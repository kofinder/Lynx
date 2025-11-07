/**
 * @file Node.hpp
 * @brief Declares the abstract base Node class for the Lynx AST.
 * 
 * The Node class is the root of the AST hierarchy, defining the common interface and behavior
 * for all AST nodes in the Lynx compiler. It provides basic functionality such as line number
 * tracking, LLVM code generation scaffolding, and deep cloning support.
 * 
 * **Key Responsibilities:**
 * - Stores the source line number for error reporting and debugging.
 * - Defines pure virtual interfaces for LLVM code generation (`generateCode`), node type identification (`getNodeType`), and deep cloning (`clone`).
 * - Provides utility methods for debugging (e.g., `llvmPrint`) and code generation preparation (`startCodeGen`).
 * 
 * **Used By:**
 * - All derived AST nodes, including expression nodes, statement nodes, and type nodes.
 * - Semantic analysis and LLVM IR generation subsystems.
 * 
 * @see NodeType, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_NODE_HPP
#define LYNX_NODE_HPP

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

            /**
             * @brief Virtual destructor.
             *
             * Ensures proper cleanup of derived classes.
            */
            virtual ~Node() noexcept {}

            /**
             * @brief Set the line number for this node.
             * 
             * This is used for error reporting and debugging.
             * @param lineNo The line number to set.
             */
            void setLineNumber(int lineNo) noexcept { lineNumber = lineNo; }

            /**
             * @brief Get the line number of this node.
             * @return Line number.
            */
            int getLineNumber() const noexcept { return lineNumber; }
            
            /**
             * @brief Print LLVM value for debugging purposes.
             * @param value LLVM Value to print.
            */
            void llvmPrint(llvm::Value* value);


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
            virtual inline constexpr NodeType getNodeType() = 0;

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
    };
}

#endif
