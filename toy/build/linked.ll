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

declare i32 @printf(i8*, ...)

; Function Attrs: alwaysinline uwtable
define dso_local signext i32 @factorial_head_recursion(i32 %n) #1 !lynx.signature !2 {
entry:
  %.ret_value = alloca i32, align 4
  %cmp.lt = icmp slt i32 %n, 0
  %cmp.eq = icmp eq i32 %n, 1
  %bool.or = or i1 %cmp.lt, %cmp.eq
  br i1 %bool.or, label %lynx.if.then, label %lynx.elseif.condition

lynx.if.then:                                     ; preds = %entry
  store i32 1, i32* %.ret_value, align 4
  br label %exit

exit:                                             ; preds = %lynx.if.merge, %lynx.if.then
  %lynx.temp.return_value = load i32, i32* %.ret_value, align 4
  ret i32 %lynx.temp.return_value

lynx.elseif.condition:                            ; preds = %entry
  br label %lynx.if.merge

lynx.if.merge:                                    ; preds = %lynx.elseif.condition
  %arith.sub = sub i32 %n, 1
  %factorial_head_recursion = call i32 @factorial_head_recursion(i32 %arith.sub)
  %result = alloca i32, align 4, !lynx.type !3
  store i32 %factorial_head_recursion, i32* %result, align 4
  %result_load = load i32, i32* %result, align 4
  %arith.mul = mul i32 %n, %result_load
  store i32 %arith.mul, i32* %.ret_value, align 4
  br label %exit
}

; Function Attrs: alwaysinline uwtable
define dso_local signext i32 @factorial_tail_recursion(i32 %n, i32 %accumulator) #1 !lynx.signature !4 {
entry:
  %.ret_value = alloca i32, align 4
  %cmp.lt = icmp slt i32 %n, 0
  %cmp.eq = icmp eq i32 %n, 1
  %bool.or = or i1 %cmp.lt, %cmp.eq
  br i1 %bool.or, label %lynx.if.then, label %lynx.elseif.condition

lynx.if.then:                                     ; preds = %entry
  store i32 %accumulator, i32* %.ret_value, align 4
  br label %exit

exit:                                             ; preds = %lynx.if.merge, %lynx.if.then
  %lynx.temp.return_value = load i32, i32* %.ret_value, align 4
  ret i32 %lynx.temp.return_value

lynx.elseif.condition:                            ; preds = %entry
  br label %lynx.if.merge

lynx.if.merge:                                    ; preds = %lynx.elseif.condition
  %arith.sub = sub i32 %n, 1
  %arith.mul = mul i32 %accumulator, %n
  %factorial_tail_recursion = call i32 @factorial_tail_recursion(i32 %arith.sub, i32 %arith.mul)
  store i32 %factorial_tail_recursion, i32* %.ret_value, align 4
  br label %exit
}

; Function Attrs: alwaysinline norecurse nounwind uwtable
define dso_local double @profit_calculation(double %revenue, double %cost) #2 !lynx.signature !5 {
entry:
  %arith.sub = fsub double %revenue, %cost
  ret double %arith.sub
}

; Function Attrs: alwaysinline norecurse nounwind uwtable
define dso_local double @profit_margin(double %revenue, double %cost) #2 !lynx.signature !6 {
entry:
  %.ret_value = alloca double, align 8
  %cmp.eq = fcmp oeq double %revenue, 0.000000e+00
  br i1 %cmp.eq, label %lynx.if.then, label %lynx.elseif.condition

lynx.if.then:                                     ; preds = %entry
  store double 0.000000e+00, double* %.ret_value, align 8
  br label %exit

exit:                                             ; preds = %lynx.if.merge, %lynx.if.then
  %lynx.temp.return_value = load double, double* %.ret_value, align 8
  ret double %lynx.temp.return_value

lynx.elseif.condition:                            ; preds = %entry
  br label %lynx.if.merge

lynx.if.merge:                                    ; preds = %lynx.elseif.condition
  %profit_calculation = call double @profit_calculation(double %revenue, double %cost)
  %profit = alloca double, align 8, !lynx.type !1
  store double %profit_calculation, double* %profit, align 8
  %profit_load = load double, double* %profit, align 8
  %arith.div = fdiv double %profit_load, %revenue
  %arith.mul = fmul double %arith.div, 1.000000e+02
  %margin = alloca double, align 8, !lynx.type !1
  store double %arith.mul, double* %margin, align 8
  %margin_load = load double, double* %margin, align 8
  store double %margin_load, double* %.ret_value, align 8
  br label %exit
}

attributes #0 = { alwaysinline norecurse uwtable }
attributes #1 = { alwaysinline uwtable }
attributes #2 = { alwaysinline norecurse nounwind uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.double.type"}
!2 = !{!"factorial_head_recursion(int, int)"}
!3 = !{!"lynx.int.type"}
!4 = !{!"factorial_tail_recursion(int, int, int)"}
!5 = !{!"profit_calculation(double, double, double)"}
!6 = !{!"profit_margin(double, double, double)"}
