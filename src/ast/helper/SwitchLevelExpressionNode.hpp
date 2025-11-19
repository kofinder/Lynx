/**
 * @file SwitchLevelExpressionNode.hpp
 * @brief Declares the SwitchLevelExpressionNode class representing arithmetic and bitwise 
 *        operations within switch-case level expressions in the Lynx AST.
 * 
 * The SwitchLevelExpressionNode class implements arithmetic and bitwise expression handling
 * within switch-case contexts. It performs LLVM IR code generation for binary operations such
 * as addition, subtraction, multiplication, and bitwise manipulations.
 * 
 * **Key Responsibilities:**
 * - Stores an operator type and its left/right operand nodes.
 * - Generates LLVM IR for arithmetic and bitwise operations.
 * - Provides internal helper methods for LLVM instruction emission.
 * - Supports cloning for AST transformations and compiler optimization passes.
 * 
 * **Used By:**
 * - Expression evaluation within switch-case constant computations.
 * - Code generation stages that handle operator-based constant folding.
 * 
 * @see Node, AstContext, OperatorType
 * 
 * @note Logs each emitted LLVM instruction for debugging and code generation tracing.
 * 
 * @authoɾ: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_SWITCH_LEVEL_EXPRESSION_NODE_HPP
#define LYNX_SWITCH_LEVEL_EXPRESSION_NODE_HPP

#include "Node.hpp"

namespace LynxAst {

    class SwitchLevelExpressionNode : public Node {

        private:

            OperatorType operatorType;

            std::unique_ptr<Node> leftOperand;

            std::unique_ptr<Node> rightOperand;

            llvm::Value* emitPlus(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::Add, lhs, rhs, "emit_plus", block);
            }
        
            llvm::Value* emitMinus(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::Sub, lhs, rhs, "emit_minus", block);
            }
        
            llvm::Value* emitMultiply(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::Mul, lhs, rhs, "emit_multiply", block);
            }
        
            llvm::Value* emitBitwiseXor(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::Xor, lhs, rhs, "emit_xor", block);
            }
        
            llvm::Value* emitBitwiseAnd(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::And, lhs, rhs, "emit_and", block);
            }
        
            llvm::Value* emitBitwiseOr(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::Or, lhs, rhs, "emit_or", block);
            }
        
            llvm::Value* emitShiftLeft(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::Shl, lhs, rhs, "emit_lshift", block);
            }
        
            llvm::Value* emitShiftRight(llvm::Value* lhs, llvm::Value* rhs, llvm::BasicBlock* block) {
                return llvm::BinaryOperator::Create(llvm::Instruction::LShr, lhs, rhs, "emit_rshift", block);
            }
        
            llvm::Value* emitDefaultConst(llvm::Value* lhsValue, llvm::BasicBlock* currentBlock) {
                if (!llvm::isa<llvm::Constant>(lhsValue)) {
                    LOG_ERROR("LHS is not a constant. Returning nullptr as fallback.");
                    return nullptr;
                }
                
                return lhsValue;
            }
    
        public:
        
            SwitchLevelExpressionNode(
                OperatorType operType,
                std::unique_ptr<Node> leftNode,
                std::unique_ptr<Node> rightNode
            ) : operatorType(operType), leftOperand(std::move(leftNode)), rightOperand(std::move(rightNode)) {}

            inline constexpr NodeType getNodeType() override { return NodeType::SWITCH_CASE_CONSTANT_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override {
                auto* currentBlock = astContext->getBuilder().GetInsertBlock();
                auto* lhsValue = leftOperand->generateCode(astContext->createContext());
                auto* rhsValue = rightOperand ? rightOperand->generateCode(astContext->createContext()) : nullptr;
                switch (operatorType) {
                    case OperatorType::PLUS:        return emitPlus(lhsValue, rhsValue, currentBlock);
                    case OperatorType::MINUS:       return emitMinus(lhsValue, rhsValue, currentBlock);
                    case OperatorType::MUL:         return emitMultiply(lhsValue, rhsValue, currentBlock);
                    case OperatorType::BITWISE_XOR: return emitBitwiseXor(lhsValue, rhsValue, currentBlock);
                    case OperatorType::BIT_AND:     return emitBitwiseAnd(lhsValue, rhsValue, currentBlock);
                    case OperatorType::BIT_OR:      return emitBitwiseOr(lhsValue, rhsValue, currentBlock);
                    case OperatorType::LSHIFT:      return emitShiftLeft(lhsValue, rhsValue, currentBlock);
                    case OperatorType::RSHIFT:      return emitShiftRight(lhsValue, rhsValue, currentBlock);
                    default:                        return emitDefaultConst(lhsValue, currentBlock);
                }
            }

            std::unique_ptr<Node> clone() const override {
                std::unique_ptr<Node> clonedLhs = leftOperand ? leftOperand->clone() : nullptr;
                std::unique_ptr<Node> clonedRhs = rightOperand ? rightOperand->clone() : nullptr;
                return std::make_unique<SwitchLevelExpressionNode>(operatorType, std::move(clonedLhs), std::move(clonedRhs));        
            }

            ~SwitchLevelExpressionNode() override {}
    };
}

#endif
