#pragma once

#include <string>      // Needed for std::string  
#include <stdexcept>   // Optional: for extending standard exceptions (if needed)  
#include <sstream>  // For std::ostringstream  

namespace LynxExceptions{
    class BaseException {  
        protected:  
            std::string message;  // Changed variable name to 'message' for clarity  

        public:  
            // Constructor that initializes exception message  
            explicit BaseException(const std::string& exceptionMessage)  
                : message(exceptionMessage) {}  

            // Virtual method to return exception message  
            virtual std::string getMessage() const {  
                return message;  
            }  

            // Destructors are defined as virtual for proper cleanup if subclassed  
            virtual ~BaseException() = default;  
        };  

}
