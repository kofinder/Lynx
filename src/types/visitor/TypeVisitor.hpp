/**
 * @file TypeVisitor.hpp
 * @brief Defines the visitor interface for all built-in Lynx types.
 *
 * This header declares the `TypeVisitor` abstract base class, which provides
 * a unified double-dispatch mechanism for operating on Lynx's built-in types
 * without relying on RTTI or manual type checks.
 *
 * Each built-in type (Byte, Short, Integer, Long, Float, Double,
 * Boolean, Char, String) implements an `accept(TypeVisitor&)` method
 * that invokes the corresponding `visit()` overload defined here.
 *
 * Benefits of the visitor pattern within the Lynx type system:
 *  - clean separation between type definitions and operations performed on them,
 *  - simplified extensibility: new operations only require new visitors,
 *  - consistent dispatch behavior across all built-in types,
 *  - strong compile-time guarantees via method overloading.
 *
 * Notes:
 *  - Adding a new built-in type requires adding a new visit() overload.
 *  - The destructor is virtual to ensure correct cleanup of derived visitors.
 *  - This interface is intentionally minimal and free of data members to
 *    maximize flexibility and avoid coupling visitors to state.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:   Jan 2, 2022
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

        TypeVisitor() = default; 

        virtual void visit(ByteType& type) = 0;

        virtual void visit(ShortType& type) = 0;

        virtual void visit(IntegerType& type) = 0;

        virtual void visit(LongType& type) = 0;

        virtual void visit(FloatType& type) = 0;

        virtual void visit(DoubleType& type) = 0;

        virtual void visit(BooleanType& type) = 0;

        virtual void visit(CharType& type) = 0;

        virtual void visit(StringType& type) = 0;
        
        // Rule of five
        TypeVisitor(const TypeVisitor&) = delete;
        TypeVisitor& operator=(const TypeVisitor&) = delete;
        TypeVisitor(TypeVisitor&&) = delete;
        TypeVisitor& operator=(TypeVisitor&&) = delete;
        virtual ~TypeVisitor() = default;
    };
}

#endif
