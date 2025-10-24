#ifndef LYNX_TYPE_NESTING_LIMITER_HPP
#define LYNX_TYPE_NESTING_LIMITER_HPP

#include <iostream>

namespace LynxLang {

    class TypeNestingLimiter {

        int& depth;

        const int maxDepth;

        bool active = false;
    
    public:

        TypeNestingLimiter(int& depthRef, int maxDepthVal) : depth(depthRef), maxDepth(maxDepthVal) {
            if (++depth > maxDepth) {
                --depth;
                throw std::runtime_error("Exceeded maximum generic type nesting depth");
            }
            active = true;
        }

        ~TypeNestingLimiter() {
            if (active) --depth;
        }

    };

}

#endif 
