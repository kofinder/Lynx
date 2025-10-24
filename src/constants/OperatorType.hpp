#ifndef LYNX_OPERATOR_TYPE_HPP  
#define LYNX_OPERATOR_TYPE_HPP  

/**  
 * @enum OPERATOR_TYPE  
 *   
 * @brief Enum representing different types of operators in the programming language.  
 *   
 * This enum categorizes the various operators that can be used in expressions within the   
 * programming language. Each operator represents a specific operation, contributing to   
 * the evaluation and manipulation of variables and expressions.  
 */  

namespace LynxConstants {

    enum OperatorType {          
        PLUS,  
        
        MINUS,  
        
        MUL,  
        
        DIV,  

        LSHIFT,

        RSHIFT,

        BIT_AND,

        BIT_OR,
    
        GREATER_THAN,  
        
        LESS_THAN,  
        
        GREATER_THAN_EQUAL,  
        
        LESS_THAN_EQUAL,  
        
        EQUALITY,  
        
        NOT_EQUALITY,  
        
        INCREMENT, 

        DECREMENT, 

        LOGICAL_NOT,  

        BITWISE_XOR,
        
        UNARY_PLUS,  
        
        UNARY_MINUS,  
        
        MODULO_DIV,  
        
        LOGICAL_AND,  
        
        LOGICAL_OR,  
    
        PLUS_ASSIGN,

        MINUS_ASSIGN,

        MULTIPLY_ASSIGN,

        DIVIDE_ASSIGN,

        MODULO_ASSIGN,

        LSHIFT_ASSIGN,

        RSHIFT_ASSIGN,

        AND_ASSIGN,

        XOR_ASSIGN,

        NULL_COALESCE,

        NONE // Default case

    };  
}
#endif // OPERATOR_TYPE_HPP