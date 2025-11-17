; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@lynx.string.constant = private constant [23 x i8] c"Max Integer limit is: \00"
@fmt = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: alwaysinline
define dso_local i32 @main() #0 !lynx.signature !0 {
entry:
  %x = alloca i32, align 4, !lynx.type !1
  store i32 20, ptr %x, align 4
  %x_load = load i32, ptr %x, align 4
  %0 = icmp slt i32 %x_load, 0
  %1 = sub i32 0, %x_load
  %2 = select i1 %0, i32 %1, i32 %x_load
  store i32 %2, ptr %x, align 4
  %x_load1 = load i32, ptr %x, align 4
  %io_printf_call = call i32 (ptr, ...) @printf(ptr @fmt, ptr @lynx.string.constant, i32 %x_load1)
  ret i32 32
}

declare i32 @printf(ptr, ...)

attributes #0 = { alwaysinline }

!0 = !{!"main(int)"}
!1 = !{!"lynx.int.type"}
