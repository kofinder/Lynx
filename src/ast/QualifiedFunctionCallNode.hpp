/**
 * @file QualifiedFunctionCallNode.hpp
 * @brief Declares the QualifiedFunctionCallNode class, representing a function call with an optional qualified prefix in the Lynx AST.
 * 
 * The QualifiedFunctionCallNode class models function calls that may include a qualified prefix such as file, module, or namespace.
 * It holds the function name, its arguments, and an optional qualified prefix for semantic analysis and code generation.
 * 
 * **Key Responsibilities:**
 * - Stores the function name, argument list, and optional qualified prefix.
 * - Supports LLVM IR code generation for qualified function calls.
 * - Provides deep cloning of the node and its arguments.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR code generation for function call expressions.
 * 
 * @see ExpressionNode, QualifiedPrefixType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


#ifndef LYNX_QUALIFIED_FUNCTION_CALL_NODE
#define LYNX_QUALIFIED_FUNCTION_CALL_NODE

#include <optional>
#include "Node.hpp"
#include "ExpressionNode.hpp"
#include <constants/Parameter.hpp>
#include <constants/runtime/QualifiedFileFunctionType.hpp>
#include <constants/runtime/QualifiedPrefixType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class QualifiedFunctionCallNode : public Node {

        private:
        
            std::string funcName;

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

            std::unique_ptr<QualifiedPrefixType> qualifiedPrefixType;

        private:

            llvm::Value* dispatchInstanceExtensionMethod(std::shared_ptr<AstContext> astContext);

        public:
            QualifiedFunctionCallNode(
                const std::string& name,
                std::unique_ptr<QualifiedPrefixType> prefixType,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : qualifiedPrefixType(std::move(prefixType)), funcName(name), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::QUALIFIED_FUNCTION_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~QualifiedFunctionCallNode() override = default;
    };

}

#endif
