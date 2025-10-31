; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@import_utils_core_utility = private constant [19 x i8] c"utils/core_utility\00"
@formatString = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatString.1 = private unnamed_addr constant [5 x i8] c"%lf\0A\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %revenue = alloca double, align 8, !lynx.type !1
  store double 1.000000e+04, double* %revenue, align 8
  %cost = alloca double, align 8, !lynx.type !1
  store double 7.500000e+03, double* %cost, align 8
  %revenue_load = load double, double* %revenue, align 8
  %cost_load = load double, double* %cost, align 8
  %profit_calculation = call double @profit_calculation(double %revenue_load, double %cost_load)
  %profit = alloca double, align 8, !lynx.type !1
  store double %profit_calculation, double* %profit, align 8
  %factorial_head_recursion = call i32 @factorial_head_recursion(i32 6)
  %result = alloca i32, align 4, !lynx.type !2
  store i32 %factorial_head_recursion, i32* %result, align 4
  %result_load = load i32, i32* %result, align 4
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @formatString, i32 0, i32 0), i32 %result_load)
  %profit_load = load double, double* %profit, align 8
  %printfCall1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @formatString.1, i32 0, i32 0), double %profit_load)
  ret i32 20
}

declare i32 @printf(i8*, ...)

; Function Attrs: alwaysinline uwtable
define dso_local signext i32 @factorial_head_recursion(i32 %n) #1 !lynx.signature !3 {
entry:
  %.ret_value = alloca i32, align 4
  %lynx.lessthantmp = icmp slt i32 %n, 0
  %lynx.equalitytmp = icmp eq i32 %n, 1
  %lynx.ortmp = or i1 %lynx.lessthantmp, %lynx.equalitytmp
  br i1 %lynx.ortmp, label %lynx.if.then, label %lynx.elseif.condition

lynx.if.then:                                     ; preds = %entry
  store i32 1, i32* %.ret_value, align 4
  br label %exit

exit:                                             ; preds = %lynx.if.merge, %lynx.if.then
  %lynx.temp.return_value = load i32, i32* %.ret_value, align 4
  ret i32 %lynx.temp.return_value

lynx.elseif.condition:                            ; preds = %entry
  br label %lynx.if.merge

lynx.if.merge:                                    ; preds = %lynx.elseif.condition
  %lynx.subtmp = sub i32 %n, 1
  %factorial_head_recursion = call i32 @factorial_head_recursion(i32 %lynx.subtmp)
  %result = alloca i32, align 4, !lynx.type !2
  store i32 %factorial_head_recursion, i32* %result, align 4
  %result_load = load i32, i32* %result, align 4
  %lynx.multhmp = mul i32 %n, %result_load
  store i32 %lynx.multhmp, i32* %.ret_value, align 4
  br label %exit
}

; Function Attrs: alwaysinline uwtable
define dso_local signext i32 @factorial_tail_recursion(i32 %n, i32 %accumulator) #1 !lynx.signature !4 {
entry:
  %.ret_value = alloca i32, align 4
  %lynx.lessthantmp = icmp slt i32 %n, 0
  %lynx.equalitytmp = icmp eq i32 %n, 1
  %lynx.ortmp = or i1 %lynx.lessthantmp, %lynx.equalitytmp
  br i1 %lynx.ortmp, label %lynx.if.then, label %lynx.elseif.condition

lynx.if.then:                                     ; preds = %entry
  store i32 %accumulator, i32* %.ret_value, align 4
  br label %exit

exit:                                             ; preds = %lynx.if.merge, %lynx.if.then
  %lynx.temp.return_value = load i32, i32* %.ret_value, align 4
  ret i32 %lynx.temp.return_value

lynx.elseif.condition:                            ; preds = %entry
  br label %lynx.if.merge

lynx.if.merge:                                    ; preds = %lynx.elseif.condition
  %lynx.subtmp = sub i32 %n, 1
  %lynx.multhmp = mul i32 %accumulator, %n
  %factorial_tail_recursion = call i32 @factorial_tail_recursion(i32 %lynx.subtmp, i32 %lynx.multhmp)
  store i32 %factorial_tail_recursion, i32* %.ret_value, align 4
  br label %exit
}

; Function Attrs: alwaysinline norecurse nounwind uwtable
define dso_local double @profit_calculation(double %revenue, double %cost) #2 !lynx.signature !5 {
entry:
  %lynx.subtmp = fsub double %revenue, %cost
  ret double %lynx.subtmp
}

attributes #0 = { alwaysinline norecurse uwtable }
attributes #1 = { alwaysinline uwtable }
attributes #2 = { alwaysinline norecurse nounwind uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.double.type"}
!2 = !{!"lynx.int.type"}
!3 = !{!"factorial_head_recursion(int, int)"}
!4 = !{!"factorial_tail_recursion(int, int, int)"}
!5 = !{!"profit_calculation(double, double, double)"}
