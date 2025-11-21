TARGET_FILE="src/types/sequential/impl/ArrayType.cpp"
CLANG_TIDY_CONFIG=".clang-tidy"

clang-tidy $TARGET_FILE --config-file="$CLANG_TIDY_CONFIG"
