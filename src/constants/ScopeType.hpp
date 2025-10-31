#ifndef LYNX_SCOPE_TYPE_HPP  
#define LYNX_SCOPE_TYPE_HPP  

namespace LynxConstants {
    // Enum representing different types of scopes in the code  
    enum ScopeType {  
        FUNCTION_DECL, // Represents a function declaration scope  
        CODE_BLOCK,    // Represents a block of code (e.g., within a function)  
        GLOBAL_BLOCK   // Represents a global scope (e.g., global variables)  
    };  
}
#endif // SCOPE_TYPE_HPP  
