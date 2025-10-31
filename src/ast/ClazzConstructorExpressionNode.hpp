#ifndef LYNX_CLAZZ_CONSTRUCTOR_EXPRESSION_NODE_HPP
#define LYNX_CLAZZ_CONSTRUCTOR_EXPRESSION_NODE_HPP

#include <string>
#include <optional>
#include <ast/Node.hpp>
#include "ObjectCreationNode.hpp"

namespace LynxAst {

    class ClazzConstructorExpressionNode : public Node {

        private:

            std::string fieldName;

            std::string argumentName;

            std::optional<std::unique_ptr<ObjectCreationNode>> objectCreationNode;

        public:
        
            ClazzConstructorExpressionNode(
                const std::string& field, 
                const std::string& argument
            ) : fieldName(field), argumentName(argument) {}
        
            ClazzConstructorExpressionNode(
                const std::string& field, 
                std::unique_ptr<ObjectCreationNode> objCreation
            ) : fieldName(field), objectCreationNode(std::move(objCreation)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::CLAZZ_CONSTRUCTOR_EXPRESSION_NODE; }
        
            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            const std::string& getFieldName() const { return fieldName; }

            llvm::Value* generate(AstContext& astContext, const ClassType& clazzType, std::vector<llvm::Value*> argOfValues, std::vector<std::string> argOfNames);
        
            ~ClazzConstructorExpressionNode() override = default;
            
        };
} 
#endif
