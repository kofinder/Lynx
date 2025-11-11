; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@lynx.string.constant = private constant [23 x i8] c"Max Integer limit is: \00"
@fmt = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@lynx.string.constant.3 = private constant [29 x i8] c"instance method call check: \00"
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@fmt.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %x = alloca i32, align 4, !lynx.type !1
  store i32 2147483647, i32* %x, align 4
  %i = alloca i32, align 4, !lynx.type !1
  store i32 10, i32* %i, align 4
  %i_load = load i32, i32* %i, align 4
  %load_lhs = load i32, i32* %i, align 4
  %int_add = add i32 %load_lhs, 5
  store i32 %int_add, i32* %i, align 4
  %x_load = load i32, i32* %x, align 4
  %io_printf_call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @lynx.string.constant, i32 0, i32 0), i32 %x_load)
  %i_load1 = load i32, i32* %i, align 4
  %io_printf_call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @lynx.string.constant.3, i32 0, i32 0), i32 %i_load1)
  ret i32 0
}

declare i32 @printf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.int.type"}
