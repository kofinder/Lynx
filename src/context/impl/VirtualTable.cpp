#include "VirtualTable.hpp"
#include <logger/Logger.hpp>
#include <iostream>

namespace LynxContext {

    using namespace LynxLogger;

    void VirtualTable::setSignature(const std::string& mangledName, const std::string& signature) {
        // set/override mapping for this mangled symbol -> canonical signature
        mangledToSignature[mangledName] = signature;
    }


    std::string VirtualTable::getSignature(const std::string& mangledName) const {
        auto it = mangledToSignature.find(mangledName);
        if (it != mangledToSignature.end()) return it->second;

        // fallback: search parents
        auto p = parent;
        while (p) {
            auto pit = p->mangledToSignature.find(mangledName);
            if (pit != p->mangledToSignature.end()) return pit->second;
            p = p->parent;
        }
        return std::string();
    }

    void VirtualTable::addMethod(const std::string& mangledName, llvm::Value* function) {
        std::cout << "Added Method on virtual table ===>" << mangledName << std::endl;
        // Must have a signature available (either locally or in parent chain)
        std::string signature = getSignature(mangledName);
        if (signature.empty()) {
            throw std::runtime_error("VirtualTable::addMethod: canonical signature not set for '" + mangledName + "'");
        }

        // If signature already maps to a slot, override that slot.
        auto sigIt = signatureToSlot.find(signature);
        if (sigIt != signatureToSlot.end()) {
            unsigned slot = sigIt->second;
            if (slot >= slotOrder.size()) {
                throw std::runtime_error("VirtualTable::addMethod: inconsistent slot state (slot >= slotOrder.size())");
            }

            // Replace occupant mangled symbol for this slot
            std::string previousMangled = slotOrder[slot];
            slotOrder[slot] = mangledName;

            // Update method table
            methodTable[mangledName] = function;

            // Update signature mapping for this mangled symbol
            mangledToSignature[mangledName] = signature;

            // Remove stale mapping for previous mangled (if different)
            if (!previousMangled.empty() && previousMangled != mangledName) {
                methodTable.erase(previousMangled);
                mangledToSignature.erase(previousMangled);
            }
        } else {
            // New slot: append to end
            unsigned newSlot = static_cast<unsigned>(slotOrder.size());
            slotOrder.push_back(mangledName);
            signatureToSlot.emplace(signature, newSlot);
            methodTable[mangledName] = function;
            mangledToSignature[mangledName] = signature;
        }
    }

    llvm::Value* VirtualTable::getMethod(const std::string& mangledName) const {
        auto it = methodTable.find(mangledName);
        if (it != methodTable.end()) return it->second;

        // fallback: search parent chain
        auto p = parent;
        while (p) {
            auto pit = p->methodTable.find(mangledName);
            if (pit != p->methodTable.end()) return pit->second;
            p = p->parent;
        }

        return nullptr;
    }

    int VirtualTable::getMethodIndex(const std::string& mangledName) const {
        // Resolve canonical signature first
        std::string signature = getSignature(mangledName);
        if (!signature.empty()) {
            auto sit = signatureToSlot.find(signature);
            if (sit != signatureToSlot.end()) return static_cast<int>(sit->second);

            // fallback: check parent chain
            auto p = parent;
            while (p) {
                auto ps = p->signatureToSlot.find(signature);
                if (ps != p->signatureToSlot.end()) return static_cast<int>(ps->second);
                p = p->parent;
            }
        }

        // Final fallback: find exact mangled symbol in slotOrder
        for (unsigned i = 0; i < slotOrder.size(); ++i) {
            if (slotOrder[i] == mangledName) return static_cast<int>(i);
        }

        return -1;
    }

    void VirtualTable::inheritFrom(const std::shared_ptr<VirtualTable>& parentVTable) {
        if (!parentVTable) return;

        // Step 1: copy all parent slots in order
        for (unsigned slot = 0; slot < parentVTable->slotOrder.size(); ++slot) {
            const std::string& mangledName = parentVTable->slotOrder[slot];
            const std::string& signature = parentVTable->getSignature(mangledName);

            // Check if this signature already exists locally
            if (signatureToSlot.find(signature) != signatureToSlot.end()) continue;

            // Append to end of slotOrder
            unsigned newSlot = static_cast<unsigned>(slotOrder.size());
            slotOrder.push_back(mangledName);

            // Copy mappings
            signatureToSlot[signature] = newSlot;
            mangledToSignature[mangledName] = signature;

            // Copy function pointer (optional, can be nullptr if interface)
            llvm::Value* fnPtr = parentVTable->getMethod(mangledName);
            if (fnPtr) methodTable[mangledName] = fnPtr;
        }
    }


    int VirtualTable::getMethodIndexBySignature(const std::string& signature) const {
        auto sit = signatureToSlot.find(signature);
        if (sit != signatureToSlot.end()) return static_cast<int>(sit->second);

        auto p = parent;
        while (p) {
            auto ps = p->signatureToSlot.find(signature);
            if (ps != p->signatureToSlot.end()) return static_cast<int>(ps->second);
            p = p->parent;
        }
        return -1;
    }
}

