#ifndef LYNX_LITERAL_NODE_HPP
#define LYNX_LITERAL_NODE_HPP

/**
 * @file LiteralNode.hpp
 * @brief Class representing a literal node in the abstract syntax tree (AST), encapsulating variable types and their associated values.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
*/

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

            NodeType getNodeType() override { return NodeType::LITERAL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            inline bool isNull() const noexcept {  return valueType == DataType::NULLPTR;  }
   
            inline constexpr DataType getVariableType() const noexcept { return valueType; }

            inline LValueType getLiteralValue() const noexcept { return literalData; }

            ~LiteralNode() override = default;
    };
}

#endif

