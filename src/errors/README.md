
# LynxErrors

LynxErrors is a modular C++ error-handling library designed with long-term maintainability in mind. It uses the **Composite** and **Visitor** design patterns to provide a flexible structure for representing and operating on different kinds of compile-time and runtime errors.

---

## 📁 Folder Structure

```
src/errors/
├── includes/
│   ├── AllErrors.hpp
│   ├── CodeGenError.hpp
│   ├── ContextError.hpp
│   ├── LinkError.hpp
│   ├── ParserError.hpp
│   ├── RuntimeError.hpp
│   ├── SymbolError.hpp
│   ├── SyntaxError.hpp
│   ├── TypeError.hpp
│   └── CompositeError.hpp
├── LynxError.hpp
├── ErrorVisitor.hpp
├── LoggingVisitor.hpp
├── ErrorHelpers.hpp
```

---

## 🧠 Design Patterns Used

- **Composite Pattern**: To group errors together (e.g., `CompositeError` can contain multiple child errors).
- **Visitor Pattern**: To operate on different error types without modifying their classes (`LoggingVisitor`, `ErrorVisitor`).

---

---

## 🚀 Usage

### Creating and Handling Errors

```cpp
#include "includes/ParserError.hpp"
#include "includes/CompositeError.hpp"
#include "LoggingVisitor.hpp"
#include "ErrorHelpers.hpp"

using namespace LynxErrors;

int main() {
    auto composite = std::make_shared<CompositeError>();
    composite->add(std::make_shared<ParserError>("Unexpected token ';'"));
    composite->add(make_runtime_error("Division by zero"));

    LoggingVisitor logger;
    composite->accept(logger);
}
```

---

## 🧰 Helpers

Use `ErrorHelpers.hpp` to create errors quickly with helper functions like:

```cpp
std::shared_ptr<LynxError> make_runtime_error(const std::string& message);
```

You can extend this with more helper methods for other error types.

---

## 📌 Notes

- All error classes inherit from `LynxError` (which extends `std::exception`).
- Leaf nodes (like `ParserError`, `SyntaxError`) override `what()` and `accept()`.
- `CompositeError` holds multiple children and delegates visiting.

---

## 🏗️ Recommended Extensions

- JSON/YAML error serialization visitor.
- GUI logging visitor.
- Error IDs or source location tracking.
