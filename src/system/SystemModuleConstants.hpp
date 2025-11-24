/**
 * @file SystemModuleConstants.hpp
 * @brief Defines constant identifiers for system modules and their methods.
 * 
 * This header provides a centralized set of constant string definitions used 
 * throughout the Lynx system to reference built-in modules and their methods 
 * consistently. It ensures type safety, avoids hardcoded string literals, and 
 * improves maintainability across module-related components.
 * 
 * **Defined Constants:**
 * - **Modules:** `io`, `time`, `net`
 * - **Methods:** `print`, `println`, `read`, `readInt`, `readLine`
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_MODULE_CONSTANTS_HPP
#define LYNX_SYSTEM_MODULE_CONSTANTS_HPP

namespace LynxSystem {

    // MODULES
    inline constexpr const char* MODULE_IO    = "io";
    inline constexpr const char* MODULE_TIME  = "time";
    inline constexpr const char* MODULE_NET   = "net";
    
    // METHODS
    inline constexpr const char* PRINT    = "print";
    inline constexpr const char* PRINTLN  = "println";
    inline constexpr const char* READ     = "read";
    inline constexpr const char* READINT  = "readInt";
    inline constexpr const char* READLINE = "readLine";

}

#endif
