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
        ToString

    };
    
}

#endif
