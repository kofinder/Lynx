#include <iostream>
#include <limits> // For numeric limits

int main() {
    float num = 3.40282e+38;

    short x;

    // Check if num is within the range of short
    if (num > std::numeric_limits<short>::max()) {
        x = std::numeric_limits<short>::max(); // clamp to max
        std::cerr << "num is too large, clamping to max short value: ";
    } else if (num < std::numeric_limits<short>::min()) {
        x = std::numeric_limits<short>::min(); // clamp to min
        std::cerr << "num is too small, clamping to min short value: ";
    } else {
        x = static_cast<short>(num); // safe conversion
        std::cerr << "num is within range, converted value: ";
    }

    std::cerr << x << std::endl;

    return 0;
}
