/**
 * @file lynx_stdapi_gc_runtime.h
 * @brief Declares the Lynx garbage collection (GC) runtime allocation functions.
 * 
 * This header provides a set of C ABI-compatible functions to allocate memory managed
 * by the Lynx runtime garbage collector. It supports generic allocations as well as
 * allocations with type information for runtime type tracking.
 * 
 * **Key Responsibilities:**
 * - Allocate memory for GC-managed objects.
 * - Optionally associate a type name with allocated objects for runtime type safety.
 * - Provide a consistent interface callable from LLVM IR-generated code.
 * 
 * **Used By:**
 * - The Lynx compiler’s code generator when creating objects that are garbage-collected.
 * - The Lynx runtime system for managing memory of objects with automatic garbage collection.
 * 
 * **Function Overview:**
 * - `LYNX_GC_ALLOC_GENERIC(size)` – Allocates a generic GC-managed object of given size.
 * - `LYNX_GC_ALLOC_TYPED(size, typeName)` – Allocates a GC-managed object and records its type.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_STDAPI_GC_RUNTIME_H
#define LYNX_STDAPI_GC_RUNTIME_H

#include <cstdint>


#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Allocate a GC-managed object (generic).
    */
    void* LYNX_GC_ALLOC_generic(uint64_t size) noexcept;

    /**
     * @brief Allocate a GC-managed object and record its type.
    */
    void* LYNX_GC_ALLOC_typed(uint64_t size, const char* typeName) noexcept;


#ifdef __cplusplus
}
#endif

#endif