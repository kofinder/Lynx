#ifndef LYNX_STATEMENT_TYPE_HPP  
#define LYNX_STATEMENT_TYPE_HPP  

/**  
 * @enum STATEMENT_TYPE  
 *   
 * @brief Enum representing various types of statements in the programming language.  
 *   
 * This enum captures the different categories of statements that can   
 * be encountered in a programming context. Organizing statements into   
 * enums assists in type safety, clarity, and maintainability of the code.  
 */  

namespace LynxConstants {

    enum StatementType {  
        EXPRESSION_STATEMENTS,   
        
        ASSIGNMENT_STATEMENTS,  
        
        BLOCK_STATEMENTS,  
        
        SYNCHRONIZED_STATEMENTS,  
        
        ASSERT_STATEMENTS,  
        
        CONDITIONAL_STATEMENTS,  
        
        LOOPING_STATEMENTS,  
        
        BRANCHING_STATEMENTS,  
        
        FUNCTION_DEFINITION_STATEMENTS,  
        
        CLASS_DEFINITION_STATEMENTS,  
        
        IMPORT_STATEMENTS,  
        
        WITH_STATEMENTS,  
        
        TRY_EXCEPT_STATEMENTS,  
        
        YIELD_STATEMENTS,

        RETURN_STATEMENTS,

        THROW_STATEMENTS
    };  
}
#endif // STATEMENT_TYPE_HPP  
