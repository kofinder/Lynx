#ifndef LYNX_LOGGER_PRINTER_NODE
#define LYNX_LOGGER_PRINTER_NODE

#include "Node.hpp"

namespace LynxAst {

    /**
     * @class LoggerPrinterNode
     * @brief Represents a node in the AST for logging or printing an expression.
     *
     * This node encapsulates an expression to be logged and generates the
     * appropriate LLVM code to handle the logging operation.
     */
    class LoggerPrinterNode: public Node {

        private:

            std::unique_ptr<Node> expressionNode;

            std::vector<llvm::Value*> preparePrintfArguments(std::shared_ptr<AstContext> astContext, llvm::Value* expressionValue);

        public:
          
            explicit LoggerPrinterNode(std::unique_ptr<Node> expNode): expressionNode(std::move(expNode)) {}

            NodeType getNodeType() override { return NodeType::LOG_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;

            ~LoggerPrinterNode() override = default;
        };

    }

#endif

