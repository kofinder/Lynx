; ModuleID = 'core_utility'
source_filename = "core_utility"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: alwaysinline uwtable
define dso_local signext i32 @factorial_head_recursion(i32 %n) #0 !lynx.signature !0 {
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
  %result = alloca i32, align 4, !lynx.type !1
  store i32 %factorial_head_recursion, i32* %result, align 4
  %result_load = load i32, i32* %result, align 4
  %lynx.multhmp = mul i32 %n, %result_load
  store i32 %lynx.multhmp, i32* %.ret_value, align 4
  br label %exit
}

; Function Attrs: alwaysinline uwtable
define dso_local signext i32 @factorial_tail_recursion(i32 %n, i32 %accumulator) #0 !lynx.signature !2 {
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
define dso_local double @profit_calculation(double %revenue, double %cost) #1 !lynx.signature !3 {
entry:
  %lynx.subtmp = fsub double %revenue, %cost
  ret double %lynx.subtmp
}

attributes #0 = { alwaysinline uwtable }
attributes #1 = { alwaysinline norecurse nounwind uwtable }

!0 = !{!"factorial_head_recursion(int, int)"}
!1 = !{!"lynx.int.type"}
!2 = !{!"factorial_tail_recursion(int, int, int)"}
!3 = !{!"profit_calculation(double, double, double)"}
