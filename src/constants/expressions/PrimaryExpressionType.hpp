#ifndef LYNX_PRIMARY_EXPRESSION_TYPE_HPP  
#define LYNX_PRIMARY_EXPRESSION_TYPE_HPP  

namespace LynxConstants {
    enum PrimaryExpressionType {  
        SCALAR_EXPR = 1, 
        IDENTIFIER_EXPR = 2,
        PARENTHESIS_EXPR = 3,
        FUNCTION_CALL_EXPR = 4,
        ENUM_ACCESS_EXPR = 5,
        OBJECT_CREATION_EXPR = 6,
    };  
}
#endif
