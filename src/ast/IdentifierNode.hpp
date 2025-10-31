#ifndef LYNX_IDENTIFIER_NODE_HPP
#define LYNX_IDENTIFIER_NODE_HPP

#include <string>
#include "Node.hpp"

using namespace LynxContext;
using namespace LynxConstants;

namespace LynxAst {

    class IdentifierNode: public Node {

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
