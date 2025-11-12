
# LLVM Function Attribute Inferer

This project provides a modular and extensible system to infer and apply LLVM function attributes using a chain-of-responsibility pattern.  
It allows you to analyze LLVM functions and automatically attach safe or optimized function attributes to functions, return values, and parameters.

---

## Overview

### Core Components

- **FunctionAttributeBuilder**: Helper class to accumulate and apply attributes on functions, their return values, and parameters.
- **FunctionAttributeHandler**: Abstract base class for attribute inference logic, allowing easy composition of attribute application handlers in a chain.
- **FunctionAttributeInferer**: Orchestrates the chain of handlers to infer and apply attributes to LLVM functions.

### Handlers

Located in the `handler/` directory, each handler implements a specific attribute inference rule (e.g., `DereferenceableParamHandler` applies `dereferenceable` attributes to function parameters). Handlers follow the chain-of-responsibility design pattern and operate independently.

---