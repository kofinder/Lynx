#ifndef LYNX_ENUM_MEMBER_HELPER_HPP
#define LYNX_ENUM_MEMBER_HELPER_HPP

#include <string>
#include <variant>

namespace LynxTypes {

    struct EnumName { std::string name; };
    struct EnumValue { std::string value; };

    class EnumMember {

        public:

            using AllowType = std::variant<std::monostate, int, char, std::string>;

        private:

            int index = -1;

            std::string name;

            AllowType value;

        public:

            EnumMember() = default;

            explicit EnumMember(const std::string& enumName, int enumValue) : name(enumName), value(enumValue) {}

            explicit EnumMember(const std::string& enumName, char enumValue) : name(enumName), value(enumValue) {}

            explicit EnumMember(const std::string& enumName, const std::string& enumValue) : name(enumName), value(enumValue) {}
    
            [[nodiscard]] bool hasValue() const {return !std::holds_alternative<std::monostate>(value); }
    
            [[nodiscard]] bool isStringValue() const { return std::holds_alternative<std::string>(value); }
    
            [[nodiscard]] bool isIntValue() const { return std::holds_alternative<int>(value); }

            [[nodiscard]] bool isCharValue() const { return std::holds_alternative<char>(value); }

            [[nodiscard]] int getIndex() const { return index; }

            [[nodiscard]] const std::string& getName() const { return name; }
            
            [[nodiscard]] const AllowType& getValue() const { return value; }
    };

}
#endif