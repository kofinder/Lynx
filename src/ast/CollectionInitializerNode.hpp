#ifndef LYNX_COLLECTION_INITIALIZER_NODE_HPP
#define LYNX_COLLECTION_INITIALIZER_NODE_HPP

#include <any>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include "Node.hpp"
#include "ExpressionNode.hpp"
#include "helper/LiteralListNode.hpp"
#include "helper/LiteralMapNode.hpp"
#include "LiteralNode.hpp"
#include <constants/LValueType.hpp>
#include <constants/VariableType.hpp>
#include <types/interfaces/CollectionType.hpp>

using namespace LynxContext;
using namespace LynxTypes;
using namespace LynxConstants;

namespace LynxAst {

    class CollectionInitializerNode : public Node {

        public:
            using InitValueType = std::variant<
                std::unique_ptr<LiteralListNode>,
                std::unique_ptr<LiteralMapNode>
            >;

        private:

            InitValueType value;

            size_t getElementsCount() const;

            bool isNestedListUniform(const std::vector<std::unique_ptr<Node>>& values) const;

            void resolveInternalTypes(std::shared_ptr<VariableType> varType, AstContext* context);

            const CollectionType* getCollectionType(const VariableType& varType, AstContext* astContext);

            llvm::Value* extractLiteralValue(const Node* node, DataType dataType, AstContext* astContext);

            const std::string mangleSequentialName(CollectionType* collectionType, std::vector<llvm::Value*> values) const;

            const std::string mangleAssociativeName(CollectionType* collectionType, std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const;

            llvm::Value* generateLLVMFromCollection(Node* node, std::shared_ptr<VariableType> varType, std::shared_ptr<AstContext> astContext);

        public:

            explicit CollectionInitializerNode(std::unique_ptr<LiteralListNode> list) : value(std::move(list)) {}
        
            explicit CollectionInitializerNode(std::unique_ptr<LiteralMapNode> map) : value(std::move(map)) {}

            NodeType getNodeType() override { return NodeType::COLLECTION_INITIALIZER_NODE; }

            const InitValueType& getValue() const { return value; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            bool isList() const { return std::holds_alternative<std::unique_ptr<LiteralListNode>>(value); }
            
            bool isMap() const { return std::holds_alternative<std::unique_ptr<LiteralMapNode>>(value); }

            std::unique_ptr<Node> clone() const override;

            ~CollectionInitializerNode() override = default;
    };
}


#endif 
