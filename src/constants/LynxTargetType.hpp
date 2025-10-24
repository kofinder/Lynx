#ifndef LYNX_TARGET_TYPE_HPP
#define LYNX_TARGET_TYPE_HPP

#include <string>
#include <vector>
#include <utility>

namespace LynxConstants {

    enum class LynxTargetType {
        none,

        // x86 Host
        host,

        // SSE2
        sse2_i32x4,
        sse2_i32x8,

        // SSE4.1
        sse41_i8x16,
        sse41_i16x8,
        sse41_i32x4,
        sse41_i32x8,

        // SSE4
        sse4_i8x16,
        sse4_i16x8,
        sse4_i32x4,
        sse4_i32x8,

        // AVX1
        avx1_i32x4,
        avx1_i32x8,
        avx1_i32x16,
        avx1_i64x4,

        // AVX2
        avx2_i8x32,
        avx2_i16x16,
        avx2_i32x4,
        avx2_i32x8,
        avx2_i32x16,
        avx2_i64x4,

        // AVX2 VNNI
        avx2vnni_i32x4,
        avx2vnni_i32x8,
        avx2vnni_i32x16,

        // AVX512 Skylake
        avx512skx_x4,
        avx512skx_x8,
        avx512skx_x16,
        avx512skx_x32,
        avx512skx_x64,

        // AVX512 ICL
        avx512icl_x4,
        avx512icl_x8,
        avx512icl_x16,
        avx512icl_x32,
        avx512icl_x64,

        // AVX512 SPR
        avx512spr_x4,
        avx512spr_x8,
        avx512spr_x16,
        avx512spr_x32,
        avx512spr_x64,

        // AVX10.2
        avx10_2_x4,
        avx10_2_x8,
        avx10_2_x16,
        avx10_2_x32,
        avx10_2_x64,

        // ARM Neon
        neon_i8x16,
        neon_i8x32,
        neon_i16x8,
        neon_i16x16,
        neon_i32x4,
        neon_i32x8,

        // WebAssembly
        wasm_i32x4,

        // Intel Gen9
        gen9_x8,
        gen9_x16,

        // Xe-LP
        xelp_x8,
        xelp_x16,

        // Xe-HPG
        xehpg_x8,
        xehpg_x16,

        // Xe-HPC
        xehpc_x16,
        xehpc_x32,

        // Xe-LPG
        xelpg_x8,
        xelpg_x16,

        // Xe2 HPG
        xe2hpg_x16,
        xe2hpg_x32,

        // Xe2 LPG
        xe2lpg_x16,
        xe2lpg_x32,

        // Generic
        generic_i1x4,
        generic_i1x8,
        generic_i1x16,
        generic_i1x32,
        generic_i1x64,
        generic_i8x16,
        generic_i8x32,
        generic_i16x8,
        generic_i16x16,
        generic_i32x4,
        generic_i32x8,
        generic_i32x16,
        generic_i64x4,

        // Sentinel
        error,

        // Keep last
        COUNT
    };

    // Optional: Enum aliases or bitmask support can be added here

    // Operator to iterate through enum
    LynxTargetType operator++(LynxTargetType &value, int);

    // String conversion
    std::string parseTargetTypeToString(LynxTargetType target);

    // Target parsing
    LynxTargetType parseTarget(const std::string &target);
    std::pair<std::vector<LynxTargetType>, std::string> parseTarget(const char *target);

    // Query functions
    bool targetIsX86(LynxTargetType target);
    bool targetIsNeon(LynxTargetType target);
    bool targetIsWasm(LynxTargetType target);
    bool targetIsGen(LynxTargetType target);
    bool targetIsGeneric(LynxTargetType target);
} // namespace LynxConstants

#endif // LYNX_TARGET_TYPE_HPP
