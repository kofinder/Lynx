#ifndef LYNX_NODE_TYPE_HPP  
#define LYNX_NODE_TYPE_HPP  

/**  
 * @enum NODE_TYPE  
 *   
 * @brief Enum representing different types of nodes in an abstract syntax tree (AST).  
 *   
 * This enum categorizes the various nodes that can exist in the AST for a compiler or interpreter.  
 * Each node type corresponds to a specific component of the syntax tree,   
 * allowing for structured manipulation and analysis of the source code.  
 */  

namespace LynxConstants {

    enum class NodeType {  
        EMPTY_NODE = 100,          // Start from 100

        IDENTIFIER_NODE = 101,            // 101
        COLLECTION_INITIALIZER_NODE,// 102

        ASSIGNMENT_NODE = 110,      // explicitly set, next will increment
        EXPRESSION_NODE,            // 111
        STATEMENT_NODE,             // 112

        LITERAL_NODE = 120,         // reset to 120
        LITERAL_MAP_NODE,           // 121
        LITERAL_LIST_NODE,          // 122
        LITERAL_PAIR_NODE,          // 123

        STATEMENT_LIST_NODE = 130,  
        VARIABLE_DECLARATION_NODE,  // 131
        VARIABLE_DEREFERENCE_NODE,  // 132

        IF_NODE = 140,  
        TERNARY_OPERATOR_NODE,      // 141
        SWITCH_NODE,                // 142
        SWITCH_CASE_CONSTANT_NODE,  // 143
        
        UNARY_OPERATION_NODE = 150,
        COMPARE_OPERATION_NODE,     // 151
        LOG_NODE,                   // 152
        SYSTEM_NODE,
        
        FUNCTION_NODE = 160,  
        METHOD_CALL_NODE = 161,
        FUNCTION_CALL_NODE = 162,  
        
        VALUE_PLACEHOLDER_NODE = 170,  
        LOOP_NODE,                  // 171  
        
        CLAZZ_DECLARATION_NODE,     // 181  
        MIXIN_DECLARATION_NODE,     // 181  


        INTERFACE_DECLARATION_NODE = 190,  

        OBJECT_CREATION_NODE = 200,
        SUPER_CONSTRUCTOR_CALL_NODE, // 201
        CLAZZ_CONSTRUCTOR_NODE,      // 202
        CLAZZ_CONSTRUCTOR_EXPRESSION_NODE,      // 202
        ENUM_NODE = 210,
        ENUM_ACCESS_NODE,            // 211
        ARRAY_ACCESS_NODE = 220,
        OBJECT_ACCESS_NODE,          // 221
        MEMBER_ACCESS_NODE,          // 222
        
        TYPE_CONV_NODE = 230,  
        EXCEPTION_HANDLE_NODE = 231,  
        
        ARRAY_NODE = 240,
        BINARY_OPERATION_NODE = 241,

        PRIMARY_EXPR_NODE = 250,
        IMPORT_STATEMENT_NODE = 251,
        QUALIFIED_FUNCTION_CALL_NODE = 252,
        QUALIFIED_IDENTIFIER_NODE = 253,
        STATIC_METHOD_CALL_NODE = 254,
        ENUM_VALUE_EXPRESSION_NODE = 255
    };

    inline NodeType parseStringToNodeType(const std::string& name) {
        static const std::unordered_map<std::string, NodeType> strToType = {
            {"EMPTY_NODE", NodeType::EMPTY_NODE},
            {"IDENTIFIER_NODE", NodeType::IDENTIFIER_NODE},
            {"COLLECTION_INITIALIZER_NODE", NodeType::COLLECTION_INITIALIZER_NODE},
            {"ASSIGNMENT_NODE", NodeType::ASSIGNMENT_NODE},
            {"EXPRESSION_NODE", NodeType::EXPRESSION_NODE},
            {"STATEMENT_NODE", NodeType::STATEMENT_NODE},
            {"LITERAL_NODE", NodeType::LITERAL_NODE},
            {"LITERAL_MAP_NODE", NodeType::LITERAL_MAP_NODE},
            {"LITERAL_LIST_NODE", NodeType::LITERAL_LIST_NODE},
            {"LITERAL_PAIR_NODE", NodeType::LITERAL_PAIR_NODE},
            {"STATEMENT_LIST_NODE", NodeType::STATEMENT_LIST_NODE},
            {"VARIABLE_DECLARATION_NODE", NodeType::VARIABLE_DECLARATION_NODE},
            {"VARIABLE_DEREFERENCE_NODE", NodeType::VARIABLE_DEREFERENCE_NODE},
            {"IF_NODE", NodeType::IF_NODE},
            {"CLAZZ_CONSTRUCTOR_EXPRESSION_NODE", NodeType::CLAZZ_CONSTRUCTOR_EXPRESSION_NODE},
            {"TERNARY_OPERATOR_NODE", NodeType::TERNARY_OPERATOR_NODE},
            {"SWITCH_NODE", NodeType::SWITCH_NODE},
            {"SWITCH_CASE_CONSTANT_NODE", NodeType::SWITCH_CASE_CONSTANT_NODE},
            {"UNARY_OPERATION_NODE", NodeType::UNARY_OPERATION_NODE},
            {"COMPARE_OPERATION_NODE", NodeType::COMPARE_OPERATION_NODE},
            {"LOG_NODE", NodeType::LOG_NODE},
            {"FUNCTION_NODE", NodeType::FUNCTION_NODE},
            {"METHOD_CALL_NODE", NodeType::METHOD_CALL_NODE},
            {"FUNCTION_CALL_NODE", NodeType::FUNCTION_CALL_NODE},
            {"VALUE_PLACEHOLDER_NODE", NodeType::VALUE_PLACEHOLDER_NODE},
            {"LOOP_NODE", NodeType::LOOP_NODE},
            {"CLAZZ_DECLARATION_NODE", NodeType::CLAZZ_DECLARATION_NODE},
            {"INTERFACE_DECLARATION_NODE", NodeType::INTERFACE_DECLARATION_NODE},
            {"OBJECT_CREATION_NODE", NodeType::OBJECT_CREATION_NODE},
            {"SUPER_CONSTRUCTOR_CALL_NODE", NodeType::SUPER_CONSTRUCTOR_CALL_NODE},
            {"CLAZZ_CONSTRUCTOR_NODE", NodeType::CLAZZ_CONSTRUCTOR_NODE},
            {"ENUM_NODE", NodeType::ENUM_NODE},
            {"ENUM_ACCESS_NODE", NodeType::ENUM_ACCESS_NODE},
            {"ARRAY_ACCESS_NODE", NodeType::ARRAY_ACCESS_NODE},
            {"OBJECT_ACCESS_NODE", NodeType::OBJECT_ACCESS_NODE},
            {"MEMBER_ACCESS_NODE", NodeType::MEMBER_ACCESS_NODE},
            {"TYPE_CONV_NODE", NodeType::TYPE_CONV_NODE},
            {"EXCEPTION_HANDLE_NODE", NodeType::EXCEPTION_HANDLE_NODE},
            {"ARRAY_NODE", NodeType::ARRAY_NODE},
            {"BINARY_OPERATION_NODE", NodeType::BINARY_OPERATION_NODE},
            {"PRIMARY_EXPR_NODE", NodeType::PRIMARY_EXPR_NODE},
            {"IMPORT_STATEMENT_NODE", NodeType::IMPORT_STATEMENT_NODE},
            {"QUALIFIED_FUNCTION_CALL_NODE", NodeType::QUALIFIED_FUNCTION_CALL_NODE},
            {"QUALIFIED_IDENTIFIER_NODE", NodeType::QUALIFIED_IDENTIFIER_NODE},
            {"STATIC_METHOD_CALL_NODE", NodeType::STATIC_METHOD_CALL_NODE},
            {"ENUM_VALUE_EXPRESSION_NODE", NodeType::ENUM_VALUE_EXPRESSION_NODE}
        };
        auto it = strToType.find(name);
        return (it != strToType.end()) ? it->second : NodeType::EMPTY_NODE;
    }

    inline std::string parseNodeTypeToString(NodeType mType) {
        switch (mType) {
            case NodeType::EMPTY_NODE: return "EMPTY_NODE";
            case NodeType::IDENTIFIER_NODE: return "IDENTIFIER_NODE";
            case NodeType::COLLECTION_INITIALIZER_NODE: return "COLLECTION_INITIALIZER_NODE";
            case NodeType::ASSIGNMENT_NODE: return "ASSIGNMENT_NODE";
            case NodeType::EXPRESSION_NODE: return "EXPRESSION_NODE";
            case NodeType::STATEMENT_NODE: return "STATEMENT_NODE";
            case NodeType::LITERAL_NODE: return "LITERAL_NODE";
            case NodeType::LITERAL_MAP_NODE: return "LITERAL_MAP_NODE";
            case NodeType::LITERAL_LIST_NODE: return "LITERAL_LIST_NODE";
            case NodeType::LITERAL_PAIR_NODE: return "LITERAL_PAIR_NODE";
            case NodeType::STATEMENT_LIST_NODE: return "STATEMENT_LIST_NODE";
            case NodeType::VARIABLE_DECLARATION_NODE: return "VARIABLE_DECLARATION_NODE";
            case NodeType::VARIABLE_DEREFERENCE_NODE: return "VARIABLE_DEREFERENCE_NODE";
            case NodeType::IF_NODE: return "IF_NODE";
            case NodeType::CLAZZ_CONSTRUCTOR_EXPRESSION_NODE: return "CLAZZ_CONSTRUCTOR_EXPRESSION_NODE";
            case NodeType::TERNARY_OPERATOR_NODE: return "TERNARY_OPERATOR_NODE";
            case NodeType::SWITCH_NODE: return "SWITCH_NODE";
            case NodeType::SWITCH_CASE_CONSTANT_NODE: return "SWITCH_CASE_CONSTANT_NODE";
            case NodeType::UNARY_OPERATION_NODE: return "UNARY_OPERATION_NODE";
            case NodeType::COMPARE_OPERATION_NODE: return "COMPARE_OPERATION_NODE";
            case NodeType::LOG_NODE: return "LOG_NODE";
            case NodeType::FUNCTION_NODE: return "FUNCTION_NODE";
            case NodeType::METHOD_CALL_NODE: return "METHOD_CALL_NODE";
            case NodeType::FUNCTION_CALL_NODE: return "FUNCTION_CALL_NODE";
            case NodeType::VALUE_PLACEHOLDER_NODE: return "VALUE_PLACEHOLDER_NODE";
            case NodeType::LOOP_NODE: return "LOOP_NODE";
            case NodeType::CLAZZ_DECLARATION_NODE: return "CLAZZ_DECLARATION_NODE";
            case NodeType::INTERFACE_DECLARATION_NODE: return "INTERFACE_DECLARATION_NODE";
            case NodeType::OBJECT_CREATION_NODE: return "OBJECT_CREATION_NODE";
            case NodeType::SUPER_CONSTRUCTOR_CALL_NODE: return "SUPER_CONSTRUCTOR_CALL_NODE";
            case NodeType::CLAZZ_CONSTRUCTOR_NODE: return "CLAZZ_CONSTRUCTOR_NODE";
            case NodeType::ENUM_NODE: return "ENUM_NODE";
            case NodeType::ENUM_ACCESS_NODE: return "ENUM_ACCESS_NODE";
            case NodeType::ARRAY_ACCESS_NODE: return "ARRAY_ACCESS_NODE";
            case NodeType::OBJECT_ACCESS_NODE: return "OBJECT_ACCESS_NODE";
            case NodeType::MEMBER_ACCESS_NODE: return "MEMBER_ACCESS_NODE";
            case NodeType::TYPE_CONV_NODE: return "TYPE_CONV_NODE";
            case NodeType::EXCEPTION_HANDLE_NODE: return "EXCEPTION_HANDLE_NODE";
            case NodeType::ARRAY_NODE: return "ARRAY_NODE";
            case NodeType::BINARY_OPERATION_NODE: return "BINARY_OPERATION_NODE";
            case NodeType::PRIMARY_EXPR_NODE: return "PRIMARY_EXPR_NODE";
            case NodeType::IMPORT_STATEMENT_NODE: return "IMPORT_STATEMENT_NODE";
            case NodeType::QUALIFIED_FUNCTION_CALL_NODE: return "QUALIFIED_FUNCTION_CALL_NODE";
            case NodeType::QUALIFIED_IDENTIFIER_NODE: return "QUALIFIED_IDENTIFIER_NODE";
            case NodeType::STATIC_METHOD_CALL_NODE: return "STATIC_METHOD_CALL_NODE";
            case NodeType::ENUM_VALUE_EXPRESSION_NODE: return "ENUM_VALUE_EXPRESSION_NODE";
            default: return "UNKNOWN_NODE";
        }
    }    
}
#endif