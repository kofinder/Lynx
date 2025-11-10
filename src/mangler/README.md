
# 🧩 Lynx Name Mangler Module

This module provides a pluggable and extensible system for generating mangled names for various language entities, such as functions, classes, variables, etc.

## 📁 Directory Structure

```
src/mangler/
├── IMangleStrategy.hpp       # Interface for all mangling strategies
├── ItaniumMangler.hpp        # Itanium ABI mangler
├── ManglerFactory.hpp        # Factory to produce manglers based on type
├── ManglingEngine.hpp        # Unified entry point for mangling
├── NameManglingContext.hpp   # Context class to set and use mangler strategies
└── kinds/
    ├── ItaniumMangler.cpp
    ├── MicrosoftMangler.cpp
    └── LynxMangler.hpp
```