#ifndef LYNX_TYPE_RESOLVER_CONSTANT_HPP
#define LYNX_TYPE_RESOLVER_CONSTANT_HPP


namespace LynxResolver {

    enum class MethodName {
        // Type-level
        Max,
        Min,
        FromBool,
        FromDouble,
        FromString,

        // Instance-level - Arithmetic
        Add,
        Subtract,
        Multiply,
        Divide,
        Mod,

        // Instance-level - Bitwise (for integers)
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        BitwiseNot,
        ShiftLeft,
        ShiftRight,

        // Instance-level - Comparison
        Equals,
        GreaterThan,
        LessThan,
        Compare,

        // Utilities
        Abs,
        IsEven,
        IsOdd,
        IsPositive,
        IsNegative,
        Clamp,
        ToHex,
        ToBinary,
        ToString,

        Negate,
        Sign,
        Sqrt,
        Pow,
        SAddSat,
        UMulWithOverflow,
        Memcpy

    };

    static inline std::unordered_map<std::string, MethodName> MethodNameMap = {
        {"max", MethodName::Max},
        {"min", MethodName::Min},
        {"add", MethodName::Add},
        {"sub", MethodName::Subtract},
        {"mul", MethodName::Multiply},
        {"div", MethodName::Divide},
        {"mod", MethodName::Mod},
    };
    
}

#endif
