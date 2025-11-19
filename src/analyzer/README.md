# 🧠 Lynx Semantic Analyzer

This module implements the **semantic analysis system** for the Lynx compiler using the **State Pattern**. Each scope or semantic domain (functions, classes, arrays, etc.) is handled by its own `SemanticState`, enabling context-sensitive analysis.

---

## 🧩 Core Components

### `SemanticAnalyzer`

- Central coordinator that delegates semantic checks to the active state
- Maintains the **symbol table**
- APIs like:
  - `handleVariableDeclaration(name, type)`
  - `validateFunctionCall(name, args)`
  - `enterScope() / exitScope()`

### `SemanticState`

- Abstract interface for all semantic states
- Defines behavior for:
  - Declarations
  - Assignments
  - Function calls
  - Scopes
  - Type checks

### State Implementations

| State Class        | Description                             |
|--------------------|-----------------------------------------|
| `GlobalState`      | Root state for top-level declarations   |
| `FunctionState`    | Validates function bodies, parameters   |
| `ClassState`       | Handles member declarations & inheritance |
| `TemplateState`    | Processes template type semantics       |
| `EnumState`        | Checks enum members and constants       |
| `ArrayState`       | Validates array usage and indexing      |
| `CollectionState`  | Analyzes map/set/list operations        |
| `DateTimeState`    | Validates `DateTime`-specific functions |
| `IOState`          | Checks I/O-related constructs           |
| `ScopeState`       | Used for generic block-level scoping    |

---

## ✅ Design Patterns Used

| Pattern                | Purpose                                   | Usage                                 |
|------------------------|-------------------------------------------|----------------------------------------|
| State Pattern          | Swap semantic logic depending on context  | `SemanticState` subclasses             |
| Interface Segregation | Different states for different domains     | Type-based class split                 |
| Strategy               | Delegation to the active state's logic    | `SemanticAnalyzer::setState()`         |
| Factory (optional)     | You can build a `SemanticStateFactory`    | Future extensibility                   |

---
