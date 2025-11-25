set(SKIA_INCLUDE_DIRS
    ${SKIA_ROOT}
    ${SKIA_ROOT}/include/core
    ${SKIA_ROOT}/include/config
    ${SKIA_ROOT}/include/gpu
    ${SKIA_ROOT}/include/utils
)

set(SKIA_LIBRARIES
    ${SKIA_OUT_DIR}/libskia.a
)

set(SKIA_SYSTEM_LIBS
    pthread
    m
    dl
    stdc++
)

set(SKIA_FOUND TRUE)
