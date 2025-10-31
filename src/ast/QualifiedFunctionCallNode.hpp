#ifndef LYNX_QUALIFIED_FUNCTION_CALL_NODE
#define LYNX_QUALIFIED_FUNCTION_CALL_NODE

#include <optional>
#include "Node.hpp"
#include "ExpressionNode.hpp"
#include <constants/Parameter.hpp>
#include <constants/runtime/QualifiedFileFunctionType.hpp>
#include <constants/runtime/QualifiedPrefixType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class QualifiedFunctionCallNode: public Node {

        private:
        
            std::string funcName;

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

            std::unique_ptr<QualifiedPrefixType> qualifiedPrefixType;

        public:
            QualifiedFunctionCallNode(
                const std::string& name,
                std::unique_ptr<QualifiedPrefixType> prefixType,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ): qualifiedPrefixType(std::move(prefixType)), funcName(name), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::QUALIFIED_FUNCTION_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~QualifiedFunctionCallNode() override = default;
    };

}

#endif
