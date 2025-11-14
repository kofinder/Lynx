/**
 * @file TypeVisitor.hpp
 * @brief Defines the visitor interface for traversing different type nodes in Lynx.
 *
 * The `TypeVisitor` class provides a visitor interface for various built-in and user-defined
 * types in the Lynx type system. It follows the Visitor design pattern, allowing different
 * operations to be performed on type instances without modifying the type classes themselves.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_VISITOR_HPP
#define LYNX_TYPE_VISITOR_HPP

#include <types/builtins/ByteType.hpp>
#include <types/builtins/ShortType.hpp>
#include <types/builtins/IntegerType.hpp>
#include <types/builtins/LongType.hpp>
#include <types/builtins/FloatType.hpp>
#include <types/builtins/DoubleType.hpp>
#include <types/builtins/BooleanType.hpp>
#include <types/builtins/CharType.hpp>
#include <types/builtins/StringType.hpp>

namespace LynxTypes {

    struct TypeVisitor {

        virtual void visit(ByteType& type) = 0;

        virtual void visit(ShortType& type) = 0;

        virtual void visit(IntegerType& type) = 0;

        virtual void visit(LongType& type) = 0;

        virtual void visit(FloatType& type) = 0;

        virtual void visit(DoubleType& type) = 0;

        virtual void visit(BooleanType& type) = 0;

        virtual void visit(CharType& type) = 0;

        virtual void visit(StringType& type) = 0;

        virtual ~TypeVisitor() = default;
        
    };
}

#endif
