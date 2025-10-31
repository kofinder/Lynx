#ifndef LYNX_OBJECT_ACCESS_NODE_HPP
#define LYNX_OBJECT_ACCESS_NODE_HPP

#include <string>
#include "Node.hpp"
#include <llvm/IR/IRBuilder.h>

using namespace LynxContext;
using namespace LynxConstants;

namespace LynxAst {

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
