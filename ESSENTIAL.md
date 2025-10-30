# C++ Lambda Expressions

If a C++ developer does not know how to use lambdas, that could be a reason for difficulty in modern C++ coding.

### Syntax
```cpp
[ captures ] ( parameters ) -> return_type {
    body
};
```

### Components

1. **captures** → Which local variables the lambda can access.
    - `[=]` → Capture all used variables by value.
    - `[&]` → Capture all used variables by reference.
    - `[x, &y]` → Capture specific variables.
2. **parameters** → Like a normal function’s arguments.
3. **return_type** → Optional; compiler can often deduce it.
4. **body** → The code to execute.

### Notes

- Lambdas are **anonymous functions** defined inline.
- Capture allows accessing outside variables without making them global.
- `[=]` is for **read-only copies**, `[&]` to **modify variables**.
- Lambdas can be **passed to functions** or **stored in variables**.

---

# std::function Expression

### Definition

- A **type-erased callable wrapper**.
- Can store **any function, lambda, or functor** with the same signature.

### Key Points

- Useful for **callbacks** or **polymorphic function storage**.
- Must match the signature exactly:  
  ```cpp
  std::function<return_type(params)>
  ```
- Can store **lambdas with captures**, **function pointers**, or **functor objects**.
- Allows assigning **different callables** to the same variable.

### Tips

- Use **lambdas** for short, inline callbacks.
- Use **std::function** when you need to **store or pass lambdas polymorphically**.
- Choose capture mode carefully (`[=]` vs `[&]`) to avoid mistakes.
