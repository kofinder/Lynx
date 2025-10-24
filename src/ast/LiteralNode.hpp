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

using namespace LynxLogger;
using namespace LynxConstants;


namespace LynxAst {
    
    class LiteralNode: public Node {
        
        protected:

            DataType valueType;

            LValueType literalData;

        public:

            explicit LiteralNode(uint8_t byteValue) : valueType(DataType::BYTE), literalData(byteValue) {
               // LOG_WARN("[LiteralNode] btye literal {}", byteValue);
            }

            explicit LiteralNode(short shortValue) : valueType(DataType::SHORT), literalData(shortValue) {
               // LOG_WARN("[LiteralNode] short literal {}", shortValue);
            }

            explicit LiteralNode(int intValue) : valueType(DataType::INT), literalData(intValue) {
               // LOG_WARN("[LiteralNode] int literal {}", intValue);
            }

            explicit LiteralNode(long longValue) : valueType(DataType::LONG), literalData(static_cast<int64_t>(longValue)) {
               // LOG_WARN("[LiteralNode] long literal {}", longValue);
            }

            explicit LiteralNode(float floatValue) : valueType(DataType::FLOAT), literalData(floatValue) {
                //LOG_WARN("[LiteralNode] float literal {}", floatValue);
            }

            explicit LiteralNode(double doubleValue) : valueType(DataType::DOUBLE), literalData(doubleValue) {
               // LOG_WARN("[LiteralNode] double literal {}", doubleValue);
            }

            explicit LiteralNode(bool booleanValue) : valueType(DataType::BOOLEAN), literalData(booleanValue) {
               // LOG_WARN("[LiteralNode] bool literal {}", booleanValue);
            }

            explicit LiteralNode(char charValue) : valueType(DataType::CHAR), literalData(charValue) {
               // LOG_WARN("[LiteralNode] char literal {}", charValue);
            }

            explicit LiteralNode(const std::nullptr_t nil) : valueType(DataType::NULLPTR), literalData(nil) {
               //LOG_WARN("[LiteralNode] null literal {}", nil);
            }

            explicit LiteralNode(const std::string& stringValue) : valueType(DataType::STRING), literalData(stringValue) {
               //LOG_WARN("[LiteralNode] string literal {}", stringValue);
            }

            explicit LiteralNode(const Date& dateValue) : valueType(DataType::DATE), literalData(dateValue) {
               // LOG_WARN("[LiteralNode] date literal");
            }

            explicit LiteralNode(const DateTime& dateTimeValue) : valueType(DataType::DATETIME), literalData(dateTimeValue) {
               // LOG_WARN("[LiteralNode] datetime literal");
            }

            explicit LiteralNode(const File& file) : valueType(DataType::FILE), literalData(file) {
                // LOG_WARN("[LiteralNode] file literal");
            }

            std::unique_ptr<Node> clone() const override;

            inline bool isNull() const {  return valueType == DataType::NULLPTR;  }
   
            NodeType getNodeType() override { return NodeType::LITERAL_NODE; }

            constexpr inline DataType getVariableType() const { return this->valueType; };

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            inline LValueType getLiteralValue() const { return literalData; }

            ~LiteralNode() override = default;
    };
}

#endif

