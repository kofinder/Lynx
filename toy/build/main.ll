; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@lynx.string.constant = private constant [11 x i8] c"PopCount: \00"
@fmt = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@lynx.string.constant.3 = private constant [20 x i8] c"countLeadingZeros: \00"
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@lynx.string.constant.7 = private constant [21 x i8] c"countTrailingZeros: \00"
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.9 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@lynx.string.constant.11 = private constant [13 x i8] c"bitReverse: \00"
@fmt.12 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.14 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: alwaysinline
define dso_local i32 @main() #0 !lynx.signature !0 {
entry:
  %x = alloca i32, align 4, !lynx.type !1
  store i32 1030, ptr %x, align 4
  %x_load = load i32, ptr %x, align 4
  %0 = call i32 @llvm.smul.fix.i32(i32 %x_load, i32 %x_load, i32 8)
  %io_printf_call = call i32 (ptr, ...) @printf(ptr @fmt, ptr @lynx.string.constant, i32 %0)
  %x_load1 = load i32, ptr %x, align 4
  %1 = call i32 @llvm.umul.fix.i32(i32 %x_load1, i32 %x_load1, i32 8)
  %io_printf_call2 = call i32 (ptr, ...) @printf(ptr @fmt.4, ptr @lynx.string.constant.3, i32 %1)
  %x_load3 = load i32, ptr %x, align 4
  %arith.div = sdiv i32 %x_load3, 8
  store i32 %arith.div, ptr %x, align 4
  %io_printf_call4 = call i32 (ptr, ...) @printf(ptr @fmt.8, ptr @lynx.string.constant.7, i32 %arith.div)
  %x_load5 = load i32, ptr %x, align 4
  %2 = call i32 @llvm.udiv.fix.i32(i32 %x_load5, i32 %x_load5, i32 8)
  %io_printf_call6 = call i32 (ptr, ...) @printf(ptr @fmt.12, ptr @lynx.string.constant.11, i32 %2)
  ret i32 32
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smul.fix.i32(i32, i32, i32 immarg) #1

declare i32 @printf(ptr, ...)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.umul.fix.i32(i32, i32, i32 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.udiv.fix.i32(i32, i32, i32 immarg) #2

attributes #0 = { alwaysinline }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(none) }

!0 = !{!"main(int)"}
!1 = !{!"lynx.int.type"}
