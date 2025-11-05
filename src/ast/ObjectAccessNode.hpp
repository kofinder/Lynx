/**
 * @file ObjectAccessNode.hpp
 * @brief Declares the ObjectAccessNode class, representing field access in objects within the Lynx AST.
 * 
 * The ObjectAccessNode class models member (field) access in objects, including chained accesses. It supports
 * AST traversal, cloning, and LLVM IR code generation for field access expressions.
 * 
 * **Key Responsibilities:**
 * - Stores the name of the field being accessed.
 * - Supports chaining of multiple field accesses via `nextAccessNode`.
 * - Provides LLVM IR code generation for object field access.
 * - Supports deep cloning of the node and its chained accesses.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for member access expressions.
 * 
 * @see Node
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_OBJECT_ACCESS_NODE_HPP
#define LYNX_OBJECT_ACCESS_NODE_HPP

#include <string>
#include "Node.hpp"
#include <llvm/IR/IRBuilder.h>


namespace LynxAst {

    using namespace LynxContext;
    using namespace LynxConstants;

    class ObjectAccessNode: public Node {
        
        public:

            std::string fieldName;
            ObjectAccessNode* nextAccessNode;

            explicit ObjectAccessNode(const std::string& field) : fieldName(field), nextAccessNode(nullptr) {}
        
            explicit ObjectAccessNode(const std::string& field, ObjectAccessNode* nextNode) : fieldName(field), nextAccessNode(nextNode) {}

            std::unique_ptr<Node> clone() const override {
                return std::make_unique<ObjectAccessNode>(*this); 
            }
    
            NodeType getNodeType() override { return NodeType::OBJECT_ACCESS_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ObjectAccessNode() {}
    };
}
#endif
