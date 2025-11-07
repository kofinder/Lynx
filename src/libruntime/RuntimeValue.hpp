/**
 * @file RuntimeValue.hpp
 * @brief Defines the RuntimeValue class representing runtime-evaluated values 
 *        in the Lynx language execution environment.
 * 
 * The RuntimeValue class provides a lightweight abstraction for representing 
 * different kinds of values at runtime, including strings, booleans, void values, 
 * and file handles. It is used primarily by the Lynx runtime system to store and 
 * manipulate evaluation results in the interpreter or execution engine.
 * 
 * **Key Responsibilities:**
 * - Encapsulate values such as strings, booleans, and file streams.
 * - Provide convenient accessors (`asString`, `asBool`, `asFile`) for value retrieval.
 * - Identify the runtime type of stored data via state flags (`isVoid`, `isFile`).
 * 
 * **Design Notes:**
 * - `RuntimeValue` acts as a polymorphic container for primitive and file-based 
 *   runtime entities.
 * - A void state (`_void`) indicates absence of a value (e.g., return type `void`).
 * - File values are managed using `std::shared_ptr<std::ofstream>` for resource safety.
 * 
 * **Used By:**
 * - Lynx interpreter components.
 * - The runtime evaluation and I/O subsystems.
 * 
 * @namespace LynxLibRuntime
 * Provides runtime library support utilities and value containers used during 
 * Lynx program execution.
 * 
 * @see LynxInterpreter, LynxRuntimeEnvironment
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_LIB_RUNTIME_VALUE_HPP
#define LYNX_LIB_RUNTIME_VALUE_HPP

#include <string>
#include <vector>
#include <memory>

namespace LynxLibRuntime {

    class RuntimeValue {

        private:
        
            std::string _str;
            bool _bool = false;
            bool _void = false;
            bool _isFile = false;
            std::shared_ptr<std::ofstream> _file;

        public:

            RuntimeValue() : _void(true) {}
            RuntimeValue(const std::string& s) : _str(s), _void(false) {}
            RuntimeValue(bool b) : _bool(b), _void(false) {}
            RuntimeValue(std::shared_ptr<std::ofstream> f) : _file(f), _void(false), _isFile(true) {}
    
            std::string asString() const { return _str; }
            bool asBool() const { return _bool; }
            std::shared_ptr<std::ofstream> asFile() const { return _file; }
    
            bool isVoid() const { return _void; }
            bool isFile() const { return _isFile; }
    };
   
}

#endif 
