# 🦊 LynxCollection

A modular, header-only C++ collection library implementing standard data structures (Array, Vector, Stack, Queue, Map, Set, Tree, Graph, Dict) using clean design patterns:

- 🧱 **Adapter Pattern**
- 🏭 **Factory Pattern**
- 🧠 **Strategy Pattern**
- 🎭 **Facade Pattern** (optional)

...

## 🧪 Example Usage

```cpp
#include "factory/CollectionFactory.hpp"
#include "kinds/CollectionKind.hpp"

int main() {
    using namespace LynxCollection;

    auto vec = CollectionFactory::createCollection<int>(CollectionKind::VECTOR);
    vec->add(10);
    vec->add(20);

    if (vec->contains(20)) {
        std::cout << "20 is in the collection.\n";
    }

    vec->remove(10);
    std::cout << "Size: " << vec->size() << std::endl;

    return 0;
}
```

...
