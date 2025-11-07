/**
 * @file UnimplementedException.hpp
 * @brief Defines the UnimplementedException class, used to signal unimplemented features in Lynx.
 * 
 * UnimplementedException extends BaseException and provides the line number context where
 * a feature is called but not yet implemented.
 * 
 * **Key Responsibilities:**
 * - Store a descriptive message indicating the unimplemented feature.
 * - Track the line number in the source code where the unimplemented feature was referenced.
 * - Provide a formatted error message including both the description and line number.
 * 
 * **Used By:**
 * - Compiler or interpreter components encountering incomplete or unimplemented functionality.
 * - Semantic checks that detect stubbed or placeholder code.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#include "BaseException.hpp"

namespace LynxExceptions {

    class UnimplementedException : public BaseException {  

        protected:  

            int lineNumber;

        public:  

            UnimplementedException(
                const std::string& error, 
                int lineNumber
            ) : BaseException(error), lineNumber(lineNumber) {}  

            std::string getMessage() const override {  
                std::ostringstream oss;
                oss << "Error: " << message << " at line " << lineNumber;  
                return oss.str();
            }  
    };  
}
