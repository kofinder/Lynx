# 🧠 Lynx Platform Module

This module provides the platform abstraction layer for Lynx, enabling the use of LLVM bitcode across architectures, OSes, and target types. It includes tools for bitcode representation, registration, and querying, used in the code generation pipeline.

---

## 📁 Directory Structure
```
src/platform/
├── Bitcode.hpp              # Bitcode abstraction header
├── TargetRegistry.hpp       # Target registration system
├── Triple.hpp               # Platform Triple abstraction
├── CMakeLists.txt           # Local build logic for this module
└── impl/
    ├── Bitcode.cpp          # Implementation of Bitcode class
    ├── TargetRegistry.cpp   # Implementation of TargetRegistry
    └── Triple.cpp           # Platform triple logic
```
---

## 📦 Key Classes

### 🔹 `Bitcode`

Represents a single bitcode module, either loaded from disk or embedded in memory.

**Usage Example:**

```cpp
#include <platform/Bitcode.hpp>

using namespace LynxConstants;
using namespace LynxPlatform;

const Bitcode* lib = new Bitcode("bitcode/builtins_x86_64.bc", TargetOSType::linux, ArchType::x86_64);
llvm::Module* mod = lib->getLLVMModule();
mod->print(llvm::outs(), nullptr);
```

### 🔹 `TargetRegistry`

Maintains an internal registry of available bitcode modules by OS, architecture, and target.

**Usage Example:**

```cpp
TargetRegistry* registry = TargetRegistry::getTargetLibRegistry();
TargetRegistry::RegisterTarget(lib);

const Bitcode* stdlib = registry->getStdLibray(LynxTargetType::avx2, TargetOSType::linux, ArchType::x86_64);
if (stdlib) {
    auto* mod = stdlib->getLLVMModule();
    // Use LLVM IR here...
}
```

---

## 🛠️ Build Instructions

Inside your root project `CMakeLists.txt`, make sure to include the platform module:

```cmake
add_subdirectory(src/platform)
```

In `src/platform/CMakeLists.txt`:

```cmake
add_library(platform
    Bitcode.hpp
    TargetRegistry.hpp
    Triple.hpp
    impl/Bitcode.cpp
    impl/TargetRegistry.cpp
    impl/Triple.cpp
)

target_include_directories(platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(platform PRIVATE LLVM) # Or use llvm-config
```

Make sure you have LLVM properly configured and `llvm-config` available.

---

## 🧬 How to Generate Bitcode (.bc) Files

```bash
# In your bitcode source directory:
clang -O2 -emit-llvm -c builtins.c -o builtins_x86_64.bc
clang -O2 -emit-llvm -c stdlib.c -o stdlib_x86_64.bc
```

Then register these in your app or test:

```cpp
Bitcode* builtin = new Bitcode("bitcode/builtins_x86_64.bc", TargetOSType::linux, ArchType::x86_64);
TargetRegistry::RegisterTarget(builtin);
```

---

## 🧪 Testing Support

```cpp
registry->printSupportMatrix();
```
