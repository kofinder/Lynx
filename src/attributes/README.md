
# LLVM Function Attribute Inferer

This project provides a modular and extensible system to infer and apply LLVM function attributes using a chain-of-responsibility pattern.  
It allows you to analyze LLVM functions and automatically attach safe or optimized function attributes to functions, return values, and parameters.

---

## Project Structure

```
├── CMakeLists.txt
├── handler
│   ├── AlwaysInlineHandler.hpp
│   ├── ArgMemOnlyHandler.hpp
│   ├── AssumeSameAlignedHandler.hpp
│   ├── DereferenceableParamHandler.hpp
│   ├── InlineHintHandler.hpp
│   ├── IntrinsicHandler.hpp
│   ├── NoCaptureHandler.hpp
│   ├── NoDeadStripHandler.hpp
│   ├── NoInlineAsmHandler.hpp
│   ├── NoInlineHandler.hpp
│   ├── NoInlineHintHandler.hpp
│   ├── NoRecurseHandler.hpp
│   ├── NoReturnHandler.hpp
│   ├── NoSanitizeAddressHandler.hpp
│   ├── NoSanitizeMemoryHandler.hpp
│   ├── NoSanitizeThreadHandler.hpp
│   ├── NoUnwindHandler.hpp
│   ├── OptimizeForSizeHandler.hpp
│   ├── ReadNoneHandler.hpp
│   ├── ReadOnlyHandler.hpp
│   ├── ReturnsTwiceHandler.hpp
│   ├── SafeDefaultsHandler.hpp
│   ├── SanitizeAddressHandler.hpp
│   ├── SanitizeCoverageHandler.hpp
│   ├── SanitizeHWAddressHandler.hpp
│   ├── SanitizeMemoryHandler.hpp
│   ├── SanitizeThreadHandler.hpp
│   ├── SignExtendHandler.hpp
│   ├── StackProtectorHandler.hpp
│   ├── StrictFPHandler.hpp
│   ├── UWTableHandler.hpp
│   └── ZeroExtendHandler.hpp
└── interfaces
    ├── FunctionAttributeBuilder.hpp
    ├── FunctionAttributeHandler.hpp
    ├── FunctionAttributeInferer.hpp
    └── impl
        ├── FunctionAttributeBuilder.cpp
        └── FunctionAttributeInferer.cpp
```

---

## Overview

### Core Components

- **FunctionAttributeBuilder**: Helper class to accumulate and apply attributes on functions, their return values, and parameters.
- **FunctionAttributeHandler**: Abstract base class for attribute inference logic, allowing easy composition of attribute application handlers in a chain.
- **FunctionAttributeInferer**: Orchestrates the chain of handlers to infer and apply attributes to LLVM functions.

### Handlers

Located in the `handler/` directory, each handler implements a specific attribute inference rule (e.g., `DereferenceableParamHandler` applies `dereferenceable` attributes to function parameters). Handlers follow the chain-of-responsibility design pattern and operate independently.

---

## Usage

### 1. Setup the Chain of Handlers

Create and chain attribute handlers according to your inference needs. For example:

```cpp
#include "interfaces/FunctionAttributeInferer.hpp"
#include "handler/DereferenceableParamHandler.hpp"
#include "handler/NoUnwindHandler.hpp"

// Create inferer
LynxFunctionAttr::FunctionAttributeInferer inferer;

// Setup handlers chain manually (example)
auto derefHandler = std::make_unique<LynxFunctionAttr::DereferenceableParamHandler>();
auto noUnwindHandler = std::make_unique<LynxFunctionAttr::NoUnwindHandler>();

derefHandler->setNext(std::move(noUnwindHandler));
inferer.setHead(std::move(derefHandler));
```

### 2. Infer and Apply Attributes on a Function

```cpp
void processFunction(llvm::Function* func) {
    inferer.inferAndApply(func);
}
```

### 3. Inside Handlers

Handlers implement the `apply()` method to add attributes to the builder:

```cpp
void DereferenceableParamHandler::apply(llvm::Function* func, FunctionAttributeBuilder& builder) {
    // Add dereferenceable(8) attribute to parameter 0
    builder.addAttributeAtParam(llvm::Attribute::Dereferenceable, 0);
}
```

### 4. Build and Apply Attributes

The `FunctionAttributeBuilder` collects attributes during handler traversal and applies them once to the LLVM function when `apply()` is called.

---

## Integration

- Designed to be integrated into LLVM passes or custom tooling.
- The system requires LLVM headers and libraries.
- Uses C++17 or later.

