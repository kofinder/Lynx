; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@import_utils_core_utility = private constant [19 x i8] c"utils/core_utility\00"
@formatString = private unnamed_addr constant [5 x i8] c"%lf\0A\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %profit_margin = call double @profit_margin(double 1.000000e+04, double 7.500000e+03)
  %margin = alloca double, align 8, !lynx.type !1
  store double %profit_margin, double* %margin, align 8
  %margin_load = load double, double* %margin, align 8
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @formatString, i32 0, i32 0), double %margin_load)
  ret i32 32
}

declare double @profit_margin(double, double)

declare i32 @printf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.double.type"}
