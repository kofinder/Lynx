#ifndef LYNX_CLAZZ_TYPE_HPP  
#define LYNX_CLAZZ_TYPE_HPP  

namespace LynxConstants {

    enum class ClazzType {

        CLASS = 0,      // Regular class type

        INTERFACE = 1,  // Interface, defining method signatures

        RECORD = 2,     // Immutable data structure, introduced in newer versions of Java
        
        STRUCT = 3      // Struct, typically used for simple data aggregation 
    };

    inline int getClazzTypePrecedence(ClazzType type) {
        return static_cast<int>(type);
    }
} 

#endif
