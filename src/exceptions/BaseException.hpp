/**
 * @file BaseException.hpp
 * @brief Defines the BaseException class, the root of all custom exceptions in the Lynx project.
 * 
 * BaseException provides a common interface for all runtime and compile-time exceptions.
 * It stores an error message and allows derived exceptions to provide additional context.
 * 
 * **Key Responsibilities:**
 * - Encapsulate an error message string.
 * - Serve as the base class for all custom exception types.
 * - Provide a consistent interface for retrieving the message.
 * 
 * **Used By:**
 * - Derived exception classes for runtime, semantic, and compilation errors.
 * - Any component requiring structured exception handling.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_BASE_EXCEPTIONS_HPP
#define LYNX_BASE_EXCEPTIONS_HPP

#include <string>  
#include <stdexcept> 
#include <sstream> 

namespace LynxExceptions{

    class BaseException {  

        private:  

            std::string message;

        public:  

            explicit BaseException(const std::string& exceptionMessage) : message(exceptionMessage) {}  

            // Rule of Five compliance
            BaseException(const BaseException&) = default;
            BaseException(BaseException&&) noexcept = default;
            BaseException& operator=(const BaseException&) = default;
            BaseException& operator=(BaseException&&) noexcept = default;

            [[nodiscard]] virtual std::string getMessage() const { return message; }  

            virtual ~BaseException() = default;  
    };  

}

#endif