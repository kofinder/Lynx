#ifndef LYNX_TYPE_SCORING_HELPER_HPP
#define LYNX_TYPE_SCORING_HELPER_HPP

#include "tmpl/TypeCaster.hpp"
#include <types/interfaces/BaseType.hpp>

namespace LynxTypes {

    static inline int64_t scoreExactMatch(BaseType* expected, const BaseType* actual) {
        constexpr int64_t EXACT_MATCH_SCORE = 10;
        return expected->equals(actual) ? EXACT_MATCH_SCORE : 0;
    }

    static inline int64_t scoreImplicitConversion(BaseType* expected, const BaseType* actual) {
        constexpr int64_t IMPLICIT_CONVERSION_SCORE = 5;
        return expected->canAccept(actual) ? IMPLICIT_CONVERSION_SCORE : 0;
    }

    static inline int64_t scoreClassHierarchy(const ClassType* expected, const BaseType* actual) {
        constexpr int64_t SCORE = 30;
        const auto* actualClass = TypeCasting::castType<const ClassType>(actual);
        return (actualClass && actualClass->isSubclassOf(expected)) ? SCORE : 0;
    }

    static inline int64_t scoreInterfaceConv(const InterfaceType* expected, const BaseType* actual) {
        constexpr int64_t SCORE = 30;
        const auto* actualClass = TypeCasting::castType<const ClassType>(actual);
        return (actualClass && actualClass->implementsInterface(expected)) ? SCORE : 0;
    }

    static inline int64_t scoreMixinConv(const MixinType* expected, const BaseType* actual) {
        constexpr int64_t SCORE = 30;
        const auto* actualClass = TypeCasting::castType<const ClassType>(actual);
        return (actualClass && actualClass->usesMixin(expected)) ? SCORE : 0;
    }

    static inline int64_t scoreSingleParameter(BaseType* expectedType, const BaseType* actualType) {
        if (!expectedType || !actualType)
            return -1;

        int64_t score = 0;

        score = scoreExactMatch(expectedType, actualType);
        if (score > 0) return score;

        score = scoreImplicitConversion(expectedType, actualType);
        if (score > 0) return score;

        if (const auto* expectedClass = TypeCasting::castType<const ClassType>(expectedType)) {
            score = scoreClassHierarchy(expectedClass, actualType);
            if (score > 0) return score;
        }

        if (const auto* expectedIface = TypeCasting::castType<const InterfaceType>(expectedType)) {
            score = scoreInterfaceConv(expectedIface, actualType);
            if (score > 0) return score;
        }

        if (const auto* expectedMixin = TypeCasting::castType<const MixinType>(expectedType)) {
            score = scoreMixinConv(expectedMixin, actualType);
            if (score > 0) return score;
        }

        return 0;
    }
}

#endif