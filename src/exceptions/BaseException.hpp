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
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#pragma once

#include <string>  
#include <stdexcept> 
#include <sstream> 

namespace LynxExceptions{

    class BaseException {  

        protected:  

            std::string message;

        public:  

            explicit BaseException(const std::string& exceptionMessage) : message(exceptionMessage) {}  

            virtual std::string getMessage() const { return message; }  

            virtual ~BaseException() = default;  
    };  

}
