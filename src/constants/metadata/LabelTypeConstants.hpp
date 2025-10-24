#ifndef LYNX_LABEL_TYPE_CONSTANTS_HPP
#define LYNX_LABEL_TYPE_CONSTANTS_HPP

#include <string>

namespace LynxLabelTypeConstants {

    // **Basic Label Types**
    inline constexpr const char* lynxEntryBB = "lynx.entry";
    inline constexpr const char* lynxEndBB = "lynx.end";

    // ** Control Flow Labels **
    inline constexpr const char* lynxIfElseMerge = "lynx.if.merge";
    inline constexpr const char* lynxElseIfCondition = "lynx.elseif.condition";
    inline constexpr const char* lynxIfThenBranch = "lynx.if.then";

    // **Loop-related Labels**
    inline constexpr const char* lynxLoopHeaderBB = "lynx.loop.header";
    inline constexpr const char* lynxLoopBodyBB = "lynx.loop.body";
    inline constexpr const char* lynxLoopExitBB = "lynx.loop.exit";
    inline constexpr const char* lynxLoopContinueBB = "lynx.loop.continue";
    inline constexpr const char* lynxLoopBreakBB = "lynx.loop.break";
    inline constexpr const char* lynxLoopHeaderEntryBB = "lynx.loop.header.entry";
    inline constexpr const char* lynxLoopConditionBB = "lynx.loop.condition";

    // **Switch-related Labels**
    inline constexpr const char* lynxSwitchCaseBB = "lynx.switch.case";
    inline constexpr const char* lynxSwitchExitBB = "lynx.switch.exit";
    inline constexpr const char* lynxSwitchDefaultBB = "lynx.switch.default";

    // **Ternary Operation Labels**
    inline constexpr const char* lynxTernaryTrueBB = "lynx.ternary.true";
    inline constexpr const char* lynxTernaryFalseBB = "lynx.ternary.false";
    inline constexpr const char* lynxTernaryMergeBB = "lynx.ternary.merge";
    inline constexpr const char* lynxTernaryResult = "lynx.ternary.result";

    // **Array-related Labels**
    inline constexpr const char* lynxArrayAccessBB = "lynx.array.access";
    inline constexpr const char* lynxArrayBoundsCheckBB = "lynx.array.bounds.check";

    // **Function Call Labels**
    inline constexpr const char* lynxUnreachableBB = "lynx.unreachable";
    inline constexpr const char* lynxDefaultBB = "lynx.default";
    inline constexpr const char* lynxReturnBB = "lynx.return";
    inline constexpr const char* lynxFunctionStartBB = "lynx.function.start";
    inline constexpr const char* lynxFunctionEndBB = "lynx.function.end";
    inline constexpr const char* lynxFunctionCallEntryBB = "lynx.function.call.entry";
    inline constexpr const char* lynxFunctionCallExitBB = "lynx.function.call.exit";

    // **Complex Labels (specific scenarios)**
    inline constexpr const char* lynxNestedLoopBB = "lynx.nested.loop";

    // **Error-handling Labels**
    inline constexpr const char* lynxErrorBB = "lynx.error";
    inline constexpr const char* lynxExceptionHandlerBB = "lynx.exception.handler";

    // **Optimization Labels**
    inline constexpr const char* lynxUnrollLoopBB = "lynx.loop.unroll";

    // **Struct-related Labels**
    inline constexpr const char* lynxStructTypeAccessBB = "lynx.struct.access";
    inline constexpr const char* lynxStructTypeCtorBB = "lynx.struct.constructor";
    inline constexpr const char* lynxStructTypeDtorBB = "lynx.struct.destructor";
    inline constexpr const char* lynxStructFieldAccessBB = "lynx.struct.field.access";

    // **Class-related Labels (OOP concepts)**
    inline constexpr const char* lynxClassMethodEntryBB = "lynx.class.method.entry";
    inline constexpr const char* lynxClassMethodExitBB = "lynx.class.method.exit";
    inline constexpr const char* lynxClassConstructorBB = "lynx.class.constructor";
    inline constexpr const char* lynxClassDestructorBB = "lynx.class.destructor";
    inline constexpr const char* lynxClassFieldAccessBB = "lynx.class.field.access";

    // **Functional Language Labels**
    inline constexpr const char* lynxClosureEntryBB = "lynx.closure.entry";
    inline constexpr const char* lynxClosureExitBB = "lynx.closure.exit";
    inline constexpr const char* lynxPatternMatchCaseBB = "lynx.pattern.match.case";
    inline constexpr const char* lynxPatternMatchDefaultBB = "lynx.pattern.match.default";
    inline constexpr const char* lynxTailCallEntryBB = "lynx.tailcall.entry";
    inline constexpr const char* lynxTailCallExitBB = "lynx.tailcall.exit";

    // **Concurrency and Parallelism Labels**
    inline constexpr const char* lynxThreadEntryBB = "lynx.thread.entry";
    inline constexpr const char* lynxThreadExitBB = "lynx.thread.exit";
    inline constexpr const char* lynxLockAcquisitionBB = "lynx.lock.acquisition";
    inline constexpr const char* lynxLockReleaseBB = "lynx.lock.release";
    inline constexpr const char* lynxAtomicLoadBB = "lynx.atomic.load";
    inline constexpr const char* lynxAtomicStoreBB = "lynx.atomic.store";
    inline constexpr const char* lynxParallelTaskStartBB = "lynx.parallel.task.start";
    inline constexpr const char* lynxParallelTaskEndBB = "lynx.parallel.task.end";

    // **Exception Handling Labels**
    inline constexpr const char* lynxTryBlockBB = "lynx.try.block";
    inline constexpr const char* lynxCatchBlockBB = "lynx.catch.block";
    inline constexpr const char* lynxFinallyBlockBB = "lynx.finally.block";
    inline constexpr const char* lynxThrowExceptionBB = "lynx.throw.exception";

    // **Low-Level/Assembly Constructs**
    inline constexpr const char* lynxSysCallEntryBB = "lynx.syscall.entry";
    inline constexpr const char* lynxSysCallExitBB = "lynx.syscall.exit";
    inline constexpr const char* lynxInterruptHandlerBB = "lynx.interrupt.handler";
    inline constexpr const char* lynxInterruptReturnBB = "lynx.interrupt.return";

    // **Memory Management Labels**
    inline constexpr const char* lynxMemoryAllocBB = "lynx.memory.alloc";
    inline constexpr const char* lynxMemoryFreeBB = "lynx.memory.free";
    inline constexpr const char* lynxGCStartBB = "lynx.gc.start";
    inline constexpr const char* lynxGCEndBB = "lynx.gc.end";
    inline constexpr const char* lynxRefCountIncrementBB = "lynx.refcount.increment";
    inline constexpr const char* lynxRefCountDecrementBB = "lynx.refcount.decrement";

    // **Debugging and Profiling Labels**
    inline constexpr const char* lynxVariableAccessBB = "lynx.variable.access";
    inline constexpr const char* lynxFunctionEntryProfileBB = "lynx.function.entry.profile";
    inline constexpr const char* lynxFunctionExitProfileBB = "lynx.function.exit.profile";
    inline constexpr const char* lynxMemAccessReadBB = "lynx.mem.access.read";
    inline constexpr const char* lynxMemAccessWriteBB = "lynx.mem.access.write";

    // **Optimization-related Labels**
    inline constexpr const char* lynxLoopUnrollEntryBB = "lynx.loop.unroll.entry";
    inline constexpr const char* lynxLoopUnrollExitBB = "lynx.loop.unroll.exit";
    inline constexpr const char* lynxFunctionInlineBB = "lynx.function.inline";
    inline constexpr const char* lynxConstantFoldingBB = "lynx.constant.folding";
    inline constexpr const char* lynxDeadCodeEliminationBB = "lynx.dead.code.elimination";
    inline constexpr const char* lynxVectorizationBB = "lynx.vectorization";

    // **Intermediate Representation Specific Labels**
    inline constexpr const char* lynxLLVMOptPassBB = "lynx.llvm.optimization.pass";
    inline constexpr const char* lynxLLVMInstCombinePassBB = "lynx.llvm.instcombine.pass";
    inline constexpr const char* lynxLLVMLoadInstBB = "lynx.llvm.load.inst";
    inline constexpr const char* lynxLLVMStoreInstBB = "lynx.llvm.store.inst";

    // **Miscellaneous**
    inline constexpr const char* lynxMMIOReadBB = "lynx.mmio.read";
    inline constexpr const char* lynxMMIOWriteBB = "lynx.mmio.write";
    inline constexpr const char* lynxFFIEntryBB = "lynx.ffi.entry";
    inline constexpr const char* lynxFFIExitBB = "lynx.ffi.exit";
    inline constexpr const char* lynxEventHandlerBB = "lynx.event.handler";

    // temporay variable
    inline constexpr const char* lynxTempVariable = "lynx.temp.var";
    inline constexpr const char* lynxTempReturnValue = "lynx.temp.return_value";

    inline constexpr const char* assignAdd = "lynx.assign.add";
    inline constexpr const char* assignMinus = "lynx.assign.sub";
    inline constexpr const char* assignMultiply = "lynx.assign.mul";
    inline constexpr const char* assignDivide = "lynx.assign.div";
    inline constexpr const char* assignModulo = "lynx.assign.mod";
    inline constexpr const char* assignLShift = "lynx.assign.lshift";
    inline constexpr const char* assignRShift = "lynx.assign.rshift";
    inline constexpr const char* assignAnd = "lynx.assign.and";
    inline constexpr const char* assignXor = "lynx.assign.xor";
}

#endif
