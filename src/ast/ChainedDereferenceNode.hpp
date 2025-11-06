/**
 * @file ChainedDereferenceNode.hpp
 * @brief Declares the ChainedDereferenceNode class representing chained variable dereferences in the Lynx AST.
 * 
 * The ChainedDereferenceNode class handles a chain of variable dereferences (e.g., `a.b.c`),
 * supporting LLVM IR code generation and cloning for AST transformations.
 * 
 * **Key Responsibilities:**
 * - Stores a sequence of VariableDereferenceNode objects representing the dereference chain.
 * - Supports addition of new dereferences to the chain.
 * - Generates LLVM IR for chained object access.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - Expression nodes that require multi-level object access.
 * 
 * @see Node, VariableDereferenceNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CHAINED_DEREFERENCE_NODE
#define LYNX_CHAINED_DEREFERENCE_NODE

#include "Node.hpp"
#include "VariableDereferenceNode.hpp"


namespace LynxAst {

    using namespace LynxConstants;

    class ChainedDereferenceNode : public Node {

        private:
        
            std::vector<std::unique_ptr<VariableDereferenceNode>> derefChain;

        public:

            explicit ChainedDereferenceNode(std::vector<std::unique_ptr<VariableDereferenceNode>> chain) : derefChain(std::move(chain)) {}

            explicit ChainedDereferenceNode(std::unique_ptr<VariableDereferenceNode> derefNode) { derefChain.push_back(std::move(derefNode)); }

            void addDereference(std::unique_ptr<VariableDereferenceNode> derefNode) { derefChain.push_back(std::move(derefNode)); }

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::OBJECT_ACCESS_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] const auto& getChainedNode() const { return derefChain; }
    };

}
#endif 
