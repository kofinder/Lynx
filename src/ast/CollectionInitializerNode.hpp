/**
 * @file CollectionInitializerNode.hpp
 * @brief Declares the CollectionInitializerNode class for initializing collections in the Lynx AST.
 * 
 * The CollectionInitializerNode class handles both sequential (lists) and associative (maps)
 * collection initializations, supporting nested and type-consistent elements. It provides
 * LLVM IR code generation for collection literals and manages internal type resolution.
 * 
 * **Key Responsibilities:**
 * - Stores either a list or a map literal as the collection value.
 * - Validates uniformity of nested lists.
 * - Resolves variable types in context.
 * - Generates LLVM IR for collection literals.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - AST nodes representing collection initializations.
 * - Code generation routines for collection literals.
 * 
 * @see Node, LiteralListNode, LiteralMapNode, CollectionType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

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


namespace LynxAst {

    using namespace LynxContext;
    using namespace LynxTypes;
    using namespace LynxConstants;

    class CollectionInitializerNode : public Node {

        public:
        
            using InitValueType = std::variant<
                std::unique_ptr<LiteralListNode>,
                std::unique_ptr<LiteralMapNode>
            >;

        private:

            InitValueType value;

        private:

            [[nodiscard]] size_t getElementsCount() const noexcept;
            [[nodiscard]] bool isNestedListUniform(const std::vector<std::unique_ptr<Node>>& values) const noexcept;

            void resolveInternalTypes(const std::shared_ptr<VariableType>& varType, const AstContext& context) const noexcept;

            [[nodiscard]] const CollectionType* getCollectionType(const VariableType& varType, const AstContext& context) const noexcept;

            [[nodiscard]] llvm::Value* extractLiteralValue(const Node* node, DataType dataType, const AstContext& context) const;

            [[nodiscard]] std::string mangleSequentialName(const CollectionType* collectionType, const std::vector<llvm::Value*>& values) const noexcept;

            [[nodiscard]] std::string mangleAssociativeName(const CollectionType* collectionType, const std::vector<std::pair<llvm::Value*, llvm::Value*>>& pairs) const noexcept;

            [[nodiscard]] llvm::Value* generateLLVMFromCollection(Node* node, std::shared_ptr<VariableType> varType, std::shared_ptr<AstContext> astContext);

        public:

            explicit CollectionInitializerNode(std::unique_ptr<LiteralListNode> list) noexcept : value(std::move(list)) {}
        
            explicit CollectionInitializerNode(std::unique_ptr<LiteralMapNode> map) noexcept : value(std::move(map)) {}

            inline constexpr NodeType getNodeType() override { return NodeType::COLLECTION_INITIALIZER_NODE; }

            [[nodiscard]] const InitValueType& getValue() const { return value; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] inline bool isList() const { return std::holds_alternative<std::unique_ptr<LiteralListNode>>(value); }
            
            [[nodiscard]] inline bool isMap() const { return std::holds_alternative<std::unique_ptr<LiteralMapNode>>(value); }

            std::unique_ptr<Node> clone() const override;

            ~CollectionInitializerNode() override = default;
    };
}


#endif 
