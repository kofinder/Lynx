; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@import_utils_core_utility = private constant [19 x i8] c"utils/core_utility\00"
@formatString = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %factorial_head_recursion = call i32 @factorial_head_recursion(i32 6)
  %result = alloca i32, align 4, !lynx.type !1
  store i32 %factorial_head_recursion, i32* %result, align 4
  %result_load = load i32, i32* %result, align 4
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @formatString, i32 0, i32 0), i32 %result_load)
  ret i32 20
}

declare i32 @factorial_head_recursion(i32)

declare i32 @printf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.int.type"}
