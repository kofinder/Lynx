#ifndef LYNX_AST_SUPER_CONSTRUCTOR_CALL_NODE_HPP
#define LYNX_AST_SUPER_CONSTRUCTOR_CALL_NODE_HPP

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Node.hpp"
#include "utils/StringUtils.hpp"
#include "PrimaryExpressionNode.hpp"
#include <constants/Parameter.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class SuperConstructorCallNode : public Node {

        private:
                
            std::string construcotrName;

            std::unique_ptr<std::vector<std::unique_ptr<PrimaryExpressionNode>>> arguments;
          
            llvm::Value* generateExpression(const AstContext& astContext, const PrimaryExpressionNode& exprNode, std::vector<llvm::Value*> argOfValues, std::vector<std::string> argOfNames);
        
        public:  

            explicit SuperConstructorCallNode(
                std::string ctorName, 
                std::unique_ptr<std::vector<std::unique_ptr<PrimaryExpressionNode>>> args
            ) : construcotrName(std::move(ctorName)), arguments(std::move(args)) {}
        
            NodeType getNodeType() override { return NodeType::SUPER_CONSTRUCTOR_CALL_NODE; }
        
            std::unique_ptr<Node> clone() const override;
        
            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* generate(const AstContext& astContext, const ClassType& bizzType, std::vector<llvm::Value*> argOfValues, std::vector<std::string> argOfNames);

            inline const std::string& getConstructorName() const { return construcotrName; }
                
            ~SuperConstructorCallNode() override = default;
        };

}

#endif
