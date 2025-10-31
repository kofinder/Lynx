#ifndef LYNX_MANGLAR_TYPE_HPP
#define LYNX_MANGLAR_TYPE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace LynxConstants {

    enum class ManglerType {
        ITANIUM,
        MICROSOFT,
        LYNX,
        __Count
    };

    inline ManglerType operator++(ManglerType& mType, int) {
        auto val = static_cast<int>(mType);
        val = (val + 1) % static_cast<int>(ManglerType::__Count);
        mType = static_cast<ManglerType>(val);
        return mType;
    }

    inline ManglerType parseStringToMangleType(const std::string& name) {
        if (name == "ItaniumMangler") return ManglerType::ITANIUM;
        if (name == "MicrosoftMangler") return ManglerType::MICROSOFT;
        if (name == "CustomMangler") return ManglerType::LYNX;
        throw std::invalid_argument("Unknown ManglerType: " + name);
    }

    inline std::string parseMangleTypeToString(ManglerType mType) {
        switch (mType) {
            case ManglerType::ITANIUM: return "ItaniumMangler";
            case ManglerType::MICROSOFT: return "MicrosoftMangler";
            case ManglerType::LYNX: return "CustomMangler";
            default: return "UnknownManglerType";
        }
    }

    // ------------------------------------------------------------------------

    enum class ManglerKind {
        CLASS,
        INTERFACE,
        FUNCTION, 
        MEMBER_FUNCTION,
        CONSTRUCTOR, 
        DESTRUCTOR, 
        VARIABLE, 
        TYPE, 
        NAMESPACE, 
        CLASSNAME, 
        OPERATOR, 
        TEMPLATE, 
        __Count // For ++ operator
    };

    inline ManglerKind operator++(ManglerKind& kind, int) {
        auto val = static_cast<int>(kind);
        val = (val + 1) % static_cast<int>(ManglerKind::__Count);
        kind = static_cast<ManglerKind>(val);
        return kind;
    }

    inline ManglerKind parseStringToMangleKind(const std::string& name) {
        if (name == "Class") return ManglerKind::CLASS;
        if (name == "Interface") return ManglerKind::INTERFACE;
        if (name == "Function") return ManglerKind::FUNCTION;
        if (name == "MemberFunction") return ManglerKind::MEMBER_FUNCTION;
        if (name == "Constructor") return ManglerKind::CONSTRUCTOR;
        if (name == "Destructor") return ManglerKind::DESTRUCTOR;
        if (name == "Variable") return ManglerKind::VARIABLE;
        if (name == "Type") return ManglerKind::TYPE;
        if (name == "Namespace") return ManglerKind::NAMESPACE;
        if (name == "Operator") return ManglerKind::OPERATOR;
        if (name == "Template") return ManglerKind::TEMPLATE;
        throw std::invalid_argument("Unknown ManglerKind: " + name);
    }

    inline std::string parseMangleKindToString(ManglerKind kind) {
        switch (kind) {
            case ManglerKind::FUNCTION: return "Function";
            case ManglerKind::MEMBER_FUNCTION: return "MemberFunction";
            case ManglerKind::CONSTRUCTOR: return "Constructor";
            case ManglerKind::DESTRUCTOR: return "Destructor";
            case ManglerKind::VARIABLE: return "Variable";
            case ManglerKind::TYPE: return "Type";
            case ManglerKind::NAMESPACE: return "Namespace";
            case ManglerKind::CLASS: return "Class";
            case ManglerKind::INTERFACE: return "Interface";
            case ManglerKind::OPERATOR: return "Operator";
            case ManglerKind::TEMPLATE: return "Template";
            default: return "UnknownManglerKind";
        }
    }

}

#endif
