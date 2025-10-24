#ifndef LYNX_VIRTUAL_TABLE_HPP
#define LYNX_VIRTUAL_TABLE_HPP 

#include <string>
#include <unordered_map>
#include <memory>
#include <llvm/IR/Value.h>
#include <types/userdefined/helper/MethodType.hpp>

namespace LynxContext {
    
    using namespace LynxTypes;

    class VirtualTable : public std::enable_shared_from_this<VirtualTable> {

        private:

            std::string className;

            std::shared_ptr<VirtualTable> parent;

            // mangled symbol -> implementation pointer (llvm::Value*)
            std::unordered_map<std::string, llvm::Value*> methodTable;

            // mangled symbol -> canonical signature (class-agnostic)
            std::unordered_map<std::string, std::string> mangledToSignature;

            // canonical signature -> slot index
            std::unordered_map<std::string, unsigned> signatureToSlot;

            // slot index -> mangled symbol occupying that slot
            std::vector<std::string> slotOrder;

        public:

            explicit VirtualTable(std::string clazzName) : className(std::move(clazzName)), parent(nullptr) {}

            explicit VirtualTable(
                std::string clazzName, 
                std::shared_ptr<VirtualTable> parent
            ) : className(std::move(clazzName)), parent(std::move(parent)) {
                if (parent) {
                    slotOrder = parent->slotOrder;
                    signatureToSlot = parent->signatureToSlot;
                    methodTable = parent->methodTable;
                    mangledToSignature = parent->mangledToSignature;
                }        
            }

            // Set canonical signature (class-agnostic) for a mangled symbol.
            // Example signature: "g(int) const" or "foo()" etc.
            void setSignature(const std::string& mangledName, const std::string& signature);

            // Get the canonical signature for a mangled symbol (searches parent chain).
            std::string getSignature(const std::string& mangledName) const;

            // Add a method implementation pointer (llvm::Value*). Must call setSignature first.
            void addMethod(const std::string& mangledName, llvm::Value* function);

            // Lookup method by mangledName (searches this table then parent).
            llvm::Value* getMethod(const std::string& mangledName) const;

            // Get the slot index for a mangled symbol (or -1 if not found).
            int getMethodIndex(const std::string& mangledName) const;

            // Get slot index by canonical signature (or -1 if not found).
            int getMethodIndexBySignature(const std::string& signature) const;

            /**
             * @brief Merge all slots and mappings from a parent vtable into this vtable.
             * 
             * Existing slots in this vtable are not overwritten unless a signature collision occurs.
             * Slots from the parent are appended if no collision occurs.
            */
            void inheritFrom(const std::shared_ptr<VirtualTable>& parentVTable);

            // Getters for introspection / codegen
            const std::vector<std::string>& getSlotOrder() const { return slotOrder; }
            const std::unordered_map<std::string, llvm::Value*>& getMethodTable() const { return methodTable; }
            const std::unordered_map<std::string, std::string>& getMangledToSignatureMap() const { return mangledToSignature; }
            const std::unordered_map<std::string, unsigned>& getSignatureToSlotMap() const { return signatureToSlot; }

            inline const std::string& getClassName() const { return className; }
            inline const std::shared_ptr<VirtualTable> getParent() const { return parent; }
    };

}

#endif
