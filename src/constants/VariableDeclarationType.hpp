#ifndef LYNX_VARIABLE_DECLARATION_TYPE
#define LYNX_VARIABLE_DECLARATION_TYPE

namespace LynxConstants {

    enum VariableDeclarationType {

        CLASS_DECLARATION,       // Declaring a class type variable

        STRUCT_DECLARATION,      // Declaring a struct type variable

        UNION_DECLARATION,       // Declaring a union type variable

        ENUM_DECLARATION,        // Declaring an enum type variable

        ARRAY_DECLARATION,       // Declaring an array variable

        POINTER_DECLARATION,     // Declaring a pointer variable

        REFERENCE_DECLARATION,   // Declaring a reference variable

        FUNCTION_DECLARATION,    // Declaring a function (not a variable but part of declarations)

        STATIC_DECLARATION,      // Declaring a static variable

        CONST_DECLARATION,       // Declaring a constant variable

        VOLATILE_DECLARATION,    // Declaring a volatile variable

        MUTABLE_DECLARATION,     // Declaring a mutable variable (inside classes)

        EXTERN_DECLARATION,      // Declaring an extern variable

        THREAD_LOCAL_DECLARATION,// Declaring a thread-local variable

        AUTO_DECLARATION,        // Declaring a variable with auto type deduction

        REGISTER_DECLARATION,    // Declaring a register variable (deprecated in modern C++)

        INLINE_DECLARATION,      // Declaring an inline function or variable (C++17)
        
        GLOBAL_DECLARATION,      // Declaring a global variable

        LOCAL_DECLARATION,       // Declaring a local (stack) variable

        MEMBER_DECLARATION,      // Declaring a member variable in a class/struct

        OTHERS,
    };
    
}

#endif
