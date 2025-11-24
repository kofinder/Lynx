/**
 * @file LiteralNode.hpp
 * @brief Declares the LiteralNode class, representing literal values in the Lynx AST.
 * 
 * The LiteralNode class models constant literal values such as integers, floats, booleans,
 * strings, dates, files, and null. It supports type tracking, LLVM IR code generation,
 * and deep cloning of literal nodes.
 * 
 * **Key Responsibilities:**
 * - Stores the literal value and its associated data type.
 * - Supports various literal types including BYTE, SHORT, INT, LONG, FLOAT, DOUBLE, BOOLEAN, CHAR, STRING, NULLPTR, DATE, DATETIME, and FILE.
 * - Provides methods to inspect literal type and value.
 * - Generates LLVM IR code for literal values.
 * - Supports deep cloning of the node.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for constant expressions.
 * 
 * @see DataType, LValueType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_LITERAL_NODE_HPP
#define LYNX_LITERAL_NODE_HPP

#include <variant>
#include "Node.hpp"
#include <logger/Logger.hpp>
#include <constants/DataType.hpp>
#include <constants/LValueType.hpp>
#include <context/AstContext.hpp>


namespace LynxAst {
    
   using namespace LynxLogger;
   using namespace LynxConstants;

    class LiteralNode : public Node {
        
        protected:

            DataType valueType;

            LValueType literalData;

        public:

            explicit LiteralNode(uint8_t byteValue) noexcept : valueType(DataType::BYTE), literalData(byteValue) {}

            explicit LiteralNode(short shortValue) noexcept : valueType(DataType::SHORT), literalData(shortValue) {}

            explicit LiteralNode(int intValue) noexcept : valueType(DataType::INT), literalData(intValue) {}

            explicit LiteralNode(long longValue) noexcept : valueType(DataType::LONG), literalData(static_cast<int64_t>(longValue)) {}

            explicit LiteralNode(float floatValue) noexcept : valueType(DataType::FLOAT), literalData(floatValue) {}

            explicit LiteralNode(double doubleValue) noexcept : valueType(DataType::DOUBLE), literalData(doubleValue) {}

            explicit LiteralNode(bool booleanValue) noexcept : valueType(DataType::BOOLEAN), literalData(booleanValue) {}

            explicit LiteralNode(char charValue) noexcept : valueType(DataType::CHAR), literalData(charValue) {}

            explicit LiteralNode(const std::nullptr_t nil) noexcept: valueType(DataType::NULLPTR), literalData(nil) {}

            explicit LiteralNode(const std::string& stringValue) : valueType(DataType::STRING), literalData(stringValue) {}

            explicit LiteralNode(const Date& dateValue) : valueType(DataType::DATE), literalData(dateValue) {}

            explicit LiteralNode(const DateTime& dateTimeValue) : valueType(DataType::DATETIME), literalData(dateTimeValue) {}

            explicit LiteralNode(const File& file) : valueType(DataType::FILE), literalData(file) {}

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::LITERAL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] inline bool isNull() const noexcept {  return valueType == DataType::NULLPTR;  }
   
            [[nodiscard]] inline constexpr DataType getVariableType() const noexcept { return valueType; }

            [[nodiscard]] inline LValueType getLiteralValue() const noexcept { return literalData; }

            ~LiteralNode() override = default;
    };
}

#endif

