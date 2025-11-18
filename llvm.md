## 💡 Tip: Finding LLVM Header Files

Sometimes LLVM moves header files between versions, or you may need to locate them for your project. You can use the `find` command to quickly locate any header file.

### Basic Usage

```bash
# Find a specific header file
find /usr/local/llvm-21/include -name "Intrinsics.h"
