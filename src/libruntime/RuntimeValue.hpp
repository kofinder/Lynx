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



    // struct RuntimeFunction {

    //     std::string name;

    //     std::string llvmName;

    //     std::vector<llvm::Type*> args;

    //     llvm::Type* returnType;

    // };