# Calling Convention Inference Module

This module provides a flexible and extensible system for analyzing and applying LLVM calling conventions to functions during code generation or optimization. It is part of the **Lynx** project and designed to work seamlessly with LLVM IR.

---

## 📌 Purpose

LLVM supports many [calling conventions](https://llvm.org/docs/LangRef.html#calling-conventions), which affect how functions pass arguments, return values, and interact with the stack/registers. This module enables:

- 🧠 **Inference**: Determine the most appropriate calling convention based on function properties.
- ⚙️ **Application**: Apply the inferred or explicitly chosen convention to LLVM functions.
- 🧩 **Extensibility**: Add new specifications and heuristics without modifying core logic.

---

## 🏗️ Design Overview

This system follows the **Specification Pattern**, allowing complex matching logic to be composed in a modular way.

### Core Components

| File | Purpose |
|------|---------|
| `CallingConventionSpecification.hpp` | Abstract base class for all specifications. |
| `CallingConventionInferer.hpp` | Entry point for inference logic. Iterates over specifications. |
| `AndSpecification.hpp` | Composite spec: both child specs must match. |
| `HasAttributeSpec.hpp` | Matches based on LLVM function attributes (e.g., `"cold"`). |
| `IsVarArgSpec.hpp` | Matches variadic functions. |
| `includes/` | Individual calling convention spec implementations (e.g., `FastCallSpec`, `SwiftSpec`). |

Each spec overrides:

```cpp
bool isSatisfiedBy(llvm::Function* func) const;
CallingConventionType getCallingConvention() const;
```

---

## 🧪 Usage

### 1. Infer and Apply Convention

```cpp
#include "CallingConventionInferer.hpp"
#include "call_conv_template.hpp"

llvm::Function* func = ...;

LynxCallConv::CallingConventionInferer inferer;
auto inferredCC = inferer.infer(func);

LynxAst::applyCallingConvention(func, inferredCC);
```

### 2. Direct Application (with fallback inference)

```cpp
// Automatically infers and applies
LynxAst::applyInferredCallingConvention(func);
```

---

## 🧩 Add a New Calling Convention Rule

1. Create a spec class:

```cpp
// MyCustomSpec.hpp
class MyCustomSpec : public CallingConvSpecification {
public:
    bool isSatisfiedBy(llvm::Function* func) const override {
        return ...; // your logic
    }

    CallingConventionType getCallingConvention() const override {
        return CallingConventionType::YourType;
    }
};
```

2. Register it in `CallingConventionInferer`:

```cpp
specs.push_back(std::make_unique<MyCustomSpec>());
```

Done! Your spec is now used during inference.

---

## ✅ Advantages

- 🔄 **Composable**: Easily combine rules using `AndSpecification`.
- 🔧 **LLVM-Native**: Works directly with `llvm::Function*`.
- 🛠️ **Tooling Ready**: Designed for integration with optimizers, JITs, or frontends.

---

## 📂 Directory Structure

```
callconv/
├── CallingConventionInferer.hpp        # Core inference loop
├── CallingConventionSpecification.hpp  # Base spec interface
├── AndSpecification.hpp                # Logical AND of two specs
├── HasAttributeSpec.hpp                # Matches function attribute
├── IsVarArgSpec.hpp                    # Matches vararg functions
├── includes/
│   ├── CFGuardCheckSpec.hpp
│   ├── ColdCallingConvSpec.hpp
│   ├── CXXFastTLSSpec.hpp
│   ├── ...
```

---

## 📎 Related Modules

- `call_conv_template.hpp`: Maps `CallingConventionType` to LLVM's `llvm::CallingConv::ID` and provides `applyCallingConvention()` helpers.

---
