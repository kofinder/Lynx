#ifndef LYNX_EXPRESSION_TYPE_HPP  
#define LYNX_EXPRESSION_TYPE_HPP  

namespace LynxConstants {
    enum ExpressionType {  

        VARIABLE_DE_REF_EXPR = 2,

        SUPER_CONSTRUCTOR_EXPR = 5,

        UNARY_EXPR = 7,

        GROUPED_EXPR = 8, 

        BINARY_EXPR = 9, 

        COMPARE_EXPR = 10,

        LOGICAL_EXPR = 11,

        TERNARY_EXPR = 12,

        ASSIGNMENT_EXPR = 13, 

        SWITCH_CASE_EXPR = 14,

        METHOD_CALL_EXPR = 15,
        
        OBJECT_ACCESS_EXPR = 16,

        PRIMARY_EXPR = 17,

        STATIC_METHOD_CALL_EXPR

    };  
}
#endif
