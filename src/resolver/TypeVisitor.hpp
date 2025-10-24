#ifndef LYNX_TYPE_VISITOR_HPP
#define LYNX_TYPE_VISITOR_HPP

#include <types/infrerence/AutoType.hpp>
#include <types/builtins/ByteType.hpp>
#include <types/builtins/ShortType.hpp>
#include <types/builtins/IntegerType.hpp>
#include <types/builtins/LongType.hpp>
#include <types/builtins/FloatType.hpp>
#include <types/builtins/DoubleType.hpp>
#include <types/builtins/BooleanType.hpp>
#include <types/builtins/CharType.hpp>
#include <types/builtins/StringType.hpp>
#include <types/userdefined/DateTimeType.hpp>

using namespace LynxTypes;

namespace LynxResolver {

    /**
     * @class TypeVisitor
     * @brief Interface for visiting different types in the Lynx type system.
     *
     * Each method should implement behavior for the corresponding concrete type.
     * Used for code generation, method resolution, type checking, etc.
    */
    class TypeVisitor {

        public:

            /// Visit a ByteType node
            virtual void visit(ByteType& type) = 0;

            /// Visit a ShortType node
            virtual void visit(ShortType& type) = 0;

            /// Visit an IntegerType node
            virtual void visit(IntegerType& type) = 0;

            /// Visit a LongType node
            virtual void visit(LongType& type) = 0;

            /// Visit a FloatType node
            virtual void visit(FloatType& type) = 0;

            /// Visit a DoubleType node
            virtual void visit(DoubleType& type) = 0;

            /// Visit a BooleanType node
            virtual void visit(BooleanType& type) = 0;

            /// Visit a CharType node
            virtual void visit(CharType& type) = 0;

            /// Visit a StringType node
            virtual void visit(StringType& type) = 0;

            /// Visit a DateTimeType node
            virtual void visit(DateTimeType& type) = 0;

            /// Visit a AutoType node
            virtual void visit(AutoType& type) = 0;

            virtual ~TypeVisitor() = default;
    };
}

#endif
