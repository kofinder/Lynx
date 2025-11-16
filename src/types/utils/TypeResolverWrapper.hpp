#ifndef LYNX_INT_METHOD_WRAPPERS_HPP
#define LYNX_INT_METHOD_WRAPPERS_HPP



namespace LynxTypes {

    // static const MethodMap<InstanceMethod> instanceMap {

    //     // Arithmetic Strategy
    //     {addKey, arithmetic.add(ctx, instance, args)},
    //     {subKey, impl.arithmetic.sub(ctx, instance, args)},
    //     {mulKey, impl.arithmetic.mul(ctx, instance, args)},
    //     {divKey, impl.arithmetic.div(ctx, instance, args)},
    //     {modKey, impl.arithmetic.mod(ctx, instance, args)},
    
    //     // Bitwise Strategy
    //     {bitAndKey, impl.bitwise.bitAnd(ctx, instance, args)},
    //     {bitOrKey, impl.bitwise.bitOr(ctx, instance, args)},
    //     {bitXorKey, impl.bitwise.bitXor(ctx, instance, args)},
    //     {bitNotKey, impl.bitwise.bitNot(ctx, instance, args)},
    //     {shlKey, impl.bitwise.shl(ctx, instance, args)},
    //     {shrKey, impl.bitwise.shr(ctx, instance, args)},
    
    //     // MinMax Strategy
    //     {sminKey, impl.minMaxStrat.smin(ctx, instance, args)},
    //     {smaxKey, impl.minMaxStrat.smax(ctx, instance, args)},
    //     {uminKey, impl.minMaxStrat.umin(ctx, instance, args)},
    //     {umaxKey, impl.minMaxStrat.umax(ctx, instance, args)},
    
    //     // Abs Strategy
    //     {absKey, impl.absStrat.abs(ctx, instance, args)},
    //     {negateKey, impl.absStrat.negate(ctx, instance, args)},
    //     {signKey, impl.absStrat.sign(ctx, instance, args)},
    //     {clampKey, impl.absStrat.clamp(ctx, instance, args)},
    //     {isEvenKey, impl.absStrat.isEven(ctx, instance, args)},
    //     {isOddKey, impl.absStrat.isOdd(ctx, instance, args)},
    
    //     // Comparison Strategy
    //     {eqKey, impl.cmpStrat.eq(ctx, instance, args,)},
    //     {neKey, impl.cmpStrat.ne(ctx, instance, args)},
    //     {ltKey, impl.cmpStrat.lt(ctx, instance, args)},
    //     {leKey, impl.cmpStrat.le(ctx, instance, args)},
    //     {gtKey, impl.cmpStrat.gt(ctx, instance, args)},
    //     {geKey, impl.cmpStrat.ge(ctx, instance, args)},
    
    //     // Math Strategy
    //     {sqrtKey, impl.mathStrat.sqrt(ctx, instance, args)},
    //     {powKey, impl.mathStrat.pow(ctx, instance, args)},
    //     {expKey, impl.mathStrat.exp(ctx, instance, args)},
    //     {exp2Key, impl.mathStrat.exp2(ctx, instance, args)},
    //     {exp10Key, impl.mathStrat.exp10(ctx, instance, args)},
    //     {logKey, impl.mathStrat.log(ctx, instance, args)},
    //     {log2Key, impl.mathStrat.log2(ctx, instance, args)},
    //     {log10Key, impl.mathStrat.log10(ctx, instance, args)},
    //     {sinKey, impl.mathStrat.sin(ctx, instance, args)},
    //     {cosKey, impl.mathStrat.cos(ctx, instance, args)},
    //     {tanKey, impl.mathStrat.tan(ctx, instance, args)},
    //     {asinKey, impl.mathStrat.asin(ctx, instance, args)},
    //     {acosKey, impl.mathStrat.acos(ctx, instance, args)},
    //     {atanKey, impl.mathStrat.atan(ctx, instance, args)},
    //     {atan2Key, impl.mathStrat.atan2(ctx, instance, args)},
    //     {sinhKey, impl.mathStrat.sinh(ctx, instance, args)},
    //     {coshKey, impl.mathStrat.cosh(ctx, instance, args)},
    //     {tanhKey, impl.mathStrat.tanh(ctx, instance, args)},
    //     {floorKey, impl.mathStrat.floor(ctx, instance, args)},
    //     {ceilKey, impl.mathStrat.ceil(ctx, instance, args)},
    //     {truncKey, impl.mathStrat.trunc(ctx, instance, args)},
    //     {roundKey, impl.mathStrat.round(ctx, instance, args)},
    //     {fabsKey, impl.mathStrat.fabs(ctx, instance, args)},
    
    //     // Memory Strategy
    //     {memcpyKey, impl.memStrat.memcpy(ctx, instance, args)},
    //     {memmoveKey, impl.memStrat.memmove(ctx, instance, args)},
    //     {memsetKey, impl.memStrat.memset(ctx, instance, args)},
    //     {memsetPatternKey, impl.memStrat.memsetPattern(ctx, instance, args)},
    
    //     // Bit Manipulation Strategy
    //     {popCountKey, impl.bitManipStrat.popCount(ctx, instance, args)},
    //     {countLeadingZerosKey, impl.bitManipStrat.countLeadingZeros(ctx, instance, args)},
    //     {countTrailingZerosKey, impl.bitManipStrat.countTrailingZeros(ctx, instance, args)},
    //     {bitReverseKey, impl.bitManipStrat.bitReverse(ctx, instance, args)},
    //     {rotateLeftKey, impl.bitManipStrat.rotateLeft(ctx, instance, args)},
    //     {rotateRightKey, impl.bitManipStrat.rotateRight(ctx, instance, args)},
    
    //     // Overflow Strategy
    //     {saddOverflowKey, impl.overflowStrat.saddWithOverflow(ctx, instance, args)},
    //     {uaddOverflowKey, impl.overflowStrat.uaddWithOverflow(ctx, instance, args)},
    //     {ssubOverflowKey, impl.overflowStrat.ssubWithOverflow(ctx, instance, args)},
    //     {usubOverflowKey, impl.overflowStrat.usubWithOverflow(ctx, instance, args)},
    //     {smulOverflowKey, impl.overflowStrat.smulWithOverflow(ctx, instance, args)},
    //     {umulOverflowKey, impl.overflowStrat.umulWithOverflow(ctx, instance, args)},
    
    //     // Saturation Strategy
    //     {saddSatKey, impl.saturationStrat.saddSat(ctx, instance, args)},
    //     {uaddSatKey, impl.saturationStrat.uaddSat(ctx, instance, args)},
    //     {ssubSatKey, impl.saturationStrat.ssubSat(ctx, instance, args)},
    //     {usubSatKey, impl.saturationStrat.usubSat(ctx, instance, args)},
    //     {sshlSatKey, impl.saturationStrat.sshlSat(ctx, instance, args)},
    //     {ushLSatKey, impl.saturationStrat.ushLSat(ctx, instance, args)},
    
    //     // FixedPoint Strategy
    //     {fpMulKey, impl.fixedPointStrat.mul(ctx, instance, args)},
    //     {fpUMulKey, impl.fixedPointStrat.umul(ctx, instance, args)},
    //     {fpDivKey, impl.fixedPointStrat.div(ctx, instance, args)},
    //     {fpUDivKey, impl.fixedPointStrat.udiv(ctx, instance, args)},
    // };

}

#endif
