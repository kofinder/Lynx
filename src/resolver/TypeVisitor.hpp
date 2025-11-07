/**
 * @file TypeVisitor.hpp
 * @brief Defines the visitor interface for traversing different type nodes in Lynx.
 *
 * The `TypeVisitor` class provides a visitor interface for various built-in and user-defined
 * types in the Lynx type system. It follows the Visitor design pattern, allowing different
 * operations to be performed on type instances without modifying the type classes themselves.
 *
 * Supported Type Nodes:
 * - Built-in types: ByteType, ShortType, IntegerType, LongType, FloatType, DoubleType, BooleanType, CharType, StringType.
 * - User-defined types: DateTimeType.
 * - Inferred types: AutoType.
 *
 * Usage Example:
 * @code
 * class PrintTypeVisitor : public LynxResolver::TypeVisitor {
 *     void visit(ByteType& type) override { std::cout << "ByteType\n"; }
 *     void visit(IntegerType& type) override { std::cout << "IntegerType\n"; }
 *     ...
 * };
 * 
 * ByteType b;
 * PrintTypeVisitor visitor;
 * b.accept(visitor); // hypothetical accept function in ByteType
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

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


namespace LynxResolver {

    using namespace LynxTypes;

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
