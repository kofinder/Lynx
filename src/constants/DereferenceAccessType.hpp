
#ifndef LYNX_MEMBER_ACCESS_TYPE_HPP  
#define LYNX_MEMBER_ACCESS_TYPE_HPP  

namespace LynxConstants {

    enum DereferenceAccessType {
        ARRAY_ACCESS = 1,

        MEMBER_ACCESS = 2,

        VARIABLE_ACCESS = 3, 
    
        POINTER_DEREFERENCE = 4,

        FUNCTION_CALL = 5,

        THIS_ACCESS = 6,

        SUPER_ACCESS = 7,
        
        INDEXED_MEMBER_ACCESS = 8
    };

}

#endif
