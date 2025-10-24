#ifndef LYNX_UNDEFINED_BEHAVIOR_HPP
#define LYNX_UNDEFINED_BEHAVIOR_HPP

/**
 * @brief Utility wrapper around LLVM IRBuilder to generate UB-aware IR safely.
 *
 * This header provides the `LynxAst::UB::Builder` class, which wraps LLVM's IRBuilder
 * and offers helper functions for creating operations that respect LLVM's undefined
 * behavior (UB) semantics. It centralizes creation of `undef`, `poison`, `freeze`,
 * and common arithmetic/memory operations that may trigger UB, ensuring consistent
 * and safe usage across the codebase.
 *
 * Motivation:
 * - Prevent misuse of LLVM APIs that can silently introduce poison or UB.
 * - Provide "safe" versions of common operations (e.g., division, select, store).
 * - Make code self-documenting and easier to audit for UB issues.
*/

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"

namespace LynxAst::UB {

    using namespace llvm;

    enum class IndeterminatePolicy { Undef, Poison };
    

    /**
     * @class Builder
     * @brief UB-aware wrapper around llvm::IRBuilder.
     *
     * Example usage:
     * @code
     * IRBuilder<> B(ctx);
     * LynxAst::UB::Builder UB(B);
     * Value *x = ...;
     * Value *y = UB.addNSW(x, B.getInt32(1));
     * Value *z = UB.freeze(y);
     * @endcode
    */
    class Builder {
        
        private:

            IRBuilder<>& B;

        public:

            Builder(IRBuilder<>& builder) : B(builder) {}

            /**
             * @brief Create an `undef` constant of the given type.
             *
             * `undef` represents an arbitrary value that may differ at each use.
             * Typically used for uninitialized values in IR.
             *
             * @param type LLVM type of the value.
             * @return An `undef` value of the specified type.
            */
            Value* getUndef(Type* type) { return UndefValue::get(type); }
        
            /**
             * @brief Create a `poison` constant of the given type.
             *
             * `poison` propagates silently through most operations until used
             * in UB-sensitive contexts (e.g., store, branch), at which point
             * it triggers undefined behavior.
             *
             * @param type LLVM type of the value.
             * @return A `poison` value of the specified type.
            */
            Value* getPoison(Type* type) { return PoisonValue::get(type); }
        
            /**
             * @brief Insert a `freeze` instruction.
             *
             * `freeze` converts a possibly `undef` or `poison` value into one
             * concrete, stable value (unique per freeze). This prevents UB
             * propagation and allows safe reuse of the value.
             *
             * @param value Input value (possibly undef/poison).
             * @param name  Optional name for the IR instruction.
             * @return Frozen (deterministic) value.
            */
            Value* freeze(Value* value, const Twine& name = "") {  return B.CreateFreeze(value, name); }
        
            /**
             * @brief Signed no-wrap addition (`nsw`).
             *
             * If signed overflow occurs, result becomes poison.
             *
             * @param lhs   Left-hand operand.
             * @param rhs   Right-hand operand.
             * @param name  Optional name for the IR instruction.
             * @return The result of `lhs + rhs` with NSW semantics.
            */
            Value* addNSW(Value* lhs, Value* rhs, const Twine& name = "") { return B.CreateNSWAdd(lhs, rhs, name); }
        
            /**
             * @brief Unsigned no-wrap addition (`nuw`).
             *
             * If unsigned overflow occurs, result becomes poison.
             *
             * @param lhs   Left-hand operand.
             * @param rhs   Right-hand operand.
             * @param name  Optional name for the IR instruction.
             * @return The result of `lhs + rhs` with NUW semantics.
            */
            Value* addNUW(Value* lhs, Value* rhs, const Twine& name = "") { return B.CreateNUWAdd(lhs, rhs, name); }
        
            /**
             * @brief Shift-left with both NSW and NUW flags set.
             *
             * If overflow occurs or a shift into the sign bit happens,
             * result becomes poison.
             *
             * @param lhs   Value to shift.
             * @param rhs   Shift amount.
             * @param name  Optional name for the IR instruction.
             * @return The result of `(lhs << rhs)` with NSW/NUW semantics.
            */
            Value* shlNSWNUW(Value* lhs, Value* rhs, const Twine& name = "") { return B.CreateShl(lhs, rhs, name, true, true); }
        
            /**
             * @brief Safe signed division.
             *
             * Freezes the divisor before division to prevent poison propagation.
             * Still UB if divisor is zero or lhs == INT_MIN and rhs == -1.
             *
             * @param lhs   Dividend.
             * @param rhs   Divisor (will be frozen).
             * @param name  Optional name for the IR instruction.
             * @return The result of `lhs / rhs` (signed division).
             */
            Value* safeSDiv(Value* lhs, Value* rhs, const Twine& name = "") { return B.CreateSDiv(lhs, freeze(rhs, "divisor_freezed"), name); }
            
            /**
             * @brief Safe signed add (NSW).
             *
             * Freezes both operands before addition, ensuring no poison
             * from inputs propagates into the result.
             *
             * @param lhs   Left-hand operand.
             * @param rhs   Right-hand operand.
             * @param name  Optional name for the IR instruction.
             * @return The result of `lhs + rhs` with NSW semantics.
            */
            Value* safeAddNSW(Value* lhs, Value* rhs, const Twine& name = "") { return addNSW(freeze(lhs, "lhs_frozen"), freeze(rhs, "rhs_frozen"), name); }

            /**
             * @brief Safe select.
             *
             * Freezes both possible values before selection, preventing
             * poison from the unchosen operand from affecting the result.
             *
             * @param cond  Condition.
             * @param tval  Value if cond is true (will be frozen).
             * @param fval  Value if cond is false (will be frozen).
             * @param name  Optional name for the IR instruction.
             * @return The result of `cond ? tval : fval`.
            */
            Value* safeSelect(Value* cond, Value* tval, Value* fval, const Twine& name = "") { return B.CreateSelect(cond, freeze(tval, "true_frozen"), freeze(fval, "false_frozen"), name); }

            /**
             * @brief Create a safe PHI node.
             *
             * Creates a PHI node where all incoming values should be frozen
             * by the caller before being added as incoming edges.
             *
             * @param type              The LLVM type of the PHI node.
             * @param numReservedValues Expected number of incoming values.
             * @param name              Optional name for the PHI node.
             * @return A PHINode with reserved capacity.
            */
            PHINode* safePhi(Type* type, unsigned numReservedValues, const Twine &name = "") { return B.CreatePHI(type, numReservedValues, name); }

            /**
             * @brief Safe store.
             *
             * Freezes the value before storing, preventing poison from being
             * written to memory. Still UB if pointer is poison or invalid.
             *
             * @param value     Value to store (will be frozen).
             * @param ptr       Destination pointer.
             * @param isVolatile Whether the store is volatile.
             * @return The created StoreInst.
            */
            StoreInst* safeStore(Value* value, Value* ptr, bool isVolatile = false) { return B.CreateStore(freeze(value, "store_frozen"), ptr, isVolatile); }
        
            /**
             * @brief Safe load.
             *
             * Performs a load and immediately freezes the result to prevent
             * poison propagation. Still UB if pointer is poison or invalid.
             *
             * @param type      Type of the value being loaded.
             * @param ptr       Pointer to load from.
             * @param name      Optional name for the load.
             * @param isVolatile Whether the load is volatile.
             * @return A frozen value loaded from memory.
            */
            Value* safeLoad(Type* type, Value* ptr, const Twine& name = "", bool isVolatile = false) { return freeze(B.CreateLoad(type, ptr, isVolatile, name), "load_frozen"); }


            /**
             * @brief Create an indeterminate value of the given type.
             *
             * The semantics of "indeterminate" vary by source language:
             * - C/C++ uninitialized variables → typically `undef`.
             * - SSA placeholders or "must not be observed" values → sometimes `poison`.
             *
             * This function delegates to either `getUndef` or `getPoison`
             * depending on the chosen policy.
             *
             * @param type   The LLVM type of the value.
             * @param policy Policy controlling whether to return `undef` or `poison`.
             *               Defaults to `IndeterminatePolicy::Undef`.
             * @return A new LLVM `Value*` representing an indeterminate value.
            */
            Value* makeIndeterminate(Type* type, IndeterminatePolicy policy = IndeterminatePolicy::Undef) {
                if(policy == IndeterminatePolicy::Poison) return getPoison(type);
                return getUndef(type);
            }
    };

}

#endif
