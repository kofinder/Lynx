; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@lynx.string.constant = private constant [12 x i8] c"Hello World\00"
@scanf_fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@formatString = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %printfCall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @formatString, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt, i32 0, i32 0))
  %scanf_tmp = alloca i32, align 4
  %scanfCall = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt.1, i32 0, i32 0), i32* %scanf_tmp)
  %read_value = load i32, i32* %scanf_tmp, align 4
  ret i32 32
}

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
