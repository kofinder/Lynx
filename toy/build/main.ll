; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@lynx.string.constant = private constant [14 x i8] c"Result ====>:\00"
@fmt = private unnamed_addr constant [7 x i8] c"%s %f\0A\00", align 1

; Function Attrs: alwaysinline
define dso_local i32 @main() #0 !lynx.signature !0 {
entry:
  %num = alloca i1, align 1, !lynx.type !1
  store i1 false, ptr %num, align 1
  %num_load = load i1, ptr %num, align 1
  %bool_to_float = select i1 %num_load, float 1.000000e+00, float 0.000000e+00
  %x = alloca float, align 4, !lynx.type !2
  store float %bool_to_float, ptr %x, align 4
  %x_load = load float, ptr %x, align 4
  %promotedFloat = fpext float %x_load to double
  %io_printf_call = call i32 (ptr, ...) @printf(ptr @fmt, ptr @lynx.string.constant, double %promotedFloat)
  ret i32 64
}

declare i32 @printf(ptr, ...)

attributes #0 = { alwaysinline }

!0 = !{!"main(int)"}
!1 = !{!"lynx.boolean.type"}
!2 = !{!"lynx.float.type"}
