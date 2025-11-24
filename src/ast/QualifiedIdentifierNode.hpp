/**
 * @file QualifiedIdentifierNode.hpp
 * @brief Declares the QualifiedIdentifierNode class, representing a qualified identifier in the Lynx AST.
 * 
 * The QualifiedIdentifierNode class models identifiers with optional qualified prefixes, such as enums,
 * system constants, or namespaced identifiers. It provides AST traversal, cloning, and LLVM IR code generation.
 * 
 * **Key Responsibilities:**
 * - Stores the identifier name and optional qualified prefix type.
 * - Supports LLVM IR generation for qualified identifiers, including enums and system values.
 * - Provides deep cloning of the node.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR code generation for expressions and variable accesses.
 * 
 * @see QualifiedPrefixType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_QUALIFIED_IDENTIFIER_NODE_HPP
#define LYNX_QUALIFIED_IDENTIFIER_NODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Node.hpp"
#include <constants/runtime/QualifiedPrefixType.hpp>

namespace LynxAst {

    class QualifiedIdentifierNode : public Node {

        private:

            std::string identifierName;

            std::unique_ptr<QualifiedPrefixType> qualifiedPrefixType;

        private:

            llvm::Value* emitEnumValue(const AstContext& context);

            llvm::Value* emitSystemValue(const AstContext& context);

        public:

            explicit QualifiedIdentifierNode(
                const std::string& name,
                std::unique_ptr<QualifiedPrefixType> prefixNode
            ) : identifierName(name), qualifiedPrefixType(std::move(prefixNode)) {}

            inline constexpr NodeType getNodeType() override { return NodeType::QUALIFIED_IDENTIFIER_NODE; }

            std::unique_ptr<Node> clone() const override;

            llvm::Value* generateCode(std::shared_ptr<AstContext> context) override;

            ~QualifiedIdentifierNode() override = default;
    };

}

#endif 
