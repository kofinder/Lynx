#ifndef LYNX_LVALUE_TYPE_HPP  
#define LYNX_LVALUE_TYPE_HPP  

/**  
 * @union LValueType  
 *   
 * @brief Union representing different types of l-values in the programming language.  
 *   
 * This union allows the storage of multiple types of l-values (left-hand values)   
 * in a single variable, providing flexible memory usage. The union can hold   
 * an integer, double, boolean, or string value, but only one value at a time.  
 */ 

#include <variant>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <runtimetypes/DateDataType.hpp>
#include <runtimetypes/FileDataType.hpp>
#include <runtimetypes/DateTimeDataType.hpp>

namespace LynxConstants {

    using LValueType = std::variant<
        uint8_t, 
        short, 
        int, 
        long, 
        float, 
        double, 
        bool, 
        char, 
        std::string, 
        std::nullptr_t,
        Date,
        DateTime,
        File
    >;
} 

#endif
