#ifndef LYNX_MAGICNUMERIC_CONSTANTS_HPP
#define LYNX_MAGICNUMERIC_CONSTANTS_HPP

namespace LynxConstants {
    // ---------------------------
    // Generic magic indices
    // ---------------------------
    constexpr unsigned INDEX_FIRST  = 0;  // Index of first element
    constexpr unsigned INDEX_SECOND = 1;  // Index of second element
    constexpr unsigned INDEX_THIRD  = 2;  // Index of third element
    
    // ---------------------------
    // Standard integer bit widths
    // ---------------------------
    constexpr unsigned BIT_WIDTH_BYTE  = 8;   // 8-bit integer
    constexpr unsigned BIT_WIDTH_SHORT = 16;  // 16-bit integer
    constexpr unsigned BIT_WIDTH_INT   = 32;  // 32-bit integer
    constexpr unsigned BIT_WIDTH_LONG  = 64;  // 64-bit integer

    // ---------------------------
    // Vector sizes and alignment
    // ---------------------------
    constexpr int SMALL_VECTOR_INITIAL_SIZE = 8;
    constexpr int VECTOR_DEFAULT_ALIGN_SIZE = 32;


    // ---------------------------
    // Numeric type promotion ranks
    // ---------------------------
    constexpr int RANK_ZERO   = 0;
    constexpr int RANK_ONE    = 1;
    constexpr int RANK_TWO    = 2;
    constexpr int RANK_THREE  = 3;
    constexpr int RANK_FOUR   = 4;
    constexpr int RANK_FIVE   = 5;
    constexpr int RANK_SIX    = 6;
    constexpr int RANK_SEVEN  = 7;

    // ---------------------------
    // Others
    // ---------------------------
    constexpr double FP_TWO = 2.0;
    constexpr uint32_t DEFAULT_ALIGN_BITS = 0;



}

#endif
