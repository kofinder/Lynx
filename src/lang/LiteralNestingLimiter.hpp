#ifndef LYNX_LITERAL_NESTING_LIMITER_HPP
#define LYNX_LITERAL_NESTING_LIMITER_HPP

#include <iostream>

namespace LynxLang {

    class LiteralNestingLimiter {

        int& depth;

        const int maxDepth;

        bool active = false;
    
    public:

        LiteralNestingLimiter(int& depthRef, int maxDepthVal) : depth(depthRef), maxDepth(maxDepthVal) {
            if (++depth > maxDepth) {
                --depth;
                throw std::runtime_error("Exceeded maximum literal nesting depth");
            }
            active = true;
        }

        ~LiteralNestingLimiter() {
            if (active) --depth;
        }
    };
}

#endif 
