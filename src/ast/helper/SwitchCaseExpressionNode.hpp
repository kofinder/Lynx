/**
 * @file SwitchCaseExpressionNode.hpp
 * @brief Declares the SwitchCaseExpressionNode class representing constant expressions 
 *        used in switch-case statements within the Lynx AST.
 * 
 * The SwitchCaseExpressionNode class models both qualified and unqualified constant values 
 * used in `case` labels of switch statements. It supports integer, character, and identifier-based 
 * expressions, resolving their corresponding LLVM IR constants or variables during code generation.
 * 
 * **Key Responsibilities:**
 * - Stores case constant values (int, char, or qualified identifier).
 * - Generates LLVM IR constants or resolved variable references.
 * - Differentiates between literal and qualified case constants.
 * - Supports cloning for AST transformations and optimizations.
 * 
 * **Used By:**
 * - Switch statement handling and code generation.
 * - Constant evaluation within control-flow constructs.
 * 
 * @see Node, AstContext, VariableUtils
 * 
 * @note Qualified constants may reference variables, enum fields, or global symbols. 
 *       String literals are not supported as case constants.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_SWITCH_CASE_EXPRESSION_NODE_HPP
#define LYNX_SWITCH_CASE_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include "utils/VariableUtils.hpp"

namespace LynxAst {

    class SwitchCaseExpressionNode : public Node {

        private:

            std::variant<int, char, std::string> value;

            bool isQualified;

            /**
             * @brief Emits the LLVM value for a qualified constant (e.g., variable or enum).
             *
             * This function resolves the variable name stored in `value` using the AST context.
             * It only supports values that are ultimately resolved to integer or character types.
             * If the resolved type is unsupported or the resolution fails, an error is logged and
             * nullptr is returned.
             *
             * @note This function should only be used when the value is a qualified identifier
             *       representing a constant of type int or char (e.g., enums or variables).
             *
             * @param astContext Shared pointer to the current AST context.
             * @return llvm::Value* The resolved LLVM IR value, or nullptr on failure.
            */
            llvm::Value* emitQualifiedConstant(std::shared_ptr<AstContext> astContext) {        
                auto& builder = astContext->getBuilder();
                auto* module = astContext->getModule();
        
                std::string identifier = std::get<std::string>(value);
                auto* resolved = VariableUtils::resolveVariable(*astContext, identifier).value;

                llvm::errs() << "resolved val ===>"; resolved->print(llvm::outs()); llvm::errs() << "\n";
                llvm::errs() << "resolved type ===>"; resolved->getType()->print(llvm::outs()); llvm::errs() << "\n";

                if (resolved) {
                    if(resolved->getType()->isStructTy()) {
                        return builder.CreateExtractValue(resolved, {0});
                    }
                    return resolved;
                }
        
                auto* globalVariable = VariableUtils::resolvedEnumField(identifier, module);
                if (globalVariable) {
                    return globalVariable;
                }
        
                LOG_ERROR("Could not qualified '{}' variable", identifier);
        
                return nullptr;
            }

            /**
             * @brief Emits the LLVM constant value for an unqualified literal (int or char).
             *
             * Converts simple literal values (e.g., `case 1:`, `case 'a':`) into corresponding
             * LLVM IR constant values using the LLVM context and type system.
             *
             * @param astContext Shared pointer to the current AST context.
             * @return llvm::Value* The LLVM constant value.
            */
            llvm::Value* emitUnqualifiedConstant(std::shared_ptr<AstContext> astContext) {
                LOG_ERROR("Procced ..............");
        
                auto& context = astContext->getLLVMContext();
                auto& builder = astContext->getBuilder();
        
                llvm::Value* result = nullptr;
        
                if (std::holds_alternative<int>(value)) {
                    int intValue = std::get<int>(value);
                    result = llvm::cast<llvm::Constant>(builder.getInt32(intValue));
                } else if (std::holds_alternative<char>(value)) {
                    int charVal = std::get<char>(value);
                    result = llvm::cast<llvm::Constant>(builder.getInt8(charVal));
                } else if(std::holds_alternative<std::string>(value)){
                    std::string stringVal = std::get<std::string>(value);
                    llvm::errs() << "ERROR: Strings are not valid case constants in LLVM IR\n";        
                }
        
                llvm::errs() << "charVal ===>"; result->print(llvm::outs()); llvm::errs() << "\n";
        
                return result;
            }   
        
        public:

            explicit SwitchCaseExpressionNode(int number) : value(number), isQualified(false) {}

            explicit SwitchCaseExpressionNode(char ch) : value(ch), isQualified(false) {}

            explicit SwitchCaseExpressionNode(const std::string& identifier) : value(identifier), isQualified(true) {}

            explicit SwitchCaseExpressionNode(const std::pair<std::string, std::string>& qualifiedId): isQualified(true) {
                value = "enum." + qualifiedId.first + "." + qualifiedId.second;
            }

            std::unique_ptr<Node> clone() const override {
                if (std::holds_alternative<int>(value)) {
                    return std::make_unique<SwitchCaseExpressionNode>(std::get<int>(value));
                } else if (std::holds_alternative<char>(value)) {
                    return std::make_unique<SwitchCaseExpressionNode>(std::get<char>(value));
                } else if (std::holds_alternative<std::string>(value)) {
                    return std::make_unique<SwitchCaseExpressionNode>(std::get<std::string>(value));
                }
                
                return nullptr;        
            }

            inline constexpr NodeType getNodeType() override { return NodeType::SWITCH_CASE_CONSTANT_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override {
                if(isQualified) return emitQualifiedConstant(astContext);
                return emitUnqualifiedConstant(astContext);        
            }

            inline const std::variant<int, char, std::string>& getValue() const { return value; }

            inline bool isQualifiedIdentifier() const { return isQualified; }
        
            ~SwitchCaseExpressionNode() override = default;
    };
}

#endif
