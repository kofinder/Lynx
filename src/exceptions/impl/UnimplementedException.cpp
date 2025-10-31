#include "BaseException.hpp"

namespace LynxExceptions {
    class UnimplementedException : public BaseException {  
        protected:  
            int lineNumber;

        public:  
            UnimplementedException(const std::string& error, int lineNumber)  
                : BaseException(error), lineNumber(lineNumber) {}  
            std::string getMessage() const override {  
                std::ostringstream oss;  // Use ostringstream to construct the message  
                oss << "Error: " << message << " at line " << lineNumber;  
                return oss.str();  // Return the constructed message  
            }  
    };  
}
