; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@lynx.string.constant = private constant [7 x i8] c"sqrt: \00"
@fmt = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.3 = private constant [6 x i8] c"pow: \00"
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.6 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.7 = private constant [6 x i8] c"exp: \00"
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.9 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.10 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.11 = private constant [7 x i8] c"exp2: \00"
@fmt.12 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.14 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.15 = private constant [8 x i8] c"exp10: \00"
@fmt.16 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.17 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.18 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.19 = private constant [6 x i8] c"log: \00"
@fmt.20 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.21 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.22 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.23 = private constant [7 x i8] c"log2: \00"
@fmt.24 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.25 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.26 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.27 = private constant [8 x i8] c"log10: \00"
@fmt.28 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.29 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.30 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.31 = private constant [6 x i8] c"sin: \00"
@fmt.32 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.33 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.34 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.35 = private constant [6 x i8] c"cos: \00"
@fmt.36 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.37 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.38 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.39 = private constant [6 x i8] c"tan: \00"
@fmt.40 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.41 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.42 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.43 = private constant [7 x i8] c"acos: \00"
@fmt.44 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.45 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.46 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.47 = private constant [7 x i8] c"atan: \00"
@fmt.48 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.49 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.50 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.51 = private constant [8 x i8] c"atan2: \00"
@fmt.52 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.53 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.54 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.55 = private constant [7 x i8] c"sinh: \00"
@fmt.56 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.57 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.58 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.59 = private constant [7 x i8] c"cosh: \00"
@fmt.60 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.61 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.62 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.63 = private constant [7 x i8] c"tanh: \00"
@fmt.64 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.65 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.66 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.67 = private constant [8 x i8] c"floor: \00"
@fmt.68 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.69 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.70 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.71 = private constant [7 x i8] c"ceil: \00"
@fmt.72 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.73 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.74 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@lynx.string.constant.75 = private constant [8 x i8] c"trunc: \00"
@fmt.76 = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1
@fmt.77 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.78 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

; Function Attrs: alwaysinline
define dso_local i32 @main() #0 !lynx.signature !0 {
entry:
  %x = alloca i32, align 4, !lynx.type !1
  store i32 300, ptr %x, align 4
  %x_load = load i32, ptr %x, align 4
  %0 = sitofp i32 %x_load to float
  %1 = call float @llvm.sqrt.f32(float %0)
  %promotedFloat = fpext float %1 to double
  %io_printf_call = call i32 (ptr, ...) @printf(ptr @fmt, ptr @lynx.string.constant, double %promotedFloat)
  %x_load1 = load i32, ptr %x, align 4
  %2 = sitofp i32 %x_load1 to float
  %3 = call float @llvm.pow.f32(float %2, float 5.000000e+00)
  %promotedFloat2 = fpext float %3 to double
  %io_printf_call3 = call i32 (ptr, ...) @printf(ptr @fmt.4, ptr @lynx.string.constant.3, double %promotedFloat2)
  %x_load4 = load i32, ptr %x, align 4
  %4 = sitofp i32 %x_load4 to float
  %5 = call float @llvm.exp.f32(float %4)
  %promotedFloat5 = fpext float %5 to double
  %io_printf_call6 = call i32 (ptr, ...) @printf(ptr @fmt.8, ptr @lynx.string.constant.7, double %promotedFloat5)
  %x_load7 = load i32, ptr %x, align 4
  %6 = sitofp i32 %x_load7 to float
  %7 = call float @llvm.exp2.f32(float %6)
  %promotedFloat8 = fpext float %7 to double
  %io_printf_call9 = call i32 (ptr, ...) @printf(ptr @fmt.12, ptr @lynx.string.constant.11, double %promotedFloat8)
  %x_load10 = load i32, ptr %x, align 4
  %8 = sitofp i32 %x_load10 to float
  %9 = call float @llvm.exp10.f32(float %8)
  %promotedFloat11 = fpext float %9 to double
  %io_printf_call12 = call i32 (ptr, ...) @printf(ptr @fmt.16, ptr @lynx.string.constant.15, double %promotedFloat11)
  %x_load13 = load i32, ptr %x, align 4
  %10 = sitofp i32 %x_load13 to float
  %11 = call float @llvm.log.f32(float %10)
  %promotedFloat14 = fpext float %11 to double
  %io_printf_call15 = call i32 (ptr, ...) @printf(ptr @fmt.20, ptr @lynx.string.constant.19, double %promotedFloat14)
  %x_load16 = load i32, ptr %x, align 4
  %12 = sitofp i32 %x_load16 to float
  %13 = call float @llvm.log2.f32(float %12)
  %promotedFloat17 = fpext float %13 to double
  %io_printf_call18 = call i32 (ptr, ...) @printf(ptr @fmt.24, ptr @lynx.string.constant.23, double %promotedFloat17)
  %x_load19 = load i32, ptr %x, align 4
  %14 = sitofp i32 %x_load19 to float
  %15 = call float @llvm.log10.f32(float %14)
  %promotedFloat20 = fpext float %15 to double
  %io_printf_call21 = call i32 (ptr, ...) @printf(ptr @fmt.28, ptr @lynx.string.constant.27, double %promotedFloat20)
  %x_load22 = load i32, ptr %x, align 4
  %16 = sitofp i32 %x_load22 to float
  %17 = call float @llvm.sin.f32(float %16)
  %promotedFloat23 = fpext float %17 to double
  %io_printf_call24 = call i32 (ptr, ...) @printf(ptr @fmt.32, ptr @lynx.string.constant.31, double %promotedFloat23)
  %x_load25 = load i32, ptr %x, align 4
  %18 = sitofp i32 %x_load25 to float
  %19 = call float @llvm.cos.f32(float %18)
  %promotedFloat26 = fpext float %19 to double
  %io_printf_call27 = call i32 (ptr, ...) @printf(ptr @fmt.36, ptr @lynx.string.constant.35, double %promotedFloat26)
  %x_load28 = load i32, ptr %x, align 4
  %20 = sitofp i32 %x_load28 to float
  %21 = call float @llvm.tan.f32(float %20)
  %promotedFloat29 = fpext float %21 to double
  %io_printf_call30 = call i32 (ptr, ...) @printf(ptr @fmt.40, ptr @lynx.string.constant.39, double %promotedFloat29)
  %x_load31 = load i32, ptr %x, align 4
  %22 = sitofp i32 %x_load31 to float
  %23 = call float @llvm.acos.f32(float %22)
  %promotedFloat32 = fpext float %23 to double
  %io_printf_call33 = call i32 (ptr, ...) @printf(ptr @fmt.44, ptr @lynx.string.constant.43, double %promotedFloat32)
  %x_load34 = load i32, ptr %x, align 4
  %24 = sitofp i32 %x_load34 to float
  %25 = call float @llvm.atan.f32(float %24)
  %promotedFloat35 = fpext float %25 to double
  %io_printf_call36 = call i32 (ptr, ...) @printf(ptr @fmt.48, ptr @lynx.string.constant.47, double %promotedFloat35)
  %x_load37 = load i32, ptr %x, align 4
  %26 = sitofp i32 %x_load37 to float
  %27 = call float @llvm.atan2.f32(float %26, float 5.000000e+00)
  %promotedFloat38 = fpext float %27 to double
  %io_printf_call39 = call i32 (ptr, ...) @printf(ptr @fmt.52, ptr @lynx.string.constant.51, double %promotedFloat38)
  %x_load40 = load i32, ptr %x, align 4
  %28 = sitofp i32 %x_load40 to float
  %29 = call float @llvm.sinh.f32(float %28)
  %promotedFloat41 = fpext float %29 to double
  %io_printf_call42 = call i32 (ptr, ...) @printf(ptr @fmt.56, ptr @lynx.string.constant.55, double %promotedFloat41)
  %x_load43 = load i32, ptr %x, align 4
  %30 = sitofp i32 %x_load43 to float
  %31 = call float @llvm.cosh.f32(float %30)
  %promotedFloat44 = fpext float %31 to double
  %io_printf_call45 = call i32 (ptr, ...) @printf(ptr @fmt.60, ptr @lynx.string.constant.59, double %promotedFloat44)
  %x_load46 = load i32, ptr %x, align 4
  %32 = sitofp i32 %x_load46 to float
  %33 = call float @llvm.tanh.f32(float %32)
  %promotedFloat47 = fpext float %33 to double
  %io_printf_call48 = call i32 (ptr, ...) @printf(ptr @fmt.64, ptr @lynx.string.constant.63, double %promotedFloat47)
  %x_load49 = load i32, ptr %x, align 4
  %34 = sitofp i32 %x_load49 to float
  %35 = call float @llvm.floor.f32(float %34)
  %promotedFloat50 = fpext float %35 to double
  %io_printf_call51 = call i32 (ptr, ...) @printf(ptr @fmt.68, ptr @lynx.string.constant.67, double %promotedFloat50)
  %x_load52 = load i32, ptr %x, align 4
  %36 = sitofp i32 %x_load52 to float
  %37 = call float @llvm.ceil.f32(float %36)
  %promotedFloat53 = fpext float %37 to double
  %io_printf_call54 = call i32 (ptr, ...) @printf(ptr @fmt.72, ptr @lynx.string.constant.71, double %promotedFloat53)
  %x_load55 = load i32, ptr %x, align 4
  %38 = sitofp i32 %x_load55 to float
  %39 = call float @llvm.trunc.f32(float %38)
  %promotedFloat56 = fpext float %39 to double
  %io_printf_call57 = call i32 (ptr, ...) @printf(ptr @fmt.76, ptr @lynx.string.constant.75, double %promotedFloat56)
  ret i32 32
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.sqrt.f32(float) #1

declare i32 @printf(ptr, ...)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.pow.f32(float, float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.exp.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.exp2.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.exp10.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.log.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.log2.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.log10.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.sin.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.cos.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.tan.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.acos.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.atan.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.atan2.f32(float, float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.sinh.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.cosh.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.tanh.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.floor.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.ceil.f32(float) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.trunc.f32(float) #1

attributes #0 = { alwaysinline }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!0 = !{!"main(int)"}
!1 = !{!"lynx.int.type"}
