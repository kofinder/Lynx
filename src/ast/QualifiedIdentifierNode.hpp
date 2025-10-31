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

            llvm::Value* emitEnumValue(AstContext& context);

            llvm::Value* emitSystemValue(AstContext& context);

        public:

            explicit QualifiedIdentifierNode(
                const std::string& name,
                std::unique_ptr<QualifiedPrefixType> prefixNode
            ) : identifierName(name), qualifiedPrefixType(std::move(prefixNode)) {}

            NodeType getNodeType() override { return NodeType::QUALIFIED_IDENTIFIER_NODE; }

            std::unique_ptr<Node> clone() const override;

            llvm::Value* generateCode(std::shared_ptr<AstContext> context) override;

            ~QualifiedIdentifierNode() override = default;
    };

}

#endif 
