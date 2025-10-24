#pragma once

#include <string>
#include <variant>

namespace LynxTypes {

    class EnumMember {

        public:

            using AllowType = std::variant<std::monostate, int, char, std::string>;

            int index = -1;

        private:

            std::string name;

            AllowType value;

        public:

            EnumMember() = default;

            explicit EnumMember(const std::string& enumName, int enumValue) : name(enumName), value(enumValue) {}

            explicit EnumMember(const std::string& enumName, char enumValue) : name(enumName), value(enumValue) {}

            explicit EnumMember(const std::string& enumName, const std::string& enumValue) : name(enumName), value(enumValue) {}
    
            inline bool hasValue() const {
                return !std::holds_alternative<std::monostate>(value);
            }
    
            inline bool isStringValue() const {
                return std::holds_alternative<std::string>(value);
            }
    
            inline bool isIntValue() const {
                return std::holds_alternative<int>(value);
            }

            inline bool isCharValue() const {
                return std::holds_alternative<char>(value);
            }

            inline const int getIndex() const { return index; }

            inline const std::string& getName() const { return name; }
            
            inline const AllowType& getValue() const { return value; }
    };

}